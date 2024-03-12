#pragma once

#include <functional>

namespace plastic {

	template<class T, ::std::size_t order, ::std::strict_weak_order<T, T> auto _compare = ::std::less<>{} >
	class b_tree {
		constexpr ::std::size_t _minSize{order - 1};
		constexpr ::std::size_t _maxSize{2 * order - 1};

		struct node {
			T _values[_maxSize];
			node* _children[_maxSize + 1];
			::std::size_t _size;
		};

		node* _root;
		::std::size_t _size;

	public:
		b_tree() {
			_root = nullptr;
			_size = 0;
		}
	};

}