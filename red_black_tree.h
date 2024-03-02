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
			node* _left;
			node* _right;
		};

		node* _root;
		::std::size_t _size;

	public:
		explicit red_black_tree() {
			_root = nullptr;
			_size = 0;
		}

		~red_black_tree() {
			// todo
		}

		bool empty() {
			return _size == 0;
		}

		::std::size_t size() {
			return _size;
		}

		bool find(const T& value) {

		}

		::std::size_t count(const T& value) {

		}

		void insert(const T& value, ::std::size_t count = 1) {

		}

		void erase(const T& value, ::std::size_t count = 1) {

		}
	};

}