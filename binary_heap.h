#pragma once

#include "vector.h"

namespace plastic {

    template<class T, class Cmp = std::less<T>>
    class binary_heap {
        static constexpr Cmp _cmp{};

        vector<T> _data;

        void _siftUp(std::size_t index) {
            T value{ std::move(_data[index]) };
            while (index != 0) {
                std::size_t parent{ (index - 1) / 2 };
                if (!_cmp(_data[parent], value)) {
                    break;
                }
                _data[index] = std::move(_data[parent]);
                index = parent;
            }
            _data[index] = std::move(value);
        }

        void _siftDown(std::size_t index) {
            T value{ std::move(_data[index]) };
            while (true) {
                std::size_t child{ index * 2 + 1 };
                if (child >= _data.size()) {
                    break;
                }
                if (child + 1 < _data.size() && _cmp(_data[child], _data[child + 1])) {
                    ++child;
                }
                if (!_cmp(value, _data[child])) {
                    break;
                }
                _data[index] = std::move(_data[child]);
                index = child;
            }
            _data[index] = std::move(value);
        }

        void _siftUpDown(std::size_t index) {
            _siftUp(index);
            _siftDown(index);
        }

        void _makeHeap() {
            std::size_t i{ _data.size() >> 1 };
            while (i != 0) {
                _siftDown(--i);
            }
        }

    public:
        explicit binary_heap(std::size_t size = {}, const T& value = {}) : _data(size, value) {}

        template<std::input_iterator It>
        explicit binary_heap(It first, It last) : _data(first, last) {
            _makeHeap();
        }

        explicit binary_heap(std::initializer_list<T> list) : binary_heap(list.begin(), list.end()) {}

        bool empty() const {
            return _data.empty();
        }

        std::size_t size() const {
            return _data.size();
        }

        void clear() {
            return _data.clear();
        }

        T& top() {
            assert(!empty());
            return _data.front();
        }

        const T& top() const {
            assert(!empty());
            return _data.front();
        }

        void push(const T& value) {
            _data.push_back(value);
            _siftUp(size() - 1);
        }

        void pop() {
            assert(!empty());
            erase(0);
        }

        void merge(const binary_heap& heap) {
            _data.insert(_data.end(), heap._data.begin(), heap._data.end());
            _makeHeap();
        }

        void assign(std::size_t index, const T& value) {
            assert(index < size());
            _data[index] = value;
            _siftUpDown(index);
        }

        void erase(std::size_t index) {
            assert(index < size());
            _data[index] = std::move(_data.back());
            _data.pop_back();
            if (!empty()) {
                _siftUpDown(index);
            }
        }

    };

    template<class It>
    explicit binary_heap(It, It)->binary_heap<std::iter_value_t<It>>;

}