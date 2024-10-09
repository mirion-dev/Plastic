#pragma once

#include "vector.h"

namespace plastic {

	template<class T, class Cmp = std::less<T>>
	class binary_heap : private vector<T> {
		using base = vector<T>;

		static constexpr Cmp _cmp{};

		void _siftUp(size_t index) {
			T value{ std::move((*this)[index]) };
			while (index != 0) {
				size_t parent{ (index - 1) / 2 };
				if (!_cmp((*this)[parent], value)) {
					break;
				}
				(*this)[index] = std::move((*this)[parent]);
				index = parent;
			}
			(*this)[index] = std::move(value);
		}

		void _siftDown(size_t index) {
			T value{ std::move((*this)[index]) };
			while (index < size() / 2) {
				size_t child{ index * 2 + 1 };
				if (child + 1 < size() && _cmp((*this)[child], (*this)[child + 1])) {
					++child;
				}
				if (!_cmp(value, (*this)[child])) {
					break;
				}
				(*this)[index] = std::move((*this)[child]);
				index = child;
			}
			(*this)[index] = std::move(value);
		}

	public:
		using base::empty;
		using base::clear;
		using base::size;
		using base::capacity;
		using base::reserve;

		explicit binary_heap() : base() {}

		template<std::input_iterator It>
		explicit binary_heap(It first, It last) : base(first, last) {
			size_t i{ (size_t)std::distance(first, last) >> 1 };
			while (i != 0) {
				_siftDown(--i);
			}
		}

		explicit binary_heap(std::initializer_list<T> list) : binary_heap(list.begin(), list.end()) {}

		T& top() {
			return base::front();
		}

		const T& top() const {
			return base::front();
		}

		void push(const T& value) {
			base::push_back(value);
			_siftUp(size() - 1);
		}

		void pop() {
			base::front() = std::move(base::back());
			base::pop_back();
			if (!empty()) {
				_siftDown(0);
			}
		}
	};

}