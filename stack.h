#pragma once

#include <algorithm>

namespace plastic {

	template<class T>
	class stack {
		T* _begin;
		T* _end;
		T* _top;
		::std::size_t _size;
		::std::size_t _capacity;

		void _grow() {
			_capacity <<= 1;
			auto newData{new T[_capacity]};
			::std::copy(_begin, _end, newData);
			delete[] _begin;
			_begin = newData;
			_end = newData + _capacity;
			_top = _begin + _size;
		}

	public:
		explicit stack(::std::size_t size = 1) {
			_begin = new T[size];
			_end = _begin + size;
			_top = _begin;
			_size = 0;
			_capacity = size;
		}

		~stack() {
			delete[] _begin;
		}

		bool empty() {
			return _size == 0;
		}

		::std::size_t size() {
			return _size;
		}

		void push(const T& value) {
			if (_size == _capacity) {
				_grow();
			}
			*_top = value;
			++_top;
			++_size;
		}

		T pop() {
			if (empty()) {
				return {};
			}
			--_top;
			--_size;
			return *_top;
		}
	};

}