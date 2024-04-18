#pragma once

#include <memory>

namespace plastic {

	template<class T>
	class vector {
		T* _begin;
		T* _last;
		T* _end;
		::std::size_t _capacity;

	public:
		vector(::std::size_t size = 0, const T& value = {}) noexcept {
			_begin = new T[size];
			_last = _begin + size;
			_end = _last;
			_capacity = size;
			::std::uninitialized_fill(_begin, _end, value);
		}

		~vector() noexcept {
			::std::destroy(_begin, _end);
		}

		bool empty() const noexcept {
			return _begin == _last;
		}

		::std::size_t size() const noexcept {
			return _last - _begin;
		}

		void reserve(::std::size_t capacity) noexcept {
			if (capacity <= _capacity) {
				return;
			}

			T* newBegin{new T[capacity]};
			T* newLast{::std::uninitialized_move(_begin, _last, newBegin)};
			T* newEnd{newBegin + capacity};
			delete _begin;

			_begin = newBegin;
			_end = newEnd;
			_last = newLast;
			_capacity = capacity;
		}

		void resize(::std::size_t size, const T& value = {}) noexcept {
			if (size == this->size()) {
				return;
			}

			if (size < this->size()) {
				T* newEnd{_begin + size};
				::std::destroy(newEnd, _end);
				_end = newEnd;
				return;
			}

			if (size <= _capacity) {
				T* newEnd{_begin + size};
				::std::uninitialized_fill(_last, newEnd, value);
				_last = newEnd;
				_end = newEnd;
				return;
			}

			reserve(size);
			::std::uninitialized_fill(_last, _end, value);
			_last = _end;
		}

		T* begin() noexcept {
			return _begin;
		}

		T* end() noexcept {
			return _last;
		}

		T& front() noexcept {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				::std::abort();
			}
#endif
			return *_begin;
		}

		T& back() noexcept {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				::std::abort();
			}
#endif
			return _last[-1];
		}

		T& operator[](::std::size_t index) noexcept {
#ifdef PLASTIC_VERIFY
			if (index >= size()) {
				::std::abort();
			}
#endif
			return _begin[index];
		}

		void push_back(const T& value) {
			if (_last == _end) {
				reserve(::std::max(size() + 1, _capacity + (_capacity >> 1)));
			}
			*_last++ = value;
		}

		void pop_back() {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				::std::abort();
			}
#endif
			::std::destroy_at(--_last);
		}

		T* insert(T* pos, const T& value, ::std::size_t count = 1) {
			if (count == 0) {
				return pos;
			}
			if (_end - _last < count) {
				::std::ptrdiff_t offset{pos - _begin};
				reserve(::std::max(size() + count, _capacity + (_capacity >> 1)));
				pos = _begin + offset;
			}
			T* i{_last + count}, * j{_last};
			while (j != pos) {
				*--i = ::std::move(*--j);
			}
			while (i != pos) {
				*--i = value;
			}
			_last += count;
			return i;
		}

		template<::std::random_access_iterator iter> requires ::std::convertible_to<::std::iter_value_t<iter>, T>
		T* insert(T* pos, iter first, iter last) {
			::std::ptrdiff_t d{::std::distance(first, last)};
			if (d <= 0) {
				return pos;
			}
			if (_end - _last < d) {
				::std::ptrdiff_t offset{pos - _begin};
				reserve(::std::max(size() + d, _capacity + (_capacity >> 1)));
				pos = _begin + offset;
			}
			T* i{_last + d}, * j{_last};
			while (j != pos) {
				*--i = ::std::move(*--j);
			}
			while (i != pos) {
				*--i = static_cast<T>(*--last);
			}
			_last += d;
			return i;
		}

		T* erase(T* pos) {
#ifdef PLASTIC_VERIFY
			if (pos == _end) {
				::std::abort();
			}
#endif
			T* i{pos}, * j{pos + 1};
			while (j != _last) {
				*i++ = ::std::move(*j++);
			}
			::std::destroy_at(j);
			_last = i;
			return pos;
		}

		T* erase(T* first, T* last) {
			::std::ptrdiff_t d{::std::distance(first, last)};
			if (d <= 0) {
				return last;
			}
			T* i{first}, * j{last};
			while (j != _last) {
				*i++ = ::std::move(*j++);
			}
			::std::destroy(j, _last);
			_last = i;
			return first;
		}
	};

}