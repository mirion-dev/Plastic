#pragma once

#include <functional>
#include <random>

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

	template<::std::forward_iterator iter1, ::std::forward_iterator iter2>
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

	template<::std::input_iterator iter, ::std::output_iterator<::std::iter_value_t<iter>> d_iter, class unary_function>
	d_iter transform(iter first, iter last, d_iter d_first, unary_function func) {
		while (first != last) {
			*d_first = func(*first);
			++d_first, ++first;
		}
		return d_first;
	}

	template<::std::input_iterator iter1, ::std::input_iterator iter2, ::std::output_iterator<::std::iter_value_t<iter1>> d_iter, class binary_function>
	d_iter transform(iter1 first1, iter1 last1, iter2 first2, d_iter d_first, binary_function func) {
		while (first1 != last1) {
			*d_first = func(*first1, *first2);
			++d_first, ++first1, ++first2;
		}
		return d_first;
	}

	template<::std::forward_iterator iter, class T = ::std::iter_value_t<iter>>
	void replace(iter first, iter last, const T& old_value, const T& value) {
		while (first != last) {
			if (*first == old_value) {
				*first = value;
			}
			++first;
		}
	}

	template<::std::forward_iterator iter, class unary_predicate, class T = ::std::iter_value_t<iter>>
	void replace_if(iter first, iter last, unary_predicate pred, const T& value) {
		while (first != last) {
			if (pred(*first)) {
				*first = value;
			}
			++first;
		}
	}

	template<::std::input_iterator iter, class d_iter, class T = ::std::iter_value_t<iter>> requires ::std::output_iterator<d_iter, T>
	d_iter replace_copy(iter first, iter last, d_iter d_first, const T& old_value, const T& value) {
		while (first != last) {
			*d_first = *first == old_value ? value : *first;
			++first, ++d_first;
		}
		return d_first;
	}

	template<::std::input_iterator iter, class d_iter, class unary_predicate, class T = ::std::iter_value_t<iter>> requires ::std::output_iterator<d_iter, T>
	d_iter replace_copy_if(iter first, iter last, d_iter d_first, unary_predicate pred, const T& value) {
		while (first != last) {
			*d_first = pred(*first) ? value : *first;
			++first, ++d_first;
		}
		return d_first;
	}

	template<::std::forward_iterator iter, class T = ::std::iter_value_t<iter>>
	void fill(iter first, iter last, const T& value) {
		while (first != last) {
			*first++ = value;
		}
	}

	template<class iter, ::std::integral size_t, class T> requires ::std::output_iterator<iter, T>
	iter fill_n(iter first, size_t count, const T& value) {
		while (count-- != 0) {
			*first++ = value;
		}
		return first;
	}

	template<::std::forward_iterator iter, class nullary_function>
	void generate(iter first, iter last, nullary_function func) {
		while (first != last) {
			*first++ = func();
		}
	}

	template<class iter, ::std::integral size_t, class nullary_function> requires ::std::output_iterator<iter, ::std::invoke_result<nullary_function>>
	iter generate_n(iter first, size_t count, nullary_function func) {
		while (count-- != 0) {
			*first++ = func();
		}
		return first;
	}

	template<::std::forward_iterator iter, class T = ::std::iter_value_t<iter>>
	iter remove(iter first, iter last, const T& value) {
		first = ::plastic::find(first, last, value);
		if (first != last) {
			iter i{first};
			while (++i != last) {
				if (*i != value) {
					*first++ = ::std::move(*i);
				}
			}
		}
		return first;
	}

	template<::std::forward_iterator iter, class unary_predicate>
	iter remove_if(iter first, iter last, unary_predicate pred) {
		first = ::plastic::find_if(first, last, pred);
		if (first != last) {
			iter i{first};
			while (++i != last) {
				if (!pred(*i)) {
					*first++ = ::std::move(*i);
				}
			}
		}
		return first;
	}

	template<::std::input_iterator iter, class d_iter, class T = ::std::iter_value_t<iter>> requires ::std::output_iterator<d_iter, T>
	d_iter remove_copy(iter first, iter last, d_iter d_first, const T& value) {
		while (first != last) {
			if (*first != value) {
				*d_first++ = *first;
			}
			++first;
		}
		return d_first;
	}

	template<::std::input_iterator iter, ::std::output_iterator<::std::iter_value_t<iter>> d_iter, class unary_predicate>
	d_iter remove_copy_if(iter first, iter last, d_iter d_first, unary_predicate pred) {
		while (first != last) {
			if (!pred(*first)) {
				*d_first++ = *first;
			}
			++first;
		}
		return d_first;
	}

	template<::std::forward_iterator iter, class binary_predicate = ::std::equal_to<>>
	iter unique(iter first, iter last, binary_predicate pred = {}) {
		if (first != last) {
			iter i{first};
			while (++i != last) {
				if (!pred(*first, *i)) {
					*++first = ::std::move(*i);
				}
			}
			++first;
		}
		return first;
	}

	template<::std::input_iterator iter, ::std::output_iterator<::std::iter_value_t<iter>> d_iter, class binary_predicate = ::std::equal_to<>>
	d_iter unique_copy(iter first, iter last, d_iter d_first, binary_predicate pred = {}) {
		if (first != last) {
			::std::iter_value_t<iter> value;
			*d_first++ = value = *first;
			while (++first != last) {
				if (!pred(value, *first)) {
					*d_first++ = value = *first;
				}
			}
		}
		return d_first;
	}

	template<::std::bidirectional_iterator iter>
	void reverse(iter first, iter last) {
		while (first != last && first != --last) {
			::std::swap(*first++, *last);
		}
	}

	template<::std::bidirectional_iterator iter, ::std::output_iterator<::std::iter_value_t<iter>> d_iter>
	d_iter reverse_copy(iter first, iter last, d_iter d_first) {
		while (first != last) {
			*d_first++ = *--last;
		}
		return d_first;
	}

	template<::std::forward_iterator iter>
	iter rotate(iter first, iter middle, iter last) {
		if (first == middle) {
			return last;
		}
		if (middle == last) {
			return first;
		}
		iter i{middle};
		do {
			::std::swap(*first++, *i++);
			if (first == middle) {
				middle = i;
			}
		} while (i != last);
		iter res{first};
		while (middle != last) {
			iter i{middle};
			do {
				::std::swap(*first++, *i++);
				if (first == middle) {
					middle = i;
				}
			} while (i != last);
		}
		return res;
	}

	template<::std::forward_iterator iter, ::std::output_iterator<::std::iter_value_t<iter>> d_iter>
	d_iter rotate_copy(iter first, iter middle, iter last, d_iter d_first) {
		return ::plastic::copy(first, middle, ::plastic::copy(middle, last, d_first));
	}

	template<::std::forward_iterator iter>
	iter shift_left(iter first, iter last, ::std::iter_difference_t<iter> count) {
		iter source{first};
		while (count-- != 0) {
			if (source == last) {
				return first;
			}
			++source;
		}
		return ::plastic::move(source, last, first);
	}

	template<::std::forward_iterator iter>
	iter shift_right(iter first, iter last, ::std::iter_difference_t<iter> count) {
		iter dest{first};
		while (count-- != 0) {
			if (dest == last) {
				return last;
			}
			++dest;
		}
		iter i{first}, j{dest};
		while (i != dest) {
			if (j == last) {
				::plastic::move(first, i, dest);
				return dest;
			}
			++i, ++j;
		}
		iter buffer{first};
		while (j != last) {
			::std::swap(*buffer++, *i);
			++i, ++j;
			if (buffer == dest) {
				buffer = first;
			}
		}
		::plastic::move(first, buffer, ::plastic::move(buffer, dest, i));
		return dest;
	}

	template<::std::random_access_iterator iter, class uniform_random_bit_generator>
	void shuffle(iter first, iter last, uniform_random_bit_generator&& gene) {
		using diff_t = ::std::iter_difference_t<iter>;
		using distr_t = ::std::uniform_int_distribution<diff_t>;
		using param_t = distr_t::param_type;
		distr_t dist;
		diff_t size{last - first};
		while (size-- != 0) {
			::std::swap(first[size], first[dist(gene, param_t{0, size})]);
		}
	}

	template<::std::input_iterator iter, ::std::output_iterator<::std::iter_value_t<iter>> d_iter, ::std::integral diff_t, class uniform_random_bit_generator> requires ::std::forward_iterator<iter> || ::std::random_access_iterator<d_iter>
	d_iter sample(iter first, iter last, d_iter d_first, diff_t count, uniform_random_bit_generator && gene) {
		using distr_t = ::std::uniform_int_distribution<diff_t>;
		using param_t = distr_t::param_type;
		distr_t dist;
		if constexpr (::std::forward_iterator<iter>) {
			diff_t size{static_cast<diff_t>(::std::distance(first, last))};
			if (count >= size) {
				return ::plastic::copy(first, last, d_first);
			}
			while (count != 0) {
				if (dist(gene, param_t{0, --size}) < count) {
					*d_first++ = *first;
					--count;
				}
				++first;
			}
			return d_first;
		}
		else {
			diff_t size{0};
			while (first != last && size != count) {
				d_first[size++] = *first;
				++first;
			}
			iter res{d_first + size};
			while (first != last) {
				diff_t r{dist(gene, param_t{0, size++})};
				if (r < count) {
					d_first[r] = *first;
				}
				++first;
			}
			return res;
		}
	}

	template<::std::input_iterator iter, class unary_predicate>
	bool is_partitioned(iter first, iter last, unary_predicate pred) {
		while (first != last && pred(*first)) {
			++first;
		}
		while (first != last && !pred(*first)) {
			++first;
		}
		return first == last;
	}

	template<::std::forward_iterator iter, class unary_predicate>
	iter partition(iter first, iter last, unary_predicate pred) {
		first = ::plastic::find_if_not(first, last, pred);
		if (first != last) {
			iter i{first};
			while (++i != last) {
				if (pred(*i)) {
					::std::swap(*i, *first);
					++first;
				}
			}
		}
		return first;
	}

	template<::std::input_iterator iter, ::std::output_iterator<::std::iter_value_t<iter>> d_iter1, ::std::output_iterator<::std::iter_value_t<iter>> d_iter2, class unary_predicate>
	::std::pair<d_iter1, d_iter2> partition_copy(iter first, iter last, d_iter1 d_first1, d_iter2 d_first2, unary_predicate pred) {
		while (first != last) {
			(pred(*first) ? *d_first1++ : *d_first2++) = *first;
			++first;
		}
		return {d_first1, d_first2};
	}

	template<::std::bidirectional_iterator iter, class unary_predicate>
	iter stable_partition(iter first, iter last, unary_predicate pred) {
		first = ::plastic::find_if_not(first, last, pred);
		while (first != last && !pred(*--last));
		if (first != last) {
			using value_t = ::std::iter_value_t<iter>;
			value_t* buffer{new value_t[::std::distance(first, last) + 1]};
			value_t* p{buffer};
			iter i{first};
			*p++ = ::std::move(*i++);
			while (i != last) {
				(pred(*i++) ? *first++ : *p++) = ::std::move(*i);
			}
			*first++ = ::std::move(*last);
			::plastic::move(buffer, p, first);
			delete[] buffer;
		}
		return first;
	}

	template<::std::forward_iterator iter, class unary_predicate>
	iter partition_point(iter first, iter last, unary_predicate pred) {
		using diff_t = ::std::iter_difference_t<iter>;
		diff_t size{::std::distance(first, last)};
		while (size != 0) {
			diff_t half{size / 2};
			iter next{::std::next(first, half)};
			if (pred(*next)) {
				first = ++next;
				size -= half + 1;
			}
			else {
				size = half;
			}
		}
		return first;
	}

	template<::std::forward_iterator iter, class T = ::std::iter_value_t<iter>, class compare = ::std::less<>>
	iter lower_bound(iter first, iter last, const T& value, compare comp = {}) {
		return ::plastic::partition_point(first, last, [&](auto&& param) { return comp(param, value); });
	}

	template<::std::forward_iterator iter, class T = ::std::iter_value_t<iter>, class compare = ::std::less<>>
	iter upper_bound(iter first, iter last, const T& value, compare comp = {}) {
		return ::plastic::partition_point(first, last, [&](auto&& param) { return !comp(value, param); });
	}

	template<::std::forward_iterator iter, class T = ::std::iter_value_t<iter>, class compare = ::std::less<>>
	::std::pair<iter, iter> equal_range(iter first, iter last, const T& value, compare comp = {}) {
		return {::plastic::lower_bound(first, last, value, comp), ::plastic::upper_bound(first, last, value, comp)};
	}

	template<::std::forward_iterator iter, class T = ::std::iter_value_t<iter>, class compare = ::std::less<>>
	bool binary_search(iter first, iter last, const T& value, compare comp = {}) {
		first = ::plastic::lower_bound(first, last, value, comp);
		return first != last && !comp(value, *first);
	}

}