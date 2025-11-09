module;

#include <cassert>

export module plastic:search_tree;

import std;

namespace plastic {

    template <class T, class Pr, class Me>
    class tree {
    public:
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;
        using comparator = Pr;

    protected:
        using metadata = Me;

        struct node_base {
            node_base* parent{ this };
            node_base* left{ this };
            node_base* right{ this };
            unsigned char is_head{ true };
            metadata meta{ is_head };

            void free() {
                if (is_head) {
                    return;
                }

                left->free();
                right->free();
                delete this;
            }

            node_base* leftmost() {
                node_base* i{ this };
                while (!i->left->is_head) {
                    i = i->left;
                }
                return i;
            }

            node_base* rightmost() {
                node_base* i{ this };
                while (!i->right->is_head) {
                    i = i->right;
                }
                return i;
            }

            void left_rotate() {
                node_base* replaced{ right };
                node_base* replaced_left{ replaced->left };

                right = replaced_left;
                if (!replaced_left->is_head) {
                    replaced_left->parent = this;
                }

                if (parent->is_head) {
                    parent->parent = replaced;
                }
                else if (this == parent->left) {
                    parent->left = replaced;
                }
                else {
                    parent->right = replaced;
                }
                replaced->parent = parent;

                replaced->left = this;
                parent = replaced;
            }

            void right_rotate() {
                node_base* replaced{ left };
                node_base* replaced_right{ replaced->right };

                left = replaced_right;
                if (!replaced_right->is_head) {
                    replaced_right->parent = this;
                }

                if (parent->is_head) {
                    parent->parent = replaced;
                }
                else if (this == parent->left) {
                    parent->left = replaced;
                }
                else {
                    parent->right = replaced;
                }
                replaced->parent = parent;

                replaced->right = this;
                parent = replaced;
            }
        };

        struct node : node_base {
            value_type value;

            node(node_base* head, node_base* parent, const_reference value) :
                node_base{ parent, head, head, false },
                value{ value } {}

            node_base* clone(node_base* head, node_base* parent) {
                auto tree{ new node{ *this } };
                tree->parent = parent;
                tree->left = tree->left->is_head ? head : static_cast<node*>(this->left)->clone(head, tree);
                tree->right = tree->right->is_head ? head : static_cast<node*>(this->right)->clone(head, tree);
                return tree;
            }
        };

    public:
        class iterator {
            friend tree;

            node_base* _ptr{};

            iterator(node_base* ptr) :
                _ptr{ ptr } {}

        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = const T*;
            using reference = const T&;
            using iterator_category = std::bidirectional_iterator_tag;

            iterator() = default;

            reference operator*() const {
                return static_cast<node*>(_ptr)->value;
            }

            pointer operator->() const {
                return &static_cast<node*>(_ptr)->value;
            }

            friend bool operator==(iterator left, iterator right) {
                return left._ptr == right._ptr;
            }

            iterator& operator++() {
                if (!_ptr->right->is_head) {
                    _ptr = _ptr->right->leftmost();
                }
                else {
                    node_base* old_ptr;
                    do {
                        old_ptr = std::exchange(_ptr, _ptr->parent);
                    } while (!_ptr->is_head && _ptr->right == old_ptr);
                }
                return *this;
            }

            iterator operator++(int) {
                iterator temp{ *this };
                ++*this;
                return temp;
            }

            iterator& operator--() {
                if (!_ptr->left->is_head) {
                    _ptr = _ptr->left->rightmost();
                }
                else {
                    node_base* old_ptr;
                    do {
                        old_ptr = std::exchange(_ptr, _ptr->parent);
                    } while (!_ptr->is_head && _ptr->left == old_ptr);
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

    protected:
        comparator _pred;
        node_base* _head{ new node_base };
        size_type _size{};

    public:
        tree() = default;

        tree(const tree& other) :
            _pred{ other._pred },
            _size{ other._size } {

            if (_size != 0) {
                node_base* tree{ static_cast<node*>(other._head->parent)->clone(_head, _head) };
                _head->parent = tree;
                _head->left = tree->rightmost();
                _head->right = tree->leftmost();
            }
        }

        tree(tree&& other) {
            swap(other);
        }

        ~tree() {
            clear();
            delete _head;
        }

        tree& operator=(const tree& other) {
            tree temp(other);
            swap(temp);
            return *this;
        }

        tree& operator=(tree&& other) {
            swap(other);
            return *this;
        }

        void swap(tree& other) {
            std::swap(_pred, other._pred);
            std::swap(_head, other._head);
            std::swap(_size, other._size);
        }

        friend void swap(tree& left, tree& right) {
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
            _head->parent->free();
            _head->parent = _head->left = _head->right = _head;
            _size = 0;
        }

        const_iterator begin() const {
            return _head->right;
        }

        const_iterator end() const {
            return _head;
        }

        const_iterator cbegin() const {
            return begin();
        }

        const_iterator cend() const {
            return end();
        }

        const_reverse_iterator rbegin() const {
            return reverse_iterator{ end() };
        }

        const_reverse_iterator rend() const {
            return reverse_iterator{ begin() };
        }

        const_reverse_iterator crbegin() const {
            return rbegin();
        }

        const_reverse_iterator crend() const {
            return rend();
        }

        const_reference front() const {
            return *begin();
        }

        const_reference back() const {
            return *--end();
        }

        const_iterator lower_bound(const_reference value) const {
            node_base *bound{ _head }, *i{ _head->parent };
            while (!i->is_head) {
                if (!_pred(static_cast<node*>(i)->value, value)) {
                    bound = i;
                    i = i->left;
                }
                else {
                    i = i->right;
                }
            }
            return bound;
        }

        const_iterator upper_bound(const_reference value) const {
            node_base *bound{ _head }, *i{ _head->parent };
            while (!i->is_head) {
                if (_pred(value, static_cast<node*>(i)->value)) {
                    bound = i;
                    i = i->left;
                }
                else {
                    i = i->right;
                }
            }
            return bound;
        }

        std::pair<const_iterator, const_iterator> equal_range(const_reference value) const {
            return { lower_bound(value), upper_bound(value) };
        }

        const_iterator find(const_reference value) const {
            node_base* bound{ lower_bound(value)._ptr };
            return !bound->is_head && !_pred(static_cast<node*>(bound)->value, value) ? bound : _head;
        }

        bool contains(const_reference value) const {
            node_base* bound{ lower_bound(value)._ptr };
            return !bound->is_head && !_pred(static_cast<node*>(bound)->value, value);
        }

        size_type count(const_reference value) const {
            auto [first, last]{ equal_range(value) };
            return std::distance(first, last);
        }

        iterator insert(this auto&& self, const_reference value) {
            node_base *parent{ self._head }, *i{ self._head->parent };
            bool is_left{};
            while (!i->is_head) {
                parent = i;
                is_left = self._pred(value, static_cast<node*>(i)->value);
                i = is_left ? i->left : i->right;
            }

            auto new_node{ new node{ self._head, parent, value } };
            if (parent->is_head) {
                parent->parent = new_node;
            }
            if (is_left) {
                parent->left = new_node;
                if (parent == self._head->right) {
                    self._head->right = new_node;
                }
            }
            else {
                parent->right = new_node;
                if (parent == self._head->left) {
                    self._head->left = new_node;
                }
            }

            self._insert_rebalance(new_node);
            ++self._size;
            return new_node;
        }

        template <std::input_iterator It>
        void insert(this auto&& self, It first, It last) {
            while (first != last) {
                self.insert(*first);
                ++first;
            }
        }

        void insert(this auto&& self, std::initializer_list<value_type> list) {
            self.insert(list.begin(), list.end());
        }

        iterator erase(this auto&& self, const_iterator pos) {
            node_base* erased{ pos++._ptr };
            assert(erased != self._head);

            node_base* parent{ erased->parent };
            node_base* left{ erased->left };
            node_base* right{ erased->right };
            node_base* replaced;
            if (left->is_head || right->is_head) {
                replaced = left->is_head ? right : left;

                if (parent->is_head) {
                    parent->parent = replaced;
                }
                else if (erased == parent->left) {
                    parent->left = replaced;
                }
                else {
                    parent->right = replaced;
                }
                if (!replaced->is_head) {
                    replaced->parent = parent;
                }

                if (erased == self._head->right) {
                    self._head->right = replaced->is_head ? parent : replaced->leftmost();
                }
                if (erased == self._head->left) {
                    self._head->left = replaced->is_head ? parent : replaced->rightmost();
                }
            }
            else {
                replaced = pos._ptr;
                node_base* replaced_parent{ replaced->parent };
                node_base* replaced_right{ replaced->right };

                replaced->left = left;
                left->parent = replaced;

                if (replaced != right) {
                    replaced_parent->left = replaced_right;
                    if (!replaced_right->is_head) {
                        replaced_right->parent = replaced_parent;
                    }

                    replaced->right = right;
                    right->parent = replaced;
                }

                if (parent->is_head) {
                    parent->parent = replaced;
                }
                else if (erased == parent->left) {
                    parent->left = replaced;
                }
                else {
                    parent->right = replaced;
                }
                replaced->parent = parent;
            }

            self._erase_rebalance(replaced, erased);
            delete erased;
            --self._size;
            return pos;
        }

        iterator erase(this auto&& self, const_iterator first, const_iterator last) {
            while (first != last) {
                self.erase(first++);
            }
            return last;
        }

        size_type erase(this auto&& self, const_reference value) {
            size_type count{};
            auto [first, last]{ self.equal_range(value) };
            while (first != last) {
                self.erase(first++);
                ++count;
            }
            return count;
        }

        friend bool operator==(const tree& left, const tree& right) {
            return std::equal(left.begin(), left.end(), right.begin(), right.end());
        }

        friend auto operator<=>(const tree& left, const tree& right) {
            return std::lexicographical_compare_three_way(left.begin(), left.end(), right.begin(), right.end());
        }
    };

    struct binary_search_tree_metadata {
        binary_search_tree_metadata(unsigned char) {}
    };

    export template <class T, class Pr = std::less<T>>
    class binary_search_tree : public tree<T, Pr, binary_search_tree_metadata> {
        using base = tree<T, Pr, binary_search_tree_metadata>;

        friend base;

        using typename base::node_base;

        void _insert_rebalance(node_base*) {}
        void _erase_rebalance(node_base*, node_base*) {}

    public:
        using base::base;
        using typename base::value_type;

        template <std::input_iterator It>
        binary_search_tree(It first, It last) {
            this->insert(first, last);
        }

        binary_search_tree(std::initializer_list<value_type> list) :
            binary_search_tree(list.begin(), list.end()) {}
    };

    template <class It>
    binary_search_tree(It, It) -> binary_search_tree<std::iter_value_t<It>>;

    struct red_black_tree_metadata {
        unsigned char is_red{ false };

        red_black_tree_metadata(unsigned char is_head) :
            is_red{ is_head } {}
    };

    export template <class T, class Pr = std::less<T>>
    class red_black_tree : public tree<T, Pr, red_black_tree_metadata> {
        using base = tree<T, Pr, red_black_tree_metadata>;

        friend base;

        using typename base::node_base;

        void _insert_rebalance(node_base* inserted) {
            node_base* i{ inserted };
            while (i->parent->meta.is_red) {
                node_base* parent{ i->parent };
                node_base* grandparent{ parent->parent };
                node_base* uncle;
                if (parent == grandparent->left) {
                    uncle = grandparent->right;
                    if (!uncle->meta.is_red) {
                        if (i == parent->right) {
                            i = parent;
                            i->left_rotate();
                        }

                        i->parent->meta.is_red = false;
                        i->parent->parent->meta.is_red = true;
                        i->parent->parent->right_rotate();
                        break;
                    }
                }
                else {
                    uncle = grandparent->left;
                    if (!uncle->meta.is_red) {
                        if (i == parent->left) {
                            i = parent;
                            i->right_rotate();
                        }

                        i->parent->meta.is_red = false;
                        i->parent->parent->meta.is_red = true;
                        i->parent->parent->left_rotate();
                        break;
                    }
                }

                parent->meta.is_red = uncle->meta.is_red = false;
                grandparent->meta.is_red = true;
                i = grandparent;
            }

            this->_head->parent->meta.is_red = false;
        }

        void _erase_rebalance(node_base* replaced, node_base* erased) {
            std::swap(replaced->meta.is_red, erased->meta.is_red);

            node_base* i{ replaced };
            if (i->meta.is_red) {
                return;
            }

            while (!i->parent->is_head && !i->meta.is_red) {
                node_base* parent{ i->parent };
                node_base* brother;
                if (i == parent->left) {
                    brother = parent->right;
                    if (brother->meta.is_red) {
                        brother->meta.is_red = false;
                        parent->meta.is_red = true;
                        parent->left_rotate();
                        brother = parent->right;
                    }

                    if (brother->left->meta.is_red || brother->right->meta.is_red) {
                        if (!brother->right->meta.is_red) {
                            brother->left->meta.is_red = false;
                            brother->meta.is_red = true;
                            brother->right_rotate();
                            brother = parent->right;
                        }

                        brother->meta.is_red = parent->meta.is_red;
                        parent->meta.is_red = false;
                        brother->right->meta.is_red = false;
                        parent->left_rotate();
                        break;
                    }
                }
                else {
                    brother = parent->left;
                    if (brother->meta.is_red) {
                        brother->meta.is_red = false;
                        parent->meta.is_red = true;
                        parent->right_rotate();
                        brother = parent->left;
                    }

                    if (brother->right->meta.is_red || brother->left->meta.is_red) {
                        if (!brother->left->meta.is_red) {
                            brother->right->meta.is_red = false;
                            brother->meta.is_red = true;
                            brother->left_rotate();
                            brother = parent->left;
                        }

                        brother->meta.is_red = parent->meta.is_red;
                        parent->meta.is_red = false;
                        brother->left->meta.is_red = false;
                        parent->right_rotate();
                        break;
                    }
                }

                brother->meta.is_red = true;
                i = parent;
            }

            i->meta.is_red = false;
        }

    public:
        using base::base;
        using typename base::value_type;

        template <std::input_iterator It>
        red_black_tree(It first, It last) {
            this->insert(first, last);
        }

        red_black_tree(std::initializer_list<value_type> list) :
            red_black_tree(list.begin(), list.end()) {}
    };

    template <class It>
    red_black_tree(It, It) -> red_black_tree<std::iter_value_t<It>>;

    struct avl_tree_metadata {
        signed char factor{};

        avl_tree_metadata(unsigned char) {}
    };

    export template <class T, class Pr = std::less<T>>
    class avl_tree : public tree<T, Pr, avl_tree_metadata> {
        using base = tree<T, Pr, avl_tree_metadata>;

        friend base;

        using typename base::node_base;

        void _insert_rebalance(node_base* inserted) {
            // TODO
        }

        void _erase_rebalance(node_base* replaced, node_base* erased) {
            // TODO
        }

    public:
        using base::base;
        using typename base::value_type;

        template <std::input_iterator It>
        avl_tree(It first, It last) {
            this->insert(first, last);
        }

        avl_tree(std::initializer_list<value_type> list) :
            avl_tree(list.begin(), list.end()) {}
    };

    template <class It>
    avl_tree(It, It) -> avl_tree<std::iter_value_t<It>>;

}
