#pragma once

#include "vector.h"

namespace plastic {

    template<class T, class Cmp = std::less<T>>
    class binary_heap {
        static constexpr Cmp _cmp{};

        plastic::vector<T> _data;

        void _sift_up(std::size_t index) noexcept {
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

        void _sift_down(std::size_t index) noexcept {
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

        void _sift_up_down(std::size_t index) noexcept {
            _sift_up(index);
            _sift_down(index);
        }

        void _make_heap() noexcept {
            std::size_t i{ _data.size() / 2 };
            while (i-- != 0) {
                _sift_down(i);
            }
        }

    public:
        explicit binary_heap() noexcept = default;

        template<std::input_iterator It>
        explicit binary_heap(It first, It last) noexcept :
            _data(first, last) {

            _make_heap();
        }

        explicit binary_heap(std::initializer_list<T> list) noexcept :
            binary_heap(list.begin(), list.end()) {}

        bool empty() const noexcept {
            return _data.empty();
        }

        std::size_t size() const noexcept {
            return _data.size();
        }

        void clear() noexcept {
            return _data.clear();
        }

        const T& top() const noexcept {
            assert(!_data.empty());
            return _data.front();
        }

        // only for test purposes
        T* data() noexcept {
            return _data.data();
        }

        void push(const T& value) noexcept {
            _data.push_back(value);
            _sift_up(_data.size() - 1);
        }

        void pop() noexcept {
            assert(!_data.empty());
            _data.front() = std::move(_data.back());
            _data.pop_back();
            _sift_down(0);
        }

        void merge(const binary_heap& other) noexcept {
            _data.insert(_data.end(), other._data.begin(), other._data.end());
            _make_heap();
        }

        void assign(T* ptr, const T& value) noexcept {
            *ptr = value;
            _sift_up_down(ptr - _data.data());
        }

        void erase(T* ptr) noexcept {
            *ptr = std::move(_data.back());
            _data.pop_back();
            if (!_data.empty()) {
                _sift_up_down(ptr - _data.data());
            }
        }
    };

    template<class It>
    explicit plastic::binary_heap(It, It)->plastic::binary_heap<std::iter_value_t<It>>;

}