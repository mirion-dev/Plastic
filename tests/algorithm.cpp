#include <doctest/doctest.h>

import std;
import plastic;
import utils;

TEST_SUITE("algorithm") {
    TEST_CASE("non_modifying_sequence") {
        std::vector<int> e, a{ 1, 3, 5, 7, 9 }, b{ 2, 4, 6, 8, 10 }, c{ 1, 2, 3, 2, 1 }, d{ 5, 5, 5, 5 }, x;

        REQUIRE(plastic::all_of(e.begin(), e.end(), [](int x) { return x % 2 == 0; }) == true);
        REQUIRE(plastic::all_of(a.begin(), a.end(), [](int x) { return x % 2 == 1; }) == true);
        REQUIRE(plastic::all_of(b.begin(), b.end(), [](int x) { return x % 2 == 1; }) == false);
        REQUIRE(plastic::all_of(c.begin(), c.end(), [](int x) { return x % 2 == 1; }) == false);

        REQUIRE(plastic::any_of(e.begin(), e.end(), [](int x) { return x > 0; }) == false);
        REQUIRE(plastic::any_of(a.begin(), a.end(), [](int x) { return x > 5; }) == true);
        REQUIRE(plastic::any_of(b.begin(), b.end(), [](int x) { return x % 2 == 0; }) == true);
        REQUIRE(plastic::any_of(c.begin(), c.end(), [](int x) { return x == 4; }) == false);

        REQUIRE(plastic::none_of(e.begin(), e.end(), [](int x) { return x < 0; }) == true);
        REQUIRE(plastic::none_of(a.begin(), a.end(), [](int x) { return x % 2 == 0; }) == true);
        REQUIRE(plastic::none_of(b.begin(), b.end(), [](int x) { return x > 2; }) == false);
        REQUIRE(plastic::none_of(c.begin(), c.end(), [](int x) { return x == 2; }) == false);

        REQUIRE(plastic::contains(e.begin(), e.end(), 3) == false);
        REQUIRE(plastic::contains(a.begin(), a.end(), 3) == true);
        REQUIRE(plastic::contains(b.begin(), b.end(), 3) == false);
        REQUIRE(plastic::contains(c.begin(), c.end(), 3) == true);

        x = { 3, 2, 1 };
        REQUIRE(plastic::contains_subrange(e.begin(), e.end(), x.begin(), x.end()) == false);
        REQUIRE(plastic::contains_subrange(a.begin(), a.end(), e.begin(), e.end()) == true);
        REQUIRE(plastic::contains_subrange(a.begin(), a.end(), x.begin(), x.end()) == false);
        REQUIRE(plastic::contains_subrange(b.begin(), b.end(), x.begin(), x.end()) == false);
        REQUIRE(plastic::contains_subrange(c.begin(), c.end(), x.begin(), x.end()) == true);

        x = { 1, 2, 3 };
        plastic::for_each(e.begin(), e.end(), [](int& x) { x += 10; });
        plastic::for_each(x.begin(), x.end(), [](int& x) { x += 10; });
        REQUIRE(format(e) == "[]");
        REQUIRE(format(x) == "[11, 12, 13]");

        x = { 1, 2, 3, 4, 5 };
        plastic::for_each_n(e.begin(), 0, [](int& x) { x += 10; });
        plastic::for_each_n(x.begin(), 3, [](int& x) { x *= 2; });
        REQUIRE(format(e) == "[]");
        REQUIRE(format(x) == "[2, 4, 6, 4, 5]");

        REQUIRE(plastic::find(e.begin(), e.end(), 0) == e.end());
        REQUIRE(plastic::find(a.begin(), a.end(), 7) == a.begin() + 3);
        REQUIRE(plastic::find(b.begin(), b.end(), 5) == b.end());

        REQUIRE(plastic::find_if(e.begin(), e.end(), [](int x) { return x == 0; }) == e.end());
        REQUIRE(plastic::find_if(a.begin(), a.end(), [](int x) { return x < 0; }) == a.end());
        REQUIRE(plastic::find_if(b.begin(), b.end(), [](int x) { return x > 5; }) == b.begin() + 2);

        REQUIRE(plastic::find_if_not(e.begin(), e.end(), [](int x) { return x != 0; }) == e.end());
        REQUIRE(plastic::find_if_not(c.begin(), c.end(), [](int x) { return x < 3; }) == c.begin() + 2);
        REQUIRE(plastic::find_if_not(d.begin(), d.end(), [](int x) { return x == 5; }) == d.end());

        REQUIRE(plastic::find_last(e.begin(), e.end(), 0).begin() == e.end());
        REQUIRE(plastic::find_last(b.begin(), b.end(), 5).begin() == b.end());
        REQUIRE(plastic::find_last(c.begin(), c.end(), 2).begin() == c.begin() + 3);

        REQUIRE(plastic::find_last_if(e.begin(), e.end(), [](int x) { return x == 0; }).begin() == e.end());
        REQUIRE(plastic::find_last_if(a.begin(), a.end(), [](int x) { return x % 2 == 0; }).begin() == a.end());
        REQUIRE(plastic::find_last_if(c.begin(), c.end(), [](int x) { return x < 3; }).begin() == c.begin() + 4);

        REQUIRE(plastic::find_last_if_not(e.begin(), e.end(), [](int x) { return x != 0; }).begin() == e.end());
        REQUIRE(plastic::find_last_if_not(c.begin(), c.end(), [](int x) { return x == 1; }).begin() == c.begin() + 3);
        REQUIRE(plastic::find_last_if_not(d.begin(), d.end(), [](int x) { return x == 5; }).begin() == d.end());

        x = { 5, 5 };
        REQUIRE(plastic::find_end(a.begin(), a.end(), e.begin(), e.end()).begin() == a.end());
        REQUIRE(plastic::find_end(e.begin(), e.end(), a.begin(), a.end()).begin() == e.end());
        REQUIRE(plastic::find_end(c.begin(), c.end(), x.begin(), x.end()).begin() == c.end());
        REQUIRE(plastic::find_end(d.begin(), d.end(), x.begin(), x.end()).begin() == d.begin() + 2);

        x = { 0, 10 };
        REQUIRE(plastic::find_first_of(a.begin(), a.end(), e.begin(), e.end()) == a.end());
        REQUIRE(plastic::find_first_of(e.begin(), e.end(), a.begin(), a.end()) == e.end());
        REQUIRE(plastic::find_first_of(a.begin(), a.end(), x.begin(), x.end()) == a.end());
        REQUIRE(plastic::find_first_of(b.begin(), b.end(), x.begin(), x.end()) == b.begin() + 4);

        REQUIRE(plastic::adjacent_find(e.begin(), e.end()) == e.end());
        REQUIRE(plastic::adjacent_find(a.begin(), a.end()) == a.end());
        REQUIRE(plastic::adjacent_find(d.begin(), d.end()) == d.begin());

        REQUIRE(plastic::count(e.begin(), e.end(), 0) == 0);
        REQUIRE(plastic::count(c.begin(), c.end(), 2) == 2);
        REQUIRE(plastic::count(d.begin(), d.end(), 5) == 4);

        REQUIRE(plastic::count_if(e.begin(), e.end(), [](int x) { return x == 0; }) == 0);
        REQUIRE(plastic::count_if(a.begin(), a.end(), [](int x) { return x > 5; }) == 2);
        REQUIRE(plastic::count_if(b.begin(), b.end(), [](int x) { return x % 4 == 2; }) == 3);

        REQUIRE(plastic::mismatch(e.begin(), e.end(), e.begin(), e.end()).in1 == e.begin());
        REQUIRE(plastic::mismatch(a.begin(), a.end(), c.begin(), c.end()).in1 == a.begin() + 1);
        REQUIRE(plastic::mismatch(b.begin(), b.end(), b.begin(), b.end()).in2 == b.end());

        REQUIRE(plastic::equal(e.begin(), e.end(), e.begin(), e.end()) == true);
        REQUIRE(plastic::equal(e.begin(), e.end(), a.begin(), a.end()) == false);
        REQUIRE(plastic::equal(a.begin(), a.end(), b.begin(), b.end()) == false);
        REQUIRE(plastic::equal(a.begin(), a.end(), b.begin(), b.end(), {}, [](int x) { return x + 1; }) == true);

        REQUIRE(plastic::is_permutation(e.begin(), e.end(), e.begin(), e.end()) == true);
        x = { 9, 7, 5, 3, 1 };
        REQUIRE(plastic::is_permutation(a.begin(), a.end(), x.begin(), x.end()) == true);
        x = { 1, 1, 3, 5, 7 };
        REQUIRE(plastic::is_permutation(a.begin(), a.end(), x.begin(), x.end()) == false);
        x = { 1, 3, 5, 7, 9, 11 };
        REQUIRE(plastic::is_permutation(a.begin(), a.end(), x.begin(), x.end()) == false);

        x = { 3, 5, 7 };
        REQUIRE(plastic::search(a.begin(), a.end(), e.begin(), e.end()).begin() == a.begin());
        REQUIRE(plastic::search(e.begin(), e.end(), a.begin(), a.end()).begin() == e.end());
        REQUIRE(plastic::search(a.begin(), a.end(), x.begin(), x.end()).begin() == a.begin() + 1);
        REQUIRE(plastic::search(b.begin(), b.end(), x.begin(), x.end()).begin() == b.end());

        REQUIRE(plastic::search_n(e.begin(), e.end(), 0, 10).begin() == e.begin());
        REQUIRE(plastic::search_n(e.begin(), e.end(), 10, 0).begin() == e.end());
        REQUIRE(plastic::search_n(c.begin(), c.end(), 2, 1, std::ranges::greater{}).begin() == c.begin() + 1);
        REQUIRE(plastic::search_n(d.begin(), d.end(), 4, 5).begin() == d.begin());

        x = { 1, 3, 5 };
        REQUIRE(plastic::starts_with(e.begin(), e.end(), x.begin(), x.end()) == false);
        REQUIRE(plastic::starts_with(a.begin(), a.end(), e.begin(), e.end()) == true);
        REQUIRE(plastic::starts_with(a.begin(), a.end(), x.begin(), x.end()) == true);
        REQUIRE(plastic::starts_with(b.begin(), b.end(), x.begin(), x.end()) == false);
        REQUIRE(plastic::starts_with(c.begin(), c.end(), x.begin(), x.end()) == false);

        x = { 3, 2, 1 };
        REQUIRE(plastic::ends_with(e.begin(), e.end(), x.begin(), x.end()) == false);
        REQUIRE(plastic::ends_with(a.begin(), a.end(), e.begin(), e.end()) == true);
        REQUIRE(plastic::ends_with(a.begin(), a.end(), x.begin(), x.end()) == false);
        REQUIRE(plastic::ends_with(b.begin(), b.end(), x.begin(), x.end()) == false);
        REQUIRE(plastic::ends_with(c.begin(), c.end(), x.begin(), x.end()) == true);

        REQUIRE(plastic::fold_left(e.begin(), e.end(), 10, std::plus{}) == 10);
        REQUIRE(plastic::fold_left(a.begin(), a.end(), 0, std::plus{}) == 25);
        REQUIRE(plastic::fold_left(a.begin(), a.end(), 10, std::minus{}) == -15);
        REQUIRE(plastic::fold_left(b.begin(), b.end(), 1, std::multiplies{}) == 3840);

        REQUIRE(!plastic::fold_left_first(e.begin(), e.end(), std::plus{}));
        REQUIRE(*plastic::fold_left_first(a.begin(), a.end(), std::plus{}) == 25);
        REQUIRE(*plastic::fold_left_first(b.begin(), b.end(), std::multiplies{}) == 3840);

        REQUIRE(plastic::fold_right(e.begin(), e.end(), 100, std::plus{}) == 100);
        REQUIRE(plastic::fold_right(a.begin(), a.end(), 0, std::plus{}) == 25);
        REQUIRE(plastic::fold_right(b.begin(), b.end(), 2, std::minus{}) == 4);

        REQUIRE(!plastic::fold_right_last(e.begin(), e.end(), std::plus{}));
        REQUIRE(*plastic::fold_right_last(a.begin(), a.end(), std::plus{}) == 25);
        REQUIRE(*plastic::fold_right_last(b.begin(), b.end(), std::minus{}) == 6);

        auto res1{ plastic::fold_left_with_iter(a.begin(), a.end(), 0, std::plus{}) };
        REQUIRE((res1.value == 25 && res1.in == a.end()));
        auto res2{ plastic::fold_left_with_iter(a.begin(), a.begin() + 3, 10, std::multiplies{}) };
        REQUIRE((res2.value == 150 && res2.in == a.begin() + 3));

        auto res3{ plastic::fold_left_first_with_iter(e.begin(), e.end(), std::plus{}) };
        REQUIRE((!res3.value && res3.in == e.end()));
        auto res4{ plastic::fold_left_first_with_iter(b.begin(), b.end(), std::multiplies{}) };
        REQUIRE((*res4.value == 3840 && res4.in == b.end()));
    }

    TEST_CASE("mutating_sequence") {
        std::vector<int> e, a{ 1, 3, 5, 7, 9 }, b{ 2, 4, 6, 8, 10 }, c{ 1, 2, 3, 2, 1 }, d{ 5, 5, 5, 5 }, x, y;

        x = { 0, 0, 0, 0, 0 };
        plastic::copy(e.begin(), e.end(), x.begin());
        REQUIRE(format(x) == "[0, 0, 0, 0, 0]");
        plastic::copy(a.begin(), a.end(), x.begin());
        REQUIRE(format(x) == "[1, 3, 5, 7, 9]");
        plastic::copy(d.begin(), d.end(), x.begin());
        REQUIRE(format(x) == "[5, 5, 5, 5, 9]");

        x = { 0, 0, 0, 0, 0 };
        plastic::copy_n(e.begin(), 0, x.begin());
        REQUIRE(format(x) == "[0, 0, 0, 0, 0]");
        plastic::copy_n(a.begin(), 3, x.begin());
        REQUIRE(format(x) == "[1, 3, 5, 0, 0]");
        plastic::copy_n(b.begin(), 4, x.begin());
        REQUIRE(format(x) == "[2, 4, 6, 8, 0]");

        x = { 0, 0, 0, 0, 0 };
        plastic::copy_if(e.begin(), e.end(), x.begin(), [](int x) { return x > 0; });
        REQUIRE(format(x) == "[0, 0, 0, 0, 0]");
        plastic::copy_if(a.begin(), a.end(), x.begin(), [](int x) { return x > 3; });
        REQUIRE(format(x) == "[5, 7, 9, 0, 0]");
        plastic::copy_if(d.begin(), d.end(), x.begin(), [](int x) { return x <= 5; });
        REQUIRE(format(x) == "[5, 5, 5, 5, 0]");

        x = { 0, 0, 0, 0, 0 };
        plastic::copy_backward(e.begin(), e.end(), x.end());
        REQUIRE(format(x) == "[0, 0, 0, 0, 0]");
        plastic::copy_backward(a.begin(), a.end(), x.end());
        REQUIRE(format(x) == "[1, 3, 5, 7, 9]");
        plastic::copy_backward(d.begin(), d.end(), x.end());
        REQUIRE(format(x) == "[1, 5, 5, 5, 5]");

        x = { 0, 0, 0, 0, 0 };
        y = { 1, 2, 3, 4, 5 };
        plastic::move(e.begin(), e.end(), x.begin());
        REQUIRE(format(x) == "[0, 0, 0, 0, 0]");
        plastic::move(y.begin(), y.end(), x.begin());
        REQUIRE(format(x) == "[1, 2, 3, 4, 5]");

        x = { 0, 0, 0, 0, 0 };
        y = { 1, 2, 3, 4, 5 };
        plastic::move_backward(e.begin(), e.end(), x.end());
        REQUIRE(format(x) == "[0, 0, 0, 0, 0]");
        plastic::move_backward(y.begin(), y.end(), x.end());
        REQUIRE(format(x) == "[1, 2, 3, 4, 5]");

        x = { 0, 0, 0, 0, 0 };
        y = e;
        plastic::swap_ranges(x.begin(), x.end(), y.begin(), y.end());
        REQUIRE(format(x) == "[0, 0, 0, 0, 0]");
        REQUIRE(format(y) == "[]");
        y = b;
        plastic::swap_ranges(x.begin(), x.end(), y.begin(), y.end());
        REQUIRE(format(x) == "[2, 4, 6, 8, 10]");
        REQUIRE(format(y) == "[0, 0, 0, 0, 0]");
        y = d;
        plastic::swap_ranges(x.begin(), x.end(), y.begin(), y.end());
        REQUIRE(format(x) == "[5, 5, 5, 5, 10]");
        REQUIRE(format(y) == "[2, 4, 6, 8]");

        x = { 0, 0, 0, 0, 0 };
        plastic::transform(e.begin(), e.end(), x.begin(), [](int x) { return x * 2; });
        REQUIRE(format(x) == "[0, 0, 0, 0, 0]");
        plastic::transform(a.begin(), a.end(), x.begin(), [](int x) { return x * 2; });
        REQUIRE(format(x) == "[2, 6, 10, 14, 18]");
        plastic::transform(d.begin(), d.end(), x.begin(), [](int x) { return x + 1; });
        REQUIRE(format(x) == "[6, 6, 6, 6, 18]");
        plastic::transform(e.begin(), e.end(), a.begin(), a.end(), x.begin(), [](int x, int y) { return x + y; });
        REQUIRE(format(x) == "[6, 6, 6, 6, 18]");
        plastic::transform(a.begin(), a.end(), b.begin(), b.end(), x.begin(), [](int x, int y) { return y - x; });
        REQUIRE(format(x) == "[1, 1, 1, 1, 1]");
        plastic::transform(a.begin(), a.end(), d.begin(), d.end(), x.begin(), [](int x, int y) {
            return y * 2 - x - 1;
        });
        REQUIRE(format(x) == "[8, 6, 4, 2, 1]");

        x = e;
        plastic::replace(x.begin(), x.end(), 2, 9);
        REQUIRE(format(x) == "[]");
        x = c;
        plastic::replace(x.begin(), x.end(), 2, 9);
        REQUIRE(format(x) == "[1, 9, 3, 9, 1]");
        x = d;
        plastic::replace(x.begin(), x.end(), 5, 0);
        REQUIRE(format(x) == "[0, 0, 0, 0]");

        x = e;
        plastic::replace_if(x.begin(), x.end(), [](int x) { return x > 0; }, 0);
        REQUIRE(format(x) == "[]");
        x = a;
        plastic::replace_if(x.begin(), x.end(), [](int x) { return x % 3 == 0; }, 0);
        REQUIRE(format(x) == "[1, 0, 5, 7, 0]");
        x = b;
        plastic::replace_if(x.begin(), x.end(), [](int x) { return x < 6; }, 1);
        REQUIRE(format(x) == "[1, 1, 6, 8, 10]");

        x = { 0, 0, 0, 0, 0 };
        plastic::replace_copy(e.begin(), e.end(), x.begin(), 2, 8);
        REQUIRE(format(x) == "[0, 0, 0, 0, 0]");
        plastic::replace_copy(c.begin(), c.end(), x.begin(), 2, 8);
        REQUIRE(format(x) == "[1, 8, 3, 8, 1]");
        plastic::replace_copy(d.begin(), d.end(), x.begin(), 5, 2);
        REQUIRE(format(x) == "[2, 2, 2, 2, 1]");

        x = { 0, 0, 0, 0, 0 };
        plastic::replace_copy_if(e.begin(), e.end(), x.begin(), [](int x) { return x > 0; }, 0);
        REQUIRE(format(x) == "[0, 0, 0, 0, 0]");
        plastic::replace_copy_if(a.begin(), a.end(), x.begin(), [](int x) { return x > 5; }, 1);
        REQUIRE(format(x) == "[1, 3, 5, 1, 1]");
        plastic::replace_copy_if(b.begin(), b.end(), x.begin(), [](int x) { return x % 2 == 0; }, 7);
        REQUIRE(format(x) == "[7, 7, 7, 7, 7]");

        x = { 0, 0, 0, 0, 0 };
        plastic::fill(e.begin(), e.end(), 3);
        plastic::fill(x.begin(), x.end(), 3);
        REQUIRE(format(e) == "[]");
        REQUIRE(format(x) == "[3, 3, 3, 3, 3]");

        x = { 0, 0, 0, 0, 0 };
        plastic::fill_n(e.begin(), 0, 7);
        plastic::fill_n(x.begin(), 3, 7);
        REQUIRE(format(e) == "[]");
        REQUIRE(format(x) == "[7, 7, 7, 0, 0]");

        x = { 0, 0, 0, 0, 0 };
        plastic::generate(x.begin(), x.end(), [v{ 0 }] mutable { return v += 2; });
        REQUIRE(format(x) == "[2, 4, 6, 8, 10]");
        plastic::generate(x.begin(), x.end(), [i{ a.begin() }] mutable { return *i++; });
        REQUIRE(format(x) == "[1, 3, 5, 7, 9]");

        x = { 0, 0, 0, 0, 0 };
        plastic::generate_n(x.begin(), 4, [v{ 0 }] mutable { return v += 2; });
        REQUIRE(format(x) == "[2, 4, 6, 8, 0]");
        plastic::generate_n(x.begin(), 5, [i{ a.begin() }] mutable { return *i++; });
        REQUIRE(format(x) == "[1, 3, 5, 7, 9]");

        x = { 1, 2, 2, 2, 5 };
        x.erase(plastic::remove(x.begin(), x.end(), 2).begin(), x.end());
        REQUIRE(format(x) == "[1, 5]");
        x.erase(plastic::remove(x.begin(), x.end(), 1).begin(), x.end());
        REQUIRE(format(x) == "[5]");
        x.erase(plastic::remove(x.begin(), x.end(), 5).begin(), x.end());
        REQUIRE(format(x) == "[]");
        x.erase(plastic::remove(x.begin(), x.end(), 0).begin(), x.end());
        REQUIRE(format(x) == "[]");

        x = { 1, 2, 3, 2, 1 };
        x.erase(plastic::remove_if(x.begin(), x.end(), [](int x) { return x % 2 == 0; }).begin(), x.end());
        REQUIRE(format(x) == "[1, 3, 1]");
        x.erase(plastic::remove_if(x.begin(), x.end(), [](int x) { return x >= 2; }).begin(), x.end());
        REQUIRE(format(x) == "[1, 1]");
        x.erase(plastic::remove_if(x.begin(), x.end(), [](int x) { return x == 1; }).begin(), x.end());
        REQUIRE(format(x) == "[]");
        x.erase(plastic::remove_if(x.begin(), x.end(), [](int x) { return true; }).begin(), x.end());
        REQUIRE(format(x) == "[]");

        x = { 0, 0, 0, 0, 0 };
        plastic::remove_copy(e.begin(), e.end(), x.begin(), 2);
        REQUIRE(format(x) == "[0, 0, 0, 0, 0]");
        plastic::remove_copy(c.begin(), c.end(), x.begin(), 2);
        REQUIRE(format(x) == "[1, 3, 1, 0, 0]");
        plastic::remove_copy(d.begin(), d.end(), x.begin(), 5);
        REQUIRE(format(x) == "[1, 3, 1, 0, 0]");

        x = { 0, 0, 0, 0, 0 };
        plastic::remove_copy_if(e.begin(), e.end(), x.begin(), [](int x) { return x > 0; });
        REQUIRE(format(x) == "[0, 0, 0, 0, 0]");
        plastic::remove_copy_if(a.begin(), a.end(), x.begin(), [](int x) { return x < 5; });
        REQUIRE(format(x) == "[5, 7, 9, 0, 0]");
        plastic::remove_copy_if(b.begin(), b.end(), x.begin(), [](int x) { return x > 5; });
        REQUIRE(format(x) == "[2, 4, 9, 0, 0]");

        x = e;
        x.erase(plastic::unique(x.begin(), x.end()).begin(), x.end());
        REQUIRE(format(x) == "[]");
        x = d;
        x.erase(plastic::unique(x.begin(), x.end()).begin(), x.end());
        REQUIRE(format(x) == "[5]");
        x = { 1, 1, 2, 2, 3 };
        x.erase(plastic::unique(x.begin(), x.end()).begin(), x.end());
        REQUIRE(format(x) == "[1, 2, 3]");

        x = { 0, 0, 0, 0, 0 };
        plastic::unique_copy(e.begin(), e.end(), x.begin());
        REQUIRE(format(x) == "[0, 0, 0, 0, 0]");
        plastic::unique_copy(c.begin(), c.end(), x.begin());
        REQUIRE(format(x) == "[1, 2, 3, 2, 1]");
        plastic::unique_copy(d.begin(), d.end(), x.begin());
        REQUIRE(format(x) == "[5, 2, 3, 2, 1]");

        x = e;
        plastic::reverse(x.begin(), x.end());
        REQUIRE(format(x) == "[]");
        x = a;
        plastic::reverse(x.begin(), x.end());
        REQUIRE(format(x) == "[9, 7, 5, 3, 1]");
        x = c;
        plastic::reverse(x.begin(), x.begin() + 3);
        REQUIRE(format(x) == "[3, 2, 1, 2, 1]");

        x = { 0, 0, 0, 0, 0 };
        plastic::reverse_copy(e.begin(), e.end(), x.begin());
        REQUIRE(format(x) == "[0, 0, 0, 0, 0]");
        plastic::reverse_copy(a.begin(), a.end(), x.begin());
        REQUIRE(format(x) == "[9, 7, 5, 3, 1]");
        plastic::reverse_copy(c.begin(), c.begin() + 3, x.begin());
        REQUIRE(format(x) == "[3, 2, 1, 3, 1]");

        x = e;
        plastic::rotate(x.begin(), x.begin(), x.end());
        REQUIRE(format(x) == "[]");
        x = a;
        plastic::rotate(x.begin(), x.begin() + 2, x.end());
        REQUIRE(format(x) == "[5, 7, 9, 1, 3]");
        x = c;
        plastic::rotate(x.begin(), x.begin() + 1, x.end());
        REQUIRE(format(x) == "[2, 3, 2, 1, 1]");

        x = { 0, 0, 0, 0, 0 };
        plastic::rotate_copy(e.begin(), e.begin(), e.end(), x.begin());
        REQUIRE(format(x) == "[0, 0, 0, 0, 0]");
        plastic::rotate_copy(a.begin(), a.begin() + 1, a.end(), x.begin());
        REQUIRE(format(x) == "[3, 5, 7, 9, 1]");
        plastic::rotate_copy(c.begin(), c.begin() + 2, c.end(), x.begin());
        REQUIRE(format(x) == "[3, 2, 1, 1, 2]");

        std::mt19937 eng{ std::random_device{}() };
        x = { 0, 0, 0, 0, 0 };
        REQUIRE(plastic::sample(e.begin(), e.end(), x.begin(), 3, eng) == x.begin());
        REQUIRE(plastic::sample(a.begin(), a.end(), x.begin(), 3, eng) == x.begin() + 3);

        x = e;
        plastic::shuffle(x.begin(), x.end(), eng);
        REQUIRE(format(e) == "[]");
        x = a;
        plastic::shuffle(x.begin(), x.end(), eng);
        REQUIRE(std::ranges::is_permutation(x, a));
        x = d;
        plastic::shuffle(x.begin(), x.end(), eng);
        REQUIRE(std::ranges::is_permutation(x, d));

        x = e;
        plastic::shift_left(x.begin(), x.end(), 2);
        REQUIRE(format(x) == "[]");
        x = a;
        plastic::shift_left(x.begin(), x.end(), 2);
        REQUIRE(format(x.begin(), x.end() - 2) == "[5, 7, 9]");
        x = c;
        plastic::shift_left(x.begin(), x.end(), 1);
        REQUIRE(format(x.begin(), x.end() - 1) == "[2, 3, 2, 1]");

        x = e;
        plastic::shift_right(x.begin(), x.end(), 1);
        REQUIRE(format(x) == "[]");
        x = a;
        plastic::shift_right(x.begin(), x.end(), 1);
        REQUIRE(format(x.begin() + 1, x.end()) == "[1, 3, 5, 7]");
        x = c;
        plastic::shift_right(x.begin(), x.end(), 2);
        REQUIRE(format(x.begin() + 2, x.end()) == "[1, 2, 3]");
    }

    TEST_CASE("sorting") {
        std::vector<int> e, a{ 1, 2, 3, 4, 5 }, b{ 3, 4, 1, 5, 2 }, c(100), x;
        std::ranges::iota(c, 1);
        std::ranges::shuffle(c, std::mt19937{ std::random_device{}() });

        x = e;
        plastic::sort(x.begin(), x.end());
        REQUIRE(format(x) == "[]");
        x = b;
        plastic::sort(x.begin(), x.end());
        REQUIRE(std::ranges::is_sorted(x));
        x = c;
        plastic::sort(x.begin(), x.end());
        REQUIRE(std::ranges::is_sorted(x));

        x = e;
        plastic::stable_sort(x.begin(), x.end());
        REQUIRE(format(x) == "[]");
        x = b;
        plastic::stable_sort(x.begin(), x.end());
        REQUIRE(std::ranges::is_sorted(x));
        x = c;
        plastic::stable_sort(x.begin(), x.end());
        REQUIRE(std::ranges::is_sorted(x));

        x = e;
        plastic::partial_sort(x.begin(), x.begin(), x.end());
        REQUIRE(format(x) == "[]");
        x = b;
        plastic::partial_sort(x.begin(), x.begin() + 3, x.end());
        REQUIRE(format(x.begin(), x.begin() + 3) == "[1, 2, 3]");
        x = b;
        plastic::partial_sort(x.begin(), x.begin() + 4, x.end());
        REQUIRE(format(x.begin(), x.begin() + 4) == "[1, 2, 3, 4]");

        x = { 0, 0, 0, 0 };
        plastic::partial_sort_copy(e.begin(), e.end(), x.begin(), x.end());
        REQUIRE(format(x) == "[0, 0, 0, 0]");
        plastic::partial_sort_copy(a.begin(), a.end(), x.begin(), x.begin() + 3);
        REQUIRE(format(x) == "[1, 2, 3, 0]");
        plastic::partial_sort_copy(b.begin(), b.end(), x.begin(), x.begin() + 4);
        REQUIRE(format(x) == "[1, 2, 3, 4]");

        REQUIRE(plastic::is_sorted(e.begin(), e.end()) == true);
        REQUIRE(plastic::is_sorted(a.begin(), a.end()) == true);
        REQUIRE(plastic::is_sorted(b.begin(), b.end()) == false);

        REQUIRE(plastic::is_sorted_until(e.begin(), e.end()) == e.end());
        REQUIRE(plastic::is_sorted_until(a.begin(), a.end()) == a.end());
        REQUIRE(plastic::is_sorted_until(b.begin(), b.end()) == b.begin() + 2);
    }

    TEST_CASE("nth_element") {
        std::vector<int> e, a{ 1, 2, 3, 4, 5 }, b{ 3, 4, 1, 5, 2 }, c(100), x;
        std::ranges::iota(c, 1);
        std::ranges::shuffle(c, std::mt19937{ std::random_device{}() });

        x = e;
        plastic::nth_element(x.begin(), x.begin(), x.end());
        REQUIRE(format(x) == "[]");
        x = b;
        plastic::nth_element(x.begin(), x.begin() + 2, x.end());
        REQUIRE(x[2] == 3);
        x = c;
        plastic::nth_element(x.begin(), x.begin() + 50, x.end());
        REQUIRE(x[50] == 51);
    }

    TEST_CASE("binary_search") {
        std::vector<int> e, a{ 2, 4, 6, 8, 10 }, b{ 1, 2, 2, 3, 3 }, c{ 5, 5, 5, 5 };

        REQUIRE(plastic::lower_bound(e.begin(), e.end(), 3) == e.end());
        REQUIRE(plastic::lower_bound(a.begin(), a.end(), 5) == a.begin() + 2);
        REQUIRE(plastic::lower_bound(b.begin(), b.end(), 2) == b.begin() + 1);
        REQUIRE(plastic::lower_bound(c.begin(), c.end(), 5) == c.begin());

        REQUIRE(plastic::upper_bound(e.begin(), e.end(), 3) == e.end());
        REQUIRE(plastic::upper_bound(a.begin(), a.end(), 5) == a.begin() + 2);
        REQUIRE(plastic::upper_bound(b.begin(), b.end(), 2) == b.begin() + 3);
        REQUIRE(plastic::upper_bound(c.begin(), c.end(), 5) == c.end());

        auto res1{ plastic::equal_range(e.begin(), e.end(), 3) };
        REQUIRE((res1.begin() == e.end() && res1.end() == e.end()));
        auto res2{ plastic::equal_range(a.begin(), a.end(), 5) };
        REQUIRE((res2.begin() == a.begin() + 2 && res2.end() == a.begin() + 2));
        auto res3{ plastic::equal_range(b.begin(), b.end(), 2) };
        REQUIRE((res3.begin() == b.begin() + 1 && res3.end() == b.begin() + 3));
        auto res4{ plastic::equal_range(c.begin(), c.end(), 5) };
        REQUIRE((res4.begin() == c.begin() && res4.end() == c.end()));

        REQUIRE(plastic::binary_search(e.begin(), e.end(), 3) == false);
        REQUIRE(plastic::binary_search(a.begin(), a.end(), 10) == true);
        REQUIRE(plastic::binary_search(b.begin(), b.end(), 2) == true);
        REQUIRE(plastic::binary_search(c.begin(), c.end(), 6) == false);
    }

    TEST_CASE("partitioning") {
        std::vector<int> e, a{ 1, 3, 5, 7, 9 }, b{ 2, 4, 6, 8, 10 }, c{ 1, 2, 3, 2, 1 }, d{ 5, 5, 5, 5 }, x, y;

        REQUIRE(plastic::is_partitioned(e.begin(), e.end(), [](int x) { return x % 2 == 0; }) == true);
        REQUIRE(plastic::is_partitioned(a.begin(), a.end(), [](int x) { return x % 2 == 1; }) == true);
        REQUIRE(plastic::is_partitioned(b.begin(), b.end(), [](int x) { return x % 4 == 2; }) == false);
        REQUIRE(plastic::is_partitioned(c.begin(), c.end(), [](int x) { return x < 3; }) == false);

        x = e;
        plastic::partition(x.begin(), x.end(), [](int x) { return x > 0; });
        REQUIRE(format(x) == "[]");
        x = a;
        plastic::partition(x.begin(), x.end(), [](int x) { return x < 5; });
        REQUIRE(std::ranges::is_partitioned(x, [](int x) { return x < 5; }));
        x = c;
        plastic::partition(x.begin(), x.end(), [](int x) { return x % 2 == 0; });
        REQUIRE(std::ranges::is_partitioned(x, [](int x) { return x % 2 == 0; }));

        x = e;
        plastic::stable_partition(x.begin(), x.end(), [](int x) { return x > 0; });
        REQUIRE(format(x) == "[]");
        x = a;
        plastic::stable_partition(x.begin(), x.end(), [](int x) { return x % 3 == 1; });
        REQUIRE(format(x) == "[1, 7, 3, 5, 9]");
        x = c;
        plastic::stable_partition(x.begin(), x.end(), [](int x) { return x % 2 == 0; });
        REQUIRE(format(x) == "[2, 2, 1, 3, 1]");

        x = y = { 0, 0, 0, 0, 0 };
        plastic::partition_copy(e.begin(), e.end(), x.begin(), y.begin(), [](int x) { return x > 0; });
        REQUIRE(format(x) == "[0, 0, 0, 0, 0]");
        REQUIRE(format(y) == "[0, 0, 0, 0, 0]");
        plastic::partition_copy(a.begin(), a.end(), x.begin(), y.begin(), [](int x) { return x > 5; });
        REQUIRE(format(x) == "[7, 9, 0, 0, 0]");
        REQUIRE(format(y) == "[1, 3, 5, 0, 0]");
        plastic::partition_copy(c.begin(), c.end(), x.begin(), y.begin(), [](int x) { return x % 2 == 1; });
        REQUIRE(format(x) == "[1, 3, 1, 0, 0]");
        REQUIRE(format(y) == "[2, 2, 5, 0, 0]");

        REQUIRE(plastic::partition_point(e.begin(), e.end(), [](int x) { return x > 0; }) == e.begin());
        REQUIRE(plastic::partition_point(a.begin(), a.end(), [](int x) { return x < 10; }) == a.end());
        REQUIRE(plastic::partition_point(b.begin(), b.end(), [](int x) { return x <= 6; }) == b.begin() + 3);
    }

    TEST_CASE("merge") {
        std::vector<int> e, a{ 1, 3, 5 }, b{ 2, 4, 6 }, c{ 1, 2, 3 }, x;

        x = { 0, 0, 0, 0, 0, 0 };
        plastic::merge(e.begin(), e.end(), e.begin(), e.end(), x.begin());
        REQUIRE(format(x) == "[0, 0, 0, 0, 0, 0]");
        plastic::merge(a.begin(), a.end(), b.begin(), b.end(), x.begin());
        REQUIRE(format(x) == "[1, 2, 3, 4, 5, 6]");
        plastic::merge(c.begin(), c.end(), c.begin(), c.end(), x.begin());
        REQUIRE(format(x) == "[1, 1, 2, 2, 3, 3]");

        x = e;
        plastic::inplace_merge(x.begin(), x.begin(), x.end());
        REQUIRE(format(x) == "[]");
        x = { 1, 3, 5, 2, 4, 6 };
        plastic::inplace_merge(x.begin(), x.begin() + 3, x.end());
        REQUIRE(format(x) == "[1, 2, 3, 4, 5, 6]");
        x = { 1, 2, 3, 1, 2, 3 };
        plastic::inplace_merge(x.begin(), x.begin() + 3, x.end());
        REQUIRE(format(x) == "[1, 1, 2, 2, 3, 3]");
    }

    TEST_CASE("set") {
        std::vector<int> e, a{ 1, 2, 3, 5, 7 }, b{ 1, 3, 4, 6, 7 }, x;

        REQUIRE(plastic::includes(e.begin(), e.end(), e.begin(), e.end()) == true);
        REQUIRE(plastic::includes(a.begin(), a.end(), e.begin(), e.end()) == true);
        REQUIRE(plastic::includes(e.begin(), e.end(), a.begin(), a.end()) == false);
        REQUIRE(plastic::includes(a.begin(), a.end(), b.begin(), b.end()) == false);
        REQUIRE(plastic::includes(a.begin(), a.end(), b.begin(), b.begin() + 2) == true);

        x = { 0, 0, 0, 0, 0, 0, 0 };
        plastic::set_union(e.begin(), e.end(), e.begin(), e.end(), x.begin());
        REQUIRE(format(x) == "[0, 0, 0, 0, 0, 0, 0]");
        plastic::set_union(a.begin(), a.end(), e.begin(), e.end(), x.begin());
        REQUIRE(format(x) == "[1, 2, 3, 5, 7, 0, 0]");
        plastic::set_union(a.begin(), a.end(), b.begin(), b.end(), x.begin());
        REQUIRE(format(x) == "[1, 2, 3, 4, 5, 6, 7]");
        plastic::set_union(b.begin(), b.end(), a.begin(), a.end(), x.begin());
        REQUIRE(format(x) == "[1, 2, 3, 4, 5, 6, 7]");

        x = { 0, 0, 0 };
        plastic::set_intersection(e.begin(), e.end(), e.begin(), e.end(), x.begin());
        REQUIRE(format(x) == "[0, 0, 0]");
        plastic::set_intersection(a.begin(), a.end(), e.begin(), e.end(), x.begin());
        REQUIRE(format(x) == "[0, 0, 0]");
        plastic::set_intersection(a.begin(), a.end(), b.begin(), b.end(), x.begin());
        REQUIRE(format(x) == "[1, 3, 7]");
        plastic::set_intersection(b.begin(), b.end(), a.begin(), a.end(), x.begin());
        REQUIRE(format(x) == "[1, 3, 7]");

        x = { 0, 0, 0, 0, 0 };
        plastic::set_difference(e.begin(), e.end(), e.begin(), e.end(), x.begin());
        REQUIRE(format(x) == "[0, 0, 0, 0, 0]");
        plastic::set_difference(a.begin(), a.end(), e.begin(), e.end(), x.begin());
        REQUIRE(format(x) == "[1, 2, 3, 5, 7]");
        plastic::set_difference(a.begin(), a.end(), b.begin(), b.end(), x.begin());
        REQUIRE(format(x) == "[2, 5, 3, 5, 7]");
        plastic::set_difference(b.begin(), b.end(), a.begin(), a.end(), x.begin());
        REQUIRE(format(x) == "[4, 6, 3, 5, 7]");

        x = { 0, 0, 0, 0, 0 };
        plastic::set_symmetric_difference(e.begin(), e.end(), e.begin(), e.end(), x.begin());
        REQUIRE(format(x) == "[0, 0, 0, 0, 0]");
        plastic::set_symmetric_difference(a.begin(), a.end(), e.begin(), e.end(), x.begin());
        REQUIRE(format(x) == "[1, 2, 3, 5, 7]");
        plastic::set_symmetric_difference(a.begin(), a.end(), b.begin(), b.end(), x.begin());
        REQUIRE(format(x) == "[2, 4, 5, 6, 7]");
        plastic::set_symmetric_difference(b.begin(), b.end(), a.begin(), a.end(), x.begin());
        REQUIRE(format(x) == "[2, 4, 5, 6, 7]");
    }

    TEST_CASE("heap") {
        std::vector<int> e, a{ 1, 2, 3, 4, 5 }, b{ 5, 4, 3, 2, 1 }, x;

        x = e;
        plastic::push_heap(x.begin(), x.end());
        REQUIRE(format(x) == "[]");
        x = { 4, 1, 2, 0, 3 };
        plastic::push_heap(x.begin(), x.end());
        REQUIRE((std::ranges::is_heap(x) && std::ranges::contains(x, 3)));
        x = { 5, 4, 3, 2, 6 };
        plastic::push_heap(x.begin(), x.end());
        REQUIRE((std::ranges::is_heap(x) && std::ranges::contains(x, 6)));

        x = e;
        plastic::pop_heap(x.begin(), x.end());
        REQUIRE(format(x) == "[]");
        x = { 4, 3, 1, 2, 0 };
        plastic::pop_heap(x.begin(), x.end());
        x.pop_back();
        REQUIRE((std::ranges::is_heap(x) && !std::ranges::contains(x, 4)));
        x = { 5, 4, 3, 2, 1 };
        plastic::pop_heap(x.begin(), x.end());
        x.pop_back();
        REQUIRE((std::ranges::is_heap(x) && !std::ranges::contains(x, 5)));

        x = e;
        plastic::make_heap(x.begin(), x.end());
        REQUIRE(format(x) == "[]");
        x = a;
        plastic::make_heap(x.begin(), x.end());
        REQUIRE(std::ranges::is_heap(x));
        x = b;
        plastic::make_heap(x.begin(), x.end());
        REQUIRE(std::ranges::is_heap(x));

        x = e;
        plastic::sort_heap(x.begin(), x.end());
        REQUIRE(format(x) == "[]");
        x = { 4, 3, 1, 2, 0 };
        plastic::sort_heap(x.begin(), x.end());
        REQUIRE(format(x) == "[0, 1, 2, 3, 4]");
        x = { 5, 4, 3, 2, 1 };
        plastic::sort_heap(x.begin(), x.end());
        REQUIRE(format(x) == "[1, 2, 3, 4, 5]");

        REQUIRE(plastic::is_heap(e.begin(), e.end()) == true);
        REQUIRE(plastic::is_heap(a.begin(), a.end()) == false);
        REQUIRE(plastic::is_heap(b.begin(), b.end()) == true);

        REQUIRE(plastic::is_heap_until(e.begin(), e.end()) == e.end());
        REQUIRE(plastic::is_heap_until(a.begin(), a.end()) == a.begin() + 1);
        REQUIRE(plastic::is_heap_until(b.begin(), b.end()) == b.end());
    }

    TEST_CASE("minimun_and_maximum") {
        std::vector<int> e, a{ 1, 3, 5, 7, 9 }, b{ 10, 8, 6, 4, 2 };

        REQUIRE(plastic::min(0, 0) == 0);
        REQUIRE(plastic::min(5, 1) == 1);
        REQUIRE(plastic::min({ 0, 0, 0, 0, 0 }) == 0);
        REQUIRE(plastic::min({ 5, 3, 2, 4, 1 }) == 1);

        REQUIRE(plastic::max(0, 0) == 0);
        REQUIRE(plastic::max(5, 1) == 5);
        REQUIRE(plastic::max({ 0, 0, 0, 0, 0 }) == 0);
        REQUIRE(plastic::max({ 5, 3, 2, 4, 1 }) == 5);

        auto res1{ plastic::minmax(0, 0) };
        REQUIRE((res1.min == 0 && res1.max == 0));
        auto res2{ plastic::minmax(5, 1) };
        REQUIRE((res2.min == 1 && res2.max == 5));
        auto res3{ plastic::minmax({ 0, 0, 0, 0, 0 }) };
        REQUIRE((res3.min == 0 && res3.max == 0));
        auto res4{ plastic::minmax({ 5, 3, 2, 4, 1 }) };
        REQUIRE((res4.min == 1 && res4.max == 5));

        REQUIRE(plastic::min_element(e.begin(), e.end()) == e.end());
        REQUIRE(plastic::min_element(a.begin(), a.end()) == a.begin());
        REQUIRE(plastic::min_element(b.begin(), b.end()) == b.end() - 1);

        REQUIRE(plastic::max_element(e.begin(), e.end()) == e.end());
        REQUIRE(plastic::max_element(a.begin(), a.end()) == a.end() - 1);
        REQUIRE(plastic::max_element(b.begin(), b.end()) == b.begin());

        auto res5{ plastic::minmax_element(e.begin(), e.end()) };
        REQUIRE((res5.min == e.end() && res5.max == e.end()));
        auto res6{ plastic::minmax_element(a.begin(), a.end()) };
        REQUIRE((res6.min == a.begin() && res6.max == a.end() - 1));
        auto res7{ plastic::minmax_element(b.begin(), b.end()) };
        REQUIRE((res7.min == b.end() - 1 && res7.max == b.begin()));
    }

    TEST_CASE("bounded_value") {
        REQUIRE(plastic::clamp(0, 1, 5) == 1);
        REQUIRE(plastic::clamp(3, 1, 5) == 3);
        REQUIRE(plastic::clamp(6, 1, 5) == 5);
        REQUIRE(plastic::clamp(2, 2, 2) == 2);
    }

    TEST_CASE("lexicographical_comparison") {
        std::vector<int> e, a{ 1, 2, 3, 4, 5 }, b{ 2, 4, 6, 8, 10 };

        REQUIRE(plastic::lexicographical_compare(e.begin(), e.end(), a.begin(), a.end()) == true);
        REQUIRE(plastic::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end()) == true);
        REQUIRE(plastic::lexicographical_compare(b.begin(), b.end(), a.begin(), a.end()) == false);
        REQUIRE(
            plastic::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end(), {}, [](int x) { return x * 3; })
            == false
        );
    }

    TEST_CASE("permutation") {
        std::string e, a{ "abc" }, b{ "cba" }, c{ "aab" }, d{ "abcd" };

        plastic::next_permutation(a.begin(), a.end());
        REQUIRE(a == "acb");
        plastic::next_permutation(a.begin(), a.end());
        REQUIRE(a == "bac");
        plastic::next_permutation(a.begin(), a.end());
        REQUIRE(a == "bca");
        plastic::next_permutation(a.begin(), a.end());
        REQUIRE(a == "cab");
        plastic::next_permutation(a.begin(), a.end());
        REQUIRE(a == "cba");
        plastic::next_permutation(a.begin(), a.end());
        REQUIRE(a == "abc");

        plastic::prev_permutation(a.begin(), a.end());
        REQUIRE(a == "cba");
        plastic::prev_permutation(a.begin(), a.end());
        REQUIRE(a == "cab");
        plastic::prev_permutation(a.begin(), a.end());
        REQUIRE(a == "bca");
        plastic::prev_permutation(a.begin(), a.end());
        REQUIRE(a == "bac");
        plastic::prev_permutation(a.begin(), a.end());
        REQUIRE(a == "acb");
        plastic::prev_permutation(a.begin(), a.end());
        REQUIRE(a == "abc");
    }
}
