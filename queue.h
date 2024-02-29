#pragma once

#include "deque.h"

namespace plastic {

	template<class T>
	class queue : public deque<T> {
	public:
		using deque<T>::deque;

		void push_front(const T&) = delete;
		void push_back(const T&) = delete;
		T pop_front() = delete;
		T pop_back() = delete;

		void push(const T& value) {
			deque<T>::push_back(value);
		}

		T pop() {
			return deque<T>::pop_front();
		}
	};

}