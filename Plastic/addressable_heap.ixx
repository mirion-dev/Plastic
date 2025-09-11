module;

#include <cassert>

export module plastic:addressable_heap;

import std;
import :linear_structure;

namespace plastic {

    export template <class T, class Pr = std::less<T>>
        class binary_heap {
        struct node {
            std::size_t index;
            T value;
        };

        Pr _pred;
        vector<node*> _data;

        void _set(std::size_t index, node* new_node) noexcept {
            _data[index] = new_node;
            _data[index]->index = index;
        }

        void _sift_up(std::size_t index) noexcept {
            node* temp{ _data[index] };
            while (index != 0) {
                std::size_t parent{ index - 1 >> 1 };
                if (!_pred(_data[parent]->value, temp->value)) {
                    break;
                }
                _set(index, _data[parent]);
                index = parent;
            }
            _set(index, temp);
        }

        void _sift_down(std::size_t index) noexcept {
            node* temp{ _data[index] };
            while (true) {
                std::size_t child{ (index << 1) + 1 };
                if (child >= size()) {
                    break;
                }
                if (child + 1 < size() && _pred(_data[child]->value, _data[child + 1]->value)) {
                    ++child;
                }
                if (!_pred(temp->value, _data[child]->value)) {
                    break;
                }
                _set(index, _data[child]);
                index = child;
            }
            _set(index, temp);
        }

        void _make_heap() noexcept {
            std::size_t i{ size() >> 1 };
            while (i-- != 0) {
                _sift_down(i);
            }
        }

        public:
            using difference_type = std::ptrdiff_t;
            using size_type = std::size_t;
            using value_type = T;
            using reference = T&;
            using const_reference = const T&;
            using comparator = Pr;

            class handle {
                friend binary_heap;

                node* _ptr{};

                handle(node* ptr) noexcept :
                    _ptr{ ptr } {
                }

            public:
                using value_type = T;
                using pointer = const T*;
                using reference = const T&;

                handle() noexcept = default;

                reference operator*() const noexcept {
                    return _ptr->value;
                }

                pointer operator->() const noexcept {
                    return &_ptr->value;
                }

                friend bool operator==(handle left, handle right) noexcept {
                    return left._ptr == right._ptr;
                }
            };

            binary_heap() noexcept = default;

            template <std::input_iterator It>
            binary_heap(It first, It last) noexcept {
                std::size_t index{};
                while (first != last) {
                    _data.push_back(new node{ index++, *first });
                    ++first;
                }

                _make_heap();
            }

            binary_heap(std::initializer_list<T> list) noexcept :
                binary_heap(list.begin(), list.end()) {
            }

            binary_heap(const binary_heap& other) noexcept :
                _pred{ other._pred },
                _data(other.size()) {

                auto i{ _data.begin() };
                for (node* j : other._data) {
                    *i++ = new node{ *j };
                }
            }

            binary_heap(binary_heap&& other) noexcept {
                swap(other);
            }

            ~binary_heap() noexcept {
                clear();
            }

            binary_heap& operator=(const binary_heap& other) noexcept {
                binary_heap temp(other);
                swap(temp);
                return *this;
            }

            binary_heap& operator=(binary_heap&& other) noexcept {
                swap(other);
                return *this;
            }

            void swap(binary_heap& other) noexcept {
                std::swap(_pred, other._pred);
                std::swap(_data, other._data);
            }

            friend void swap(binary_heap& left, binary_heap& right) noexcept {
                left.swap(right);
            }

            bool empty() const noexcept {
                return _data.empty();
            }

            size_type size() const noexcept {
                return _data.size();
            }

            static size_type max_size() noexcept {
                return std::numeric_limits<size_type>::max();
            }

            void clear() noexcept {
                for (node* i : _data) {
                    delete i;
                }
                _data.clear();
            }

            handle top_handle() const noexcept {
                assert(!empty());
                return _data[0];
            }

            const_reference top() const noexcept {
                assert(!empty());
                return _data[0]->value;
            }

            handle push(const_reference value) noexcept {
                std::size_t index{ size() };
                auto new_node{ new node{ index, value } };
                _data.push_back(new_node);
                _sift_up(index);
                return new_node;
            }

            void pop() noexcept {
                assert(!empty());
                delete _data[0];
                if (size() == 1) {
                    _data.pop_back();
                }
                else {
                    _set(0, _data.back());
                    _data.pop_back();
                    _sift_down(0);
                }
            }

            void merge(const binary_heap& other) noexcept {
                _data.reserve(size() + other.size());

                std::size_t index{ size() };
                for (node* i : other._data) {
                    _data.push_back(new node{ index++, i->value });
                }

                _make_heap();
            }

            void update(handle pos, const_reference new_value) noexcept {
                std::size_t index{ pos._ptr->index };
                assert(index < size());
                bool is_greater{ _pred(pos._ptr->value, new_value) };
                pos._ptr->value = new_value;
                if (is_greater) {
                    _sift_up(index);
                }
                else {
                    _sift_down(index);
                }
            }

            void erase(handle pos) noexcept {
                std::size_t index{ pos._ptr->index };
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