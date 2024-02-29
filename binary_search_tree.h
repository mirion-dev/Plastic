#pragma once

#include <functional>

namespace plastic {

	template<class T, ::std::strict_weak_order<T, T> auto _compare = ::std::less<>{} >
	class binary_search_tree {
		struct node {
			T _value;
			::std::size_t _count;
			node* _parent;
			node* _left;
			node* _right;
		};

		node* _root;
		::std::size_t _size;

	public:
		explicit binary_search_tree() {
			_root = nullptr;
			_size = 0;
		}

		~binary_search_tree() {
			// TODO
		}

		bool empty() {
			return _size == 0;
		}

		::std::size_t size() {
			return _size;
		}

		void insert(const T& value) {
			for (node* i{_root};;) {
				if (i == nullptr) {
					i = new node;
					i->_value = value;
					i->_count = 1;
					i->_left = nullptr;
					i->_right = nullptr;
					break;
				}
				if (value == i->_value) {
					++i->_count;
					break;
				}
				if (_compare(value, i->_value)) {
					i = i->_left;
				}
				else {
					i = i->_right;
				}
			}
			++_size;
		}

		void erase(const T& value) {
			/*if (nd == nullptr) {
				return;
			}
			if (nd->_left == nullptr) {
				node* temp{nd->_right};
				delete nd;
				nd = temp;
			}
			else if (nd->_right == nullptr) {
				node* temp{nd->_left};
				delete nd;
				nd = temp;
			}
			else {
				node* i{nd->_left};
				while (true) {
					node* t{i->_right};
					if (t == nullptr) {
						break;
					}
					i = t;
				}
				i->_parent->_right = i->_left;
				nd = i;
			}*/
		}

		node* find(const T& value) {
			for (node* i{_root}; i != nullptr;) {
				if (value == i->_value) {
					return i;
				}
				if (_compare(value, i->_value)) {
					i = i->_left;
				}
				else {
					i = i->_right;
				}
			}
			return nullptr;
		}

		node* max_element() {
			if (_root == nullptr) {
				return nullptr;
			}
			for (node* i{_root};;) {
				node* temp{i->_right};
				if (temp == nullptr) {
					return i;
				}
				i = temp;
			}
		}

		node* min_element() {
			if (_root == nullptr) {
				return nullptr;
			}
			for (node* i{_root};;) {
				node* temp{i->_left};
				if (temp == nullptr) {
					return i;
				}
				i = temp;
			}
		}
	};

}