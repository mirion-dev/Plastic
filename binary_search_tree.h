#pragma once

#include "utils.h"

#include <functional>

namespace plastic {

	template<class T, class Cmp = std::less<T>>
	class binary_search_tree {
		struct node {
			T value;
			size_t count;
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

		node*& _find(const T& value) {
			node** p{ &_root };
			while (true) {
				node*& current{ *p };
				if (current == nullptr || current->value == value) {
					return current;
				}
				p = _cmp(value, current->value) ? &current->left : &current->right;
			}
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

	public:
		explicit binary_search_tree() {
			_root = nullptr;
			_size = 0;
		}

		~binary_search_tree() {
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
			node*& nd{ _find(value) };
			if (nd == nullptr) {
				nd = new node{ value, count, nullptr, nullptr };
			}
			else {
				nd->count += count;
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
			if (nd->left == nullptr) {
				node* temp{ nd };
				nd = nd->right;
				delete temp;
			}
			else if (nd->right == nullptr) {
				node* temp{ nd };
				nd = nd->left;
				delete temp;
			}
			else {
				node*& successor{ _min(nd->right) };
				nd->value = successor->value;
				nd->count = successor->count;
				node* temp{ successor };
				successor = successor->right;
				delete temp;
			}
		}
	};

}