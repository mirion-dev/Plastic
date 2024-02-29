#pragma once

#include <cstddef>

namespace plastic {

	template<class T>
	class forward_list {
		struct node {
			T _value;
			node* _next;

			node* advance(::std::size_t n) {
				node* result{this};
				for (::std::size_t i{0}; i < n; ++i) {
					result = result->_next;
				}
				return result;
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
			for (node* i{begin()}; i != end();) {
				node* next{i->_next};
				delete i;
				i = next;
			}
			delete _sentinel;
		}

		bool empty() {
			return _size == 0;
		}

		bool size() {
			return _size;
		}

		node* begin() {
			return _sentinel->_next;
		}

		node* end() {
			return _sentinel;
		}

		void insert_after(node* nd, const T& value) {
			auto newNode{new node};
			newNode->_value = value;
			newNode->_next = nd->_next;
			nd->_next = newNode;
			++_size;
		}

		T erase_after(node* nd) {
			if (nd->_next == _sentinel) {
				return {};
			}
			node* next{nd->_next};
			T value{next->_value};
			nd->_next = next->_next;
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