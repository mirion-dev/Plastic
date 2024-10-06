#pragma once

#include "vector.h"

namespace plastic {

	template<class T, std::strict_weak_order<T, T> compare = std::less<>>
	class priority_queue : private vector<T> {
		static constexpr compare _compare{};

		void _siftUp(size_t index) const {
			while (index != 0) {
				size_t parent{ (index - 1) >> 1 };
				if (!_compare((*this)[parent], (*this)[index])) {
					return;
				}
				std::swap((*this)[index], (*this)[parent]);
				index = parent;
			}
		}

		void _siftDown(size_t index) const {
			size_t size{ this->size() };
			while (true) {
				size_t left{ (index << 1) + 1 };
				if (left >= size) {
					return;
				}
				size_t right{ left + 1 }, swapped{ left };
				if (right < size && _compare((*this)[left], (*this)[right])) {
					swapped = right;
				}
				if (!_compare((*this)[index], (*this)[swapped])) {
					return;
				}
				std::swap((*this)[index], (*this)[swapped]);
				index = swapped;
			}
		}

	public:
		using vector<T>::empty;
		using vector<T>::clear;
		using vector<T>::size;
		using vector<T>::capacity;
		using vector<T>::reserve;

		explicit priority_queue() : vector<T>{}
		{
		}

		template<std::input_iterator iter>
		explicit priority_queue(iter first, iter last) : vector<T>{ first, last }
		{
			ptrdiff_t i{ std::distance(first, last) >> 1 };
			while (i != 0) {
				_siftDown(--i);
			}
		}

		T& top() const {
			return this->front();
		}

		void push(const T& value) {
			size_t back{ this->size() };
			this->push_back(value);
			_siftUp(back);
		}

		void pop() {
			this->front() = std::move(this->back());
			this->pop_back();
			_siftDown(0);
		}
	};

}