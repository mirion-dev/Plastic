#pragma once

#include <cassert>
#include <functional>

namespace plastic {

    template<class T, class Cmp = std::less<T>>
    class binary_search_tree {
        static constexpr Cmp _cmp{};

        struct node {
            node* parent;
            node* left;
            node* right;
            T value;
            bool nil;
        };

        struct find_result {
            node* parent;
            bool is_left;
            node* bound;
        };

        node* _head;
        std::size_t _size;

        void _free(node* node) noexcept {
            if (node->nil) {
                return;
            }
            _free(node->left);
            _free(node->right);
            delete node;
        }

        node* _leftmost(node* node) noexcept {
            while (!node->left->nil) {
                node = node->left;
            }
            return node;
        }

        node* _rightmost(node* node) noexcept {
            while (!node->right->nil) {
                node = node->right;
            }
            return node;
        }

        find_result _upper_bound(const T& value) noexcept {
            node* current{ _head->parent }, * parent{ current }, * bound{ _head };
            bool left{};
            while (!current->nil) {
                parent = current;
                left = _cmp(value, current->value);
                if (left) {
                    bound = current;
                    current = current->left;
                }
                else {
                    current = current->right;
                }
            }
            return { parent, left, bound };
        }

        find_result _lower_bound(const T& value) noexcept {
            node* current{ _head->parent }, * parent{ current }, * bound{ _head };
            bool left{};
            while (!current->nil) {
                parent = current;
                left = !_cmp(current->value, value);
                if (left) {
                    bound = current;
                    current = current->left;
                }
                else {
                    current = current->right;
                }
            }
            return { parent, left, bound };
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
                if (_ptr->right->nil) {
                    node* temp;
                    do {
                        temp = _ptr;
                        _ptr = _ptr->parent;
                    } while (!_ptr->nil && _ptr->right == temp);
                }
                else {
                    _ptr = _leftmost(_ptr->right);
                }
                return *this;
            }

            iterator operator++(int) noexcept {
                iterator temp{ *this };
                ++*this;
                return temp;
            }

            iterator& operator--() noexcept {
                if (_ptr->left->nil) {
                    node* temp;
                    do {
                        temp = _ptr;
                        _ptr = _ptr->parent;
                    } while (!_ptr->nil && _ptr->left == temp);
                }
                else {
                    _ptr = _rightmost(_ptr->left);
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

        explicit binary_search_tree(std::size_t size = {}, const T& value = {}) noexcept :
            _head{ new node },
            _size{} {

            _head->parent = _head->left = _head->right = _head;
            _head->nil = true;
            // TODO
        }

        template<std::input_iterator It>
        explicit binary_search_tree(It first, It last) noexcept {

        }

        explicit binary_search_tree(const binary_search_tree& other) noexcept {

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
            _free(_head->parent);
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
            return _head->right->value;
        }

        const T& back() const noexcept {
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
            auto it{ lower_bound(value) };
            return *it == value ? it : end();
        }

        bool contains(const T& value) const noexcept {
            return find(value) != end();
        }

        std::size_t count(const T& value) const noexcept {
            return std::distance(lower_bound(value), upper_bound(value));
        }

        iterator insert(const T& value) noexcept {
            auto result{ _lower_bound(value) };
            node* new_node{ new node{ result.parent, _head, _head, value, false } };
            if (result.left) {
                result.parent->left = new_node;
            }
            else {
                result.parent->right = new_node;
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
            if (new_node->left->nil || new_node->right->nil) {
                node* parent{ new_node->parent };
                node* erased{ new_node->left->nil ? new_node->right : new_node->left };
                erased->parent = parent;
                if (parent->nil) {
                    _head->parent = erased;
                }
                else if (parent->left == new_node) {
                    parent->left = erased;
                }
                else {
                    parent->right = erased;
                }
                if (_head->left == new_node) {
                    _head->left = erased->nil ? parent : _leftmost(erased);
                }
                if (_head->right == new_node) {
                    _head->right = erased->nil ? parent : _rightmost(erased);
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
            auto [first, last] {equal_range()};
            erase(first, last);
        }

        void merge(const binary_search_tree& other) noexcept {

        }

        friend bool operator==(const binary_search_tree& cont1, const binary_search_tree& cont2) noexcept {
            return std::equal(cont1.begin(), cont1.end(), cont2.begin(), cont2.end());
        }

        friend auto operator<=>(const binary_search_tree& cont1, const binary_search_tree& cont2) noexcept {
            return std::lexicographical_compare_three_way(cont1.begin(), cont1.end(), cont2.begin(), cont2.end());
        }
    };

    template<class It>
    explicit plastic::binary_search_tree(It, It)->plastic::binary_search_tree<std::iter_value_t<It>>;

}