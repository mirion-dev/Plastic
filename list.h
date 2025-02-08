#pragma once

#include <cassert>
#include <iterator>

namespace plastic {

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

            node* _ptr;

        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using iterator_category = std::bidirectional_iterator_tag;

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

        explicit list(std::size_t size = {}, const T& value = {}) noexcept :
            _sentinel{ new node },
            _size{} {

            _sentinel->next = _sentinel->prev = _sentinel;
            insert(end(), size, value);
        }

        template<std::input_iterator It>
        explicit list(It first, It last) noexcept :
            _sentinel{ new node },
            _size{} {

            _sentinel->next = _sentinel->prev = _sentinel;
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

}