#pragma once

#include <functional>

namespace plastic {

	template<class T, size_t order, std::strict_weak_order<T, T> auto _compare = std::less<>{} >
	class b_tree {
		constexpr size_t _minSize{ order - 1 };
		constexpr size_t _maxSize{ 2 * order - 1 };

		struct node {
			T _values[_maxSize];
			node* _children[_maxSize + 1];
			size_t _size;
		};

		node* _root;
		size_t _size;

	public:
		b_tree() {
			_root = nullptr;
			_size = 0;
		}
	};

}