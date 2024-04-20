#pragma once

#include "deque.h"

namespace plastic {

	template<class T>
	class queue : private deque<T> {
	public:
		using deque<T>::deque;
		using deque<T>::empty;
		using deque<T>::size;
		using deque<T>::capacity;
		using deque<T>::reserve;
		using deque<T>::front;
		using deque<T>::back;

		void push(const T& value) noexcept {
			this->push_back(value);
		}

		void pop() noexcept {
			this->pop_front();
		}
	};

}