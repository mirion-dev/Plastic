#pragma once

#include "vector.h"

namespace plastic {

	template<class T>
	class stack : private vector<T> {
	public:
		using vector<T>::empty;
		using vector<T>::size;
		using vector<T>::reserve;

		T& top() {
			return this->back();
		}

		void push(const T& value) {
			this->push_back(value);
		}

		void pop() {
			this->pop_back();
		}
	};

}