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

		void _freeNode(node*& nd) {
			if (nd == nullptr) {
				return;
			}
			_freeNode(nd->_left);
			_freeNode(nd->_right);
			delete nd;
		}

		::std::pair<node*&, node*&> _findNodeWithParent(const T& value) {
			node** i{&_root};
			node** j{&_root->_right};
			while (true) {
				node*& parent{*i};
				node*& current{*j};
				if (current == nullptr || value == current->_value) {
					return {current, parent};
				}
				i = j;
				if (_compare(value, current->_value)) {
					j = &current->_left;
				}
				else {
					j = &current->_right;
				}
			}
		}

		node*& _findNode(const T& node) {
			return _findNodeWithParent(node).first;
		}

		node*& _minChild(node*& nd) {
			node** i{&nd};
			while (true) {
				node*& current{*i};
				node*& left{current->_left};
				if (left == nullptr) {
					return current;
				}
				i = &left;
			}
		}

		node*& _maxChild(node*& nd) {
			node** i{&nd};
			while (true) {
				node*& current{*i};
				node*& right{current->_right};
				if (right == nullptr) {
					return current;
				}
				i = &right;
			}
		}

		void _leftRotate(node*& nd) {
			node* left{nd->_left};
			node* parent{nd->_parent};
			node* grandparent{parent->_parent};
			parent->_parent = nd;
			nd->_left = parent;
			nd->_parent = grandparent;
			if (grandparent->_left == parent) {
				grandparent->_left = nd;
			}
			else {
				grandparent->_right = nd;
			}
			if (left != nullptr) {
				left->_parent = parent;
			}
			parent->_right = left;
		}

		void _rightRotate(node*& nd) {
			node* right{nd->_right};
			node* parent{nd->_parent};
			node* grandparent{parent->_parent};
			parent->_parent = nd;
			nd->_right = parent;
			nd->_parent = grandparent;
			if (grandparent->_left == parent) {
				grandparent->_left = nd;
			}
			else {
				grandparent->_right = nd;
			}
			if (right != nullptr) {
				right->_parent = parent;
			}
			parent->_left = right;
		}

	public:
		explicit red_black_tree() {
			_root = new node{{}, 0, node::color::black, nullptr, nullptr, nullptr};
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
			node*& nd{_findNode(value)};
			return nd == nullptr ? 0 : nd->_count;
		}

		void insert(const T& value, ::std::size_t count = 1) {
			auto [nd, pr] {_findNodeWithParent(value)};
			if (nd == nullptr) {
				nd = new node{value, count, node::color::red, pr, nullptr, nullptr};
				node* current{nd};
				while (true) {
					if (current == _root->_right) {
						current->_color = node::color::black;
						break;
					}
					node* parent{current->_parent};
					if (parent->_color == node::color::black) {
						break;
					}
					node* grandparent{parent->_parent};
					bool isXL{current == parent->_left};
					bool isLX{parent == grandparent->_left};
					node* uncle{isLX ? grandparent->_right : grandparent->_left};
					if (uncle != nullptr && uncle->_color == node::color::red) {
						parent->_color = node::color::black;
						uncle->_color = node::color::black;
						grandparent->_color = node::color::red;
						current = grandparent;
					}
					else {
						grandparent->_color = node::color::red;
						if (isLX && isXL) {
							parent->_color = node::color::black;
							_rightRotate(parent);
						}
						else if (isLX && !isXL) {
							current->_color = node::color::black;
							_leftRotate(current);
							_rightRotate(current);
						}
						else if (!isLX && isXL) {
							current->_color = node::color::black;
							_rightRotate(current);
							_leftRotate(current);
						}
						else {
							parent->_color = node::color::black;
							_leftRotate(parent);
						}
						break;
					}
				}
			}
			else {
				nd->_count += count;
			}
			_size += count;
		}

		void erase(const T& value, ::std::size_t count = 1) {

		}

		T min() {
			if (_root->_right == nullptr) {
				return {};
			}
			return _minChild(_root->_right)->_value;
		}

		T max() {
			if (_root->_right == nullptr) {
				return {};
			}
			return _maxChild(_root->_right)->_value;
		}
	};

}