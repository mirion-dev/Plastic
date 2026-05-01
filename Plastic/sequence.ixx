module;

#include <cassert>

export module plastic.sequence;

import std;

namespace plastic {

    template <class T>
    class Storage {
    public:
        using value_type = T;
        using size_type = std::size_t;
        using pointer = value_type*;
        using const_pointer = const value_type*;

    private:
#ifdef _MSC_VER
        [[msvc::no_unique_address]]
#else
        [[no_unique_address]] 
#endif
        std::allocator<value_type> _alloc;
        pointer _ptr{};
        size_type _size{};

    public:
        Storage() = default;

        explicit Storage(size_type size) {
            if (size != 0) {
                _ptr = _alloc.allocate(size);
                _size = size;
            }
        }

        Storage(Storage&& other) noexcept {
            this->swap(other);
        }

        ~Storage() {
            if (_ptr != nullptr) {
                _alloc.deallocate(_ptr, _size);
            }
        }

        Storage& operator=(Storage&& other) noexcept {
            this->swap(other);
            return *this;
        }

        pointer begin() {
            return _ptr;
        }

        const_pointer begin() const {
            return _ptr;
        }

        pointer end() {
            return _ptr + _size;
        }

        const_pointer end() const {
            return _ptr + _size;
        }

        size_type size() const {
            return _size;
        }

        void swap(Storage& other) noexcept {
            std::ranges::swap(_ptr, other._ptr);
            std::ranges::swap(_size, other._size);
        }

        friend void swap(Storage& left, Storage& right) noexcept {
            left.swap(right);
        }
    };

    export template <class T>
    class Vector {
    public:
        using value_type = T;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using reference = value_type&;
        using const_reference = const value_type&;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using iterator = pointer;
        using const_iterator = const_pointer;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    private:
        Storage<value_type> _data;
        size_type _size{ capacity() };

        void _grow(size_type new_capacity) {
            size_type size{ this->size() };
            Storage<value_type> new_data{ std::ranges::max(new_capacity, capacity() + (capacity() >> 1)) };
            std::ranges::uninitialized_move(*this, new_data);
            clear();

            _data = std::move(new_data);
            _size = size;
        }

        template <class... Args>
            requires (sizeof...(Args) <= 1)
        void _resize(size_type new_size, const Args&... args) {
            if (new_size > capacity()) {
                _grow(new_size);
            }

            iterator new_end{ begin() + new_size };
            if (new_size <= size()) {
                std::ranges::destroy(new_end, end());
            }
            else if constexpr (sizeof...(Args) == 0) {
                std::ranges::uninitialized_value_construct(end(), new_end);
            }
            else {
                std::ranges::uninitialized_fill(end(), new_end, args...);
            }
            _size = new_size;
        }

    public:
        Vector() = default;

        explicit Vector(size_type size) :
            _data{ size } {

            std::ranges::uninitialized_value_construct(*this);
        }

        Vector(size_type size, const_reference value) :
            _data{ size } {

            std::ranges::uninitialized_fill(*this, value);
        }

        template <std::input_iterator It>
        Vector(It first, It last) {
            std::ranges::copy(first, last, std::back_inserter(*this));
        }

        Vector(const Vector& other) :
            _data{ other.size() } {

            std::ranges::uninitialized_copy(other, *this);
        }

        Vector(Vector&& other) noexcept {
            this->swap(other);
        }

        Vector(std::initializer_list<value_type> list) :
            _data{ list.size() } {

            std::ranges::uninitialized_copy(list, *this);
        }

        ~Vector() {
            clear();
        }

        Vector& operator=(const Vector& other) {
            Vector temp(other);
            this->swap(temp);
            return *this;
        }

        Vector& operator=(Vector&& other) noexcept {
            this->swap(other);
            return *this;
        }

        Vector& operator=(std::initializer_list<value_type> list) {
            Vector temp(list);
            this->swap(temp);
            return *this;
        }

        iterator begin() {
            return _data.begin();
        }

        const_iterator begin() const {
            return _data.begin();
        }

        iterator end() {
            return _data.begin() + _size;
        }

        const_iterator end() const {
            return _data.begin() + _size;
        }

        reverse_iterator rbegin() {
            return reverse_iterator{ end() };
        }

        const_reverse_iterator rbegin() const {
            return const_reverse_iterator{ end() };
        }

        reverse_iterator rend() {
            return reverse_iterator{ begin() };
        }

        const_reverse_iterator rend() const {
            return const_reverse_iterator{ begin() };
        }

        const_iterator cbegin() const {
            return begin();
        }

        const_iterator cend() const {
            return end();
        }

        const_reverse_iterator crbegin() const {
            return rbegin();
        }

        const_reverse_iterator crend() const {
            return rend();
        }

        bool empty() const {
            return _size == 0;
        }

        size_type size() const {
            return _size;
        }

        size_type max_size() const {
            return static_cast<size_type>(-1) / sizeof(value_type);
        }

        size_type capacity() const {
            return _data.size();
        }

        void resize(size_type new_size) {
            _resize(new_size);
        }

        void resize(size_type new_size, const_reference value) {
            this->_resize(new_size, value_type{ value });
        }

        void reserve(size_type new_capacity) {
            if (new_capacity > capacity()) {
                _grow(new_capacity);
            }
        }

        reference operator[](size_type index) {
            assert(index < size());
            return begin()[index];
        }

        const_reference operator[](size_type index) const {
            assert(index < size());
            return begin()[index];
        }

        reference front() {
            assert(!empty());
            return *begin();
        }

        const_reference front() const {
            assert(!empty());
            return *begin();
        }

        reference back() {
            assert(!empty());
            return end()[-1];
        }

        const_reference back() const {
            assert(!empty());
            return end()[-1];
        }

        pointer data() {
            return begin();
        }

        const_pointer data() const {
            return begin();
        }

        void push_back(const_reference value) {
            value_type clone{ value };
            if (size() == capacity()) {
                _grow(size() + 1);
            }

            std::ranges::construct_at(end(), std::move(clone));
            ++_size;
        }

        void pop_back() {
            assert(!empty());
            --_size;
            std::ranges::destroy_at(end());
        }

        iterator insert(const_iterator pos, const_reference value) {
            value_type clone{ value };
            difference_type offset{ pos - begin() };
            if (size() == capacity()) {
                _grow(size() + 1);
        }

            iterator pos_iter{ begin() + offset };
            if (pos_iter == end()) {
                std::ranges::construct_at(end(), std::move(clone));
            }
            else {
                std::ranges::construct_at(end(), std::move(back()));
                std::ranges::move_backward(pos_iter, end() - 1, end());
                *pos_iter = std::move(clone);
            }
            ++_size;
            return pos_iter;
        }

        iterator insert(const_iterator pos, size_type count, const_reference value) {
            if (count == 0) {
                return const_cast<iterator>(pos);
            }

            value_type clone{ value };
            difference_type offset{ pos - begin() };
            if (capacity() - size() < count) {
                _grow(size() + count);
            }

            iterator pos_iter{ begin() + offset }, new_pos{ pos_iter + count };
            if (new_pos <= end()) {
                iterator middle{ end() - count };
                std::ranges::uninitialized_move(middle, end(), end(), std::unreachable_sentinel);
                std::ranges::move_backward(pos_iter, middle, end());
                std::ranges::fill(pos_iter, new_pos, clone);
                }
            else {
                std::ranges::uninitialized_move(pos_iter, end(), new_pos, std::unreachable_sentinel);
                std::ranges::fill(pos_iter, end(), clone);
                std::ranges::uninitialized_fill(end(), new_pos, clone);
            }
            _size += count;
            return pos_iter;
        }

        template <std::input_iterator It>
        iterator insert(const_iterator pos, It first, It last) {
            difference_type pos_offset{ pos - begin() }, pos_end_offset{ static_cast<difference_type>(size()) };
            std::ranges::copy(first, last, std::back_inserter(*this));

            iterator pos_iter{ begin() + pos_offset }, pos_end{ begin() + pos_end_offset };
            std::ranges::rotate(pos_iter, pos_end, end());
            return pos_iter;
        }

        iterator insert(const_iterator pos, std::initializer_list<value_type> list) {
            if (list.empty()) {
                return const_cast<iterator>(pos);
        }

            difference_type offset{ pos - begin() };
            if (capacity() - size() < list.size()) {
                _grow(size() + list.size());
            }

            iterator pos_iter{ begin() + offset }, new_pos{ pos_iter + list.size() };
            if (new_pos <= end()) {
                iterator middle{ end() - list.size() };
                std::ranges::uninitialized_move(middle, end(), end(), std::unreachable_sentinel);
                std::ranges::move_backward(pos_iter, middle, end());
                std::ranges::move(list, pos_iter);
            }
            else {
                auto middle{ list.begin() + (end() - pos_iter) };
                std::ranges::uninitialized_move(pos_iter, end(), new_pos, std::unreachable_sentinel);
                std::ranges::copy(list.begin(), middle, pos_iter);
                std::ranges::uninitialized_copy(middle, list.end(), end(), std::unreachable_sentinel);
            }
            _size += list.size();
            return pos_iter;
        }

        iterator erase(const_iterator pos) {
            assert(pos != end());
            auto pos_iter{ const_cast<iterator>(pos) };
            std::ranges::move(pos_iter + 1, end(), pos_iter);
            --_size;
            std::ranges::destroy_at(end());
            return pos_iter;
        }

        iterator erase(const_iterator first, const_iterator last) {
            auto first_iter{ const_cast<iterator>(first) }, last_iter{ const_cast<iterator>(last) };
            if (first_iter == last_iter) {
                return first_iter;
            }

                iterator new_end{ std::ranges::move(last_iter, end(), first_iter).out };
                std::ranges::destroy(new_end, end());
                _size = new_end - begin();
            return first_iter;
        }

        void swap(Vector& other) noexcept {
            std::ranges::swap(_data, other._data);
            std::ranges::swap(_size, other._size);
        }

        friend void swap(Vector& left, Vector& right) noexcept {
            left.swap(right);
        }

        void clear() {
            std::ranges::destroy(*this);
            _size = 0;
        }

        friend bool operator==(const Vector& left, const Vector& right) {
            return std::ranges::equal(left, right);
        }

        friend auto operator<=>(const Vector& left, const Vector& right) {
            return std::lexicographical_compare_three_way(left.begin(), left.end(), right.begin(), right.end());
        }
    };

    template <class It>
    Vector(It, It) -> Vector<std::iter_value_t<It>>;

    export template <class T>
    class Deque {
    public:
        using value_type = T;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using reference = value_type&;
        using const_reference = const value_type&;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using iterator = pointer;
        using const_iterator = const_pointer;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    private:
        Storage<value_type> _data;
        pointer _first_ptr{ _begin() };
        pointer _last_ptr{ _begin() };

        auto _begin(this auto& self) {
            return self._data.begin();
        }

        auto _end(this auto& self) {
            return self._data.end();
        }

        pointer _first() {
            return _first_ptr;
        }

        const_pointer _first() const {
            return _first_ptr;
        }

        pointer _last() {
            return _last_ptr;
        }

        const_pointer _last() const {
            return _last_ptr;
        }

        pointer _reallocate(size_type new_capacity, size_type right_reserved = {}, size_type left_reserved = {}) {
            size_type new_size{ size() + left_reserved + right_reserved };

            Storage<value_type> new_data{ std::ranges::max(new_capacity, capacity() << 1) };
            pointer new_first{ new_data.begin() + (new_capacity - new_size >> 1) + left_reserved };
            pointer new_last{ std::ranges::uninitialized_move(_first(), _last(), new_first, new_data.end()).out };
            clear();
            _data = std::move(new_data);
            _first_ptr = new_first;
            _last_ptr = new_last;

            return _first() + new_size;
        }

        void _resize(size_type new_size, const auto&... args) {
            if (new_size > capacity()) {
                pointer new_last{ _reallocate(new_size, new_size - size()) };
                if constexpr (sizeof...(args) == 0) {
                    std::ranges::uninitialized_value_construct(_last(), new_last);
                }
                else {
                    std::ranges::uninitialized_fill(_last(), new_last, args...);
                }
                _last_ptr = new_last;
                return;
            }

            pointer new_first{ _begin() + (capacity() - new_size >> 1) }, new_last{ new_first + new_size };
            if (new_size <= size()) {
                pointer middle{ _first() + new_size };
                if (_last() <= new_first || _first() >= new_last) {
                    std::ranges::uninitialized_move(_first(), middle, new_first, _end());
                    std::ranges::destroy(_first(), _last());
                }
                else if (_last() <= new_last) {
                    pointer j{ middle - (new_last - _last()) };
                    std::ranges::uninitialized_move(j, middle, _last(), _end());
                    std::ranges::move_backward(_first(), j, _last());
                    std::ranges::destroy(_first(), new_first);
                }
                else if (_first() >= new_first) {
                    pointer j{ _first() + (_first() - new_first) };
                    std::ranges::uninitialized_move(_first(), j, new_first, _end());
                    std::ranges::move(j, middle, _first());
                    std::ranges::destroy(new_last, _last());
                }
                else {
                    std::ranges::move_backward(_first(), middle, new_last);
                    std::ranges::destroy(_first(), new_first);
                    std::ranges::destroy(new_last, _last());
                }
            }
            else {
                pointer new_middle{ new_first + size() };
                if (_last() <= new_first || _first() >= new_middle) {
                    std::ranges::uninitialized_move(_first(), _last(), new_first, _end());
                    std::ranges::destroy(_first(), _last());
                }
                else if (_first() <= new_first) {
                    pointer j{ _last() - (new_middle - _last()) };
                    std::ranges::uninitialized_move(j, _last(), _last(), _end());
                    std::ranges::move_backward(_first(), j, _last());
                    std::ranges::destroy(_first(), new_first);
                }
                else {
                    pointer j{ _first() + (_first() - new_first) };
                    std::ranges::uninitialized_move(_first(), j, new_first, _end());
                    std::ranges::move(j, _last(), _first());
                    std::ranges::destroy(new_middle, _last());
                }
                if constexpr (sizeof...(args) == 0) {
                    std::ranges::uninitialized_value_construct(new_middle, new_last);
                }
                else {
                    std::ranges::uninitialized_fill(new_middle, new_last, args...);
                }
            }
            _first_ptr = new_first;
            _last_ptr = new_last;
        }

    public:
        Deque() = default;

        explicit Deque(size_type size) :
            _data{ size },
            _first_ptr{ _begin() },
            _last_ptr{ _end() } {

            std::ranges::uninitialized_value_construct(_first(), _last());
        }

        Deque(size_type size, const_reference value) :
            _data{ size },
            _first_ptr{ _begin() },
            _last_ptr{ _end() } {

            std::ranges::uninitialized_fill(_first(), _last(), value);
        }

        template <std::input_iterator It>
        Deque(It first, It last) {
            std::ranges::copy(first, last, std::back_inserter(*this));
        }

        Deque(std::initializer_list<value_type> list) :
            Deque(list.begin(), list.end()) {}

        Deque(const Deque& other) :
            Deque(other._first(), other._last()) {}

        Deque(Deque&& other) noexcept {
            this->swap(other);
        }

        ~Deque() {
            clear();
        }

        Deque& operator=(const Deque& other) {
            Deque temp(other);
            this->swap(temp);
            return *this;
        }

        Deque& operator=(Deque&& other) noexcept {
            this->swap(other);
            return *this;
        }

        void swap(Deque& other) noexcept {
            std::ranges::swap(_data, other._data);
            std::ranges::swap(_first_ptr, other._first_ptr);
            std::ranges::swap(_last_ptr, other._last_ptr);
        }

        friend void swap(Deque& left, Deque& right) noexcept {
            left.swap(right);
        }

        bool empty() const {
            return _first() == _last();
        }

        size_type size() const {
            return _last() - _first();
        }

        void clear() {
            std::ranges::destroy(_first(), _last());
            _first_ptr = _last_ptr = _begin() + (capacity() >> 1);
        }

        void resize(size_type new_size) {
            _resize(new_size);
        }

        void resize(size_type new_size, const_reference value) {
            value_type clone{ value };
            this->_resize(new_size, clone);
        }

        size_type capacity() const {
            return _end() - _begin();
        }

        void reserve(size_type new_capacity) {
            if (new_capacity > capacity()) {
                _reallocate(new_capacity);
            }
        }

        iterator begin() {
            return _first();
        }

        const_iterator begin() const {
            return _first();
        }

        iterator end() {
            return _last();
        }

        const_iterator end() const {
            return _last();
        }

        const_iterator cbegin() const {
            return begin();
        }

        const_iterator cend() const {
            return end();
        }

        reverse_iterator rbegin() {
            return reverse_iterator{ end() };
        }

        const_reverse_iterator rbegin() const {
            return const_reverse_iterator{ end() };
        }

        reverse_iterator rend() {
            return reverse_iterator{ begin() };
        }

        const_reverse_iterator rend() const {
            return const_reverse_iterator{ begin() };
        }

        const_reverse_iterator crbegin() const {
            return rbegin();
        }

        const_reverse_iterator crend() const {
            return rend();
        }

        reference operator[](size_type index) {
            assert(index < size());
            return _first()[index];
        }

        const_reference operator[](size_type index) const {
            assert(index < size());
            return _first()[index];
        }

        reference front() {
            assert(!empty());
            return *_first();
        }

        const_reference front() const {
            assert(!empty());
            return *_first();
        }

        reference back() {
            assert(!empty());
            return _last()[-1];
        }

        const_reference back() const {
            assert(!empty());
            return _last()[-1];
        }

        pointer data() {
            return _first();
        }

        const_pointer data() const {
            return _first();
        }

        void push_front(const_reference value) {
            value_type clone{ value };
            if (_first() == _begin()) {
                _reallocate(size() + 1, 0, 1);
            }
            std::ranges::construct_at(--_first_ptr, clone);
        }

        void pop_front() {
            assert(!empty());
            std::ranges::destroy_at(_first_ptr++);
        }

        void push_back(const_reference value) {
            value_type clone{ value };
            if (_last() == _end()) {
                _reallocate(size() + 1, 1);
            }
            std::ranges::construct_at(_last_ptr++, clone);
        }

        void pop_back() {
            assert(!empty());
            std::ranges::destroy_at(--_last_ptr);
        }

        iterator insert(const_iterator pos, const_reference value) {
            return this->insert(pos, 1, value);
        }

        iterator insert(const_iterator pos, size_type count, const_reference value) {
            value_type clone{ value };
            difference_type offset{ pos - _first() };
            if (static_cast<size_type>(_end() - _last()) < count) {
                _reallocate(size() + count, count);
            }

            pointer i{ _first() + offset }, new_i{ i + count };
            if (new_i <= _last()) {
                pointer j{ _last() - count };
                std::ranges::uninitialized_move(j, _last(), _last(), _end());
                std::ranges::move_backward(i, j, _last());
                std::ranges::fill(i, new_i, clone);
            }
            else {
                std::ranges::uninitialized_move(i, _last(), new_i, _end());
                std::ranges::uninitialized_fill(_last(), new_i, clone);
                std::ranges::fill(i, _last(), clone);
            }
            _last_ptr += count;

            return i;
        }

        template <std::input_iterator It>
        iterator insert(const_iterator pos, It first, It last) {
            difference_type pos_offset{ pos - _first() }, last_offset{ _last() - _first() };
            std::ranges::copy(first, last, std::back_inserter(*this));
            std::ranges::rotate(_first() + pos_offset, _first() + last_offset, _last());
            return _first() + pos_offset;
        }

        iterator insert(const_iterator pos, std::initializer_list<value_type> list) {
            return this->insert(pos, list.begin(), list.end());
        }

        iterator erase(const_iterator pos) {
            auto i{ const_cast<pointer>(pos) };
            assert(i != _last());
            _last_ptr = std::ranges::move(i + 1, _last(), i).out;
            std::ranges::destroy_at(_last());
            return i;
        }

        iterator erase(const_iterator first, const_iterator last) {
            auto i{ const_cast<pointer>(first) }, e{ const_cast<pointer>(last) }, new_last{ std::ranges::move(e, _last(), i).out };
            std::ranges::destroy(new_last, _last());
            _last_ptr = new_last;
            return i;
        }

        friend bool operator==(const Deque& left, const Deque& right) {
            return std::ranges::equal(left, right);
        }

        friend auto operator<=>(const Deque& left, const Deque& right) {
            return std::lexicographical_compare_three_way(left.begin(), left.end(), right.begin(), right.end());
        }
    };

    template <class It>
    Deque(It, It) -> Deque<std::iter_value_t<It>>;

    export template <class T>
    class List {
    public:
        using value_type = T;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using reference = value_type&;
        using const_reference = const value_type&;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

    private:
        struct NodeBase {
            NodeBase* prev{ this };
            NodeBase* next{ this };
        };

        struct Node : NodeBase {
            value_type value;
        };

    public:
        class iterator {
            friend List;

        public:
            using value_type = T;
            using pointer = value_type*;
            using reference = value_type&;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::bidirectional_iterator_tag;

        private:
            NodeBase* _ptr{};

            iterator(NodeBase* ptr) :
                _ptr{ ptr } {}

        public:
            iterator() = default;

            reference operator*() const {
                return static_cast<Node*>(_ptr)->value;
            }

            pointer operator->() const {
                return std::addressof(static_cast<Node*>(_ptr)->value);
            }

            friend bool operator==(iterator left, iterator right) {
                return left._ptr == right._ptr;
            }

            iterator& operator++() {
                _ptr = _ptr->next;
                return *this;
            }

            iterator operator++(int) {
                iterator temp{ *this };
                ++*this;
                return temp;
            }

            iterator& operator--() {
                _ptr = _ptr->prev;
                return *this;
            }

            iterator operator--(int) {
                iterator temp{ *this };
                --*this;
                return temp;
            }
        };

        using const_iterator = std::const_iterator<iterator>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    private:
        NodeBase* _head{ new NodeBase };
        size_type _size{};

        template <class... Args>
        NodeBase* _insert(NodeBase* pos, size_type count, const Args&... args) {
            NodeBase *prev{ pos->prev }, *i{ prev };
            _size += count;
            while (count-- != 0) {
                i = i->next = new Node{ i, i->next, args... };
            }
            i->next->prev = i;
            return prev->next;
        }

        template <class... Args>
        void _resize(size_type new_size, const Args&... args) {
            if (new_size <= _size) {
                while (_size != new_size) {
                    pop_back();
                }
            }
            else {
                this->_insert(_head, new_size - _size, args...);
            }
        }

    public:
        List() = default;

        explicit List(size_type size) {
            this->insert(end(), size, {});
        }

        List(size_type size, const_reference value) {
            this->insert(end(), size, value);
        }

        template <std::input_iterator It>
        List(It first, It last) {
            this->insert(end(), first, last);
        }

        List(std::initializer_list<value_type> list) :
            List(list.begin(), list.end()) {}

        List(const List& other) :
            List(other.begin(), other.end()) {}

        List(List&& other) noexcept {
            this->swap(other);
        }

        ~List() {
            clear();
            delete _head;
        }

        List& operator=(const List& other) {
            List temp{ other };
            this->swap(temp);
            return *this;
        }

        List& operator=(List&& other) noexcept {
            this->swap(other);
            return *this;
        }

        void swap(List& other) noexcept {
            std::ranges::swap(_head, other._head);
            std::ranges::swap(_size, other._size);
        }

        friend void swap(List& left, List& right) noexcept {
            left.swap(right);
        }

        bool empty() const {
            return _size == 0;
        }

        size_type size() const {
            return _size;
        }

        void clear() {
            this->erase(begin(), end());
        }

        void resize(size_type new_size) {
            _resize(new_size);
        }

        void resize(size_type new_size, const_reference value) {
            this->_resize(new_size, value);
        }

        iterator begin() {
            return _head->next;
        }

        const_iterator begin() const {
            return iterator{ _head->next };
        }

        iterator end() {
            return _head;
        }

        const_iterator end() const {
            return iterator{ _head };
        }

        const_iterator cbegin() const {
            return begin();
        }

        const_iterator cend() const {
            return end();
        }

        reverse_iterator rbegin() {
            return reverse_iterator{ end() };
        }

        const_reverse_iterator rbegin() const {
            return const_reverse_iterator{ end() };
        }

        reverse_iterator rend() {
            return reverse_iterator{ begin() };
        }

        const_reverse_iterator rend() const {
            return const_reverse_iterator{ begin() };
        }

        const_reverse_iterator crbegin() const {
            return rbegin();
        }

        const_reverse_iterator crend() const {
            return rend();
        }

        reference front() {
            assert(!empty());
            return *begin();
        }

        const_reference front() const {
            assert(!empty());
            return *begin();
        }

        reference back() {
            assert(!empty());
            return *--end();
        }

        const_reference back() const {
            assert(!empty());
            return *--end();
        }

        void push_front(const_reference value) {
            _head->next->next->prev = _head->next = new Node{ _head, _head->next, value };
            ++_size;
        }

        void pop_front() {
            assert(!empty());
            this->erase(begin());
        }

        void push_back(const_reference value) {
            _head->prev->prev->next = _head->prev = new Node{ _head->prev, _head, value };
            ++_size;
        }

        void pop_back() {
            assert(!empty());
            this->erase(--end());
        }

        iterator insert(const_iterator pos, const_reference value) {
            return this->insert(pos, 1, value);
        }

        iterator insert(const_iterator pos, size_type count, const_reference value) {
            return this->_insert(pos.base()._ptr, count, value);
        }

        template <std::input_iterator It>
        iterator insert(const_iterator pos, It first, It last) {
            NodeBase *prev{ pos.base()._ptr->prev }, *i{ prev };
            while (first != last) {
                i = i->next = new Node{ i, i->next, *first };
                ++first, ++_size;
            }
            i->next->prev = i;
            return prev->next;
        }

        iterator insert(const_iterator pos, std::initializer_list<value_type> list) {
            return this->insert(pos, list.begin(), list.end());
        }

        iterator erase(const_iterator pos) {
            NodeBase* i{ pos.base()._ptr->prev };
            delete static_cast<Node*>(std::exchange(i->next, i->next->next));
            --_size;
            i->next->prev = i;
            return i->next;
        }

        iterator erase(const_iterator first, const_iterator last) {
            NodeBase *i{ first.base()._ptr }, *e{ last.base()._ptr };
            i->prev->next = e;
            e->prev = i->prev;
            while (i != e) {
                delete static_cast<Node*>(std::exchange(i, i->next));
                --_size;
            }
            return i;
        }

        friend bool operator==(const List& left, const List& right) {
            return std::ranges::equal(left, right);
        }

        friend auto operator<=>(const List& left, const List& right) {
            return std::lexicographical_compare_three_way(left.begin(), left.end(), right.begin(), right.end());
        }
    };

    template <class It>
    List(It, It) -> List<std::iter_value_t<It>>;

}
