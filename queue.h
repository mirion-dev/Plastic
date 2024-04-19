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

	public:
		explicit queue() noexcept {
			_begin = _end = _head = _tail = new T[0];
			_size = 0;
		}

		template<::std::input_iterator iter>
		explicit queue(iter first, iter last) noexcept {
			::std::size_t size{::std::distance(first, last)};
			_begin = _head = new T[size];
			_end = _tail = _begin + size;
			_size = size;
			::std::uninitialized_copy(first, last, _begin);
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

		::std::size_t capacity() const noexcept {
			return _end - _begin;
		}

		void reserve(::std::size_t capacity) noexcept {
			if (capacity <= this->capacity()) {
				return;
			}

			T* newBegin{new T[capacity]};
			T* newEnd{newBegin + capacity};
			T* newTail{::std::uninitialized_move(_begin, _head, ::std::uninitialized_move(_head, _end, newBegin))};
			delete[] _begin;

			_begin = _head = newBegin;
			_end = newEnd;
			_tail = newTail;
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
			if (_size == capacity()) {
				reserve(_size + ::std::max(_size >> 1, ::std::size_t{1}));
			}
			*_tail = value;
			if (++_tail == _end) {
				_tail = _begin;
			}
			++_size;
		}

		void pop() noexcept {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				::std::abort();
			}
#endif
			if (++_head == _end) {
				_head = _begin;
			}
			--_size;
		}
	};

}