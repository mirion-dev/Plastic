module;

#include <cassert>

export module plastic:data_structure;

import std;

// for internal implementation
namespace plastic {

    struct uninitialized_t {} uninitialized;

    template <class It, class... Args>
        requires (sizeof...(Args) <= 1)
    void construct(It first, It last, const Args&... args) {
        if constexpr (sizeof...(Args) == 0) {
            std::uninitialized_value_construct(first, last);
        }
        else {
            std::uninitialized_fill(first, last, args...);
        }
    }

}

// linear structures
namespace plastic {

    export template <class T, std::size_t N>
    class inplace_vector {
        T _data[N];
        T* _last{ _data };

        template <class... Args>
        void _resize(std::size_t new_size, const Args&... args) noexcept {
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

        inplace_vector() noexcept = default;

        explicit inplace_vector(size_type size) noexcept {
            assert(size <= N);
            _last = _data + size;
            plastic::construct(_data, _last);
        }

        inplace_vector(size_type size, const_reference value) noexcept {
            assert(size <= N);
            _last = _data + size;
            plastic::construct(_data, _last, value);
        }

        template <std::input_iterator It>
        inplace_vector(It first, It last) noexcept :
            _last{ std::uninitialized_copy(first, last, _data) } {}

        inplace_vector(std::initializer_list<T> list) noexcept {
            assert(list.size() <= N);
            _last = std::uninitialized_copy(list.begin(), list.end(), _data);
        }

        inplace_vector(const inplace_vector& other) noexcept :
            inplace_vector(const_cast<T*>(other._data), other._last) {}

        inplace_vector(inplace_vector&& other) noexcept {
            swap(other);
        }

        inplace_vector& operator=(const inplace_vector& other) noexcept {
            inplace_vector temp(other);
            swap(temp);
            return *this;
        }

        inplace_vector& operator=(inplace_vector&& other) noexcept {
            swap(other);
            return *this;
        }

        void swap(inplace_vector& other) noexcept {
            std::swap(_data, other._data);

            std::ptrdiff_t offset{ _last - _data };
            _last = _data + (other._last - other._data);
            other._last = other._data + offset;
        }

        friend void swap(inplace_vector& left, inplace_vector& right) noexcept {
            left.swap(right);
        }

        bool empty() const noexcept {
            return _data == _last;
        }

        size_type size() const noexcept {
            return _last - _data;
        }

        static size_type max_size() noexcept {
            return N;
        }

        void clear() noexcept {
            std::destroy(_data, _last);
            _last = _data;
        }

        void resize(size_type new_size) noexcept {
            assert(new_size <= N);
            _resize(new_size);
        }

        void resize(size_type new_size, const_reference value) noexcept {
            assert(new_size <= N);
            _resize(new_size, value);
        }

        static size_type capacity() noexcept {
            return N;
        }

        static void reserve(size_type new_capacity) noexcept {
            assert(new_capacity <= N);
        }

        iterator begin() noexcept {
            return _data;
        }

        const_iterator begin() const noexcept {
            return const_cast<T*>(_data);
        }

        iterator end() noexcept {
            return _last;
        }

        const_iterator end() const noexcept {
            return _last;
        }

        const_iterator cbegin() const noexcept {
            return begin();
        }

        const_iterator cend() const noexcept {
            return end();
        }

        reverse_iterator rbegin() noexcept {
            return reverse_iterator{ end() };
        }

        const_reverse_iterator rbegin() const noexcept {
            return const_reverse_iterator{ end() };
        }

        reverse_iterator rend() noexcept {
            return reverse_iterator{ begin() };
        }

        const_reverse_iterator rend() const noexcept {
            return const_reverse_iterator{ begin() };
        }

        const_reverse_iterator crbegin() const noexcept {
            return rbegin();
        }

        const_reverse_iterator crend() const noexcept {
            return rend();
        }

        reference operator[](size_type index) noexcept {
            assert(index < size());
            return _data[index];
        }

        const_reference operator[](size_type index) const noexcept {
            assert(index < size());
            return _data[index];
        }

        reference front() noexcept {
            assert(!empty());
            return *_data;
        }

        const_reference front() const noexcept {
            assert(!empty());
            return *_data;
        }

        reference back() noexcept {
            assert(!empty());
            return _last[-1];
        }

        const_reference back() const noexcept {
            assert(!empty());
            return _last[-1];
        }

        pointer data() noexcept {
            return _data;
        }

        const_pointer data() const noexcept {
            return _data;
        }

        void push_back(const_reference value) noexcept {
            assert(size() != N);
            std::construct_at(_last++, value);
        }

        void pop_back() noexcept {
            assert(!empty());
            std::destroy_at(--_last);
        }

        iterator insert(const_iterator pos, const_reference value) noexcept {
            return insert(pos, 1, value);
        }

        iterator insert(const_iterator pos, size_type count, const_reference value) noexcept {
            assert(size() + count <= N);
            T* i{ pos.base() };
            T* new_last{ _last + count };
            std::fill(i, std::move_backward(i, _last, new_last), value);
            _last = new_last;
            return i;
        }

        template <std::input_iterator It>
        iterator insert(const_iterator pos, It first, It last) noexcept {
            T* i{ pos.base() };
            T* new_last{ std::uninitialized_copy(first, last, _last) };
            std::rotate(i, _last, new_last);
            _last = new_last;
            return i;
        }

        iterator insert(const_iterator pos, std::initializer_list<T> list) noexcept {
            return insert(pos, list.begin(), list.end());
        }

        iterator erase(const_iterator pos) noexcept {
            T* i{ pos.base() };
            assert(i != _last);
            _last = std::move(i + 1, _last, i);
            std::destroy_at(_last);
            return i;
        }

        iterator erase(const_iterator first, const_iterator last) noexcept {
            T *i{ first.base() }, *s{ last.base() };
            T* new_last{ std::move(s, _last, i) };
            std::destroy(new_last, _last);
            _last = new_last;
            return i;
        }

        friend bool operator==(const inplace_vector& left, const inplace_vector& right) noexcept {
            return std::equal(left.begin(), left.end(), right.begin(), right.end());
        }

        friend auto operator<=>(const inplace_vector& left, const inplace_vector& right) noexcept {
            return std::lexicographical_compare_three_way(left.begin(), left.end(), right.begin(), right.end());
        }

    };

    export template <class T>
    class vector {
        T* _begin{};
        T* _last{};
        T* _end{};

        vector(uninitialized_t, std::size_t size) noexcept :
            _begin{ new T[size] },
            _last{ _begin + size },
            _end{ _last } {}

        void _reallocate(std::size_t new_capacity) noexcept {
            T* new_begin{ new T[new_capacity] };
            T* new_last{ std::uninitialized_move(_begin, _last, new_begin) };
            T* new_end{ new_begin + new_capacity };
            delete[] _begin;

            _begin = new_begin;
            _last = new_last;
            _end = new_end;
        }

        void _extend(std::size_t size) noexcept {
            _reallocate(capacity() + std::max(capacity() >> 1, size));
        }

        template <class... Args>
        void _resize(std::size_t new_size, const Args&... args) noexcept {
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

        vector() noexcept = default;

        explicit vector(size_type size) noexcept :
            vector(uninitialized, size) {

            plastic::construct(_begin, _last);
        }

        vector(size_type size, const_reference value) noexcept :
            vector(uninitialized, size) {

            plastic::construct(_begin, _last, value);
        }

        template <std::input_iterator It>
        vector(It first, It last) noexcept {
            while (first != last) {
                push_back(*first);
                ++first;
            }
        }

        vector(std::initializer_list<T> list) noexcept :
            vector(list.begin(), list.end()) {}

        vector(const vector& other) noexcept :
            vector(other._begin, other._last) {}

        vector(vector&& other) noexcept {
            swap(other);
        }

        ~vector() noexcept {
            delete[] _begin;
        }

        vector& operator=(const vector& other) noexcept {
            vector temp(other);
            swap(temp);
            return *this;
        }

        vector& operator=(vector&& other) noexcept {
            swap(other);
            return *this;
        }

        void swap(vector& other) noexcept {
            std::swap(_begin, other._begin);
            std::swap(_last, other._last);
            std::swap(_end, other._end);
        }

        friend void swap(vector& left, vector& right) noexcept {
            left.swap(right);
        }

        bool empty() const noexcept {
            return _begin == _last;
        }

        size_type size() const noexcept {
            return _last - _begin;
        }

        static size_type max_size() noexcept {
            return std::numeric_limits<size_type>::max();
        }

        void clear() noexcept {
            std::destroy(_begin, _last);
            _last = _begin;
        }

        void resize(size_type new_size) noexcept {
            _resize(new_size);
        }

        void resize(size_type new_size, const_reference value) noexcept {
            _resize(new_size, value);
        }

        size_type capacity() const noexcept {
            return _end - _begin;
        }

        void reserve(size_type new_capacity) noexcept {
            if (new_capacity > capacity()) {
                _reallocate(new_capacity);
            }
        }

        iterator begin() noexcept {
            return _begin;
        }

        const_iterator begin() const noexcept {
            return _begin;
        }

        iterator end() noexcept {
            return _last;
        }

        const_iterator end() const noexcept {
            return _last;
        }

        const_iterator cbegin() const noexcept {
            return begin();
        }

        const_iterator cend() const noexcept {
            return end();
        }

        reverse_iterator rbegin() noexcept {
            return reverse_iterator{ end() };
        }

        const_reverse_iterator rbegin() const noexcept {
            return const_reverse_iterator{ end() };
        }

        reverse_iterator rend() noexcept {
            return reverse_iterator{ begin() };
        }

        const_reverse_iterator rend() const noexcept {
            return const_reverse_iterator{ begin() };
        }

        const_reverse_iterator crbegin() const noexcept {
            return rbegin();
        }

        const_reverse_iterator crend() const noexcept {
            return rend();
        }

        reference operator[](size_type index) noexcept {
            assert(index < size());
            return _begin[index];
        }

        const_reference operator[](size_type index) const noexcept {
            assert(index < size());
            return _begin[index];
        }

        reference front() noexcept {
            assert(!empty());
            return *_begin;
        }

        const_reference front() const noexcept {
            assert(!empty());
            return *_begin;
        }

        reference back() noexcept {
            assert(!empty());
            return _last[-1];
        }

        const_reference back() const noexcept {
            assert(!empty());
            return _last[-1];
        }

        pointer data() noexcept {
            return _begin;
        }

        const_pointer data() const noexcept {
            return _begin;
        }

        void push_back(const_reference value) noexcept {
            if (_last == _end) {
                _extend(1);
            }
            std::construct_at(_last++, value);
        }

        void pop_back() noexcept {
            assert(!empty());
            std::destroy_at(--_last);
        }

        iterator insert(const_iterator pos, const_reference value) noexcept {
            return insert(pos, 1, value);
        }

        iterator insert(const_iterator pos, size_type count, const_reference value) noexcept {
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
        iterator insert(const_iterator pos, It first, It last) noexcept {
            T* i{ pos.base() };
            std::ptrdiff_t offset1{ i - _begin }, offset2{ _last - _begin };
            while (first != last) {
                push_back(*first);
                ++first;
            }
            std::rotate(_begin + offset1, _begin + offset2, _last);
            return i;
        }

        iterator insert(const_iterator pos, std::initializer_list<T> list) noexcept {
            return insert(pos, list.begin(), list.end());
        }

        iterator erase(const_iterator pos) noexcept {
            T* i{ pos.base() };
            assert(i != _last);
            _last = std::move(i + 1, _last, i);
            std::destroy_at(_last);
            return i;
        }

        iterator erase(const_iterator first, const_iterator last) noexcept {
            T *i{ first.base() }, *s{ last.base() };
            T* new_last{ std::move(s, _last, i) };
            std::destroy(new_last, _last);
            _last = new_last;
            return i;
        }

        friend bool operator==(const vector& left, const vector& right) noexcept {
            return std::equal(left._begin, left._last, right._begin, right._last);
        }

        friend auto operator<=>(const vector& left, const vector& right) noexcept {
            return std::lexicographical_compare_three_way(left._begin, left._last, right._begin, right._last);
        }

    };

    template <class It>
    explicit vector(It, It) -> vector<std::iter_value_t<It>>;

    export template <class T, std::size_t N>
    class inplace_deque {
        T _data[N + 1];
        T* _first{ _data };
        T* _last{ _data };

        template <class... Args>
        void _resize(std::size_t new_size, const Args&... args) noexcept {
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

            iterator(T* ptr, inplace_deque* cont) noexcept :
                _ptr{ ptr },
                _cont{ cont } {}

            std::ptrdiff_t _offset() const noexcept {
                std::ptrdiff_t diff{ _cont->_first - _ptr };
                return diff >= 0 ? diff : diff + N + 1;
            }

        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using iterator_category = std::random_access_iterator_tag;

            iterator() noexcept = default;

            reference operator*() const noexcept {
                return *_ptr;
            }

            pointer operator->() const noexcept {
                return _ptr;
            }

            reference operator[](difference_type index) const noexcept {
                return *(*this + index);
            }

            friend bool operator==(iterator left, iterator right) noexcept {
                return left._ptr == right._ptr;
            }

            friend auto operator<=>(iterator left, iterator right) noexcept {
                return left._offset() <=> right._offset();
            }

            iterator& operator+=(difference_type diff) noexcept {
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

            friend difference_type operator-(iterator left, iterator right) noexcept {
                return left._offset() - right._offset();
            }

            iterator& operator++() noexcept {
                if (_ptr == _cont->_data + N) {
                    _ptr = _cont->_data;
                }
                else {
                    ++_ptr;
                }
                return *this;
            }

            iterator operator++(int) noexcept {
                iterator temp{ *this };
                ++*this;
                return temp;
            }

            iterator& operator--() noexcept {
                if (_ptr == _cont->_data) {
                    _ptr = _cont->_data + N;
                }
                else {
                    --_ptr;
                }
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
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        inplace_deque() noexcept = default;

        explicit inplace_deque(size_type size) noexcept {
            assert(size <= N);
            _last = _data + size;
            plastic::construct(_data, _last);
        }

        inplace_deque(size_type size, const_reference value) noexcept {
            assert(size <= N);
            _last = _data + size;
            plastic::construct(_data, _last, value);
        }

        template <std::input_iterator It>
        inplace_deque(It first, It last) noexcept :
            _last{ std::uninitialized_copy(first, last, _data) } {}

        inplace_deque(std::initializer_list<T> list) noexcept {
            assert(list.size() <= N);
            _last = std::uninitialized_copy(list.begin(), list.end(), _data);
        }

        inplace_deque(const inplace_deque& other) noexcept {
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

        inplace_deque(inplace_deque&& other) noexcept {
            swap(other);
        }

        inplace_deque& operator=(const inplace_deque& other) noexcept {
            inplace_deque temp(other);
            swap(temp);
            return *this;
        }

        inplace_deque& operator=(inplace_deque&& other) noexcept {
            swap(other);
            return *this;
        }

        void swap(inplace_deque& other) noexcept {
            std::swap(_data, other._data);

            std::ptrdiff_t first_offset{ _first - _data }, last_offset{ _last - _data };
            _first = _data + (other._first - other._data);
            _last = _data + (other._last - other._data);
            other._first = other._data + first_offset;
            other._last = other._data + last_offset;
        }

        friend void swap(inplace_deque& left, inplace_deque& right) noexcept {
            left.swap(right);
        }

        bool empty() const noexcept {
            return _first == _last;
        }

        size_type size() const noexcept {
            std::ptrdiff_t diff{ _last - _first };
            return diff >= 0 ? diff : diff + N + 1;
        }

        static size_type max_size() noexcept {
            return N;
        }

        void clear() noexcept {
            if (_first <= _last) {
                std::destroy(_first, _last);
            }
            else {
                std::destroy(_first, _data + N + 1);
                std::destroy(_data, _last);
            }
            _first = _last = _data;
        }

        void resize(size_type new_size) noexcept {
            assert(new_size <= N);
            _resize(new_size);
        }

        void resize(size_type new_size, const_reference value) noexcept {
            assert(new_size <= N);
            _resize(new_size, value);
        }

        static size_type capacity() noexcept {
            return N;
        }

        static void reserve(size_type new_capacity) noexcept {
            assert(new_capacity <= N);
        }

        iterator begin() noexcept {
            return { _first, this };
        }

        const_iterator begin() const noexcept {
            return iterator{ _first, const_cast<inplace_deque*>(this) };
        }

        iterator end() noexcept {
            return { _last, this };
        }

        const_iterator end() const noexcept {
            return iterator{ _last, const_cast<inplace_deque*>(this) };
        }

        const_iterator cbegin() const noexcept {
            return begin();
        }

        const_iterator cend() const noexcept {
            return end();
        }

        reverse_iterator rbegin() noexcept {
            return reverse_iterator{ end() };
        }

        const_reverse_iterator rbegin() const noexcept {
            return const_reverse_iterator{ end() };
        }

        reverse_iterator rend() noexcept {
            return reverse_iterator{ begin() };
        }

        const_reverse_iterator rend() const noexcept {
            return const_reverse_iterator{ begin() };
        }

        const_reverse_iterator crbegin() const noexcept {
            return rbegin();
        }

        const_reverse_iterator crend() const noexcept {
            return rend();
        }

        reference operator[](size_type index) noexcept {
            assert(index < size());
            return begin()[index];
        }

        const_reference operator[](size_type index) const noexcept {
            assert(index < size());
            return begin()[index];
        }

        reference front() noexcept {
            assert(!empty());
            return *_first;
        }

        const_reference front() const noexcept {
            assert(!empty());
            return *_first;
        }

        reference back() noexcept {
            assert(!empty());
            return *--end();
        }

        const_reference back() const noexcept {
            assert(!empty());
            return *--end();
        }

        void push_front(const_reference value) noexcept {
            assert(size() != N);
            if (_first == _data) {
                _first = _data + N;
                std::construct_at(_first, value);
            }
            else {
                std::construct_at(--_first, value);
            }
        }

        void pop_front() noexcept {
            assert(!empty());
            if (_first == _data + N) {
                std::destroy_at(_first);
                _first = _data;
            }
            else {
                std::destroy_at(_first++);
            }
        }

        void push_back(const_reference value) noexcept {
            assert(size() != N);
            if (_last == _data + N) {
                std::construct_at(_last, value);
                _last = _data;
            }
            else {
                std::construct_at(_last++, value);
            }
        }

        void pop_back() noexcept {
            assert(!empty());
            if (_last == _data) {
                _last = _data + N;
                std::destroy_at(_last);
            }
            else {
                std::destroy_at(--_last);
            }
        }

        iterator insert(const_iterator pos, const_reference value) noexcept {
            return insert(pos, 1, value);
        }

        iterator insert(const_iterator pos, size_type count, const_reference value) noexcept {
            assert(size() + count <= N);
            iterator i{ pos.base() };
            iterator new_last{ end() + count };
            std::fill(i, std::move_backward(i, end(), new_last), value);
            _last = new_last._ptr;
            return i;
        }

        template <std::input_iterator It>
        iterator insert(const_iterator pos, It first, It last) noexcept {
            iterator i{ pos.base() };
            iterator new_last{ std::uninitialized_copy(first, last, end()) };
            std::rotate(i, end(), new_last);
            _last = new_last._ptr;
            return i;
        }

        iterator insert(const_iterator pos, std::initializer_list<T> list) noexcept {
            return insert(pos, list.begin(), list.end());
        }

        iterator erase(const_iterator pos) noexcept {
            iterator i{ pos.base() };
            assert(i != end());
            _last = std::move(i + 1, end(), i)._ptr;
            std::destroy_at(_last);
            return i;
        }

        iterator erase(const_iterator first, const_iterator last) noexcept {
            iterator i{ first.base() }, s{ last.base() };
            iterator new_last{ std::move(s, end(), i) };
            std::destroy(new_last, end());
            _last = new_last._ptr;
            return i;
        }

        friend bool operator==(const inplace_deque& left, const inplace_deque& right) noexcept {
            return std::equal(left.begin(), left.end(), right.begin(), right.end());
        }

        friend auto operator<=>(const inplace_deque& left, const inplace_deque& right) noexcept {
            return std::lexicographical_compare_three_way(left.begin(), left.end(), right.begin(), right.end());
        }
    };

    export template <class T>
    class deque {
        T* _begin{};
        T* _end{};
        T* _first{};
        T* _last{};

        deque(uninitialized_t, std::size_t size) noexcept:
            _begin{ new T[size] },
            _end{ _begin + size },
            _first{ _begin },
            _last{ _end } {}

        void _reallocate(std::size_t new_capacity, std::ptrdiff_t offset) noexcept {
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

        void _extend(std::size_t size, std::ptrdiff_t offset) noexcept {
            std::size_t new_capacity{ capacity() + std::max(capacity(), size) };
            std::size_t new_size{ this->size() + size };
            _reallocate(new_capacity, (new_capacity - new_size >> 1) + offset);
        }

        template <class... Args>
        void _resize(std::size_t new_size, const Args&... args) noexcept {
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

        deque() noexcept = default;

        explicit deque(size_type size) noexcept :
            deque(uninitialized, size) {

            plastic::construct(_first, _last);
        }

        deque(size_type size, const_reference value) noexcept :
            deque(uninitialized, size) {

            plastic::construct(_first, _last, value);
        }

        template <std::input_iterator It>
        deque(It first, It last) noexcept {
            while (first != last) {
                push_back(*first);
                ++first;
            }
        }

        deque(std::initializer_list<T> list) noexcept :
            deque(list.begin(), list.end()) {}

        deque(const deque& other) noexcept :
            deque(other._first, other._last) {}

        deque(deque&& other) noexcept {
            swap(other);
        }

        ~deque() noexcept {
            delete[] _begin;
        }

        deque& operator=(const deque& other) noexcept {
            deque temp{ other };
            swap(temp);
            return *this;
        }

        deque& operator=(deque&& other) noexcept {
            swap(other);
            return *this;
        }

        void swap(deque& other) noexcept {
            std::swap(_begin, other._begin);
            std::swap(_end, other._end);
            std::swap(_first, other._first);
            std::swap(_last, other._last);
        }

        friend void swap(deque& left, deque& right) noexcept {
            left.swap(right);
        }

        bool empty() const noexcept {
            return _first == _last;
        }

        size_type size() const noexcept {
            return _last - _first;
        }

        static size_type max_size() noexcept {
            return std::numeric_limits<size_type>::max();
        }

        void clear() noexcept {
            std::destroy(_first, _last);
            _first = _last = _begin + (size() >> 1);
        }

        void resize(size_type new_size) noexcept {
            _resize(new_size);
        }

        void resize(size_type new_size, const_reference value) noexcept {
            _resize(new_size, value);
        }

        size_type capacity() const noexcept {
            return _end - _begin;
        }

        void reserve(size_type new_capacity) noexcept {
            if (new_capacity > capacity()) {
                _reallocate(new_capacity, capacity() - size() >> 1);
            }
        }

        iterator begin() noexcept {
            return _first;
        }

        const_iterator begin() const noexcept {
            return _first;
        }

        iterator end() noexcept {
            return _last;
        }

        const_iterator end() const noexcept {
            return _last;
        }

        const_iterator cbegin() const noexcept {
            return begin();
        }

        const_iterator cend() const noexcept {
            return end();
        }

        reverse_iterator rbegin() noexcept {
            return reverse_iterator{ end() };
        }

        const_reverse_iterator rbegin() const noexcept {
            return const_reverse_iterator{ end() };
        }

        reverse_iterator rend() noexcept {
            return reverse_iterator{ begin() };
        }

        const_reverse_iterator rend() const noexcept {
            return const_reverse_iterator{ begin() };
        }

        const_reverse_iterator crbegin() const noexcept {
            return rbegin();
        }

        const_reverse_iterator crend() const noexcept {
            return rend();
        }

        reference operator[](size_type index) noexcept {
            assert(index < size());
            return _first[index];
        }

        const_reference operator[](size_type index) const noexcept {
            assert(index < size());
            return _first[index];
        }

        reference front() noexcept {
            assert(!empty());
            return *_first;
        }

        const_reference front() const noexcept {
            assert(!empty());
            return *_first;
        }

        reference back() noexcept {
            assert(!empty());
            return _last[-1];
        }

        const_reference back() const noexcept {
            assert(!empty());
            return _last[-1];
        }

        pointer data() noexcept {
            return _first;
        }

        const_pointer data() const noexcept {
            return _first;
        }

        void push_front(const_reference value) noexcept {
            if (_first == _begin) {
                _extend(1, 1);
            }
            std::construct_at(--_first, value);
        }

        void pop_front() noexcept {
            assert(!empty());
            std::destroy_at(_first++);
        }

        void push_back(const_reference value) noexcept {
            if (_last == _end) {
                _extend(1, 0);
            }
            std::construct_at(_last++, value);
        }

        void pop_back() noexcept {
            assert(!empty());
            std::destroy_at(--_last);
        }

        iterator insert(const_iterator pos, const_reference value) noexcept {
            return insert(pos, 1, value);
        }

        iterator insert(const_iterator pos, size_type count, const_reference value) noexcept {
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
        iterator insert(const_iterator pos, It first, It last) noexcept {
            T* i{ pos.base() };
            std::ptrdiff_t offset1{ i - _first }, offset2{ _last - _first };
            while (first != last) {
                push_back(*first);
                ++first;
            }
            std::rotate(_first + offset1, _first + offset2, _last);
            return i;
        }

        iterator insert(const_iterator pos, std::initializer_list<T> list) noexcept {
            return insert(pos, list.begin(), list.end());
        }

        iterator erase(const_iterator pos) noexcept {
            T* i{ pos.base() };
            assert(i != _last);
            _last = std::move(i + 1, _last, i);
            std::destroy_at(_last);
            return i;
        }

        iterator erase(const_iterator first, const_iterator last) noexcept {
            T *i{ first.base() }, *s{ last.base() };
            T* new_last{ std::move(s, _last, i) };
            std::destroy(new_last, _last);
            _last = new_last;
            return i;
        }

        friend bool operator==(const deque& left, const deque& right) noexcept {
            return std::equal(left.begin(), left.end(), right.begin(), right.end());
        }

        friend auto operator<=>(const deque& left, const deque& right) noexcept {
            return std::lexicographical_compare_three_way(left.begin(), left.end(), right.begin(), right.end());
        }
    };

    template <class It>
    explicit deque(It, It) -> deque<std::iter_value_t<It>>;

    export template <class T>
    class forward_list {
        struct node {
            node* next;
            T value;
        };

        node* _head;
        std::size_t _size;

        template <class... Args>
        node* _insert_after(node* pos, std::size_t count, const Args&... args) noexcept {
            _size += count;
            while (count-- != 0) {
                pos = pos->next = new node{ pos->next, args... };
            }
            return pos;
        }

        template <class... Args>
        void _resize(std::size_t new_size, const Args&... args) noexcept {
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

            iterator(node* ptr) noexcept :
                _ptr{ ptr } {}

        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using iterator_category = std::forward_iterator_tag;

            iterator() noexcept = default;

            reference operator*() const noexcept {
                return _ptr->value;
            }

            pointer operator->() const noexcept {
                return &_ptr->value;
            }

            friend bool operator==(iterator left, iterator right) noexcept {
                return left._ptr == right._ptr;
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

        forward_list() noexcept :
            _head{ new node },
            _size{} {

            _head->next = _head;
        }

        explicit forward_list(size_type size) noexcept :
            forward_list() {

            _insert_after(_head, size);
        }

        forward_list(size_type size, const_reference value) noexcept :
            forward_list() {

            insert_after(end(), size, value);
        }

        template <std::input_iterator It>
        forward_list(It first, It last) noexcept :
            forward_list() {

            insert_after(end(), first, last);
        }

        forward_list(std::initializer_list<T> list) noexcept :
            forward_list(list.begin(), list.end()) {}

        forward_list(const forward_list& other) noexcept :
            forward_list(other.begin(), other.end()) {}

        forward_list(forward_list&& other) noexcept {
            swap(other);
        }

        ~forward_list() noexcept {
            clear();
            delete _head;
        }

        forward_list& operator=(const forward_list& other) noexcept {
            forward_list temp{ other };
            swap(temp);
            return *this;
        }

        forward_list& operator=(forward_list&& other) noexcept {
            swap(other);
            return *this;
        }

        void swap(forward_list& other) noexcept {
            std::swap(_head, other._head);
            std::swap(_size, other._size);
        }

        friend void swap(forward_list& left, forward_list& right) noexcept {
            left.swap(right);
        }

        bool empty() const noexcept {
            return _size == 0;
        }

        size_type size() const noexcept {
            return _size;
        }

        static size_type max_size() noexcept {
            return std::numeric_limits<size_type>::max();
        }

        void clear() noexcept {
            erase_after(end(), end());
        }

        void resize(size_type new_size) noexcept {
            _resize(new_size);
        }

        void resize(size_type new_size, const_reference value) noexcept {
            _resize(new_size, value);
        }

        iterator begin() noexcept {
            return _head->next;
        }

        const_iterator begin() const noexcept {
            return iterator{ _head->next };
        }

        iterator end() noexcept {
            return _head;
        }

        const_iterator end() const noexcept {
            return iterator{ _head };
        }

        const_iterator cbegin() const noexcept {
            return begin();
        }

        const_iterator cend() const noexcept {
            return end();
        }

        reference front() noexcept {
            assert(!empty());
            return _head->next->value;
        }

        const_reference front() const noexcept {
            assert(!empty());
            return _head->next->value;
        }

        void push_front(const_reference value) noexcept {
            _head->next = new node{ _head->next, value };
            ++_size;
        }

        void pop_front() noexcept {
            assert(!empty());
            erase_after(end());
        }

        iterator insert_after(const_iterator pos, const_reference value) noexcept {
            return insert_after(pos, 1, value);
        }

        iterator insert_after(const_iterator pos, size_type count, const_reference value) noexcept {
            return _insert_after(pos.base()._ptr, count, value);
        }

        template <std::input_iterator It>
        iterator insert_after(const_iterator pos, It first, It last) noexcept {
            node* i{ pos.base()._ptr };
            while (first != last) {
                i = i->next = new node{ i->next, *first };
                ++first;
                ++_size;
            }
            return i;
        }

        iterator insert_after(const_iterator pos, std::initializer_list<T> list) noexcept {
            return insert_after(pos, list.begin(), list.end());
        }

        iterator erase_after(const_iterator pos) noexcept {
            node* i{ pos.base()._ptr };
            delete std::exchange(i->next, i->next->next);
            --_size;
            return i->next;
        }

        iterator erase_after(const_iterator first, const_iterator last) noexcept {
            node *i{ first.base()._ptr }, *j{ last.base()._ptr };
            i = std::exchange(i->next, j);
            while (i != j) {
                delete std::exchange(i, i->next);
                --_size;
            }
            return i;
        }

        friend bool operator==(const forward_list& left, const forward_list& right) noexcept {
            return std::equal(left.begin(), left.end(), right.begin(), right.end());
        }

        friend auto operator<=>(const forward_list& left, const forward_list& right) noexcept {
            return std::lexicographical_compare_three_way(left.begin(), left.end(), right.begin(), right.end());
        }
    };

    template <class It>
    explicit forward_list(It, It) -> forward_list<std::iter_value_t<It>>;

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
        node* _insert(node* pos, std::size_t count, const Args&... args) noexcept {
            node *prev{ pos->prev }, *i{ prev };
            _size += count;
            while (count-- != 0) {
                i = i->next = new node{ i, i->next, args... };
            }
            i->next->prev = i;
            return prev->next;
        }

        template <class... Args>
        void _resize(std::size_t new_size, const Args&... args) noexcept {
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

            iterator(node* ptr) noexcept :
                _ptr{ ptr } {}

        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using iterator_category = std::bidirectional_iterator_tag;

            iterator() noexcept = default;

            reference operator*() const noexcept {
                return _ptr->value;
            }

            pointer operator->() const noexcept {
                return &_ptr->value;
            }

            friend bool operator==(iterator left, iterator right) noexcept {
                return left._ptr == right._ptr;
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
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        list() noexcept :
            _head{ new node },
            _size{} {

            _head->prev = _head->next = _head;
        }

        explicit list(size_type size) noexcept :
            list() {

            _insert(_head, size);
        }

        list(size_type size, const T& value) noexcept :
            list() {

            insert(end(), size, value);
        }

        template <std::input_iterator It>
        list(It first, It last) noexcept :
            list() {

            insert(end(), first, last);
        }

        list(std::initializer_list<T> list) noexcept :
            list(list.begin(), list.end()) {}

        list(const list& other) noexcept :
            list(other.begin(), other.end()) {}

        list(list&& other) noexcept {
            swap(other);
        }

        ~list() noexcept {
            clear();
            delete _head;
        }

        list& operator=(const list& other) noexcept {
            list temp{ other };
            swap(temp);
            return *this;
        }

        list& operator=(list&& other) noexcept {
            swap(other);
            return *this;
        }

        void swap(list& other) noexcept {
            std::swap(_head, other._head);
            std::swap(_size, other._size);
        }

        bool empty() const noexcept {
            return _size == 0;
        }

        size_type size() const noexcept {
            return _size;
        }

        static size_type max_size() noexcept {
            return std::numeric_limits<size_type>::max();
        }

        void clear() noexcept {
            erase(begin(), end());
        }

        void resize(size_type new_size) noexcept {
            _resize(new_size);
        }

        void resize(size_type new_size, const_reference value) noexcept {
            _resize(new_size, value);
        }

        iterator begin() noexcept {
            return _head->next;
        }

        const_iterator begin() const noexcept {
            return iterator{ _head->next };
        }

        iterator end() noexcept {
            return _head;
        }

        const_iterator end() const noexcept {
            return iterator{ _head };
        }

        const_iterator cbegin() const noexcept {
            return begin();
        }

        const_iterator cend() const noexcept {
            return end();
        }

        reverse_iterator rbegin() noexcept {
            return reverse_iterator{ end() };
        }

        const_reverse_iterator rbegin() const noexcept {
            return const_reverse_iterator{ end() };
        }

        reverse_iterator rend() noexcept {
            return reverse_iterator{ begin() };
        }

        const_reverse_iterator rend() const noexcept {
            return const_reverse_iterator{ begin() };
        }

        const_reverse_iterator crbegin() const noexcept {
            return rbegin();
        }

        const_reverse_iterator crend() const noexcept {
            return rend();
        }

        reference front() noexcept {
            assert(!empty());
            return _head->next->value;
        }

        const_reference front() const noexcept {
            assert(!empty());
            return _head->next->value;
        }

        reference back() noexcept {
            assert(!empty());
            return _head->prev->value;
        }

        const_reference back() const noexcept {
            assert(!empty());
            return _head->prev->value;
        }

        void push_front(const_reference value) noexcept {
            _head->next->next->prev = _head->next = new node{ _head, _head->next, value };
            ++_size;
        }

        void pop_front() noexcept {
            assert(!empty());
            erase(begin());
        }

        void push_back(const_reference value) noexcept {
            _head->prev->prev->next = _head->prev = new node{ _head->prev, _head, value };
            ++_size;
        }

        void pop_back() noexcept {
            assert(!empty());
            erase(--end());
        }

        iterator insert(const_iterator pos, const_reference value) noexcept {
            return insert(pos, 1, value);
        }

        iterator insert(const_iterator pos, size_type count, const_reference value) noexcept {
            return _insert(pos.base()._ptr, count, value);
        }

        template <std::input_iterator It>
        iterator insert(const_iterator pos, It first, It last) noexcept {
            node *prev{ pos.base()._ptr->prev }, *i{ prev };
            while (first != last) {
                i = i->next = new node{ i, i->next, *first };
                ++first;
                ++_size;
            }
            i->next->prev = i;
            return prev->next;
        }

        iterator insert(const_iterator pos, std::initializer_list<T> list) noexcept {
            return insert(pos, list.begin(), list.end());
        }

        iterator erase(const_iterator pos) noexcept {
            node* i{ pos.base()._ptr->prev };
            delete std::exchange(i->next, i->next->next);
            --_size;
            i->next->prev = i;
            return i->next;
        }

        iterator erase(const_iterator first, const_iterator last) noexcept {
            node *i{ first.base()._ptr }, *j{ last.base()._ptr };
            i->prev->next = j;
            j->prev = i->prev;
            while (i != j) {
                delete std::exchange(i, i->next);
                --_size;
            }
            return i;
        }

        friend bool operator==(const list& left, const list& right) noexcept {
            return std::equal(left.begin(), left.end(), right.begin(), right.end());
        }

        friend auto operator<=>(const list& left, const list& right) noexcept {
            return std::lexicographical_compare_three_way(left.begin(), left.end(), right.begin(), right.end());
        }
    };

    template <class It>
    explicit list(It, It) -> list<std::iter_value_t<It>>;

}

// search trees
namespace plastic {

    export template <class T, class Pr = std::less<T>>
    class binary_search_tree {
        struct node {
            node* parent;
            node* left;
            node* right;
            unsigned char is_head;
            T value;

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

            void free() const noexcept {
                if (is_head) {
                    return;
                }
                left->free();
                right->free();
                delete this;
            }

            node* clone(node* head) noexcept {
                if (is_head) {
                    return head;
                }
                return new node{ this, left->clone(head), right->clone(head), value, false };
            }
        };

        Pr _pred;
        node* _head;
        std::size_t _size;

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

            friend bool operator==(iterator left, iterator right) noexcept {
                return left._ptr == right._ptr;
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

        binary_search_tree() noexcept :
            _head{ new node },
            _size{} {

            _head->parent = _head->left = _head->right = _head;
            _head->is_head = true;
        }

        template <std::input_iterator It>
        binary_search_tree(It first, It last) noexcept :
            binary_search_tree() {

            insert(first, last);
        }

        binary_search_tree(std::initializer_list<T> list) noexcept :
            binary_search_tree(list.begin(), list.end()) {}

        binary_search_tree(const binary_search_tree& other) noexcept :
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
            std::swap(_head, temp._head);
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
            _head->parent->free();
        }

        const_iterator begin() const noexcept {
            return _head->right;
        }

        const_iterator end() const noexcept {
            return _head;
        }

        const_iterator cbegin() const noexcept {
            return _head->right;
        }

        const_iterator cend() const noexcept {
            return _head;
        }

        const_reverse_iterator rbegin() const noexcept {
            return reverse_iterator{ _head };
        }

        const_reverse_iterator rend() const noexcept {
            return reverse_iterator{ _head->right };
        }

        const_reverse_iterator crbegin() const noexcept {
            return reverse_iterator{ _head };
        }

        const_reverse_iterator crend() const noexcept {
            return reverse_iterator{ _head->right };
        }

        const_iterator lower_bound(const T& value) const noexcept {
            node* bound{ _head };
            node* i{ _head->parent };
            while (!i->is_head) {
                if (!_pred(i->value, value)) {
                    bound = i;
                    i = i->left;
                }
                else {
                    i = i->right;
                }
            }
            return bound;
        }

        const_iterator upper_bound(const T& value) const noexcept {
            node* bound{ _head };
            node* i{ _head->parent };
            while (!i->is_head) {
                if (_pred(value, i->value)) {
                    bound = i;
                    i = i->left;
                }
                else {
                    i = i->right;
                }
            }
            return bound;
        }

        std::pair<const_iterator, const_iterator> equal_range(const T& value) const noexcept {
            return { lower_bound(value), upper_bound(value) };
        }

        const_iterator find(const T& value) const noexcept {
            node* bound{ lower_bound(value)._ptr };
            return !bound->is_head && !_pred(bound->value, value) ? bound : _head;
        }

        bool contains(const T& value) const noexcept {
            node* bound{ lower_bound(value)._ptr };
            return !bound->is_head && !_pred(bound->value, value);
        }

        std::size_t count(const T& value) const noexcept {
            auto [first, last]{ equal_range(value) };
            return std::distance(first, last);
        }

        iterator insert(const T& value) noexcept {
            node* parent{ _head };
            bool is_left{};
            node* i{ _head->parent };
            while (!i->is_head) {
                parent = i;
                is_left = _pred(value, i->value);
                i = is_left ? i->left : i->right;
            }

            node* new_node{ new node{ parent, _head, _head, false, value } };

            if (parent->is_head) {
                _head->parent = new_node;
            }

            if (is_left) {
                parent->left = new_node;
                if (parent == _head->right) {
                    _head->right = new_node;
                }
            }
            else {
                parent->right = new_node;
                if (parent == _head->left) {
                    _head->left = new_node;
                }
            }
            ++_size;

            return new_node;
        }

        template <std::input_iterator It>
        void insert(It first, It last) noexcept {
            while (first != last) {
                insert(*first);
                ++first;
            }
        }

        void insert(std::initializer_list<T> list) noexcept {
            insert(list.begin(), list.end());
        }

        iterator erase(iterator pos) noexcept {
            node* erased{ (pos++)._ptr };

            if (erased->left->is_head || erased->right->is_head) {
                node* parent{ erased->parent };
                node* replaced{ erased->left->is_head ? erased->right : erased->left };

                if (!replaced->is_head) {
                    replaced->parent = parent;
                }

                if (erased == _head->parent) {
                    _head->parent = replaced;
                }
                else if (erased == parent->left) {
                    parent->left = replaced;
                }
                else {
                    parent->right = replaced;
                }

                if (erased == _head->right) {
                    _head->right = replaced->is_head ? parent : replaced->rightmost();
                }
                if (erased == _head->left) {
                    _head->left = replaced->is_head ? parent : replaced->leftmost();
                }
            }
            else {
                node* replaced{ pos._ptr };
                node* replaced_parent{ replaced->parent };
                node* replaced_child{ replaced->right };

                erased->left->parent = replaced;
                replaced->left = erased->left;

                if (replaced != erased->right) {
                    if (!replaced_child->is_head) {
                        replaced_child->parent = replaced_parent;
                    }
                    replaced_parent->left = replaced_child;
                    replaced->right = erased->right;
                    erased->right->parent = replaced;
                }

                if (erased == _head->parent) {
                    _head->parent = replaced;
                }
                else if (erased == erased->parent->left) {
                    erased->parent->left = replaced;
                }
                else {
                    erased->parent->right = replaced;
                }

                replaced->parent = erased->parent;
            }

            delete erased;
            --_size;

            return pos;
        }

        iterator erase(iterator first, iterator last) noexcept {
            while (first != last) {
                erase(first++);
            }
            return last;
        }

        std::size_t erase(const T& value) noexcept {
            std::size_t count{};
            auto [first, last]{ equal_range(value) };
            while (first != last) {
                erase(first++);
                ++count;
            }
            return count;
        }

        friend bool operator==(const binary_search_tree& left, const binary_search_tree& right) noexcept {
            return std::equal(left.begin(), left.end(), right.begin(), right.end());
        }

        friend auto operator<=>(const binary_search_tree& left, const binary_search_tree& right) noexcept {
            return std::lexicographical_compare_three_way(left.begin(), left.end(), right.begin(), right.end());
        }
    };

    template <class It>
    explicit binary_search_tree(It, It) -> binary_search_tree<std::iter_value_t<It>>;

}

// addressable heaps
namespace plastic {

    export template <class T, class Pr = std::less<T>>
    class binary_heap {
        struct node {
            std::size_t index;
            T value;
        };

        Pr _pred;
        vector<node*> _data;

        void _set(std::size_t index, node* ptr) noexcept {
            _data[index] = ptr;
            _data[index]->index = index;
        }

        void _sift_up(std::size_t index) noexcept {
            node* ptr{ _data[index] };
            while (index != 0) {
                std::size_t parent{ index - 1 >> 1 };
                if (!_pred(_data[parent]->value, ptr->value)) {
                    break;
                }
                _set(index, _data[parent]);
                index = parent;
            }
            _set(index, ptr);
        }

        void _sift_down(std::size_t index) noexcept {
            node* ptr{ _data[index] };
            while (true) {
                std::size_t child{ (index << 1) + 1 };
                if (child >= size()) {
                    break;
                }
                if (child + 1 < size() && _pred(_data[child]->value, _data[child + 1]->value)) {
                    ++child;
                }
                if (!_pred(ptr->value, _data[child]->value)) {
                    break;
                }
                _set(index, _data[child]);
                index = child;
            }
            _set(index, ptr);
        }

        void _make_heap() noexcept {
            std::size_t i{ size() >> 1 };
            while (i-- != 0) {
                _sift_down(i);
            }
        }

    public:
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;

        class handle {
            friend binary_heap;

            node* _ptr{};

            handle(node* ptr) noexcept :
                _ptr{ ptr } {}

        public:
            using value_type = T;
            using pointer = const T*;
            using reference = const T&;

            handle() noexcept = default;

            reference operator*() const noexcept {
                return _ptr->value;
            }

            pointer operator->() const noexcept {
                return &_ptr->value;
            }

            friend bool operator==(handle left, handle right) noexcept {
                return left._ptr == right._ptr;
            }
        };

        binary_heap() noexcept = default;

        template <std::input_iterator It>
        binary_heap(It first, It last) noexcept {
            std::size_t index{};
            while (first != last) {
                _data.push_back(new node{ index++, *first });
                ++first;
            }

            _make_heap();
        }

        binary_heap(std::initializer_list<T> list) noexcept :
            binary_heap(list.begin(), list.end()) {}

        binary_heap(const binary_heap& other) noexcept :
            _data(other.size()) {

            auto i{ _data.begin() };
            for (node* j : other._data) {
                *i++ = new node{ *j };
            }
        }

        binary_heap(binary_heap&& other) noexcept {
            swap(other);
        }

        ~binary_heap() noexcept {
            clear();
        }

        binary_heap& operator=(const binary_heap& other) noexcept {
            binary_heap temp(other);
            swap(temp);
            return *this;
        }

        binary_heap& operator=(binary_heap&& other) noexcept {
            swap(other);
            return *this;
        }

        void swap(binary_heap& other) noexcept {
            std::swap(_pred, other._pred);
            std::swap(_data, other._data);
        }

        friend void swap(binary_heap& left, binary_heap& right) noexcept {
            left.swap(right);
        }

        bool empty() const noexcept {
            return _data.empty();
        }

        size_type size() const noexcept {
            return _data.size();
        }

        static size_type max_size() noexcept {
            return std::numeric_limits<size_type>::max();
        }

        void clear() noexcept {
            for (node* i : _data) {
                delete i;
            }
            _data.clear();
        }

        handle top_handle() const noexcept {
            assert(!empty());
            return _data[0];
        }

        const_reference top() const noexcept {
            assert(!empty());
            return _data[0]->value;
        }

        handle push(const_reference value) noexcept {
            std::size_t index{ size() };
            auto ptr{ new node{ index, value } };
            _data.push_back(ptr);
            _sift_up(index);
            return ptr;
        }

        void pop() noexcept {
            assert(!empty());
            delete _data[0];
            if (size() == 1) {
                _data.pop_back();
            }
            else {
                _set(0, _data.back());
                _data.pop_back();
                _sift_down(0);
            }
        }

        void merge(const binary_heap& other) noexcept {
            _data.reserve(size() + other.size());

            std::size_t index{ size() };
            for (node* i : other._data) {
                _data.push_back(new node{ index++, i->value });
            }

            _make_heap();
        }

        void update(handle pos, const_reference new_value) noexcept {
            std::size_t index{ pos._ptr->index };
            assert(index < size());
            bool is_greater{ _pred(pos._ptr->value, new_value) };
            pos._ptr->value = new_value;
            if (is_greater) {
                _sift_up(index);
            }
            else {
                _sift_down(index);
            }
        }

        void erase(handle pos) noexcept {
            std::size_t index{ pos._ptr->index };
            assert(index < size());
            if (index == size() - 1) {
                _data.pop_back();
            }
            else {
                _set(index, _data.back());
                _data.pop_back();
                if (_pred(pos._ptr->value, _data[index]->value)) {
                    _sift_up(index);
                }
                else {
                    _sift_down(index);
                }
            }
            delete pos._ptr;
        }
    };

    template <class It>
    explicit binary_heap(It, It) -> binary_heap<std::iter_value_t<It>>;

}
