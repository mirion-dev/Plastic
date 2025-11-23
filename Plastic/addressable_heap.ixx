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
        using pointer = T*;
        using const_pointer = const T*;
        using comparator = Pr;

    private:
        struct node {
            size_type index{};
            value_type value;
        };

    public:
        using const_reference = const T&;

        class reference {
            friend class handle;

            node* _ptr;
            binary_heap* _cont;

            reference(node* ptr, binary_heap* cont) :
                _ptr{ ptr },
                _cont{ cont } {}

        public:
            operator const_reference() const {
                return _ptr->value;
            }

            reference operator=(const value_type& other) const {
                bool is_greater{ std::invoke(_cont->_pred, _ptr->value, other) };
                _ptr->value = other;
                if (is_greater) {
                    _cont->_sift_up(_ptr->index);
                }
                else {
                    _cont->_sift_down(_ptr->index);
                }
                return *this;
            }
        };

        using const_handle = const_pointer;

        class handle {
            friend binary_heap;

            node* _ptr{};
            binary_heap* _cont{};

            handle(node* ptr, binary_heap* cont) :
                _ptr{ ptr },
                _cont{ cont } {}

        public:
            handle() = default;

            operator const_handle() const {
                return &_ptr->value;
            }

            reference operator*() const {
                return { _ptr, _cont };
            }
        };

    private:
        comparator _pred;
        std::vector<node*> _data;

        void _set(size_type index, node* new_node) {
            _data[index] = new_node;
            new_node->index = index;
        }

        void _sift_up(size_type index) {
            node* ptr{ _data[index] };
            while (index != 0) {
                size_type parent{ index - 1 >> 1 };
                if (!std::invoke(_pred, _data[parent]->value, ptr->value)) {
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
                if (child + 1 < size() && std::invoke(_pred, _data[child]->value, _data[child + 1]->value)) {
                    ++child;
                }
                if (!std::invoke(_pred, ptr->value, _data[child]->value)) {
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

        binary_heap(std::initializer_list<value_type> list) :
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

        const_handle capex() const {
            assert(!empty());
            return apex();
        }

        reference top() {
            assert(!empty());
            return *apex();
        }

        const_reference top() const {
            assert(!empty());
            return *apex();
        }

        handle push(const_reference value) {
            auto new_node{ new node{ size(), value } };
            _data.push_back(new_node);
            _sift_up(new_node->index);
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

        void erase(handle pos) {
            auto& [index, value]{ *pos._ptr };
            assert(index < size());
            if (index == size() - 1) {
                _data.pop_back();
            }
            else {
                _set(index, _data.back());
                _data.pop_back();
                if (std::invoke(_pred, value, _data[index]->value)) {
                    _sift_up(index);
                }
                else {
                    _sift_down(index);
                }
            }
            delete pos._ptr;
        }

        void merge(binary_heap& other) {
            size_type index{ size() };
            _data.resize(index + other.size());

            auto i{ _data.begin() + index };
            for (node* ptr : other._data) {
                ptr->index = index++;
                *i++ = ptr;
            }
            other._data.clear();

            _make_heap();
        }
    };

    template <class It>
    binary_heap(It, It) -> binary_heap<std::iter_value_t<It>>;

}
