#pragma once

#include <iterator>

namespace plastic {

	template<class T>
	class forward_list {
		struct _node {
			T _value;
			_node* _next;
		};

		_node* _sentinel;

	public:
		class iterator {
			friend forward_list;

			_node* _ptr;

		public:
			using difference_type = ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;
			using iterator_category = std::input_iterator_tag;

			explicit iterator(_node* node = nullptr) {
				_ptr = node;
			}

			reference operator*() const {
				return _ptr->_value;
			}

			pointer operator->() const {
				return &_ptr->_value;
			}

			bool operator==(iterator it) const {
				return _ptr == it._ptr;
			}

			iterator& operator++() {
				_ptr = _ptr->_next;
				return *this;
			}

			iterator operator++(int) {
				iterator temp{ *this };
				++*this;
				return temp;
			}
		};

		explicit forward_list(size_t count = 0, const T& value = {}) {
			_sentinel = new _node;
			_sentinel->_next = _sentinel;
			resize(count, value);
		}

		template<std::input_iterator iter>
		explicit forward_list(iter first, iter last) {
			_sentinel = new _node;
			_sentinel->_next = _sentinel;
			while (first != last) {
				push_front(*--last);
			}
		}

		~forward_list() {
			clear();
			delete _sentinel;
		}

		bool empty() const {
			return _sentinel->_next == _sentinel;
		}

		void clear() const {
			_node* i{ _sentinel->_next };
			while (i != _sentinel) {
				_node* next{ i->_next };
				delete i;
				i = next;
			}
		}

		void resize(size_t count, const T& value = {}) const {
			for (size_t i{ 0 }; i < count; ++i) {
				push_front(value);
			}
		}

		iterator begin() const {
			return iterator{ _sentinel->_next };
		}

		iterator end() const {
			return iterator{ _sentinel };
		}

		T& front() const {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				std::abort();
			}
#endif
			return _sentinel->_next->_value;
		}

		void push_front(const T& value) const {
			_sentinel->_next = new _node{ value, _sentinel->_next };
		}

		void pop_front() const {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				std::abort();
			}
#endif
			_node* temp{ _sentinel->_next };
			_sentinel->_next = temp->_next;
			delete temp;
		}

		iterator insert_after(iterator pos, const T& value, size_t count = 1) const {
			_node* i{ pos._ptr };
			for (size_t j{ 0 }; j < count; ++j) {
				i = i->_next = new _node{ value, i->_next };
			}
			return iterator{ i };
		}

		template<std::input_iterator iter>
		iterator insert_after(iterator pos, iter first, iter last) const {
			_node* i{ pos._ptr };
			while (first != last) {
				i = i->_next = new _node{ *first++, i->_next };
			}
			return iterator{ i };
		}

		iterator erase_after(iterator pos) const {
			_node* i{ pos._ptr };
			_node* next{ i->_next };
#ifdef PLASTIC_VERIFY
			if (next == _sentinel) {
				std::abort();
			}
#endif
			i->_next = next->_next;
			delete next;
			return ++pos;
		}

		iterator erase_after(iterator first, iterator last) const {
			if (first == last) {
				return last;
			}
			_node* i{ first._ptr }, * j{ last._ptr };
			_node* next{ i->_next };
#ifdef PLASTIC_VERIFY
			if (next == _sentinel) {
				std::abort();
			}
#endif
			i->_next = j;
			i = next;
			while (i != j) {
				next = i->_next;
				delete i;
				i = next;
			}
			return last;
		}

		std::compare_three_way_result<T> operator<=>(const forward_list& container) const {
			iterator i{ _sentinel->_next }, j{ container._sentinel->_next };
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