#pragma once

#include <functional>

namespace plastic {

	template<::std::input_iterator iter, class unary_function>
	unary_function for_each(iter first, iter last, unary_function func) {
		while (first != last) {
			func(*first);
			++first;
		}
		return func;
	}

	template<::std::input_iterator iter, ::std::integral size_t, class unary_function>
	iter for_each_n(iter first, size_t count, unary_function func) {
		while (count-- != 0) {
			func(*first);
			++first;
		}
		return first;
	}

	template<::std::input_iterator iter, class T = ::std::iter_value_t<iter>>
	iter find(iter first, iter last, const T& value) {
		while (first != last && *first != value) {
			++first;
		}
		return first;
	}

	template<::std::input_iterator iter, class unary_predicate>
	iter find_if(iter first, iter last, unary_predicate pred) {
		while (first != last && !pred(*first)) {
			++first;
		}
		return first;
	}

	template<::std::input_iterator iter, class unary_predicate>
	iter find_if_not(iter first, iter last, unary_predicate pred) {
		while (first != last && pred(*first)) {
			++first;
		}
		return first;
	}

	template<::std::input_iterator iter, class unary_predicate>
	bool all_of(iter first, iter last, unary_predicate pred) {
		return ::plastic::find_if_not(first, last, pred) == last;
	}

	template<::std::input_iterator iter, class unary_predicate>
	bool any_of(iter first, iter last, unary_predicate pred) {
		return ::plastic::find_if(first, last, pred) != last;
	}

	template<::std::input_iterator iter, class unary_predicate>
	bool none_of(iter first, iter last, unary_predicate pred) {
		return ::plastic::find_if(first, last, pred) == last;
	}

	template<::std::forward_iterator iter1, ::std::forward_iterator iter2, class binary_predicate = ::std::equal_to<>>
	iter1 search(iter1 first1, iter1 last1, iter2 first2, iter2 last2, binary_predicate pred = {}) {
		while (true) {
			iter1 i1{first1};
			iter2 i2{first2};
			do {
				if (i2 == last2) {
					return first1;
				}
				if (i1 == last1) {
					return last1;
				}
			} while (pred(*i1++, *i2++));
			++first1;
		}
	}

	template<::std::forward_iterator iter, class searcher>
	iter search(iter first, iter last, const searcher& srch) {
		return srch(first, last).first;
	}

	template<::std::forward_iterator iter, ::std::integral size_t, class T = ::std::iter_value_t<iter>, class binary_predicate = ::std::equal_to<>>
	iter search_n(iter first, iter last, size_t count, const T& value, binary_predicate pred = {}) {
		while (first != last) {
			if (pred(*first, value)) {
				iter next{first};
				size_t n{count};
				do {
					if (--n == 0) {
						return next;
					}
					if (++first == last) {
						return last;
					}
				} while (pred(*first, value));
				first = next;
			}
			++first;
		}
		return last;
	}

	template<::std::forward_iterator iter1, ::std::forward_iterator iter2, class binary_predicate = ::std::equal_to<>>
	iter1 find_end(iter1 first1, iter1 last1, iter2 first2, iter2 last2, binary_predicate pred = {}) {
		iter1 res{last1}, next;
		while (next = ::plastic::search(first1, last1, first2, last2, pred), next != last1) {
			first1 = res = next;
			++first1;
		}
		return res;
	}

	template<::std::input_iterator iter1, ::std::forward_iterator iter2, class binary_predicate = ::std::equal_to<>>
	iter1 find_first_of(iter1 first1, iter1 last1, iter2 first2, iter2 last2, binary_predicate pred = {}) {
		while (first1 != last1) {
			iter2 i{first2};
			while (i != last2) {
				if (pred(*first1, *i++)) {
					return first1;
				}
			}
			++first1;
		}
		return last1;
	}

	template<::std::forward_iterator iter, class binary_predicate = ::std::equal_to<>>
	iter adjacent_find(iter first, iter last, binary_predicate pred = {}) {
		if (first != last) {
			iter i{first};
			while (++i != last && !pred(*first, *i)) {
				++first;
			}
		}
		return first;
	}

	template<::std::input_iterator iter, class T = ::std::iter_value_t<iter>>
	std::iter_difference_t<iter> count(iter first, iter last, const T& value) {
		std::iter_difference_t<iter> res{0};
		while (first != last) {
			if (*first == value) {
				++res;
			}
			++first;
		}
		return res;
	}

	template<::std::input_iterator iter, class unary_predicate>
	std::iter_difference_t<iter> count_if(iter first, iter last, unary_predicate pred) {
		std::iter_difference_t<iter> res{0};
		while (first != last) {
			if (pred(*first)) {
				++res;
			}
			++first;
		}
		return res;
	}

	template<::std::input_iterator iter1, ::std::input_iterator iter2, class binary_predicate = ::std::equal_to<>>
	::std::pair<iter1, iter2> mismatch(iter1 first1, iter1 last1, iter2 first2, binary_predicate pred = {}) {
		while (first1 != last1 && pred(*first1, *first2)) {
			++first1, ++first2;
		}
		return {first1, first2};
	}

	template<::std::input_iterator iter1, ::std::input_iterator iter2, class binary_predicate = ::std::equal_to<>>
	::std::pair<iter1, iter2> mismatch(iter1 first1, iter1 last1, iter2 first2, iter2 last2, binary_predicate pred = {}) {
		while (first1 != last1 && first2 != last2 && pred(*first1, *first2)) {
			++first1, ++first2;
		}
		return {first1, first2};
	}

	template<::std::input_iterator iter1, ::std::input_iterator iter2, class binary_predicate = ::std::equal_to<>>
	bool equal(iter1 first1, iter1 last1, iter2 first2, binary_predicate pred = {}) {
		while (first1 != last1) {
			if (!pred(*first1, *first2)) {
				return false;
			}
			++first1, ++first2;
		}
		return true;
	}

	template<::std::input_iterator iter1, ::std::input_iterator iter2, class binary_predicate = ::std::equal_to<>>
	bool equal(iter1 first1, iter1 last1, iter2 first2, iter2 last2, binary_predicate pred = {}) {
		while (first1 != last1 && first2 != last2) {
			if (!pred(*first1, *first2)) {
				return false;
			}
			++first1, ++first2;
		}
		return first1 == last1 && first2 == last2;
	}

	template<::std::input_iterator iter, ::std::output_iterator<::std::iter_value_t<iter>> d_iter>
	d_iter copy(iter first, iter last, d_iter d_first) {
		while (first != last) {
			*d_first = *first;
			++d_first, ++first;
		}
		return d_first;
	}

	template<::std::input_iterator iter, ::std::output_iterator<::std::iter_value_t<iter>> d_iter, class unary_predicate>
	d_iter copy_if(iter first, iter last, d_iter d_first, unary_predicate pred) {
		while (first != last) {
			if (pred(*first)) {
				*d_first = *first;
			}
			++d_first, ++first;
		}
		return d_first;
	}

	template<::std::input_iterator iter, ::std::integral size_t, ::std::output_iterator<::std::iter_value_t<iter>> d_iter>
	d_iter copy_n(iter first, size_t count, d_iter d_first) {
		while (count-- != 0) {
			*d_first = *first;
			++d_first, ++first;
		}
		return d_first;
	}

	template<::std::bidirectional_iterator iter, ::std::bidirectional_iterator d_iter>
	d_iter copy_backward(iter first, iter last, d_iter d_last) {
		while (first != last) {
			*--d_last = *--last;
		}
		return d_last;
	}

	template<::std::input_iterator iter, ::std::output_iterator<::std::iter_value_t<iter>> d_iter>
	d_iter move(iter first, iter last, d_iter d_first) {
		while (first != last) {
			*d_first = ::std::move(*first);
			++d_first, ++first;
		}
		return d_first;
	}

	template<::std::bidirectional_iterator iter, ::std::bidirectional_iterator d_iter>
	d_iter move_backward(iter first, iter last, d_iter d_last) {
		while (first != last) {
			*--d_last = ::std::move(*--last);
		}
		return d_last;
	}

	template<::std::forward_iterator iter1,::std::forward_iterator iter2>
	iter2 swap_ranges(iter1 first1, iter1 last1, iter2 first2) {
		while (first1 != last1) {
			::std::swap(*first1++, *first2++);
		}
		return first2;
	}

	template<::std::forward_iterator iter1, ::std::forward_iterator iter2>
	void iter_swap(iter1 i1, iter2 i2) {
		::std::swap(*i1, *i2);
	}

}