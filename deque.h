#pragma once

#include <memory>

namespace plastic {

	template<class T>
	class deque {
		T* _begin;
		T* _end;
		T* _first;
		T* _last;
		::std::size_t _size;

	public:
		class iterator {
			friend deque;

			T* _ptr;

		public:
			using difference_type = ::std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;
			using iterator_category = ::std::random_access_iterator_tag;

			explicit iterator(T* node) noexcept {
				_ptr = node;
			}

			reference operator*() const noexcept {
				return *_ptr;
			}

			pointer operator->() const noexcept {
				return _ptr;
			}

			iterator operator[](::std::size_t index) const noexcept {
				return _ptr + index;
			}

			bool operator==(iterator it) const noexcept {
				return _ptr == it._ptr;
			}

			iterator& operator++() noexcept {
				++_ptr;
				return *this;
			}

			iterator operator++(int) noexcept {
				iterator temp{*this};
				++*this;
				return temp;
			}

			iterator& operator--() noexcept {
				--_ptr;
				return *this;
			}

			iterator operator--(int) noexcept {
				iterator temp{*this};
				--*this;
				return temp;
			}
		};

		explicit deque(::std::size_t count = 0, const T& value = {}) noexcept {
			_begin = _first = new T[count];
			_end = _last = _begin + count;
			_size = count;
			::std::uninitialized_fill(_first, _last, value);
		}

		template<::std::input_iterator iter>
		explicit deque(iter first, iter last) noexcept {
			::std::size_t d{::std::distance(first, last)};
			_begin = _first = new T[d];
			_end = _last = _begin + d;
			_size = d;
			::std::uninitialized_copy(first, last, _begin);
		}

		~deque() noexcept {
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
			T* newTail{::std::uninitialized_move(_begin, _first, ::std::uninitialized_move(_first, _end, newBegin))};
			delete[] _begin;

			_begin = _first = newBegin;
			_end = newEnd;
			_last = newTail;
		}

		void resize(::std::size_t size, const T& value = {}) noexcept {

		}

		iterator begin() const noexcept {
			return _first;
		}

		iterator end() const noexcept {
			return _last;
		}

		T& operator[](::std::size_t index) const noexcept {

		}

		T& front() const noexcept {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				::std::abort();
			}
#endif
			return *_first;
		}

		T& back() const noexcept {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				::std::abort();
			}
#endif
			return _last[-1];
		}

		void push_front(const T& value) noexcept {

		}

		void pop_front() noexcept {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				::std::abort();
			}
#endif
			if (++_first == _end) {
				_first = _begin;
			}
			--_size;
		}

		void push_back(const T& value) noexcept {
			if (_size == capacity()) {
				reserve(_size + ::std::max(_size >> 1, ::std::size_t{1}));
			}
			*_last = value;
			if (++_last == _end) {
				_last = _begin;
			}
			++_size;
		}

		void pop_back() noexcept {

		}

		iterator insert(iterator pos, const T& value, ::std::size_t count = 1) noexcept {

		}

		template<::std::input_iterator iter>
		iterator insert(iterator pos, iter first, iter last) noexcept {

		}

		iterator erase(iterator pos) noexcept {

		}

		iterator erase(iterator first, iterator last) noexcept {

		}
	};

}