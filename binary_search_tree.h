#pragma once

#include <cassert>
#include <functional>

namespace plastic {

    template<class T, class Cmp = std::less<T>>
    class binary_search_tree {
        static constexpr Cmp _cmp{};

        struct _node {
            _node* parent;
            _node* left;
            _node* right;
            T value;
            bool nil;
        };

        struct _findResult {
            _node* parent;
            bool left;
            _node* bound;
        };

        _node* _head;
        std::size_t _size;

        void _free(_node* node) {
            if (node->nil) {
                return;
            }
            _free(node->left);
            _free(node->right);
            delete node;
        }

        _node* _leftmost(_node* node) {
            while (!node->left->nil) {
                node = node->left;
            }
            return node;
        }

        _node* _rightmost(_node* node) {
            while (!node->right->nil) {
                node = node->right;
            }
            return node;
        }

        _findResult _upperBound(const T& value) {
            _node* current{ _head->parent }, * parent{ current }, * bound{ _head };
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

        _findResult _lowerBound(const T& value) {
            _node* current{ _head->parent }, * parent{ current }, * bound{ _head };
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

            _node* _ptr;

        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = const T*;
            using reference = const T&;
            using iterator_category = std::bidirectional_iterator_tag;

            iterator(_node* node = {}) {
                _ptr = node;
            }

            reference operator*() const {
                return _ptr->value;
            }

            pointer operator->() const {
                return &_ptr->value;
            }

            bool operator==(iterator it) const {
                return _ptr == it._ptr;
            }

            iterator& operator++() {
                if (_ptr->right->nil) {
                    _node* temp;
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

            iterator operator++(int) {
                iterator temp{ *this };
                ++*this;
                return temp;
            }

            iterator& operator--() {
                if (_ptr->left->nil) {
                    _node* temp;
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

            iterator operator--(int) {
                iterator temp{ *this };
                --*this;
                return temp;
            }
        };

        using const_iterator = iterator;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = reverse_iterator;

        explicit binary_search_tree() {
            _head->parent = _head->left = _head->right = _head = new _node;
            _head->nil = true;
            _size = 0;
        }

        ~binary_search_tree() {
            clear();
            delete _head;
        }

        bool empty() const {
            return _size == 0;
        }

        std::size_t size() const {
            return _size;
        }

        void clear() {
            _free(_head->parent);
        }

        const_iterator begin() const {
            return _head->right;
        }

        const_iterator end() const {
            return _head;
        }

        const_reverse_iterator rbegin() const {
            return _head;
        }

        const_reverse_iterator rend() const {
            return _head->right;
        }

        const T& front() const {
            return _head->right->value;
        }

        const T& back() const {
            return _head->left->value;
        }

        const_iterator lower_bound(const T& value) const {
            return _lowerBound(value).bound;
        }

        const_iterator upper_bound(const T& value) const {
            return _upperBound(value).bound;
        }

        std::pair<const_iterator, const_iterator> equal_range(const T& value) const {
            return { lower_bound(value), upper_bound(value) };
        }

        const_iterator find(const T& value) const {
            auto it{ lower_bound(value) };
            return *it == value ? it : end();
        }

        bool contains(const T& value) const {
            return find(value) != end();
        }

        std::size_t count(const T& value) const {
            return std::distance(lower_bound(value), upper_bound(value));
        }

        iterator insert(const T& value) {
            auto result{ _lowerBound(value) };
            _node* node{ new _node{ result.parent, _head, _head, value, false } };
            if (result.left) {
                result.parent->left = node;
            }
            else {
                result.parent->right = node;
            }
            ++_size;
            return node;
        }

        template<std::input_iterator It>
        void insert(It first, It last) {
            while (first != last) {
                insert(*first++);
            }
        }

        void insert(std::initializer_list<T> list) {
            insert(list.begin(), list.end());
        }

        iterator erase(iterator pos) {
            _node* node{ pos._ptr };
            if (node->left->nil || node->right->nil) {
                _node* parent{ node->parent };
                _node* erased{ node->left->nil ? node->right : node->left };
                erased->parent = parent;
                if (parent->nil) {
                    _head->parent = erased;
                }
                else if (parent->left == node) {
                    parent->left = erased;
                }
                else {
                    parent->right = erased;
                }
                if (_head->left == node) {
                    _head->left = erased->nil ? parent : _leftmost(erased);
                }
                if (_head->right == node) {
                    _head->right = erased->nil ? parent : _rightmost(erased);
                }
                --_size;
                return ++pos;
            }

        }

        iterator erase(iterator first, iterator last) {
            while (first != last) {
                erase(first++);
            }
            return last;
        }

        std::size_t erase(const T& value) {
            auto [first, last] {equal_range()};
            erase(first, last);
        }

        template<class Cmp2>
        void merge(const binary_search_tree<T, Cmp2>& cont) {

        }

        bool operator==(const binary_search_tree& cont) const {
            return std::equal(begin(), end(), cont.begin(), cont.end());
        }

        auto operator<=>(const binary_search_tree& cont) const {
            return std::lexicographical_compare_three_way(begin(), end(), cont.begin(), cont.end());
        }

    };

    template<class It>
    explicit binary_search_tree(It, It)->binary_search_tree<std::iter_value_t<It>>;

}