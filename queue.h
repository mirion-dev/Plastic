#pragma once

#include "deque.h"

namespace plastic {

	template<class T>
	class queue : private deque<T> {
	public:
		using deque<T>::deque;
		using deque<T>::empty;
		using deque<T>::size;

		void push(const T& value) {
			deque<T>::push_back(value);
		}

		T pop() {
			return deque<T>::pop_front();
		}
	};

}