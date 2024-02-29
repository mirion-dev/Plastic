#pragma once

#include <utility>

namespace plastic {

	template<class T>
	class deque {
		T* _begin;
		T* _end;
		T* _head;
		T* _tail;
		::std::size_t _size;
		::std::size_t _capacity;

		void _grow() {
			_capacity <<= 1;

			auto newData{new T[_capacity]};
			T* j{newData};
			for (T* i{_head}; i != _end; ++i, ++j) {
				*j = ::std::move(*i);
			}
			for (T* i{_begin}; i != _head; ++i, ++j) {
				*j = ::std::move(*i);
			}
			delete[] _begin;

			_begin = newData;
			_end = _begin + _capacity;
			_head = _begin;
			_tail = _begin + _size;
		}

	public:
		explicit deque(::std::size_t size = 1) {
			_begin = new T[size];
			_end = _begin + size;
			_head = _begin;
			_tail = _begin;
			_size = 0;
			_capacity = size;
		}

		~deque() {
			delete[] _begin;
		}

		bool empty() {
			return _size == 0;
		}

		::std::size_t size() {
			return _size;
		}

		void push_front(const T& value) {
			if (_size == _capacity) {
				_grow();
			}
			if (_head == _begin) {
				_head = _end;
			}
			--_head;
			*_head = value;
			++_size;
		}

		void push_back(const T& value) {
			if (_size == _capacity) {
				_grow();
			}
			*_tail = value;
			++_tail;
			if (_tail == _end) {
				_tail = _begin;
			}
			++_size;
		}

		T pop_front() {
			if (empty()) {
				return {};
			}
			T value{*_head};
			++_head;
			if (_head == _end) {
				_head = _begin;
			}
			--_size;
			return value;
		}

		T pop_back() {
			if (empty()) {
				return {};
			}
			if (_tail == _begin) {
				_tail = _end;
			}
			--_tail;
			--_size;
			return *_tail;
		}
	};

}