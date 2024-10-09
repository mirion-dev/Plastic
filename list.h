#pragma once

#include "utils.h"

#include <iterator>

namespace plastic {

	template<class T>
	class list {
		struct node {
			T value;
			node* prev;
			node* next;
		};

		node* _sentinel;

	public:
		class iterator {
			friend class list;

			node* _ptr;

		public:
			using difference_type = ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;
			using iterator_category = std::bidirectional_iterator_tag;

			explicit iterator(node* node = nullptr) {
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

			iterator& operator--() {
				_ptr = _ptr->prev;
				return *this;
			}

			iterator operator--(int) {
				iterator temp{ *this };
				--*this;
				return temp;
			}
		};

		explicit list(size_t count = {}, const T& value = {}) {
			_sentinel = new node;
			_sentinel->next = _sentinel->prev = _sentinel;
			resize(count, value);
		}

		template<std::input_iterator iter>
		explicit list(iter first, iter last) {
			_sentinel = new node;
			_sentinel->next = _sentinel->prev = _sentinel;
			while (first != last) {
				_sentinel->next = new node{ *--last, _sentinel, _sentinel->next };
			}
		}

		~list() {
			clear();
			delete _sentinel;
		}

		bool empty() const {
			return _sentinel->next == _sentinel;
		}

		void clear() const {
			node* i{ _sentinel->next };
			while (i != _sentinel) {
				node* next{ i->next };
				delete i;
				i = next;
			}
		}

		void resize(size_t count, const T& value = {}) const {
			for (size_t i{}; i != count; ++i) {
				_sentinel->next = new node{ value, _sentinel, _sentinel->next };
			}
		}

		iterator begin() const {
			return iterator{ _sentinel->next };
		}

		iterator end() const {
			return iterator{ _sentinel };
		}

		T& front() const {
			PLASTIC_VERIFY(!empty());
			return _sentinel->next->value;
		}

		T& back() const {
			PLASTIC_VERIFY(!empty());
			return _sentinel->prev->value;
		}

		void push_front(const T& value) const {
			_sentinel->next->next->prev = _sentinel->next = new node{ value, _sentinel, _sentinel->next };
		}

		void pop_front() const {
			PLASTIC_VERIFY(!empty());
			node* temp{ _sentinel->next };
			_sentinel->next = temp->next;
			temp->next->prev = _sentinel;
			delete temp;
		}

		void push_back(const T& value) const {
			_sentinel->prev->prev->next = _sentinel->prev = new node{ value, _sentinel->prev, _sentinel };
		}

		void pop_back() const {
			PLASTIC_VERIFY(!empty());
			node* temp{ _sentinel->prev };
			_sentinel->prev = temp->prev;
			temp->prev->next = _sentinel;
			delete temp;
		}

		iterator insert(iterator pos, const T& value, size_t count = 1) const {
			node* i{ pos._ptr };
			for (size_t j{}; j != count; ++j) {
				i = i->prev = new node{ value, i->prev, i };
			}
			i->prev->next = i;
			return iterator{ i };
		}

		template<std::input_iterator iter>
		iterator insert(iterator pos, iter first, iter last) const {
			node* i{ pos._ptr };
			while (first != last) {
				i = i->prev = new node{ *first++, i->prev, i };
			}
			i->prev->next = i;
			return iterator{ i };
		}

		iterator erase(iterator pos) const {
			node* i{ (++pos)._ptr };
			node* prev{ i->prev };
			PLASTIC_VERIFY(prev != _sentinel);
			i->prev = prev->prev;
			i->prev->next = i;
			delete prev;
			return pos;
		}

		iterator erase(iterator first, iterator last) const {
			node* i{ first._ptr }, * j{ last._ptr };
			i->prev->next = j;
			j->prev = i->prev;
			while (i != j) {
				node* next = i->next;
				delete i;
				i = next;
			}
			return last;
		}

		std::compare_three_way_result<T> operator<=>(const list& container) const {
			iterator i{ _sentinel->next }, j{ container._sentinel->next };
			while (i != _sentinel && j != container._sentinel) {
				std::compare_three_way_result<T> cmp{ *i++ <=> *j++ };
				if (cmp != 0) {
					return cmp;
				}
			}
			return i == _sentinel && j != container._sentinel ? std::strong_ordering::less
				: i != _sentinel && j == container._sentinel ? std::strong_ordering::greater
				: std::strong_ordering::equal;
		}
	};

}