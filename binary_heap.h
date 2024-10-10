#pragma once

#include "vector.h"

namespace plastic {

	template<class T, class Cmp = std::less<T>>
	class binary_heap {
		static constexpr Cmp _cmp{};

		vector<T> _data;

		void _siftUp(size_t index) {
			T value{ std::move(_data[index]) };
			while (index != 0) {
				size_t parent{ (index - 1) / 2 };
				if (!_cmp(_data[parent], value)) {
					break;
				}
				_data[index] = std::move(_data[parent]);
				index = parent;
			}
			_data[index] = std::move(value);
		}

		void _siftDown(size_t index) {
			T value{ std::move(_data[index]) };
			while (true) {
				size_t child{ index * 2 + 1 };
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

		void _siftUpDown(size_t index) {
			_siftUp(index);
			_siftDown(index);
		}

		void _makeHeap() {
			size_t i{ _data.size() >> 1 };
			while (i != 0) {
				_siftDown(--i);
			}
		}

	public:
		explicit binary_heap() : _data() {}

		template<std::input_iterator It>
		explicit binary_heap(It first, It last) : _data(first, last) {
			_makeHeap();
		}

		explicit binary_heap(std::initializer_list<T> list) : binary_heap(list.begin(), list.end()) {}

		bool empty() const {
			return _data.empty();
		}

		size_t size() const {
			return _data.size();
		}

		void clear() {
			return _data.clear();
		}

		T& top() {
			PLASTIC_VERIFY(!empty());
			return _data.front();
		}

		const T& top() const {
			PLASTIC_VERIFY(!empty());
			return _data.front();
		}

		void push(const T& value) {
			_data.push_back(value);
			_siftUp(size() - 1);
		}

		void pop() {
			PLASTIC_VERIFY(!empty());
			erase(0);
		}

		void merge(const binary_heap& heap) {
			_data.insert(_data.end(), heap._data.begin(), heap._data.end());
			_makeHeap();
		}

		void assign(size_t index, const T& value) {
			PLASTIC_VERIFY(index < size());
			_data[index] = value;
			_siftUpDown(index);
		}

		void erase(size_t index) {
			PLASTIC_VERIFY(index < size());
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