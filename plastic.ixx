module;

#include <assert.h>

export module plastic;

import std;

export namespace plastic {

    struct uninitialized_t {
        explicit uninitialized_t() noexcept = default;
    };

    plastic::uninitialized_t uninitialized{};

    template<class T>
    class vector {
        T* _begin{};
        T* _last{};
        T* _end{};

        void _extend(std::size_t size) noexcept {
            reserve(capacity() + std::max(capacity() >> 1, size));
        }

    public:
        class iterator {
            friend class vector;

            T* _ptr{};

        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using iterator_category = std::contiguous_iterator_tag;

            iterator() noexcept = default;

            iterator(T* ptr) noexcept :
                _ptr{ ptr } {}

            reference operator*() const noexcept {
                return *_ptr;
            }

            pointer operator->() const noexcept {
                return _ptr;
            }

            reference operator[](std::size_t index) const noexcept {
                return _ptr[index];
            }

            friend bool operator==(iterator iter1, iterator iter2) noexcept {
                return iter1._ptr == iter2._ptr;
            }

            friend auto operator<=>(iterator iter1, iterator iter2) noexcept {
                return iter1._ptr <=> iter2._ptr;
            }

            iterator& operator+=(difference_type diff) noexcept {
                _ptr += diff;
                return *this;
            }

            iterator& operator-=(difference_type diff) noexcept {
                _ptr -= diff;
                return *this;
            }

            friend iterator operator+(iterator iter, difference_type diff) noexcept {
                return iter += diff;
            }

            friend iterator operator+(difference_type diff, iterator iter) noexcept {
                return iter += diff;
            }

            friend iterator operator-(iterator iter, difference_type diff) noexcept {
                return iter -= diff;
            }

            friend difference_type operator-(iterator iter1, iterator iter2) noexcept {
                return iter1._ptr - iter2._ptr;
            }

            iterator& operator++() noexcept {
                ++_ptr;
                return *this;
            }

            iterator operator++(int) noexcept {
                iterator temp{ *this };
                ++*this;
                return temp;
            }

            iterator& operator--() noexcept {
                --_ptr;
                return *this;
            }

            iterator operator--(int) noexcept {
                iterator temp{ *this };
                --*this;
                return temp;
            }
        };

        using const_iterator = std::const_iterator<iterator>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::const_iterator<reverse_iterator>;

        explicit vector() noexcept = default;

        explicit vector(plastic::uninitialized_t, std::size_t size) noexcept :
            _begin{ new T[size] },
            _last{ _begin + size },
            _end{ _last } {}

        explicit vector(std::size_t size, const T& value = {}) noexcept :
            vector(plastic::uninitialized, size) {

            std::uninitialized_fill(_begin, _end, value);
        }

        template<std::input_iterator It>
        explicit vector(It first, It last) noexcept :
            vector(plastic::uninitialized, std::distance(first, last)) {

            std::uninitialized_copy(first, last, _begin);
        }

        explicit vector(std::initializer_list<T> list) noexcept :
            vector(list.begin(), list.end()) {}

        explicit vector(const vector& other) noexcept :
            vector(other._begin, other._last) {}

        ~vector() noexcept {
            delete[] _begin;
        }

        vector& operator=(const vector& other) noexcept {
            if (this == &other) {
                return *this;
            }

            vector temp{ other };
            std::swap(_begin, temp._begin);
            std::swap(_last, temp._last);
            std::swap(_end, temp._end);

            return *this;
        }

        bool empty() const noexcept {
            return _begin == _last;
        }

        std::size_t size() const noexcept {
            return _last - _begin;
        }

        void clear() noexcept {
            std::destroy(_begin, _last);
            _last = _begin;
        }

        void resize(std::size_t new_size, const T& value = {}) noexcept {
            if (new_size == size()) {
                return;
            }

            if (new_size < size()) {
                T* new_last{ _begin + new_size };
                std::destroy(new_last, _last);
                _last = new_last;
                return;
            }

            if (new_size <= capacity()) {
                T* new_last{ _begin + new_size };
                std::uninitialized_fill(_last, new_last, value);
                _last = new_last;
                return;
            }

            reserve(new_size);
            std::uninitialized_fill(_last, _end, value);
            _last = _end;
        }

        std::size_t capacity() const noexcept {
            return _end - _begin;
        }

        void reserve(std::size_t new_capacity) noexcept {
            if (new_capacity <= capacity()) {
                return;
            }

            T* new_begin{ new T[new_capacity] };
            T* new_last{ std::uninitialized_move(_begin, _last, new_begin) };
            T* new_end{ new_begin + new_capacity };
            delete[] _begin;

            _begin = new_begin;
            _last = new_last;
            _end = new_end;
        }

        iterator begin() noexcept {
            return _begin;
        }

        const_iterator begin() const noexcept {
            return _begin;
        }

        const_iterator cbegin() const noexcept {
            return _begin;
        }

        iterator end() noexcept {
            return _last;
        }

        const_iterator end() const noexcept {
            return _last;
        }

        const_iterator cend() const noexcept {
            return _last;
        }

        reverse_iterator rbegin() noexcept {
            return reverse_iterator{ _last };
        }

        const_reverse_iterator rbegin() const noexcept {
            return reverse_iterator{ _last };
        }

        const_reverse_iterator crbegin() const noexcept {
            return reverse_iterator{ _last };
        }

        reverse_iterator rend() noexcept {
            return reverse_iterator{ _begin };
        }

        const_reverse_iterator rend() const noexcept {
            return reverse_iterator{ _begin };
        }

        const_reverse_iterator crend() const noexcept {
            return reverse_iterator{ _begin };
        }

        auto&& operator[](this auto&& self, std::size_t index) noexcept {
            assert(index < self.size());
            return std::forward_like<decltype(self)>(self._begin[index]);
        }

        auto&& front(this auto&& self) noexcept {
            assert(!self.empty());
            return std::forward_like<decltype(self)>(*self._begin);
        }

        auto&& back(this auto&& self) noexcept {
            assert(!self.empty());
            return std::forward_like<decltype(self)>(self._last[-1]);
        }

        auto data(this auto&& self) noexcept {
            return self._begin;
        }

        void push_back(const T& value) noexcept {
            if (_last == _end) {
                _extend(1);
            }
            *_last++ = value;
        }

        void pop_back() noexcept {
            assert(!empty());
            std::destroy_at(--_last);
        }

        iterator insert(iterator pos, const T& value) noexcept {
            return insert(pos, 1, value);
        }

        iterator insert(iterator pos, std::size_t count, const T& value) noexcept {
            if (count == 0) {
                return pos;
            }

            if (_end - _last < static_cast<std::ptrdiff_t>(count)) {
                std::ptrdiff_t offset{ pos - begin() };
                _extend(count);
                pos = begin() + offset;
            }

            std::fill(pos, std::move_backward(pos, end(), end() + count), value);
            _last += count;

            return pos;
        }

        template<std::input_iterator It>
        iterator insert(iterator pos, It first, It last) noexcept {
            if (first == last) {
                return pos;
            }

            std::ptrdiff_t count{ std::distance(first, last) };
            if (_end - _last < count) {
                std::ptrdiff_t offset{ pos - begin() };
                _extend(count);
                pos = begin() + offset;
            }

            std::copy_backward(first, last, std::move_backward(pos, end(), end() + count));
            _last += count;

            return pos;
        }

        iterator insert(iterator pos, std::initializer_list<T> list) noexcept {
            return insert(pos, list.begin(), list.end());
        }

        iterator erase(iterator pos) noexcept {
            _last = std::move(++iterator{ pos }, end(), pos)._ptr;
            std::destroy_at(_last);
            return pos;
        }

        iterator erase(iterator first, iterator last) noexcept {
            if (first == last) {
                return first;
            }

            T* new_last{ std::move(last, end(), first)._ptr };
            std::destroy(new_last, _last);
            _last = new_last;

            return first;
        }

        friend bool operator==(const vector& cont1, const vector& cont2) noexcept {
            return std::equal(cont1.begin(), cont1.end(), cont2.begin(), cont2.end());
        }

        friend auto operator<=>(const vector& cont1, const vector& cont2) noexcept {
            return std::lexicographical_compare_three_way(cont1.begin(), cont1.end(), cont2.begin(), cont2.end());
        }
    };

    template<class It>
    explicit plastic::vector(It, It)->plastic::vector<std::iter_value_t<It>>;

    template<class T>
    class deque {
        T* _begin;
        T* _end;
        T* _first;
        T* _last;

        bool _is_contiguous() const noexcept {
            return _first <= _last;
        }

        std::size_t _true_capacity() const noexcept {
            return _end - _begin;
        }

        void _extend(std::size_t size) noexcept {
            reserve(capacity() + std::max(capacity() >> 1, size));
        }

    public:
        class iterator {
            friend class deque;

            T* _ptr{};
            const deque* _cont{};

            std::ptrdiff_t _offset() const noexcept {
                std::ptrdiff_t offset{ _ptr - _cont->_first };
                if (offset < 0) {
                    offset += _cont->_true_capacity();
                }
                return offset;
            }

        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using iterator_category = std::random_access_iterator_tag;

            iterator() noexcept = default;

            iterator(T* ptr, const deque* cont) noexcept :
                _ptr{ ptr },
                _cont{ cont } {}

            reference operator*() const noexcept {
                return *_ptr;
            }

            pointer operator->() const noexcept {
                return _ptr;
            }

            reference operator[](std::size_t index) const noexcept {
                return *(*this + index);
            }

            friend bool operator==(iterator iter1, iterator iter2) noexcept {
                return iter1._ptr == iter2._ptr;
            }

            friend auto operator<=>(iterator iter1, iterator iter2) noexcept {
                return iter1._offset() <=> iter2._offset();
            }

            iterator& operator+=(difference_type diff) noexcept {
                if (_cont->_end - _ptr <= diff) {
                    diff -= _cont->_true_capacity();
                }
                else if (_ptr - _cont->_begin <= -diff) {
                    diff += _cont->_true_capacity();
                }
                _ptr += diff;
                return *this;
            }

            iterator& operator-=(difference_type diff) noexcept {
                return *this += -diff;
            }

            friend iterator operator+(iterator iter, difference_type diff) noexcept {
                return iter += diff;
            }

            friend iterator operator+(difference_type diff, iterator iter) noexcept {
                return iter += diff;
            }

            friend iterator operator-(iterator iter, difference_type diff) noexcept {
                return iter -= diff;
            }

            friend difference_type operator-(iterator iter1, iterator iter2) noexcept {
                return iter1._offset() - iter2._offset();
            }

            iterator& operator++() noexcept {
                ++_ptr;
                if (_ptr == _cont->_end) {
                    _ptr = _cont->_begin;
                }
                return *this;
            }

            iterator operator++(int) noexcept {
                iterator temp{ *this };
                ++*this;
                return temp;
            }

            iterator& operator--() noexcept {
                if (_ptr == _cont->_begin) {
                    _ptr = _cont->_end;
                }
                --_ptr;
                return *this;
            }

            iterator operator--(int) noexcept {
                iterator temp{ *this };
                --*this;
                return temp;
            }
        };

        using const_iterator = std::const_iterator<iterator>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::const_iterator<reverse_iterator>;

        explicit deque(plastic::uninitialized_t, std::size_t size) noexcept :
            _begin{ new T[size + 1] },
            _end{ _begin + size + 1 },
            _first{ _begin },
            _last{ _begin + size } {}

        explicit deque(std::size_t size = {}, const T& value = {}) noexcept :
            deque(plastic::uninitialized, size) {

            std::uninitialized_fill(_first, _last, value);
        }

        template<std::input_iterator It>
        explicit deque(It first, It last) noexcept :
            deque(plastic::uninitialized, std::distance(first, last)) {

            std::uninitialized_copy(first, last, _first);
        }

        explicit deque(std::initializer_list<T> list) noexcept :
            deque(list.begin(), list.end()) {}

        explicit deque(const deque& other) noexcept :
            deque(plastic::uninitialized, other.size()) {

            if (other._is_contiguous()) {
                std::uninitialized_copy(other._first, other._last, _first);
            }
            else {
                std::uninitialized_copy(other._begin, other._last, std::uninitialized_copy(other._first, other._end, _first));
            }
        }

        ~deque() noexcept {
            delete[] _begin;
        }

        deque& operator=(const deque& other) noexcept {
            if (this == &other) {
                return *this;
            }

            deque temp{ other };
            std::swap(_begin, temp._begin);
            std::swap(_end, temp._end);
            std::swap(_first, temp._first);
            std::swap(_last, temp._last);

            return *this;
        }

        bool empty() const noexcept {
            return _first == _last;
        }

        std::size_t size() const noexcept {
            return end() - begin();
        }

        void clear() noexcept {
            if (_is_contiguous()) {
                std::destroy(_first, _last);
            }
            else {
                std::destroy(_first, _end);
                std::destroy(_begin, _first);
            }
            _first = _last = _begin;
        }

        void resize(std::size_t new_size, const T& value = {}) noexcept {
            if (new_size == size()) {
                return;
            }

            if (new_size < size()) {
                T* new_last{ (begin() + new_size)._ptr };
                if (new_last <= _last) {
                    std::destroy(new_last, _last);
                }
                else {
                    std::destroy(new_last, _end);
                    std::destroy(_begin, _last);
                }
                _last = new_last;
                return;
            }

            if (new_size <= capacity()) {
                T* new_last{ (begin() + new_size)._ptr };
                if (_last <= new_last) {
                    std::uninitialized_fill(_last, new_last, value);
                }
                else {
                    std::uninitialized_fill(_last, _end, value);
                    std::uninitialized_fill(_begin, new_last, value);
                }
                _last = new_last;
                return;
            }

            reserve(new_size);
            std::uninitialized_fill(_last, _end - 1, value);
            _last = _end - 1;
        }

        std::size_t capacity() const noexcept {
            return _true_capacity() - 1;
        }

        void reserve(std::size_t new_capacity) noexcept {
            if (new_capacity <= capacity()) {
                return;
            }

            T* new_begin{ new T[new_capacity + 1] };
            T* new_end{ new_begin + new_capacity + 1 };
            T* new_last;
            if (_is_contiguous()) {
                new_last = std::uninitialized_move(_first, _last, new_begin);
            }
            else {
                new_last = std::uninitialized_move(_begin, _last, std::uninitialized_move(_first, _end, new_begin));
            }
            delete[] _begin;

            _begin = new_begin;
            _end = new_end;
            _first = new_begin;
            _last = new_last;
        }

        iterator begin() noexcept {
            return { _first, this };
        }

        const_iterator begin() const noexcept {
            return iterator{ _first, this };
        }

        const_iterator cbegin() const noexcept {
            return iterator{ _first, this };
        }

        iterator end() noexcept {
            return { _last, this };
        }

        const_iterator end() const noexcept {
            return iterator{ _last, this };
        }

        const_iterator cend() const noexcept {
            return iterator{ _last, this };
        }

        reverse_iterator rbegin() noexcept {
            return reverse_iterator{ { _last, this } };
        }

        const_reverse_iterator rbegin() const noexcept {
            return reverse_iterator{ { _last, this } };
        }

        const_reverse_iterator crbegin() const noexcept {
            return reverse_iterator{ { _last, this } };
        }

        reverse_iterator rend() noexcept {
            return reverse_iterator{ { _first, this } };
        }

        const_reverse_iterator rend() const noexcept {
            return reverse_iterator{ { _first, this } };
        }

        const_reverse_iterator crend() const noexcept {
            return reverse_iterator{ { _first, this } };
        }

        auto&& operator[](this auto&& self, std::size_t index) noexcept {
            assert(index < self.size());
            return std::forward_like<decltype(self)>(self.begin()[index]);
        }

        auto&& front(this auto&& self) noexcept {
            assert(!self.empty());
            return std::forward_like<decltype(self)>(*self._first);
        }

        auto&& back(this auto&& self) noexcept {
            assert(!self.empty());
            return std::forward_like<decltype(self)>(*--self.end());
        }

        void push_front(const T& value) noexcept {
            if (++end() == begin()) {
                _extend(1);
            }
            if (_first == _begin) {
                _first = _end;
            }
            *--_first = value;
        }

        void pop_front() noexcept {
            assert(!empty());
            std::destroy_at(_first++);
            if (_first == _end) {
                _first = _begin;
            }
        }

        void push_back(const T& value) noexcept {
            if (++end() == begin()) {
                _extend(1);
            }
            *_last++ = value;
            if (_last == _end) {
                _last = _begin;
            }
        }

        void pop_back() noexcept {
            assert(!empty());
            if (_last == _begin) {
                _last = _end;
            }
            std::destroy_at(--_last);
        }

        iterator insert(iterator pos, const T& value) noexcept {
            return insert(pos, 1, value);
        }

        iterator insert(iterator pos, std::size_t count, const T& value) noexcept {
            if (count == 0) {
                return pos;
            }

            if (capacity() - size() < count) {
                std::ptrdiff_t offset{ pos - begin() };
                _extend(count);
                pos = begin() + offset;
            }

            std::fill(pos, std::move_backward(pos, end(), end() + count), value);
            _last = (end() + count)._ptr;

            return pos;
        }

        template<std::input_iterator It>
        iterator insert(iterator pos, It first, It last) noexcept {
            if (first == last) {
                return pos;
            }

            std::ptrdiff_t count{ std::distance(first, last) };
            if (static_cast<std::ptrdiff_t>(capacity() - size()) < count) {
                std::ptrdiff_t offset{ pos - begin() };
                _extend(count);
                pos = begin() + offset;
            }

            std::copy_backward(first, last, std::move_backward(pos, end(), end() + count));
            _last = (end() + count)._ptr;

            return pos;
        }

        iterator insert(iterator pos, std::initializer_list<T> list) noexcept {
            return insert(pos, list.begin(), list.end());
        }

        iterator erase(iterator pos) noexcept {
            _last = std::move(++iterator{ pos }, end(), pos)._ptr;
            std::destroy_at(_last);
            return pos;
        }

        iterator erase(iterator first, iterator last) noexcept {
            if (first == last) {
                return first;
            }

            T* new_last{ std::move(last, end(), first)._ptr };
            std::destroy(new_last, _last);
            _last = new_last;

            return first;
        }

        friend bool operator==(const deque& cont1, const deque& cont2) noexcept {
            return std::equal(cont1.begin(), cont1.end(), cont2.begin(), cont2.end());
        }

        friend auto operator<=>(const deque& cont1, const deque& cont2) noexcept {
            return std::lexicographical_compare_three_way(cont1.begin(), cont1.end(), cont2.begin(), cont2.end());
        }
    };

    template<class It>
    explicit plastic::deque(It, It)->plastic::deque<std::iter_value_t<It>>;

    template<class T>
    class forward_list {
        struct node {
            T value;
            node* next;
        };

        node* _sentinel;
        std::size_t _size;

    public:
        class iterator {
            friend class forward_list;

            node* _ptr{};

        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using iterator_category = std::forward_iterator_tag;

            iterator() noexcept = default;

            iterator(node* ptr) noexcept :
                _ptr{ ptr } {}

            reference operator*() const noexcept {
                return _ptr->value;
            }

            pointer operator->() const noexcept {
                return &_ptr->value;
            }

            friend bool operator==(iterator iter1, iterator iter2) noexcept {
                return iter1._ptr == iter2._ptr;
            }

            iterator& operator++() noexcept {
                _ptr = _ptr->next;
                return *this;
            }

            iterator operator++(int) noexcept {
                iterator temp{ *this };
                ++*this;
                return temp;
            }
        };

        using const_iterator = std::const_iterator<iterator>;

        explicit forward_list() noexcept :
            _sentinel{ new node },
            _size{} {

            _sentinel->next = _sentinel;
        }

        explicit forward_list(std::size_t size, const T& value = {}) noexcept :
            forward_list() {

            insert_after(end(), size, value);
        }

        template<std::input_iterator It>
        explicit forward_list(It first, It last) noexcept :
            forward_list() {

            insert_after(end(), first, last);
        }

        explicit forward_list(std::initializer_list<T> list) noexcept :
            forward_list(list.begin(), list.end()) {}

        explicit forward_list(const forward_list& other) noexcept :
            forward_list(other.begin(), other.end()) {}

        ~forward_list() noexcept {
            clear();
            delete _sentinel;
        }

        forward_list& operator=(const forward_list& other) noexcept {
            if (this == &other) {
                return *this;
            }

            forward_list temp{ other };
            std::swap(_sentinel, temp._sentinel);
            std::swap(_size, temp._size);

            return *this;
        }

        bool empty() const noexcept {
            return _size == 0;
        }

        std::size_t size() const noexcept {
            return _size;
        }

        void clear() noexcept {
            erase_after(end(), end());
        }

        void resize(std::size_t new_size, const T& value = {}) noexcept {
            if (new_size == _size) {
                return;
            }
            if (new_size < _size) {
                erase_after(std::next(end(), new_size), end());
                return;
            }
            insert_after(std::next(end(), _size), new_size - _size, value);
        }

        iterator begin() noexcept {
            return _sentinel->next;
        }

        const_iterator begin() const noexcept {
            return iterator{ _sentinel->next };
        }

        const_iterator cbegin() const noexcept {
            return iterator{ _sentinel->next };
        }

        iterator end() noexcept {
            return _sentinel;
        }

        const_iterator end() const noexcept {
            return iterator{ _sentinel };
        }

        const_iterator cend() const noexcept {
            return iterator{ _sentinel };
        }

        auto&& front(this auto&& self) noexcept {
            assert(!self.empty());
            return std::forward_like<decltype(self)>(self._sentinel->next->value);
        }

        void push_front(const T& value) noexcept {
            _sentinel->next = new node{ value, _sentinel->next };
            ++_size;
        }

        void pop_front() noexcept {
            assert(!empty());
            erase_after(end());
        }

        iterator insert_after(iterator pos, const T& value) noexcept {
            return insert_after(pos, 1, value);
        }

        iterator insert_after(iterator pos, std::size_t count, const T& value) noexcept {
            node* i{ pos._ptr };
            _size += count;
            while (count-- != 0) {
                i = i->next = new node{ value, i->next };
            }
            return i;
        }

        template<std::input_iterator It>
        iterator insert_after(iterator pos, It first, It last) noexcept {
            node* i{ pos._ptr };
            while (first != last) {
                i = i->next = new node{ *first++, i->next };
                ++_size;
            }
            return i;
        }

        iterator insert_after(iterator pos, std::initializer_list<T> list) noexcept {
            return insert_after(pos, list.begin(), list.end());
        }

        iterator erase_after(iterator pos) noexcept {
            node* i{ pos._ptr };
            delete std::exchange(i->next, i->next->next);
            --_size;
            return ++pos;
        }

        iterator erase_after(iterator first, iterator last) noexcept {
            node* i{ first._ptr }, * j{ last._ptr };
            i = std::exchange(i->next, j);
            while (i != j) {
                delete std::exchange(i, i->next);
                --_size;
            }
            return last;
        }

        friend bool operator==(const forward_list& cont1, const forward_list& cont2) noexcept {
            return std::equal(cont1.begin(), cont1.end(), cont2.begin(), cont2.end());
        }

        friend auto operator<=>(const forward_list& cont1, const forward_list& cont2) noexcept {
            return std::lexicographical_compare_three_way(cont1.begin(), cont1.end(), cont2.begin(), cont2.end());
        }
    };

    template<class It>
    explicit plastic::forward_list(It, It)->plastic::forward_list<std::iter_value_t<It>>;

    template<class T>
    class list {
        struct node {
            T value;
            node* prev;
            node* next;
        };

        node* _sentinel;
        std::size_t _size;

    public:
        class iterator {
            friend class list;

            node* _ptr{};

        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using iterator_category = std::bidirectional_iterator_tag;

            iterator() noexcept = default;

            iterator(node* ptr) noexcept :
                _ptr{ ptr } {}

            reference operator*() const noexcept {
                return _ptr->value;
            }

            pointer operator->() const noexcept {
                return &_ptr->value;
            }

            friend bool operator==(iterator iter1, iterator iter2) noexcept {
                return iter1._ptr == iter2._ptr;
            }

            iterator& operator++() noexcept {
                _ptr = _ptr->next;
                return *this;
            }

            iterator operator++(int) noexcept {
                iterator temp{ *this };
                ++*this;
                return temp;
            }

            iterator& operator--() noexcept {
                _ptr = _ptr->prev;
                return *this;
            }

            iterator operator--(int) noexcept {
                iterator temp{ *this };
                --*this;
                return temp;
            }
        };

        using const_iterator = std::const_iterator<iterator>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::const_iterator<reverse_iterator>;

        explicit list() noexcept :
            _sentinel{ new node },
            _size{} {

            _sentinel->next = _sentinel->prev = _sentinel;
        }

        explicit list(std::size_t size, const T& value = {}) noexcept :
            list() {

            insert(end(), size, value);
        }

        template<std::input_iterator It>
        explicit list(It first, It last) noexcept :
            list() {

            insert(end(), first, last);
        }

        explicit list(std::initializer_list<T> list) noexcept :
            list(list.begin(), list.end()) {}

        explicit list(const list& other) noexcept :
            list(other.begin(), other.end()) {}

        ~list() noexcept {
            clear();
            delete _sentinel;
        }

        list& operator=(const list& other) noexcept {
            if (this == &other) {
                return *this;
            }

            list temp{ other };
            std::swap(_sentinel, temp._sentinel);
            std::swap(_size, temp._size);

            return *this;
        }

        bool empty() const noexcept {
            return _size == 0;
        }

        std::size_t size() const noexcept {
            return _size;
        }

        void clear() noexcept {
            erase(begin(), end());
        }

        void resize(std::size_t new_size, const T& value = {}) noexcept {
            if (new_size == _size) {
                return;
            }
            if (new_size < _size) {
                while (_size != new_size) {
                    pop_back();
                }
                return;
            }
            insert(end(), new_size - _size, value);
        }

        iterator begin() noexcept {
            return _sentinel->next;
        }

        const_iterator begin() const noexcept {
            return iterator{ _sentinel->next };
        }

        const_iterator cbegin() const noexcept {
            return iterator{ _sentinel->next };
        }

        iterator end() noexcept {
            return _sentinel;
        }

        const_iterator end() const noexcept {
            return iterator{ _sentinel };
        }

        const_iterator cend() const noexcept {
            return iterator{ _sentinel };
        }

        reverse_iterator rbegin() noexcept {
            return reverse_iterator{ { _sentinel } };
        }

        const_reverse_iterator rbegin() const noexcept {
            return reverse_iterator{ { _sentinel } };
        }

        const_reverse_iterator crbegin() const noexcept {
            return reverse_iterator{ { _sentinel } };
        }

        reverse_iterator rend() noexcept {
            return reverse_iterator{ { _sentinel->next } };
        }

        const_reverse_iterator rend() const noexcept {
            return reverse_iterator{ { _sentinel->next } };
        }

        const_reverse_iterator crend() const noexcept {
            return reverse_iterator{ { _sentinel->next } };
        }

        auto&& front(this auto&& self) noexcept {
            assert(!self.empty());
            return std::forward_like<decltype(self)>(self._sentinel->next->value);
        }

        auto&& back(this auto&& self) noexcept {
            assert(!self.empty());
            return std::forward_like<decltype(self)>(self._sentinel->prev->value);
        }

        void push_front(const T& value) noexcept {
            _sentinel->next->next->prev = _sentinel->next = new node{ value, _sentinel, _sentinel->next };
            ++_size;
        }

        void pop_front() noexcept {
            assert(!empty());
            erase(begin());
        }

        void push_back(const T& value) noexcept {
            _sentinel->prev->prev->next = _sentinel->prev = new node{ value, _sentinel->prev, _sentinel };
            ++_size;
        }

        void pop_back() noexcept {
            assert(!empty());
            erase(--end());
        }

        iterator insert(iterator pos, const T& value) noexcept {
            return insert(pos, 1, value);
        }

        iterator insert(iterator pos, std::size_t count, const T& value) noexcept {
            node* i{ pos._ptr->prev };
            _size += count;
            while (count-- != 0) {
                i = i->next = new node{ value, i, i->next };
            }
            i->next->prev = i;
            return pos;
        }

        template<std::input_iterator It>
        iterator insert(iterator pos, It first, It last) noexcept {
            node* i{ pos._ptr->prev };
            while (first != last) {
                i = i->next = new node{ *first++, i, i->next };
                ++_size;
            }
            i->next->prev = i;
            return pos;
        }

        iterator insert(iterator pos, std::initializer_list<T> list) noexcept {
            return insert(pos, list.begin(), list.end());
        }

        iterator erase(iterator pos) noexcept {
            node* i{ (++pos)._ptr };
            delete std::exchange(i->prev, i->prev->prev);
            --_size;
            i->prev->next = i;
            return pos;
        }

        iterator erase(iterator first, iterator last) noexcept {
            node* i{ first._ptr }, * j{ last._ptr };
            i->prev->next = j;
            j->prev = i->prev;
            while (i != j) {
                delete std::exchange(i, i->next);
                --_size;
            }
            return last;
        }

        friend bool operator==(const list& cont1, const list& cont2) noexcept {
            return std::equal(cont1.begin(), cont1.end(), cont2.begin(), cont2.end());
        }

        friend auto operator<=>(const list& cont1, const list& cont2) noexcept {
            return std::lexicographical_compare_three_way(cont1.begin(), cont1.end(), cont2.begin(), cont2.end());
        }
    };

    template<class It>
    explicit plastic::list(It, It)->plastic::list<std::iter_value_t<It>>;

    template<class T, class Cmp = std::less<T>>
    class binary_search_tree {
        static constexpr Cmp _cmp{};

        struct node {
            node* parent;
            node* left;
            node* right;
            T value;
            bool is_head;

            node* leftmost() noexcept {
                node* i{ this };
                while (!i->left->is_head) {
                    i = i->left;
                }
                return i;
            }

            node* rightmost() noexcept {
                node* i{ this };
                while (!i->right->is_head) {
                    i = i->right;
                }
                return i;
            }

            void free() noexcept {
                if (is_head) {
                    return;
                }
                free(left);
                free(right);
                delete this;
            }

            node* clone(node* head) noexcept {
                if (is_head) {
                    return head;
                }
                return new node{ this, left->clone(), right->clone(), value, false };
            }
        };

        enum class node_location : unsigned char {
            left,
            right
        };

        struct find_result {
            node* parent;
            node_location location;
            node* bound;
        };

        node* _head;
        std::size_t _size;

        find_result _upper_bound(const T& value) noexcept {
            find_result result{ _head->parent, node_location::right, _head };
            node* i{ result.parent };
            while (!i->is_head) {
                result.parent = i;
                if (!_cmp(i->value, value)) {
                    result.location = node_location::left;
                    result.bound = i;
                    i = i->left;
                }
                else {
                    result.location = node_location::right;
                    i = i->right;
                }
            }
            return result;
        }

        find_result _lower_bound(const T& value) noexcept {
            find_result result{ _head->parent, node_location::right, _head };
            node* i{ result.parent };
            while (!i->is_head) {
                result.parent = i;
                if (_cmp(value, i->value)) {
                    result.bound = i;
                    result.location = node_location::left;
                    i = i->left;
                }
                else {
                    result.location = node_location::right;
                    i = i->right;
                }
            }
            return result;
        }

    public:
        class iterator {
            friend class binary_search_tree;

            node* _ptr;

        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = const T*;
            using reference = const T&;
            using iterator_category = std::bidirectional_iterator_tag;

            iterator(node* ptr = {}) noexcept :
                _ptr{ ptr } {}

            reference operator*() const noexcept {
                return _ptr->value;
            }

            pointer operator->() const noexcept {
                return &_ptr->value;
            }

            friend bool operator==(iterator iter1, iterator iter2) noexcept {
                return iter1._ptr == iter2._ptr;
            }

            iterator& operator++() noexcept {
                if (_ptr->right->is_head) {
                    node* old_ptr;
                    do {
                        old_ptr = _ptr;
                        _ptr = _ptr->parent;
                    } while (!_ptr->is_head && _ptr->right == old_ptr);
                }
                else {
                    _ptr = _ptr->right->leftmost();
                }
                return *this;
            }

            iterator operator++(int) noexcept {
                iterator temp{ *this };
                ++*this;
                return temp;
            }

            iterator& operator--() noexcept {
                if (_ptr->left->is_head) {
                    node* old_ptr;
                    do {
                        old_ptr = _ptr;
                        _ptr = _ptr->parent;
                    } while (!_ptr->is_head && _ptr->left == old_ptr);
                }
                else {
                    _ptr = _ptr->left->rightmost();
                }
                return *this;
            }

            iterator operator--(int) noexcept {
                iterator temp{ *this };
                --*this;
                return temp;
            }
        };

        using const_iterator = iterator;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = reverse_iterator;

        explicit binary_search_tree() noexcept :
            _head{ new node },
            _size{} {

            _head->parent = _head->left = _head->right = _head;
            _head->is_head = true;
        }

        template<std::input_iterator It>
        explicit binary_search_tree(It first, It last) noexcept :
            binary_search_tree() {

            insert(first, last);
        }

        explicit binary_search_tree(std::initializer_list<T> list) noexcept :
            binary_search_tree(list.begin(), list.end()) {}

        explicit binary_search_tree(const binary_search_tree& other) noexcept :
            binary_search_tree() {

            if (other._size != 0) {
                _head->parent = other._head->clone(_head);
                _head->left = _head->parent->leftmost();
                _head->right = _head->parent->rightmost();
            }
        }

        ~binary_search_tree() noexcept {
            clear();
            delete _head;
        }

        binary_search_tree& operator=(const binary_search_tree& other) noexcept {
            if (this == &other) {
                return *this;
            }

            binary_search_tree temp{ other };
            std::swap(_head, other._head);
            std::swap(_size, other._size);

            return *this;
        }

        bool empty() const noexcept {
            return _size == 0;
        }

        std::size_t size() const noexcept {
            return _size;
        }

        void clear() noexcept {
            _head->parent->free();
        }

        const_iterator begin() const noexcept {
            return _head->right;
        }

        const_iterator end() const noexcept {
            return _head;
        }

        const_reverse_iterator rbegin() const noexcept {
            return _head;
        }

        const_reverse_iterator rend() const noexcept {
            return _head->right;
        }

        const T& front() const noexcept {
            assert(!empty());
            return _head->right->value;
        }

        const T& back() const noexcept {
            assert(!empty());
            return _head->left->value;
        }

        const_iterator lower_bound(const T& value) const noexcept {
            return _lower_bound(value).bound;
        }

        const_iterator upper_bound(const T& value) const noexcept {
            return _upper_bound(value).bound;
        }

        std::pair<const_iterator, const_iterator> equal_range(const T& value) const noexcept {
            return { lower_bound(value), upper_bound(value) };
        }

        const_iterator find(const T& value) const noexcept {
            node* ptr{ lower_bound(value) };
            return !ptr->is_head && !_cmp(*ptr, value) ? ptr : _head;
        }

        bool contains(const T& value) const noexcept {
            node* ptr{ lower_bound(value) };
            return !ptr->is_head && !_cmp(*ptr, value);
        }

        std::size_t count(const T& value) const noexcept {
            return std::distance(lower_bound(value), upper_bound(value));
        }

        iterator insert(const T& value) noexcept {
            auto [parent, location, _] { _head->lower_bound(value) };
            node* new_node{ new node{ parent, _head, _head, value, false } };
            if (location == node_location::left) {
                parent->left = new_node;
            }
            else {
                parent->right = new_node;
            }
            ++_size;
            return new_node;
        }

        template<std::input_iterator It>
        void insert(It first, It last) noexcept {
            while (first != last) {
                insert(*first++);
            }
        }

        void insert(std::initializer_list<T> list) noexcept {
            insert(list.begin(), list.end());
        }

        iterator erase(iterator pos) noexcept {
            node* new_node{ pos._ptr };
            if (new_node->left->is_head || new_node->right->is_head) {
                node* parent{ new_node->parent };
                node* erased{ new_node->left->is_head ? new_node->right : new_node->left };
                erased->parent = parent;
                if (parent->is_head) {
                    _head->parent = erased;
                }
                else if (parent->left == new_node) {
                    parent->left = erased;
                }
                else {
                    parent->right = erased;
                }
                if (_head->left == new_node) {
                    _head->left = erased->is_head ? parent : erased->leftmost();
                }
                if (_head->right == new_node) {
                    _head->right = erased->is_head ? parent : erased->rightmost();
                }
                --_size;
                return ++pos;
            }
        }

        iterator erase(iterator first, iterator last) noexcept {
            while (first != last) {
                erase(first++);
            }
            return last;
        }

        std::size_t erase(const T& value) noexcept {
            erase(lower_bound(value), upper_bound(value));
        }

        void merge(const binary_search_tree& other) noexcept {

        }
    };

    template<class It>
    explicit plastic::binary_search_tree(It, It)->plastic::binary_search_tree<std::iter_value_t<It>>;

    template<class T, class Cmp = std::less<T>>
    class binary_heap {
        static constexpr Cmp _cmp{};

        plastic::vector<T> _data;

        void _sift_up(std::size_t index) noexcept {
            T value{ std::move(_data[index]) };
            while (index != 0) {
                std::size_t parent{ (index - 1) / 2 };
                if (!_cmp(_data[parent], value)) {
                    break;
                }
                _data[index] = std::move(_data[parent]);
                index = parent;
            }
            _data[index] = std::move(value);
        }

        void _sift_down(std::size_t index) noexcept {
            T value{ std::move(_data[index]) };
            while (true) {
                std::size_t child{ index * 2 + 1 };
                if (child >= _data.size()) {
                    break;
                }
                if (child + 1 < _data.size() && _cmp(_data[child], _data[child + 1])) {
                    ++child;
                }
                if (!_cmp(value, _data[child])) {
                    break;
                }
                _data[index] = std::move(_data[child]);
                index = child;
            }
            _data[index] = std::move(value);
        }

        void _sift_up_down(std::size_t index) noexcept {
            _sift_up(index);
            _sift_down(index);
        }

        void _make_heap() noexcept {
            std::size_t i{ _data.size() / 2 };
            while (i-- != 0) {
                _sift_down(i);
            }
        }

    public:
        explicit binary_heap() noexcept = default;

        template<std::input_iterator It>
        explicit binary_heap(It first, It last) noexcept :
            _data(first, last) {

            _make_heap();
        }

        explicit binary_heap(std::initializer_list<T> list) noexcept :
            binary_heap(list.begin(), list.end()) {}

        bool empty() const noexcept {
            return _data.empty();
        }

        std::size_t size() const noexcept {
            return _data.size();
        }

        void clear() noexcept {
            return _data.clear();
        }

        const T& top() const noexcept {
            assert(!_data.empty());
            return _data.front();
        }

        // only for test purposes
        T* data() noexcept {
            return _data.data();
        }

        void push(const T& value) noexcept {
            _data.push_back(value);
            _sift_up(_data.size() - 1);
        }

        void pop() noexcept {
            assert(!_data.empty());
            _data.front() = std::move(_data.back());
            _data.pop_back();
            _sift_down(0);
        }

        void merge(const binary_heap& other) noexcept {
            _data.insert(_data.end(), other._data.begin(), other._data.end());
            _make_heap();
        }

        void assign(T* ptr, const T& value) noexcept {
            *ptr = value;
            _sift_up_down(ptr - _data.data());
        }

        void erase(T* ptr) noexcept {
            *ptr = std::move(_data.back());
            _data.pop_back();
            if (!_data.empty()) {
                _sift_up_down(ptr - _data.data());
            }
        }
    };

    template<class It>
    explicit plastic::binary_heap(It, It)->plastic::binary_heap<std::iter_value_t<It>>;

}