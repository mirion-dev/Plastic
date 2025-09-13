// ReSharper disable CppSpecialFunctionWithoutNoexceptSpecification

module;

#include <cassert>

export module plastic:linear_structure;

import std;

namespace plastic {

    struct uninitialized_t {} uninitialized;

    template <class It, class... Args>
    void construct(It first, It last, const Args&... args) {
        if constexpr (sizeof...(Args) == 0) {
            std::uninitialized_value_construct(first, last);
        }
        else {
            std::uninitialized_fill(first, last, args...);
        }
    }

    export template <class T, std::size_t N>
    class inplace_vector {
        T _data[N];
        T* _last{ _data };

        template <class... Args>
        void _resize(std::size_t new_size, const Args&... args) {
            T* new_last{ _data + new_size };
            if (new_size <= size()) {
                std::destroy(new_last, _last);
            }
            else {
                plastic::construct(_last, new_last, args...);
            }
            _last = new_last;
        }

    public:
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using iterator = pointer;
        using const_iterator = std::const_iterator<iterator>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        inplace_vector() = default;

        explicit inplace_vector(size_type size) {
            assert(size <= N);
            _last = _data + size;
            plastic::construct(_data, _last);
        }

        inplace_vector(size_type size, const_reference value) {
            assert(size <= N);
            _last = _data + size;
            plastic::construct(_data, _last, value);
        }

        template <std::input_iterator It>
        inplace_vector(It first, It last) :
            _last{ std::uninitialized_copy(first, last, _data) } {}

        inplace_vector(std::initializer_list<T> list) {
            assert(list.size() <= N);
            _last = std::uninitialized_copy(list.begin(), list.end(), _data);
        }

        inplace_vector(const inplace_vector& other) :
            inplace_vector(const_cast<T*>(other._data), other._last) {}

        inplace_vector(inplace_vector&& other) {
            swap(other);
        }

        inplace_vector& operator=(const inplace_vector& other) {
            inplace_vector temp(other);
            swap(temp);
            return *this;
        }

        inplace_vector& operator=(inplace_vector&& other) {
            swap(other);
            return *this;
        }

        void swap(inplace_vector& other) {
            std::swap(_data, other._data);

            std::ptrdiff_t offset{ _last - _data };
            _last = _data + (other._last - other._data);
            other._last = other._data + offset;
        }

        friend void swap(inplace_vector& left, inplace_vector& right) {
            left.swap(right);
        }

        bool empty() const {
            return _data == _last;
        }

        size_type size() const {
            return _last - _data;
        }

        static size_type max_size() {
            return N;
        }

        void clear() {
            std::destroy(_data, _last);
            _last = _data;
        }

        void resize(size_type new_size) {
            assert(new_size <= N);
            _resize(new_size);
        }

        void resize(size_type new_size, const_reference value) {
            assert(new_size <= N);
            _resize(new_size, value);
        }

        static size_type capacity() {
            return N;
        }

        static void reserve(size_type new_capacity) {
            assert(new_capacity <= N);
        }

        iterator begin() {
            return _data;
        }

        const_iterator begin() const {
            return const_cast<T*>(_data);
        }

        iterator end() {
            return _last;
        }

        const_iterator end() const {
            return _last;
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
            return _data[index];
        }

        const_reference operator[](size_type index) const {
            assert(index < size());
            return _data[index];
        }

        reference front() {
            assert(!empty());
            return *_data;
        }

        const_reference front() const {
            assert(!empty());
            return *_data;
        }

        reference back() {
            assert(!empty());
            return _last[-1];
        }

        const_reference back() const {
            assert(!empty());
            return _last[-1];
        }

        pointer data() {
            return _data;
        }

        const_pointer data() const {
            return _data;
        }

        void push_back(const_reference value) {
            assert(size() != N);
            std::construct_at(_last++, value);
        }

        void pop_back() {
            assert(!empty());
            std::destroy_at(--_last);
        }

        iterator insert(const_iterator pos, const_reference value) {
            return insert(pos, 1, value);
        }

        iterator insert(const_iterator pos, size_type count, const_reference value) {
            assert(size() + count <= N);
            T* i{ pos.base() };
            T* new_last{ _last + count };
            std::fill(i, std::move_backward(i, _last, new_last), value);
            _last = new_last;
            return i;
        }

        template <std::input_iterator It>
        iterator insert(const_iterator pos, It first, It last) {
            T* i{ pos.base() };
            T* new_last{ std::uninitialized_copy(first, last, _last) };
            std::rotate(i, _last, new_last);
            _last = new_last;
            return i;
        }

        iterator insert(const_iterator pos, std::initializer_list<T> list) {
            return insert(pos, list.begin(), list.end());
        }

        iterator erase(const_iterator pos) {
            T* i{ pos.base() };
            assert(i != _last);
            _last = std::move(i + 1, _last, i);
            std::destroy_at(_last);
            return i;
        }

        iterator erase(const_iterator first, const_iterator last) {
            T *i{ first.base() }, *s{ last.base() };
            T* new_last{ std::move(s, _last, i) };
            std::destroy(new_last, _last);
            _last = new_last;
            return i;
        }

        friend bool operator==(const inplace_vector& left, const inplace_vector& right) {
            return std::equal(left.begin(), left.end(), right.begin(), right.end());
        }

        friend auto operator<=>(const inplace_vector& left, const inplace_vector& right) {
            return std::lexicographical_compare_three_way(left.begin(), left.end(), right.begin(), right.end());
        }

    };

    export template <class T>
    class vector {
        T* _begin{};
        T* _last{};
        T* _end{};

        vector(uninitialized_t, std::size_t size) :
            _begin{ new T[size] },
            _last{ _begin + size },
            _end{ _last } {}

        void _reallocate(std::size_t new_capacity) {
            T* new_begin{ new T[new_capacity] };
            T* new_last{ std::uninitialized_move(_begin, _last, new_begin) };
            T* new_end{ new_begin + new_capacity };
            delete[] _begin;

            _begin = new_begin;
            _last = new_last;
            _end = new_end;
        }

        void _extend(std::size_t size) {
            _reallocate(capacity() + std::max(capacity() >> 1, size));
        }

        template <class... Args>
        void _resize(std::size_t new_size, const Args&... args) {
            if (new_size > capacity()) {
                _reallocate(new_size);
                plastic::construct(_last, _end, args...);
                _last = _end;
                return;
            }

            T* new_last{ _begin + new_size };
            if (new_size <= size()) {
                std::destroy(new_last, _last);
            }
            else {
                plastic::construct(_last, new_last, args...);
            }
            _last = new_last;
        }

    public:
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using iterator = pointer;
        using const_iterator = std::const_iterator<iterator>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        vector() = default;

        explicit vector(size_type size) :
            vector(uninitialized, size) {

            plastic::construct(_begin, _last);
        }

        vector(size_type size, const_reference value) :
            vector(uninitialized, size) {

            plastic::construct(_begin, _last, value);
        }

        template <std::input_iterator It>
        vector(It first, It last) {
            while (first != last) {
                push_back(*first);
                ++first;
            }
        }

        vector(std::initializer_list<T> list) :
            vector(list.begin(), list.end()) {}

        vector(const vector& other) :
            vector(other._begin, other._last) {}

        vector(vector&& other) {
            swap(other);
        }

        ~vector() {
            delete[] _begin;
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
            std::swap(_begin, other._begin);
            std::swap(_last, other._last);
            std::swap(_end, other._end);
        }

        friend void swap(vector& left, vector& right) {
            left.swap(right);
        }

        bool empty() const {
            return _begin == _last;
        }

        size_type size() const {
            return _last - _begin;
        }

        static size_type max_size() {
            return std::numeric_limits<size_type>::max();
        }

        void clear() {
            std::destroy(_begin, _last);
            _last = _begin;
        }

        void resize(size_type new_size) {
            _resize(new_size);
        }

        void resize(size_type new_size, const_reference value) {
            _resize(new_size, value);
        }

        size_type capacity() const {
            return _end - _begin;
        }

        void reserve(size_type new_capacity) {
            if (new_capacity > capacity()) {
                _reallocate(new_capacity);
            }
        }

        iterator begin() {
            return _begin;
        }

        const_iterator begin() const {
            return _begin;
        }

        iterator end() {
            return _last;
        }

        const_iterator end() const {
            return _last;
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
            return _begin[index];
        }

        const_reference operator[](size_type index) const {
            assert(index < size());
            return _begin[index];
        }

        reference front() {
            assert(!empty());
            return *_begin;
        }

        const_reference front() const {
            assert(!empty());
            return *_begin;
        }

        reference back() {
            assert(!empty());
            return _last[-1];
        }

        const_reference back() const {
            assert(!empty());
            return _last[-1];
        }

        pointer data() {
            return _begin;
        }

        const_pointer data() const {
            return _begin;
        }

        void push_back(const_reference value) {
            if (_last == _end) {
                _extend(1);
            }
            std::construct_at(_last++, value);
        }

        void pop_back() {
            assert(!empty());
            std::destroy_at(--_last);
        }

        iterator insert(const_iterator pos, const_reference value) {
            return insert(pos, 1, value);
        }

        iterator insert(const_iterator pos, size_type count, const_reference value) {
            T* i{ pos.base() };
            if (_end - _last < static_cast<std::ptrdiff_t>(count)) {
                std::ptrdiff_t offset{ i - _begin };
                _extend(count);
                i = _begin + offset;
            }

            std::fill(i, std::move_backward(i, _last, _last + count), value);
            _last += count;
            return i;
        }

        template <std::input_iterator It>
        iterator insert(const_iterator pos, It first, It last) {
            T* i{ pos.base() };
            std::ptrdiff_t offset1{ i - _begin }, offset2{ _last - _begin };
            while (first != last) {
                push_back(*first);
                ++first;
            }
            std::rotate(_begin + offset1, _begin + offset2, _last);
            return i;
        }

        iterator insert(const_iterator pos, std::initializer_list<T> list) {
            return insert(pos, list.begin(), list.end());
        }

        iterator erase(const_iterator pos) {
            T* i{ pos.base() };
            assert(i != _last);
            _last = std::move(i + 1, _last, i);
            std::destroy_at(_last);
            return i;
        }

        iterator erase(const_iterator first, const_iterator last) {
            T *i{ first.base() }, *s{ last.base() };
            T* new_last{ std::move(s, _last, i) };
            std::destroy(new_last, _last);
            _last = new_last;
            return i;
        }

        friend bool operator==(const vector& left, const vector& right) {
            return std::equal(left._begin, left._last, right._begin, right._last);
        }

        friend auto operator<=>(const vector& left, const vector& right) {
            return std::lexicographical_compare_three_way(left._begin, left._last, right._begin, right._last);
        }

    };

    template <class It>
    vector(It, It) -> vector<std::iter_value_t<It>>;

    export template <class T, std::size_t N>
    class inplace_deque {
        T _data[N + 1];
        T* _first{ _data };
        T* _last{ _data };

        template <class... Args>
        void _resize(std::size_t new_size, const Args&... args) {
            T* new_last{ (begin() + new_size)._ptr };
            if (new_size <= size()) {
                if (new_last <= _last) {
                    std::destroy(new_last, _last);
                }
                else {
                    std::destroy(new_last, _data + N + 1);
                    std::destroy(_data, _last);
                }
            }
            else {
                if (_last <= new_last) {
                    plastic::construct(_last, new_last, args...);
                }
                else {
                    plastic::construct(_last, _data + N + 1, args...);
                    plastic::construct(_data, new_last, args...);
                }
            }
            _last = new_last;
        }

    public:
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;

        class iterator {
            friend inplace_deque;

            T* _ptr{};
            inplace_deque* _cont{};

            iterator(T* ptr, inplace_deque* cont) :
                _ptr{ ptr },
                _cont{ cont } {}

            std::ptrdiff_t _offset() const {
                std::ptrdiff_t diff{ _cont->_first - _ptr };
                return diff >= 0 ? diff : diff + N + 1;
            }

        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using iterator_category = std::random_access_iterator_tag;

            iterator() = default;

            reference operator*() const {
                return *_ptr;
            }

            pointer operator->() const {
                return _ptr;
            }

            reference operator[](difference_type index) const {
                return *(*this + index);
            }

            friend bool operator==(iterator left, iterator right) {
                return left._ptr == right._ptr;
            }

            friend auto operator<=>(iterator left, iterator right) {
                return left._offset() <=> right._offset();
            }

            iterator& operator+=(difference_type diff) {
                if (_cont->_data + N - _ptr < diff) {
                    _ptr += diff - N - 1;
                }
                else if (_ptr - _cont->_data < -diff) {
                    _ptr += diff + N + 1;
                }
                else {
                    _ptr += diff;
                }
                return *this;
            }

            iterator& operator-=(difference_type diff) {
                return *this += -diff;
            }

            friend iterator operator+(iterator iter, difference_type diff) {
                return iter += diff;
            }

            friend iterator operator+(difference_type diff, iterator iter) {
                return iter += diff;
            }

            friend iterator operator-(iterator iter, difference_type diff) {
                return iter -= diff;
            }

            friend difference_type operator-(iterator left, iterator right) {
                return left._offset() - right._offset();
            }

            iterator& operator++() {
                if (_ptr == _cont->_data + N) {
                    _ptr = _cont->_data;
                }
                else {
                    ++_ptr;
                }
                return *this;
            }

            iterator operator++(int) {
                iterator temp{ *this };
                ++*this;
                return temp;
            }

            iterator& operator--() {
                if (_ptr == _cont->_data) {
                    _ptr = _cont->_data + N;
                }
                else {
                    --_ptr;
                }
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

        inplace_deque() = default;

        explicit inplace_deque(size_type size) {
            assert(size <= N);
            _last = _data + size;
            plastic::construct(_data, _last);
        }

        inplace_deque(size_type size, const_reference value) {
            assert(size <= N);
            _last = _data + size;
            plastic::construct(_data, _last, value);
        }

        template <std::input_iterator It>
        inplace_deque(It first, It last) :
            _last{ std::uninitialized_copy(first, last, _data) } {}

        inplace_deque(std::initializer_list<T> list) {
            assert(list.size() <= N);
            _last = std::uninitialized_copy(list.begin(), list.end(), _data);
        }

        inplace_deque(const inplace_deque& other) {
            if (other._first <= other._last) {
                _last = std::uninitialized_copy(other._first, other._last, _data);
            }
            else {
                _last = std::uninitialized_copy<const T*>(
                    other._data,
                    other._last,
                    std::uninitialized_copy<const T*>(
                        other._first,
                        other._data + N + 1,
                        _data
                    )
                );
            }
        }

        inplace_deque(inplace_deque&& other) {
            swap(other);
        }

        inplace_deque& operator=(const inplace_deque& other) {
            inplace_deque temp(other);
            swap(temp);
            return *this;
        }

        inplace_deque& operator=(inplace_deque&& other) {
            swap(other);
            return *this;
        }

        void swap(inplace_deque& other) {
            std::swap(_data, other._data);

            std::ptrdiff_t first_offset{ _first - _data }, last_offset{ _last - _data };
            _first = _data + (other._first - other._data);
            _last = _data + (other._last - other._data);
            other._first = other._data + first_offset;
            other._last = other._data + last_offset;
        }

        friend void swap(inplace_deque& left, inplace_deque& right) {
            left.swap(right);
        }

        bool empty() const {
            return _first == _last;
        }

        size_type size() const {
            std::ptrdiff_t diff{ _last - _first };
            return diff >= 0 ? diff : diff + N + 1;
        }

        static size_type max_size() {
            return N;
        }

        void clear() {
            if (_first <= _last) {
                std::destroy(_first, _last);
            }
            else {
                std::destroy(_first, _data + N + 1);
                std::destroy(_data, _last);
            }
            _first = _last = _data;
        }

        void resize(size_type new_size) {
            assert(new_size <= N);
            _resize(new_size);
        }

        void resize(size_type new_size, const_reference value) {
            assert(new_size <= N);
            _resize(new_size, value);
        }

        static size_type capacity() {
            return N;
        }

        static void reserve(size_type new_capacity) {
            assert(new_capacity <= N);
        }

        iterator begin() {
            return { _first, this };
        }

        const_iterator begin() const {
            return iterator{ _first, const_cast<inplace_deque*>(this) };
        }

        iterator end() {
            return { _last, this };
        }

        const_iterator end() const {
            return iterator{ _last, const_cast<inplace_deque*>(this) };
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
            return begin()[index];
        }

        const_reference operator[](size_type index) const {
            assert(index < size());
            return begin()[index];
        }

        reference front() {
            assert(!empty());
            return *_first;
        }

        const_reference front() const {
            assert(!empty());
            return *_first;
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
            assert(size() != N);
            if (_first == _data) {
                _first = _data + N;
                std::construct_at(_first, value);
            }
            else {
                std::construct_at(--_first, value);
            }
        }

        void pop_front() {
            assert(!empty());
            if (_first == _data + N) {
                std::destroy_at(_first);
                _first = _data;
            }
            else {
                std::destroy_at(_first++);
            }
        }

        void push_back(const_reference value) {
            assert(size() != N);
            if (_last == _data + N) {
                std::construct_at(_last, value);
                _last = _data;
            }
            else {
                std::construct_at(_last++, value);
            }
        }

        void pop_back() {
            assert(!empty());
            if (_last == _data) {
                _last = _data + N;
                std::destroy_at(_last);
            }
            else {
                std::destroy_at(--_last);
            }
        }

        iterator insert(const_iterator pos, const_reference value) {
            return insert(pos, 1, value);
        }

        iterator insert(const_iterator pos, size_type count, const_reference value) {
            assert(size() + count <= N);
            iterator i{ pos.base() };
            iterator new_last{ end() + count };
            std::fill(i, std::move_backward(i, end(), new_last), value);
            _last = new_last._ptr;
            return i;
        }

        template <std::input_iterator It>
        iterator insert(const_iterator pos, It first, It last) {
            iterator i{ pos.base() };
            iterator new_last{ std::uninitialized_copy(first, last, end()) };
            std::rotate(i, end(), new_last);
            _last = new_last._ptr;
            return i;
        }

        iterator insert(const_iterator pos, std::initializer_list<T> list) {
            return insert(pos, list.begin(), list.end());
        }

        iterator erase(const_iterator pos) {
            iterator i{ pos.base() };
            assert(i != end());
            _last = std::move(i + 1, end(), i)._ptr;
            std::destroy_at(_last);
            return i;
        }

        iterator erase(const_iterator first, const_iterator last) {
            iterator i{ first.base() }, s{ last.base() };
            iterator new_last{ std::move(s, end(), i) };
            std::destroy(new_last, end());
            _last = new_last._ptr;
            return i;
        }

        friend bool operator==(const inplace_deque& left, const inplace_deque& right) {
            return std::equal(left.begin(), left.end(), right.begin(), right.end());
        }

        friend auto operator<=>(const inplace_deque& left, const inplace_deque& right) {
            return std::lexicographical_compare_three_way(left.begin(), left.end(), right.begin(), right.end());
        }
    };

    export template <class T>
    class deque {
        T* _begin{};
        T* _end{};
        T* _first{};
        T* _last{};

        deque(uninitialized_t, std::size_t size) :
            _begin{ new T[size] },
            _end{ _begin + size },
            _first{ _begin },
            _last{ _end } {}

        void _reallocate(std::size_t new_capacity, std::ptrdiff_t offset) {
            T* new_begin{ new T[new_capacity] };
            T* new_end{ new_begin + new_capacity };
            T* new_first{ new_begin + offset };
            T* new_last{ std::uninitialized_move(_first, _last, new_first) };
            delete[] _begin;

            _begin = new_begin;
            _end = new_end;
            _first = new_first;
            _last = new_last;
        }

        void _extend(std::size_t size, std::ptrdiff_t offset) {
            std::size_t new_capacity{ capacity() + std::max(capacity(), size) };
            std::size_t new_size{ this->size() + size };
            _reallocate(new_capacity, (new_capacity - new_size >> 1) + offset);
        }

        template <class... Args>
        void _resize(std::size_t new_size, const Args&... args) {
            if (new_size > capacity()) {
                _reallocate(new_size, 0);
                plastic::construct(_last, _end, args...);
                _last = _end;
                return;
            }

            T* new_first{ _begin + (capacity() - new_size >> 1) };
            T* new_last{ new_first + new_size };
            if (new_size <= size()) {
                T* middle{ _first + new_size };
                if (_first <= new_first && _last <= new_first || _first > new_last && _last > new_last) {
                    std::uninitialized_move(_first, middle, new_first);
                    std::destroy(_first, _last);
                }
                else if (_last <= new_last) {
                    std::move_backward(_first, middle, new_last);
                    std::destroy(_first, new_first);
                }
                else if (_first > new_first) {
                    std::move(_first, middle, new_first);
                    std::destroy(new_last, _last);
                }
                else {
                    std::move(_first, middle, new_first);
                    std::destroy(_first, new_first);
                    std::destroy(new_last, _last);
                }
            }
            else {
                T* new_middle{ new_first + size() };
                if (_first <= new_first && _last <= new_first || _first > new_middle && _last > new_middle) {
                    std::uninitialized_move(_first, _last, new_first);
                    std::destroy(new_first, new_middle);
                }
                else if (_first <= new_first) {
                    std::move_backward(_first, _last, new_middle);
                    std::destroy(_first, new_first);
                }
                else {
                    std::move(_first, _last, new_first);
                    std::destroy(new_middle, _last);
                }
                plastic::construct(new_middle, new_last, args...);
            }
            _first = new_first;
            _last = new_last;
        }

    public:
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using iterator = T*;
        using const_iterator = std::const_iterator<iterator>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        deque() = default;

        explicit deque(size_type size) :
            deque(uninitialized, size) {

            plastic::construct(_first, _last);
        }

        deque(size_type size, const_reference value) :
            deque(uninitialized, size) {

            plastic::construct(_first, _last, value);
        }

        template <std::input_iterator It>
        deque(It first, It last) {
            while (first != last) {
                push_back(*first);
                ++first;
            }
        }

        deque(std::initializer_list<T> list) :
            deque(list.begin(), list.end()) {}

        deque(const deque& other) :
            deque(other._first, other._last) {}

        deque(deque&& other) {
            swap(other);
        }

        ~deque() {
            delete[] _begin;
        }

        deque& operator=(const deque& other) {
            deque temp{ other };
            swap(temp);
            return *this;
        }

        deque& operator=(deque&& other) {
            swap(other);
            return *this;
        }

        void swap(deque& other) {
            std::swap(_begin, other._begin);
            std::swap(_end, other._end);
            std::swap(_first, other._first);
            std::swap(_last, other._last);
        }

        friend void swap(deque& left, deque& right) {
            left.swap(right);
        }

        bool empty() const {
            return _first == _last;
        }

        size_type size() const {
            return _last - _first;
        }

        static size_type max_size() {
            return std::numeric_limits<size_type>::max();
        }

        void clear() {
            std::destroy(_first, _last);
            _first = _last = _begin + (size() >> 1);
        }

        void resize(size_type new_size) {
            _resize(new_size);
        }

        void resize(size_type new_size, const_reference value) {
            _resize(new_size, value);
        }

        size_type capacity() const {
            return _end - _begin;
        }

        void reserve(size_type new_capacity) {
            if (new_capacity > capacity()) {
                _reallocate(new_capacity, capacity() - size() >> 1);
            }
        }

        iterator begin() {
            return _first;
        }

        const_iterator begin() const {
            return _first;
        }

        iterator end() {
            return _last;
        }

        const_iterator end() const {
            return _last;
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
            return _first[index];
        }

        const_reference operator[](size_type index) const {
            assert(index < size());
            return _first[index];
        }

        reference front() {
            assert(!empty());
            return *_first;
        }

        const_reference front() const {
            assert(!empty());
            return *_first;
        }

        reference back() {
            assert(!empty());
            return _last[-1];
        }

        const_reference back() const {
            assert(!empty());
            return _last[-1];
        }

        pointer data() {
            return _first;
        }

        const_pointer data() const {
            return _first;
        }

        void push_front(const_reference value) {
            if (_first == _begin) {
                _extend(1, 1);
            }
            std::construct_at(--_first, value);
        }

        void pop_front() {
            assert(!empty());
            std::destroy_at(_first++);
        }

        void push_back(const_reference value) {
            if (_last == _end) {
                _extend(1, 0);
            }
            std::construct_at(_last++, value);
        }

        void pop_back() {
            assert(!empty());
            std::destroy_at(--_last);
        }

        iterator insert(const_iterator pos, const_reference value) {
            return insert(pos, 1, value);
        }

        iterator insert(const_iterator pos, size_type count, const_reference value) {
            T* i{ pos.base() };
            if (_end - _last < static_cast<std::ptrdiff_t>(count)) {
                std::ptrdiff_t offset{ i - _begin };
                _extend(count, 0);
                i = _begin + offset;
            }

            std::fill(i, std::move_backward(i, _last, _last + count), value);
            _last += count;
            return i;
        }

        template <std::input_iterator It>
        iterator insert(const_iterator pos, It first, It last) {
            T* i{ pos.base() };
            std::ptrdiff_t offset1{ i - _first }, offset2{ _last - _first };
            while (first != last) {
                push_back(*first);
                ++first;
            }
            std::rotate(_first + offset1, _first + offset2, _last);
            return i;
        }

        iterator insert(const_iterator pos, std::initializer_list<T> list) {
            return insert(pos, list.begin(), list.end());
        }

        iterator erase(const_iterator pos) {
            T* i{ pos.base() };
            assert(i != _last);
            _last = std::move(i + 1, _last, i);
            std::destroy_at(_last);
            return i;
        }

        iterator erase(const_iterator first, const_iterator last) {
            T *i{ first.base() }, *s{ last.base() };
            T* new_last{ std::move(s, _last, i) };
            std::destroy(new_last, _last);
            _last = new_last;
            return i;
        }

        friend bool operator==(const deque& left, const deque& right) {
            return std::equal(left.begin(), left.end(), right.begin(), right.end());
        }

        friend auto operator<=>(const deque& left, const deque& right) {
            return std::lexicographical_compare_three_way(left.begin(), left.end(), right.begin(), right.end());
        }
    };

    template <class It>
    deque(It, It) -> deque<std::iter_value_t<It>>;

    export template <class T>
    class forward_list {
        struct node {
            node* next;
            T value;
        };

        node* _head;
        std::size_t _size;

        template <class... Args>
        node* _insert_after(node* pos, std::size_t count, const Args&... args) {
            _size += count;
            while (count-- != 0) {
                pos = pos->next = new node{ pos->next, args... };
            }
            return pos;
        }

        template <class... Args>
        void _resize(std::size_t new_size, const Args&... args) {
            if (new_size <= _size) {
                erase_after(std::next(end(), new_size), end());
            }
            else {
                _insert_after(std::next(end(), _size)._ptr, new_size - _size, args...);
            }
        }

    public:
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;

        class iterator {
            friend forward_list;

            node* _ptr{};

            iterator(node* ptr) :
                _ptr{ ptr } {}

        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using iterator_category = std::forward_iterator_tag;

            iterator() = default;

            reference operator*() const {
                return _ptr->value;
            }

            pointer operator->() const {
                return &_ptr->value;
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

        forward_list() :
            _head{ new node },
            _size{} {

            _head->next = _head;
        }

        explicit forward_list(size_type size) :
            forward_list() {

            _insert_after(_head, size);
        }

        forward_list(size_type size, const_reference value) :
            forward_list() {

            insert_after(end(), size, value);
        }

        template <std::input_iterator It>
        forward_list(It first, It last) :
            forward_list() {

            insert_after(end(), first, last);
        }

        forward_list(std::initializer_list<T> list) :
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
            std::swap(_head, other._head);
            std::swap(_size, other._size);
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
            erase_after(end(), end());
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

        reference front() {
            assert(!empty());
            return _head->next->value;
        }

        const_reference front() const {
            assert(!empty());
            return _head->next->value;
        }

        void push_front(const_reference value) {
            _head->next = new node{ _head->next, value };
            ++_size;
        }

        void pop_front() {
            assert(!empty());
            erase_after(end());
        }

        iterator insert_after(const_iterator pos, const_reference value) {
            return insert_after(pos, 1, value);
        }

        iterator insert_after(const_iterator pos, size_type count, const_reference value) {
            return _insert_after(pos.base()._ptr, count, value);
        }

        template <std::input_iterator It>
        iterator insert_after(const_iterator pos, It first, It last) {
            node* i{ pos.base()._ptr };
            while (first != last) {
                i = i->next = new node{ i->next, *first };
                ++first;
                ++_size;
            }
            return i;
        }

        iterator insert_after(const_iterator pos, std::initializer_list<T> list) {
            return insert_after(pos, list.begin(), list.end());
        }

        iterator erase_after(const_iterator pos) {
            node* i{ pos.base()._ptr };
            delete std::exchange(i->next, i->next->next);
            --_size;
            return i->next;
        }

        iterator erase_after(const_iterator first, const_iterator last) {
            node *i{ first.base()._ptr }, *j{ last.base()._ptr };
            i = std::exchange(i->next, j);
            while (i != j) {
                delete std::exchange(i, i->next);
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
        struct node {
            node* prev;
            node* next;
            T value;
        };

        node* _head;
        std::size_t _size;

        template <class... Args>
        node* _insert(node* pos, std::size_t count, const Args&... args) {
            node *prev{ pos->prev }, *i{ prev };
            _size += count;
            while (count-- != 0) {
                i = i->next = new node{ i, i->next, args... };
            }
            i->next->prev = i;
            return prev->next;
        }

        template <class... Args>
        void _resize(std::size_t new_size, const Args&... args) {
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
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;

        class iterator {
            friend list;

            node* _ptr{};

            iterator(node* ptr) :
                _ptr{ ptr } {}

        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using iterator_category = std::bidirectional_iterator_tag;

            iterator() = default;

            reference operator*() const {
                return _ptr->value;
            }

            pointer operator->() const {
                return &_ptr->value;
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

        list() :
            _head{ new node },
            _size{} {

            _head->prev = _head->next = _head;
        }

        explicit list(size_type size) :
            list() {

            _insert(_head, size);
        }

        list(size_type size, const T& value) :
            list() {

            insert(end(), size, value);
        }

        template <std::input_iterator It>
        list(It first, It last) :
            list() {

            insert(end(), first, last);
        }

        list(std::initializer_list<T> list) :
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
            std::swap(_head, other._head);
            std::swap(_size, other._size);
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
            return _head->next->value;
        }

        const_reference front() const {
            assert(!empty());
            return _head->next->value;
        }

        reference back() {
            assert(!empty());
            return _head->prev->value;
        }

        const_reference back() const {
            assert(!empty());
            return _head->prev->value;
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
            node *prev{ pos.base()._ptr->prev }, *i{ prev };
            while (first != last) {
                i = i->next = new node{ i, i->next, *first };
                ++first;
                ++_size;
            }
            i->next->prev = i;
            return prev->next;
        }

        iterator insert(const_iterator pos, std::initializer_list<T> list) {
            return insert(pos, list.begin(), list.end());
        }

        iterator erase(const_iterator pos) {
            node* i{ pos.base()._ptr->prev };
            delete std::exchange(i->next, i->next->next);
            --_size;
            i->next->prev = i;
            return i->next;
        }

        iterator erase(const_iterator first, const_iterator last) {
            node *i{ first.base()._ptr }, *j{ last.base()._ptr };
            i->prev->next = j;
            j->prev = i->prev;
            while (i != j) {
                delete std::exchange(i, i->next);
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
