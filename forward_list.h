#pragma once

#include <iterator>

namespace plastic {

	template<class T>
	class forward_list {
		struct _node {
			T _value;
			_node* _next;
		};

	public:
		class iterator {
			friend forward_list;

			_node* _ptr;

		public:
			using difference_type = ::std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;
			using iterator_category = ::std::input_iterator_tag;

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
		};

	private:
		_node* _sentinel;

	public:
		explicit forward_list() noexcept {
			_sentinel = new _node;
			_sentinel->_next = _sentinel;
		}

		template<::std::input_iterator iter>
		explicit forward_list(iter first, iter last) noexcept {
			_sentinel = new _node;
			_sentinel->_next = _sentinel;
			while (first != last) {
				_sentinel = new _node{*first++, _sentinel};
			}
		}

		~forward_list() noexcept {
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

		void resize(::std::size_t count, const T& value = {}) noexcept {
			for (::std::size_t i{0}; i < count; ++i) {
				_sentinel = new _node{value, _sentinel};
			}
		}

		iterator begin() noexcept {
			return iterator{_sentinel->_next};
		}

		iterator end() noexcept {
			return iterator{_sentinel};
		}

		T& front() noexcept {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				::std::abort();
			}
#endif
			return _sentinel->_next->_value;
		}

		void push_front(const T& value) noexcept {
			_sentinel->_next = new _node{value, _sentinel->_next};
		}

		void pop_front() noexcept {
			_node* temp{_sentinel->_next};
			_sentinel->_next = temp->_next;
			delete temp;
		}

		iterator insert_after(iterator pos, const T& value, std::size_t count = 1) noexcept {
			_node* i{pos._ptr};
			for (::std::size_t j{0}; j < count; ++j) {
				i = i->_next = new _node{value, i->_next};
			}
			return iterator{i};
		}

		template<::std::input_iterator iter>
		iterator insert_after(iterator pos, iter first, iter last) noexcept {
			_node* i{pos._ptr};
			while (first != last) {
				i = i->_next = new _node{*first++, i->_next};
			}
			return iterator{i};
		}

		iterator erase_after(iterator pos) noexcept {
			_node* i{pos._ptr};
			_node* next{i->_next};
#ifdef PLASTIC_VERIFY
			if (next == _sentinel) {
				::std::abort();
			}
#endif
			i->_next = next->_next;
			delete next;
			return ++pos;
		}

		iterator erase_after(iterator first, iterator last) noexcept {
			if (first == last) {
				return last;
			}
			_node* i{first._ptr}, * j{last._ptr};
			_node* next{i->_next};
#ifdef PLASTIC_VERIFY
			if (next == _sentinel) {
				::std::abort();
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
	};

}