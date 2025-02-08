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

            T* _ptr;
            const deque* _cont;

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

            iterator(T* ptr = {}, const deque* cont = {}) noexcept :
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

        explicit deque(std::size_t size = {}, const T& value = {}) noexcept :
            _begin{ new T[size + 1] },
            _end{ _begin + size + 1 },
            _first{ _begin },
            _last{ _end - 1 } {

            std::uninitialized_fill(_first, _last, value);
        }

        template<std::input_iterator It>
        explicit deque(It first, It last) noexcept {
            std::ptrdiff_t count{ std::distance(first, last) };
            _begin = new T[count + 1];
            _end = _begin + count + 1;
            _first = _begin;
            _last = _end - 1;

            std::uninitialized_copy(first, last, _first);
        }

        explicit deque(std::initializer_list<T> list) noexcept :
            deque(list.begin(), list.end()) {}

        explicit deque(const deque& other) noexcept {
            std::size_t size{ other.size() };
            _begin = new T[size + 1];
            _end = _begin + size + 1;
            _first = _begin;
            _last = _end - 1;

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

}