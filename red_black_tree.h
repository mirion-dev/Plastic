#pragma once

#include "utils.h"

#include <functional>

namespace plastic {

	template<class T, class Cmp = std::less<T>>
	class red_black_tree {
		struct node {
			enum class color {
				red,
				black
			};

			T value;
			size_t count;
			color color;
			node* parent;
			node* left;
			node* right;
		};

		static constexpr Cmp _cmp{};

		node* _root;
		size_t _size;

		void _free(node* nd) {
			if (nd == nullptr) {
				return;
			}
			_free(nd->left);
			_free(nd->right);
			delete nd;
		}

		std::pair<node*&, node*&> _findWithParent(const T& value) {
			node** pLast{ &_root };
			node** p{ &_root };
			while (true) {
				node*& parent{ *pLast };
				node*& current{ *p };
				if (current == nullptr || current->value == value) {
					return { current, parent };
				}
				pLast = p;
				p = _cmp(value, current->value) ? &current->left : &current->right;
			}
		}

		node*& _find(const T& node) {
			return _findWithParent(node).first;
		}

		node*& _min(node*& nd) {
			PLASTIC_VERIFY(nd != nullptr);
			node** p{ &nd };
			while (true) {
				node*& current{ *p };
				if (current->left == nullptr) {
					return current;
				}
				p = &current->left;
			}
		}

		node*& _max(node*& nd) {
			PLASTIC_VERIFY(nd != nullptr);
			node** p{ &nd };
			while (true) {
				node*& current{ *p };
				if (current->right == nullptr) {
					return current;
				}
				p = &current->right;
			}
		}

		void _leftRotate(node* nd) {
			node* left{ nd->left };
			node* parent{ nd->parent };
			node* grandparent{ parent->parent };
			nd->left = parent;
			nd->parent = grandparent;
			parent->right = left;
			parent->parent = nd;
			if (left != nullptr) {
				left->parent = parent;
			}
			if (grandparent != nullptr) {
				(parent == grandparent->left ? grandparent->left : grandparent->right) = nd;
			}
			else {
				_root = nd;
			}
		}

		void _rightRotate(node* nd) {
			node* right{ nd->right };
			node* parent{ nd->parent };
			node* grandparent{ parent->parent };
			nd->right = parent;
			nd->parent = grandparent;
			parent->left = right;
			parent->parent = nd;
			if (right != nullptr) {
				right->parent = parent;
			}
			if (grandparent != nullptr) {
				(parent == grandparent->left ? grandparent->left : grandparent->right) = nd;
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
			_free(_root);
		}

		bool empty() const {
			return _size == 0;
		}

		size_t size() const {
			return _size;
		}

		size_t count(const T& value) const {
			node* nd{ _find(value) };
			return nd == nullptr ? 0 : nd->count;
		}

		bool contains(const T& value) const {
			return _find(value) != nullptr;
		}

		T min() const {
			return _min(_root)->value;
		}

		T max() const {
			return _max(_root)->value;
		}

		void insert(const T& value, size_t count = 1) {
			_size += count;
			if (_root == nullptr) {
				_root = new node{ value, count, node::color::black, nullptr, nullptr, nullptr };
				return;
			}
			auto [nd, pr] {_findWithParent(value)};
			if (nd != nullptr) {
				nd->count += count;
				return;
			}
			nd = new node{ value, count, node::color::red, pr, nullptr, nullptr };
			node* current{ nd };
			while (true) {
				if (current == _root) {
					current->color = node::color::black;
					break;
				}
				node* parent{ current->parent };
				if (parent->color == node::color::black) {
					break;
				}
				node* grandparent{ parent->parent };
				bool isXL{ current == parent->left };
				bool isLX{ parent == grandparent->left };
				node* uncle{ isLX ? grandparent->right : grandparent->left };
				if (uncle == nullptr || uncle->color == node::color::black) {
					if (isLX && isXL) {
						_rightRotate(parent);
						parent->color = node::color::black;
					}
					else if (isLX) {
						_leftRotate(current);
						_rightRotate(current);
						current->color = node::color::black;
					}
					else if (isXL) {
						_rightRotate(current);
						_leftRotate(current);
						current->color = node::color::black;
					}
					else {
						_leftRotate(parent);
						parent->color = node::color::black;
					}
					grandparent->color = node::color::red;
					break;
				}
				parent->color = node::color::black;
				uncle->color = node::color::black;
				grandparent->color = node::color::red;
				current = grandparent;
			}
		}

		void erase(const T& value, size_t count = 1) {
			node*& nd{ _find(value) };
			if (nd == nullptr) {
				return;
			}
			if (nd->count > count) {
				_size -= count;
				nd->count -= count;
				return;
			}
			_size -= nd->count;
			node** pErased{ &nd };
			if (nd->left != nullptr && nd->right != nullptr) {
				node*& successor{ _min(nd->right) };
				nd->value = successor->value;
				nd->count = successor->count;
				pErased = &successor;
			}
			node*& erased{ *pErased };
			if (erased->left != nullptr) {
				node* temp{ erased };
				erased = erased->left;
				erased->color = node::color::black;
				erased->parent = temp->parent;
				delete temp;
				return;
			}
			if (erased->right != nullptr) {
				node* temp{ erased };
				erased = erased->right;
				erased->color = node::color::black;
				erased->parent = temp->parent;
				delete temp;
				return;
			}
			node* current{ erased };
			while (true) {
				if (current == _root || current->color == node::color::red) {
					break;
				}
				node* parent{ current->parent };
				bool isLX{ current == parent->right };
				bool isRX{ !isLX };
				node* sibling{ isLX ? parent->left : parent->right };
				if (sibling->color == node::color::red) {
					sibling->color = node::color::black;
					parent->color = node::color::red;
					if (isLX) {
						_rightRotate(sibling);
						sibling = parent->left;
					}
					else {
						_leftRotate(sibling);
						sibling = parent->right;
					}
				}
				node* left{ sibling->left };
				node* right{ sibling->right };
				bool isXL{ left != nullptr && left->color == node::color::red };
				bool isXR{ right != nullptr && right->color == node::color::red };
				if (isXL || isXR) {
					if (isLX && isXL) {
						_rightRotate(sibling);
						sibling->color = parent->color;
						left->color = node::color::black;
					}
					else if (isRX && isXR) {
						_leftRotate(sibling);
						sibling->color = parent->color;
						right->color = node::color::black;
					}
					else if (isLX) {
						_leftRotate(right);
						_rightRotate(right);
						right->color = parent->color;
					}
					else {
						_rightRotate(left);
						_leftRotate(left);
						left->color = parent->color;
					}
					parent->color = node::color::black;
					break;
				}
				bool isRed{ parent->color == node::color::red };
				parent->color = node::color::black;
				sibling->color = node::color::red;
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