#pragma once

#include <functional>

namespace plastic {

	template<class T, std::strict_weak_order<T, T> auto _compare = std::less<>{} >
	class binary_search_tree {
		struct node {
			T _value;
			size_t _count;
			node* _left;
			node* _right;
		};

		node* _root;
		size_t _size;

		void _freeNode(node* nd) {
			if (nd == nullptr) {
				return;
			}
			_freeNode(nd->_left);
			_freeNode(nd->_right);
			delete nd;
		}

		node*& _findNode(const T& value) {
			node** p{ &_root };
			while (true) {
				node*& current{ *p };
				if (current == nullptr || current->_value == value) {
					return current;
				}
				p = _compare(value, current->_value) ? &current->_left : &current->_right;
			}
		}

		node*& _minChild(node*& nd) {
			if (nd == nullptr) {
				std::abort();
			}
			node** p{ &nd };
			while (true) {
				node*& current{ *p };
				if (current->_left == nullptr) {
					return current;
				}
				p = &current->_left;
			}
		}

		node*& _maxChild(node*& nd) {
			if (nd == nullptr) {
				std::abort();
			}
			node** p{ &nd };
			while (true) {
				node*& current{ *p };
				if (current->_right == nullptr) {
					return current;
				}
				p = &current->_right;
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

		bool empty() const {
			return _size == 0;
		}

		size_t size() const {
			return _size;
		}

		size_t count(const T& value) const {
			node* nd{ _findNode(value) };
			return nd == nullptr ? 0 : nd->_count;
		}

		bool contains(const T& value) const {
			return _findNode(value) != nullptr;
		}

		T min() const {
			return _minChild(_root)->_value;
		}

		T max() const {
			return _maxChild(_root)->_value;
		}

		void insert(const T& value, size_t count = 1) {
			_size += count;
			node*& nd{ _findNode(value) };
			if (nd == nullptr) {
				nd = new node{ value, count, nullptr, nullptr };
			}
			else {
				nd->_count += count;
			}
		}

		void erase(const T& value, size_t count = 1) {
			node*& nd{ _findNode(value) };
			if (nd == nullptr) {
				return;
			}
			if (nd->_count > count) {
				_size -= count;
				nd->_count -= count;
				return;
			}
			_size -= nd->_count;
			if (nd->_left == nullptr) {
				node* temp{ nd };
				nd = nd->_right;
				delete temp;
			}
			else if (nd->_right == nullptr) {
				node* temp{ nd };
				nd = nd->_left;
				delete temp;
			}
			else {
				node*& successor{ _minChild(nd->_right) };
				nd->_value = successor->_value;
				nd->_count = successor->_count;
				node* temp{ successor };
				successor = successor->_right;
				delete temp;
			}
		}
	};

}