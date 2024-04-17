#pragma once

#include <utility>

namespace plastic {

	template<class T, bool unchecked = false>
	class queue {
		T* _begin;
		T* _end;
		T* _head;
		T* _tail;
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
			T* j{newBegin};
			for (T* i{_head}; i != _end; ++i, ++j) {
				*j = ::std::move(*i);
			}
			for (T* i{_begin}; i != _head; ++i, ++j) {
				*j = ::std::move(*i);
			}
			delete[] _begin;

			_begin = newBegin;
			_end = _begin + _capacity;
			_head = _begin;
			_tail = _begin + _size;
		}

	public:
		explicit queue(::std::size_t capacity = 0) {
			_begin = new T[capacity];
			_end = _begin + capacity;
			_head = _begin;
			_tail = _begin;
			_size = 0;
			_capacity = capacity;
		}

		~queue() {
			delete[] _begin;
		}

		bool empty() const {
			return _size == 0;
		}

		::std::size_t size() const {
			return _size;
		}

		T& front() {
			if constexpr (!unchecked) {
				if (empty()) {
					::std::abort();
				}
			}
			return *_head;
		}

		T& back() {
			if constexpr (!unchecked) {
				if (empty()) {
					::std::abort();
				}
			}
			return *(_tail - 1);
		}

		void push(const T& value) {
			if (_size == _capacity) {
				_grow();
			}
			++_size;
			*_tail = value;
			++_tail;
			if (_tail == _end) {
				_tail = _begin;
			}
		}

		void pop() {
			if constexpr (!unchecked) {
				if (empty()) {
					::std::abort();
				}
			}
			--_size;
			++_head;
			if (_head == _end) {
				_head = _begin;
			}
		}
	};

}