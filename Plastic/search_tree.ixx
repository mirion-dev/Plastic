module;

#include <cassert>

export module plastic:search_tree;

import std;

namespace plastic {

    template <class Nd>
    struct basic_binary_search_tree_node {
    private:
        Nd* _this() {
            return static_cast<Nd*>(this);
        }

    public:
        Nd* parent;
        Nd* left;
        Nd* right;
        unsigned char is_head; // `sizeof(bool)` is unspecified
        // <other metadata>;
        // T value;

        void free() {
            if (is_head) {
                return;
            }

            left->free();
            right->free();
            delete _this();
        }

        Nd* leftmost() {
            auto i{ _this() };
            while (!i->left->is_head) {
                i = i->left;
            }
            return i;
        }

        Nd* rightmost() {
            auto i{ _this() };
            while (!i->right->is_head) {
                i = i->right;
            }
            return i;
        }

        void left_rotate() {
            Nd* replaced{ right };
            Nd* replaced_left{ replaced->left };

            right = replaced_left;
            if (!replaced_left->is_head) {
                replaced_left->parent = _this();
            }

            if (parent->is_head) {
                parent->parent = replaced;
            }
            else if (_this() == parent->left) {
                parent->left = replaced;
            }
            else {
                parent->right = replaced;
            }
            replaced->parent = parent;

            replaced->left = _this();
            parent = replaced;
        }

        void right_rotate() {
            Nd* replaced{ left };
            Nd* replaced_right{ replaced->right };

            left = replaced_right;
            if (!replaced_right->is_head) {
                replaced_right->parent = _this();
            }

            if (parent->is_head) {
                parent->parent = replaced;
            }
            else if (_this() == parent->left) {
                parent->left = replaced;
            }
            else {
                parent->right = replaced;
            }
            replaced->parent = parent;

            replaced->right = _this();
            parent = replaced;
        }
    };

    template <class Nd, class Pr>
    class basic_binary_search_tree {
        using T = decltype(Nd::value);

    protected:
        Pr _pred;
        Nd* _head; // `parent` is the root, `left` is the maximum and `right` is the minimum
        std::size_t _size;

    public:
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;
        using comparator = Pr;

        class iterator {
            friend basic_binary_search_tree;

            Nd* _ptr{};

            iterator(Nd* ptr) :
                _ptr{ ptr } {}

        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = const T*;
            using reference = const T&;
            using iterator_category = std::bidirectional_iterator_tag;

            iterator() = default;

            reference operator*() const {
                return _ptr->value;
            }

            pointer operator->() const {
                return &_ptr->value;
            }

            friend bool operator==(iterator left, iterator right) {
                return left._ptr == right._ptr;
            }

            iterator& operator++() {
                if (!_ptr->right->is_head) {
                    _ptr = _ptr->right->leftmost();
                }
                else {
                    Nd* old_ptr;
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
                    Nd* old_ptr;
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

        basic_binary_search_tree() :
            _head{ new Nd },
            _size{} {

            _head->construct_head();
        }

        basic_binary_search_tree(const basic_binary_search_tree& other) :
            basic_binary_search_tree() {

            _pred = other._pred;
            _size = other._size;

            Nd* tree{ other._head->parent->clone(_head, _head) };
            _head->parent = tree;
            _head->left = tree->rightmost();
            _head->right = tree->leftmost();
        }

        basic_binary_search_tree(basic_binary_search_tree&& other) {
            swap(other);
        }

        ~basic_binary_search_tree() {
            clear();
            delete _head;
        }

        basic_binary_search_tree& operator=(const basic_binary_search_tree& other) {
            basic_binary_search_tree temp(other);
            swap(temp);
            return *this;
        }

        basic_binary_search_tree& operator=(basic_binary_search_tree&& other) {
            swap(other);
            return *this;
        }

        void swap(basic_binary_search_tree& other) {
            std::swap(_pred, other._pred);
            std::swap(_head, other._head);
            std::swap(_size, other._size);
        }

        friend void swap(basic_binary_search_tree& left, basic_binary_search_tree& right) {
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

        const_iterator lower_bound(const_reference value) const {
            Nd *bound{ _head }, *i{ _head->parent };
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

        const_iterator upper_bound(const_reference value) const {
            Nd *bound{ _head }, *i{ _head->parent };
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

        std::pair<const_iterator, const_iterator> equal_range(const_reference value) const {
            return { lower_bound(value), upper_bound(value) };
        }

        const_iterator find(const_reference value) const {
            Nd* bound{ lower_bound(value)._ptr };
            return !bound->is_head && !_pred(bound->value, value) ? bound : _head;
        }

        bool contains(const_reference value) const {
            Nd* bound{ lower_bound(value)._ptr };
            return !bound->is_head && !_pred(bound->value, value);
        }

        size_type count(const_reference value) const {
            auto [first, last]{ equal_range(value) };
            return std::distance(first, last);
        }

        iterator insert(this auto&& self, const_reference value) {
            Nd *parent{ self._head }, *i{ self._head->parent };
            bool is_left{};
            while (!i->is_head) {
                parent = i;
                is_left = self._pred(value, i->value);
                i = is_left ? i->left : i->right;
            }

            auto new_node{ new Nd };
            new_node->construct(self._head, parent, value);

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

        void insert(this auto&& self, std::initializer_list<T> list) {
            self.insert(list.begin(), list.end());
        }

        iterator erase(this auto&& self, const_iterator pos) {
            Nd* erased{ pos++._ptr };
            assert(erased != self._head);

            Nd* parent{ erased->parent };
            Nd* left{ erased->left };
            Nd* right{ erased->right };
            Nd* replaced;
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
                Nd* replaced_parent{ replaced->parent };
                Nd* replaced_right{ replaced->right };

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
            std::size_t count{};
            auto [first, last]{ self.equal_range(value) };
            while (first != last) {
                self.erase(first++);
                ++count;
            }
            return count;
        }

        friend bool operator==(const basic_binary_search_tree& left, const basic_binary_search_tree& right) {
            return std::equal(left.begin(), left.end(), right.begin(), right.end());
        }

        friend auto operator<=>(const basic_binary_search_tree& left, const basic_binary_search_tree& right) {
            return std::lexicographical_compare_three_way(left.begin(), left.end(), right.begin(), right.end());
        }
    };

    template <class T>
    struct binary_search_tree_node : basic_binary_search_tree_node<binary_search_tree_node<T>> {
    private:
        using Nd = binary_search_tree_node;

    public:
        T value;

        Nd* clone(Nd* head, Nd* parent) {
            if (this->is_head) {
                return head;
            }

            auto tree{ new Nd };
            new(tree) Nd{ parent, this->left->clone(head, tree), this->right->clone(head, tree), false, value };
            return tree;
        }

        void construct_head() {
            this->parent = this->left = this->right = this;
            this->is_head = true;
        }

        void construct(Nd* head, Nd* parent, const T& value) {
            new(this) Nd{ parent, head, head, false, value };
        }
    };

    export template <class T, class Pr = std::less<T>>
    class binary_search_tree : public basic_binary_search_tree<binary_search_tree_node<T>, Pr> {
        using Nd = binary_search_tree_node<T>;
        using base = basic_binary_search_tree<Nd, Pr>;

        friend base;

        static void _insert_rebalance(Nd* inserted) {}

        static void _erase_rebalance(Nd* replaced, Nd* erased) {}

    public:
        using base::base;

        template <std::input_iterator It>
        binary_search_tree(It first, It last) :
            binary_search_tree() {

            this->insert(first, last);
        }

        binary_search_tree(std::initializer_list<T> list) :
            binary_search_tree(list.begin(), list.end()) {}
    };

    template <class It>
    binary_search_tree(It, It) -> binary_search_tree<std::iter_value_t<It>>;

    template <class T>
    struct red_black_tree_node : basic_binary_search_tree_node<red_black_tree_node<T>> {
    private:
        using Nd = red_black_tree_node;

    public:
        unsigned char is_red;
        T value;

        Nd* clone(Nd* head, Nd* parent) {
            if (this->is_head) {
                return head;
            }

            auto tree{ new Nd };
            new(tree) Nd{ parent, this->left->clone(head, tree), this->right->clone(head, tree), false, is_red, value };
            return tree;
        }

        void construct_head() {
            this->parent = this->left = this->right = this;
            this->is_head = true;
            this->is_red = false;
        }

        void construct(Nd* head, Nd* parent, const T& value) {
            new(this) Nd{ parent, head, head, false, true, value };
        }
    };

    export template <class T, class Pr = std::less<T>>
    class red_black_tree : public basic_binary_search_tree<red_black_tree_node<T>, Pr> {
        using Nd = red_black_tree_node<T>;
        using base = basic_binary_search_tree<Nd, Pr>;

        friend base;

        void _insert_rebalance(Nd* inserted) {
            Nd* i{ inserted };
            while (i->parent->is_red) {
                Nd* parent{ i->parent };
                Nd* grandparent{ parent->parent };
                Nd* uncle;
                if (parent == grandparent->left) {
                    uncle = grandparent->right;
                    if (!uncle->is_red) {
                        if (i == parent->right) {
                            i = parent;
                            i->left_rotate();
                        }

                        i->parent->is_red = false;
                        i->parent->parent->is_red = true;
                        i->parent->parent->right_rotate();
                        break;
                    }
                }
                else {
                    uncle = grandparent->left;
                    if (!uncle->is_red) {
                        if (i == parent->left) {
                            i = parent;
                            i->right_rotate();
                        }

                        i->parent->is_red = false;
                        i->parent->parent->is_red = true;
                        i->parent->parent->left_rotate();
                        break;
                    }
                }

                parent->is_red = uncle->is_red = false;
                grandparent->is_red = true;
                i = grandparent;
            }

            this->_head->parent->is_red = false;
        }

        void _erase_rebalance(Nd* replaced, Nd* erased) {
            std::swap(replaced->is_red, erased->is_red);

            Nd* i{ replaced };
            if (i->is_red) {
                return;
            }

            while (!i->parent->is_head && !i->is_red) {
                Nd* parent{ i->parent };
                Nd* brother;
                if (i == parent->left) {
                    brother = parent->right;
                    if (brother->is_red) {
                        brother->is_red = false;
                        parent->is_red = true;
                        parent->left_rotate();
                        brother = parent->right;
                    }

                    if (brother->left->is_red || brother->right->is_red) {
                        if (!brother->right->is_red) {
                            brother->left->is_red = false;
                            brother->is_red = true;
                            brother->right_rotate();
                            brother = parent->right;
                        }

                        brother->is_red = parent->is_red;
                        parent->is_red = false;
                        brother->right->is_red = false;
                        parent->left_rotate();
                        break;
                    }
                }
                else {
                    brother = parent->left;
                    if (brother->is_red) {
                        brother->is_red = false;
                        parent->is_red = true;
                        parent->right_rotate();
                        brother = parent->left;
                    }

                    if (brother->right->is_red || brother->left->is_red) {
                        if (!brother->left->is_red) {
                            brother->right->is_red = false;
                            brother->is_red = true;
                            brother->left_rotate();
                            brother = parent->left;
                        }

                        brother->is_red = parent->is_red;
                        parent->is_red = false;
                        brother->left->is_red = false;
                        parent->right_rotate();
                        break;
                    }
                }

                brother->is_red = true;
                i = parent;
            }

            i->is_red = false;
        }

    public:
        using base::base;

        template <std::input_iterator It>
        red_black_tree(It first, It last) :
            red_black_tree() {

            this->insert(first, last);
        }

        red_black_tree(std::initializer_list<T> list) :
            red_black_tree(list.begin(), list.end()) {}
    };

    template <class It>
    red_black_tree(It, It) -> red_black_tree<std::iter_value_t<It>>;

    template <class T>
    struct avl_tree_node : basic_binary_search_tree_node<avl_tree_node<T>> {
    private:
        using Nd = avl_tree_node;

    public:
        signed char factor;
        T value;

        Nd* clone(Nd* head, Nd* parent) {
            if (this->is_head) {
                return head;
            }

            auto tree{ new Nd };
            new(tree) Nd{ parent, this->left->clone(head, tree), this->right->clone(head, tree), false, factor, value };
            return tree;
        }

        void construct_head() {
            this->parent = this->left = this->right = this;
            this->is_head = true;
            this->factor = 0;
        }

        void construct(Nd* head, Nd* parent, const T& value) {
            new(this) Nd{ parent, head, head, false, 0, value };
        }
    };

    export template <class T, class Pr = std::less<T>>
    class avl_tree : public basic_binary_search_tree<avl_tree_node<T>, Pr> {
        using Nd = avl_tree_node<T>;
        using base = basic_binary_search_tree<Nd, Pr>;

        friend base;

        void _insert_rebalance(Nd* inserted) {
            // TODO
        }

        void _erase_rebalance(Nd* replaced, Nd* erased) {
            // TODO
        }

    public:
        using base::base;

        template <std::input_iterator It>
        avl_tree(It first, It last) :
            avl_tree() {

            this->insert(first, last);
        }

        avl_tree(std::initializer_list<T> list) :
            avl_tree(list.begin(), list.end()) {}
    };

    template <class It>
    avl_tree(It, It) -> avl_tree<std::iter_value_t<It>>;

}
