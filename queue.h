#pragma once

#include <memory>

namespace plastic {

	template<class T>
	class queue {
		T* _begin;
		T* _end;
		T* _head;
		T* _tail;
		::std::size_t _size;
		::std::size_t _capacity;

	public:
		explicit queue() noexcept {
			_begin = new T[0];
			_end = _begin;
			_head = _begin;
			_tail = _begin;
			_size = 0;
			_capacity = 0;
		}

		~queue() noexcept {
			delete[] _begin;
		}

		bool empty() const noexcept {
			return _size == 0;
		}

		::std::size_t size() const noexcept {
			return _size;
		}

		void reserve(::std::size_t capacity) noexcept {
			if (capacity <= _capacity) {
				return;
			}

			T* newBegin{new T[capacity]};
			::std::uninitialized_move(_begin, _head, ::std::uninitialized_move(_head, _end, newBegin));
			delete[] _begin;

			_begin = newBegin;
			_end = _begin + capacity;
			_head = _begin;
			_tail = _begin + _size;
			_capacity = capacity;
		}

		T& front() noexcept {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				::std::abort();
			}
#endif
			return *_head;
		}

		T& back() noexcept {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				::std::abort();
			}
#endif
			return *(_tail - 1);
		}

		void push(const T& value) noexcept {
			if (_size == _capacity) {
				reserve(::std::max(_size + 1, _capacity + (_capacity >> 1)));
			}
			++_size;
			*_tail = value;
			++_tail;
			if (_tail == _end) {
				_tail = _begin;
			}
		}

		void pop() noexcept {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				::std::abort();
			}
#endif
			--_size;
			++_head;
			if (_head == _end) {
				_head = _begin;
			}
		}
	};

}