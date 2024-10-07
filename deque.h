#pragma once

#include <memory>

namespace plastic {

	template<class T>
	class deque {
		T* _begin;
		T* _end;
		T* _first;
		T* _last;

		template<bool is_const>
		class iterator_impl {
			friend deque;

			T* _ptr;
			const deque* _cont;

		public:
			using difference_type = ptrdiff_t;
			using value_type = T;
			using pointer = std::conditional_t<is_const, const T*, T*>;
			using reference = std::conditional_t<is_const, const T&, T&>;
			using iterator_category = std::random_access_iterator_tag;

			explicit iterator_impl(T* ptr = {}, const deque* cont = {}) {
				_ptr = ptr;
				_cont = cont;
			}

			reference operator*() const {
				return *_ptr;
			}

			pointer operator->() const {
				return _ptr;
			}

			reference operator[](size_t index) const {
				return *(*this + index);
			}

			bool operator==(iterator_impl iter) const {
				return _ptr == iter._ptr;
			}

			std::strong_ordering operator<=>(iterator_impl iter) const {
				return *this - iter <=> 0;
			}

			iterator_impl& operator+=(difference_type diff) {
				if (_cont->_end - _ptr <= diff) {
					_ptr += diff - _cont->size();
				}
				else if (_cont->_begin - _ptr > diff) {
					_ptr += diff + _cont->size();
				}
				else {
					_ptr += diff;
				}
				return *this;
			}

			iterator_impl& operator-=(difference_type diff) {
				if (_ptr - _cont->_end >= diff) {
					_ptr -= diff + _cont->size();
				}
				else if (_ptr - _cont->_begin < diff) {
					_ptr -= diff - _cont->size();
				}
				else {
					_ptr -= diff;
				}
				return *this;
			}

			iterator_impl operator+(difference_type diff) const {
				return iterator_impl{ *this } += diff;
			}

			friend iterator_impl operator+(difference_type diff, iterator_impl it) {
				return it += diff;
			}

			iterator_impl operator-(difference_type diff) const {
				return iterator_impl{ *this } -= diff;
			}

			difference_type operator-(iterator_impl it) const {
				ptrdiff_t index1{ _ptr - _cont->_first }, index2{ it._ptr - _cont->_first };
				return (index1 >= 0 ? index1 : index1 + _cont->size()) - (index2 >= 0 ? index2 : index2 + _cont->size());
			}

			iterator_impl& operator++() {
				++_ptr;
				if (_ptr == _cont->_end) {
					_ptr = _cont->_begin;
				}
				return *this;
			}

			iterator_impl operator++(int) {
				iterator_impl temp{ *this };
				++*this;
				return temp;
			}

			iterator_impl& operator--() {
				if (_ptr == _cont->_begin) {
					_ptr = _cont->_end;
				}
				--_ptr;
				return *this;
			}

			iterator_impl operator--(int) {
				iterator_impl temp{ *this };
				--*this;
				return temp;
			}
		};

	public:
		using iterator = iterator_impl<false>;
		using const_iterator = iterator_impl<true>;

		explicit deque(size_t count = {}, const T& value = {}) {
			_begin = _first = new T[count + 1];
			_end = _begin + count + 1;
			_last = _end - 1;
			std::uninitialized_fill(_first, _last, value);
		}

		template<std::input_iterator It>
		explicit deque(It first, It last) {
			size_t n{ (size_t)std::distance(first, last) };
			_begin = _first = new T[n + 1];
			_end = _begin + n + 1;
			_last = _end - 1;
			std::uninitialized_copy(first, last, _first);
		}

		explicit deque(std::initializer_list<T> list) : deque(list.begin(), list.end()) {}

		~deque() {
			delete[] _begin;
		}

		bool empty() const {
			return _first == _last;
		}

		void clear() {
			resize(0);
		}

		size_t size() const {
			return end() - begin();
		}

		void resize(size_t size, const T& value = {}) {
			if (size == this->size()) {
				return;
			}

			if (size < this->size()) {
				T* newLast{ (begin() + size)._ptr };
				if (newLast < _last) {
					std::destroy(newLast, _last);
				}
				else {
					std::destroy(newLast, _end);
					std::destroy(_begin, _last);
				}
				_last = newLast;
				return;
			}

			if (size <= capacity()) {
				T* newLast{ (begin() + size)._ptr };
				if (_last < newLast) {
					std::uninitialized_fill(_last, newLast, value);
				}
				else {
					std::uninitialized_fill(_last, _end, value);
					std::uninitialized_fill(_begin, newLast, value);
				}
				_last = newLast;
				return;
			}

			reserve(size);
			std::uninitialized_fill(_last, _end - 1, value);
			_last = _end - 1;
		}

		size_t capacity() const {
			return _end - _begin - 1;
		}

		void reserve(size_t capacity) {
			if (capacity <= this->capacity()) {
				return;
			}

			T* newBegin{ new T[capacity + 1] };
			T* newEnd{ newBegin + capacity + 1 };
			T* newLast;
			if (_first <= _last) {
				newLast = std::uninitialized_move(_first, _last, newBegin);
			}
			else {
				newLast = std::uninitialized_move(_begin, _last, std::uninitialized_move(_first, _end, newBegin));
			}
			delete[] _begin;

			_begin = _first = newBegin;
			_end = newEnd;
			_last = newLast;
		}

		iterator begin() {
			return iterator{ _first, this };
		}

		const_iterator begin() const {
			return const_iterator{ _first, this };
		}

		const_iterator cbegin() const {
			return const_iterator{ _first, this };
		}

		iterator end() {
			return iterator{ _last, this };
		}

		const_iterator end() const {
			return const_iterator{ _last, this };
		}

		const_iterator cend() const {
			return const_iterator{ _last, this };
		}

		T& operator[](size_t index) {
#ifdef PLASTIC_VERIFY
			if (index >= size()) {
				std::abort();
			}
#endif
			return begin()[index];
		}

		const T& operator[](size_t index) const {
#ifdef PLASTIC_VERIFY
			if (index >= size()) {
				std::abort();
			}
#endif
			return begin()[index];
		}

		T& front() {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				std::abort();
			}
#endif
			return *_first;
		}

		const T& front() const {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				std::abort();
			}
#endif
			return *_first;
		}

		T& back() {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				std::abort();
			}
#endif
			return *--end();
		}

		const T& back() const {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				std::abort();
			}
#endif
			return *--end();
		}

		void push_front(const T& value) {
			if (++end() == begin()) {
				reserve(capacity() + (capacity() <= 1 ? 1 : capacity() >> 1));
			}
			if (_first == _begin) {
				_first = _end;
			}
			*--_first = value;
		}

		void pop_front() {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				std::abort();
			}
#endif
			std::destroy_at(_first++);
			if (_first == _end) {
				_first = _begin;
			}
		}

		void push_back(const T& value) {
			if (++end() == begin()) {
				reserve(capacity() + (capacity() <= 1 ? 1 : capacity() >> 1));
			}
			*_last++ = value;
			if (_last == _end) {
				_last = _begin;
			}
		}

		void pop_back() {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				std::abort();
			}
#endif
			if (_last == _begin) {
				_last = _end;
			}
			std::destroy_at(--_last);
		}

		iterator insert(iterator pos, const T& value) {
			return insert(pos, 1, value);
		}

		iterator insert(iterator pos, size_t count, const T& value) {
			if (count == 0) {
				return pos;
			}
			if (capacity() - size() < count) {
				size_t offset{ size_t(pos - begin()) };
				reserve(capacity() + (capacity() >> 1 < count ? count : capacity() >> 1));
				pos = begin() + offset;
			}
			std::fill(pos, std::move_backward(pos, end(), end() + count), value);
			_last = (end() + count)._ptr;
			return pos;
		}

		template<std::input_iterator It>
		iterator insert(iterator pos, It first, It last) {
			if (first == last) {
				return pos;
			}
			size_t n{ (size_t)std::distance(first, last) };
			if (capacity() - size() < n) {
				size_t offset{ size_t(pos - begin()) };
				reserve(capacity() + (capacity() >> 1 < n ? n : capacity() >> 1));
				pos = begin() + offset;
			}
			std::copy_backward(first, last, std::move_backward(pos, end(), end() + n));
			_last = (end() + n)._ptr;
			return pos;
		}

		iterator insert(iterator pos, std::initializer_list<T> list) {
			return insert(pos, list.begin(), list.end());
		}

		iterator erase(iterator pos) {
#ifdef PLASTIC_VERIFY
			if (pos == end()) {
				std::abort();
			}
#endif
			_last = std::move(std::next(pos), end(), pos)._ptr;
			std::destroy_at(_last);
			return pos;
		}

		iterator erase(iterator first, iterator last) {
			if (first == last) {
				return first;
			}
			T* newLast{ std::move(last, end(), first)._ptr };
			std::destroy(newLast, _last);
			_last = newLast;
			return first;
		}

	};

	template<class It>
	explicit deque(It, It)->deque<std::iter_value_t<It>>;

	template<class T>
	explicit deque(std::initializer_list<T>)->deque<T>;

}