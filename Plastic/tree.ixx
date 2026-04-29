module;

#include <cassert>

export module plastic.tree;

import std;

namespace plastic {

    template <class T, class Pr, class Me>
    class Tree {
    public:
        using value_type = T;
        using comparator = Pr;
        using reference = value_type&;
        using const_reference = const value_type&;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

    protected:
        using Metadata = Me;

        struct NodeBase {
            NodeBase* parent{ this };
            NodeBase* left{ this };
            NodeBase* right{ this };
            bool is_head{ true };
            Metadata meta;

            NodeBase* leftmost() {
                NodeBase* i{ this };
                while (!i->left->is_head) {
                    i = i->left;
                }
                return i;
            }

            NodeBase* rightmost() {
                NodeBase* i{ this };
                while (!i->right->is_head) {
                    i = i->right;
                }
                return i;
            }

            void left_rotate() {
                NodeBase* replaced{ right };
                NodeBase* replaced_left{ replaced->left };

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
                NodeBase* replaced{ left };
                NodeBase* replaced_right{ replaced->right };

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

        struct Node : NodeBase {
            value_type value;

            NodeBase* clone(NodeBase* head, NodeBase* parent) {
                auto clone{ new Node{ parent, head, head, false, this->meta, value } };
                if (!this->left->is_head) {
                    clone->left = static_cast<Node*>(this->left)->clone(head, clone);
                }
                if (!this->right->is_head) {
                    clone->right = static_cast<Node*>(this->right)->clone(head, clone);
                }
                return clone;
            }

            void free() {
                if (!this->left->is_head) {
                    static_cast<Node*>(this->left)->free();
                }
                if (!this->right->is_head) {
                    static_cast<Node*>(this->right)->free();
                }
                delete this;
            }
        };

    public:
        class iterator {
            friend Tree;

            NodeBase* _ptr{};

            iterator(NodeBase* ptr) :
                _ptr{ ptr } {}

        public:
            using value_type = T;
            using pointer = const value_type*;
            using reference = const value_type&;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::bidirectional_iterator_tag;

            iterator() = default;

            reference operator*() const {
                return static_cast<Node*>(_ptr)->value;
            }

            pointer operator->() const {
                return std::addressof(static_cast<Node*>(_ptr)->value);
            }

            friend bool operator==(iterator left, iterator right) {
                return left._ptr == right._ptr;
            }

            iterator& operator++() {
                if (!_ptr->right->is_head) {
                    _ptr = _ptr->right->leftmost();
                }
                else {
                    NodeBase* old_ptr;
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
                    NodeBase* old_ptr;
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
        NodeBase* _head{ new NodeBase };
        size_type _size{};

    public:
        Tree() = default;

        Tree(const Tree& other) :
            _pred{ other._pred },
            _size{ other._size } {

            if (_size != 0) {
                NodeBase* clone{ static_cast<Node*>(other._head->parent)->clone(_head, _head) };
                _head->parent = clone;
                _head->left = clone->rightmost();
                _head->right = clone->leftmost();
            }
        }

        Tree(Tree&& other) noexcept {
            swap(other);
        }

        ~Tree() {
            clear();
            delete _head;
        }

        Tree& operator=(const Tree& other) {
            Tree temp(other);
            swap(temp);
            return *this;
        }

        Tree& operator=(Tree&& other) noexcept {
            swap(other);
            return *this;
        }

        void swap(Tree& other) noexcept {
            std::ranges::swap(_pred, other._pred);
            std::ranges::swap(_head, other._head);
            std::ranges::swap(_size, other._size);
        }

        friend void swap(Tree& left, Tree& right) noexcept {
            left.swap(right);
        }

        bool empty() const {
            return _size == 0;
        }

        size_type size() const {
            return _size;
        }

        void clear() {
            if (_size != 0) {
                static_cast<Node*>(_head->parent)->free();
                _head->parent = _head->left = _head->right = _head;
                _size = 0;
            }
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
            NodeBase *bound{ _head }, *i{ _head->parent };
            while (!i->is_head) {
                if (!std::invoke(_pred, static_cast<Node*>(i)->value, value)) {
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
            NodeBase *bound{ _head }, *i{ _head->parent };
            while (!i->is_head) {
                if (std::invoke(_pred, value, static_cast<Node*>(i)->value)) {
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
            NodeBase* bound{ lower_bound(value)._ptr };
            return !bound->is_head && !std::invoke(_pred, value, static_cast<Node*>(bound)->value) ? bound : _head;
        }

        bool contains(const_reference value) const {
            NodeBase* bound{ lower_bound(value)._ptr };
            return !bound->is_head && !std::invoke(_pred, value, static_cast<Node*>(bound)->value);
        }

        size_type count(const_reference value) const {
            auto [first, last]{ equal_range(value) };
            return std::ranges::distance(first, last);
        }

        iterator insert(this auto&& self, const_reference value) {
            NodeBase *parent{ self._head }, *i{ self._head->parent };
            bool is_left{};
            while (!i->is_head) {
                parent = i;
                is_left = std::invoke(self._pred, value, static_cast<Node*>(i)->value);
                i = is_left ? i->left : i->right;
            }

            auto new_node{ new Node{ parent, self._head, self._head, false, {}, value } };
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
            NodeBase* erased{ pos++._ptr };
            assert(erased != self._head);

            NodeBase* parent{ erased->parent };
            NodeBase* left{ erased->left };
            NodeBase* right{ erased->right };
            NodeBase* replaced;
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
                NodeBase* replaced_parent{ replaced->parent };
                NodeBase* replaced_right{ replaced->right };

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
            delete static_cast<Node*>(erased);
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

        void merge(this auto&& self, Tree& other) {
            if (std::addressof(self) == std::addressof(other)) {
                return;
            }

            self.insert(other.begin(), other.end());
            other.clear();
        }

        friend bool operator==(const Tree& left, const Tree& right) {
            return std::equal(left.begin(), left.end(), right.begin(), right.end());
        }

        friend auto operator<=>(const Tree& left, const Tree& right) {
            return std::lexicographical_compare_three_way(left.begin(), left.end(), right.begin(), right.end());
        }
    };

    struct RedBlackTreeMetadata {
        bool is_red{ true };
    };

    export template <class T, class Pr = std::less<T>>
    class RedBlackTree : public Tree<T, Pr, RedBlackTreeMetadata> {
        using Base = Tree<T, Pr, RedBlackTreeMetadata>;

        friend Base;

        using typename Base::NodeBase;

        void _insert_rebalance(NodeBase* inserted) {
            NodeBase* i{ inserted };
            while (i->parent->meta.is_red) {
                NodeBase* parent{ i->parent };
                NodeBase* grandparent{ parent->parent };
                NodeBase* uncle;
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

        void _erase_rebalance(NodeBase* replaced, NodeBase* erased) {
            std::ranges::swap(replaced->meta.is_red, erased->meta.is_red);

            NodeBase* i{ replaced };
            if (i->meta.is_red) {
                return;
            }

            while (!i->parent->is_head && !i->meta.is_red) {
                NodeBase* parent{ i->parent };
                NodeBase* brother;
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
        using Base::Base;
        using typename Base::value_type;

        template <std::input_iterator It>
        RedBlackTree(It first, It last) {
            this->insert(first, last);
        }

        RedBlackTree(std::initializer_list<value_type> list) :
            RedBlackTree(list.begin(), list.end()) {}
    };

    template <class It>
    RedBlackTree(It, It) -> RedBlackTree<std::iter_value_t<It>>;

    struct AvlTreeMetadata {
        signed char factor{};
    };

    export template <class T, class Pr = std::less<T>>
    class AvlTree : public Tree<T, Pr, AvlTreeMetadata> {
        using Base = Tree<T, Pr, AvlTreeMetadata>;

        friend Base;

        using typename Base::NodeBase;

        static void _insert_rebalance(NodeBase* inserted) {
            // TODO
        }

        static void _erase_rebalance(NodeBase* replaced, NodeBase* erased) {
            // TODO
        }

    public:
        using Base::Base;
        using typename Base::value_type;

        template <std::input_iterator It>
        AvlTree(It first, It last) {
            this->insert(first, last);
        }

        AvlTree(std::initializer_list<value_type> list) :
            AvlTree(list.begin(), list.end()) {}
    };

    template <class It>
    AvlTree(It, It) -> AvlTree<std::iter_value_t<It>>;

}
