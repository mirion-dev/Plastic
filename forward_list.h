#pragma once

#include <cstddef>

namespace plastic {

	template<class T>
	class forward_list {
		struct _node {
			T _value;
			_node* _next;
		};

	public:
		class iterator {
			_node* _nd;

			friend forward_list;

		public:
			explicit iterator(_node* node) noexcept {
				_nd = node;
			}

			T& operator*() const noexcept {
				return _nd->_value;
			}

			bool operator==(iterator it) const noexcept {
				return _nd == it._nd;
			}

			iterator& operator++() noexcept {
				_nd = _nd->_next;
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

		~forward_list() noexcept {
			_node* i{_sentinel->_next};
			while (i != _sentinel) {
				_node* temp{i->_next};
				delete i;
				i = temp;
			}
			delete i;
		}

		bool empty() const noexcept {
			return _sentinel->_next == _sentinel;
		}

		void resize(::std::size_t count, const T& value = {}) noexcept {
			// TODO
		}

		iterator begin() const noexcept {
			return iterator{_sentinel->_next};
		}

		iterator end() const noexcept {
			return iterator{_sentinel};
		}

		T& front() noexcept {
#ifdef PLASTIC_VERIFY
			if (empty()) {
				::std::abort();
			}
#endif
			return begin()->_data;
		}

		void push_front(const T& value) noexcept {
			insert_after(end(), value);
		}

		void pop_front() noexcept {
			erase_after(end());
		}

		void insert_after(iterator pos, const T& value) noexcept {
			_node* posNode{pos._nd};
			_node* newNode{new _node{value, posNode->_next}};
			posNode->_next = newNode;
		}

		void erase_after(iterator pos) noexcept {
			_node* posNode{pos._nd};
			_node* temp{posNode->_next};
#ifdef PLASTIC_VERIFY
			if (temp == _sentinel) {
				::std::abort();
			}
#endif
			posNode->_next = temp->_next;
			delete temp;
		}
	};

}