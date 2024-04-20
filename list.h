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

	public:
		class iterator {
			friend list;

			_node* _ptr;

		public:
			using difference_type = ::std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;
			using iterator_category = ::std::bidirectional_iterator_tag;

			explicit iterator(_node* node) noexcept {
				_ptr = node;
			}

			reference operator*() const noexcept {
				return _ptr->_value;
			}

			pointer operator->() const noexcept {
				return &_ptr->_value;
			}

			bool operator==(iterator it) const noexcept {
				return _ptr == it._ptr;
			}

			iterator& operator++() noexcept {
				_ptr = _ptr->_next;
				return *this;
			}

			iterator operator++(int) noexcept {
				iterator temp{*this};
				++*this;
				return temp;
			}

			iterator& operator--() noexcept {
				_ptr = _ptr->_prev;
				return *this;
			}

			iterator operator--(int) noexcept {
				iterator temp{*this};
				--*this;
				return temp;
			}
		};

	private:
		_node* _sentinel;

	public:
		explicit list(::std::size_t count = 0, const T& value = {}) noexcept {
			_sentinel = new _node;
			_sentinel->_next = _sentinel->_prev = _sentinel;
			resize(count, value);
		}

		template<::std::input_iterator iter>
		explicit list(iter first, iter last) noexcept {
			_sentinel = new _node;
			_sentinel->_next = _sentinel->_prev = _sentinel;
			while (first != last) {
				_sentinel->_next = new _node{*--last, _sentinel, _sentinel->_next};
			}
		}

		~list() noexcept {
			_node* i{_sentinel->_next};
			while (i != _sentinel) {
				_node* next{i->_next};
				delete i;
				i = next;
			}
			delete i;
		}

		bool empty() const noexcept {
			return _sentinel->_next == _sentinel;
		}

		void resize(::std::size_t count, const T& value = {}) const noexcept {
			for (::std::size_t i{0}; i < count; ++i) {
				_sentinel->_next = new _node{value, _sentinel, _sentinel->_next};
			}
		}

		iterator begin() const noexcept {
			return iterator{_sentinel->_next};
		}

		iterator end() const noexcept {
			return iterator{_sentinel};
		}

		T& front() const noexcept {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				::std::abort();
			}
#endif
			return _sentinel->_next->_value;
		}

		T& back() const noexcept {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				::std::abort();
			}
#endif
			return _sentinel->_prev->_value;
		}

		void push_front(const T& value) const noexcept {
			_sentinel->_next->_next->_prev = _sentinel->_next = new _node{value, _sentinel, _sentinel->_next};
		}

		void pop_front() const noexcept {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				::std::abort();
			}
#endif
			_node* temp{_sentinel->_next};
			_sentinel->_next = temp->_next;
			temp->_next->_prev = _sentinel;
			delete temp;
		}

		void push_back(const T& value) const noexcept {
			_sentinel->_prev->_prev->_next = _sentinel->_prev = new _node{value, _sentinel->_prev, _sentinel};
		}

		void pop_back() const noexcept {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				::std::abort();
			}
#endif
			_node* temp{_sentinel->_prev};
			_sentinel->_prev = temp->_prev;
			temp->_prev->_next = _sentinel;
			delete temp;
		}

		iterator insert(iterator pos, const T& value, ::std::size_t count = 1) const noexcept {
			_node* i{pos._ptr};
			for (::std::size_t j{0}; j < count; ++j) {
				i = i->_prev = new _node{value, i->_prev, i};
			}
			i->_prev->_next = i;
			return iterator{i};
		}

		template<::std::input_iterator iter>
		iterator insert(iterator pos, iter first, iter last) const noexcept {
			_node* i{pos._ptr};
			while (first != last) {
				i = i->_prev = new _node{*first++, i->_prev, i};
			}
			i->_prev->_next = i;
			return iterator{i};
		}

		iterator erase(iterator pos) const noexcept {
			_node* i{(++pos)._ptr};
			_node* prev{i->_prev};
#ifdef PLASTIC_VERIFY
			if (prev == _sentinel) {
				::std::abort();
			}
#endif
			i->_prev = prev->_prev;
			i->_prev->_next = i;
			delete prev;
			return pos;
		}

		iterator erase(iterator first, iterator last) const noexcept {
			_node* i{first._ptr}, * j{last._ptr};
			i->_prev->_next = j;
			j->_prev = i->_prev;
			while (i != j) {
				_node* next = i->_next;
				delete i;
				i = next;
			}
			return last;
		}
	};

}