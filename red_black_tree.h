#pragma once

#include <functional>

namespace plastic {

	template<class T, ::std::strict_weak_order<T, T> auto _compare = ::std::less<>{} >
	class red_black_tree {
		struct node {
			enum class color {
				red,
				black
			};

			T _value;
			::std::size_t _count;
			color _color;
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
		explicit red_black_tree() {
			_root = nullptr;
			_size = 0;
		}

		~red_black_tree() {
			_freeNode(_root);
		}

		bool empty() {
			return _size == 0;
		}

		::std::size_t size() {
			return _size;
		}

		bool find(const T& value) {
			return _findNode(value) != nullptr;
		}

		::std::size_t count(const T& value) {
			node* nd{_findNode(value)};
			return nd == nullptr ? 0 : nd->_count;
		}

		void insert(const T& value, ::std::size_t count = 1) {
			_size += count;
			if (_root == nullptr) {
				_root = new node{value, count, node::color::black, nullptr, nullptr, nullptr};
				return;
			}
			auto [nd, pr] {_findNodeWithParent(value)};
			if (nd != nullptr) {
				nd->_count += count;
				return;
			}
			nd = new node{value, count, node::color::red, pr, nullptr, nullptr};
			node* current{nd};
			while (true) {
				if (current == _root) {
					current->_color = node::color::black;
					return;
				}
				node* parent{current->_parent};
				if (parent->_color == node::color::black) {
					return;
				}
				node* grandparent{parent->_parent};
				bool isXL{current == parent->_left};
				bool isLX{parent == grandparent->_left};
				node* uncle{isLX ? grandparent->_right : grandparent->_left};
				if (uncle == nullptr || uncle->_color == node::color::black) {
					if (isLX && isXL) {
						_rightRotate(parent);
						parent->_color = node::color::black;
					}
					else if (isLX) {
						_leftRotate(current);
						_rightRotate(current);
						current->_color = node::color::black;
					}
					else if (isXL) {
						_rightRotate(current);
						_leftRotate(current);
						current->_color = node::color::black;
					}
					else {
						_leftRotate(parent);
						parent->_color = node::color::black;
					}
					grandparent->_color = node::color::red;
					return;
				}
				parent->_color = node::color::black;
				uncle->_color = node::color::black;
				grandparent->_color = node::color::red;
				current = grandparent;
			}
		}

		void erase(const T& value, ::std::size_t count = 1) {
			node** pnd{&_findNode(value)};
			node*& nd{*pnd};
			if (nd == nullptr) {
				return;
			}
			if (nd->_count > count) {
				nd->_count -= count;
				_size -= count;
				return;
			}
			_size -= nd->_count;
			if (nd->_left != nullptr && nd->_right != nullptr) {
				node*& next{_minChild(nd->_right)};
				nd->_value = next->_value;
				nd->_count = next->_count;
				pnd = &next;
			}
			if (nd->_left != nullptr) {
				node* temp{nd};
				nd = nd->_left;
				nd->_color = node::color::black;
				delete temp;
				return;
			}
			if (nd->_right != nullptr) {
				node* temp{nd};
				nd = nd->_right;
				nd->_color = node::color::black;
				delete temp;
				return;
			}
			node* current{nd};
			while (true) {
				if (current == _root || current->_color == node::color::red) {
					return;
				}
				node* parent{current->_parent};
				bool isLX{current == parent->_left};
				node* sibling{isLX ? parent->_right : parent->_left};
				bool isXL{sibling->_left != nullptr && sibling->_left->_color == node::color::red};
				bool isBlack{(sibling->_left == nullptr || sibling->_left->_color == node::color::black)
						&& (sibling->_right == nullptr || sibling->_right->_color == node::color::black)};
				if (sibling->_color == node::color::black && isBlack) {
					bool isBlack{parent->_color == node::color::black};
					parent->_color = node::color::black;
					sibling->_color = node::color::red;
					if (isBlack) {
						current = parent;
						continue;
					}
					break;
				}
				if (sibling->_color == node::color::red) {
					sibling->_color = node::color::black;
					parent->_color = node::color::red;
					if (isLX) {
						_rightRotate(sibling);
					}
					else {
						_leftRotate(sibling);
					}
				}
				if (sibling->_color == node::color::black) {
					if (isLX && isXL) {
						sibling->_color = parent->_color;
						sibling->_left->_color = node::color::black;
						_rightRotate(sibling->_left);
					}
					else if (!isLX && !isXL) {
						sibling->_color = parent->_color;
						sibling->_right->_color = node::color::black;
						_leftRotate(sibling->_right);
					}
					else if (isLX) {
						sibling->_right->_color = parent->_color;
						_leftRotate(sibling->_right);
						_rightRotate(sibling->_right);
					}
					else {
						sibling->_left->_color = parent->_color;
						_rightRotate(sibling->_left);
						_leftRotate(sibling->_left);
					}
					parent->_color = node::color::black;
				}
			}
		}

		T min() {
			return _minChild(_root)->_value;
		}

		T max() {
			return _maxChild(_root)->_value;
		}
	};

}