#pragma once

#include <memory>

namespace plastic {

	template<class T>
	class deque {
		T* _begin;
		T* _end;
		T* _first;
		T* _last;

	public:
		class iterator {
			friend deque;

			T* _ptr;
			const deque* _container;

		public:
			using difference_type = ::std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;
			using iterator_category = ::std::random_access_iterator_tag;

			explicit iterator(T* node = nullptr, const deque* container = nullptr) noexcept {
				_ptr = node;
				_container = container;
			}

			reference operator*() const noexcept {
				return *_ptr;
			}

			pointer operator->() const noexcept {
				return _ptr;
			}

			reference operator[](::std::size_t index) const noexcept {
				return *(*this + index);
			}

			bool operator==(iterator it) const noexcept {
				return _ptr == it._ptr;
			}

			::std::strong_ordering operator<=>(iterator it) const noexcept {
				return *this - it <=> 0;
			}

			iterator& operator+=(difference_type diff) noexcept {
				::std::ptrdiff_t d{_container->_end - _container->_begin};
				if (_container->_end - _ptr <= diff) {
					_ptr += diff - d;
				}
				else if (_container->_begin - _ptr > diff) {
					_ptr += diff + d;
				}
				else {
					_ptr += diff;
				}
				return *this;
			}

			iterator& operator-=(difference_type diff) noexcept {
				::std::ptrdiff_t d{_container->_end - _container->_begin};
				if (_ptr - _container->_end >= diff) {
					_ptr -= diff + d;
				}
				else if (_ptr - _container->_begin < diff) {
					_ptr -= diff - d;
				}
				else {
					_ptr -= diff;
				}
				return *this;
			}

			iterator operator+(difference_type diff) const noexcept {
				return iterator{*this} += diff;
			}

			friend iterator operator+(difference_type diff, iterator it) noexcept {
				return it += diff;
			}

			iterator operator-(difference_type diff) const noexcept {
				return iterator{*this} -= diff;
			}

			difference_type operator-(iterator it) const noexcept {
				::std::ptrdiff_t d{_container->_end - _container->_begin};
				::std::ptrdiff_t index1{_ptr - _container->_first};
				::std::ptrdiff_t index2{it._ptr - _container->_first};
				return (index1 >= 0 ? index1 : index1 + d) - (index2 >= 0 ? index2 : index2 + d);
			}

			iterator& operator++() noexcept {
				++_ptr;
				if (_ptr == _container->_end) {
					_ptr = _container->_begin;
				}
				return *this;
			}

			iterator operator++(int) noexcept {
				iterator temp{*this};
				++*this;
				return temp;
			}

			iterator& operator--() noexcept {
				if (_ptr == _container->_begin) {
					_ptr = _container->_end;
				}
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
			_begin = _first = new T[count + 1];
			_end = _begin + count + 1;
			_last = _end - 1;
			::std::uninitialized_fill(_first, _last, value);
		}

		template<::std::input_iterator iter>
		explicit deque(iter first, iter last) noexcept {
			::std::size_t d{::std::distance(first, last)};
			_begin = _first = new T[d + 1];
			_end = _begin + d + 1;
			_last = _end - 1;
			::std::uninitialized_copy(first, last, _first);
		}

		~deque() noexcept {
			delete[] _begin;
		}

		bool empty() const noexcept {
			return _first == _last;
		}

		void clear() noexcept {
			resize(0);
		}

		::std::size_t size() const noexcept {
			return end() - begin();
		}

		void resize(::std::size_t size, const T& value = {}) noexcept {
			::std::size_t oldSize{this->size()};

			if (size == oldSize) {
				return;
			}

			if (size < oldSize) {
				T* newLast{(begin() + size)._ptr};
				if (newLast < _last) {
					::std::destroy(newLast, _last);
				}
				else {
					::std::destroy(newLast, _end);
					::std::destroy(_begin, _last);
				}
				_last = newLast;
				return;
			}

			if (size <= capacity()) {
				T* newLast{(begin() + size)._ptr};
				if (_last < newLast) {
					::std::uninitialized_fill(_last, newLast, value);
				}
				else {
					::std::uninitialized_fill(_last, _end, value);
					::std::uninitialized_fill(_begin, newLast, value);
				}
				_last = newLast;
				return;
			}

			reserve(size);
			T* newLast{_end - 1};
			::std::uninitialized_fill(_last, newLast, value);
			_last = newLast;
		}

		::std::size_t capacity() const noexcept {
			return _end - _begin - 1;
		}

		void reserve(::std::size_t capacity) noexcept {
			if (capacity <= this->capacity()) {
				return;
			}

			T* newBegin{new T[capacity + 1]};
			T* newEnd{newBegin + capacity + 1};
			T* newLast;
			if (_first <= _last) {
				newLast = ::std::uninitialized_move(_first, _last, newBegin);
			}
			else {
				newLast = ::std::uninitialized_move(_begin, _last, ::std::uninitialized_move(_first, _end, newBegin));
			}
			delete[] _begin;

			_begin = _first = newBegin;
			_end = newEnd;
			_last = newLast;
		}

		iterator begin() const noexcept {
			return iterator{_first, this};
		}

		iterator end() const noexcept {
			return iterator{_last, this};
		}

		T& operator[](::std::size_t index) const noexcept {
#ifdef PLASTIC_VERIFY
			if (index >= size()) {
				::std::abort();
			}
#endif
			return begin()[index];
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
			return (_last == _begin ? _end : _last)[-1];
		}

		void push_front(const T& value) noexcept {
			if ((_first == _begin ? _end : _first) - 1 == _last) {
				::std::size_t capacity{this->capacity()};
				reserve(capacity + (capacity <= 1 ? 1 : capacity >> 1));
			}
			if (_first == _begin) {
				_first = _end;
			}
			*--_first = value;
		}

		void pop_front() noexcept {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				::std::abort();
			}
#endif
			::std::destroy_at(_first++);
			if (_first == _end) {
				_first = _begin;
			}
		}

		void push_back(const T& value) noexcept {
			if ((_first == _begin ? _end : _first) - 1 == _last) {
				::std::size_t capacity{this->capacity()};
				reserve(capacity + (capacity <= 1 ? 1 : capacity >> 1));
			}
			*_last++ = value;
			if (_last == _end) {
				_last = _begin;
			}
		}

		void pop_back() noexcept {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				::std::abort();
			}
#endif
			if (_last == _begin) {
				_last = _end;
			}
			::std::destroy_at(--_last);
		}

		iterator insert(iterator pos, const T& value, ::std::size_t count = 1) noexcept {
			if (count == 0) {
				return pos;
			}
			::std::size_t capacity{this->capacity()};
			if (capacity - size() < count) {
				::std::ptrdiff_t offset{pos - begin()};
				reserve(capacity + ((capacity >> 1) < count ? count : capacity >> 1));
				pos = begin() + offset;
			}
			iterator i{end() + count}, j{end()};
			while (j != pos) {
				*--i = ::std::move(*--j);
			}
			while (i != pos) {
				*--i = value;
			}
			_last = (end() + count)._ptr;
			return pos;
		}

		template<::std::input_iterator iter>
		iterator insert(iterator pos, iter first, iter last) noexcept {
			if (first == last) {
				return pos;
			}
			::std::ptrdiff_t d{::std::distance(first, last)};
			::std::size_t capacity{this->capacity()};
			if (capacity - size() < d) {
				::std::ptrdiff_t offset{pos - begin()};
				reserve(capacity + ((capacity >> 1) < d ? d : capacity >> 1));
				pos = begin() + offset;
			}
			iterator i{end() + d}, j{end()};
			while (j != pos) {
				*--i = ::std::move(*--j);
			}
			while (i != pos) {
				*--i = static_cast<T>(*--last);
			}
			_last = (end() + d)._ptr;
			return pos;
		}

		iterator erase(iterator pos) noexcept {
#ifdef PLASTIC_VERIFY
			if (pos == end()) {
				::std::abort();
			}
#endif
			iterator i{pos}, j{pos + 1}, e{end()};
			while (j != e) {
				*i++ = ::std::move(*j++);
			}
			::std::destroy_at(i._ptr);
			_last = i._ptr;
			return pos;
		}

		iterator erase(iterator first, iterator last) noexcept {
			if (first == last) {
				return first;
			}
			iterator i{first}, j{last}, e{end()};
			while (j != e) {
				*i++ = ::std::move(*j++);
			}
			::std::destroy(i, e);
			_last = i._ptr;
			return first;
		}

		void swap(const deque& container) noexcept {
			if (this == &container) {
				return;
			}
			::std::swap(_begin, container._begin);
			::std::swap(_end, container._end);
			::std::swap(_first, container._first);
			::std::swap(_last, container._last);
		}

		::std::compare_three_way_result<T> operator<=>(const deque& container) const noexcept {
			iterator i{begin()}, j{container.begin()}, ie{end()}, je{container.end()};
			while (i != ie && j != je) {
				::std::compare_three_way_result<T> cmp{*i++ <=> *j++};
				if (cmp != 0) {
					return cmp;
				}
			}
			return i == ie && j != je ? ::std::strong_ordering::less
				: i != ie && j == je ? ::std::strong_ordering::greater
				: ::std::strong_ordering::equal;
		}
	};

}