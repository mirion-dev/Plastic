#pragma once

#include <memory>

namespace plastic {

	template<class T>
	class vector {
		T* _begin;
		T* _last;
		T* _end;

	public:
		explicit vector(size_t count = 0, const T& value = {}) {
			_begin = new T[count];
			_last = _end = _begin + count;
			std::uninitialized_fill(_begin, _end, value);
		}

		template<std::input_iterator It>
		explicit vector(It first, It last) {
			size_t n{ (size_t)std::distance(first, last) };
			_begin = new T[n];
			_last = _end = _begin + n;
			std::uninitialized_copy(first, last, _begin);
		}

		explicit vector(std::initializer_list<T> list) : vector(list.begin(), list.end()) {}

		~vector() {
			delete[] _begin;
		}

		bool empty() const {
			return _begin == _last;
		}

		void clear() {
			resize(0);
		}

		size_t size() const {
			return _last - _begin;
		}

		void resize(size_t size, const T& value = {}) {
			if (size == this->size()) {
				return;
			}

			if (size < this->size()) {
				T* newLast{ _begin + size };
				std::destroy(newLast, _last);
				_last = newLast;
				return;
			}

			if (size <= capacity()) {
				T* newLast{ _begin + size };
				std::uninitialized_fill(_last, newLast, value);
				_last = newLast;
				return;
			}

			reserve(size);
			std::uninitialized_fill(_last, _end, value);
			_last = _end;
		}

		size_t capacity() const {
			return _end - _begin;
		}

		void reserve(size_t capacity) {
			if (capacity <= this->capacity()) {
				return;
			}

			T* newBegin{ new T[capacity] };
			T* newLast{ std::uninitialized_move(_begin, _last, newBegin) };
			T* newEnd{ newBegin + capacity };
			delete[] _begin;

			_begin = newBegin;
			_last = newLast;
			_end = newEnd;
		}

		T* begin() {
			return _begin;
		}

		const T* begin() const {
			return _begin;
		}

		const T* cbegin() const {
			return _begin;
		}

		T* end() {
			return _last;
		}

		const T* end() const {
			return _last;
		}

		const T* cend() const {
			return _last;
		}

		T& operator[](size_t index) {
#ifdef PLASTIC_VERIFY
			if (index >= size()) {
				std::abort();
			}
#endif
			return _begin[index];
		}

		const T& operator[](size_t index) const {
#ifdef PLASTIC_VERIFY
			if (index >= size()) {
				std::abort();
			}
#endif
			return _begin[index];
		}

		T& front() {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				std::abort();
			}
#endif
			return *_begin;
		}

		const T& front() const {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				std::abort();
			}
#endif
			return *_begin;
		}

		T& back() {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				std::abort();
			}
#endif
			return _last[-1];
		}

		const T& back() const {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				std::abort();
			}
#endif
			return _last[-1];
		}

		T* data() {
			return _begin;
		}

		const T* data() const {
			return _begin;
		}

		void push_back(const T& value) {
			if (_last == _end) {
				reserve(capacity() + (capacity() <= 1 ? 1 : capacity() >> 1));
			}
			*_last++ = value;
		}

		void pop_back() {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				std::abort();
			}
#endif
			std::destroy_at(--_last);
		}

		T* insert(T* pos, const T& value) {
			return insert(pos, 1, value);
		}

		T* insert(T* pos, size_t count, const T& value) {
			if (count == 0) {
				return pos;
			}
			if (size_t(_end - _last) < count) {
				size_t offset{ size_t(pos - _begin) };
				reserve(capacity() + (capacity() >> 1 < count ? count : capacity() >> 1));
				pos = _begin + offset;
			}
			std::fill(pos, std::move_backward(pos, _last, _last + count), value);
			_last += count;
			return pos;
		}

		template<std::input_iterator It>
		T* insert(T* pos, It first, It last) {
			if (first == last) {
				return pos;
			}
			size_t n{ (size_t)std::distance(first, last) };
			if (size_t(_end - _last) < n) {
				size_t offset{ size_t(pos - _begin) };
				reserve(capacity() + (capacity() >> 1 < n ? n : capacity() >> 1));
				pos = _begin + offset;
			}
			std::copy_backward(first, last, std::move_backward(pos, _last, _last + n));
			_last += n;
			return pos;
		}

		T* insert(T* pos, std::initializer_list<T> list) {
			return insert(pos, list.begin(), list.end());
		}

		T* erase(T* pos) {
#ifdef PLASTIC_VERIFY
			if (pos == _last) {
				std::abort();
			}
#endif
			_last = std::move(pos + 1, _last, pos);
			std::destroy_at(_last);
			return pos;
		}

		T* erase(T* first, T* last) {
			T* newLast{ std::move(last, _last, first) };
			std::destroy(newLast, _last);
			_last = newLast;
			return first;
		}

	};

	template<class T>
	explicit vector(std::initializer_list<T>)->vector<T>;

}