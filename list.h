#pragma once

#include <cstddef>

namespace plastic {

	template<class T>
	class list {
		struct node {
			T _value;
			node* _prev;
			node* _next;
		};

		struct iterator {
			node* _node;

			explicit iterator(node* nd) {
				_node = nd;
			}

			T& operator*() {
				return _node->_value;
			}

			bool operator==(iterator it) {
				return _node == it._node;
			}

			iterator& operator++() {
				_node = _node->_next;
				return *this;
			}

			iterator operator++(int) {
				iterator temp{*this};
				operator++();
				return temp;
			}

			iterator& operator--() {
				_node = _node->_prev;
				return *this;
			}

			iterator operator--(int) {
				iterator temp{*this};
				operator--();
				return temp;
			}
		};

		node* _sentinel;
		::std::size_t _size;

	public:
		explicit list() {
			_sentinel = new node;
			_sentinel->_next = _sentinel;
			_sentinel->_prev = _sentinel;
			_size = 0;
		}

		~list() {
			iterator i{begin()};
			while (i != end()) {
				delete i++._node;
			}
			delete i._node;
		}

		bool empty() {
			return _size == 0;
		}

		bool size() {
			return _size;
		}

		iterator begin() {
			return iterator{_sentinel->_next};
		}

		iterator end() {
			return iterator{_sentinel};
		}

		void insert(iterator pos, const T& value) {
			node* posNode{pos._node};
			node* newNode{new node{value, posNode->_prev, posNode}};
			posNode->_prev->_next = newNode;
			posNode->_prev = newNode;
			++_size;
		}

		T erase(iterator pos) {
			T value{*pos};
			node* posNode{pos._node};
			posNode->_prev->_next = posNode->_next;
			posNode->_next->_prev = posNode->_prev;
			delete posNode;
			--_size;
			return value;
		}

		void push_front(const T& value) {
			insert(begin(), value);
		}

		void push_back(const T& value) {
			insert(end(), value);
		}

		T pop_front() {
			return erase(begin());
		}

		T pop_back() {
			return erase(--end());
		}
	};

}