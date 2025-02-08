#pragma once

#include <cassert>
#include <iterator>

namespace plastic {

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

            node* _ptr;

        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using iterator_category = std::forward_iterator_tag;

            iterator(node* node = {}) noexcept :
                _ptr{ node } {}

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

        explicit forward_list(std::size_t size = {}, const T& value = {}) noexcept :
            _sentinel{ new node },
            _size{} {

            _sentinel->next = _sentinel;
            insert_after(end(), size, value);
        }

        template<std::input_iterator It>
        explicit forward_list(It first, It last) noexcept :
            _sentinel{ new node },
            _size{} {

            _sentinel->next = _sentinel;
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

}