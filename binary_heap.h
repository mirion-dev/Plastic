#pragma once

#include "vector.h"

namespace plastic {

	template<class T, class Cmp = std::less<T>>
	class binary_heap : private vector<T> {
		using base = vector<T>;

		static constexpr Cmp _cmp{};

		void _siftUp(size_t index) {
			while (index != 0) {
				size_t parent{ (index - 1) >> 1 };
				if (!_cmp((*this)[parent], (*this)[index])) {
					return;
				}
				std::swap((*this)[index], (*this)[parent]);
				index = parent;
			}
		}

		void _siftDown(size_t index) {
			while (true) {
				size_t left{ (index << 1) + 1 };
				if (left >= base::size()) {
					return;
				}
				size_t right{ left + 1 }, swapped{ left };
				if (right < base::size() && _cmp((*this)[left], (*this)[right])) {
					swapped = right;
				}
				if (!_cmp((*this)[index], (*this)[swapped])) {
					return;
				}
				std::swap((*this)[index], (*this)[swapped]);
				index = swapped;
			}
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
			_siftUp(base::size() - 1);
		}

		void pop() {
			base::front() = std::move(base::back());
			base::pop_back();
			_siftDown(0);
		}
	};

}