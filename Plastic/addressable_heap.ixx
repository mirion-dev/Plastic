module;

#include <cassert>

export module plastic:addressable_heap;

import std;

namespace plastic {

    export template <class T, class Pr = std::less<T>>
    class binary_heap {
    public:
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = T;

    private:
        struct node {
            size_type index;
            value_type value;
        };

    public:
        using const_reference = const T&;

        class reference {
            friend class handle;

            node* _ptr{};
            binary_heap* _cont{};

            reference(node* ptr, binary_heap* cont) :
                _ptr{ ptr },
                _cont{ cont } {}

        public:
            reference operator=(const value_type& other) {
                bool is_greater{ _cont->_pred(_ptr->value, other) };
                _ptr->value = other;
                if (is_greater) {
                    _cont->_sift_up(_ptr->index);
                }
                else {
                    _cont->_sift_down(_ptr->index);
                }
                return *this;
            }

            operator const_reference() {
                return _ptr->value;
            }
        };

        using const_handle = const T*;

        class handle {
            friend binary_heap;

        public:
            using value_type = T;
            using pointer = const T*;
            using reference = reference;

        private:
            node* _ptr{};
            binary_heap* _cont{};

            handle(node* ptr, binary_heap* cont) :
                _ptr{ ptr },
                _cont{ cont } {}

        public:
            handle() = default;

            reference operator*() const {
                return { _ptr, _cont };
            }

            pointer operator->() const {
                return &_ptr->value;
            }

            friend bool operator==(handle left, handle right) {
                return left._ptr == right._ptr;
            }

            operator const_handle() {
                return &_ptr->value;
            }
        };

        using comparator = Pr;

    private:
        Pr _pred;
        std::vector<node*> _data;

        void _set(size_type index, node* new_node) {
            _data[index] = new_node;
            _data[index]->index = index;
        }

        void _sift_up(size_type index) {
            node* ptr{ _data[index] };
            while (index != 0) {
                size_type parent{ index - 1 >> 1 };
                if (!_pred(_data[parent]->value, ptr->value)) {
                    break;
                }
                _set(index, _data[parent]);
                index = parent;
            }
            _set(index, ptr);
        }

        void _sift_down(size_type index) {
            node* ptr{ _data[index] };
            while (true) {
                size_type child{ (index << 1) + 1 };
                if (child >= size()) {
                    break;
                }
                if (child + 1 < size() && _pred(_data[child]->value, _data[child + 1]->value)) {
                    ++child;
                }
                if (!_pred(ptr->value, _data[child]->value)) {
                    break;
                }
                _set(index, _data[child]);
                index = child;
            }
            _set(index, ptr);
        }

        void _make_heap() {
            size_type i{ size() >> 1 };
            while (i-- != 0) {
                _sift_down(i);
            }
        }

    public:
        binary_heap() = default;

        template <std::input_iterator It>
        binary_heap(It first, It last) {
            size_type index{};
            while (first != last) {
                _data.push_back(new node{ index++, *first });
                ++first;
            }
            _make_heap();
        }

        binary_heap(std::initializer_list<T> list) :
            binary_heap(list.begin(), list.end()) {}

        binary_heap(const binary_heap& other) :
            _pred{ other._pred },
            _data(other.size()) {

            auto i{ _data.begin() };
            for (node* ptr : other._data) {
                *i++ = new node{ *ptr };
            }
        }

        binary_heap(binary_heap&& other) {
            swap(other);
        }

        ~binary_heap() {
            clear();
        }

        binary_heap& operator=(const binary_heap& other) {
            binary_heap temp(other);
            swap(temp);
            return *this;
        }

        binary_heap& operator=(binary_heap&& other) {
            swap(other);
            return *this;
        }

        void swap(binary_heap& other) {
            std::swap(_pred, other._pred);
            std::swap(_data, other._data);
        }

        friend void swap(binary_heap& left, binary_heap& right) {
            left.swap(right);
        }

        bool empty() const {
            return _data.empty();
        }

        size_type size() const {
            return _data.size();
        }

        static size_type max_size() {
            return std::numeric_limits<size_type>::max();
        }

        void clear() {
            for (node* ptr : _data) {
                delete ptr;
            }
            _data.clear();
        }

        handle apex() {
            assert(!empty());
            return { _data.front(), this };
        }

        const_handle apex() const {
            assert(!empty());
            return &_data.front()->value;
        }

        reference top() {
            assert(!empty());
            return *apex();
        }

        const_reference top() const {
            assert(!empty());
            return _data.front()->value;
        }

        handle push(const_reference value) {
            size_type index{ size() };
            auto new_node{ new node{ index, value } };
            _data.push_back(new_node);
            _sift_up(index);
            return { new_node, this };
        }

        void pop() {
            assert(!empty());
            delete _data.front();
            if (size() == 1) {
                _data.pop_back();
            }
            else {
                _set(0, _data.back());
                _data.pop_back();
                _sift_down(0);
            }
        }

        void merge(const binary_heap& other) {
            size_type index{ size() };
            _data.resize(size() + other.size());

            auto i{ _data.begin() + index };
            for (node* ptr : other._data) {
                *i++ = new node{ index++, ptr->value };
            }
            _make_heap();
        }

        void erase(handle pos) {
            size_type index{ pos._ptr->index };
            assert(index < size());
            if (index == size() - 1) {
                _data.pop_back();
            }
            else {
                _set(index, _data.back());
                _data.pop_back();
                if (_pred(pos._ptr->value, _data[index]->value)) {
                    _sift_up(index);
                }
                else {
                    _sift_down(index);
                }
            }
            delete pos._ptr;
        }
    };

    template <class It>
    binary_heap(It, It) -> binary_heap<std::iter_value_t<It>>;

}
