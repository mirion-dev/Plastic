#pragma once

#include "vector.h"

namespace plastic {

	template<class T, ::std::strict_weak_order<T, T> compare = ::std::less<>>
	class priority_queue : private vector<T> {
		static constexpr compare _compare{};

		void _siftUp(::std::size_t index) noexcept {
			while (index != 0) {
				::std::size_t parent{(index - 1) >> 1};
				if (!_compare((*this)[parent], (*this)[index])) {
					return;
				}
				::std::swap((*this)[index], (*this)[parent]);
				index = parent;
			}
		}

		void _siftDown(::std::size_t index) noexcept {
			::std::size_t size{this->size()};
			while (true) {
				::std::size_t left{(index << 1) + 1};
				if (left >= size) {
					return;
				}
				::std::size_t right{left + 1}, max{left};
				if (right < size && _compare((*this)[left], (*this)[right])) {
					max = right;
				}
				if (!_compare((*this)[index], (*this)[max])) {
					return;
				}
				::std::swap((*this)[index], (*this)[max]);
				index = max;
			}
		}

	public:
		using vector<T>::empty;
		using vector<T>::size;
		using vector<T>::reserve;

		T& top() noexcept {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				::std::abort();
			}
#endif
			return this->front();
		}

		void push(const T& value) noexcept {
			::std::size_t i{this->size()};
			this->push_back(value);
			_siftUp(i);
		}

		void pop() noexcept {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				::std::abort();
			}
#endif
			this->front() = ::std::move(this->back());
			this->pop_back();
			_siftDown(0);
		}
	};

}