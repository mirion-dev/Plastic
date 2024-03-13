#pragma once

#include <functional>

namespace plastic {

	template<class T, ::std::strict_weak_order<T, T> auto _compare = ::std::less<>{} >
	class avl_tree {
		struct node {
			T _value;
			::std::size_t _count;
			int _balanceFactor;
			node* _parent;
			node* _left;
			node* _right;
		};

		node* _root;
		::std::size_t _size;

		void _freeNode(node* nd) {
			if (nd == nullptr) {
				return;
			}
			_freeNode(nd->_left);
			_freeNode(nd->_right);
			delete nd;
		}

		::std::pair<node*&, node*&> _findNodeWithParent(const T& value) {
			node** pLast{&_root};
			node** p{&_root};
			while (true) {
				node*& parent{*pLast};
				node*& current{*p};
				if (current == nullptr || current->_value == value) {
					return {current, parent};
				}
				pLast = p;
				p = _compare(value, current->_value) ? &current->_left : &current->_right;
			}
		}

		node*& _findNode(const T& node) {
			return _findNodeWithParent(node).first;
		}

		node*& _minChild(node*& nd) {
			if (nd == nullptr) {
				::std::abort();
			}
			node** p{&nd};
			while (true) {
				node*& current{*p};
				if (current->_left == nullptr) {
					return current;
				}
				p = &current->_left;
			}
		}

		node*& _maxChild(node*& nd) {
			if (nd == nullptr) {
				::std::abort();
			}
			node** p{&nd};
			while (true) {
				node*& current{*p};
				if (current->_right == nullptr) {
					return current;
				}
				p = &current->_right;
			}
		}

		void _leftRotate(node* nd) {
			node* left{nd->_left};
			node* parent{nd->_parent};
			node* grandparent{parent->_parent};
			nd->_left = parent;
			nd->_parent = grandparent;
			parent->_right = left;
			parent->_parent = nd;
			if (left != nullptr) {
				left->_parent = parent;
			}
			if (grandparent != nullptr) {
				(parent == grandparent->_left ? grandparent->_left : grandparent->_right) = nd;
			}
			else {
				_root = nd;
			}
		}

		void _rightRotate(node* nd) {
			node* right{nd->_right};
			node* parent{nd->_parent};
			node* grandparent{parent->_parent};
			nd->_right = parent;
			nd->_parent = grandparent;
			parent->_left = right;
			parent->_parent = nd;
			if (right != nullptr) {
				right->_parent = parent;
			}
			if (grandparent != nullptr) {
				(parent == grandparent->_left ? grandparent->_left : grandparent->_right) = nd;
			}
			else {
				_root = nd;
			}
		}

	public:
		explicit avl_tree() {
			_root = nullptr;
			_size = 0;
		}

		~avl_tree() {
			_freeNode(_root);
		}

		bool empty() {
			return _size == 0;
		}

		::std::size_t size() {
			return _size;
		}

		::std::size_t count(const T& value) {
			node* nd{_findNode(value)};
			return nd == nullptr ? 0 : nd->_count;
		}

		bool contains(const T& value) {
			return _findNode(value) != nullptr;
		}

		void insert(const T& value, ::std::size_t count = 1) {
			_size += count;
			if (_root == nullptr) {
				_root = new node{value, count, 0, nullptr, nullptr, nullptr};
				return;
			}
			auto [nd, pr] {_findNodeWithParent(value)};
			if (nd != nullptr) {
				nd->_count += count;
				return;
			}
			nd = new node{value, count, 0, pr, nullptr, nullptr};
			node* current{nd};
			while (true) {
				if (current == _root) {
					break;
				}
				node* parent{current->_parent};
				if (current == parent->_left) {
					++parent->_balanceFactor;
				}
				else {
					--parent->_balanceFactor;
				}
				if (parent->_balanceFactor == 0) {
					break;
				}
				bool isLX{parent->_balanceFactor == 2};
				bool isRX{parent->_balanceFactor == -2};
				bool isXL{current->_balanceFactor == 1};
				if (isLX || isRX) {
					if (isLX && isXL) {
						_rightRotate(current);
						current->_balanceFactor = 0;
						parent->_balanceFactor = 0;
					}
					else if (isLX) {
						node* right{current->_right};
						_leftRotate(right);
						_rightRotate(right);
						current->_balanceFactor = right->_balanceFactor == -1 ? 1 : 0;
						parent->_balanceFactor = right->_balanceFactor == 1 ? -1 : 0;
						right->_balanceFactor = 0;

					}
					else if (isXL) {
						node* left{current->_left};
						_rightRotate(left);
						_leftRotate(left);
						current->_balanceFactor = left->_balanceFactor == -1 ? 1 : 0;
						parent->_balanceFactor = left->_balanceFactor == 1 ? -1 : 0;
						left->_balanceFactor = 0;
					}
					else {
						_leftRotate(current);
						current->_balanceFactor = 0;
						parent->_balanceFactor = 0;
					}
					break;
				}
				current = parent;
			}
		}

		void erase(const T& value, ::std::size_t count = 1) {
			node*& nd{_findNode(value)};
			if (nd == nullptr) {
				return;
			}
			if (nd->_count > count) {
				_size -= count;
				nd->_count -= count;
				return;
			}
			_size -= nd->_count;
			node** pErased{&nd};
			if (nd->_left != nullptr && nd->_right != nullptr) {
				node*& successor{_minChild(nd->_right)};
				nd->_value = successor->_value;
				nd->_count = successor->_count;
				pErased = &successor;
			}
			if (node*& erased{*pErased}, *& left{erased->_left}, *& right{erased->_right};
				left != nullptr) {
				erased->_value = left->_value;
				erased->_count = left->_count;
				erased->_balanceFactor = 0;
				pErased = &left;
			}
			else if (right != nullptr) {
				erased->_value = right->_value;
				erased->_count = right->_count;
				erased->_balanceFactor = 0;
				pErased = &right;
			}
			node*& erased{*pErased};
			node* current{erased};
			while (true) {
				if (current == _root) {
					break;
				}
				node* parent{current->_parent};
				if (current == parent->_left) {
					--parent->_balanceFactor;
				}
				else {
					++parent->_balanceFactor;
				}
				if (parent->_balanceFactor == 1 || parent->_balanceFactor == -1) {
					break;
				}
				bool isLX{parent->_balanceFactor == 2};
				bool isRX{parent->_balanceFactor == -2};
				if (isLX || isRX) {
					node* sibling{isLX ? parent->_left : parent->_right};
					bool isXL{isLX && sibling->_balanceFactor != -1 || isRX && sibling->_balanceFactor == 1};
					if (isLX && isXL) {
						_rightRotate(sibling);
						if (sibling->_balanceFactor == 0) {
							sibling->_balanceFactor = -1;
							parent->_balanceFactor = 1;
							break;
						}
						sibling->_balanceFactor = 0;
						parent->_balanceFactor = 0;
					}
					else if (isLX) {
						node* right{sibling->_right};
						_leftRotate(right);
						_rightRotate(right);
						sibling->_balanceFactor = right->_balanceFactor == -1 ? 1 : 0;
						parent->_balanceFactor = right->_balanceFactor == 1 ? -1 : 0;
						right->_balanceFactor = 0;
					}
					else if (isXL) {
						node* left{sibling->_left};
						_rightRotate(left);
						_leftRotate(left);
						sibling->_balanceFactor = left->_balanceFactor == -1 ? 1 : 0;
						parent->_balanceFactor = left->_balanceFactor == 1 ? -1 : 0;
						left->_balanceFactor = 0;
					}
					else {
						_leftRotate(sibling);
						if (sibling->_balanceFactor == 0) {
							sibling->_balanceFactor = 1;
							parent->_balanceFactor = -1;
							break;
						}
						sibling->_balanceFactor = 0;
						parent->_balanceFactor = 0;
					}
				}
				current = parent;

			}
			node* temp{erased};
			erased = nullptr;
			delete temp;
		}

		T min() {
			return _minChild(_root)->_value;
		}

		T max() {
			return _maxChild(_root)->_value;
		}
	};

}