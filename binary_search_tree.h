#pragma once

#include <functional>

namespace plastic {

	template<class T, ::std::strict_weak_order<T, T> auto _compare = ::std::less<>{} >
	class binary_search_tree {
		struct node {
			T _value;
			::std::size_t _count;
			node* _left;
			node* _right;
		};

		node* _root;
		::std::size_t _size;

		node** _findNode(const T& value) {
			node** i{&_root};
			while (true) {
				if (*i == nullptr) {
					return nullptr;
				}
				if (value == (*i)->_value) {
					return i;
				}
				if (_compare(value, (*i)->_value)) {
					i = &(*i)->_left;
				}
				else {
					i = &(*i)->_right;
				}
			}
		}

		void _eraseNode(node*& nd) {
			if (nd->_left == nullptr) {
				node* temp{nd};
				nd = nd->_right;
				delete temp;
			}
			else if (nd->_right == nullptr) {
				node* temp{nd};
				nd = nd->_left;
				delete temp;
			}
			else {
				node** replaced{_minChild(nd->_right)};
				nd->_value = (*replaced)->_value;
				nd->_count = (*replaced)->_count;
				node* temp{*replaced};
				*replaced = (*replaced)->_right;
				delete temp;
			}
		}

		void _freeNode(node*& nd) {
			if (nd == nullptr) {
				return;
			}
			_freeNode(nd->_left);
			_freeNode(nd->_right);
			delete nd;
		}

		node** _minChild(node*& nd) {
			node** i{&nd};
			while (true) {
				node** temp{&(*i)->_left};
				if (*temp == nullptr) {
					return i;
				}
				i = temp;
			}
		}

		node** _maxChild(node*& nd) {
			node** i{&nd};
			while (true) {
				node** temp{&(*i)->_right};
				if (*temp == nullptr) {
					return i;
				}
				i = temp;
			}
		}

	public:
		explicit binary_search_tree() {
			_root = nullptr;
			_size = 0;
		}

		~binary_search_tree() {
			_freeNode(_root);
		}

		bool empty() {
			return _size == 0;
		}

		::std::size_t size() {
			return _size;
		}

		void insert(const T& value, ::std::size_t count = 1) {
			node** i{&_root};
			while (true) {
				if (*i == nullptr) {
					*i = new node{value, count, nullptr, nullptr};
					break;
				}
				if (value == (*i)->_value) {
					(*i)->_count += count;
					break;
				}
				if (_compare(value, (*i)->_value)) {
					i = &(*i)->_left;
				}
				else {
					i = &(*i)->_right;
				}
			}
			_size += count;
		}

		void erase(const T& value, ::std::size_t count = 1) {
			node** nd{_findNode(value)};
			if (nd == nullptr) {
				return;
			}
			if ((*nd)->_count > count) {
				(*nd)->_count -= count;
				_size -= count;
				return;
			}
			_size -= (*nd)->_count;
			_eraseNode(*nd);
		}

		::std::size_t count(const T& value) {
			node** nd{_findNode(value)};
			return nd == nullptr ? 0 : (*nd)->_count;
		}

		T min() {
			if (_root == nullptr) {
				return {};
			}
			return (*_minChild(_root))->_value;
		}

		T max() {
			if (_root == nullptr) {
				return {};
			}
			return (*_maxChild(_root))->_value;
		}
	};

}