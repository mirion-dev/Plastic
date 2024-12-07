#pragma once

#include "macro.h"

#include <iterator>

namespace plastic {

	template<class T>
	class forward_list {
		struct node {
			T value;
			node* next;
		};

		node* _sentinel;
		size_t _size;

	public:
		class iterator {
			friend class forward_list;

			node* _ptr;

		public:
			using difference_type = ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;
			using iterator_category = std::forward_iterator_tag;

			iterator(node* node = {}) {
				_ptr = node;
			}

			reference operator*() const {
				return _ptr->value;
			}

			pointer operator->() const {
				return &_ptr->value;
			}

			bool operator==(iterator it) const {
				return _ptr == it._ptr;
			}

			iterator& operator++() {
				_ptr = _ptr->next;
				return *this;
			}

			iterator operator++(int) {
				iterator temp{ *this };
				++*this;
				return temp;
			}
		};

		using const_iterator = std::const_iterator<iterator>;

		explicit forward_list(size_t size = {}, const T& value = {}) {
			_sentinel->next = _sentinel = new node;
			_size = 0;
			insert_after(end(), size, value);
		}

		template<std::input_iterator It>
		explicit forward_list(It first, It last) {
			_sentinel->next = _sentinel = new node;
			_size = 0;
			insert_after(end(), first, last);
		}

		explicit forward_list(std::initializer_list<T> list) : forward_list(list.begin(), list.end()) {}

		~forward_list() {
			clear();
			delete _sentinel;
		}

		bool empty() const {
			return _size == 0;
		}

		size_t size() const {
			return _size;
		}

		void clear() {
			resize(0);
		}

		void resize(size_t size, const T& value = {}) {
			if (size == _size) {
				return;
			}
			if (size < _size) {
				erase_after(std::next(end(), size), end());
				return;
			}
			insert_after(std::next(end(), _size), size - _size, value);
		}

		iterator begin() {
			return _sentinel->next;
		}

		const_iterator begin() const {
			return iterator{ _sentinel->next };
		}

		const_iterator cbegin() const {
			return iterator{ _sentinel->next };
		}

		iterator end() {
			return _sentinel;
		}

		const_iterator end() const {
			return iterator{ _sentinel };
		}

		const_iterator cend() const {
			return iterator{ _sentinel };
		}

		T& front() {
			PLASTIC_VERIFY(!empty());
			return _sentinel->next->value;
		}

		const T& front() const {
			PLASTIC_VERIFY(!empty());
			return _sentinel->next->value;
		}

		void push_front(const T& value) {
			_sentinel->next = new node{ value, _sentinel->next };
			++_size;
		}

		void pop_front() {
			PLASTIC_VERIFY(!empty());
			erase_after(end());
		}

		iterator insert_after(iterator pos, const T& value) {
			return insert_after(pos, 1, value);
		}

		iterator insert_after(iterator pos, size_t count, const T& value) {
			node* i{ pos._ptr };
			for (size_t j{}; j != count; ++j) {
				i = i->next = new node{ value, i->next };
			}
			_size += count;
			return i;
		}

		template<std::input_iterator It>
		iterator insert_after(iterator pos, It first, It last) {
			node* i{ pos._ptr };
			while (first != last) {
				i = i->next = new node{ *first++, i->next };
				++_size;
			}
			return i;
		}

		iterator insert_after(iterator pos, std::initializer_list<T> list) {
			return insert_after(pos, list.begin(), list.end());
		}

		iterator erase_after(iterator pos) {
			node* i{ pos._ptr };
			PLASTIC_VERIFY(i->next != _sentinel);
			delete std::exchange(i->next, i->next->next);
			--_size;
			return ++pos;
		}

		iterator erase_after(iterator first, iterator last) {
			PLASTIC_VERIFY(first != last || first == end());
			node* i{ first._ptr }, * j{ last._ptr };
			i = std::exchange(i->next, j);
			while (i != j) {
				delete std::exchange(i, i->next);
				--_size;
			}
			return last;
		}

		bool operator==(const forward_list& cont) const {
			return std::equal(begin(), end(), cont.begin(), cont.end());
		}

		auto operator<=>(const forward_list& cont) const {
			return std::lexicographical_compare_three_way(begin(), end(), cont.begin(), cont.end());
		}

	};

	template<class It>
	explicit forward_list(It, It)->forward_list<std::iter_value_t<It>>;

}