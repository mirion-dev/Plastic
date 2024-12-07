#pragma once

#include "macro.h"

#include <functional>

namespace plastic {

	template<class T, class Cmp = std::less<T>>
	class avl_tree {
		struct node {
			T value;
			size_t count;
			int factor;
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

		node*& _find(const T& value) {
			return _findWithParent(value).first;
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
		explicit avl_tree() {
			_root = nullptr;
			_size = 0;
		}

		~avl_tree() {
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
				_root = new node{ value, count, 0, nullptr, nullptr, nullptr };
				return;
			}
			auto [nd, pr] {_findWithParent(value)};
			if (nd != nullptr) {
				nd->count += count;
				return;
			}
			nd = new node{ value, count, 0, pr, nullptr, nullptr };
			node* current{ nd };
			while (true) {
				if (current == _root) {
					break;
				}
				node* parent{ current->parent };
				if (current == parent->left) {
					++parent->factor;
				}
				else {
					--parent->factor;
				}
				if (parent->factor == 0) {
					break;
				}
				bool isLX{ parent->factor == 2 };
				bool isRX{ parent->factor == -2 };
				bool isXL{ current->factor == 1 };
				if (isLX || isRX) {
					if (isLX && isXL) {
						_rightRotate(current);
						current->factor = 0;
						parent->factor = 0;
					}
					else if (isLX) {
						node* right{ current->right };
						_leftRotate(right);
						_rightRotate(right);
						current->factor = right->factor == -1 ? 1 : 0;
						parent->factor = right->factor == 1 ? -1 : 0;
						right->factor = 0;

					}
					else if (isXL) {
						node* left{ current->left };
						_rightRotate(left);
						_leftRotate(left);
						current->factor = left->factor == -1 ? 1 : 0;
						parent->factor = left->factor == 1 ? -1 : 0;
						left->factor = 0;
					}
					else {
						_leftRotate(current);
						current->factor = 0;
						parent->factor = 0;
					}
					break;
				}
				current = parent;
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
			if (node*& erased{ *pErased }, *& left{ erased->left }, *& right{ erased->right };
				left != nullptr) {
				erased->value = left->value;
				erased->count = left->count;
				erased->factor = 0;
				pErased = &left;
			}
			else if (right != nullptr) {
				erased->value = right->value;
				erased->count = right->count;
				erased->factor = 0;
				pErased = &right;
			}
			node*& erased{ *pErased };
			node* current{ erased };
			while (true) {
				if (current == _root) {
					break;
				}
				node* parent{ current->parent };
				if (current == parent->left) {
					--parent->factor;
				}
				else {
					++parent->factor;
				}
				if (parent->factor == 1 || parent->factor == -1) {
					break;
				}
				bool isLX{ parent->factor == 2 };
				bool isRX{ parent->factor == -2 };
				if (isLX || isRX) {
					node* sibling{ isLX ? parent->left : parent->right };
					bool isXL{ isLX && sibling->factor != -1 || isRX && sibling->factor == 1 };
					if (isLX && isXL) {
						_rightRotate(sibling);
						if (sibling->factor == 0) {
							sibling->factor = -1;
							parent->factor = 1;
							break;
						}
						sibling->factor = 0;
						parent->factor = 0;
					}
					else if (isLX) {
						node* right{ sibling->right };
						_leftRotate(right);
						_rightRotate(right);
						sibling->factor = right->factor == -1 ? 1 : 0;
						parent->factor = right->factor == 1 ? -1 : 0;
						right->factor = 0;
					}
					else if (isXL) {
						node* left{ sibling->left };
						_rightRotate(left);
						_leftRotate(left);
						sibling->factor = left->factor == -1 ? 1 : 0;
						parent->factor = left->factor == 1 ? -1 : 0;
						left->factor = 0;
					}
					else {
						_leftRotate(sibling);
						if (sibling->factor == 0) {
							sibling->factor = 1;
							parent->factor = -1;
							break;
						}
						sibling->factor = 0;
						parent->factor = 0;
					}
				}
				current = parent;

			}
			node* temp{ erased };
			erased = nullptr;
			delete temp;
		}
	};

}