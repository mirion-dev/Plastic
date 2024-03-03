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
			node** parent{i};
			while (true) {
				node*& temp{*i};
				if (temp == nullptr || value == temp->_value) {
					return ::std::make_pair(temp, *parent);
				}
				parent = i;
				if (_compare(value, temp->_value)) {
					i = &temp->_left;
				}
				else {
					i = &temp->_right;
				}
			}
		}

		node*& _findNode(const T& node) {
			return _findNodeWithParent(node).first;
		}

		node*& _minChild(node*& nd) {
			node** i{&nd};
			while (true) {
				node*& temp{(*i)->_left};
				if (temp == nullptr) {
					return *i;
				}
				i = &temp;
			}
		}

		node*& _maxChild(node*& nd) {
			node** i{&nd};
			while (true) {
				node*& temp{(*i)->_right};
				if (temp == nullptr) {
					return *i;
				}
				i = &temp;
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
			node*& nd{_findNode(value)};
			return nd == nullptr ? 0 : nd->_count;
		}

		void insert(const T& value, ::std::size_t count = 1) {

		}

		void erase(const T& value, ::std::size_t count = 1) {

		}

		T min() {
			if (_root == nullptr) {
				return {};
			}
			return _minChild(_root)->_value;
		}

		T max() {
			if (_root == nullptr) {
				return {};
			}
			return _maxChild(_root)->_value;
		}
	};

}