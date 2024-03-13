#pragma once

#include <cstddef>

namespace plastic {

	template<class T>
	class forward_list {
		struct node {
			T _value;
			node* _next;
		};

		struct iterator {
			node* _node;
			node* _sentinel;

			explicit iterator(node* nd, node* sentinel) {
				_node = nd;
				_sentinel = sentinel;
			}

			T& operator*() {
				if (_node == _sentinel) {
					::std::abort();
				}
				return _node->_value;
			}

			bool operator==(iterator it) {
				return _node == it._node;
			}

			iterator& operator++() {
				if (_node == _sentinel) {
					::std::abort();
				}
				_node = _node->_next;
				return *this;
			}

			iterator operator++(int) {
				iterator temp{*this};
				operator++();
				return temp;
			}
		};

		node* _sentinel;
		::std::size_t _size;

	public:
		explicit forward_list() {
			_sentinel = new node;
			_sentinel->_next = _sentinel;
			_size = 0;
		}

		~forward_list() {
			for (node* i{_sentinel->_next}; i != _sentinel;) {
				node* temp{i->_next};
				delete i;
				i = temp;
			}
			delete _sentinel;
		}

		bool empty() {
			return _size == 0;
		}

		bool size() {
			return _size;
		}

		iterator begin() {
			return iterator{_sentinel->_next, _sentinel};
		}

		iterator end() {
			return iterator{_sentinel, _sentinel};
		}

		void insert_after(iterator pos, const T& value) {
			node* posNode{pos._node};
			node* newNode{new node{value, posNode->_next}};
			posNode->_next = newNode;
			++_size;
		}

		T erase_after(iterator pos) {
			node* posNode{pos._node};
			node* next{posNode->_next};
			if (next == _sentinel) {
				::std::abort();
			}
			T value{next->_value};
			posNode->_next = next->_next;
			delete next;
			--_size;
			return value;
		}

		void push_front(const T& value) {
			insert_after(end(), value);
		}

		T pop_front() {
			return erase_after(end());
		}
	};

}