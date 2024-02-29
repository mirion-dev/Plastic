#pragma once

#include <iterator>

namespace plastic {

	template<::std::bidirectional_iterator iter, ::std::strict_weak_order<::std::iter_value_t<iter>, ::std::iter_value_t<iter>> compare = ::std::less<>>
	void insertion_sort(iter first, iter last, compare comp = {}) {
		for (iter i{first}; i != last; ++i) {
			iter insertionPoint{first};
			for (::std::ptrdiff_t count{::std::distance(first, i)}; count > 0; ) {
				::std::ptrdiff_t step{count >> 1};
				iter middle{::std::next(insertionPoint, step)};
				if (comp(*i, *middle)) {
					count = step;
				}
				else {
					insertionPoint = ::std::move(++middle);
					count -= step + 1;
				}
			}
			::std::iter_value_t<iter> temp{::std::move(*i)};
			for (iter j{i}; j != insertionPoint; --j) {
				*j = ::std::move(*::std::prev(j));
			}
			*insertionPoint = ::std::move(temp);
		}
	}

	template<::std::bidirectional_iterator iter, ::std::strict_weak_order<::std::iter_value_t<iter>, ::std::iter_value_t<iter>> compare = ::std::less<>>
	void selection_sort(iter first, iter last, compare comp = {}) {
		for (; first != last && first != --last; ++first) {
			iter min{first}, max{first};
			for (iter i{first}; i != ::std::next(last); ++i) {
				if (comp(*max, *i)) {
					max = i;
				}
				if (comp(*i, *min)) {
					min = i;
				}
			}
			if (min == max) {
				break;
			}
			::std::swap(*min, *first);
			::std::swap(max == first ? *min : *max, *last);
		}
	}

	template<::std::bidirectional_iterator iter, ::std::strict_weak_order<::std::iter_value_t<iter>, ::std::iter_value_t<iter>> compare = ::std::less<>>
	void bubble_sort(iter first, iter last, compare comp = {}) {
		for (; first != last && first != --last; ++first) {
			bool isSorted{true};
			iter i{first}, j{::std::next(i)};
			for (; i != last; ++i, ++j) {
				if (comp(*j, *i)) {
					::std::swap(*i, *j);
					isSorted = false;
				}
			}
			if (isSorted) {
				break;
			}
			do {
				--i, --j;
				if (comp(*j, *i)) {
					::std::swap(*i, *j);
					isSorted = false;
				}
			} while (i != first);
			if (isSorted) {
				break;
			}
		}
	}

	template<::std::random_access_iterator iter, ::std::strict_weak_order<::std::iter_value_t<iter>, ::std::iter_value_t<iter>> compare = ::std::less<>>
	void heapsort(iter first, iter last, compare comp = {}) {
		auto siftDown{[&](::std::ptrdiff_t node, ::std::ptrdiff_t bottom) {
			while (node < bottom >> 1) {
				::std::ptrdiff_t child{(node << 1) + 1};
				if (child + 1 < bottom && comp(first[child], first[child + 1])) {
					++child;
				}
				if (comp(first[node], first[child])) {
					::std::swap(first[node], first[child]);
					node = child;
				}
				else {
					break;
				}
			}
		}};
		::std::ptrdiff_t size{last - first};
		for (::std::ptrdiff_t i{(size >> 1) - 1}; i >= 0; --i) {
			siftDown(i, size);
		}
		for (::std::ptrdiff_t i{size - 1}; i > 0; --i) {
			::std::swap(first[0], first[i]);
			siftDown(0, i);
		}
	}

	template<::std::bidirectional_iterator iter, ::std::strict_weak_order<::std::iter_value_t<iter>, ::std::iter_value_t<iter>> compare = ::std::less<>>
	void merge_sort(iter first, iter last, compare comp = {}) {
		::std::ptrdiff_t size{::std::distance(first, last)};
		if (size <= 16) {
			::plastic::bubble_sort(first, last, comp);
			return;
		}
		iter middle{::std::next(first, size >> 1)};
		::plastic::merge_sort(first, middle, comp);
		::plastic::merge_sort(middle, last, comp);
		if (comp(*::std::prev(middle), *middle)) {
			return;
		}
		auto temp{new ::std::iter_value_t<iter>[size]};
		::std::iter_value_t<iter>* i{temp};
		iter left{first}, right{middle};
		while (left != middle && right != last) {
			if (comp(*left, *right)) {
				*i = ::std::move(*left);
				++i;
				++left;
			}
			else {
				*i = ::std::move(*right);
				++i;
				++right;
			}
		}
		if (left == middle) {
			::std::move(right, last, i);
		}
		else {
			::std::move(left, middle, i);
		}
		::std::move(temp, temp + size, first);
		delete[] temp;
	}

	template<::std::bidirectional_iterator iter, ::std::strict_weak_order<::std::iter_value_t<iter>, ::std::iter_value_t<iter>> compare = ::std::less<>>
	void quicksort(iter first, iter last, compare comp = {}) {
		::std::ptrdiff_t size{::std::distance(first, last)};
		if (size <= 16) {
			::plastic::bubble_sort(first, last, comp);
			return;
		}
		iter left{first}, middle{::std::next(first, size >> 1)}, right{::std::prev(last)};
		if (comp(*middle, *left)) {
			::std::swap(*middle, *left);
		}
		if (comp(*left, *right)) {
			::std::swap(*left, *right);
		}
		if (comp(*middle, *left)) {
			::std::swap(*middle, *left);
		}
		bool status{true};
		while (left != right) {
			if (status) {
				if (comp(*right, *left)) {
					::std::swap(*left, *right);
					status = false;
				}
				else {
					--right;
				}
			}
			else {
				if (comp(*right, *left)) {
					::std::swap(*left, *right);
					status = true;
				}
				else {
					++left;
				}
			}
		}
		::plastic::quicksort(first, left, comp);
		::plastic::quicksort(::std::next(left), last, comp);
	}

}