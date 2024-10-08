#pragma once

#include "utils.h"

#include <memory>

namespace plastic {

	template<class T>
	class vector {
		T* _begin;
		T* _last;
		T* _end;

	public:
		using iterator = T*;
		using const_iterator = std::const_iterator<iterator>;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::const_iterator<reverse_iterator>;

		explicit vector(size_t count = {}, const T& value = {}) {
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

		iterator begin() {
			return _begin;
		}

		const_iterator begin() const {
			return _begin;
		}

		const_iterator cbegin() const {
			return _begin;
		}

		iterator end() {
			return _last;
		}

		const_iterator end() const {
			return _last;
		}

		const_iterator cend() const {
			return _last;
		}

		reverse_iterator rbegin() {
			return _last;
		}

		const_reverse_iterator rbegin() const {
			return _last;
		}

		const_reverse_iterator crbegin() const {
			return _last;
		}

		reverse_iterator rend() {
			return _begin;
		}

		const_reverse_iterator rend() const {
			return _begin;
		}

		const_reverse_iterator crend() const {
			return _begin;
		}

		T& operator[](size_t index) {
			PLASTIC_VERIFY(index < size());
			return _begin[index];
		}

		const T& operator[](size_t index) const {
			PLASTIC_VERIFY(index < size());
			return _begin[index];
		}

		T& front() {
			PLASTIC_VERIFY(!empty());
			return *_begin;
		}

		const T& front() const {
			PLASTIC_VERIFY(!empty());
			return *_begin;
		}

		T& back() {
			PLASTIC_VERIFY(!empty());
			return _last[-1];
		}

		const T& back() const {
			PLASTIC_VERIFY(!empty());
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
			PLASTIC_VERIFY(!empty());
			std::destroy_at(--_last);
		}

		iterator insert(iterator pos, const T& value) {
			return insert(pos, 1, value);
		}

		iterator insert(iterator pos, size_t count, const T& value) {
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
		iterator insert(iterator pos, It first, It last) {
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

		iterator insert(iterator pos, std::initializer_list<T> list) {
			return insert(pos, list.begin(), list.end());
		}

		iterator erase(iterator pos) {
			PLASTIC_VERIFY(pos != _last);
			_last = std::move(pos + 1, _last, pos);
			std::destroy_at(_last);
			return pos;
		}

		iterator erase(iterator first, iterator last) {
			if (first == last) {
				return first;
			}
			T* newLast{ std::move(last, _last, first) };
			std::destroy(newLast, _last);
			_last = newLast;
			return first;
		}

		bool operator==(const vector& cont) const {
			return std::equal(begin(), end(), cont.begin(), cont.end());
		}

		auto operator<=>(const vector& cont) const {
			return std::lexicographical_compare_three_way(begin(), end(), cont.begin(), cont.end());
		}

	};

	template<class It>
	explicit vector(It, It)->vector<std::iter_value_t<It>>;

	template<class T>
	explicit vector(std::initializer_list<T>)->vector<T>;

}