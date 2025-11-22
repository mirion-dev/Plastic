module;

#include <cassert>

export module plastic:algorithm;

import std;

// introduced in C++26, but not yet implemented in MSVC
namespace std {
    template <std::indirectly_readable It, std::indirectly_regular_unary_invocable<It> Pj>
    using projected_value_t = std::remove_cvref_t<std::invoke_result_t<Pj&, std::iter_value_t<It>&>>;
}

namespace plastic {

#pragma region for internal implementation

    template <class Fn, class T, class It, class U>
    concept indirectly_binary_left_foldable_impl = std::movable<T> && std::movable<U> && std::convertible_to<T, U> && std::invocable<Fn&, U, std::iter_reference_t<It>> && std::assignable_from<U&, std::invoke_result_t<Fn&, U, std::iter_reference_t<It>>>;

    template <class Fn, class T, class It>
    concept indirectly_binary_left_foldable = std::copy_constructible<Fn> && std::indirectly_readable<It> && std::invocable<Fn&, T, std::iter_reference_t<It>> && std::convertible_to<std::invoke_result_t<Fn&, T, std::iter_reference_t<It>>, std::decay_t<std::invoke_result_t<Fn&, T, std::iter_reference_t<It>>>> && indirectly_binary_left_foldable_impl<Fn, T, It, std::decay_t<std::invoke_result_t<Fn&, T, std::iter_reference_t<It>>>>;

    template <class Fn>
    class flipped {
        Fn _;

    public:
        template <class T, class U>
            requires std::invocable<Fn&, U, T>
        std::invoke_result_t<Fn&, U, T> operator()(T&&, U&&);
    };

    template <class Fn, class T, class It>
    concept indirectly_binary_right_foldable = indirectly_binary_left_foldable<flipped<Fn>, T, It>;

    template <class Fn, class T, class It>
    using fold_left_result_t = std::decay_t<std::invoke_result_t<Fn&, T, std::iter_reference_t<It>>>;
    template <class Fn, class T, class It>
    using fold_right_result_t = std::decay_t<std::invoke_result_t<Fn&, std::iter_reference_t<It>, T>>;

    enum class satisfy_type {
        value,
        predicate,
        negated_predicate
    };

    template <satisfy_type Sat, class T, class UPr>
    bool satisfy(const T& given, const UPr& value_or_pred) {
        if constexpr (Sat == satisfy_type::value) {
            return given == value_or_pred;
        }
        if constexpr (Sat == satisfy_type::predicate) {
            return std::invoke(value_or_pred, given);
        }
        if constexpr (Sat == satisfy_type::negated_predicate) {
            return !std::invoke(value_or_pred, given);
        }
        std::unreachable();
    }

#pragma endregion

#pragma region comparison operations

    export template <std::input_iterator It1, std::sentinel_for<It1> Se1, std::input_iterator It2, std::sentinel_for<It2> Se2, class Pr = std::ranges::equal_to, class Pj1 = std::identity, class Pj2 = std::identity>
        requires std::indirectly_comparable<It1, It2, Pr, Pj1, Pj2>
    bool equal(It1 first1, Se1 last1, It2 first2, Se2 last2, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        if constexpr (std::sized_sentinel_for<Se1, It1> && std::sized_sentinel_for<Se2, It2>) {
            if (last1 - first1 != last2 - first2) {
                return false;
            }
        }

        while (first2 != last2) {
            if (first1 == last1 || !std::invoke(pred, std::invoke(proj1, *first1), std::invoke(proj2, *first2))) {
                return false;
            }
            ++first1, ++first2;
        }
        return first1 == last1;
    }

    export template <std::input_iterator It1, std::sentinel_for<It1> Se1, std::input_iterator It2, std::sentinel_for<It2> Se2, class Pj1 = std::identity, class Pj2 = std::identity, std::indirect_strict_weak_order<std::projected<It1, Pj1>, std::projected<It2, Pj2>> Pr = std::ranges::less>
    bool lexicographical_compare(It1 first1, Se1 last1, It2 first2, Se2 last2, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        while (first2 != last2) {
            if (first1 == last1 || std::invoke(pred, std::invoke(proj1, *first1), std::invoke(proj2, *first2))) {
                return true;
            }
            if (std::invoke(pred, std::invoke(proj2, *first2), std::invoke(proj1, *first1))) {
                return false;
            }
            ++first1, ++first2;
        }
        return false;
    }

#pragma endregion

#pragma region non-modifying sequence operations

    export template <std::input_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirectly_unary_invocable<std::projected<It, Pj>> Fn>
    std::ranges::in_fun_result<It, Fn> for_each(It first, Se last, Fn func, Pj proj = {}) {
        while (first != last) {
            std::invoke(func, std::invoke(proj, *first));
            ++first;
        }
        return { std::move(first), std::move(func) };
    }

    export template <std::input_iterator It, class Pj = std::identity, std::indirectly_unary_invocable<std::projected<It, Pj>> Fn>
    std::ranges::in_fun_result<It, Fn> for_each_n(It first, std::iter_difference_t<It> count, Fn func, Pj proj = {}) {
        assert(count >= 0);
        while (count-- != 0) {
            std::invoke(func, std::invoke(proj, *first));
            ++first;
        }
        return { std::move(first), std::move(func) };
    }

    template <satisfy_type Sat, std::input_iterator It, std::sentinel_for<It> Se, class TPr, class Pj>
    std::iter_difference_t<It> count_impl(It first, Se last, const TPr& value_or_pred, Pj proj) {
        std::iter_difference_t<It> count{};
        while (first != last) {
            if (plastic::satisfy<Sat>(std::invoke(proj, *first), value_or_pred)) {
                ++count;
            }
            ++first;
        }
        return count;
    }

    export template <std::input_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, class T = std::projected_value_t<It, Pj>>
        requires std::indirect_binary_predicate<std::ranges::equal_to, std::projected<It, Pj>, const T*>
    std::iter_difference_t<It> count(It first, Se last, const T& value, Pj proj = {}) {
        return plastic::count_impl<satisfy_type::value>(first, last, value, proj);
    }

    export template <std::input_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
    std::iter_difference_t<It> count_if(It first, Se last, Pr pred, Pj proj = {}) {
        return plastic::count_impl<satisfy_type::predicate>(first, last, pred, proj);
    }

    export template <std::input_iterator It1, std::sentinel_for<It1> S1, std::input_iterator It2, std::sentinel_for<It2> S2, class Pr = std::ranges::equal_to, class Pj1 = std::identity, class Pj2 = std::identity>
        requires std::indirectly_comparable<It1, It2, Pr, Pj1, Pj2>
    std::ranges::in_in_result<It1, It2> mismatch(It1 first1, S1 last1, It2 first2, S2 last2, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        while (first1 != last1 && first2 != last2) {
            if (!std::invoke(pred, std::invoke(proj1, *first1), std::invoke(proj2, *first2))) {
                break;
            }
            ++first1, ++first2;
        }
        return { std::move(first1), std::move(first2) };
    }

    template <satisfy_type Sat, std::input_iterator It, std::sentinel_for<It> Se, class TPr, class Pj>
    It find_impl(It first, Se last, const TPr& value_or_pred, Pj proj) {
        while (first != last) {
            if (plastic::satisfy<Sat>(std::invoke(proj, *first), value_or_pred)) {
                break;
            }
            ++first;
        }
        return first;
    }

    export template <std::input_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, class T = std::projected_value_t<It, Pj>>
        requires std::indirect_binary_predicate<std::ranges::equal_to, std::projected<It, Pj>, const T*>
    It find(It first, Se last, const T& value, Pj proj = {}) {
        return plastic::find_impl<satisfy_type::value>(first, last, value, proj);
    }

    export template <std::input_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
    It find_if(It first, Se last, Pr pred, Pj proj = {}) {
        return plastic::find_impl<satisfy_type::predicate>(first, last, pred, proj);
    }

    export template <std::input_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
    It find_if_not(It first, Se last, Pr pred, Pj proj = {}) {
        return plastic::find_impl<satisfy_type::negated_predicate>(first, last, pred, proj);
    }

    export template <std::input_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
    bool all_of(It first, Se last, Pr pred, Pj proj = {}) {
        return plastic::find_if_not(first, last, pred, proj) == last;
    }

    export template <std::input_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
    bool any_of(It first, Se last, Pr pred, Pj proj = {}) {
        return plastic::find_if(first, last, pred, proj) != last;
    }

    export template <std::input_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
    bool none_of(It first, Se last, Pr pred, Pj proj = {}) {
        return plastic::find_if(first, last, pred, proj) == last;
    }

    template <satisfy_type Sat, std::forward_iterator It, std::sentinel_for<It> Se, class TPr, class Pj>
    std::ranges::subrange<It> find_last_impl(It first, Se last, const TPr& value_or_pred, Pj proj) {
        It i{ plastic::find_impl<Sat>(first, last, value_or_pred, proj) };
        if (i == last) {
            return { i, i };
        }

        while (true) {
            It j{ plastic::find_impl<Sat>(std::ranges::next(i), last, value_or_pred, proj) };
            if (j == last) {
                return { std::move(i), std::move(j) };
            }
            i = j;
        }
    }

    export template <std::forward_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, class T = std::projected_value_t<It, Pj>>
        requires std::indirect_binary_predicate<std::ranges::equal_to, std::projected<It, Pj>, const T*>
    std::ranges::subrange<It> find_last(It first, Se last, const T& value, Pj proj = {}) {
        return plastic::find_last_impl<satisfy_type::value>(first, last, value, proj);
    }

    export template <std::forward_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
    std::ranges::subrange<It> find_last_if(It first, Se last, Pr pred, Pj proj = {}) {
        return plastic::find_last_impl<satisfy_type::predicate>(first, last, pred, proj);
    }

    export template <std::forward_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
    std::ranges::subrange<It> find_last_if_not(It first, Se last, Pr pred, Pj proj = {}) {
        return plastic::find_last_impl<satisfy_type::negated_predicate>(first, last, pred, proj);
    }

    export template <std::forward_iterator It1, std::sentinel_for<It1> Se1, std::forward_iterator It2, std::sentinel_for<It2> Se2, class Pr = std::ranges::equal_to, class Pj1 = std::identity, class Pj2 = std::identity>
        requires std::indirectly_comparable<It1, It2, Pr, Pj1, Pj2>
    std::ranges::subrange<It1> search(It1 first1, Se1 last1, It2 first2, Se2 last2, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
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
            } while (std::invoke(pred, std::invoke(proj1, *i++), std::invoke(proj2, *j++)));
            ++first1;
        }
    }

    export template <std::forward_iterator It, std::sentinel_for<It> Se, class Pr = std::ranges::equal_to, class Pj = std::identity, class T = std::projected_value_t<It, Pj>>
        requires std::indirectly_comparable<It, const T*, Pr, Pj>
    std::ranges::subrange<It> search_n(It first, Se last, std::iter_difference_t<It> count, const T& value, Pr pred = {}, Pj proj = {}) {
        if (count <= 0) {
            return { first, first };
        }

        while (first != last) {
            if (!std::invoke(pred, std::invoke(proj, *first), value)) {
                ++first;
                continue;
            }

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
            } while (std::invoke(pred, std::invoke(proj, *i++), value));
            first = i;
        }
        return { first, first };
    }

    export template <std::forward_iterator It1, std::sentinel_for<It1> Se1, std::forward_iterator It2, std::sentinel_for<It2> Se2, class Pr = std::ranges::equal_to, class Pj1 = std::identity, class Pj2 = std::identity>
        requires std::indirectly_comparable<It1, It2, Pr, Pj1, Pj2>
    std::ranges::subrange<It1> find_end(It1 first1, Se1 last1, It2 first2, Se2 last2, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        if (first2 == last2) {
            It1 r_last{ std::ranges::next(first1, last1) };
            return { r_last, r_last };
        }

        auto r{ plastic::search(first1, last1, first2, last2, pred, proj1, proj2) };
        if (r.empty()) {
            return r;
        }

        while (true) {
            auto s{ plastic::search(++r.begin(), last1, first2, last2, pred, proj1, proj2) };
            if (s.empty()) {
                return r;
            }
            r = s;
        }
    }

    export template <std::input_iterator It1, std::sentinel_for<It1> Se1, std::forward_iterator It2, std::sentinel_for<It2> Se2, class Pr = std::ranges::equal_to, class Pj1 = std::identity, class Pj2 = std::identity>
        requires std::indirectly_comparable<It1, It2, Pr, Pj1, Pj2>
    It1 find_first_of(It1 first1, Se1 last1, It2 first2, Se2 last2, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        while (first1 != last1) {
            It2 i{ first2 };
            while (i != last2) {
                if (std::invoke(pred, std::invoke(proj1, *first1), std::invoke(proj2, *i++))) {
                    return first1;
                }
            }
            ++first1;
        }
        return first1;
    }

    export template <std::forward_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_binary_predicate<std::projected<It, Pj>, std::projected<It, Pj>> Pr = std::ranges::equal_to>
    It adjacent_find(It first, Se last, Pr pred = {}, Pj proj = {}) {
        if (first == last) {
            return first;
        }

        It i{ first };
        while (++i != last) {
            if (std::invoke(pred, std::invoke(proj, *first), std::invoke(proj, *i))) {
                return first;
            }
            ++first;
        }
        return i;
    }

    export template <std::input_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, class T = std::projected_value_t<It, Pj>>
        requires std::indirect_binary_predicate<std::ranges::equal_to, std::projected<It, Pj>, const T*>
    bool contains(It first, Se last, const T& value, Pj proj = {}) {
        return plastic::find(first, last, value, proj) != last;
    }

    export template <std::forward_iterator It1, std::sentinel_for<It1> Se1, std::forward_iterator It2, std::sentinel_for<It2> Se2, class Pr = std::ranges::equal_to, class Pj1 = std::identity, class Pj2 = std::identity>
        requires std::indirectly_comparable<It1, It2, Pr, Pj1, Pj2>
    bool contains_subrange(It1 first1, Se1 last1, It2 first2, Se2 last2, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        return first2 == last2 || !plastic::search(first1, last1, first2, last2, pred, proj1, proj2).empty();
    }

    export template <std::input_iterator It1, std::sentinel_for<It1> Se1, std::input_iterator It2, std::sentinel_for<It2> Se2, class Pr = std::ranges::equal_to, class Pj1 = std::identity, class Pj2 = std::identity>
        requires std::indirectly_comparable<It1, It2, Pr, Pj1, Pj2>
    bool starts_with(It1 first1, Se1 last1, It2 first2, Se2 last2, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        return plastic::mismatch(first1, last1, first2, last2, pred, proj1, proj2).in2 == last2;
    }

    export template <std::input_iterator It1, std::sentinel_for<It1> Se1, std::input_iterator It2, std::sentinel_for<It2> Se2, class Pr = std::ranges::equal_to, class Pj1 = std::identity, class Pj2 = std::identity>
        requires (std::forward_iterator<It1> || std::sized_sentinel_for<Se1, It1>) && (std::forward_iterator<It2> || std::sized_sentinel_for<Se2, It2>) && std::indirectly_comparable<It1, It2, Pr, Pj1, Pj2>
    bool ends_with(It1 first1, Se1 last1, It2 first2, Se2 last2, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
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
            std::iter_difference_t<It1> size1{ last1 - first1 }, size2{};
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
            std::iter_difference_t<It1> size1{ last1 - first1 }, size2{ last2 - first2 };
            if (size1 < size2) {
                return false;
            }

            first1 = std::ranges::next(first1, size1 - size2, last1);
        }

        return plastic::equal(first1, last1, first2, last2, pred, proj1, proj2);
    }

#pragma endregion

#pragma region fold operations

    export template <std::input_iterator It, std::sentinel_for<It> Se, class T = std::iter_value_t<It>, indirectly_binary_left_foldable<T, It> Fn>
    std::ranges::in_value_result<It, fold_left_result_t<Fn, T, It>> fold_left_with_iter(It first, Se last, T init, Fn func) {
        using U = fold_left_result_t<Fn, T, It>;

        if (first == last) {
            return { std::move(first), static_cast<U>(std::move(init)) };
        }

        U value{ std::invoke(func, std::move(init), *first) };
        while (++first != last) {
            value = std::invoke(func, std::move(value), *first);
        }
        return { std::move(first), std::move(value) };
    }

    export template <std::input_iterator It, std::sentinel_for<It> Se, indirectly_binary_left_foldable<std::iter_value_t<It>, It> Fn>
        requires std::constructible_from<std::iter_value_t<It>, std::iter_reference_t<It>>
    std::ranges::in_value_result<It, std::optional<fold_left_result_t<Fn, std::iter_value_t<It>, It>>> fold_left_first_with_iter(It first, Se last, Fn func) {
        using U = fold_left_result_t<Fn, std::iter_value_t<It>, It>;

        if (first == last) {
            return { std::move(first), {} };
        }

        std::optional<U> opt{ std::in_place, *first };
        U& value{ *opt };
        while (++first != last) {
            value = std::invoke(func, std::move(value), *first);
        }
        return { std::move(first), std::move(opt) };
    }

    export template <std::input_iterator It, std::sentinel_for<It> Se, class T = std::iter_value_t<It>, indirectly_binary_left_foldable<T, It> Fn>
    fold_left_result_t<Fn, T, It> fold_left(It first, Se last, T init, Fn func) {
        return plastic::fold_left_with_iter(first, last, std::move(init), func).value;
    }

    export template <std::input_iterator It, std::sentinel_for<It> Se, indirectly_binary_left_foldable<std::iter_value_t<It>, It> Fn>
        requires std::constructible_from<std::iter_value_t<It>, std::iter_reference_t<It>>
    std::optional<fold_left_result_t<Fn, std::iter_value_t<It>, It>> fold_left_first(It first, Se last, Fn func) {
        return plastic::fold_left_first_with_iter(first, last, func).value;
    }

    export template <std::bidirectional_iterator It, std::sentinel_for<It> Se, class T = std::iter_value_t<It>, indirectly_binary_right_foldable<T, It> Fn>
    fold_right_result_t<Fn, T, It> fold_right(It first, Se last, T init, Fn func) {
        using U = fold_right_result_t<Fn, T, It>;

        if (first == last) {
            return static_cast<U>(std::move(init));
        }

        It i{ std::ranges::next(first, last) };
        U value{ std::invoke(func, *--i, std::move(init)) };
        while (first != i) {
            value = std::invoke(func, *--i, std::move(value));
        }
        return value;
    }

    export template <std::bidirectional_iterator It, std::sentinel_for<It> Se, indirectly_binary_right_foldable<std::iter_value_t<It>, It> Fn>
        requires std::constructible_from<std::iter_value_t<It>, std::iter_reference_t<It>>
    std::optional<fold_right_result_t<Fn, std::iter_value_t<It>, It>> fold_right_last(It first, Se last, Fn func) {
        using U = fold_right_result_t<Fn, std::iter_value_t<It>, It>;

        if (first == last) {
            return {};
        }

        It i{ std::ranges::next(first, last) };
        std::optional<U> opt{ std::in_place, *--i };
        U& value{ *opt };
        while (first != i) {
            value = std::invoke(func, *--i, std::move(value));
        }
        return opt;
    }

#pragma endregion

#pragma region modifying sequence operations

    export template <std::input_iterator It, std::sentinel_for<It> Se, std::weakly_incrementable Out>
        requires std::indirectly_copyable<It, Out>
    std::ranges::in_out_result<It, Out> copy(It first, Se last, Out output) {
        while (first != last) {
            *output++ = *first;
            ++first;
        }
        return { std::move(first), std::move(output) };
    }

    export template <std::input_iterator It, std::sentinel_for<It> Se, std::weakly_incrementable Out, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
        requires std::indirectly_copyable<It, Out>
    std::ranges::in_out_result<It, Out> copy_if(It first, Se last, Out output, Pr pred, Pj proj = {}) {
        while (first != last) {
            if (std::invoke(pred, std::invoke(proj, *first))) {
                *output++ = *first;
            }
            ++first;
        }
        return { std::move(first), std::move(output) };
    }

    export template <std::input_iterator It, std::weakly_incrementable Out>
        requires std::indirectly_copyable<It, Out>
    std::ranges::in_out_result<It, Out> copy_n(It first, std::iter_difference_t<It> count, Out output) {
        if (count <= 0) {
            return { std::move(first), std::move(output) };
        }

        while (count-- != 0) {
            *output++ = *first;
            ++first;
        }
        return { std::move(first), std::move(output) };
    }

    export template <std::bidirectional_iterator It1, std::sentinel_for<It1> Se1, std::bidirectional_iterator It2>
        requires std::indirectly_copyable<It1, It2>
    std::ranges::in_out_result<It1, It2> copy_backward(It1 first, Se1 last, It2 output) {
        It1 r_last{ std::ranges::next(first, last) }, i{ r_last };
        while (first != i) {
            *--output = *--i;
        }
        return { std::move(r_last), std::move(output) };
    }

    export template <std::input_iterator It, std::sentinel_for<It> Se, std::weakly_incrementable Out>
        requires std::indirectly_movable<It, Out>
    std::ranges::in_out_result<It, Out> move(It first, Se last, Out output) {
        while (first != last) {
            *output++ = std::move(*first);
            ++first;
        }
        return { std::move(first), std::move(output) };
    }

    export template <std::bidirectional_iterator It1, std::sentinel_for<It1> Se1, std::bidirectional_iterator It2>
        requires std::indirectly_movable<It1, It2>
    std::ranges::in_out_result<It1, It2> move_backward(It1 first, Se1 last, It2 output) {
        It1 r_last{ std::ranges::next(first, last) }, i{ r_last };
        while (first != i) {
            *--output = std::move(*--i);
        }
        return { std::move(r_last), std::move(output) };
    }

    export template <class Out, std::sentinel_for<Out> Se, class T = std::iter_value_t<Out>>
        requires std::output_iterator<Out, const T&>
    Out fill(Out first, Se last, const T& value) {
        while (first != last) {
            *first++ = value;
        }
        return first;
    }

    export template <class Out, class T = std::iter_value_t<Out>>
        requires std::output_iterator<Out, const T&>
    Out fill_n(Out first, std::iter_difference_t<Out> count, const T& value) {
        if (count <= 0) {
            return first;
        }

        while (count-- != 0) {
            *first++ = value;
        }
        return first;
    }

    export template <std::input_iterator It, std::sentinel_for<It> Se, std::weakly_incrementable Out, std::copy_constructible Fn, class Pj = std::identity>
        requires std::indirectly_writable<Out, std::indirect_result_t<Fn&, std::projected<It, Pj>>>
    std::ranges::in_out_result<It, Out> transform(It first, Se last, Out output, Fn func, Pj proj = {}) {
        while (first != last) {
            *output++ = std::invoke(func, std::invoke(proj, *first));
            ++first;
        }
        return { std::move(first), std::move(output) };
    }

    export template <std::input_iterator It1, std::sentinel_for<It1> Se1, std::input_iterator It2, std::sentinel_for<It2> Se2, std::weakly_incrementable Out, std::copy_constructible Fn, class Pj1 = std::identity, class Pj2 = std::identity>
        requires std::indirectly_writable<Out, std::indirect_result_t<Fn&, std::projected<It1, Pj1>, std::projected<It2, Pj2>>>
    std::ranges::in_in_out_result<It1, It2, Out> transform(It1 first1, Se1 last1, It2 first2, Se2 last2, Out output, Fn func, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        while (first1 != last1 && first2 != last2) {
            *output++ = std::invoke(func, std::invoke(proj1, *first1), std::invoke(proj2, *first2));
            ++first1, ++first2;
        }
        return { std::move(first1), std::move(first2), std::move(output) };
    }

    export template <std::input_or_output_iterator Out, std::sentinel_for<Out> Se, std::copy_constructible Fn>
        requires std::invocable<Fn&> && std::indirectly_writable<Out, std::invoke_result_t<Fn&>>
    Out generate(Out first, Se last, Fn func) {
        while (first != last) {
            *first++ = std::invoke(func);
        }
        return first;
    }

    export template <std::input_or_output_iterator Out, std::copy_constructible Fn>
        requires std::invocable<Fn&> && std::indirectly_writable<Out, std::invoke_result_t<Fn&>>
    Out generate_n(Out first, std::iter_difference_t<Out> count, Fn func) {
        if (count <= 0) {
            return first;
        }

        while (count-- != 0) {
            *first++ = std::invoke(func);
        }
        return first;
    }

    template <satisfy_type Sat, std::forward_iterator It, std::sentinel_for<It> Se, class TPr, class Pj>
    std::ranges::subrange<It> remove_impl(It first, Se last, const TPr& value_or_pred, Pj proj) {
        first = plastic::find_impl<Sat>(first, last, value_or_pred, proj);
        if (first == last) {
            return { first, first };
        }

        It i{ first };
        while (++i != last) {
            if (!plastic::satisfy<Sat>(std::invoke(proj, *i), value_or_pred)) {
                *first++ = std::move(*i);
            }
        }
        return { std::move(first), std::move(i) };
    }

    export template <std::permutable It, std::sentinel_for<It> Se, class Pj = std::identity, class T = std::projected_value_t<It, Pj>>
        requires std::indirect_binary_predicate<std::ranges::equal_to, std::projected<It, Pj>, const T*>
    std::ranges::subrange<It> remove(It first, Se last, const T& value, Pj proj = {}) {
        return plastic::remove_impl<satisfy_type::value>(first, last, value, proj);
    }

    export template <std::permutable It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
    std::ranges::subrange<It> remove_if(It first, Se last, Pr pred, Pj proj = {}) {
        return plastic::remove_impl<satisfy_type::predicate>(first, last, pred, proj);
    }

    template <satisfy_type Sat, std::input_iterator It, std::sentinel_for<It> Se, class Out, class TPr, class Pj>
    std::ranges::in_out_result<It, Out> remove_copy_impl(It first, Se last, Out output, const TPr& value_or_pred, Pj proj) {
        while (first != last) {
            if (!plastic::satisfy<Sat>(std::invoke(proj, *first), value_or_pred)) {
                *output++ = *first;
            }
            ++first;
        }
        return { std::move(first), std::move(output) };
    }

    export template <std::input_iterator It, std::sentinel_for<It> Se, std::weakly_incrementable Out, class Pj = std::identity, class T = std::projected_value_t<It, Pj>>
        requires std::indirectly_copyable<It, Out> && std::indirect_binary_predicate<std::ranges::equal_to, std::projected<It, Pj>, const T*>
    std::ranges::in_out_result<It, Out> remove_copy(It first, Se last, Out output, const T& value, Pj proj = {}) {
        return plastic::remove_copy_impl<satisfy_type::value>(first, last, output, value, proj);
    }

    export template <std::input_iterator It, std::sentinel_for<It> Se, std::weakly_incrementable Out, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
        requires std::indirectly_copyable<It, Out>
    std::ranges::in_out_result<It, Out> remove_copy_if(It first, Se last, Out output, Pr pred, Pj proj = {}) {
        return plastic::remove_copy_impl<satisfy_type::predicate>(first, last, output, pred, proj);
    }

    template <satisfy_type Sat, std::input_iterator It, std::sentinel_for<It> Se, class TPr, class U, class Pj>
    It replace_impl(It first, Se last, const TPr& value_or_pred, const U& value, Pj proj) {
        while (first != last) {
            if (plastic::satisfy<Sat>(std::invoke(proj, *first), value_or_pred)) {
                *first = value;
            }
            ++first;
        }
        return first;
    }

    export template <std::input_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, class T = std::projected_value_t<It, Pj>, class U = T>
        requires std::indirectly_writable<It, const U&> && std::indirect_binary_predicate<std::ranges::equal_to, std::projected<It, Pj>, const T*>
    It replace(It first, Se last, const T& old_value, const U& new_value, Pj proj = {}) {
        return plastic::replace_impl<satisfy_type::value>(first, last, old_value, new_value, proj);
    }

    export template <std::input_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, class T = std::projected_value_t<It, Pj>, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
        requires std::indirectly_writable<It, const T&>
    It replace_if(It first, Se last, Pr pred, const T& value, Pj proj = {}) {
        return plastic::replace_impl<satisfy_type::predicate>(first, last, pred, value, proj);
    }

    template <satisfy_type Sat, std::input_iterator It, std::sentinel_for<It> Se, class Out, class TPr, class U, class Pj>
    std::ranges::in_out_result<It, Out> replace_copy_impl(It first, Se last, Out output, const TPr& value_or_pred, const U& value, Pj proj) {
        while (first != last) {
            if (plastic::satisfy<Sat>(std::invoke(proj, *first), value_or_pred)) {
                *output++ = value;
            }
            else {
                *output++ = *first;
            }
            ++first;
        }
        return { std::move(first), std::move(output) };
    }

    export template <std::input_iterator It, std::sentinel_for<It> Se, class Out, class Pj = std::identity, class T = std::projected_value_t<It, Pj>, class U = std::iter_value_t<Out>>
        requires std::indirectly_copyable<It, Out> && std::indirect_binary_predicate<std::ranges::equal_to, std::projected<It, Pj>, const T*> && std::output_iterator<Out, const U&>
    std::ranges::in_out_result<It, Out> replace_copy(It first, Se last, Out output, const T& old_value, const U& new_value, Pj proj = {}) {
        return plastic::replace_copy_impl<satisfy_type::value>(first, last, output, old_value, new_value, proj);
    }

    export template <std::input_iterator It, std::sentinel_for<It> Se, class Out, class T = std::iter_value_t<Out>, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
        requires std::indirectly_copyable<It, Out> && std::output_iterator<Out, const T&>
    std::ranges::in_out_result<It, Out> replace_copy_if(It first, Se last, Out output, Pr pred, const T& value, Pj proj = {}) {
        return plastic::replace_copy_impl<satisfy_type::predicate>(first, last, output, pred, value, proj);
    }

    export template <std::input_iterator It1, std::sentinel_for<It1> Se1, std::input_iterator It2, std::sentinel_for<It2> Se2>
        requires std::indirectly_swappable<It1, It2>
    std::ranges::in_in_result<It1, It2> swap_ranges(It1 first1, Se1 last1, It2 first2, Se2 last2) {
        while (first1 != last1 && first2 != last2) {
            std::swap(*first1, *first2);
            ++first1, ++first2;
        }
        return { std::move(first1), std::move(first2) };
    }

    export template <std::bidirectional_iterator It, std::sentinel_for<It> Se>
        requires std::permutable<It>
    It reverse(It first, Se last) {
        It r_last{ std::ranges::next(first, last) }, i{ r_last };
        while (first != i && first != --i) {
            std::swap(*first++, *i);
        }
        return r_last;
    }

    export template <std::bidirectional_iterator It, std::sentinel_for<It> Se, std::weakly_incrementable Out>
        requires std::indirectly_copyable<It, Out>
    std::ranges::in_out_result<It, Out> reverse_copy(It first, Se last, Out output) {
        It r_last{ std::ranges::next(first, last) }, i{ r_last };
        while (first != i) {
            *output++ = *--i;
        }
        return { std::move(r_last), std::move(output) };
    }

    export template <std::permutable It, std::sentinel_for<It> Se>
    std::ranges::subrange<It> rotate(It first, It middle, Se last) {
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

    export template <std::forward_iterator It, std::sentinel_for<It> Se, std::weakly_incrementable Out>
        requires std::indirectly_copyable<It, Out>
    std::ranges::in_out_result<It, Out> rotate_copy(It first, It middle, Se last, Out output) {
        auto res1{ plastic::copy(middle, last, output) }, res2{ plastic::copy(first, middle, res1.out) };
        return { std::move(res1.in), std::move(res2.out) };
    }

    export template <std::permutable It, std::sentinel_for<It> Se>
    std::ranges::subrange<It> shift_left(It first, Se last, std::iter_difference_t<It> count) {
        assert(count >= 0);
        It dest{ std::ranges::next(first, count, last) };
        if (dest == last) {
            return { dest, dest };
        }
        return { std::move(first), std::move(plastic::move(dest, last, first).out) };
    }

    export template <std::permutable It, std::sentinel_for<It> Se>
    std::ranges::subrange<It> shift_right(It first, Se last, std::iter_difference_t<It> count) {
        assert(count >= 0);

        It dest{ std::ranges::next(first, count, last) };
        if (dest == last) {
            return { dest, dest };
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
            std::swap(*buf++, *i++);
            ++j;
            if (buf == dest) {
                buf = first;
            }
        }

        plastic::move(first, buf, plastic::move(buf, dest, i).out);
        return { std::move(dest), std::move(j) };
    }

    export template <std::input_iterator It, std::sentinel_for<It> Se, std::weakly_incrementable Out, class Gen>
        requires (std::forward_iterator<It> || std::random_access_iterator<Out>) && std::indirectly_copyable<It, Out> && std::uniform_random_bit_generator<std::remove_reference_t<Gen>>
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

    export template <std::random_access_iterator It, std::sentinel_for<It> Se, class Gen>
        requires std::permutable<It> && std::uniform_random_bit_generator<std::remove_reference_t<Gen>>
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

    export template <std::permutable It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_equivalence_relation<std::projected<It, Pj>> Pr = std::ranges::equal_to>
    std::ranges::subrange<It> unique(It first, Se last, Pr pred = {}, Pj proj = {}) {
        first = plastic::adjacent_find(first, last, pred, proj);
        if (first == last) {
            return { first, first };
        }

        It i{ first };
        while (++i != last) {
            if (!std::invoke(pred, std::invoke(proj, *first), std::invoke(proj, *i))) {
                *++first = std::move(*i);
            }
        }
        return { ++first, std::move(i) };
    }

    export template <std::input_iterator It, std::sentinel_for<It> Se, std::weakly_incrementable Out, class Pj = std::identity, std::indirect_equivalence_relation<std::projected<It, Pj>> Pr = std::ranges::equal_to>
        requires std::indirectly_copyable<It, Out> && (std::forward_iterator<It> || std::input_iterator<Out> && std::same_as<std::iter_value_t<It>, std::iter_value_t<Out>> || std::indirectly_copyable_storable<It, Out>)
    std::ranges::in_out_result<It, Out> unique_copy(It first, Se last, Out output, Pr pred = {}, Pj proj = {}) {
        if (first == last) {
            return { std::move(first), std::move(output) };
        }

        if constexpr (std::forward_iterator<It>) {
            It i{ first };
            *output = *first;
            while (++first != last) {
                if (!std::invoke(pred, std::invoke(proj, *i), std::invoke(proj, *first))) {
                    i = first;
                    *++output = *first;
                }
            }
        }
        else if constexpr (std::input_iterator<Out> && std::same_as<std::iter_value_t<It>, std::iter_value_t<Out>>) {
            *output = *first;
            while (++first != last) {
                if (!std::invoke(pred, std::invoke(proj, *output), std::invoke(proj, *first))) {
                    *++output = *first;
                }
            }
        }
        else {
            auto value{ *first };
            *output = value;
            while (++first != last) {
                if (!std::invoke(pred, std::invoke(proj, value), std::invoke(proj, *first))) {
                    *++output = value = *first;
                }
            }
        }

        return { std::move(first), ++output };
    }

#pragma endregion

#pragma region partitioning operations

    export template <std::input_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
    bool is_partitioned(It first, Se last, Pr pred, Pj proj = {}) {
        return plastic::find_if(plastic::find_if_not(first, last, pred, proj), last, pred, proj) == last;
    }

    export template <std::permutable It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
    std::ranges::subrange<It> partition(It first, Se last, Pr pred, Pj proj = {}) {
        first = plastic::find_if_not(first, last, pred, proj);
        if (first == last) {
            return { first, first };
        }

        if constexpr (std::bidirectional_iterator<It>) {
            It r_last{ std::ranges::next(first, last) }, i{ r_last };
            while (true) {
                first = plastic::find_if_not(first, i, pred, proj);
                do {
                    if (first == i) {
                        return { std::move(first), std::move(r_last) };
                    }
                } while (!std::invoke(pred, std::invoke(proj, *--i)));
                std::swap(*first++, *i);
            }
        }
        else {
            It i{ first };
            while (++i != last) {
                if (std::invoke(pred, std::invoke(proj, *i))) {
                    std::swap(*first++, *i);
                }
            }
            return { std::move(first), std::move(i) };
        }
    }

    export template <std::input_iterator It, std::sentinel_for<It> Se, std::weakly_incrementable Out1, std::weakly_incrementable Out2, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
        requires std::indirectly_copyable<It, Out1> && std::indirectly_copyable<It, Out2>
    std::ranges::in_out_out_result<It, Out1, Out2> partition_copy(It first, Se last, Out1 output_true, Out2 output_false, Pr pred, Pj proj = {}) {
        while (first != last) {
            if (std::invoke(pred, std::invoke(proj, *first))) {
                *output_true++ = *first;
            }
            else {
                *output_false++ = *first;
            }
            ++first;
        }
        return { std::move(first), std::move(output_true), std::move(output_false) };
    }

    export template <std::bidirectional_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
        requires std::permutable<It>
    std::ranges::subrange<It> stable_partition(It first, Se last, Pr pred, Pj proj = {}) {
        first = plastic::find_if_not(first, last, pred, proj);
        if (first == last) {
            return { first, first };
        }

        It r_last{ std::ranges::next(first, last) };
        do {
            if (first == --r_last) {
                return { std::move(first), std::move(r_last) };
            }
        } while (!std::invoke(pred, std::invoke(proj, *r_last)));

        std::allocator<std::iter_value_t<It>> alloc;
        auto size{ static_cast<std::size_t>(std::ranges::distance(first, r_last)) + 1 };
        auto buf{ alloc.allocate(size) };

        It i{ first };
        auto j{ buf };
        std::construct_at(j++, std::move(*i++));
        while (i != r_last) {
            if (std::invoke(pred, std::invoke(proj, *i))) {
                *first++ = std::move(*i);
            }
            else {
                std::construct_at(j++, std::move(*i));
            }
            ++i;
        }
        *first++ = std::move(*r_last);

        It temp{ plastic::move(buf, j, first).out };
        std::destroy(buf, j);
        alloc.deallocate(buf, size);
        return { std::move(first), std::move(temp) };
    }

    export template <std::forward_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_unary_predicate<std::projected<It, Pj>> Pr>
    It partition_point(It first, Se last, Pr pred, Pj proj = {}) {
        auto size{ std::ranges::distance(first, last) };
        while (size != 0) {
            auto half{ size >> 1 };
            It i{ std::ranges::next(first, half) };
            if (std::invoke(pred, std::invoke(proj, *i))) {
                first = ++i;
                size -= half + 1;
            }
            else {
                size = half;
            }
        }
        return first;
    }

#pragma endregion

#pragma region binary search operations

    export template <std::forward_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, class T = std::projected_value_t<It, Pj>, std::indirect_strict_weak_order<const T*, std::projected<It, Pj>> Pr = std::ranges::less>
    It lower_bound(It first, Se last, const T& value, Pr pred = {}, Pj proj = {}) {
        auto size{ std::ranges::distance(first, last) };
        while (size != 0) {
            auto half{ size >> 1 };
            It i{ std::ranges::next(first, half) };
            if (std::invoke(pred, std::invoke(proj, *i), value)) {
                first = ++i;
                size -= half + 1;
            }
            else {
                size = half;
            }
        }
        return first;
    }

    export template <std::forward_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, class T = std::projected_value_t<It, Pj>, std::indirect_strict_weak_order<const T*, std::projected<It, Pj>> Pr = std::ranges::less>
    It upper_bound(It first, Se last, const T& value, Pr pred = {}, Pj proj = {}) {
        auto size{ std::ranges::distance(first, last) };
        while (size != 0) {
            auto half{ size >> 1 };
            It i{ std::ranges::next(first, half) };
            if (!std::invoke(pred, value, std::invoke(proj, *i))) {
                first = ++i;
                size -= half + 1;
            }
            else {
                size = half;
            }
        }
        return first;
    }

    export template <std::forward_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, class T = std::projected_value_t<It, Pj>, std::indirect_strict_weak_order<const T*, std::projected<It, Pj>> Pr = std::ranges::less>
    bool binary_search(It first, Se last, const T& value, Pr pred = {}, Pj proj = {}) {
        first = plastic::lower_bound(first, last, value, pred, proj);
        return first != last && !std::invoke(pred, value, std::invoke(proj, *first));
    }

    export template <std::forward_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, class T = std::projected_value_t<It, Pj>, std::indirect_strict_weak_order<const T*, std::projected<It, Pj>> Pr = std::ranges::less>
    std::ranges::subrange<It> equal_range(It first, Se last, const T& value, Pr pred = {}, Pj proj = {}) {
        return { plastic::lower_bound(first, last, value, pred, proj), plastic::upper_bound(first, last, value, pred, proj) };
    }

#pragma endregion

#pragma region heap operations

    export template <std::random_access_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_strict_weak_order<std::projected<It, Pj>> Pr = std::ranges::less>
    It is_heap_until(It first, Se last, Pr pred = {}, Pj proj = {}) {
        if (first == last) {
            return first;
        }

        std::iter_difference_t<It> size{ last - first }, i{ 1 };
        while (i != size) {
            if (std::invoke(pred, std::invoke(proj, first[(i - 1) >> 1]), std::invoke(proj, first[i]))) {
                break;
            }
            ++i;
        }
        return first + i;
    }

    export template <std::random_access_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_strict_weak_order<std::projected<It, Pj>> Pr = std::ranges::less>
    bool is_heap(It first, Se last, Pr pred = {}, Pj proj = {}) {
        return plastic::is_heap_until(first, last, pred, proj) == last;
    }

    template <std::random_access_iterator It, class Pr, class Pj>
    void sift_up(It first, std::iter_difference_t<It> index, Pr pred, Pj proj) {
        auto value{ std::move(first[index]) };
        while (index != 0) {
            auto parent{ index - 1 >> 1 };
            if (!std::invoke(pred, std::invoke(proj, first[parent]), std::invoke(proj, value))) {
                break;
            }
            first[index] = std::move(first[parent]);
            index = parent;
        }
        first[index] = std::move(value);
    }

    template <std::random_access_iterator It, class Pr, class Pj>
    void sift_down(It first, std::iter_difference_t<It> index, std::iter_difference_t<It> size, Pr pred, Pj proj) {
        auto value{ std::move(first[index]) };
        while (true) {
            auto child{ (index << 1) + 1 };
            if (child >= size) {
                break;
            }
            if (child + 1 < size && std::invoke(pred, std::invoke(proj, first[child]), std::invoke(proj, first[child + 1]))) {
                ++child;
            }
            if (!std::invoke(pred, std::invoke(proj, value), std::invoke(proj, first[child]))) {
                break;
            }
            first[index] = std::move(first[child]);
            index = child;
        }
        first[index] = std::move(value);
    }

    export template <std::random_access_iterator It, std::sentinel_for<It> Se, class Pr = std::ranges::less, class Pj = std::identity>
        requires std::sortable<It, Pr, Pj>
    It make_heap(It first, Se last, Pr pred = {}, Pj proj = {}) {
        It r_last{ std::ranges::next(first, last) };
        auto size{ r_last - first }, i{ size >> 1 };
        while (i-- != 0) {
            plastic::sift_down(first, i, size, pred, proj);
        }
        return r_last;
    }

    export template <std::random_access_iterator It, std::sentinel_for<It> Se, class Pr = std::ranges::less, class Pj = std::identity>
        requires std::sortable<It, Pr, Pj>
    It push_heap(It first, Se last, Pr pred = {}, Pj proj = {}) {
        It r_last{ std::ranges::next(first, last) };
        auto size{ r_last - first };
        if (size > 1) {
            plastic::sift_up(first, size - 1, pred, proj);
        }
        return r_last;
    }

    export template <std::random_access_iterator It, std::sentinel_for<It> Se, class Pr = std::ranges::less, class Pj = std::identity>
        requires std::sortable<It, Pr, Pj>
    It pop_heap(It first, Se last, Pr pred = {}, Pj proj = {}) {
        It r_last{ std::ranges::next(first, last) }, i{ r_last };
        auto size{ r_last - first };
        if (size > 1) {
            std::swap(*first, *--i);
            plastic::sift_down(first, 0, size - 1, pred, proj);
        }
        return r_last;
    }

    export template <std::random_access_iterator It, std::sentinel_for<It> Se, class Pr = std::ranges::less, class Pj = std::identity>
        requires std::sortable<It, Pr, Pj>
    It sort_heap(It first, Se last, Pr pred = {}, Pj proj = {}) {
        It r_last{ std::ranges::next(first, last) }, i{ r_last };
        auto size{ r_last - first };
        while (size > 1) {
            std::swap(*first, *--i);
            plastic::sift_down(first, 0, --size, pred, proj);
        }
        return r_last;
    }

#pragma endregion

#pragma region merge operations

    export template <std::input_iterator It1, std::sentinel_for<It1> Se1, std::input_iterator It2, std::sentinel_for<It2> Se2, std::weakly_incrementable Out, class Pr = std::ranges::less, class Pj1 = std::identity, class Pj2 = std::identity>
        requires std::mergeable<It1, It2, Out, Pr, Pj1, Pj2>
    std::ranges::in_in_out_result<It1, It2, Out> merge(It1 first1, Se1 last1, It2 first2, Se2 last2, Out output, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        while (first1 != last1 && first2 != last2) {
            if (std::invoke(pred, std::invoke(proj2, *first2), std::invoke(proj1, *first1))) {
                *output++ = *first2;
                ++first2;
            }
            else {
                *output++ = *first1;
                ++first1;
            }
        }

        if (first1 == last1) {
            auto res{ plastic::copy(first2, last2, output) };
            return { std::move(first1), std::move(res.in), std::move(res.out) };
        }
        auto res{ plastic::copy(first1, last1, output) };
        return { std::move(res.in), std::move(first2), std::move(res.out) };
    }

    export template <std::bidirectional_iterator It, std::sentinel_for<It> Se, class Pr = std::ranges::less, class Pj = std::identity>
        requires std::sortable<It, Pr, Pj>
    It inplace_merge(It first, It middle, Se last, Pr pred = {}, Pj proj = {}) {
        std::allocator<std::iter_value_t<It>> alloc;
        auto size{ static_cast<std::size_t>(std::ranges::distance(first, last)) };
        auto buf{ alloc.allocate(size) };

        It i{ first }, j{ middle };
        auto k{ buf };
        while (i != middle && j != last) {
            if (!std::invoke(pred, std::invoke(proj, *j), std::invoke(proj, *i))) {
                std::construct_at(k++, std::move(*i++));
            }
            else {
                std::construct_at(k++, std::move(*j++));
            }
        }

        if (i == middle) {
            j = std::ranges::next(j, last);
        }
        else {
            plastic::move_backward(i, middle, j);
        }
        plastic::move(buf, k, first);
        std::destroy(buf, k);
        alloc.deallocate(buf, size);

        return j;
    }

#pragma endregion

#pragma region sorting operations

    export template <std::forward_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_strict_weak_order<std::projected<It, Pj>> Pr = std::ranges::less>
    It is_sorted_until(It first, Se last, Pr pred = {}, Pj proj = {}) {
        if (first == last) {
            return first;
        }

        It i{ first };
        while (++i != last) {
            if (std::invoke(pred, std::invoke(proj, *i), std::invoke(proj, *first++))) {
                break;
            }
        }
        return i;
    }

    export template <std::forward_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_strict_weak_order<std::projected<It, Pj>> Pr = std::ranges::less>
    bool is_sorted(It first, Se last, Pr pred = {}, Pj proj = {}) {
        return plastic::is_sorted_until(first, last, pred, proj) == last;
    }

    template <std::bidirectional_iterator It, class Pr, class Pj>
    void insertion_sort(It first, It last, Pr pred, Pj proj) {
        if (first == last) {
            return;
        }

        It i{ first };
        while (++i != last) {
            auto value{ std::move(*i) };
            It hole{ i }, prev{ i };
            while (prev != first) {
                if (!std::invoke(pred, std::invoke(proj, value), std::invoke(proj, *--prev))) {
                    break;
                }
                *hole-- = std::move(*prev);
            }
            *hole = std::move(value);
        }
    }

    static constexpr std::ptrdiff_t INSERTION_SORT_THRESHOLD{ 32 };

    template <std::bidirectional_iterator It, class Pr, class Pj>
    std::ranges::subrange<It> median_partition(It first, It last, Pr pred, Pj proj) {
        assert(first != last);
        It middle{ std::ranges::next(first, std::ranges::distance(first, last) >> 1) };
        if (std::invoke(pred, std::invoke(proj, *middle), std::invoke(proj, *first))) {
            std::swap(*first, *middle);
        }
        if (std::invoke(pred, std::invoke(proj, *std::ranges::prev(last)), std::invoke(proj, *middle))) {
            std::swap(*middle, *std::ranges::prev(last));
            if (std::invoke(pred, std::invoke(proj, *middle), std::invoke(proj, *first))) {
                std::swap(*first, *middle);
            }
        }

        It left{ middle }, right{ middle }, i{ middle }, j{ middle };
        while (true) {
            while (first != i && !std::invoke(pred, std::invoke(proj, *left), std::invoke(proj, *std::ranges::prev(i)))) {
                if (!std::invoke(pred, std::invoke(proj, *std::ranges::prev(i)), std::invoke(proj, *left))) {
                    std::swap(*--left, *std::ranges::prev(i));
                }
                --i;
            }
            while (j != last && !std::invoke(pred, std::invoke(proj, *j), std::invoke(proj, *left))) {
                if (!std::invoke(pred, std::invoke(proj, *left), std::invoke(proj, *j))) {
                    std::swap(*right++, *j);
                }
                ++j;
            }

            if (i == first && j == last) {
                return { std::move(left), std::move(right) };
            }

            if (i == first) {
                std::swap(*right, *j++);
                std::swap(*left++, *right++);
            }
            else if (j == last) {
                std::swap(*--left, *--i);
                std::swap(*left, *--right);
            }
            else {
                std::swap(*j++, *--i);
            }
        }
    }

    template <std::random_access_iterator It, class Pr, class Pj>
    void intro_sort(It first, It last, std::iter_difference_t<It> margin, Pr pred, Pj proj) {
        if (last - first <= INSERTION_SORT_THRESHOLD) {
            plastic::insertion_sort(first, last, pred, proj);
            return;
        }

        if (margin == 0) {
            plastic::make_heap(first, last, pred, proj);
            plastic::sort_heap(first, last, pred, proj);
            return;
        }

        auto [left, right]{ plastic::median_partition(first, last, pred, proj) };
        margin = (margin >> 1) + (margin >> 2);
        intro_sort(first, left, margin, pred, proj);
        intro_sort(right, last, margin, pred, proj);
    }

    export template <std::random_access_iterator It, std::sentinel_for<It> Se, class Pr = std::ranges::less, class Pj = std::identity>
        requires std::sortable<It, Pr, Pj>
    It sort(It first, Se last, Pr pred = {}, Pj proj = {}) {
        It r_last{ std::ranges::next(first, last) };
        plastic::intro_sort(first, r_last, r_last - first, pred, proj);
        return r_last;
    }

    export template <std::random_access_iterator It, std::sentinel_for<It> Se, class Pr = std::ranges::less, class Pj = std::identity>
        requires std::sortable<It, Pr, Pj>
    It partial_sort(It first, It middle, Se last, Pr pred = {}, Pj proj = {}) {
        plastic::make_heap(first, middle, pred, proj);
        It i{ middle };
        auto size{ middle - first };
        while (i != last) {
            if (std::invoke(pred, std::invoke(proj, *i), std::invoke(proj, *first))) {
                std::swap(*i, *first);
                plastic::sift_down(first, 0, size, pred, proj);
            }
            ++i;
        }
        plastic::sort_heap(first, middle, pred, proj);
        return i;
    }

    export template <std::input_iterator It1, std::sentinel_for<It1> Se1, std::random_access_iterator It2, std::sentinel_for<It2> Se2, class Pr = std::ranges::less, class Pj1 = std::identity, class Pj2 = std::identity>
        requires std::indirectly_copyable<It1, It2> && std::sortable<It2, Pr, Pj2> && std::indirect_strict_weak_order<Pr, std::projected<It1, Pj1>, std::projected<It2, Pj2>>
    std::ranges::in_out_result<It1, It2> partial_sort_copy(It1 first1, Se1 last1, It2 first2, Se2 last2, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        It2 i{ first2 };
        while (first1 != last1 && i != last2) {
            *i++ = *first1;
            ++first1;
        }

        auto size{ i - first2 };
        plastic::make_heap(first2, i, pred, proj2);
        while (first1 != last1) {
            if (std::invoke(pred, std::invoke(proj1, *first1), std::invoke(proj2, *first2))) {
                *first2 = *first1;
                plastic::sift_down(first2, 0, size, pred, proj2);
            }
            ++first1;
        }
        plastic::sort_heap(first2, i, pred, proj2);
        return { std::move(first1), std::move(i) };
    }

    template <std::bidirectional_iterator It, class Pr, class Pj>
    void merge_sort(It first, It last, Pr pred, Pj proj) {
        auto size{ std::ranges::distance(first, last) };
        if (size <= INSERTION_SORT_THRESHOLD) {
            plastic::insertion_sort(first, last, pred, proj);
            return;
        }

        It middle{ std::ranges::next(first, size >> 1) };
        merge_sort(first, middle, pred, proj);
        merge_sort(middle, last, pred, proj);
        if (std::invoke(pred, std::invoke(proj, *middle), std::invoke(proj, *std::ranges::prev(middle)))) {
            plastic::inplace_merge(first, middle, last, pred, proj);
        }
    }

    export template <std::random_access_iterator It, std::sentinel_for<It> Se, class Pr = std::ranges::less, class Pj = std::identity>
        requires std::sortable<It, Pr, Pj>
    It stable_sort(It first, Se last, Pr pred = {}, Pj proj = {}) {
        It r_last{ std::ranges::next(first, last) };
        plastic::merge_sort(first, r_last, pred, proj);
        return r_last;
    }

    export template <std::random_access_iterator It, std::sentinel_for<It> Se, class Pr = std::ranges::less, class Pj = std::identity>
        requires std::sortable<It, Pr, Pj>
    It nth_element(It first, It middle, Se last, Pr pred = {}, Pj proj = {}) {
        It r_last{ std::ranges::next(first, last) }, i{ r_last };
        while (i - first > INSERTION_SORT_THRESHOLD) {
            auto [left, right]{ plastic::median_partition(first, i, pred, proj) };
            if (middle < left) {
                i = left;
            }
            else if (right <= middle) {
                first = right;
            }
            else {
                return r_last;
            }
        }
        plastic::insertion_sort(first, i, pred, proj);
        return r_last;
    }

#pragma endregion

#pragma region set operations

    export template <std::input_iterator It1, std::sentinel_for<It1> Se1, std::input_iterator It2, std::sentinel_for<It2> Se2, class Pj1 = std::identity, class Pj2 = std::identity, std::indirect_strict_weak_order<std::projected<It1, Pj1>, std::projected<It2, Pj2>> Pr = std::ranges::less>
    bool includes(It1 first1, Se1 last1, It2 first2, Se2 last2, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        while (first1 != last1 && first2 != last2) {
            if (std::invoke(pred, std::invoke(proj2, *first2), std::invoke(proj1, *first1))) {
                return false;
            }
            if (!std::invoke(pred, std::invoke(proj1, *first1), std::invoke(proj2, *first2))) {
                ++first2;
            }
            ++first1;
        }
        return first2 == last2;
    }

    export template <std::input_iterator It1, std::sentinel_for<It1> Se1, std::input_iterator It2, std::sentinel_for<It2> Se2, std::weakly_incrementable Out, class Pr = std::ranges::less, class Pj1 = std::identity, class Pj2 = std::identity>
        requires std::mergeable<It1, It2, Out, Pr, Pj1, Pj2>
    std::ranges::in_out_result<It1, Out> set_difference(It1 first1, Se1 last1, It2 first2, Se2 last2, Out output, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        while (first1 != last1 && first2 != last2) {
            if (std::invoke(pred, std::invoke(proj1, *first1), std::invoke(proj2, *first2))) {
                *output++ = *first1;
                ++first1;
            }
            else {
                if (!std::invoke(pred, std::invoke(proj2, *first2), std::invoke(proj1, *first1))) {
                    ++first1;
                }
                ++first2;
            }
        }
        return plastic::copy(first1, last1, output);
    }

    export template <std::input_iterator It1, std::sentinel_for<It1> Se1, std::input_iterator It2, std::sentinel_for<It2> Se2, std::weakly_incrementable Out, class Pr = std::ranges::less, class Pj1 = std::identity, class Pj2 = std::identity>
        requires std::mergeable<It1, It2, Out, Pr, Pj1, Pj2>
    std::ranges::in_in_out_result<It1, It2, Out> set_intersection(It1 first1, Se1 last1, It2 first2, Se2 last2, Out output, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        while (first1 != last1 && first2 != last2) {
            if (std::invoke(pred, std::invoke(proj1, *first1), std::invoke(proj2, *first2))) {
                ++first1;
            }
            else {
                if (!std::invoke(pred, std::invoke(proj2, *first2), std::invoke(proj1, *first1))) {
                    *output++ = *first1;
                    ++first1;
                }
                ++first2;
            }
        }
        return { std::ranges::next(first1, last1), std::ranges::next(first2, last2), output };
    }

    export template <std::input_iterator It1, std::sentinel_for<It1> Se1, std::input_iterator It2, std::sentinel_for<It2> Se2, std::weakly_incrementable Out, class Pr = std::ranges::less, class Pj1 = std::identity, class Pj2 = std::identity>
        requires std::mergeable<It1, It2, Out, Pr, Pj1, Pj2>
    std::ranges::in_in_out_result<It1, It2, Out> set_symmetric_difference(It1 first1, Se1 last1, It2 first2, Se2 last2, Out output, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        while (first1 != last1 && first2 != last2) {
            if (std::invoke(pred, std::invoke(proj1, *first1), std::invoke(proj2, *first2))) {
                *output++ = *first1;
                ++first1;
            }
            else if (std::invoke(pred, std::invoke(proj2, *first2), std::invoke(proj1, *first1))) {
                *output++ = *first2;
                ++first2;
            }
            else {
                ++first1, ++first2;
            }
        }

        if (first1 == last1) {
            auto res{ plastic::copy(first2, last2, output) };
            return { std::move(first1), std::move(res.in), std::move(res.out) };
        }
        auto res{ plastic::copy(first1, last1, output) };
        return { std::move(res.in), std::move(first2), std::move(res.out) };
    }

    export template <std::input_iterator It1, std::sentinel_for<It1> Se1, std::input_iterator It2, std::sentinel_for<It2> Se2, std::weakly_incrementable Out, class Pr = std::ranges::less, class Pj1 = std::identity, class Pj2 = std::identity>
        requires std::mergeable<It1, It2, Out, Pr, Pj1, Pj2>
    std::ranges::in_in_out_result<It1, It2, Out> set_union(It1 first1, Se1 last1, It2 first2, Se2 last2, Out output, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        while (first1 != last1 && first2 != last2) {
            if (std::invoke(pred, std::invoke(proj2, *first2), std::invoke(proj1, *first1))) {
                *output++ = *first2;
                ++first2;
            }
            else {
                *output++ = *first1;
                if (!std::invoke(pred, std::invoke(proj1, *first1), std::invoke(proj2, *first2))) {
                    ++first2;
                }
                ++first1;
            }
        }

        if (first1 == last1) {
            auto res{ plastic::copy(first2, last2, output) };
            return { std::move(first1), std::move(res.in), std::move(res.out) };
        }
        auto res{ plastic::copy(first1, last1, output) };
        return { std::move(res.in), std::move(first2), std::move(res.out) };
    }

#pragma endregion

#pragma region minimum/maximum operations

    export template <std::forward_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_strict_weak_order<std::projected<It, Pj>> Pr = std::ranges::less>
    It max_element(It first, Se last, Pr pred = {}, Pj proj = {}) {
        if (first == last) {
            return first;
        }

        It i{ first };
        while (++first != last) {
            if (std::invoke(pred, std::invoke(proj, *i), std::invoke(proj, *first))) {
                i = first;
            }
        }
        return i;
    }

    export template <std::forward_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_strict_weak_order<std::projected<It, Pj>> Pr = std::ranges::less>
    It min_element(It first, Se last, Pr pred = {}, Pj proj = {}) {
        if (first == last) {
            return first;
        }

        It i{ first };
        while (++first != last) {
            if (std::invoke(pred, std::invoke(proj, *first), std::invoke(proj, *i))) {
                i = first;
            }
        }
        return i;
    }

    export template <std::forward_iterator It, std::sentinel_for<It> Se, class Pj = std::identity, std::indirect_strict_weak_order<std::projected<It, Pj>> Pr = std::ranges::less>
    std::ranges::minmax_result<It> minmax_element(It first, Se last, Pr pred = {}, Pj proj = {}) {
        if (first == last) {
            return { first, first };
        }

        It min{ first }, max{ first };
        while (++first != last) {
            It i{ first };
            if (++first == last) {
                if (std::invoke(pred, std::invoke(proj, *i), std::invoke(proj, *min))) {
                    min = i;
                }
                else if (!std::invoke(pred, std::invoke(proj, *i), std::invoke(proj, *max))) {
                    max = i;
                }
                break;
            }

            if (std::invoke(pred, std::invoke(proj, *first), std::invoke(proj, *i))) {
                if (std::invoke(pred, std::invoke(proj, *first), std::invoke(proj, *min))) {
                    min = first;
                }
                if (!std::invoke(pred, std::invoke(proj, *i), std::invoke(proj, *max))) {
                    max = i;
                }
            }
            else {
                if (std::invoke(pred, std::invoke(proj, *i), std::invoke(proj, *min))) {
                    min = i;
                }
                if (!std::invoke(pred, std::invoke(proj, *first), std::invoke(proj, *max))) {
                    max = first;
                }
            }
        }

        return { std::move(min), std::move(max) };
    }

    export template <class T, class Pj = std::identity, std::indirect_strict_weak_order<std::projected<const T*, Pj>> Pr = std::ranges::less>
    const T& max(const T& a, const T& b, Pr pred = {}, Pj proj = {}) {
        return std::invoke(pred, std::invoke(proj, a), std::invoke(proj, b)) ? b : a;
    }

    export template <std::copyable T, class Pj = std::identity, std::indirect_strict_weak_order<std::projected<const T*, Pj>> Pr = std::ranges::less>
    T max(std::initializer_list<T> list, Pr pred = {}, Pj proj = {}) {
        assert(list.size() != 0);
        return *plastic::max_element(list.begin(), list.end(), pred, proj);
    }

    export template <class T, class Pj = std::identity, std::indirect_strict_weak_order<std::projected<const T*, Pj>> Pr = std::ranges::less>
    const T& min(const T& a, const T& b, Pr pred = {}, Pj proj = {}) {
        return std::invoke(pred, std::invoke(proj, b), std::invoke(proj, a)) ? b : a;
    }

    export template <std::copyable T, class Pj = std::identity, std::indirect_strict_weak_order<std::projected<const T*, Pj>> Pr = std::ranges::less>
    T min(std::initializer_list<T> list, Pr pred = {}, Pj proj = {}) {
        assert(list.size() != 0);
        return *plastic::min_element(list.begin(), list.end(), pred, proj);
    }

    export template <class T, class Pj = std::identity, std::indirect_strict_weak_order<std::projected<const T*, Pj>> Pr = std::ranges::less>
    std::ranges::minmax_result<const T&> minmax(const T& a, const T& b, Pr pred = {}, Pj proj = {}) {
        if (std::invoke(pred, std::invoke(proj, b), std::invoke(proj, a))) {
            return { b, a };
        }
        return { a, b };
    }

    export template <std::copyable T, class Pj = std::identity, std::indirect_strict_weak_order<std::projected<const T*, Pj>> Pr = std::ranges::less>
    std::ranges::minmax_result<T> minmax(std::initializer_list<T> list, Pr pred = {}, Pj proj = {}) {
        assert(list.size() != 0);
        auto res{ plastic::minmax_element(list.begin(), list.end(), pred, proj) };
        return { *res.min, *res.max };
    }

    export template <class T, class Pj = std::identity, std::indirect_strict_weak_order<std::projected<const T*, Pj>> Pr = std::ranges::less>
    const T& clamp(const T& value, const T& lowest, const T& highest, Pr pred = {}, Pj proj = {}) {
        assert(!std::invoke(pred, std::invoke(proj, highest), std::invoke(proj, lowest)));
        if (std::invoke(pred, std::invoke(proj, value), std::invoke(proj, lowest))) {
            return lowest;
        }
        if (std::invoke(pred, std::invoke(proj, highest), std::invoke(proj, value))) {
            return highest;
        }
        return value;
    }

#pragma endregion

#pragma region permutation operations

    export template <std::forward_iterator It1, std::sentinel_for<It1> Se1, std::forward_iterator It2, std::sentinel_for<It2> Se2, class Pj1 = std::identity, class Pj2 = std::identity, std::indirect_equivalence_relation<std::projected<It1, Pj1>, std::projected<It2, Pj2>> Pr = std::ranges::equal_to>
    bool is_permutation(It1 first1, Se1 last1, It2 first2, Se2 last2, Pr pred = {}, Pj1 proj1 = {}, Pj2 proj2 = {}) {
        It1 i{ first1 };
        It2 j{ first2 };
        while (j != last2) {
            if (i == last1) {
                return false;
            }
            ++i, ++j;
        }
        if (i != last1) {
            return false;
        }

        i = first1;
        while (i != last1) {
            if (!plastic::contains(first1, i, *i, proj1)) {
                auto count{ plastic::count(first2, last2, *i, proj2) };
                if (count == 0 || count != plastic::count(i, last1, *i, proj1)) {
                    return false;
                }
            }
            ++i;
        }
        return true;
    }

    export template <std::bidirectional_iterator It, std::sentinel_for<It> Se, class Pr = std::ranges::less, class Pj = std::identity>
        requires std::sortable<It, Pr, Pj>
    std::ranges::in_found_result<It> next_permutation(It first, Se last, Pr pred = {}, Pj proj = {}) {
        It r_last{ std::ranges::next(first, last) }, i{ r_last }, j{ r_last };
        if (first == r_last || first == --i) {
            return { std::move(first), false };
        }

        while (!std::invoke(pred, std::invoke(proj, *--i), std::invoke(proj, *--j))) {
            if (i == first) {
                plastic::reverse(first, r_last);
                return { std::move(r_last), false };
            }
        }

        j = r_last;
        while (!std::invoke(pred, std::invoke(proj, *i), std::invoke(proj, *--j))) {}
        std::swap(*i, *j);
        plastic::reverse(++i, r_last);

        return { std::move(r_last), true };
    }

    export template <std::bidirectional_iterator It, std::sentinel_for<It> Se, class Pr = std::ranges::less, class Pj = std::identity>
        requires std::sortable<It, Pr, Pj>
    std::ranges::in_found_result<It> prev_permutation(It first, Se last, Pr pred = {}, Pj proj = {}) {
        It r_last{ std::ranges::next(first, last) }, i{ r_last }, j{ r_last };
        if (first == r_last || first == --i) {
            return { std::move(first), false };
        }

        while (!std::invoke(pred, std::invoke(proj, *--j), std::invoke(proj, *--i))) {
            if (i == first) {
                plastic::reverse(first, r_last);
                return { std::move(r_last), false };
            }
        }

        j = r_last;
        while (!std::invoke(pred, std::invoke(proj, *--j), std::invoke(proj, *i))) {}
        std::swap(*i, *j);
        plastic::reverse(++i, r_last);

        return { std::move(r_last), true };
    }

#pragma endregion

}
