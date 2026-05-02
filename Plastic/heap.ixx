module;

#include <cassert>

export module plastic.heap;

import std;

namespace plastic {

    export template <class T, class Pr = std::less<T>>
    class BinaryHeap {
    public:
        using value_type = T;
        using comparator = Pr;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

    private:
        struct Node {
            size_type index{};
            value_type value;
        };

    public:
        using const_reference = const value_type&;

        class reference {
            friend class handle;

            BinaryHeap* _cont;
            Node* _ptr;

            reference(BinaryHeap* cont, Node* ptr) :
                _cont{ cont },
                _ptr{ ptr } {}

        public:
            operator const_reference() const {
                return _ptr->value;
            }

            reference operator=(const_reference other) const {
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
            friend BinaryHeap;

            BinaryHeap* _cont{};
            Node* _ptr{};

            handle(BinaryHeap* cont, Node* ptr) :
                _cont{ cont },
                _ptr{ ptr } {}

        public:
            handle() = default;

            operator const_handle() const {
                return std::addressof(_ptr->value);
            }

            reference operator*() const {
                return { _cont, _ptr };
            }
        };

    private:
        comparator _pred;
        std::vector<std::unique_ptr<Node>> _data;

        void _set(size_type index, std::unique_ptr<Node> ptr) {
            ptr->index = index;
            _data[index] = std::move(ptr);
        }

        void _sift_up(size_type index) {
            std::unique_ptr<Node> i{ std::move(_data[index]) };
            while (index != 0) {
                size_type parent{ index - 1 >> 1 };
                if (!std::invoke(_pred, _data[parent]->value, i->value)) {
                    break;
                }
                this->_set(index, std::move(_data[parent]));
                index = parent;
            }
            this->_set(index, std::move(i));
        }

        void _sift_down(size_type index) {
            std::unique_ptr<Node> i{ std::move(_data[index]) };
            while (true) {
                size_type child{ (index << 1) + 1 };
                if (child >= size()) {
                    break;
                }
                if (child + 1 < size() && std::invoke(_pred, _data[child]->value, _data[child + 1]->value)) {
                    ++child;
                }
                if (!std::invoke(_pred, i->value, _data[child]->value)) {
                    break;
                }
                this->_set(index, std::move(_data[child]));
                index = child;
            }
            this->_set(index, std::move(i));
        }

        void _make_heap() {
            size_type i{ size() >> 1 };
            while (i-- != 0) {
                _sift_down(i);
            }
        }

    public:
        BinaryHeap() = default;

        template <std::input_iterator It>
        BinaryHeap(It first, It last) {
            size_type index{};
            while (first != last) {
                _data.emplace_back(std::make_unique<Node>(index++, *first));
                ++first;
            }
            _make_heap();
        }

        BinaryHeap(const BinaryHeap& other) :
            _pred{ other._pred },
            _data(other.size()) {

            auto i{ _data.begin() };
            for (auto& ptr : other._data) {
                *i++ = std::make_unique<Node>(*ptr);
            }
        }

        BinaryHeap(BinaryHeap&& other) noexcept {
            this->swap(other);
        }

        BinaryHeap(std::initializer_list<value_type> list) :
            BinaryHeap(list.begin(), list.end()) {}

        BinaryHeap& operator=(const BinaryHeap& other) {
            BinaryHeap temp(other);
            this->swap(temp);
            return *this;
        }

        BinaryHeap& operator=(BinaryHeap&& other) noexcept {
            this->swap(other);
            return *this;
        }

        BinaryHeap& operator=(std::initializer_list<value_type> list) {
            BinaryHeap temp(list);
            this->swap(temp);
            return *this;
        }

        handle apex() {
            assert(!empty());
            return { this, _data.front().get() };
        }

        const_handle apex() const {
            assert(!empty());
            return std::addressof(_data.front()->value);
        }

        const_handle capex() const {
            assert(!empty());
            return apex();
        }

        bool empty() const {
            return _data.empty();
        }

        size_type size() const {
            return _data.size();
        }

        size_type max_size() const {
            return static_cast<size_type>(-1) / sizeof(value_type);
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
            auto ptr{ std::make_unique<Node>(size(), value) };
            Node* raw{ ptr.get() };
            _data.emplace_back(std::move(ptr));
            this->_sift_up(raw->index);
            return { this, raw };
        }

        void pop() {
            assert(!empty());
            if (size() == 1) {
                _data.pop_back();
                return;
            }

            this->_set(0, std::move(_data.back()));
            _data.pop_back();
            _sift_down(0);
        }

        void erase(handle pos) {
            size_type index{ pos._ptr->index };
            assert(index < size());
            if (index == size() - 1) {
                _data.pop_back();
                return;
            }

            bool is_greater{ std::invoke(_pred, pos._ptr->value, _data.back()->value) };
            this->_set(index, std::move(_data.back()));
            _data.pop_back();
            if (is_greater) {
                _sift_up(index);
            }
            else {
                _sift_down(index);
            }
        }

        void swap(BinaryHeap& other) noexcept {
            std::ranges::swap(_pred, other._pred);
            std::ranges::swap(_data, other._data);
        }

        friend void swap(BinaryHeap& left, BinaryHeap& right) noexcept {
            left.swap(right);
        }

        void clear() {
            _data.clear();
        }

        void merge(BinaryHeap& other) {
            if (this == std::addressof(other)) {
                return;
            }

            size_type index{ size() };
            _data.resize(index + other.size());
            auto i{ _data.begin() + index };
            for (auto& ptr : other._data) {
                ptr->index = index++;
                *i++ = std::move(ptr);
            }
            other._data.clear();
            _make_heap();
        }
    };

    template <class It>
    BinaryHeap(It, It) -> BinaryHeap<std::iter_value_t<It>>;

}
