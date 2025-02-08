#pragma once

#include <cassert>
#include <memory>

namespace plastic {

    template<class T>
    class vector {
        T* _begin;
        T* _last;
        T* _end;
        void _extend(std::size_t size) noexcept {
            reserve(capacity() + std::max(capacity() >> 1, size));
        }

    public:
        class iterator {
            friend class vector;

            T* _ptr;

        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using iterator_category = std::contiguous_iterator_tag;

            iterator(T* ptr = {}) noexcept :
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

        explicit vector(std::size_t size = {}, const T& value = {}) noexcept :
            _begin{ new T[size] },
            _last{ _begin + size },
            _end{ _last } {

            std::uninitialized_fill(_begin, _end, value);
        }

        template<std::input_iterator It>
        explicit vector(It first, It last) noexcept {
            std::ptrdiff_t count{ std::distance(first, last) };
            _begin = new T[count];
            _last = _begin + count;
            _end = _last;
            std::uninitialized_copy(first, last, _begin);
        }

        explicit vector(std::initializer_list<T> list) noexcept :
            vector(list.begin(), list.end()) {}

        explicit vector(const vector& other) noexcept :
            vector(other._begin, other._end) {}

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
            return std::forward_like<decltype(self)>(self._begin);
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
            _last = std::move(pos + 1, end(), pos)._ptr;
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

}