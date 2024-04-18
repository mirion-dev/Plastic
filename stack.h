#pragma once

#include "vector.h"

namespace plastic {

	template<class T>
	class stack : private vector<T> {
	public:
		using vector<T>::empty;
		using vector<T>::size;
		using vector<T>::capacity;
		using vector<T>::reserve;

		T& top() noexcept {
			return this->back();
		}

		void push(const T& value) noexcept {
			this->push_back(value);
		}

		void pop() noexcept {
			this->pop_back();
		}
	};

}