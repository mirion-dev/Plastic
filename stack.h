#pragma once

#include <utility>

namespace plastic {

	template<class T, bool unchecked = false>
	class stack {
		T* _begin;
		T* _end;
		T* _top;
		::std::size_t _size;
		::std::size_t _capacity;

		void _grow() {
			_capacity += _capacity <= 1 ? 1 : _capacity >> 1;

			T* newBegin{new T[_capacity]};
			if constexpr (!unchecked) {
				if (newBegin == nullptr) {
					::std::abort();
				}
			}
			for (T* i{_begin}, * j{newBegin}; i != _end; ++i, ++j) {
				*j = ::std::move(*i);
			}
			delete[] _begin;

			_begin = newBegin;
			_end = _begin + _capacity;
			_top = _begin + _size;
		}

	public:
		explicit stack(::std::size_t capacity = 0) {
			_begin = new T[capacity];
			_end = _begin + capacity;
			_top = _begin;
			_size = 0;
			_capacity = capacity;
		}

		~stack() {
			delete[] _begin;
		}

		bool empty() const {
			return _size == 0;
		}

		::std::size_t size() const {
			return _size;
		}

		T& top() {
			if constexpr (!unchecked) {
				if (empty()) {
					::std::abort();
				}
			}
			return *(_top - 1);
		}

		void push(const T& value) {
			if (_size == _capacity) {
				_grow();
			}
			++_size;
			*_top = value;
			++_top;
		}

		void pop() {
			if constexpr (!unchecked) {
				if (empty()) {
					::std::abort();
				}
			}
			--_size;
			--_top;
		}
	};

}