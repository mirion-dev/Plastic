#pragma once

#include "vector.h"

namespace plastic {

	template<class T, ::std::strict_weak_order<T, T> compare = ::std::less<>>
	class priority_queue : private vector<T> {
		static constexpr compare _compare{};

		void _siftUp(::std::size_t index) const noexcept {
			while (index != 0) {
				::std::size_t parent{(index - 1) >> 1};
				if (!_compare((*this)[parent], (*this)[index])) {
					return;
				}
				::std::swap((*this)[index], (*this)[parent]);
				index = parent;
			}
		}

		void _siftDown(::std::size_t index) const noexcept {
			::std::size_t size{this->size()};
			while (true) {
				::std::size_t left{(index << 1) + 1};
				if (left >= size) {
					return;
				}
				::std::size_t right{left + 1}, swapped{left};
				if (right < size && _compare((*this)[left], (*this)[right])) {
					swapped = right;
				}
				if (!_compare((*this)[index], (*this)[swapped])) {
					return;
				}
				::std::swap((*this)[index], (*this)[swapped]);
				index = swapped;
			}
		}

	public:
		using vector<T>::empty;
		using vector<T>::clear;
		using vector<T>::size;
		using vector<T>::capacity;
		using vector<T>::reserve;
		using vector<T>::swap;

		explicit priority_queue() noexcept : vector<T>{}
		{
		}

		template<::std::input_iterator iter>
		explicit priority_queue(iter first, iter last) noexcept : vector<T>{first, last}
		{
			::std::ptrdiff_t i{::std::distance(first, last) >> 1};
			while (i != 0) {
				_siftDown(--i);
			}
		}

		T& top() const noexcept {
			return this->front();
		}

		void push(const T& value) noexcept {
			::std::size_t back{this->size()};
			this->push_back(value);
			_siftUp(back);
		}

		void pop() noexcept {
			this->front() = ::std::move(this->back());
			this->pop_back();
			_siftDown(0);
		}
	};

}