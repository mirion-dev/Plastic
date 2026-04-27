module;

#include <cassert>

export module plastic.linear_structure;

import std;

namespace plastic {

    template <class T>
    class dynamic_storage : std::allocator<T> {
        using allocator = std::allocator<T>;

        T* _begin{};
        T* _end{};

    public:
        dynamic_storage() = default;

        dynamic_storage(std::size_t size) {
            if (size != 0) {
                _begin = allocator::allocate(size);
                _end = _begin + size;
            }
        }

        dynamic_storage(dynamic_storage&& other) {
            swap(other);
        }

        ~dynamic_storage() {
            if (_begin != nullptr) {
                allocator::deallocate(_begin, _end - _begin);
            }
        }

        dynamic_storage& operator=(dynamic_storage&& other) {
            swap(other);
            return *this;
        }

        void swap(dynamic_storage& other) {
            std::ranges::swap(_begin, other._begin);
            std::ranges::swap(_end, other._end);
        }

        friend void swap(dynamic_storage& left, dynamic_storage& right) {
            left.swap(right);
        }

        T* begin() {
            return _begin;
        }

        const T* begin() const {
            return _begin;
        }

        T* end() {
            return _end;
        }

        const T* end() const {
            return _end;
        }
    };

    template <std::input_iterator It, class... Args>
        requires (sizeof...(Args) <= 1)
    static void construct(It first, It last, const Args&... args) {
        if constexpr (sizeof...(Args) == 0) {
            std::uninitialized_value_construct(first, last);
        }
        else {
            std::uninitialized_fill(first, last, args...);
        }
    }

    export template <class T>
    class vector {
    public:
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using iterator = pointer;
        using const_iterator = const_pointer;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    private:
        dynamic_storage<value_type> _data;
        pointer _last_ptr{ _begin() };

        auto _begin(this auto&& self) {
            return self._data.begin();
        }

        auto _end(this auto&& self) {
            return self._data.end();
        }

        pointer _last() {
            return _last_ptr;
        }

        const_pointer _last() const {
            return _last_ptr;
        }

        void _reallocate(size_type new_capacity) {
            dynamic_storage<value_type> new_data{ std::max(new_capacity, capacity() + (capacity() >> 1)) };
            pointer new_last{ std::uninitialized_move(_begin(), _last(), new_data.begin()) };
            clear();
            _data = std::move(new_data);
            _last_ptr = new_last;
        }

        void _resize(size_type new_size, const auto&... args) {
            if (new_size > capacity()) {
                _reallocate(new_size);
            }

            pointer new_last{ _begin() + new_size };
            if (new_size <= size()) {
                std::destroy(new_last, _last());
            }
            else {
                plastic::construct(_last(), new_last, args...);
            }
            _last_ptr = new_last;
        }

    public:
        vector() = default;

        explicit vector(size_type size) :
            _data{ size },
            _last_ptr{ _end() } {

            plastic::construct(_begin(), _last());
        }

        vector(size_type size, const_reference value) :
            _data{ size },
            _last_ptr{ _end() } {

            plastic::construct(_begin(), _last(), value);
        }

        template <std::input_iterator It>
        vector(It first, It last) {
            std::copy(first, last, std::back_inserter(*this));
        }

        vector(std::initializer_list<value_type> list) :
            vector(list.begin(), list.end()) {}

        vector(const vector& other) :
            vector(other._begin(), other._last()) {}

        vector(vector&& other) {
            swap(other);
        }

        ~vector() {
            clear();
        }

        vector& operator=(const vector& other) {
            vector temp(other);
            swap(temp);
            return *this;
        }

        vector& operator=(vector&& other) {
            swap(other);
            return *this;
        }

        void swap(vector& other) {
            std::ranges::swap(_data, other._data);
            std::ranges::swap(_last_ptr, other._last_ptr);
        }

        friend void swap(vector& left, vector& right) {
            left.swap(right);
        }

        bool empty() const {
            return _begin() == _last();
        }

        size_type size() const {
            return _last() - _begin();
        }

        static size_type max_size() {
            return std::numeric_limits<size_type>::max();
        }

        void clear() {
            std::destroy(_begin(), _last());
            _last_ptr = _begin();
        }

        void resize(size_type new_size) {
            _resize(new_size);
        }

        void resize(size_type new_size, const_reference value) {
            value_type clone{ value };
            _resize(new_size, clone);
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
            return _begin();
        }

        const_iterator begin() const {
            return _begin();
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
            return _begin()[index];
        }

        const_reference operator[](size_type index) const {
            assert(index < size());
            return _begin()[index];
        }

        reference front() {
            assert(!empty());
            return *_begin();
        }

        const_reference front() const {
            assert(!empty());
            return *_begin();
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
            return _begin();
        }

        const_pointer data() const {
            return _begin();
        }

        void push_back(const_reference value) {
            value_type clone{ value };
            if (_last() == _end()) {
                _reallocate(size() + 1);
            }
            std::construct_at(_last_ptr++, clone);
        }

        void pop_back() {
            assert(!empty());
            std::destroy_at(--_last_ptr);
        }

        iterator insert(const_iterator pos, const_reference value) {
            return insert(pos, 1, value);
        }

        iterator insert(const_iterator pos, size_type count, const_reference value) {
            value_type clone{ value };
            difference_type offset{ pos - _begin() };
            if (static_cast<size_type>(_end() - _last()) < count) {
                _reallocate(size() + count);
            }

            pointer i{ _begin() + offset }, new_i{ i + count };
            if (new_i <= _last()) {
                pointer j{ _last() - count };
                std::uninitialized_move(j, _last(), _last());
                std::move_backward(i, j, _last());
                std::fill(i, new_i, clone);
            }
            else {
                std::uninitialized_move(i, _last(), new_i);
                plastic::construct(_last(), new_i, clone);
                std::fill(i, _last(), clone);
            }
            _last_ptr += count;

            return i;
        }

        template <std::input_iterator It>
        iterator insert(const_iterator pos, It first, It last) {
            difference_type pos_offset{ pos - _begin() }, last_offset{ _last() - _begin() };
            std::copy(first, last, std::back_inserter(*this));
            std::rotate(_begin() + pos_offset, _begin() + last_offset, _last());
            return _begin() + pos_offset;
        }

        iterator insert(const_iterator pos, std::initializer_list<value_type> list) {
            return insert(pos, list.begin(), list.end());
        }

        iterator erase(const_iterator pos) {
            auto i{ const_cast<pointer>(pos) };
            assert(i != _last());
            _last_ptr = std::move(i + 1, _last(), i);
            std::destroy_at(_last());
            return i;
        }

        iterator erase(const_iterator first, const_iterator last) {
            auto i{ const_cast<pointer>(first) }, e{ const_cast<pointer>(last) }, new_last{ std::move(e, _last(), i) };
            std::destroy(new_last, _last());
            _last_ptr = new_last;
            return i;
        }

        friend bool operator==(const vector& left, const vector& right) {
            return std::equal(left._begin(), left._last(), right._begin(), right._last());
        }

        friend auto operator<=>(const vector& left, const vector& right) {
            return std::lexicographical_compare_three_way(left._begin(), left._last(), right._begin(), right._last());
        }
    };

    template <class It>
    vector(It, It) -> vector<std::iter_value_t<It>>;

    export template <class T>
    class deque {
    public:
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using iterator = pointer;
        using const_iterator = const_pointer;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    private:
        dynamic_storage<value_type> _data;
        pointer _first_ptr{ _begin() };
        pointer _last_ptr{ _begin() };

        auto _begin(this auto&& self) {
            return self._data.begin();
        }

        auto _end(this auto&& self) {
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

            dynamic_storage<value_type> new_data{ std::max(new_capacity, capacity() << 1) };
            pointer new_first{ new_data.begin() + (new_capacity - new_size >> 1) + left_reserved };
            pointer new_last{ std::uninitialized_move(_first(), _last(), new_first) };
            clear();
            _data = std::move(new_data);
            _first_ptr = new_first;
            _last_ptr = new_last;

            return _first() + new_size;
        }

        void _resize(size_type new_size, const auto&... args) {
            if (new_size > capacity()) {
                pointer new_last{ _reallocate(new_size, new_size - size()) };
                plastic::construct(_last(), new_last, args...);
                _last_ptr = new_last;
                return;
            }

            pointer new_first{ _begin() + (capacity() - new_size >> 1) }, new_last{ new_first + new_size };
            if (new_size <= size()) {
                pointer middle{ _first() + new_size };
                if (_last() <= new_first || _first() >= new_last) {
                    std::uninitialized_move(_first(), middle, new_first);
                    std::destroy(_first(), _last());
                }
                else if (_last() <= new_last) {
                    pointer j{ middle - (new_last - _last()) };
                    std::uninitialized_move(j, middle, _last());
                    std::move_backward(_first(), j, _last());
                    std::destroy(_first(), new_first);
                }
                else if (_first() >= new_first) {
                    pointer j{ _first() + (_first() - new_first) };
                    std::uninitialized_move(_first(), j, new_first);
                    std::move(j, middle, _first());
                    std::destroy(new_last, _last());
                }
                else {
                    std::move_backward(_first(), middle, new_last);
                    std::destroy(_first(), new_first);
                    std::destroy(new_last, _last());
                }
            }
            else {
                pointer new_middle{ new_first + size() };
                if (_last() <= new_first || _first() >= new_middle) {
                    std::uninitialized_move(_first(), _last(), new_first);
                    std::destroy(_first(), _last());
                }
                else if (_first() <= new_first) {
                    pointer j{ _last() - (new_middle - _last()) };
                    std::uninitialized_move(j, _last(), _last());
                    std::move_backward(_first(), j, _last());
                    std::destroy(_first(), new_first);
                }
                else {
                    pointer j{ _first() + (_first() - new_first) };
                    std::uninitialized_move(_first(), j, new_first);
                    std::move(j, _last(), _first());
                    std::destroy(new_middle, _last());
                }
                plastic::construct(new_middle, new_last, args...);
            }
            _first_ptr = new_first;
            _last_ptr = new_last;
        }

    public:
        deque() = default;

        explicit deque(size_type size) :
            _data{ size },
            _first_ptr{ _begin() },
            _last_ptr{ _end() } {

            plastic::construct(_first(), _last());
        }

        deque(size_type size, const_reference value) :
            _data{ size },
            _first_ptr{ _begin() },
            _last_ptr{ _end() } {

            plastic::construct(_first(), _last(), value);
        }

        template <std::input_iterator It>
        deque(It first, It last) {
            std::copy(first, last, std::back_inserter(*this));
        }

        deque(std::initializer_list<value_type> list) :
            deque(list.begin(), list.end()) {}

        deque(const deque& other) :
            deque(other._first(), other._last()) {}

        deque(deque&& other) {
            swap(other);
        }

        ~deque() {
            clear();
        }

        deque& operator=(const deque& other) {
            deque temp(other);
            swap(temp);
            return *this;
        }

        deque& operator=(deque&& other) {
            swap(other);
            return *this;
        }

        void swap(deque& other) {
            std::ranges::swap(_data, other._data);
            std::ranges::swap(_first_ptr, other._first_ptr);
            std::ranges::swap(_last_ptr, other._last_ptr);
        }

        friend void swap(deque& left, deque& right) {
            left.swap(right);
        }

        bool empty() const {
            return _first() == _last();
        }

        size_type size() const {
            return _last() - _first();
        }

        static size_type max_size() {
            return std::numeric_limits<size_type>::max();
        }

        void clear() {
            std::destroy(_first(), _last());
            _first_ptr = _last_ptr = _begin() + (capacity() >> 1);
        }

        void resize(size_type new_size) {
            _resize(new_size);
        }

        void resize(size_type new_size, const_reference value) {
            value_type clone{ value };
            _resize(new_size, clone);
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
            std::construct_at(--_first_ptr, clone);
        }

        void pop_front() {
            assert(!empty());
            std::destroy_at(_first_ptr++);
        }

        void push_back(const_reference value) {
            value_type clone{ value };
            if (_last() == _end()) {
                _reallocate(size() + 1, 1);
            }
            std::construct_at(_last_ptr++, clone);
        }

        void pop_back() {
            assert(!empty());
            std::destroy_at(--_last_ptr);
        }

        iterator insert(const_iterator pos, const_reference value) {
            return insert(pos, 1, value);
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
                std::uninitialized_move(j, _last(), _last());
                std::move_backward(i, j, _last());
                std::fill(i, new_i, clone);
            }
            else {
                std::uninitialized_move(i, _last(), new_i);
                plastic::construct(_last(), new_i, clone);
                std::fill(i, _last(), clone);
            }
            _last_ptr += count;

            return i;
        }

        template <std::input_iterator It>
        iterator insert(const_iterator pos, It first, It last) {
            difference_type pos_offset{ pos - _first() }, last_offset{ _last() - _first() };
            std::copy(first, last, std::back_inserter(*this));
            std::rotate(_first() + pos_offset, _first() + last_offset, _last());
            return _first() + pos_offset;
        }

        iterator insert(const_iterator pos, std::initializer_list<value_type> list) {
            return insert(pos, list.begin(), list.end());
        }

        iterator erase(const_iterator pos) {
            auto i{ const_cast<pointer>(pos) };
            assert(i != _last());
            _last_ptr = std::move(i + 1, _last(), i);
            std::destroy_at(_last());
            return i;
        }

        iterator erase(const_iterator first, const_iterator last) {
            auto i{ const_cast<pointer>(first) }, e{ const_cast<pointer>(last) }, new_last{ std::move(e, _last(), i) };
            std::destroy(new_last, _last());
            _last_ptr = new_last;
            return i;
        }

        friend bool operator==(const deque& left, const deque& right) {
            return std::equal(left._first(), left._last(), right._first(), right._last());
        }

        friend auto operator<=>(const deque& left, const deque& right) {
            return std::lexicographical_compare_three_way(left._first(), left._last(), right._first(), right._last());
        }
    };

    template <class It>
    deque(It, It) -> deque<std::iter_value_t<It>>;

    export template <class T>
    class forward_list {
    public:
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;

    private:
        struct node_base {
            node_base* next{ this };
        };

        struct node : node_base {
            value_type value;
        };

    public:
        class iterator {
            friend forward_list;

        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using iterator_category = std::forward_iterator_tag;

        private:
            node_base* _ptr{};

            iterator(node_base* ptr) :
                _ptr{ ptr } {}

        public:
            iterator() = default;

            reference operator*() const {
                return static_cast<node*>(_ptr)->value;
            }

            pointer operator->() const {
                return std::addressof(static_cast<node*>(_ptr)->value);
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
        };

        using const_iterator = std::const_iterator<iterator>;

    private:
        node_base* _head{ new node_base };
        size_type _size{};

        node_base* _insert_after(node_base* pos, size_type count, const auto&... args) {
            _size += count;
            while (count-- != 0) {
                pos = pos->next = new node{ pos->next, args... };
            }
            return pos;
        }

        void _resize(size_type new_size, const auto&... args) {
            if (new_size <= _size) {
                erase_after(std::ranges::next(before_begin(), new_size), end());
            }
            else {
                _insert_after(std::ranges::next(before_begin(), _size)._ptr, new_size - _size, args...);
            }
        }

    public:
        forward_list() = default;

        explicit forward_list(size_type size) {
            insert_after(before_begin(), size, {});
        }

        forward_list(size_type size, const_reference value) {
            insert_after(before_begin(), size, value);
        }

        template <std::input_iterator It>
        forward_list(It first, It last) {
            insert_after(before_begin(), first, last);
        }

        forward_list(std::initializer_list<value_type> list) :
            forward_list(list.begin(), list.end()) {}

        forward_list(const forward_list& other) :
            forward_list(other.begin(), other.end()) {}

        forward_list(forward_list&& other) {
            swap(other);
        }

        ~forward_list() {
            clear();
            delete _head;
        }

        forward_list& operator=(const forward_list& other) {
            forward_list temp{ other };
            swap(temp);
            return *this;
        }

        forward_list& operator=(forward_list&& other) {
            swap(other);
            return *this;
        }

        void swap(forward_list& other) {
            std::ranges::swap(_head, other._head);
            std::ranges::swap(_size, other._size);
        }

        friend void swap(forward_list& left, forward_list& right) {
            left.swap(right);
        }

        bool empty() const {
            return _size == 0;
        }

        size_type size() const {
            return _size;
        }

        static size_type max_size() {
            return std::numeric_limits<size_type>::max();
        }

        void clear() {
            erase_after(before_begin(), end());
        }

        void resize(size_type new_size) {
            _resize(new_size);
        }

        void resize(size_type new_size, const_reference value) {
            _resize(new_size, value);
        }

        iterator before_begin() {
            return end();
        }

        const_iterator before_begin() const {
            return end();
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

        const_iterator cbefore_begin() const {
            return before_begin();
        }

        const_iterator cbegin() const {
            return begin();
        }

        const_iterator cend() const {
            return end();
        }

        reference front() {
            assert(!empty());
            return *begin();
        }

        const_reference front() const {
            assert(!empty());
            return *begin();
        }

        void push_front(const_reference value) {
            _head->next = new node{ _head->next, value };
            ++_size;
        }

        void pop_front() {
            assert(!empty());
            erase_after(before_begin());
        }

        iterator insert_after(const_iterator pos, const_reference value) {
            return insert_after(pos, 1, value);
        }

        iterator insert_after(const_iterator pos, size_type count, const_reference value) {
            return _insert_after(pos.base()._ptr, count, value);
        }

        template <std::input_iterator It>
        iterator insert_after(const_iterator pos, It first, It last) {
            node_base* i{ pos.base()._ptr };
            while (first != last) {
                i = i->next = new node{ i->next, *first };
                ++first, ++_size;
            }
            return i;
        }

        iterator insert_after(const_iterator pos, std::initializer_list<value_type> list) {
            return insert_after(pos, list.begin(), list.end());
        }

        iterator erase_after(const_iterator pos) {
            node_base* i{ pos.base()._ptr };
            delete static_cast<node*>(std::exchange(i->next, i->next->next));
            --_size;
            return i->next;
        }

        iterator erase_after(const_iterator first, const_iterator last) {
            node_base *i{ first.base()._ptr }, *e{ last.base()._ptr };
            i = std::exchange(i->next, e);
            while (i != e) {
                delete static_cast<node*>(std::exchange(i, i->next));
                --_size;
            }
            return i;
        }

        friend bool operator==(const forward_list& left, const forward_list& right) {
            return std::equal(left.begin(), left.end(), right.begin(), right.end());
        }

        friend auto operator<=>(const forward_list& left, const forward_list& right) {
            return std::lexicographical_compare_three_way(left.begin(), left.end(), right.begin(), right.end());
        }
    };

    template <class It>
    forward_list(It, It) -> forward_list<std::iter_value_t<It>>;

    export template <class T>
    class list {
    public:
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;

    private:
        struct node_base {
            node_base* prev{ this };
            node_base* next{ this };
        };

        struct node : node_base {
            value_type value;
        };

    public:
        class iterator {
            friend list;

        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using iterator_category = std::bidirectional_iterator_tag;

        private:
            node_base* _ptr{};

            iterator(node_base* ptr) :
                _ptr{ ptr } {}

        public:
            iterator() = default;

            reference operator*() const {
                return static_cast<node*>(_ptr)->value;
            }

            pointer operator->() const {
                return std::addressof(static_cast<node*>(_ptr)->value);
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
        node_base* _head{ new node_base };
        size_type _size{};

        node_base* _insert(node_base* pos, size_type count, const auto&... args) {
            node_base *prev{ pos->prev }, *i{ prev };
            _size += count;
            while (count-- != 0) {
                i = i->next = new node{ i, i->next, args... };
            }
            i->next->prev = i;
            return prev->next;
        }

        void _resize(size_type new_size, const auto&... args) {
            if (new_size <= _size) {
                while (_size != new_size) {
                    pop_back();
                }
            }
            else {
                _insert(_head, new_size - _size, args...);
            }
        }

    public:
        list() = default;

        explicit list(size_type size) {
            insert(end(), size, {});
        }

        list(size_type size, const_reference value) {
            insert(end(), size, value);
        }

        template <std::input_iterator It>
        list(It first, It last) {
            insert(end(), first, last);
        }

        list(std::initializer_list<value_type> list) :
            list(list.begin(), list.end()) {}

        list(const list& other) :
            list(other.begin(), other.end()) {}

        list(list&& other) {
            swap(other);
        }

        ~list() {
            clear();
            delete _head;
        }

        list& operator=(const list& other) {
            list temp{ other };
            swap(temp);
            return *this;
        }

        list& operator=(list&& other) {
            swap(other);
            return *this;
        }

        void swap(list& other) {
            std::ranges::swap(_head, other._head);
            std::ranges::swap(_size, other._size);
        }

        friend void swap(list& left, list& right) {
            left.swap(right);
        }

        bool empty() const {
            return _size == 0;
        }

        size_type size() const {
            return _size;
        }

        static size_type max_size() {
            return std::numeric_limits<size_type>::max();
        }

        void clear() {
            erase(begin(), end());
        }

        void resize(size_type new_size) {
            _resize(new_size);
        }

        void resize(size_type new_size, const_reference value) {
            _resize(new_size, value);
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
            _head->next->next->prev = _head->next = new node{ _head, _head->next, value };
            ++_size;
        }

        void pop_front() {
            assert(!empty());
            erase(begin());
        }

        void push_back(const_reference value) {
            _head->prev->prev->next = _head->prev = new node{ _head->prev, _head, value };
            ++_size;
        }

        void pop_back() {
            assert(!empty());
            erase(--end());
        }

        iterator insert(const_iterator pos, const_reference value) {
            return insert(pos, 1, value);
        }

        iterator insert(const_iterator pos, size_type count, const_reference value) {
            return _insert(pos.base()._ptr, count, value);
        }

        template <std::input_iterator It>
        iterator insert(const_iterator pos, It first, It last) {
            node_base *prev{ pos.base()._ptr->prev }, *i{ prev };
            while (first != last) {
                i = i->next = new node{ i, i->next, *first };
                ++first, ++_size;
            }
            i->next->prev = i;
            return prev->next;
        }

        iterator insert(const_iterator pos, std::initializer_list<value_type> list) {
            return insert(pos, list.begin(), list.end());
        }

        iterator erase(const_iterator pos) {
            node_base* i{ pos.base()._ptr->prev };
            delete static_cast<node*>(std::exchange(i->next, i->next->next));
            --_size;
            i->next->prev = i;
            return i->next;
        }

        iterator erase(const_iterator first, const_iterator last) {
            node_base *i{ first.base()._ptr }, *e{ last.base()._ptr };
            i->prev->next = e;
            e->prev = i->prev;
            while (i != e) {
                delete static_cast<node*>(std::exchange(i, i->next));
                --_size;
            }
            return i;
        }

        friend bool operator==(const list& left, const list& right) {
            return std::equal(left.begin(), left.end(), right.begin(), right.end());
        }

        friend auto operator<=>(const list& left, const list& right) {
            return std::lexicographical_compare_three_way(left.begin(), left.end(), right.begin(), right.end());
        }
    };

    template <class It>
    list(It, It) -> list<std::iter_value_t<It>>;

}
