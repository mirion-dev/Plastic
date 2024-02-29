#pragma once

#include <cstddef>

namespace plastic {

	template<class T>
	class list {
		struct node {
			T _value;
			node* _prev;
			node* _next;

			node* advance(::std::ptrdiff_t n) {
				node* result{this};
				if (n < 0) {
					for (::std::ptrdiff_t i{0}; i > n; --i) {
						result = result->_prev;
					}
				}
				else if (n > 0) {
					for (::std::ptrdiff_t i{0}; i < n; ++i) {
						result = result->_next;
					}
				}
				return result;
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

		void insert(node* nd, const T& value) {
			auto newNode{new node};
			newNode->_value = value;
			newNode->_prev = nd->_prev;
			newNode->_next = nd;
			nd->_prev->_next = newNode;
			nd->_prev = newNode;
			++_size;
		}

		T erase(node* nd) {
			if (nd == _sentinel) {
				return {};
			}
			T value{nd->_value};
			nd->_prev->_next = nd->_next;
			nd->_next->_prev = nd->_prev;
			delete nd;
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
			return erase(end()->_prev);
		}
	};

}