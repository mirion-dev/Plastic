#pragma once

#include <cassert>
#include <memory>

namespace plastic {

    template<class T>
    class deque {
        T* _begin;
        T* _end;
        T* _first;
        T* _last;

        size_t _trueCapacity() const {
            return _end - _begin;
        }

        void _extend(size_t size) {
            reserve(capacity() + std::max(capacity() >> 1, size));
        }

    public:
        class iterator {
            friend class deque;

            T* _ptr;
            const deque* _cont;

        public:
            using difference_type = ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using iterator_category = std::random_access_iterator_tag;

            iterator(T* ptr = {}, const deque* cont = {}) {
                _ptr = ptr;
                _cont = cont;
            }

            reference operator*() const {
                return *_ptr;
            }

            pointer operator->() const {
                return _ptr;
            }

            reference operator[](size_t index) const {
                return *(*this + index);
            }

            bool operator==(iterator iter) const {
                return _ptr == iter._ptr;
            }

            std::strong_ordering operator<=>(iterator iter) const {
                return *this - iter <=> 0;
            }

            iterator& operator+=(difference_type diff) {
                if (_cont->_end - _ptr < diff) {
                    diff -= _cont->_trueCapacity();
                }
                else if (_ptr - _cont->_begin < -diff) {
                    diff += _cont->_trueCapacity();
                }
                _ptr += diff;
                return *this;
            }

            iterator& operator-=(difference_type diff) {
                return *this += -diff;
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
                ptrdiff_t diff{ _ptr - it._ptr };
                if (_cont->_first > _cont->_last) {
                    diff += _cont->_trueCapacity();
                }
                return diff;
            }

            iterator& operator++() {
                ++_ptr;
                if (_ptr == _cont->_end) {
                    _ptr = _cont->_begin;
                }
                return *this;
            }

            iterator operator++(int) {
                iterator temp{ *this };
                ++*this;
                return temp;
            }

            iterator& operator--() {
                if (_ptr == _cont->_begin) {
                    _ptr = _cont->_end;
                }
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

        explicit deque(size_t size = {}, const T& value = {}) {
            _begin = _first = new T[size + 1];
            _end = _begin + size + 1;
            _last = _end - 1;
            std::uninitialized_fill(_first, _last, value);
        }

        template<std::input_iterator It>
        explicit deque(It first, It last) {
            size_t size{ (size_t)std::distance(first, last) };
            _begin = _first = new T[size + 1];
            _end = _begin + size + 1;
            _last = _end - 1;
            std::uninitialized_copy(first, last, _first);
        }

        explicit deque(std::initializer_list<T> list) : deque(list.begin(), list.end()) {}

        ~deque() {
            delete[] _begin;
        }

        bool empty() const {
            return _first == _last;
        }

        size_t size() const {
            return end() - begin();
        }

        void clear() {
            resize(0);
        }

        void resize(size_t size, const T& value = {}) {
            if (size == this->size()) {
                return;
            }

            if (size < this->size()) {
                T* newLast{ (begin() + size)._ptr };
                if (newLast < _last) {
                    std::destroy(newLast, _last);
                }
                else {
                    std::destroy(newLast, _end);
                    std::destroy(_begin, _last);
                }
                _last = newLast;
                return;
            }

            if (size <= capacity()) {
                T* newLast{ (begin() + size)._ptr };
                if (_last < newLast) {
                    std::uninitialized_fill(_last, newLast, value);
                }
                else {
                    std::uninitialized_fill(_last, _end, value);
                    std::uninitialized_fill(_begin, newLast, value);
                }
                _last = newLast;
                return;
            }

            reserve(size);
            std::uninitialized_fill(_last, _end - 1, value);
            _last = _end - 1;
        }

        size_t capacity() const {
            return _trueCapacity() - 1;
        }

        void reserve(size_t capacity) {
            if (capacity <= this->capacity()) {
                return;
            }

            T* newBegin{ new T[capacity + 1] };
            T* newEnd{ newBegin + capacity + 1 };
            T* newLast;
            if (_first <= _last) {
                newLast = std::uninitialized_move(_first, _last, newBegin);
            }
            else {
                newLast = std::uninitialized_move(_begin, _last, std::uninitialized_move(_first, _end, newBegin));
            }
            delete[] _begin;

            _begin = _first = newBegin;
            _end = newEnd;
            _last = newLast;
        }

        iterator begin() {
            return { _first, this };
        }

        const_iterator begin() const {
            return iterator{ _first, this };
        }

        const_iterator cbegin() const {
            return iterator{ _first, this };
        }

        iterator end() {
            return { _last, this };
        }

        const_iterator end() const {
            return iterator{ _last, this };
        }

        const_iterator cend() const {
            return iterator{ _last, this };
        }

        reverse_iterator rbegin() {
            return reverse_iterator{ { _last, this } };
        }

        const_reverse_iterator rbegin() const {
            return reverse_iterator{ { _last, this } };
        }

        const_reverse_iterator crbegin() const {
            return reverse_iterator{ { _last, this } };
        }

        reverse_iterator rend() {
            return reverse_iterator{ { _first, this } };
        }

        const_reverse_iterator rend() const {
            return reverse_iterator{ { _first, this } };
        }

        const_reverse_iterator crend() const {
            return reverse_iterator{ { _first, this } };
        }

        T& operator[](size_t index) {
            assert(index < size());
            return begin()[index];
        }

        const T& operator[](size_t index) const {
            assert(index < size());
            return begin()[index];
        }

        T& front() {
            assert(!empty());
            return *_first;
        }

        const T& front() const {
            assert(!empty());
            return *_first;
        }

        T& back() {
            assert(!empty());
            return *--end();
        }

        const T& back() const {
            assert(!empty());
            return *--end();
        }

        void push_front(const T& value) {
            if (++end() == begin()) {
                _extend(1);
            }
            if (_first == _begin) {
                _first = _end;
            }
            *--_first = value;
        }

        void pop_front() {
            assert(!empty());
            std::destroy_at(_first++);
            if (_first == _end) {
                _first = _begin;
            }
        }

        void push_back(const T& value) {
            if (++end() == begin()) {
                _extend(1);
            }
            *_last++ = value;
            if (_last == _end) {
                _last = _begin;
            }
        }

        void pop_back() {
            assert(!empty());
            if (_last == _begin) {
                _last = _end;
            }
            std::destroy_at(--_last);
        }

        iterator insert(iterator pos, const T& value) {
            return insert(pos, 1, value);
        }

        iterator insert(iterator pos, size_t count, const T& value) {
            if (count == 0) {
                return pos;
            }
            if (capacity() - size() < count) {
                ptrdiff_t offset{ pos - begin() };
                _extend(count);
                pos = begin() + offset;
            }
            std::fill(pos, std::move_backward(pos, end(), end() + count), value);
            _last = (end() + count)._ptr;
            return pos;
        }

        template<std::input_iterator It>
        iterator insert(iterator pos, It first, It last) {
            if (first == last) {
                return pos;
            }
            size_t size{ (size_t)std::distance(first, last) };
            if (capacity() - this->size() < size) {
                ptrdiff_t offset{ pos - begin() };
                _extend(size);
                pos = begin() + offset;
            }
            std::copy_backward(first, last, std::move_backward(pos, end(), end() + size));
            _last = (end() + size)._ptr;
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

        bool operator==(const deque& cont) const {
            return std::equal(begin(), end(), cont.begin(), cont.end());
        }

        auto operator<=>(const deque& cont) const {
            return std::lexicographical_compare_three_way(begin(), end(), cont.begin(), cont.end());
        }

    };

    template<class It>
    explicit deque(It, It)->deque<std::iter_value_t<It>>;

}