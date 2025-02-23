export module plastic:algorithm;

import std;

// non-modifying sequence operations
export namespace plastic {

    template<class It, class Fn>
    Fn for_each(It first, It last, Fn func) {
        while (first != last) {
            func(*first);
            ++first;
        }
        return func;
    }

    template<class It, class size_t, class Fn>
    It for_each_n(It first, size_t count, Fn func) {
        while (count-- != 0) {
            func(*first);
            ++first;
        }
        return first;
    }

    template<class It, class T = std::iter_value_t<It>>
    std::iter_difference_t<It> count(It first, It last, const T& value) {
        using diff_t = std::iter_difference_t<It>;
        diff_t res{ 0 };
        while (first != last) {
            if (*first == value) {
                ++res;
            }
            ++first;
        }
        return res;
    }

    template<class It, class Pr>
    std::iter_difference_t<It> count_if(It first, It last, Pr pred) {
        using diff_t = std::iter_difference_t<It>;
        diff_t res{ 0 };
        while (first != last) {
            if (pred(*first)) {
                ++res;
            }
            ++first;
        }
        return res;
    }

    template<class It1, class It2, class Pr = std::equal_to<>>
    std::pair<It1, It2> mismatch(It1 first1, It1 last1, It2 first2, Pr pred = {}) {
        while (first1 != last1 && pred(*first1, *first2)) {
            ++first1, ++first2;
        }
        return { first1, first2 };
    }

    template<class It1, class It2, class Pr = std::equal_to<>>
    std::pair<It1, It2> mismatch(It1 first1, It1 last1, It2 first2, It2 last2, Pr pred = {}) {
        while (first1 != last1 && first2 != last2 && pred(*first1, *first2)) {
            ++first1, ++first2;
        }
        return { first1, first2 };
    }

    template<class It, class T = std::iter_value_t<It>>
    It find(It first, It last, const T& value) {
        while (first != last && *first != value) {
            ++first;
        }
        return first;
    }

    template<class It, class Pr>
    It find_if(It first, It last, Pr pred) {
        while (first != last && !pred(*first)) {
            ++first;
        }
        return first;
    }

    template<class It, class Pr>
    It find_if_not(It first, It last, Pr pred) {
        while (first != last && pred(*first)) {
            ++first;
        }
        return first;
    }

    template<class It, class Pr>
    bool all_of(It first, It last, Pr pred) {
        return find_if_not(first, last, pred) == last;
    }

    template<class It, class Pr>
    bool any_of(It first, It last, Pr pred) {
        return find_if(first, last, pred) != last;
    }

    template<class It, class Pr>
    bool none_of(It first, It last, Pr pred) {
        return find_if(first, last, pred) == last;
    }

    template<class It1, class It2, class Pr = std::equal_to<>>
    It1 search(It1 first1, It1 last1, It2 first2, It2 last2, Pr pred = {}) {
        while (true) {
            It1 i1{ first1 };
            It2 i2{ first2 };
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

    template<class It, class Searcher>
    It search(It first, It last, const Searcher& srch) {
        return srch(first, last).first;
    }

    template<class It, class size_t, class T = std::iter_value_t<It>, class Pr = std::equal_to<>>
    It search_n(It first, It last, size_t count, const T& value, Pr pred = {}) {
        while (first != last) {
            if (pred(*first, value)) {
                It next{ first };
                size_t n{ count };
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

    template<class It1, class It2, class Pr = std::equal_to<>>
    It1 find_end(It1 first1, It1 last1, It2 first2, It2 last2, Pr pred = {}) {
        It1 res{ last1 }, next;
        while (next = search(first1, last1, first2, last2, pred), next != last1) {
            first1 = res = next;
            ++first1;
        }
        return res;
    }

    template<class It1, class It2, class Pr = std::equal_to<>>
    It1 find_first_of(It1 first1, It1 last1, It2 first2, It2 last2, Pr pred = {}) {
        while (first1 != last1) {
            It2 i{ first2 };
            while (i != last2) {
                if (pred(*first1, *i++)) {
                    return first1;
                }
            }
            ++first1;
        }
        return last1;
    }

    template<class It, class Pr = std::equal_to<>>
    It adjacent_find(It first, It last, Pr pred = {}) {
        if (first != last) {
            It i{ first };
            while (++i != last && !pred(*first, *i)) {
                ++first;
            }
        }
        return first;
    }

}

// modifying sequence operations
export namespace plastic {

    template<class It, class ItD>
    ItD copy(It first, It last, ItD d_first) {
        while (first != last) {
            *d_first = *first;
            ++d_first, ++first;
        }
        return d_first;
    }

    template<class It, class ItD, class Pr>
    ItD copy_if(It first, It last, ItD d_first, Pr pred) {
        while (first != last) {
            if (pred(*first)) {
                *d_first = *first;
            }
            ++d_first, ++first;
        }
        return d_first;
    }

    template<class It, class size_t, class ItD>
    ItD copy_n(It first, size_t count, ItD d_first) {
        while (count-- != 0) {
            *d_first = *first;
            ++d_first, ++first;
        }
        return d_first;
    }

    template<class It, class ItD>
    ItD copy_backward(It first, It last, ItD d_last) {
        while (first != last) {
            *--d_last = *--last;
        }
        return d_last;
    }

    template<class It, class ItD>
    ItD move(It first, It last, ItD d_first) {
        while (first != last) {
            *d_first = std::move(*first);
            ++d_first, ++first;
        }
        return d_first;
    }

    template<class It, class ItD>
    ItD move_backward(It first, It last, ItD d_last) {
        while (first != last) {
            *--d_last = std::move(*--last);
        }
        return d_last;
    }

    template<class It, class T = std::iter_value_t<It>>
    void fill(It first, It last, const T& value) {
        while (first != last) {
            *first++ = value;
        }
    }

    template<class It, class size_t, class T>
    It fill_n(It first, size_t count, const T& value) {
        while (count-- != 0) {
            *first++ = value;
        }
        return first;
    }

    template<class It, class ItD, class Fn>
    ItD transform(It first, It last, ItD d_first, Fn func) {
        while (first != last) {
            *d_first = func(*first);
            ++d_first, ++first;
        }
        return d_first;
    }

    template<class It1, class It2, std::output_iterator<std::iter_value_t<It1>> ItD, class Fn>
    ItD transform(It1 first1, It1 last1, It2 first2, ItD d_first, Fn func) {
        while (first1 != last1) {
            *d_first = func(*first1, *first2);
            ++d_first, ++first1, ++first2;
        }
        return d_first;
    }

    template<class It, class Fn>
    void generate(It first, It last, Fn func) {
        while (first != last) {
            *first++ = func();
        }
    }

    template<class It, class size_t, class Fn>
    It generate_n(It first, size_t count, Fn func) {
        while (count-- != 0) {
            *first++ = func();
        }
        return first;
    }

    template<class It, class T = std::iter_value_t<It>>
    It remove(It first, It last, const T& value) {
        first = find(first, last, value);
        if (first != last) {
            It i{ first };
            while (++i != last) {
                if (*i != value) {
                    *first++ = std::move(*i);
                }
            }
        }
        return first;
    }

    template<class It, class Pr>
    It remove_if(It first, It last, Pr pred) {
        first = find_if(first, last, pred);
        if (first != last) {
            It i{ first };
            while (++i != last) {
                if (!pred(*i)) {
                    *first++ = std::move(*i);
                }
            }
        }
        return first;
    }

    template<class It, class ItD, class T = std::iter_value_t<It>>
    ItD remove_copy(It first, It last, ItD d_first, const T& value) {
        while (first != last) {
            if (*first != value) {
                *d_first++ = *first;
            }
            ++first;
        }
        return d_first;
    }

    template<class It, class ItD, class Pr>
    ItD remove_copy_if(It first, It last, ItD d_first, Pr pred) {
        while (first != last) {
            if (!pred(*first)) {
                *d_first++ = *first;
            }
            ++first;
        }
        return d_first;
    }

    template<class It, class T = std::iter_value_t<It>>
    void replace(It first, It last, const T& old_value, const T& value) {
        while (first != last) {
            if (*first == old_value) {
                *first = value;
            }
            ++first;
        }
    }

    template<class It, class Pr, class T = std::iter_value_t<It>>
    void replace_if(It first, It last, Pr pred, const T& value) {
        while (first != last) {
            if (pred(*first)) {
                *first = value;
            }
            ++first;
        }
    }

    template<class It, class ItD, class T = std::iter_value_t<It>>
    ItD replace_copy(It first, It last, ItD d_first, const T& old_value, const T& value) {
        while (first != last) {
            *d_first = *first == old_value ? value : *first;
            ++first, ++d_first;
        }
        return d_first;
    }

    template<class It, class ItD, class Pr, class T = std::iter_value_t<It>>
    ItD replace_copy_if(It first, It last, ItD d_first, Pr pred, const T& value) {
        while (first != last) {
            *d_first = pred(*first) ? value : *first;
            ++first, ++d_first;
        }
        return d_first;
    }

    // swap

    template<class It1, class It2>
    It2 swap_ranges(It1 first1, It1 last1, It2 first2) {
        while (first1 != last1) {
            std::swap(*first1++, *first2++);
        }
        return first2;
    }

    template<class It1, class It2>
    void iter_swap(It1 i1, It2 i2) {
        std::swap(*i1, *i2);
    }

    template<class It>
    void reverse(It first, It last) {
        while (first != last && first != --last) {
            std::swap(*first++, *last);
        }
    }

    template<class It, class ItD>
    ItD reverse_copy(It first, It last, ItD d_first) {
        while (first != last) {
            *d_first++ = *--last;
        }
        return d_first;
    }

    template<class It>
    It rotate(It first, It middle, It last) {
        if (first == middle) {
            return last;
        }
        if (middle == last) {
            return first;
        }
        It i{ middle };
        do {
            std::swap(*first++, *i++);
            if (first == middle) {
                middle = i;
            }
        } while (i != last);
        It res{ first };
        while (middle != last) {
            It i{ middle };
            do {
                std::swap(*first++, *i++);
                if (first == middle) {
                    middle = i;
                }
            } while (i != last);
        }
        return res;
    }

    template<class It, class ItD>
    ItD rotate_copy(It first, It middle, It last, ItD d_first) {
        return copy(first, middle, copy(middle, last, d_first));
    }

    template<class It>
    It shift_left(It first, It last, std::iter_difference_t<It> count) {
        It source{ first };
        while (count-- != 0) {
            if (source == last) {
                return first;
            }
            ++source;
        }
        return move(source, last, first);
    }

    template<class It>
    It shift_right(It first, It last, std::iter_difference_t<It> count) {
        It dest{ first };
        while (count-- != 0) {
            if (dest == last) {
                return last;
            }
            ++dest;
        }
        It i{ first }, j{ dest };
        while (i != dest) {
            if (j == last) {
                move(first, i, dest);
                return dest;
            }
            ++i, ++j;
        }
        It buffer{ first };
        while (j != last) {
            std::swap(*buffer++, *i);
            ++i, ++j;
            if (buffer == dest) {
                buffer = first;
            }
        }
        move(first, buffer, move(buffer, dest, i));
        return dest;
    }

    template<std::random_access_iterator It, class URBG>
    void shuffle(It first, It last, URBG&& gen) {
        using diff_t = std::iter_difference_t<It>;
        using distr_t = std::uniform_int_distribution<diff_t>;
        using param_t = distr_t::param_type;
        distr_t dist;
        diff_t size{ last - first };
        while (size-- != 0) {
            std::swap(first[size], first[dist(gen, param_t{ 0, size })]);
        }
    }

    template<class It, class ItD, class size_t, class URBG>
    ItD sample(It first, It last, ItD d_first, size_t count, URBG&& gen) {
        using diff_t = std::iter_difference_t<It>;
        using distr_t = std::uniform_int_distribution<diff_t>;
        using param_t = distr_t::param_type;
        distr_t dist;
        if constexpr (std::forward_iterator<It>) {
            diff_t size{ std::distance(first, last) };
            if (count >= size) {
                return copy(first, last, d_first);
            }
            while (count != 0) {
                if (dist(gen, param_t{ 0, --size }) < count) {
                    *d_first++ = *first;
                    --count;
                }
                ++first;
            }
            return d_first;
        }
        else {
            diff_t size{ 0 };
            while (first != last && size != count) {
                d_first[size++] = *first;
                ++first;
            }
            It res{ d_first + size };
            while (first != last) {
                diff_t r{ dist(gen, param_t{ 0, size++ }) };
                if (r < count) {
                    d_first[r] = *first;
                }
                ++first;
            }
            return res;
        }
    }

    template<class It, class Pr = std::equal_to<>>
    It unique(It first, It last, Pr pred = {}) {
        if (first != last) {
            It i{ first };
            while (++i != last) {
                if (!pred(*first, *i)) {
                    *++first = std::move(*i);
                }
            }
            ++first;
        }
        return first;
    }

    template<class It, class ItD, class Pr = std::equal_to<>>
    ItD unique_copy(It first, It last, ItD d_first, Pr pred = {}) {
        using value_t = std::iter_value_t<It>;
        if (first != last) {
            value_t value;
            *d_first++ = value = *first;
            while (++first != last) {
                if (!pred(value, *first)) {
                    *d_first++ = value = *first;
                }
            }
        }
        return d_first;
    }

}

// partitioning operations
export namespace plastic {

    template<class It, class Pr>
    bool is_partitioned(It first, It last, Pr pred) {
        while (first != last && pred(*first)) {
            ++first;
        }
        while (first != last && !pred(*first)) {
            ++first;
        }
        return first == last;
    }

    template<class It, class Pr>
    It partition(It first, It last, Pr pred) {
        first = find_if_not(first, last, pred);
        if (first != last) {
            It i{ first };
            while (++i != last) {
                if (pred(*i)) {
                    std::swap(*i, *first);
                    ++first;
                }
            }
        }
        return first;
    }

    template<class It, class d_iter1, class d_iter2, class Pr>
    std::pair<d_iter1, d_iter2> partition_copy(It first, It last, d_iter1 d_first1, d_iter2 d_first2, Pr pred) {
        while (first != last) {
            (pred(*first) ? *d_first1++ : *d_first2++) = *first;
            ++first;
        }
        return { d_first1, d_first2 };
    }

    template<class It, class Pr>
    It stable_partition(It first, It last, Pr pred) {
        using value_t = std::iter_value_t<It>;
        first = find_if_not(first, last, pred);
        while (first != last && !pred(*--last));
        if (first != last) {
            value_t* buffer{ new value_t[std::distance(first, last) + 1] };
            value_t* p{ buffer };
            It i{ first };
            *p++ = std::move(*i++);
            while (i != last) {
                (pred(*i++) ? *first++ : *p++) = std::move(*i);
            }
            *first++ = std::move(*last);
            move(buffer, p, first);
            delete[] buffer;
        }
        return first;
    }

    template<class It, class Pr>
    It partition_point(It first, It last, Pr pred) {
        using diff_t = std::iter_difference_t<It>;
        diff_t size{ std::distance(first, last) };
        while (size != 0) {
            diff_t half{ size / 2 };
            It next{ std::next(first, half) };
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

}

// heap operations
export namespace plastic {

    template<std::random_access_iterator It, class Cmp = std::less<>>
    It is_heap_until(It first, It last, Cmp cmp = {}) {
        using diff_t = std::iter_difference_t<It>;
        if (first != last) {
            diff_t i{ 0 }, size{ last - first };
            while (++i != size) {
                if (cmp(first[(i - 1) >> 1], first[i])) {
                    return first + i;
                }
            }
        }
        return last;
    }

    template<std::random_access_iterator It, class Cmp = std::less<>>
    bool is_heap(It first, It last, Cmp cmp = {}) {
        return is_heap_until(first, last, cmp) == last;
    }

    namespace detail {

        template<std::random_access_iterator It, class Cmp = std::less<>>
        void sift_up(It first, It middle, It last, Cmp cmp = {}) {
            using value_t = std::iter_value_t<It>;
            using diff_t = std::iter_difference_t<It>;
            if (first != last) {
                value_t value{ std::move(*middle) };
                diff_t hole{ middle - first };
                while (hole != 0) {
                    diff_t parent{ (hole - 1) / 2 };
                    if (!cmp(first[parent], value)) {
                        break;
                    }
                    first[hole] = std::move(first[parent]);
                    hole = parent;
                }
                first[hole] = std::move(value);
            }
        }

        template<std::random_access_iterator It, class Cmp = std::less<>>
        void sift_down(It first, It middle, It last, Cmp cmp = {}) {
            using value_t = std::iter_value_t<It>;
            using diff_t = std::iter_difference_t<It>;
            if (first != last) {
                value_t value{ std::move(*middle) };
                diff_t hole{ middle - first }, size{ last - first }, child;
                while (child = hole * 2 + 1, child < size) {
                    if (child + 1 < size && cmp(first[child], first[child + 1])) {
                        ++child;
                    }
                    if (!cmp(value, first[child])) {
                        break;
                    }
                    first[hole] = std::move(first[child]);
                    hole = child;
                }
                first[hole] = std::move(value);
            }
        }

    }

    template<std::random_access_iterator It, class Cmp = std::less<>>
    void make_heap(It first, It last, Cmp cmp = {}) {
        It i{ first + (last - first) / 2 };
        while (i != first) {
            detail::sift_down(first, --i, last, cmp);
        }
    }

    template<std::random_access_iterator It, class Cmp = std::less<>>
    void push_heap(It first, It last, Cmp cmp = {}) {
        detail::sift_up(first, last - 1, last, cmp);
    }

    template<std::random_access_iterator It, class Cmp = std::less<>>
    void pop_heap(It first, It last, Cmp cmp = {}) {
        std::swap(*first, *--last);
        detail::sift_down(first, first, last, cmp);
    }

    template<std::random_access_iterator It, class Cmp = std::less<>>
    void sort_heap(It first, It last, Cmp cmp = {}) {
        while (first != last) {
            pop_heap(first, last--, cmp);
        }
    }

}

// sorting operations
export namespace plastic {

    template<class It, class Cmp = std::less<>>
    It is_sorted_until(It first, It last, Cmp cmp = {}) {
        if (first != last) {
            It i{ first };
            while (++i != last) {
                if (cmp(*i, *first++)) {
                    return i;
                }
            }
        }
        return last;
    }

    template<class It, class Cmp = std::less<>>
    bool is_sorted(It first, It last, Cmp cmp = {}) {
        return is_sorted_until(first, last, cmp) == last;
    }

    namespace detail {

        constexpr std::size_t insertion_sort_threshold{ 32 };

        template<class It, class Cmp = std::less<>>
        void insertion_sort(It first, It last, Cmp cmp = {}) {
            using value_t = std::iter_value_t<It>;
            if (first != last) {
                It i{ first };
                while (++i != last) {
                    value_t value{ std::move(*i) };
                    It j{ i };
                    while (j != first && cmp(value, *--j)) {
                        *i-- = std::move(*j);
                    }
                    *i = std::move(value);
                }
            }
        }

        template<class It, class Cmp = std::less<>>
        void triple_sort(It a, It b, It c, Cmp cmp = {}) {
            if (cmp(*b, *a)) {
                std::swap(*b, *a);
            }
            if (cmp(*c, *b)) {
                std::swap(*c, *b);
                if (cmp(*b, *a)) {
                    std::swap(*b, *a);
                }
            }
        }

        template<std::random_access_iterator It, class Cmp = std::less<>>
        void intro_sort(It first, It last, std::iter_difference_t<It> threshold, Cmp cmp = {}) {
            using diff_t = std::iter_difference_t<It>;
            if (last - first <= detail::insertion_sort_threshold) {
                detail::insertion_sort(first, last, cmp);
            }
            else if (threshold == 0) {
                make_heap(first, last, cmp);
                sort_heap(first, last, cmp);
            }
            else {
                It median{ first + (last - first) / 2 };
                detail::triple_sort(first, median, last - 1);
                It point{ partition(first, last, [&](auto&& param) { return cmp(param, *median); }) };
                threshold = threshold / 2 + threshold / 4;
                detail::intro_sort(first, point, threshold, cmp);
                detail::intro_sort(point, last, threshold, cmp);
            }
        }

    }

    template<std::random_access_iterator It, class Cmp = std::less<>>
    void sort(It first, It last, Cmp cmp = {}) {
        detail::intro_sort(first, last, last - first, cmp);
    }

    template<std::random_access_iterator It, class Cmp = std::less<>>
    void partial_sort(It first, It middle, It last, Cmp cmp = {}) {
        make_heap(first, middle, cmp);
        It i{ middle };
        while (i != last) {
            if (cmp(*i, *first)) {
                std::swap(*i, *first);
                detail::sift_down(first, first, middle, cmp);
            }
            ++i;
        }
        sort_heap(first, middle, cmp);
    }

    template<class It, std::random_access_iterator ItD, class Cmp = std::less<>>
    ItD partial_sort_copy(It first, It last, ItD d_first, ItD d_last, Cmp cmp = {}) {
        ItD i{ d_first };
        while (first != last && i != d_last) {
            *i++ = *first;
            ++first;
        }
        make_heap(d_first, i, cmp);
        while (first != last) {
            if (cmp(*first, *d_first)) {
                *d_first = *first;
                detail::sift_down(d_first, d_first, i, cmp);
            }
            ++first;
        }
        sort_heap(d_first, i, cmp);
        return i;
    }

    template<std::random_access_iterator It, class Cmp = std::less<>>
    void stable_sort(It first, It last, Cmp cmp = {}) {
        using diff_t = std::iter_difference_t<It>;
        if (last - first <= detail::insertion_sort_threshold) {
            detail::insertion_sort(first, last, cmp);
        }
        else {
            It middle{ first + (last - first) / 2 };
            stable_sort(first, middle, cmp);
            stable_sort(middle, last, cmp);
            if (cmp(*middle, *std::prev(middle))) {
                inplace_merge(first, middle, last, cmp);
            }
        }
    }

    template<std::random_access_iterator It, class Cmp = std::less<>>
    void nth_element(It first, It middle, It last, Cmp cmp = {}) {
        using diff_t = std::iter_difference_t<It>;
        while (last - first > detail::insertion_sort_threshold) {
            It median{ first + (last - first) / 2 };
            detail::triple_sort(first, median, last - 1);
            It point{ partition(first, last, [&](auto&& param) { return cmp(param, *median); }) };
            if (point == middle) {
                return;
            }
            (point < middle ? first : last) = point;
        }
        detail::insertion_sort(first, last, cmp);
    }

}

// binary search operations
export namespace plastic {

    template<class It, class T = std::iter_value_t<It>, class Cmp = std::less<>>
    It lower_bound(It first, It last, const T& value, Cmp cmp = {}) {
        return partition_point(first, last, [&](auto&& param) { return cmp(param, value); });
    }

    template<class It, class T = std::iter_value_t<It>, class Cmp = std::less<>>
    It upper_bound(It first, It last, const T& value, Cmp cmp = {}) {
        return partition_point(first, last, [&](auto&& param) { return !cmp(value, param); });
    }

    template<class It, class T = std::iter_value_t<It>, class Cmp = std::less<>>
    bool binary_search(It first, It last, const T& value, Cmp cmp = {}) {
        first = lower_bound(first, last, value, cmp);
        return first != last && !cmp(value, *first);
    }

    template<class It, class T = std::iter_value_t<It>, class Cmp = std::less<>>
    std::pair<It, It> equal_range(It first, It last, const T& value, Cmp cmp = {}) {
        return { lower_bound(first, last, value, cmp), upper_bound(first, last, value, cmp) };
    }

}

// merge operations
export namespace plastic {

    template<class It1, class It2, std::output_iterator<std::iter_value_t<It1>> ItD, class Cmp = std::less<>>
    ItD merge(It1 first1, It1 last1, It2 first2, It2 last2, ItD d_first, Cmp cmp = {}) {
        while (first1 != last1 && first2 != last2) {
            if (cmp(*first2, *first1)) {
                *d_first++ = *first2;
                ++first2;
            }
            else {
                *d_first++ = *first1;
                ++first1;
            }
        }
        return copy(first2, last2, copy(first1, last1, d_first));
    }

    template<class It, class Cmp = std::less<>>
    void inplace_merge(It first, It middle, It last, Cmp cmp = {}) {
        using value_t = std::iter_value_t<It>;
        value_t* buffer{ new value_t[std::distance(first, last)] };
        move(buffer, merge(first, middle, middle, last, buffer, cmp), first);
        delete[] buffer;
    }

}

// set operations
export namespace plastic {

    template<class It1, class It2, class Cmp = std::less<>>
    bool includes(It1 first1, It1 last1, It2 first2, It2 last2, Cmp cmp = {}) {
        while (first1 != last1 && first2 != last2) {
            if (cmp(*first2, *first1)) {
                return false;
            }
            if (!cmp(*first1, *first2)) {
                ++first2;
            }
            ++first1;
        }
        return first2 == last2;
    }

    template<class It1, class It2, std::output_iterator<std::iter_value_t<It1>> ItD, class Cmp = std::less<>>
    ItD set_difference(It1 first1, It1 last1, It2 first2, It2 last2, ItD d_first, Cmp cmp = {}) {
        while (first1 != last1 && first2 != last2) {
            if (cmp(*first1, *first2)) {
                *d_first++ = *first1;
                ++first1;
            }
            else {
                if (!cmp(*first2, *first1)) {
                    ++first1;
                }
                ++first2;
            }
        }
        return copy(first1, last1, d_first);
    }

    template<class It1, class It2, std::output_iterator<std::iter_value_t<It1>> ItD, class Cmp = std::less<>>
    ItD set_intersection(It1 first1, It1 last1, It2 first2, It2 last2, ItD d_first, Cmp cmp = {}) {
        while (first1 != last1 && first2 != last2) {
            if (cmp(*first1, *first2)) {
                ++first1;
            }
            else if (cmp(*first2, *first1)) {
                ++first2;
            }
            else {
                *d_first++ = *first1;
                ++first1, ++first2;
            }
        }
        return d_first;
    }

    template<class It1, class It2, std::output_iterator<std::iter_value_t<It1>> ItD, class Cmp = std::less<>>
    ItD set_symmetric_difference(It1 first1, It1 last1, It2 first2, It2 last2, ItD d_first, Cmp cmp = {}) {
        while (first1 != last1 && first2 != last2) {
            if (cmp(*first1, *first2)) {
                *d_first++ = *first1;
                ++first1;
            }
            else if (cmp(*first2, *first1)) {
                *d_first++ = *first2;
                ++first2;
            }
            else {
                ++first1, ++first2;
            }
        }
        return copy(first2, last2, copy(first1, last1, d_first));
    }

    template<class It1, class It2, std::output_iterator<std::iter_value_t<It1>> ItD, class Cmp = std::less<>>
    ItD set_union(It1 first1, It1 last1, It2 first2, It2 last2, ItD d_first, Cmp cmp = {}) {
        while (first1 != last1 && first2 != last2) {
            if (cmp(*first1, *first2)) {
                *d_first++ = *first1;
                ++first1;
            }
            else if (cmp(*first2, *first1)) {
                *d_first++ = *first2;
                ++first2;
            }
            else {
                *d_first++ = *first1;
                ++first1, ++first2;
            }
        }
        return copy(first2, last2, copy(first1, last1, d_first));
    }

}

// minimum/maximum operations
export namespace plastic {

    template<class It, class Cmp = std::less<>>
    It max_element(It first, It last, Cmp cmp = {}) {
        It res{ first };
        if (first != last) {
            while (++first != last) {
                if (cmp(*res, *first)) {
                    res = first;
                }
            }
        }
        return res;
    }

    template<class It, class Cmp = std::less<>>
    It min_element(It first, It last, Cmp cmp = {}) {
        It res{ first };
        if (first != last) {
            while (++first != last) {
                if (cmp(*first, *res)) {
                    res = first;
                }
            }
        }
        return res;
    }

    template<class It, class Cmp = std::less<>>
    std::pair<It, It> minmax_element(It first, It last, Cmp cmp = {}) {
        return { min_element(first, last, cmp), max_element(first, last, cmp) };
    }

    template<class T, class Cmp = std::less<>>
    const T& max(const T& a, const T& b, Cmp cmp = {}) {
        return cmp(a, b) ? b : a;
    }

    template<class T, class Cmp = std::less<>>
    T max(std::initializer_list<T> list, Cmp cmp = {}) {
        return *max_element(list.begin(), list.end(), cmp);
    }

    template<class T, class Cmp = std::less<>>
    const T& min(const T& a, const T& b, Cmp cmp = {}) {
        return cmp(b, a) ? b : a;
    }

    template<class T, class Cmp = std::less<>>
    T min(std::initializer_list<T> list, Cmp cmp = {}) {
        return *min_element(list.begin(), list.end(), cmp);
    }

    template<class T, class Cmp = std::less<>>
    std::pair<const T&, const T&> minmax(const T& a, const T& b, Cmp cmp = {}) {
        return { min(a, b, cmp), max(a, b, cmp) };
    }

    template<class T, class Cmp = std::less<>>
    std::pair<T, T> minmax(std::initializer_list<T> list, Cmp cmp = {}) {
        return { min(list, cmp), max(list, cmp) };
    }

    template<class T, class Cmp = std::less<>>
    const T& clamp(const T& value, const T& lowest, const T& highest, Cmp cmp = {}) {
        return cmp(value, lowest) ? lowest : cmp(highest, value) ? highest : value;
    }

}

// comparison operations
export namespace plastic {

    template<class It1, class It2, class Pr = std::equal_to<>>
    bool equal(It1 first1, It1 last1, It2 first2, Pr pred = {}) {
        while (first1 != last1) {
            if (!pred(*first1, *first2)) {
                return false;
            }
            ++first1, ++first2;
        }
        return true;
    }

    template<class It1, class It2, class Pr = std::equal_to<>>
    bool equal(It1 first1, It1 last1, It2 first2, It2 last2, Pr pred = {}) {
        while (first1 != last1 && first2 != last2) {
            if (!pred(*first1, *first2)) {
                return false;
            }
            ++first1, ++first2;
        }
        return first1 == last1 && first2 == last2;
    }

    template<class It1, class It2, class Cmp = std::less<>>
    bool lexicographical_compare(It1 first1, It1 last1, It2 first2, It2 last2, Cmp cmp = {}) {
        while (first1 != last1 && first2 != last2) {
            if (cmp(*first1, *first2)) {
                return true;
            }
            if (cmp(*first2++, *first1++)) {
                return false;
            }
        }
        return first1 == last1 && first2 != last2;
    }

    template<class It1, class It2, class Cmp = std::compare_three_way>
    std::compare_three_way_result_t<It1, It2> lexicographical_compare_three_way(It1 first1, It1 last1, It2 first2, It2 last2, Cmp cmp = {}) {
        using result_t = std::compare_three_way_result_t<It1, It2>;
        while (first1 != last1 && first2 != last2) {
            result_t cmp{ cmp(*first1++, *first2++) };
            if (cmp != 0) {
                return cmp;
            }
        }
        return first1 != last1 ? std::strong_ordering::greater : first2 != last2 ? std::strong_ordering::less : std::strong_ordering::equal;
    }

}

// permutation operations
export namespace plastic {

    template<class It1, class It2, class Pr = std::equal_to<>>
    bool is_permutation(It1 first1, It1 last1, It2 first2, It2 last2, Pr cmp = {}) {
        std::tie(first1, first2) = mismatch(first1, last1, first2, last2, cmp);
        if ((first1 == last1) != (first2 == last2)) {
            return false;
        }
        if (first1 != last1) {
            It1 i{ first1 };
            while (i != last1) {
                if (count(i, last1, *i) != count(first2, last2, *i)) {
                    return false;
                }
                ++i;
            }
        }
        return true;
    }

    template<class It1, class It2, class Pr = std::equal_to<>>
    bool is_permutation(It1 first1, It1 last1, It2 first2, Pr cmp = {}) {
        std::tie(first1, first2) = mismatch(first1, last1, first2, cmp);
        return first1 == last1 || is_permutation(first1, last1, first2, std::next(first2, std::distance(first1, last1)), cmp);
    }

    template<class It, class Cmp = std::less<>>
    bool next_permutation(It first, It last, Cmp cmp = {}) {
        It i{ last }, j{ last };
        if (first != last && first != --i) {
            while (!cmp(*--i, *--j)) {
                if (i == first) {
                    reverse(first, last);
                    return false;
                }
            }
            j = last;
            while (!cmp(*i, *--j));
            std::swap(*i, *j);
            reverse(++i, last);
            return true;
        }
        return false;
    }

    template<class It, class Cmp = std::less<>>
    bool prev_permutation(It first, It last, Cmp cmp = {}) {
        return next_permutation(first, last, [&](auto&& param1, auto&& param2) { return cmp(param2, param1); });
    }

}