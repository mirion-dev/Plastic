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
		::std::size_t size;

	public:
		explicit red_black_tree() {

		}

		~red_black_tree() {
			// todo
		}
	};

}