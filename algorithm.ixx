module;

#include <assert.h>

export module plastic:algorithm;

import std;

// temporary fixes
namespace std {

    // introduced in C++20, but EDG cannot recognize it within modules
#ifdef __EDG__
    template<class Fn, class... It>
        requires (std::indirectly_readable<It> && ...) && std::invocable<Fn, std::iter_reference_t<It>...>
    using indirect_result_t = std::invoke_result_t<Fn, std::iter_reference_t<It>...>;
#endif

    // introduced in C++26, but not yet implemented in MSVC
    template<std::indirectly_readable It, std::indirectly_regular_unary_invocable<It> Pj>
    using projected_value_t = std::remove_cvref_t<std::invoke_result_t<Pj&, std::iter_value_t<It>&>>;

}

// exposition-only concepts
namespace plastic {

    template<class Fn>
    class flipped {
        Fn _func;
    public:
        template<class T, class U>
            requires std::invocable<Fn&, U, T>
        std::invoke_result_t<Fn&, U, T> operator()(T&&, U&&);
    };

    template<class Fn, class T, class It, class U>
    concept indirectly_binary_left_foldable_impl =
        std::movable<T>
        && std::movable<U>
        && std::convertible_to<T, U>
        && std::invocable<Fn&, U, std::iter_reference_t<It>>
        && std::assignable_from<U&, std::invoke_result_t<Fn&, U, std::iter_reference_t<It>>>;

    template<class Fn, class T, class It>
    concept indirectly_binary_left_foldable =
        std::copy_constructible<Fn>
        && std::indirectly_readable<It>
        && std::invocable<Fn&, T, std::iter_reference_t<It>>
        && std::convertible_to<std::invoke_result_t<Fn&, T, std::iter_reference_t<It>>, std::decay_t<std::invoke_result_t<Fn&, T, std::iter_reference_t<It>>>>
        && indirectly_binary_left_foldable_impl<Fn, T, It, std::decay_t<std::invoke_result_t<Fn&, T, std::iter_reference_t<It>>>>;

    template <class Fn, class T, class It>
    concept indirectly_binary_right_foldable = indirectly_binary_left_foldable<flipped<Fn>, T, It>;

}

// for internal implementation
namespace plastic {

    template<class Fn, class T, class It>
    using fold_left_result_t = std::decay_t<std::invoke_result_t<Fn&, T, std::iter_reference_t<It>>>;
    template<class Fn, class T, class It>
    using fold_right_result_t = std::decay_t<std::invoke_result_t<flipped<Fn>&, T, std::iter_reference_t<It>>>;

    struct satisfy_value {};
    struct satisfy_predicate {};
    struct satisfy_negated_predicate {};

    template<class Sat, class T, class UPr>
        requires std::same_as<Sat, satisfy_value> || std::same_as<Sat, satisfy_predicate> || std::same_as<Sat, satisfy_negated_predicate>
    constexpr bool satisfy(const T& given, const UPr& value_or_pred) {
        if constexpr (std::same_as<Sat, satisfy_value>) {
            return given == value_or_pred;
        }
        else if constexpr (std::same_as<Sat, satisfy_predicate>) {
            return value_or_pred(given);
        }
        else {
            return !value_or_pred(given);
        }
    }

}

// comparison operations
namespace plastic {

    export
        template<std::input_iterator It1, std::sentinel_for<It1> Se1, std::input_iterator It2, std::sentinel_for<It2> Se2, class Pr = std::ranges::equal_to, class Pj1 = std::identity, class Pj2 = std::identity>
        requires std::indirectly_comparable<It1, It2, Pr, Pj1, Pj2>
    constexpr bool equal(It1 first1, Se1 last1, It2 first2, Se2 last2, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        while (first2 != last2) {
            if (first1 == last1 || !pred(proj1(*first1), proj2(*first2))) {
                return false;
            }
            ++first1, ++first2;
        }
        return first1 == last1;
    }

    export
        template<std::input_iterator It1, std::sentinel_for<It1> Se1, std::input_iterator It2, std::sentinel_for<It2> Se2, class Pj1 = std::identity, class Pj2 = std::identity, std::indirect_strict_weak_order<std::projected<It1, Pj1>, std::projected<It2, Pj2>> Pr = std::ranges::less>
    constexpr bool lexicographical_compare(It1 first1, Se1 last1, It2 first2, Se2 last2, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        while (first2 != last2) {
            if (first1 == last1 || pred(proj1(*first1), proj2(*first2))) {
                return true;
            }
            if (pred(proj2(*first2), proj1(*first1))) {
                return false;
            }
            ++first1, ++first2;
        }
        return false;
    }

}

// non-modifying sequence operations
namespace plastic {

    export
        template<std::input_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirectly_unary_invocable<std::projected<It, Pj>> Fn>
    constexpr std::ranges::in_fun_result<It, Fn> for_each(It first, Se last, Fn func, Pj proj = {}) {
        while (first != last) {
            func(proj(*first));
            ++first;
        }
        return { std::move(first), std::move(func) };
    }

    export
        template<std::input_iterator It, class Pj = std::identity, std::indirectly_unary_invocable<std::projected<It, Pj>> Fn>
    constexpr std::ranges::in_fun_result<It, Fn> for_each_n(It first, std::iter_difference_t<It> count, Fn func, Pj proj = {}) {
        assert(count >= 0);
        while (count-- != 0) {
            func(proj(*first));
            ++first;
        }
        return { std::move(first), std::move(func) };
    }

    template<class Sat, class It, class Se, class TPr, class Pj>
    constexpr std::iter_difference_t<It> count_impl(It first, Se last, const TPr& value_or_pred, Pj proj) {
        std::iter_difference_t<It> count{};
        while (first != last) {
            if (plastic::satisfy<Sat>(proj(*first), value_or_pred)) {
                ++count;
            }
            ++first;
        }
        return count;
    }

    export
        template<std::input_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, class T = std::projected_value_t<It, Pj>>
        requires std::indirect_binary_predicate<std::ranges::equal_to, std::projected<It, Pj>, const T*>
    constexpr std::iter_difference_t<It> count(It first, Se last, const T& value, Pj proj = {}) {
        return plastic::count_impl<satisfy_value>(first, last, value, proj);
    }

    export
        template<std::input_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
    constexpr std::iter_difference_t<It> count_if(It first, Se last, Pr pred, Pj proj = {}) {
        return plastic::count_impl<satisfy_predicate>(first, last, pred, proj);
    }

    export
        template<std::input_iterator It1, std::sentinel_for<It1> S1, std::input_iterator It2, std::sentinel_for<It2> S2, class Pr = std::ranges::equal_to, class Pj1 = std::identity, class Pj2 = std::identity>
        requires std::indirectly_comparable<It1, It2, Pr, Pj1, Pj2>
    constexpr std::ranges::in_in_result<It1, It2> mismatch(It1 first1, S1 last1, It2 first2, S2 last2, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        while (first1 != last1 && first2 != last2) {
            if (!pred(proj1(*first1), proj2(*first2))) {
                break;
            }
            ++first1, ++first2;
        }
        return { std::move(first1), std::move(first2) };
    }

    template<class Sat, class It, class Se, class TPr, class Pj>
    constexpr It find_impl(It first, Se last, const TPr& value_or_pred, Pj proj) {
        while (first != last) {
            if (plastic::satisfy<Sat>(proj(*first), value_or_pred)) {
                break;
            }
            ++first;
        }
        return first;
    }

    export
        template<std::input_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, class T = std::projected_value_t<It, Pj>>
        requires std::indirect_binary_predicate<std::ranges::equal_to, std::projected<It, Pj>, const T*>
    constexpr It find(It first, Se last, const T& value, Pj proj = {}) {
        return plastic::find_impl<satisfy_value>(first, last, value, proj);
    }

    export
        template<std::input_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
    constexpr It find_if(It first, Se last, Pr pred, Pj proj = {}) {
        return plastic::find_impl<satisfy_predicate>(first, last, pred, proj);
    }

    export
        template<std::input_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
    constexpr It find_if_not(It first, Se last, Pr pred, Pj proj = {}) {
        return plastic::find_impl<satisfy_negated_predicate>(first, last, pred, proj);
    }

    export
        template<std::input_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
    constexpr bool all_of(It first, Se last, Pr pred, Pj proj = {}) {
        return plastic::find_if_not(first, last, pred, proj) == last;
    }

    export
        template<std::input_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
    constexpr bool any_of(It first, Se last, Pr pred, Pj proj = {}) {
        return plastic::find_if(first, last, pred, proj) != last;
    }

    export
        template<std::input_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
    constexpr bool none_of(It first, Se last, Pr pred, Pj proj = {}) {
        return plastic::find_if(first, last, pred, proj) == last;
    }

    template<class Sat, class It, class Se, class TPr, class Pj>
    constexpr std::ranges::subrange<It> find_last_impl(It first, Se last, const TPr& value_or_pred, Pj proj) {
        It i;
        bool found{};
        while (first != last) {
            if (plastic::satisfy<Sat>(proj(*first), value_or_pred)) {
                i = first;
                found = true;
            }
            ++first;
        }
        if (!found) {
            i = first;
        }
        return { std::move(i), std::move(first) };
    }

    export
        template<std::forward_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, class T = std::projected_value_t<It, Pj>>
        requires std::indirect_binary_predicate<std::ranges::equal_to, std::projected<It, Pj>, const T*>
    constexpr std::ranges::subrange<It> find_last(It first, Se last, const T& value, Pj proj = {}) {
        return plastic::find_last_impl<satisfy_value>(first, last, value, proj);
    }

    export
        template<std::forward_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
    constexpr std::ranges::subrange<It> find_last_if(It first, Se last, Pr pred, Pj proj = {}) {
        return plastic::find_last_impl<satisfy_predicate>(first, last, pred, proj);
    }

    export
        template<std::forward_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
    constexpr std::ranges::subrange<It> find_last_if_not(It first, Se last, Pr pred, Pj proj = {}) {
        return plastic::find_last_impl<satisfy_negated_predicate>(first, last, pred, proj);
    }

    export
        template<std::forward_iterator It1, std::sentinel_for<It1> Se1, std::forward_iterator It2, std::sentinel_for<It2> Se2, class Pr = std::ranges::equal_to, class Pj1 = std::identity, class Pj2 = std::identity>
        requires std::indirectly_comparable<It1, It2, Pr, Pj1, Pj2>
    constexpr std::ranges::subrange<It1> find_end(It1 first1, Se1 last1, It2 first2, Se2 last2, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        std::ranges::subrange<It1> range;
        bool found{};
        while (true) {
            It1 i{ first1 };
            It2 j{ first2 };
            do {
                if (j == last2) {
                    range = { first1, i };
                    found = true;
                }
                if (i == last1) {
                    if (!found) {
                        range = { i, i };
                    }
                    return range;
                }
            } while (j != last2 && pred(proj1(*i++), proj2(*j++)));
            ++first1;
        }
    }

    export
        template<std::input_iterator It1, std::sentinel_for<It1> Se1, std::forward_iterator It2, std::sentinel_for<It2> Se2, class Pr = std::ranges::equal_to, class Pj1 = std::identity, class Pj2 = std::identity>
        requires std::indirectly_comparable<It1, It2, Pr, Pj1, Pj2>
    constexpr It1 find_first_of(It1 first1, Se1 last1, It2 first2, Se2 last2, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        while (first1 != last1) {
            It2 i{ first2 };
            while (i != last2) {
                if (pred(proj1(*first1), proj2(*i++))) {
                    return first1;
                }
            }
            ++first1;
        }
        return first1;
    }

    export
        template<std::forward_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_binary_predicate<std::projected<It, Pj>, std::projected<It, Pj>> Pr = std::ranges::equal_to>
    constexpr It adjacent_find(It first, Se last, Pr pred = {}, Pj proj = {}) {
        if (first == last) {
            return first;
        }

        It i{ first };
        while (++i != last) {
            if (pred(proj(*first), proj(*i))) {
                return first;
            }
            ++first;
        }
        return i;
    }

    export
        template<std::forward_iterator It1, std::sentinel_for<It1> Se1, std::forward_iterator It2, std::sentinel_for<It2> Se2, class Pr = std::ranges::equal_to, class Pj1 = std::identity, class Pj2 = std::identity>
        requires std::indirectly_comparable<It1, It2, Pr, Pj1, Pj2>
    constexpr std::ranges::subrange<It1> search(It1 first1, Se1 last1, It2 first2, Se2 last2, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        while (true) {
            It1 i{ first1 };
            It2 j{ first2 };
            do {
                if (j == last2) {
                    return { std::move(first1), std::move(i) };
                }
                if (i == last1) {
                    return { i, i };
                }
            } while (pred(proj1(*i++), proj2(*j++)));
            ++first1;
        }
    }

    export
        template<std::forward_iterator It, std::sentinel_for<It> Se, class Pr = std::ranges::equal_to, class Pj = std::identity, class T = std::projected_value_t<It, Pj>>
        requires std::indirectly_comparable<It, const T*, Pr, Pj>
    constexpr std::ranges::subrange<It> search_n(It first, Se last, std::iter_difference_t<It> count, const T& value, Pr pred = {}, Pj proj = {}) {
        if (count <= 0) {
            return { first, first };
        }

        while (first != last) {
            if (pred(proj(*first), value)) {
                It i{ first };
                auto n{ count };
                ++i, --n;
                do {
                    if (n-- == 0) {
                        return { std::move(first), std::move(i) };
                    }
                    if (i == last) {
                        return { i, i };
                    }
                } while (pred(proj(*i++), value));
                first = i;
            }
            else {
                ++first;
            }
        }
        return { first, first };
    }

    export
        template<std::input_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, class T = std::projected_value_t<It, Pj>>
        requires std::indirect_binary_predicate<std::ranges::equal_to, std::projected<It, Pj>, const T*>
    constexpr bool contains(It first, Se last, const T& value, Pj proj = {}) {
        return plastic::find(first, last, value, proj) != last;
    }

    export
        template<std::forward_iterator It1, std::sentinel_for<It1> Se1, std::forward_iterator It2, std::sentinel_for<It2> Se2, class Pr = std::ranges::equal_to, class Pj1 = std::identity, class Pj2 = std::identity>
        requires std::indirectly_comparable<It1, It2, Pr, Pj1, Pj2>
    constexpr bool contains_subrange(It1 first1, Se1 last1, It2 first2, Se2 last2, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        return first2 == last2 || !plastic::search(first1, last1, first2, last2, pred, proj1, proj2).empty();
    }

    export
        template<std::input_iterator It1, std::sentinel_for<It1> Se1, std::input_iterator It2, std::sentinel_for<It2> Se2, class Pr = std::ranges::equal_to, class Pj1 = std::identity, class Pj2 = std::identity>
        requires std::indirectly_comparable<It1, It2, Pr, Pj1, Pj2>
    constexpr bool starts_with(It1 first1, Se1 last1, It2 first2, Se2 last2, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        return plastic::mismatch(first1, last1, first2, last2, pred, proj1, proj2).in2 == last2;
    }

    export
        template<std::input_iterator It1, std::sentinel_for<It1> Se1, std::input_iterator It2, std::sentinel_for<It2> Se2, class Pr = std::ranges::equal_to, class Pj1 = std::identity, class Pj2 = std::identity>
        requires (std::forward_iterator<It1> || std::sized_sentinel_for<Se1, It1>) && (std::forward_iterator<It2> || std::sized_sentinel_for<Se2, It2>) && std::indirectly_comparable<It1, It2, Pr, Pj1, Pj2>
    constexpr bool ends_with(It1 first1, Se1 last1, It2 first2, Se2 last2, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        if constexpr (std::forward_iterator<It1> && std::forward_iterator<It2>) {
            It1 i{ first1 };
            It2 j{ first2 };
            while (j != last2) {
                if (i == last1) {
                    return false;
                }
                ++i, ++j;
            }
            while (i != last1) {
                ++i, ++first1;
            }
        }
        else if constexpr (std::forward_iterator<It1>) {
            It1 i{ std::ranges::next(first1, last2 - first2, last1) };
            if (i == last1) {
                return false;
            }
            while (i != last1) {
                ++i, ++first1;
            }
        }
        else if constexpr (std::forward_iterator<It2>) {
            auto size1{ last1 - first1 }, size2{};
            It2 i{ first2 };
            while (i != last2) {
                if (size1 == size2) {
                    return false;
                }
                ++i, ++size2;
            }
            first1 = std::ranges::next(first1, size1 - size2, last1);
        }
        else {
            first1 = std::ranges::next(first1, (last2 - first2) - (last1 - first1), last1);
        }

        return plastic::equal(first1, last1, first2, last2, pred, proj1, proj2);
    }

}

// fold operations
namespace plastic {

    export
        template<std::input_iterator It, std::sentinel_for<It> Se, class T = std::iter_value_t<It>, indirectly_binary_left_foldable<T, It> Fn>
    constexpr std::ranges::in_value_result<It, fold_left_result_t<Fn, T, It>> fold_left_with_iter(It first, Se last, T init, Fn func) {
        using U = fold_left_result_t<Fn, T, It>;

        if (first == last) {
            return { std::move(first), static_cast<U>(std::move(init)) };
        }

        U value{ func(init, *first) };
        ++first;
        while (first != last) {
            value = func(value, *first);
            ++first;
        }

        return { std::move(first), std::move(value) };
    }

    export
        template<std::input_iterator It, std::sentinel_for<It> Se, indirectly_binary_left_foldable<std::iter_value_t<It>, It> Fn>
        requires std::constructible_from<std::iter_value_t<It>, std::iter_reference_t<It>>
    constexpr std::ranges::in_value_result<It, std::optional<fold_left_result_t<Fn, std::iter_value_t<It>, It>>> fold_left_first_with_iter(It first, Se last, Fn func) {
        using U = fold_left_result_t<Fn, std::iter_value_t<It>, It>;

        if (first == last) {
            return { std::move(first), {} };
        }

        std::optional<U> opt{ std::in_place, *first };
        U& value{ *opt };
        ++first;
        while (first != last) {
            value = func(value, *first);
            ++first;
        }

        return { std::move(first), std::move(opt) };
    }

    export
        template<std::input_iterator It, std::sentinel_for<It> Se, class T = std::iter_value_t<It>, indirectly_binary_left_foldable<T, It> Fn>
    constexpr fold_left_result_t<Fn, T, It> fold_left(It first, Se last, T init, Fn func) {
        return plastic::fold_left_with_iter(first, last, init, func).value;
    }

    export
        template<std::input_iterator It, std::sentinel_for<It> Se, indirectly_binary_left_foldable<std::iter_value_t<It>, It> Fn>
        requires std::constructible_from<std::iter_value_t<It>, std::iter_reference_t<It>>
    constexpr std::optional<fold_left_result_t<Fn, std::iter_value_t<It>, It>> fold_left_first(It first, Se last, Fn func) {
        return plastic::fold_left_first_with_iter(first, last, func).value;
    }

    export
        template<std::bidirectional_iterator It, std::sentinel_for<It> Se, class T = std::iter_value_t<It>, indirectly_binary_right_foldable<T, It> Fn>
    constexpr fold_right_result_t<Fn, T, It> fold_right(It first, Se last, T init, Fn func) {
        using U = fold_right_result_t<Fn, T, It>;

        if (first == last) {
            return static_cast<U>(std::move(init));
        }

        It i{ std::ranges::next(first, last) };
        U value{ func(*--i, init) };
        while (first != i) {
            value = func(*--i, value);
        }

        return value;
    }

    export
        template<std::bidirectional_iterator It, std::sentinel_for<It> Se, indirectly_binary_right_foldable<std::iter_value_t<It>, It> Fn>
        requires std::constructible_from<std::iter_value_t<It>, std::iter_reference_t<It>>
    constexpr std::optional<fold_right_result_t<Fn, std::iter_value_t<It>, It>> fold_right_last(It first, Se last, Fn func) {
        using U = fold_right_result_t<Fn, std::iter_value_t<It>, It>;

        if (first == last) {
            return {};
        }

        It i{ std::ranges::next(first, last) };
        std::optional<U> opt{ std::in_place, *--i };
        U& value{ *opt };
        while (first != i) {
            value = func(*--i, value);
        }

        return opt;
    }

}

// modifying sequence operations
namespace plastic {

    export
        template<std::input_iterator It, std::sentinel_for<It> Se, std::weakly_incrementable Out>
        requires std::indirectly_copyable<It, Out>
    constexpr std::ranges::in_out_result<It, Out> copy(It first, Se last, Out output) {
        while (first != last) {
            *output = *first;
            ++first, ++output;
        }
        return { std::move(first), std::move(output) };
    }

    export
        template<std::input_iterator It, std::sentinel_for<It> Se, std::weakly_incrementable Out, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
        requires std::indirectly_copyable<It, Out>
    constexpr std::ranges::in_out_result<It, Out> copy_if(It first, Se last, Out output, Pr pred, Pj proj = {}) {
        while (first != last) {
            if (pred(proj(*first))) {
                *output = *first;
                ++output;
            }
            ++first;
        }
        return { std::move(first), std::move(output) };
    }

    export
        template<std::input_iterator It, std::weakly_incrementable Out>
        requires std::indirectly_copyable<It, Out>
    constexpr std::ranges::in_out_result<It, Out> copy_n(It first, std::iter_difference_t<It> count, Out output) {
        if (count <= 0) {
            return { std::move(first), std::move(output) };
        }

        while (count-- != 0) {
            *output = *first;
            ++first, ++output;
        }

        return { std::move(first), std::move(output) };
    }

    export
        template<std::bidirectional_iterator It1, std::sentinel_for<It1> Se1, std::bidirectional_iterator It2>
        requires std::indirectly_copyable<It1, It2>
    constexpr std::ranges::in_out_result<It1, It2> copy_backward(It1 first, Se1 last, It2 output) {
        It1 r_last{ std::ranges::next(first, last) }, i{ r_last };
        while (first != i) {
            *--output = *--i;
        }
        return { std::move(r_last), std::move(output) };
    }

    export
        template<std::input_iterator It, std::sentinel_for<It> Se, std::weakly_incrementable Out>
        requires std::indirectly_movable<It, Out>
    constexpr std::ranges::in_out_result<It, Out> move(It first, Se last, Out output) {
        while (first != last) {
            *output = std::move(*first);
            ++first, ++output;
        }
        return { std::move(first), std::move(output) };
    }

    export
        template<std::bidirectional_iterator It1, std::sentinel_for<It1> Se1, std::bidirectional_iterator It2>
        requires std::indirectly_movable<It1, It2>
    constexpr std::ranges::in_out_result<It1, It2> move_backward(It1 first, Se1 last, It2 output) {
        It1 r_last{ std::ranges::next(first, last) }, i{ r_last };
        while (first != i) {
            *--output = std::move(*--i);
        }
        return { std::move(r_last), std::move(output) };
    }

    export
        template<class Out, std::sentinel_for<Out> Se, class T = std::iter_value_t<Out>>
        requires std::output_iterator<Out, const T&>
    constexpr Out fill(Out first, Se last, const T& value) {
        while (first != last) {
            *first = value;
            ++first;
        }
        return first;
    }

    export
        template<class Out, class T = std::iter_value_t<Out>>
        requires std::output_iterator<Out, const T&>
    constexpr Out fill_n(Out first, std::iter_difference_t<Out> count, const T& value) {
        if (count <= 0) {
            return first;
        }

        while (count-- != 0) {
            *first = value;
            ++first;
        }

        return first;
    }

    export
        template<std::input_iterator It, std::sentinel_for<It> Se, std::weakly_incrementable Out, std::copy_constructible Fn, class Pj = std::identity>
        requires std::indirectly_writable<Out, std::indirect_result_t<Fn&, std::projected<It, Pj>>>
    constexpr std::ranges::in_out_result<It, Out> transform(It first, Se last, Out output, Fn func, Pj proj = {}) {
        while (first != last) {
            *output = func(proj(*first));
            ++first, ++output;
        }
        return { std::move(first), std::move(output) };
    }

    export
        template<std::input_iterator It1, std::sentinel_for<It1> Se1, std::input_iterator It2, std::sentinel_for<It2> Se2, std::weakly_incrementable Out, std::copy_constructible Fn, class Pj1 = std::identity, class Pj2 = std::identity>
        requires std::indirectly_writable<Out, std::indirect_result_t<Fn&, std::projected<It1, Pj1>, std::projected<It2, Pj2>>>
    constexpr std::ranges::in_in_out_result<It1, It2, Out> transform(It1 first1, Se1 last1, It2 first2, Se2 last2, Out output, Fn func, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        while (first1 != last1 && first2 != last2) {
            *output = func(proj1(*first1), proj2(*first2));
            ++first1, ++first2, ++output;
        }
        return { std::move(first1), std::move(first2), std::move(output) };
    }

    export
        template<std::input_or_output_iterator Out, std::sentinel_for<Out> Se, std::copy_constructible Fn>
        requires std::invocable<Fn&>&& std::indirectly_writable<Out, std::invoke_result_t<Fn&>>
    constexpr Out generate(Out first, Se last, Fn func) {
        while (first != last) {
            *first = func();
            ++first;
        }
        return first;
    }

    export
        template<std::input_or_output_iterator Out, std::copy_constructible Fn>
        requires std::invocable<Fn&>&& std::indirectly_writable<Out, std::invoke_result_t<Fn&>>
    constexpr Out generate_n(Out first, std::iter_difference_t<Out> count, Fn func) {
        if (count <= 0) {
            return first;
        }

        while (count-- != 0) {
            *first = func();
            ++first;
        }

        return first;
    }

    template<class Sat, class It, class Se, class TPr, class Pj>
    constexpr std::ranges::subrange<It> remove_impl(It first, Se last, const TPr& value_or_pred, Pj proj) {
        first = plastic::find_impl<Sat>(first, last, value_or_pred, proj);
        if (first == last) {
            return { first, first };
        }

        It i{ first };
        while (++i != last) {
            if (!plastic::satisfy<Sat>(proj(*i), value_or_pred)) {
                *first++ = std::move(*i);
            }
        }

        return { std::move(first), std::move(i) };
    }

    export
        template<std::permutable It, std::sentinel_for<It> Se, class Pj = std::identity, class T = std::projected_value_t<It, Pj>>
        requires std::indirect_binary_predicate<std::ranges::equal_to, std::projected<It, Pj>, const T*>
    constexpr std::ranges::subrange<It> remove(It first, Se last, const T& value, Pj proj = {}) {
        return plastic::remove_impl<satisfy_value>(first, last, value, proj);
    }

    export
        template<std::permutable It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
    constexpr std::ranges::subrange<It> remove_if(It first, Se last, Pr pred, Pj proj = {}) {
        return plastic::remove_impl<satisfy_predicate>(first, last, pred, proj);
    }

    template<class Sat, class It, class Se, class Out, class TPr, class Pj >
    constexpr std::ranges::in_out_result<It, Out> remove_copy_impl(It first, Se last, Out output, const TPr& value_or_pred, Pj proj) {
        while (first != last) {
            if (!plastic::satisfy<Sat>(proj(*first), value_or_pred)) {
                *output = *first;
                ++output;
            }
            ++first;
        }
        return { std::move(first), std::move(output) };
    }

    export
        template<std::input_iterator It, std::sentinel_for<It> Se, std::weakly_incrementable Out, class Pj = std::identity, class T = std::projected_value_t<It, Pj>>
        requires std::indirectly_copyable<It, Out>&& std::indirect_binary_predicate<std::ranges::equal_to, std::projected<It, Pj>, const T*>
    constexpr std::ranges::in_out_result<It, Out> remove_copy(It first, Se last, Out output, const T& value, Pj proj = {}) {
        return plastic::remove_copy_impl<satisfy_value>(first, last, output, value, proj);
    }

    export
        template<std::input_iterator It, std::sentinel_for<It> Se, std::weakly_incrementable Out, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
        requires std::indirectly_copyable<It, Out>
    constexpr std::ranges::in_out_result<It, Out> remove_copy_if(It first, Se last, Out output, Pr pred, Pj proj = {}) {
        return plastic::remove_copy_impl<satisfy_predicate>(first, last, output, pred, proj);
    }

    template<class Sat, class It, class Se, class TPr, class U, class Pj>
    constexpr It replace_impl(It first, Se last, const TPr& value_or_pred, const U& value, Pj proj) {
        while (first != last) {
            if (plastic::satisfy<Sat>(proj(*first), value_or_pred)) {
                *first = value;
            }
            ++first;
        }
        return first;
    }

    export
        template<std::input_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, class T = std::projected_value_t<It, Pj>, class U = T>
        requires std::indirectly_writable<It, const U&>&& std::indirect_binary_predicate<std::ranges::equal_to, std::projected<It, Pj>, const T*>
    constexpr It replace(It first, Se last, const T& old_value, const U& new_value, Pj proj = {}) {
        return plastic::replace_impl<satisfy_value>(first, last, old_value, new_value, proj);
    }

    export
        template<std::input_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, class T = std::projected_value_t<It, Pj>, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
        requires std::indirectly_writable<It, const T&>
    constexpr It replace_if(It first, Se last, Pr pred, const T& value, Pj proj = {}) {
        return plastic::replace_impl<satisfy_predicate>(first, last, pred, value, proj);
    }

    export
        template<class Sat, class It, class Se, class Out, class TPr, class U, class Pj>
    constexpr std::ranges::in_out_result<It, Out> replace_copy_impl(It first, Se last, Out output, const TPr& value_or_pred, const U& value, Pj proj) {
        while (first != last) {
            *output = plastic::satisfy<Sat>(proj(*first), value_or_pred) ? value : *first;
            ++first, ++output;
        }
        return { std::move(first), std::move(output) };
    }

    export
        template<std::input_iterator It, std::sentinel_for<It> Se, class Out, class Pj = std::identity, class T = std::projected_value_t<It, Pj>, class U = std::iter_value_t<Out>>
        requires std::indirectly_copyable<It, Out>&& std::indirect_binary_predicate<std::ranges::equal_to, std::projected<It, Pj>, const T*>&& std::output_iterator<Out, const U&>
    constexpr std::ranges::in_out_result<It, Out> replace_copy(It first, Se last, Out output, const T& old_value, const U& new_value, Pj proj = {}) {
        return plastic::replace_copy_impl<satisfy_value>(first, last, output, old_value, new_value, proj);
    }

    export
        template<std::input_iterator It, std::sentinel_for<It> Se, class Out, class T = std::iter_value_t<Out>, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
        requires std::indirectly_copyable<It, Out>&& std::output_iterator<Out, const T&>
    constexpr std::ranges::in_out_result<It, Out> replace_copy_if(It first, Se last, Out output, Pr pred, const T& value, Pj proj = {}) {
        return plastic::replace_copy_impl<satisfy_predicate>(first, last, output, pred, value, proj);
    }

    export
        template<std::input_iterator It1, std::sentinel_for<It1> Se1, std::input_iterator It2, std::sentinel_for<It2> Se2>
        requires std::indirectly_swappable<It1, It2>
    constexpr std::ranges::in_in_result<It1, It2> swap_ranges(It1 first1, Se1 last1, It2 first2, Se2 last2) {
        while (first1 != last1 && first2 != last2) {
            std::swap(*first1, *first2);
            ++first1, ++first2;
        }
        return { std::move(first1), std::move(first2) };
    }

    export
        template<std::bidirectional_iterator It, std::sentinel_for<It> Se>
        requires std::permutable<It>
    constexpr It reverse(It first, Se last) {
        It i{ std::ranges::next(first, last) }, temp{ i };
        while (first != i && first != --i) {
            std::swap(*first++, *i);
        }
        return temp;
    }

    export
        template<std::bidirectional_iterator It, std::sentinel_for<It> Se, std::weakly_incrementable Out>
        requires std::indirectly_copyable<It, Out>
    constexpr std::ranges::in_out_result<It, Out> reverse_copy(It first, Se last, Out output) {
        It r_last{ std::ranges::next(first, last) }, i{ r_last };
        while (first != i) {
            *output = *--i;
            ++output;
        }
        return { std::move(r_last), std::move(output) };
    }

    export
        template<std::permutable It, std::sentinel_for<It> Se>
    constexpr std::ranges::subrange<It> rotate(It first, It middle, Se last) {
        if (first == middle) {
            It i{ std::ranges::next(middle, last) };
            return { i, i };
        }

        if (middle == last) {
            return { std::move(first), std::move(middle) };
        }

        It i{ middle };
        do {
            std::swap(*first++, *i++);
            if (first == middle) {
                middle = i;
            }
        } while (i != last);
        It dest{ first };

        while (middle != last) {
            It i{ middle };
            do {
                std::swap(*first++, *i++);
                if (first == middle) {
                    middle = i;
                }
            } while (i != last);
        }

        return { std::move(dest), std::move(middle) };
    }

    export
        template<std::forward_iterator It, std::sentinel_for<It> Se, std::weakly_incrementable Out>
        requires std::indirectly_copyable<It, Out>
    constexpr std::ranges::in_out_result<It, Out> rotate_copy(It first, It middle, Se last, Out output) {
        auto res1{ plastic::copy(middle, last, output) }, res2{ plastic::copy(first, middle, res1.out) };
        return { std::move(res1.in), std::move(res2.out) };
    }

    export
        template<std::permutable It, std::sentinel_for<It> Se>
    constexpr std::ranges::subrange<It> shift_left(It first, Se last, std::iter_difference_t<It> count) {
        assert(count >= 0);
        return { first, plastic::move(std::ranges::next(first, count, last), last, first).out };
    }

    export
        template<std::permutable It, std::sentinel_for<It> Se>
    constexpr std::ranges::subrange<It> shift_right(It first, Se last, std::iter_difference_t<It> count) {
        assert(count >= 0);

        It dest{ std::ranges::next(first, count, last) };
        if (dest == last) {
            return { std::move(dest), std::move(dest) };
        }

        It i{ first }, j{ dest };
        while (i != dest) {
            if (j == last) {
                plastic::move(first, i, dest);
                return { std::move(dest), std::move(j) };
            }
            ++i, ++j;
        }

        It buf{ first };
        while (j != last) {
            std::swap(*buf, *i);
            ++buf, ++i, ++j;
            if (buf == dest) {
                buf = first;
            }
        }

        plastic::move(first, buf, plastic::move(buf, dest, i).out);
        return { std::move(dest), std::move(j) };
    }

    export
        template<std::input_iterator It, std::sentinel_for<It> Se, std::weakly_incrementable Out, class Gen>
        requires (std::forward_iterator<It> || std::random_access_iterator<Out>) && std::indirectly_copyable<It, Out>&& std::uniform_random_bit_generator<std::remove_reference_t<Gen>>
    Out sample(It first, Se last, Out output, std::iter_difference_t<It> count, Gen&& gen) {
        using Diff = std::iter_difference_t<It>;
        using Distr = std::uniform_int_distribution<Diff>;
        using Param = Distr::param_type;

        Distr distr;
        if constexpr (std::forward_iterator<It>) {
            Diff size{ std::ranges::distance(first, last) };
            if (count >= size) {
                return plastic::copy(first, last, output).out;
            }

            while (count != 0) {
                if (distr(gen, Param{ 0, --size }) < count) {
                    *output++ = *first;
                    --count;
                }
                ++first;
            }

            return output;
        }
        else {
            Diff size{};
            while (first != last && size != count) {
                output[size++] = *first;
                ++first;
            }

            Out temp{ output + size };
            while (first != last) {
                Diff i{ distr(gen, Param{ 0, size++ }) };
                if (i < count) {
                    output[i] = *first;
                }
                ++first;
            }

            return temp;
        }
    }

    export
        template<std::random_access_iterator It, std::sentinel_for<It> Se, class Gen>
        requires std::permutable<It>&& std::uniform_random_bit_generator<std::remove_reference_t<Gen>>
    It shuffle(It first, Se last, Gen&& gen) {
        using Diff = std::iter_difference_t<It>;
        using Distr = std::uniform_int_distribution<Diff>;
        using Param = Distr::param_type;

        Distr distr;
        Diff size{ last - first };
        while (size-- != 0) {
            std::swap(first[size], first[distr(gen, Param{ 0, size })]);
        }

        return std::ranges::next(first, last);
    }

    export
        template<std::permutable It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_equivalence_relation<std::projected<It, Pj>> Pr = std::ranges::equal_to>
    constexpr std::ranges::subrange<It> unique(It first, Se last, Pr pred = {}, Pj proj = {}) {
        first = plastic::adjacent_find(first, last, pred, proj);
        if (first == last) {
            return { first, first };
        }

        It i{ first };
        while (++i != last) {
            if (!pred(proj(*first), proj(*i))) {
                *++first = std::move(*i);
            }
        }
        ++first;

        return { std::move(first), std::move(i) };
    }

    export
        template<std::input_iterator It, std::sentinel_for<It> Se, std::weakly_incrementable Out, class Pj = std::identity, std::indirect_equivalence_relation<std::projected<It, Pj>> Pr = std::ranges::equal_to>
        requires std::indirectly_copyable<It, Out> && (std::forward_iterator<It> || std::input_iterator<Out> && std::same_as<std::iter_value_t<It>, std::iter_value_t<Out>> || std::indirectly_copyable_storable<It, Out>)
    constexpr std::ranges::in_out_result<It, Out> unique_copy(It first, Se last, Out output, Pr pred = {}, Pj proj = {}) {
        if (first == last) {
            return { std::move(first), std::move(output) };
        }

        if constexpr (std::forward_iterator<It>) {
            It i{ first };
            *output = *first;
            while (++first != last) {
                if (!pred(proj(*i), proj(*first))) {
                    i = first;
                    *++output = *first;
                }
            }
        }
        else if constexpr (std::input_iterator<Out> && std::same_as<std::iter_value_t<It>, std::iter_value_t<Out>>) {
            *output = *first;
            while (++first != last) {
                if (!pred(proj(*output), proj(*first))) {
                    *++output = *first;
                }
            }
        }
        else {
            auto value{ *first };
            *output = value;
            while (++first != last) {
                if (!pred(proj(value), proj(*first))) {
                    *++output = value = *first;
                }
            }
        }

        return { std::move(first), std::move(++output) };
    }

}

// partition operations
namespace plastic {

    export
        template<std::input_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
    constexpr bool is_partitioned(It first, Se last, Pr pred, Pj proj = {}) {
        return plastic::find_if(plastic::find_if_not(first, last, pred, proj), last, pred, proj) == last;
    }

    export
        template<std::permutable It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
    constexpr std::ranges::subrange<It> partition(It first, Se last, Pr pred, Pj proj = {}) {
        first = plastic::find_if_not(first, last, pred, proj);
        if (first == last) {
            return { first, first };
        }

        It i{ first };
        while (++i != last) {
            if (pred(proj(*i))) {
                std::swap(*first++, *i);
            }
        }

        return { std::move(first), std::move(i) };
    }

    export
        template<std::input_iterator It, std::sentinel_for<It> Se, std::weakly_incrementable Out1, std::weakly_incrementable Out2, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
        requires std::indirectly_copyable<It, Out1>&& std::indirectly_copyable<It, Out2>
    constexpr std::ranges::in_out_out_result<It, Out1, Out2> partition_copy(It first, Se last, Out1 output_true, Out2 output_false, Pr pred, Pj proj = {}) {
        while (first != last) {
            if (pred(proj(*first))) {
                *output_true = *first;
                ++output_true;
            }
            else {
                *output_false = *first;
                ++output_false;
            }
            ++first;
        }
        return { std::move(first), std::move(output_true), std::move(output_false) };
    }

    export
        template<std::bidirectional_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
        requires std::permutable<It>
    constexpr std::ranges::subrange<It> stable_partition(It first, Se last, Pr pred, Pj proj = {}) {
        first = plastic::find_if_not(first, last, pred, proj);
        if (first == last) {
            return { first, first };
        }

        It r_last{ std::ranges::next(first, last) };
        do {
            if (first == --r_last) {
                return { std::move(first), std::move(r_last) };
            }
        } while (!pred(proj(*r_last)));

        auto buf{ new std::iter_value_t<It>[std::ranges::distance(first, r_last) + 1] };
        It i{ first };
        auto j{ buf };

        *j++ = std::move(*i++);
        while (i != r_last) {
            if (pred(proj(*i))) {
                *first++ = std::move(*i);
            }
            else {
                *j++ = std::move(*i);
            }
            ++i;
        }
        *first++ = std::move(*r_last);

        It temp{ plastic::move(buf, j, first).out };
        delete[] buf;

        return { std::move(first), std::move(temp) };
    }

    export
        template<std::forward_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
    constexpr It partition_point(It first, Se last, Pr pred, Pj proj = {}) {
        auto size{ std::ranges::distance(first, last) };
        while (size != 0) {
            auto half{ size >> 1 };
            It i{ std::ranges::next(first, half) };
            if (pred(proj(*i))) {
                first = ++i;
                size -= half + 1;
            }
            else {
                size = half;
            }
        }
        return first;
    }

}

// minimum/maximum operations
namespace plastic {

    export
        template<std::forward_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_strict_weak_order<std::projected<It, Pj>> Pr = std::ranges::less>
    constexpr It max_element(It first, Se last, Pr pred = {}, Pj proj = {}) {
        if (first == last) {
            return first;
        }

        It i{ first };
        while (++first != last) {
            if (pred(proj(*i), proj(*first))) {
                i = first;
            }
        }

        return i;
    }

    export
        template<std::forward_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_strict_weak_order<std::projected<It, Pj>> Pr = std::ranges::less>
    constexpr It min_element(It first, Se last, Pr pred = {}, Pj proj = {}) {
        if (first == last) {
            return first;
        }

        It i{ first };
        while (++first != last) {
            if (pred(proj(*first), proj(*i))) {
                i = first;
            }
        }

        return i;
    }

    export
        template<std::forward_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_strict_weak_order<std::projected<It, Pj>> Pr = std::ranges::less>
    constexpr std::ranges::minmax_result<It> minmax_element(It first, Se last, Pr pred = {}, Pj proj = {}) {
        return { plastic::min_element(first, last, pred, proj), plastic::max_element(first, last, pred, proj) };
    }

    export
        template<class T, class Pj = std::identity, std::indirect_strict_weak_order<std::projected<const T*, Pj>> Pr = std::ranges::less>
    constexpr const T& max(const T& a, const T& b, Pr pred = {}, Pj proj = {}) {
        return pred(proj(a), proj(b)) ? b : a;
    }

    export
        template<std::copyable T, class Pj = std::identity, std::indirect_strict_weak_order<std::projected<const T*, Pj>> Pr = std::ranges::less>
    constexpr T max(std::initializer_list<T> list, Pr pred = {}, Pj proj = {}) {
        assert(list.size() != 0);
        return *plastic::max_element(list.begin(), list.end(), pred, proj);
    }

    export
        template<class T, class Pj = std::identity, std::indirect_strict_weak_order<std::projected<const T*, Pj>> Pr = std::ranges::less>
    constexpr const T& min(const T& a, const T& b, Pr pred = {}, Pj proj = {}) {
        return pred(proj(b), proj(a)) ? b : a;
    }

    export
        template<std::copyable T, class Pj = std::identity, std::indirect_strict_weak_order<std::projected<const T*, Pj>> Pr = std::ranges::less>
    constexpr T min(std::initializer_list<T> list, Pr pred = {}, Pj proj = {}) {
        assert(list.size() != 0);
        return *plastic::min_element(list.begin(), list.end(), pred, proj);
    }

    export
        template<class T, class Pj = std::identity, std::indirect_strict_weak_order<std::projected<const T*, Pj>> Pr = std::ranges::less>
    constexpr std::ranges::minmax_result<const T&> minmax(const T& a, const T& b, Pr pred = {}, Pj proj = {}) {
        return { plastic::min(a, b, pred, proj), plastic::max(a, b, pred, proj) };
    }

    export
        template<std::copyable T, class Pj = std::identity, std::indirect_strict_weak_order<std::projected<const T*, Pj>> Pr = std::ranges::less>
    constexpr std::ranges::minmax_result<T> minmax(std::initializer_list<T> list, Pr pred = {}, Pj proj = {}) {
        assert(list.size() != 0);
        return { plastic::min(list, pred, proj), plastic::max(list, pred, proj) };
    }

    export
        template<class T, class Pj = std::identity, std::indirect_strict_weak_order<std::projected<const T*, Pj>> Pr = std::ranges::less>
    constexpr const T& clamp(const T& value, const T& lowest, const T& highest, Pr pred = {}, Pj proj = {}) {
        assert(!pred(proj(highest), proj(lowest)));
        if (pred(proj(value), proj(lowest))) {
            return lowest;
        }
        if (pred(proj(highest), proj(value))) {
            return highest;
        }
        return value;
    }

}

// binary search operations
namespace plastic {

    export
        template<std::forward_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, class T = std::projected_value_t<It, Pj>, std::indirect_strict_weak_order<const T*, std::projected<It, Pj>> Pr = std::ranges::less>
    constexpr It lower_bound(It first, Se last, const T& value, Pr pred = {}, Pj proj = {}) {
        auto size{ std::ranges::distance(first, last) };
        while (size != 0) {
            auto half{ size >> 1 };
            It i{ std::ranges::next(first, half) };
            if (pred(proj(*i), proj(value))) {
                first = ++i;
                size -= half + 1;
            }
            else {
                size = half;
            }
        }
        return first;
    }

    export
        template<std::forward_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, class T = std::projected_value_t<It, Pj>, std::indirect_strict_weak_order<const T*, std::projected<It, Pj>> Pr = std::ranges::less>
    constexpr It upper_bound(It first, Se last, const T& value, Pr pred = {}, Pj proj = {}) {
        auto size{ std::ranges::distance(first, last) };
        while (size != 0) {
            auto half{ size >> 1 };
            It i{ std::ranges::next(first, half) };
            if (!pred(proj(value), proj(*i))) {
                first = ++i;
                size -= half + 1;
            }
            else {
                size = half;
            }
        }
        return first;
    }

    export
        template<std::forward_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, class T = std::projected_value_t<It, Pj>, std::indirect_strict_weak_order<const T*, std::projected<It, Pj>> Pr = std::ranges::less>
    constexpr bool binary_search(It first, Se last, const T& value, Pr pred = {}, Pj proj = {}) {
        first = plastic::lower_bound(first, last, value, pred, proj);
        return first != last && !pred(proj(value), proj(*first));
    }

    export
        template<std::forward_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, class T = std::projected_value_t<It, Pj>, std::indirect_strict_weak_order<const T*, std::projected<It, Pj>> Pr = std::ranges::less>
    constexpr std::ranges::subrange<It> equal_range(It first, Se last, const T& value, Pr pred = {}, Pj proj = {}) {
        return { plastic::lower_bound(first, last, value, pred, proj), plastic::upper_bound(first, last, value, pred, proj) };
    }

}

// heap operations
namespace plastic {

    export
        template<std::random_access_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_strict_weak_order<std::projected<It, Pj>> Pr = std::ranges::less>
    constexpr It is_heap_until(It first, Se last, Pr pred = {}, Pj proj = {}) {
        auto size{ last - first }, i{};
        while (i != size) {
            if (pred(proj(first[(i - 1) >> 1]), proj(first[i]))) {
                break;
            }
            ++i;
        }
        return first + i;
    }

    export
        template<std::random_access_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_strict_weak_order<std::projected<It, Pj>> Pr = std::ranges::less>
    constexpr bool is_heap(It first, Se last, Pr pred = {}, Pj proj = {}) {
        return plastic::is_heap_until(first, last, pred, proj) == last;
    }

    template<class It, class Pr, class Pj>
    constexpr void sift_up(It first, std::iter_difference_t<It> index, Pr pred, Pj proj) {
        auto value{ std::move(first[index]) };
        while (index != 0) {
            auto parent{ (index - 1) >> 1 };
            if (!pred(proj(first[parent]), proj(value))) {
                break;
            }
            first[index] = std::move(first[parent]);
            index = parent;
        }
        first[index] = std::move(value);
    }

    template<class It, class Pr, class Pj>
    constexpr void sift_down(It first, std::iter_difference_t<It> index, std::iter_difference_t<It> size, Pr pred, Pj proj) {
        auto value{ std::move(first[index]) };
        while (true) {
            auto child{ (index << 1) + 1 };
            if (child >= size) {
                break;
            }
            if (child + 1 < size && pred(proj(first[child]), proj(first[child + 1]))) {
                ++child;
            }
            if (!pred(proj(value), proj(first[child]))) {
                break;
            }
            first[index] = std::move(first[child]);
            index = child;
        }
        first[index] = std::move(value);
    }

    export
        template<std::random_access_iterator It, std::sentinel_for<It> Se, class Pr = std::ranges::less, class Pj = std::identity>
        requires std::sortable<It, Pr, Pj>
    constexpr It make_heap(It first, Se last, Pr pred = {}, Pj proj = {}) {
        It r_last{ std::ranges::next(first, last) };
        auto size{ r_last - first }, i{ size >> 1 };
        while (i-- != 0) {
            plastic::sift_down(first, i, size, pred, proj);
        }
        return r_last;
    }

    export
        template<std::random_access_iterator It, std::sentinel_for<It> Se, class Pr = std::ranges::less, class Pj = std::identity>
        requires std::sortable<It, Pr, Pj>
    constexpr It push_heap(It first, Se last, Pr pred = {}, Pj proj = {}) {
        It r_last{ std::ranges::next(first, last) };
        auto size{ r_last - first };
        if (size > 1) {
            plastic::sift_up(first, size - 1, pred, proj);
        }
        return r_last;
    }

    export
        template<std::random_access_iterator It, std::sentinel_for<It> Se, class Pr = std::ranges::less, class Pj = std::identity>
        requires std::sortable<It, Pr, Pj>
    constexpr It pop_heap(It first, Se last, Pr pred = {}, Pj proj = {}) {
        It r_last{ std::ranges::next(first, last) }, i{ r_last };
        auto size{ r_last - first };
        if (size > 1) {
            std::swap(*first, *--i);
            plastic::sift_down(first, 0, size, pred, proj);
        }
        return r_last;
    }

    export
        template<std::random_access_iterator It, std::sentinel_for<It> Se, class Pr = std::ranges::less, class Pj = std::identity>
        requires std::sortable<It, Pr, Pj>
    constexpr It sort_heap(It first, Se last, Pr pred = {}, Pj proj = {}) {
        It r_last{ std::ranges::next(first, last) }, i{ r_last };
        auto size{ r_last - first };
        while (size > 1) {
            std::swap(*first, *--i);
            plastic::sift_down(first, 0, size--, pred, proj);
        }
        return r_last;
    }

}

// merging operations
namespace plastic {

    export
        template<std::input_iterator It1, std::sentinel_for<It1> Se1, std::input_iterator It2, std::sentinel_for<It2> Se2, std::weakly_incrementable Out, class Pr = std::ranges::less, class Pj1 = std::identity, class Pj2 = std::identity>
        requires std::mergeable<It1, It2, Out, Pr, Pj1, Pj2>
    constexpr std::ranges::in_in_out_result<It1, It2, Out> merge(It1 first1, Se1 last1, It2 first2, Se2 last2, Out output, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        while (first1 != last1 && first2 != last2) {
            if (pred(proj2(*first2), proj1(*first1))) {
                *output = *first2;
                ++first2, ++output;
            }
            else {
                *output = *first1;
                ++first1, ++output;
            }
        }

        if (first1 == last1) {
            auto res{ plastic::copy(first2, last2, output) };
            return { std::move(first1), std::move(res.in), std::move(res.out) };
        }

        auto res{ plastic::copy(first1, last1, output) };
        return { std::move(res.in), std::move(first2), std::move(res.out) };
    }

    export
        template<std::bidirectional_iterator It, std::sentinel_for<It> Se, class Pr = std::ranges::less, class Pj = std::identity>
        requires std::sortable<It, Pr, Pj>
    constexpr It inplace_merge(It first, It middle, Se last, Pr pred = {}, Pj proj = {}) {
        auto buf{ new std::iter_value_t<It>[std::ranges::distance(first, last)] };

        It i{ first }, j{ middle };
        auto k{ buf };
        while (i != middle && j != last) {
            *k++ = pred(proj(*j), proj(*i)) ? std::move(*j++) : std::move(*i++);
        }

        if (i == middle) {
            plastic::move(j, last, k);
        }
        else {
            plastic::move(i, middle, k);
        }

        It temp{ plastic::move(buf, k, first).out };
        delete[] buf;

        return temp;
    }

}

// sorting operations
namespace plastic {

    export
        template<std::forward_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_strict_weak_order<std::projected<It, Pj>> Pr = std::ranges::less>
    constexpr It is_sorted_until(It first, Se last, Pr pred = {}, Pj proj = {}) {
        if (first == last) {
            return first;
        }

        It i{ first };
        while (++i != last) {
            if (!pred(proj(*i), proj(*first++))) {
                break;
            }
        }

        return i;
    }

    export
        template<std::forward_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_strict_weak_order<std::projected<It, Pj>> Pr = std::ranges::less>
    constexpr bool is_sorted(It first, Se last, Pr pred = {}, Pj proj = {}) {
        return plastic::is_sorted_until(first, last, pred, proj) == last;
    }

    template<class It, class Pr, class Pj>
    void insertion_sort(It first, It last, Pr pred, Pj proj) {
        if (first == last) {
            return;
        }

        It i{ first };
        while (++i != last) {
            auto value{ std::move(*i) };
            It hole{ i }, prev{ i };
            while (prev != first) {
                if (!pred(proj(value), proj(*--prev))) {
                    break;
                }
                *hole-- = std::move(*prev);
            }
            *hole = std::move(value);
        }
    }

    constexpr std::ptrdiff_t insertion_sort_threshold{ 32 };

    template<class It, class Pr, class Pj>
    It median_partition(It first, It last, Pr pred, Pj proj) {
        auto pivot{ plastic::max({ *first, first[last - first >> 1], *std::ranges::prev(last) }, pred, proj) };
        first = plastic::upper_bound(first, last, pivot, pred, proj);
        if (first == last) {
            return first;
        }

        It i{ first };
        while (++i != last) {
            if (pred(proj(*i), proj(pivot))) {
                std::swap(*first++, *i);
            }
        }

        return first;
    }

    template<class It, class Pr, class Pj>
    void intro_sort(It first, It last, std::iter_difference_t<It> margin, Pr pred, Pj proj) {
        if (last - first <= plastic::insertion_sort_threshold) {
            plastic::insertion_sort(first, last, pred, proj);
            return;
        }

        if (margin == 0) {
            plastic::make_heap(first, last, pred, proj);
            plastic::sort_heap(first, last, pred, proj);
            return;
        }

        margin = (margin >> 1) + (margin >> 2);
        It point{ plastic::median_partition(first, last, pred, proj) };
        plastic::intro_sort(first, point, margin, pred, proj);
        plastic::intro_sort(point, last, margin, pred, proj);
    }

    export
        template<std::random_access_iterator It, std::sentinel_for<It> Se, class Pr = std::ranges::less, class Pj = std::identity>
        requires std::sortable<It, Pr, Pj>
    constexpr It sort(It first, Se last, Pr pred = {}, Pj proj = {}) {
        It r_last{ std::ranges::next(first, last) };
        plastic::intro_sort(first, r_last, r_last - first, pred, proj);
        return r_last;
    }

    export
        template<std::random_access_iterator It, std::sentinel_for<It> Se, class Pr = std::ranges::less, class Pj = std::identity>
        requires std::sortable<It, Pr, Pj>
    constexpr It partial_sort(It first, It middle, Se last, Pr pred = {}, Pj proj = {}) {
        plastic::make_heap(first, middle, pred, proj);
        It i{ middle };
        auto size{ middle - first };
        while (i != last) {
            if (pred(proj(*i), proj(*first))) {
                std::swap(*i, *first);
                plastic::sift_down(first, 0, size, pred, proj);
            }
            ++i;
        }
        plastic::sort_heap(first, middle, pred, proj);
        return i;
    }

    export
        template<std::input_iterator It1, std::sentinel_for<It1> Se1, std::random_access_iterator It2, std::sentinel_for<It2> Se2, class Pr = std::ranges::less, class Pj1 = std::identity, class Pj2 = std::identity>
        requires std::indirectly_copyable<It1, It2>&& std::sortable<It2, Pr, Pj2>&& std::indirect_strict_weak_order<Pr, std::projected<It1, Pj1>, std::projected<It2, Pj2>>
    constexpr std::ranges::in_out_result<It1, It2> partial_sort_copy(It1 first1, Se1 last1, It2 first2, Se2 last2, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        It2 i{ first2 };
        while (first1 != last1 && i != first2) {
            *i++ = *first1;
            ++first1;
        }

        auto size{ i - first2 };
        plastic::make_heap(first2, i, pred, proj2);
        while (first1 != last1) {
            if (pred(proj1(*first1), proj2(*first2))) {
                *first2 = *first1;
                plastic::sift_down(first2, 0, size, pred, proj2);
            }
            ++first1;
        }
        plastic::sort_heap(first2, i, pred, proj2);

        return i;
    }

    export
        template<std::random_access_iterator It, std::sentinel_for<It> Se, class Pr = std::ranges::less, class Pj = std::identity>
        requires std::sortable<It, Pr, Pj>
    constexpr It stable_sort(It first, Se last, Pr pred = {}, Pj proj = {}) {
        It r_last{ std::ranges::next(first, last) };
        if (r_last - first <= plastic::insertion_sort_threshold) {
            plastic::insertion_sort(first, r_last, pred, proj);
            return r_last;
        }

        It middle{ first + (r_last - first >> 1) };
        plastic::stable_sort(first, middle, pred, proj);
        plastic::stable_sort(middle, r_last, pred, proj);
        if (pred(proj(*middle), proj(*std::ranges::prev(middle)))) {
            plastic::inplace_merge(first, middle, r_last, pred, proj);
        }

        return r_last;
    }

    export
        template<std::random_access_iterator It, std::sentinel_for<It> Se, class Pr = std::ranges::less, class Pj = std::identity>
        requires std::sortable<It, Pr, Pj>
    constexpr It nth_element(It first, It middle, Se last, Pr pred = {}, Pj proj = {}) {
        It r_last{ std::ranges::next(first, last) };
        while (r_last - first > plastic::insertion_sort_threshold) {
            It point{ plastic::median_partition(first, r_last, pred, proj) };
            if (point == middle) {
                break;
            }
            if (point < middle) {
                first = point;
            }
            else {
                r_last = point;
            }
        }
        plastic::insertion_sort(first, r_last, pred, proj);
        return r_last;
    }

}

// set operations
namespace plastic {

    export
        template<std::input_iterator It1, std::sentinel_for<It1> Se1, std::input_iterator It2, std::sentinel_for<It2> Se2, class Pj1 = std::identity, class Pj2 = std::identity, std::indirect_strict_weak_order<std::projected<It1, Pj1>, std::projected<It2, Pj2>> Pr = std::ranges::less>
    constexpr bool includes(It1 first1, Se1 last1, It2 first2, Se2 last2, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        while (first1 != last1 && first2 != last2) {
            if (pred(proj2(*first2), proj1(*first1))) {
                return false;
            }
            if (!pred(proj1(*first1), proj2(*first2))) {
                ++first2;
            }
            ++first1;
        }
        return first2 == last2;
    }

    export
        template<std::input_iterator It1, std::sentinel_for<It1> Se1, std::input_iterator It2, std::sentinel_for<It2> Se2, std::weakly_incrementable Out, class Pr = std::ranges::less, class Pj1 = std::identity, class Pj2 = std::identity>
        requires std::mergeable<It1, It2, Out, Pr, Pj1, Pj2>
    constexpr std::ranges::in_out_result<It1, Out> set_difference(It1 first1, Se1 last1, It2 first2, Se2 last2, Out output, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        while (first1 != last1 && first2 != last2) {
            if (pred(proj1(*first1), proj2(*first2))) {
                *output = *first1;
                ++first1, ++output;
            }
            else {
                if (!pred(proj2(*first2), proj1(*first1))) {
                    ++first1;
                }
                ++first2;
            }
        }
        return plastic::copy(first1, last1, output);
    }

    export
        template<std::input_iterator It1, std::sentinel_for<It1> Se1, std::input_iterator It2, std::sentinel_for<It2> Se2, std::weakly_incrementable Out, class Pr = std::ranges::less, class Pj1 = std::identity, class Pj2 = std::identity>
        requires std::mergeable<It1, It2, Out, Pr, Pj1, Pj2>
    constexpr std::ranges::in_in_out_result<It1, It2, Out> set_intersection(It1 first1, Se1 last1, It2 first2, Se2 last2, Out output, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        while (first1 != last1 && first2 != last2) {
            if (pred(proj1(*first1), proj2(*first2))) {
                ++first1;
            }
            else if (pred(proj2(*first2), proj1(*first1))) {
                ++first2;
            }
            else {
                *output = *first1;
                ++first1, ++first2, ++output;
            }
        }
        return { std::ranges::next(first1, last1), std::ranges::next(first2, last2), std::move(output) };
    }

    export
        template<std::input_iterator It1, std::sentinel_for<It1> Se1, std::input_iterator It2, std::sentinel_for<It2> Se2, std::weakly_incrementable Out, class Pr = std::ranges::less, class Pj1 = std::identity, class Pj2 = std::identity>
        requires std::mergeable<It1, It2, Out, Pr, Pj1, Pj2>
    constexpr std::ranges::in_in_out_result<It1, It2, Out> set_symmetric_difference(It1 first1, Se1 last1, It2 first2, Se2 last2, Out output, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        while (first1 != last1 && first2 != last2) {
            if (pred(proj1(*first1), proj2(*first2))) {
                *output = *first1;
                ++first1, ++output;
            }
            else if (pred(proj2(*first2), proj1(*first1))) {
                *output = *first2;
                ++first2, ++output;
            }
            else {
                ++first1, ++first2;
            }
        }
        auto res1{ plastic::copy(first1, last1, output) }, res2{ plastic::copy(first2, last2, res1.out) };
        return { std::move(res1.in), std::move(res2.in), std::move(res2.out) };
    }

    export
        template<std::input_iterator It1, std::sentinel_for<It1> Se1, std::input_iterator It2, std::sentinel_for<It2> Se2, std::weakly_incrementable Out, class Pr = std::ranges::less, class Pj1 = std::identity, class Pj2 = std::identity>
        requires std::mergeable<It1, It2, Out, Pr, Pj1, Pj2>
    constexpr std::ranges::in_in_out_result<It1, It2, Out> set_union(It1 first1, Se1 last1, It2 first2, Se2 last2, Out output, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        while (first1 != last1 && first2 != last2) {
            if (pred(proj1(*first1), proj2(*first2))) {
                *output = *first1;
                ++first1, ++output;
            }
            else if (pred(proj2(*first2), proj1(*first1))) {
                *output = *first2;
                ++first2, ++output;
            }
            else {
                *output = *first1;
                ++first1, ++first2, ++output;
            }
        }
        auto res1{ plastic::copy(first1, last1, output) }, res2{ plastic::copy(first2, last2, res1.out) };
        return { std::move(res1.in), std::move(res2.in), std::move(res2.out) };
    }

}

// permutation operations
namespace plastic {

    export
        template<std::forward_iterator It1, std::sentinel_for<It1> Se1, std::forward_iterator It2, std::sentinel_for<It2> Se2, class Pj1 = std::identity, class Pj2 = std::identity, std::indirect_equivalence_relation<std::projected<It1, Pj1>, std::projected<It2, Pj2>> Pr = std::ranges::equal_to>
    constexpr bool is_permutation(It1 first1, Se1 last1, It2 first2, Se2 last2, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        It1 i{ first1 };
        while (i != last1) {
            if (plastic::count(i, last1, *i, proj1) != plastic::count(first2, last2, *i, proj2)) {
                return false;
            }
            ++i;
        }
        return true;
    }

    export
        template<std::bidirectional_iterator It, std::sentinel_for<It> Se, class Pr = std::ranges::less, class Pj = std::identity>
        requires std::sortable<It, Pr, Pj>
    constexpr std::ranges::in_found_result<It> next_permutation(It first, Se last, Pr pred = {}, Pj proj = {}) {
        It r_last{ std::ranges::next(first, last) }, i{ r_last }, j{ r_last };
        if (first == r_last || first == --i) {
            return { std::move(first), false };
        }

        while (!pred(proj(*--i), proj(*--j))) {
            if (i == first) {
                plastic::reverse(first, r_last);
                return { std::move(r_last), false };
            }
        }

        j = r_last;
        while (!pred(proj(*i), proj(*--j))) {
        }
        std::swap(*i, *j);
        plastic::reverse(++i, r_last);

        return { std::move(r_last), true };
    }

    export
        template<std::bidirectional_iterator It, std::sentinel_for<It> Se, class Pr = std::ranges::less, class Pj = std::identity>
        requires std::sortable<It, Pr, Pj>
    constexpr std::ranges::in_found_result<It> prev_permutation(It first, Se last, Pr pred = {}, Pj proj = {}) {
        It r_last{ std::ranges::next(first, last) }, i{ r_last }, j{ r_last };
        if (first == r_last || first == --i) {
            return { std::move(first), false };
        }

        while (!pred(proj(*--j), proj(*--i))) {
            if (i == first) {
                plastic::reverse(first, r_last);
                return { std::move(r_last), false };
            }
        }

        j = r_last;
        while (!pred(proj(*--j), proj(*i))) {
        }
        std::swap(*i, *j);
        plastic::reverse(++i, r_last);

        return { std::move(r_last), true };
    }

}