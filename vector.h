#pragma once

#include <cassert>
#include <memory>

namespace plastic {

    template<class T>
    class vector {
        T* _begin;
        T* _last;
        T* _end;

        void _extend(size_t size) {
            reserve(capacity() + std::max(capacity() >> 1, size));
        }

    public:
        class iterator {
            friend class vector;

            T* _ptr;

        public:
            using difference_type = ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using iterator_category = std::contiguous_iterator_tag;

            iterator(T* ptr = {}) {
                _ptr = ptr;
            }

            reference operator*() const {
                return *_ptr;
            }

            pointer operator->() const {
                return _ptr;
            }

            reference operator[](size_t index) const {
                return _ptr[index];
            }

            bool operator==(iterator iter) const {
                return _ptr == iter._ptr;
            }

            std::strong_ordering operator<=>(iterator iter) const {
                return _ptr <=> iter._ptr;
            }

            iterator& operator+=(difference_type diff) {
                _ptr += diff;
                return *this;
            }

            iterator& operator-=(difference_type diff) {
                _ptr -= diff;
                return *this;
            }

            iterator operator+(difference_type diff) const {
                return iterator{ *this } += diff;
            }

            friend iterator operator+(difference_type diff, iterator iter) {
                return iter += diff;
            }

            iterator operator-(difference_type diff) const {
                return iterator{ *this } -= diff;
            }

            difference_type operator-(iterator it) const {
                return _ptr - it._ptr;
            }

            iterator& operator++() {
                ++_ptr;
                return *this;
            }

            iterator operator++(int) {
                iterator temp{ *this };
                ++*this;
                return temp;
            }

            iterator& operator--() {
                --_ptr;
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
        using const_reverse_iterator = std::const_iterator<reverse_iterator>;

        explicit vector(size_t size = {}, const T& value = {}) {
            _begin = new T[size];
            _last = _end = _begin + size;
            std::uninitialized_fill(_begin, _end, value);
        }

        template<std::input_iterator It>
        explicit vector(It first, It last) {
            size_t size{ (size_t)std::distance(first, last) };
            _begin = new T[size];
            _last = _end = _begin + size;
            std::uninitialized_copy(first, last, _begin);
        }

        explicit vector(std::initializer_list<T> list) : vector(list.begin(), list.end()) {}

        ~vector() {
            delete[] _begin;
        }

        bool empty() const {
            return _begin == _last;
        }

        size_t size() const {
            return _last - _begin;
        }

        void clear() {
            resize(0);
        }

        void resize(size_t size, const T& value = {}) {
            if (size == this->size()) {
                return;
            }

            if (size < this->size()) {
                T* newLast{ _begin + size };
                std::destroy(newLast, _last);
                _last = newLast;
                return;
            }

            if (size <= capacity()) {
                T* newLast{ _begin + size };
                std::uninitialized_fill(_last, newLast, value);
                _last = newLast;
                return;
            }

            reserve(size);
            std::uninitialized_fill(_last, _end, value);
            _last = _end;
        }

        size_t capacity() const {
            return _end - _begin;
        }

        void reserve(size_t capacity) {
            if (capacity <= this->capacity()) {
                return;
            }

            T* newBegin{ new T[capacity] };
            T* newLast{ std::uninitialized_move(_begin, _last, newBegin) };
            T* newEnd{ newBegin + capacity };
            delete[] _begin;

            _begin = newBegin;
            _last = newLast;
            _end = newEnd;
        }

        iterator begin() {
            return _begin;
        }

        const_iterator begin() const {
            return _begin;
        }

        const_iterator cbegin() const {
            return _begin;
        }

        iterator end() {
            return _last;
        }

        const_iterator end() const {
            return _last;
        }

        const_iterator cend() const {
            return _last;
        }

        reverse_iterator rbegin() {
            return reverse_iterator{ _last };
        }

        const_reverse_iterator rbegin() const {
            return reverse_iterator{ _last };
        }

        const_reverse_iterator crbegin() const {
            return reverse_iterator{ _last };
        }

        reverse_iterator rend() {
            return reverse_iterator{ _begin };
        }

        const_reverse_iterator rend() const {
            return reverse_iterator{ _begin };
        }

        const_reverse_iterator crend() const {
            return reverse_iterator{ _begin };
        }

        T& operator[](size_t index) {
            assert(index < size());
            return _begin[index];
        }

        const T& operator[](size_t index) const {
            assert(index < size());
            return _begin[index];
        }

        T& front() {
            assert(!empty());
            return *_begin;
        }

        const T& front() const {
            assert(!empty());
            return *_begin;
        }

        T& back() {
            assert(!empty());
            return _last[-1];
        }

        const T& back() const {
            assert(!empty());
            return _last[-1];
        }

        T* data() {
            return _begin;
        }

        const T* data() const {
            return _begin;
        }

        void push_back(const T& value) {
            if (_last == _end) {
                _extend(1);
            }
            *_last++ = value;
        }

        void pop_back() {
            assert(!empty());
            std::destroy_at(--_last);
        }

        iterator insert(iterator pos, const T& value) {
            return insert(pos, 1, value);
        }

        iterator insert(iterator pos, size_t count, const T& value) {
            if (count == 0) {
                return pos;
            }
            if (size_t(_end - _last) < count) {
                ptrdiff_t offset{ pos - begin() };
                _extend(count);
                pos = begin() + offset;
            }
            std::fill(pos, std::move_backward(pos, end(), end() + count), value);
            _last += count;
            return pos;
        }

        template<std::input_iterator It>
        iterator insert(iterator pos, It first, It last) {
            if (first == last) {
                return pos;
            }
            size_t size{ (size_t)std::distance(first, last) };
            if (size_t(_end - _last) < size) {
                ptrdiff_t offset{ pos - begin() };
                _extend(size);
                pos = begin() + offset;
            }
            std::copy_backward(first, last, std::move_backward(pos, end(), end() + size));
            _last += size;
            return pos;
        }

        iterator insert(iterator pos, std::initializer_list<T> list) {
            return insert(pos, list.begin(), list.end());
        }

        iterator erase(iterator pos) {
            assert(pos != end());
            _last = std::move(std::next(pos), end(), pos)._ptr;
            std::destroy_at(_last);
            return pos;
        }

        iterator erase(iterator first, iterator last) {
            if (first == last) {
                return first;
            }
            T* newLast{ std::move(last, end(), first)._ptr };
            std::destroy(newLast, _last);
            _last = newLast;
            return first;
        }

        bool operator==(const vector& cont) const {
            return std::equal(begin(), end(), cont.begin(), cont.end());
        }

        auto operator<=>(const vector& cont) const {
            return std::lexicographical_compare_three_way(begin(), end(), cont.begin(), cont.end());
        }

    };

    template<class It>
    explicit vector(It, It)->vector<std::iter_value_t<It>>;

}