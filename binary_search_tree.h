#pragma once

#include "utils.h"

#include <functional>

namespace plastic {

	template<class T, class Cmp = std::less<T>>
	class binary_search_tree {
		static constexpr Cmp _cmp{};

		struct _node {
			_node* parent;
			_node* left;
			_node* right;
			T value;
			bool nil;
		};

		_node* _head;
		size_t _size;

		void _free(_node* node) {
			if (node->nil) {
				return;
			}
			_free(node->left);
			_free(node->right);
			delete node;
		}

		std::pair<_node*, bool> _upperBound(const T& value) {
			_node* current{ _head->parent }, * parent{ current };
			bool isLeft{};
			while (!current->nil) {
				isLeft = _cmp(value, current->value);
				parent = current;
				current = isLeft ? current->left : current->right;
			}
			return { parent, isLeft };
		}

		std::pair<_node*, bool> _lowerBound(const T& value) {
			_node* parent{ _head->parent }, * current{ parent };
			bool isLeft{};
			while (!current->nil) {
				isLeft = !_cmp(current->value, value);
				parent = current;
				current = isLeft ? current->left : current->right;
			}
			return { parent, isLeft };
		}

	public:
		class iterator {
			friend class binary_search_tree;

			_node* _ptr;
			const binary_search_tree* _cont;

		public:
			using difference_type = ptrdiff_t;
			using value_type = T;
			using pointer = const T*;
			using reference = const T&;
			using iterator_category = std::bidirectional_iterator_tag;

			iterator(_node* node = {}, const binary_search_tree* cont = {}) {
				_ptr = node;
				_cont = cont;
			}

			reference operator*() const {
				return _ptr->value;
			}

			pointer operator->() const {
				return &_ptr->value;
			}

			bool operator==(iterator it) const {
				return _ptr == it._ptr;
			}

			iterator& operator++() {
				if (_ptr->right->nil) {
					_node* temp;
					do {
						temp = _ptr;
						_ptr = _ptr->parent;
					} while (!_ptr->nil && _ptr->right == temp);
				}
				else {
					_ptr = _ptr->right;
					while (!_ptr->left->nil) {
						_ptr = _ptr->left;
					}
				}
				return *this;
			}

			iterator operator++(int) {
				iterator temp{ *this };
				++*this;
				return temp;
			}

			iterator& operator--() {
				if (_ptr->left->nil) {
					_node* temp;
					do {
						temp = _ptr;
						_ptr = _ptr->parent;
					} while (!_ptr->nil && _ptr->left == temp);
				}
				else {
					_ptr = _ptr->left;
					while (!_ptr->right->nil) {
						_ptr = _ptr->right;
					}
				}
				return *this;
			}

			iterator operator--(int) {
				iterator temp{ *this };
				--*this;
				return temp;
			}
		};

		using const_iterator = iterator;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = reverse_iterator;

		explicit binary_search_tree() {
			_head->parent = _head->left = _head->right = _head = new _node;
			_head->nil = true;
			_size = 0;
		}

		~binary_search_tree() {
			clear();
			delete _head;
		}

		bool empty() const {
			return _size == 0;
		}

		size_t size() const {
			return _size;
		}

		void clear() {
			_free(_head->parent);
		}

		const_iterator begin() const {
			return { _head->right, this };
		}

		const_iterator end() const {
			return { _head, this };
		}

		const_reverse_iterator rbegin() const {
			return { _head, this };
		}

		const_reverse_iterator rend() const {
			return { _head->right, this };
		}

		const T& front() const {
			return _head->right->value;
		}

		const T& back() const {
			return _head->left->value;
		}

		const_iterator lower_bound(const T& value) const {
			return { _lowerBound(value).first, this };
		}

		const_iterator upper_bound(const T& value) const {
			return { _upperBound(value).first, this };
		}

		std::pair<const_iterator, const_iterator> equal_range(const T& value) const {
			return { lower_bound(value), upper_bound(value) };
		}

		const_iterator find(const T& value) const {
			return lower_bound(value);
		}

		bool contains(const T& value) const {
			return find(value) != end();
		}

		size_t count(const T& value) const {
			return std::distance(lower_bound(value), upper_bound(value));
		}

		void insert(const T& value) {
		}

		void erase(const T& value) {
		}
	};

}