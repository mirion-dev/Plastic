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

		template<std::input_iterator iter>
		explicit vector(iter first, iter last) {
			ptrdiff_t d{ std::distance(first, last) };
			_begin = new T[d];
			_last = _end = _begin + d;
			std::uninitialized_copy(first, last, _begin);
		}

		explicit vector(std::initializer_list<T> list) {
			size_t n{ list.size() };
			_begin = new T[n];
			_last = _end = _begin + n;
			std::uninitialized_copy(list.begin(), list.end(), _begin);
		}

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
			size_t oldSize{ this->size() };

			if (size == oldSize) {
				return;
			}

			if (size < oldSize) {
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
				size_t capacity{ this->capacity() };
				reserve(capacity + (capacity <= 1 ? 1 : capacity >> 1));
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

		T* insert(T* pos, const T& value, size_t count = 1) {
			if (count == 0) {
				return pos;
			}
			if (static_cast<size_t>(_end - _last) < count) {
				size_t capacity{ this->capacity() };
				ptrdiff_t offset{ pos - _begin };
				reserve(capacity + ((capacity >> 1) < count ? count : capacity >> 1));
				pos = _begin + offset;
			}
			T* i{ _last + count }, * j{ _last };
			while (j != pos) {
				*--i = std::move(*--j);
			}
			while (i != pos) {
				*--i = value;
			}
			_last += count;
			return pos;
		}

		template<std::input_iterator iter>
		T* insert(T* pos, iter first, iter last) {
			if (first == last) {
				return pos;
			}
			ptrdiff_t d{ std::distance(first, last) };
			if (_end - _last < d) {
				size_t capacity{ this->capacity() };
				ptrdiff_t offset{ pos - _begin };
				reserve(capacity + ((capacity >> 1) < d ? d : capacity >> 1));
				pos = _begin + offset;
			}
			T* i{ _last + d }, * j{ _last };
			while (j != pos) {
				*--i = std::move(*--j);
			}
			while (i != pos) {
				*--i = static_cast<T>(*--last);
			}
			_last += d;
			return pos;
		}

		T* erase(T* pos) {
#ifdef PLASTIC_VERIFY
			if (pos == _last) {
				std::abort();
			}
#endif
			T* i{ pos }, * j{ pos + 1 };
			while (j != _last) {
				*i++ = std::move(*j++);
			}
			std::destroy_at(i);
			_last = i;
			return pos;
		}

		T* erase(T* first, T* last) {
			if (first == last) {
				return first;
			}
			T* i{ first }, * j{ last };
			while (j != _last) {
				*i++ = std::move(*j++);
			}
			std::destroy(i, _last);
			_last = i;
			return first;
		}

		std::compare_three_way_result<T> operator<=>(const vector& container) const {
			T* i{ _begin }, j{ container._begin };
			while (i != _last && j != container._last) {
				std::compare_three_way_result<T> cmp{ *i++ <=> *j++ };
				if (cmp != 0) {
					return cmp;
				}
			}
			return i == _last && j != container._last ? std::strong_ordering::less
				: i != _last && j == container._last ? std::strong_ordering::greater
				: std::strong_ordering::equal;
		}
	};

	template<class T>
	explicit vector(std::initializer_list<T>)->vector<T>;

}