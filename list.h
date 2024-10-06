#pragma once

#include <iterator>

namespace plastic {

	template<class T>
	class list {
		struct _node {
			T _value;
			_node* _prev;
			_node* _next;
		};

		_node* _sentinel;

	public:
		class iterator {
			friend list;

			_node* _ptr;

		public:
			using difference_type = ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;
			using iterator_category = std::bidirectional_iterator_tag;

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

			iterator& operator--() {
				_ptr = _ptr->_prev;
				return *this;
			}

			iterator operator--(int) {
				iterator temp{ *this };
				--*this;
				return temp;
			}
		};

		explicit list(size_t count = 0, const T& value = {}) {
			_sentinel = new _node;
			_sentinel->_next = _sentinel->_prev = _sentinel;
			resize(count, value);
		}

		template<std::input_iterator iter>
		explicit list(iter first, iter last) {
			_sentinel = new _node;
			_sentinel->_next = _sentinel->_prev = _sentinel;
			while (first != last) {
				_sentinel->_next = new _node{ *--last, _sentinel, _sentinel->_next };
			}
		}

		~list() {
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
				_sentinel->_next = new _node{ value, _sentinel, _sentinel->_next };
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

		T& back() const {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				std::abort();
			}
#endif
			return _sentinel->_prev->_value;
		}

		void push_front(const T& value) const {
			_sentinel->_next->_next->_prev = _sentinel->_next = new _node{ value, _sentinel, _sentinel->_next };
		}

		void pop_front() const {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				std::abort();
			}
#endif
			_node* temp{ _sentinel->_next };
			_sentinel->_next = temp->_next;
			temp->_next->_prev = _sentinel;
			delete temp;
		}

		void push_back(const T& value) const {
			_sentinel->_prev->_prev->_next = _sentinel->_prev = new _node{ value, _sentinel->_prev, _sentinel };
		}

		void pop_back() const {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				std::abort();
			}
#endif
			_node* temp{ _sentinel->_prev };
			_sentinel->_prev = temp->_prev;
			temp->_prev->_next = _sentinel;
			delete temp;
		}

		iterator insert(iterator pos, const T& value, size_t count = 1) const {
			_node* i{ pos._ptr };
			for (size_t j{ 0 }; j < count; ++j) {
				i = i->_prev = new _node{ value, i->_prev, i };
			}
			i->_prev->_next = i;
			return iterator{ i };
		}

		template<std::input_iterator iter>
		iterator insert(iterator pos, iter first, iter last) const {
			_node* i{ pos._ptr };
			while (first != last) {
				i = i->_prev = new _node{ *first++, i->_prev, i };
			}
			i->_prev->_next = i;
			return iterator{ i };
		}

		iterator erase(iterator pos) const {
			_node* i{ (++pos)._ptr };
			_node* prev{ i->_prev };
#ifdef PLASTIC_VERIFY
			if (prev == _sentinel) {
				std::abort();
			}
#endif
			i->_prev = prev->_prev;
			i->_prev->_next = i;
			delete prev;
			return pos;
		}

		iterator erase(iterator first, iterator last) const {
			_node* i{ first._ptr }, * j{ last._ptr };
			i->_prev->_next = j;
			j->_prev = i->_prev;
			while (i != j) {
				_node* next = i->_next;
				delete i;
				i = next;
			}
			return last;
		}

		std::compare_three_way_result<T> operator<=>(const list& container) const {
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