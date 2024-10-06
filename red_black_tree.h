#pragma once

#include <functional>

namespace plastic {

	template<class T, std::strict_weak_order<T, T> auto _compare = std::less<>{} >
	class red_black_tree {
		struct node {
			enum class color {
				red,
				black
			};

			T _value;
			size_t _count;
			color _color;
			node* _parent;
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

		std::pair<node*&, node*&> _findNodeWithParent(const T& value) {
			node** pLast{ &_root };
			node** p{ &_root };
			while (true) {
				node*& parent{ *pLast };
				node*& current{ *p };
				if (current == nullptr || current->_value == value) {
					return { current, parent };
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

		void _leftRotate(node* nd) {
			node* left{ nd->_left };
			node* parent{ nd->_parent };
			node* grandparent{ parent->_parent };
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
			node* right{ nd->_right };
			node* parent{ nd->_parent };
			node* grandparent{ parent->_parent };
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
			if (_root == nullptr) {
				_root = new node{ value, count, node::color::black, nullptr, nullptr, nullptr };
				return;
			}
			auto [nd, pr] {_findNodeWithParent(value)};
			if (nd != nullptr) {
				nd->_count += count;
				return;
			}
			nd = new node{ value, count, node::color::red, pr, nullptr, nullptr };
			node* current{ nd };
			while (true) {
				if (current == _root) {
					current->_color = node::color::black;
					break;
				}
				node* parent{ current->_parent };
				if (parent->_color == node::color::black) {
					break;
				}
				node* grandparent{ parent->_parent };
				bool isXL{ current == parent->_left };
				bool isLX{ parent == grandparent->_left };
				node* uncle{ isLX ? grandparent->_right : grandparent->_left };
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
					break;
				}
				parent->_color = node::color::black;
				uncle->_color = node::color::black;
				grandparent->_color = node::color::red;
				current = grandparent;
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
			node** pErased{ &nd };
			if (nd->_left != nullptr && nd->_right != nullptr) {
				node*& successor{ _minChild(nd->_right) };
				nd->_value = successor->_value;
				nd->_count = successor->_count;
				pErased = &successor;
			}
			node*& erased{ *pErased };
			if (erased->_left != nullptr) {
				node* temp{ erased };
				erased = erased->_left;
				erased->_color = node::color::black;
				erased->_parent = temp->_parent;
				delete temp;
				return;
			}
			if (erased->_right != nullptr) {
				node* temp{ erased };
				erased = erased->_right;
				erased->_color = node::color::black;
				erased->_parent = temp->_parent;
				delete temp;
				return;
			}
			node* current{ erased };
			while (true) {
				if (current == _root || current->_color == node::color::red) {
					break;
				}
				node* parent{ current->_parent };
				bool isLX{ current == parent->_right };
				bool isRX{ !isLX };
				node* sibling{ isLX ? parent->_left : parent->_right };
				if (sibling->_color == node::color::red) {
					sibling->_color = node::color::black;
					parent->_color = node::color::red;
					if (isLX) {
						_rightRotate(sibling);
						sibling = parent->_left;
					}
					else {
						_leftRotate(sibling);
						sibling = parent->_right;
					}
				}
				node* left{ sibling->_left };
				node* right{ sibling->_right };
				bool isXL{ left != nullptr && left->_color == node::color::red };
				bool isXR{ right != nullptr && right->_color == node::color::red };
				if (isXL || isXR) {
					if (isLX && isXL) {
						_rightRotate(sibling);
						sibling->_color = parent->_color;
						left->_color = node::color::black;
					}
					else if (isRX && isXR) {
						_leftRotate(sibling);
						sibling->_color = parent->_color;
						right->_color = node::color::black;
					}
					else if (isLX) {
						_leftRotate(right);
						_rightRotate(right);
						right->_color = parent->_color;
					}
					else {
						_rightRotate(left);
						_leftRotate(left);
						left->_color = parent->_color;
					}
					parent->_color = node::color::black;
					break;
				}
				bool isRed{ parent->_color == node::color::red };
				parent->_color = node::color::black;
				sibling->_color = node::color::red;
				if (isRed) {
					break;
				}
				current = parent;
			}
			node* temp{ erased };
			erased = nullptr;
			delete temp;
		}
	};

}