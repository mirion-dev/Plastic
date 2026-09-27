#include <doctest/doctest.h>

import std;
import plastic;
import utils;

TEST_SUITE("algorithm") {
    TEST_CASE("non_modifying_sequence") {
        std::vector<int> e, a{ 1, 3, 5, 7, 9 }, b{ 2, 4, 6, 8, 10 }, c{ 1, 2, 3, 2, 1 }, d{ 5, 5, 5, 5 };

        SUBCASE("all_of") {
            CHECK(plastic::all_of(e.begin(), e.end(), [](int x) { return x % 2 == 0; }) == true);
            CHECK(plastic::all_of(a.begin(), a.end(), [](int x) { return x % 2 == 1; }) == true);
            CHECK(plastic::all_of(b.begin(), b.end(), [](int x) { return x % 2 == 1; }) == false);
            CHECK(plastic::all_of(c.begin(), c.end(), [](int x) { return x % 2 == 1; }) == false);
        }

        SUBCASE("any_of") {
            CHECK(plastic::any_of(e.begin(), e.end(), [](int x) { return x > 0; }) == false);
            CHECK(plastic::any_of(a.begin(), a.end(), [](int x) { return x > 5; }) == true);
            CHECK(plastic::any_of(b.begin(), b.end(), [](int x) { return x % 2 == 0; }) == true);
            CHECK(plastic::any_of(c.begin(), c.end(), [](int x) { return x == 4; }) == false);
        }

        SUBCASE("none_of") {
            CHECK(plastic::none_of(e.begin(), e.end(), [](int x) { return x < 0; }) == true);
            CHECK(plastic::none_of(a.begin(), a.end(), [](int x) { return x % 2 == 0; }) == true);
            CHECK(plastic::none_of(b.begin(), b.end(), [](int x) { return x > 2; }) == false);
            CHECK(plastic::none_of(c.begin(), c.end(), [](int x) { return x == 2; }) == false);
        }

        SUBCASE("contains") {
            CHECK(plastic::contains(e.begin(), e.end(), 3) == false);
            CHECK(plastic::contains(a.begin(), a.end(), 3) == true);
            CHECK(plastic::contains(b.begin(), b.end(), 3) == false);
            CHECK(plastic::contains(c.begin(), c.end(), 3) == true);
        }

        SUBCASE("contains_subrange") {
            std::vector x{ 3, 2, 1 };
            CHECK(plastic::contains_subrange(e.begin(), e.end(), x.begin(), x.end()) == false);
            CHECK(plastic::contains_subrange(a.begin(), a.end(), e.begin(), e.end()) == true);
            CHECK(plastic::contains_subrange(a.begin(), a.end(), x.begin(), x.end()) == false);
            CHECK(plastic::contains_subrange(b.begin(), b.end(), x.begin(), x.end()) == false);
            CHECK(plastic::contains_subrange(c.begin(), c.end(), x.begin(), x.end()) == true);
        }

        SUBCASE("for_each") {
            std::vector x{ 1, 2, 3 };
            plastic::for_each(e.begin(), e.end(), [](int& x) { x += 10; });
            CHECK(format(e) == "[]");
            plastic::for_each(x.begin(), x.end(), [](int& x) { x += 10; });
            CHECK(format(x) == "[11, 12, 13]");
        }

        SUBCASE("for_each_n") {
            std::vector x{ 1, 2, 3, 4, 5 };
            plastic::for_each_n(e.begin(), 0, [](int& x) { x += 10; });
            CHECK(format(e) == "[]");
            plastic::for_each_n(x.begin(), 3, [](int& x) { x *= 2; });
            CHECK(format(x) == "[2, 4, 6, 4, 5]");
        }

        SUBCASE("find") {
            CHECK(plastic::find(e.begin(), e.end(), 0) == e.end());
            CHECK(plastic::find(a.begin(), a.end(), 7) == a.begin() + 3);
            CHECK(plastic::find(b.begin(), b.end(), 5) == b.end());
        }

        SUBCASE("find_if") {
            CHECK(plastic::find_if(e.begin(), e.end(), [](int x) { return x == 0; }) == e.end());
            CHECK(plastic::find_if(a.begin(), a.end(), [](int x) { return x < 0; }) == a.end());
            CHECK(plastic::find_if(b.begin(), b.end(), [](int x) { return x > 5; }) == b.begin() + 2);
        }

        SUBCASE("find_if_not") {
            CHECK(plastic::find_if_not(e.begin(), e.end(), [](int x) { return x != 0; }) == e.end());
            CHECK(plastic::find_if_not(c.begin(), c.end(), [](int x) { return x < 3; }) == c.begin() + 2);
            CHECK(plastic::find_if_not(d.begin(), d.end(), [](int x) { return x == 5; }) == d.end());
        }

        SUBCASE("find_last") {
            CHECK(plastic::find_last(e.begin(), e.end(), 0).begin() == e.end());
            CHECK(plastic::find_last(b.begin(), b.end(), 5).begin() == b.end());
            CHECK(plastic::find_last(c.begin(), c.end(), 2).begin() == c.begin() + 3);
        }

        SUBCASE("find_last_if") {
            CHECK(plastic::find_last_if(e.begin(), e.end(), [](int x) { return x == 0; }).begin() == e.end());
            CHECK(plastic::find_last_if(a.begin(), a.end(), [](int x) { return x % 2 == 0; }).begin() == a.end());
            CHECK(plastic::find_last_if(c.begin(), c.end(), [](int x) { return x < 3; }).begin() == c.begin() + 4);
        }

        SUBCASE("find_last_if_not") {
            CHECK(plastic::find_last_if_not(e.begin(), e.end(), [](int x) { return x != 0; }).begin() == e.end());
            CHECK(plastic::find_last_if_not(c.begin(), c.end(), [](int x) { return x == 1; }).begin() == c.begin() + 3);
            CHECK(plastic::find_last_if_not(d.begin(), d.end(), [](int x) { return x == 5; }).begin() == d.end());
        }

        SUBCASE("find_end") {
            std::vector x{ 5, 5 };
            CHECK(plastic::find_end(a.begin(), a.end(), e.begin(), e.end()).begin() == a.end());
            CHECK(plastic::find_end(e.begin(), e.end(), a.begin(), a.end()).begin() == e.end());
            CHECK(plastic::find_end(c.begin(), c.end(), x.begin(), x.end()).begin() == c.end());
            CHECK(plastic::find_end(d.begin(), d.end(), x.begin(), x.end()).begin() == d.begin() + 2);
        }

        SUBCASE("find_first_of") {
            std::vector x{ 0, 10 };
            CHECK(plastic::find_first_of(a.begin(), a.end(), e.begin(), e.end()) == a.end());
            CHECK(plastic::find_first_of(e.begin(), e.end(), a.begin(), a.end()) == e.end());
            CHECK(plastic::find_first_of(a.begin(), a.end(), x.begin(), x.end()) == a.end());
            CHECK(plastic::find_first_of(b.begin(), b.end(), x.begin(), x.end()) == b.begin() + 4);
        }

        SUBCASE("adjacent_find") {
            CHECK(plastic::adjacent_find(e.begin(), e.end()) == e.end());
            CHECK(plastic::adjacent_find(a.begin(), a.end()) == a.end());
            CHECK(plastic::adjacent_find(d.begin(), d.end()) == d.begin());
        }

        SUBCASE("count") {
            CHECK(plastic::count(e.begin(), e.end(), 0) == 0);
            CHECK(plastic::count(c.begin(), c.end(), 2) == 2);
            CHECK(plastic::count(d.begin(), d.end(), 5) == 4);
        }

        SUBCASE("count_if") {
            CHECK(plastic::count_if(e.begin(), e.end(), [](int x) { return x == 0; }) == 0);
            CHECK(plastic::count_if(a.begin(), a.end(), [](int x) { return x > 5; }) == 2);
            CHECK(plastic::count_if(b.begin(), b.end(), [](int x) { return x % 4 == 2; }) == 3);
        }

        SUBCASE("mismatch") {
            CHECK(plastic::mismatch(e.begin(), e.end(), e.begin(), e.end()).in1 == e.begin());
            CHECK(plastic::mismatch(a.begin(), a.end(), c.begin(), c.end()).in1 == a.begin() + 1);
            CHECK(plastic::mismatch(b.begin(), b.end(), b.begin(), b.end()).in2 == b.end());
        }

        SUBCASE("equal") {
            CHECK(plastic::equal(e.begin(), e.end(), e.begin(), e.end()) == true);
            CHECK(plastic::equal(e.begin(), e.end(), a.begin(), a.end()) == false);
            CHECK(plastic::equal(a.begin(), a.end(), b.begin(), b.end()) == false);
            CHECK(plastic::equal(a.begin(), a.end(), b.begin(), b.end(), {}, [](int x) { return x + 1; }) == true);
        }

        SUBCASE("is_permutation") {
            std::vector<int> x;
            CHECK(plastic::is_permutation(e.begin(), e.end(), e.begin(), e.end()) == true);
            x = { 9, 7, 5, 3, 1 };
            CHECK(plastic::is_permutation(a.begin(), a.end(), x.begin(), x.end()) == true);
            x = { 1, 1, 3, 5, 7 };
            CHECK(plastic::is_permutation(a.begin(), a.end(), x.begin(), x.end()) == false);
            x = { 1, 3, 5, 7, 9, 11 };
            CHECK(plastic::is_permutation(a.begin(), a.end(), x.begin(), x.end()) == false);
        }

        SUBCASE("search") {
            std::vector x{ 3, 5, 7 };
            CHECK(plastic::search(a.begin(), a.end(), e.begin(), e.end()).begin() == a.begin());
            CHECK(plastic::search(e.begin(), e.end(), a.begin(), a.end()).begin() == e.end());
            CHECK(plastic::search(a.begin(), a.end(), x.begin(), x.end()).begin() == a.begin() + 1);
            CHECK(plastic::search(b.begin(), b.end(), x.begin(), x.end()).begin() == b.end());
        }

        SUBCASE("search_n") {
            CHECK(plastic::search_n(e.begin(), e.end(), 0, 10).begin() == e.begin());
            CHECK(plastic::search_n(e.begin(), e.end(), 10, 0).begin() == e.end());
            CHECK(plastic::search_n(c.begin(), c.end(), 2, 1, std::ranges::greater{}).begin() == c.begin() + 1);
            CHECK(plastic::search_n(d.begin(), d.end(), 4, 5).begin() == d.begin());
        }

        SUBCASE("starts_with") {
            std::vector x{ 1, 3, 5 };
            CHECK(plastic::starts_with(e.begin(), e.end(), x.begin(), x.end()) == false);
            CHECK(plastic::starts_with(a.begin(), a.end(), e.begin(), e.end()) == true);
            CHECK(plastic::starts_with(a.begin(), a.end(), x.begin(), x.end()) == true);
            CHECK(plastic::starts_with(b.begin(), b.end(), x.begin(), x.end()) == false);
            CHECK(plastic::starts_with(c.begin(), c.end(), x.begin(), x.end()) == false);
        }

        SUBCASE("ends_with") {
            std::vector x{ 3, 2, 1 };
            CHECK(plastic::ends_with(e.begin(), e.end(), x.begin(), x.end()) == false);
            CHECK(plastic::ends_with(a.begin(), a.end(), e.begin(), e.end()) == true);
            CHECK(plastic::ends_with(a.begin(), a.end(), x.begin(), x.end()) == false);
            CHECK(plastic::ends_with(b.begin(), b.end(), x.begin(), x.end()) == false);
            CHECK(plastic::ends_with(c.begin(), c.end(), x.begin(), x.end()) == true);
        }

        SUBCASE("fold_left") {
            CHECK(plastic::fold_left(e.begin(), e.end(), 10, std::plus{}) == 10);
            CHECK(plastic::fold_left(a.begin(), a.end(), 0, std::plus{}) == 25);
            CHECK(plastic::fold_left(a.begin(), a.end(), 10, std::minus{}) == -15);
            CHECK(plastic::fold_left(b.begin(), b.end(), 1, std::multiplies{}) == 3840);
        }

        SUBCASE("fold_left_first") {
            CHECK(!plastic::fold_left_first(e.begin(), e.end(), std::plus{}));
            CHECK(*plastic::fold_left_first(a.begin(), a.end(), std::plus{}) == 25);
            CHECK(*plastic::fold_left_first(b.begin(), b.end(), std::multiplies{}) == 3840);
        }

        SUBCASE("fold_right") {
            CHECK(plastic::fold_right(e.begin(), e.end(), 100, std::plus{}) == 100);
            CHECK(plastic::fold_right(a.begin(), a.end(), 0, std::plus{}) == 25);
            CHECK(plastic::fold_right(b.begin(), b.end(), 2, std::minus{}) == 4);
        }

        SUBCASE("fold_right_last") {
            CHECK(!plastic::fold_right_last(e.begin(), e.end(), std::plus{}));
            CHECK(*plastic::fold_right_last(a.begin(), a.end(), std::plus{}) == 25);
            CHECK(*plastic::fold_right_last(b.begin(), b.end(), std::minus{}) == 6);
        }

        SUBCASE("fold_left_with_iter") {
            auto res1{ plastic::fold_left_with_iter(a.begin(), a.end(), 0, std::plus{}) };
            CHECK((res1.value == 25 && res1.in == a.end()));
            auto res2{ plastic::fold_left_with_iter(a.begin(), a.begin() + 3, 10, std::multiplies{}) };
            CHECK((res2.value == 150 && res2.in == a.begin() + 3));
        }

        SUBCASE("fold_left_first_with_iter") {
            auto res1{ plastic::fold_left_first_with_iter(e.begin(), e.end(), std::plus{}) };
            CHECK((!res1.value && res1.in == e.end()));
            auto res2{ plastic::fold_left_first_with_iter(b.begin(), b.end(), std::multiplies{}) };
            CHECK((*res2.value == 3840 && res2.in == b.end()));
        }
    }

    TEST_CASE("mutating_sequence") {
        std::vector<int> e, a{ 1, 3, 5, 7, 9 }, b{ 2, 4, 6, 8, 10 }, c{ 1, 2, 3, 2, 1 }, d{ 5, 5, 5, 5 };

        SUBCASE("copy") {
            std::vector x{ 0, 0, 0, 0, 0 };
            plastic::copy(e.begin(), e.end(), x.begin());
            CHECK(format(x) == "[0, 0, 0, 0, 0]");
            plastic::copy(a.begin(), a.end(), x.begin());
            CHECK(format(x) == "[1, 3, 5, 7, 9]");
            plastic::copy(d.begin(), d.end(), x.begin());
            CHECK(format(x) == "[5, 5, 5, 5, 9]");
        }

        SUBCASE("copy_n") {
            std::vector x{ 0, 0, 0, 0, 0 };
            plastic::copy_n(e.begin(), 0, x.begin());
            CHECK(format(x) == "[0, 0, 0, 0, 0]");
            plastic::copy_n(a.begin(), 3, x.begin());
            CHECK(format(x) == "[1, 3, 5, 0, 0]");
            plastic::copy_n(b.begin(), 4, x.begin());
            CHECK(format(x) == "[2, 4, 6, 8, 0]");
        }

        SUBCASE("copy_if") {
            std::vector x{ 0, 0, 0, 0, 0 };
            plastic::copy_if(e.begin(), e.end(), x.begin(), [](int x) { return x > 0; });
            CHECK(format(x) == "[0, 0, 0, 0, 0]");
            plastic::copy_if(a.begin(), a.end(), x.begin(), [](int x) { return x > 3; });
            CHECK(format(x) == "[5, 7, 9, 0, 0]");
            plastic::copy_if(d.begin(), d.end(), x.begin(), [](int x) { return x <= 5; });
            CHECK(format(x) == "[5, 5, 5, 5, 0]");
        }

        SUBCASE("copy_backward") {
            std::vector x{ 0, 0, 0, 0, 0 };
            plastic::copy_backward(e.begin(), e.end(), x.end());
            CHECK(format(x) == "[0, 0, 0, 0, 0]");
            plastic::copy_backward(a.begin(), a.end(), x.end());
            CHECK(format(x) == "[1, 3, 5, 7, 9]");
            plastic::copy_backward(d.begin(), d.end(), x.end());
            CHECK(format(x) == "[1, 5, 5, 5, 5]");
        }

        SUBCASE("move") {
            std::vector x{ 0, 0, 0, 0, 0 }, y{ 1, 2, 3, 4, 5 };
            plastic::move(e.begin(), e.end(), x.begin());
            CHECK(format(x) == "[0, 0, 0, 0, 0]");
            plastic::move(y.begin(), y.end(), x.begin());
            CHECK(format(x) == "[1, 2, 3, 4, 5]");
        }

        SUBCASE("move_backward") {
            std::vector x{ 0, 0, 0, 0, 0 }, y{ 1, 2, 3, 4, 5 };
            plastic::move_backward(e.begin(), e.end(), x.end());
            CHECK(format(x) == "[0, 0, 0, 0, 0]");
            plastic::move_backward(y.begin(), y.end(), x.end());
            CHECK(format(x) == "[1, 2, 3, 4, 5]");
        }

        SUBCASE("swap_ranges") {
            std::vector<int> x{ 0, 0, 0, 0, 0 }, y;
            plastic::swap_ranges(x.begin(), x.end(), y.begin(), y.end());
            CHECK(format(x) == "[0, 0, 0, 0, 0]");
            CHECK(format(y) == "[]");
            y = b;
            plastic::swap_ranges(x.begin(), x.end(), y.begin(), y.end());
            CHECK(format(x) == "[2, 4, 6, 8, 10]");
            CHECK(format(y) == "[0, 0, 0, 0, 0]");
            y = d;
            plastic::swap_ranges(x.begin(), x.end(), y.begin(), y.end());
            CHECK(format(x) == "[5, 5, 5, 5, 10]");
            CHECK(format(y) == "[2, 4, 6, 8]");
        }

        SUBCASE("transform") {
            std::vector x{ 0, 0, 0, 0, 0 };
            plastic::transform(e.begin(), e.end(), x.begin(), [](int x) { return x * 2; });
            CHECK(format(x) == "[0, 0, 0, 0, 0]");
            plastic::transform(a.begin(), a.end(), x.begin(), [](int x) { return x * 2; });
            CHECK(format(x) == "[2, 6, 10, 14, 18]");
            plastic::transform(d.begin(), d.end(), x.begin(), [](int x) { return x + 1; });
            CHECK(format(x) == "[6, 6, 6, 6, 18]");
            plastic::transform(e.begin(), e.end(), a.begin(), a.end(), x.begin(), [](int x, int y) { return x + y; });
            CHECK(format(x) == "[6, 6, 6, 6, 18]");
            plastic::transform(a.begin(), a.end(), b.begin(), b.end(), x.begin(), [](int x, int y) { return y - x; });
            CHECK(format(x) == "[1, 1, 1, 1, 1]");
            plastic::transform(a.begin(), a.end(), d.begin(), d.end(), x.begin(), [](int x, int y) {
                return y * 2 - x - 1;
            });
            CHECK(format(x) == "[8, 6, 4, 2, 1]");
        }

        SUBCASE("replace") {
            std::vector<int> x;
            plastic::replace(x.begin(), x.end(), 2, 9);
            CHECK(format(x) == "[]");
            x = c;
            plastic::replace(x.begin(), x.end(), 2, 9);
            CHECK(format(x) == "[1, 9, 3, 9, 1]");
            x = d;
            plastic::replace(x.begin(), x.end(), 5, 0);
            CHECK(format(x) == "[0, 0, 0, 0]");
        }

        SUBCASE("replace_if") {
            std::vector<int> x;
            plastic::replace_if(x.begin(), x.end(), [](int x) { return x > 0; }, 0);
            CHECK(format(x) == "[]");
            x = a;
            plastic::replace_if(x.begin(), x.end(), [](int x) { return x % 3 == 0; }, 0);
            CHECK(format(x) == "[1, 0, 5, 7, 0]");
            x = b;
            plastic::replace_if(x.begin(), x.end(), [](int x) { return x < 6; }, 1);
            CHECK(format(x) == "[1, 1, 6, 8, 10]");
        }

        SUBCASE("replace_copy") {
            std::vector x{ 0, 0, 0, 0, 0 };
            plastic::replace_copy(e.begin(), e.end(), x.begin(), 2, 8);
            CHECK(format(x) == "[0, 0, 0, 0, 0]");
            plastic::replace_copy(c.begin(), c.end(), x.begin(), 2, 8);
            CHECK(format(x) == "[1, 8, 3, 8, 1]");
            plastic::replace_copy(d.begin(), d.end(), x.begin(), 5, 2);
            CHECK(format(x) == "[2, 2, 2, 2, 1]");
        }

        SUBCASE("replace_copy_if") {
            std::vector x{ 0, 0, 0, 0, 0 };
            plastic::replace_copy_if(e.begin(), e.end(), x.begin(), [](int x) { return x > 0; }, 0);
            CHECK(format(x) == "[0, 0, 0, 0, 0]");
            plastic::replace_copy_if(a.begin(), a.end(), x.begin(), [](int x) { return x > 5; }, 1);
            CHECK(format(x) == "[1, 3, 5, 1, 1]");
            plastic::replace_copy_if(b.begin(), b.end(), x.begin(), [](int x) { return x % 2 == 0; }, 7);
            CHECK(format(x) == "[7, 7, 7, 7, 7]");
        }

        SUBCASE("fill") {
            std::vector x{ 0, 0, 0, 0, 0 };
            plastic::fill(e.begin(), e.end(), 3);
            CHECK(format(e) == "[]");
            plastic::fill(x.begin(), x.end(), 3);
            CHECK(format(x) == "[3, 3, 3, 3, 3]");
        }

        SUBCASE("fill_n") {
            std::vector x{ 0, 0, 0, 0, 0 };
            plastic::fill_n(e.begin(), 0, 7);
            CHECK(format(e) == "[]");
            plastic::fill_n(x.begin(), 3, 7);
            CHECK(format(x) == "[7, 7, 7, 0, 0]");
        }

        SUBCASE("generate") {
            std::vector x{ 0, 0, 0, 0, 0 };
            plastic::generate(x.begin(), x.end(), [v{ 0 }] mutable { return v += 2; });
            CHECK(format(x) == "[2, 4, 6, 8, 10]");
            plastic::generate(x.begin(), x.end(), [i{ a.begin() }] mutable { return *i++; });
            CHECK(format(x) == "[1, 3, 5, 7, 9]");
        }

        SUBCASE("generate_n") {
            std::vector x{ 0, 0, 0, 0, 0 };
            plastic::generate_n(x.begin(), 4, [v{ 0 }] mutable { return v += 2; });
            CHECK(format(x) == "[2, 4, 6, 8, 0]");
            plastic::generate_n(x.begin(), 5, [i{ a.begin() }] mutable { return *i++; });
            CHECK(format(x) == "[1, 3, 5, 7, 9]");
        }

        SUBCASE("remove") {
            std::vector x{ 1, 2, 2, 2, 5 };
            x.erase(plastic::remove(x.begin(), x.end(), 2).begin(), x.end());
            REQUIRE(format(x) == "[1, 5]");
            x.erase(plastic::remove(x.begin(), x.end(), 1).begin(), x.end());
            REQUIRE(format(x) == "[5]");
            x.erase(plastic::remove(x.begin(), x.end(), 5).begin(), x.end());
            REQUIRE(format(x) == "[]");
            x.erase(plastic::remove(x.begin(), x.end(), 0).begin(), x.end());
            REQUIRE(format(x) == "[]");
        }

        SUBCASE("remove_if") {
            std::vector x{ 1, 2, 3, 2, 1 };
            x.erase(plastic::remove_if(x.begin(), x.end(), [](int x) { return x % 2 == 0; }).begin(), x.end());
            REQUIRE(format(x) == "[1, 3, 1]");
            x.erase(plastic::remove_if(x.begin(), x.end(), [](int x) { return x >= 2; }).begin(), x.end());
            REQUIRE(format(x) == "[1, 1]");
            x.erase(plastic::remove_if(x.begin(), x.end(), [](int x) { return x == 1; }).begin(), x.end());
            REQUIRE(format(x) == "[]");
            x.erase(plastic::remove_if(x.begin(), x.end(), [](int x) { return true; }).begin(), x.end());
            REQUIRE(format(x) == "[]");
        }

        SUBCASE("remove_copy") {
            std::vector x{ 0, 0, 0, 0, 0 };
            plastic::remove_copy(e.begin(), e.end(), x.begin(), 2);
            CHECK(format(x) == "[0, 0, 0, 0, 0]");
            plastic::remove_copy(c.begin(), c.end(), x.begin(), 2);
            CHECK(format(x) == "[1, 3, 1, 0, 0]");
            plastic::remove_copy(d.begin(), d.end(), x.begin(), 5);
            CHECK(format(x) == "[1, 3, 1, 0, 0]");
        }

        SUBCASE("remove_copy_if") {
            std::vector x{ 0, 0, 0, 0, 0 };
            plastic::remove_copy_if(e.begin(), e.end(), x.begin(), [](int x) { return x > 0; });
            CHECK(format(x) == "[0, 0, 0, 0, 0]");
            plastic::remove_copy_if(a.begin(), a.end(), x.begin(), [](int x) { return x < 5; });
            CHECK(format(x) == "[5, 7, 9, 0, 0]");
            plastic::remove_copy_if(b.begin(), b.end(), x.begin(), [](int x) { return x > 5; });
            CHECK(format(x) == "[2, 4, 9, 0, 0]");
        }

        SUBCASE("unique") {
            std::vector<int> x;
            x.erase(plastic::unique(x.begin(), x.end()).begin(), x.end());
            CHECK(format(x) == "[]");
            x = d;
            x.erase(plastic::unique(x.begin(), x.end()).begin(), x.end());
            CHECK(format(x) == "[5]");
            x = { 1, 1, 2, 2, 3 };
            x.erase(plastic::unique(x.begin(), x.end()).begin(), x.end());
            CHECK(format(x) == "[1, 2, 3]");
        }

        SUBCASE("unique_copy") {
            std::vector x{ 0, 0, 0, 0, 0 };
            plastic::unique_copy(e.begin(), e.end(), x.begin());
            CHECK(format(x) == "[0, 0, 0, 0, 0]");
            plastic::unique_copy(c.begin(), c.end(), x.begin());
            CHECK(format(x) == "[1, 2, 3, 2, 1]");
            plastic::unique_copy(d.begin(), d.end(), x.begin());
            CHECK(format(x) == "[5, 2, 3, 2, 1]");
        }

        SUBCASE("reverse") {
            std::vector<int> x;
            plastic::reverse(x.begin(), x.end());
            CHECK(format(x) == "[]");
            x = a;
            plastic::reverse(x.begin(), x.end());
            CHECK(format(x) == "[9, 7, 5, 3, 1]");
            x = c;
            plastic::reverse(x.begin(), x.begin() + 3);
            CHECK(format(x) == "[3, 2, 1, 2, 1]");
        }

        SUBCASE("reverse_copy") {
            std::vector x{ 0, 0, 0, 0, 0 };
            plastic::reverse_copy(e.begin(), e.end(), x.begin());
            CHECK(format(x) == "[0, 0, 0, 0, 0]");
            plastic::reverse_copy(a.begin(), a.end(), x.begin());
            CHECK(format(x) == "[9, 7, 5, 3, 1]");
            plastic::reverse_copy(c.begin(), c.begin() + 3, x.begin());
            CHECK(format(x) == "[3, 2, 1, 3, 1]");
        }

        SUBCASE("rotate") {
            std::vector<int> x;
            plastic::rotate(x.begin(), x.begin(), x.end());
            CHECK(format(x) == "[]");
            x = a;
            plastic::rotate(x.begin(), x.begin() + 2, x.end());
            CHECK(format(x) == "[5, 7, 9, 1, 3]");
            x = c;
            plastic::rotate(x.begin(), x.begin() + 1, x.end());
            CHECK(format(x) == "[2, 3, 2, 1, 1]");
        }

        SUBCASE("rotate_copy") {
            std::vector x{ 0, 0, 0, 0, 0 };
            plastic::rotate_copy(e.begin(), e.begin(), e.end(), x.begin());
            CHECK(format(x) == "[0, 0, 0, 0, 0]");
            plastic::rotate_copy(a.begin(), a.begin() + 1, a.end(), x.begin());
            CHECK(format(x) == "[3, 5, 7, 9, 1]");
            plastic::rotate_copy(c.begin(), c.begin() + 2, c.end(), x.begin());
            CHECK(format(x) == "[3, 2, 1, 1, 2]");
        }

        SUBCASE("sample") {
            std::mt19937 eng{ std::random_device{}() };
            std::vector x{ 0, 0, 0, 0, 0 };
            CHECK(plastic::sample(e.begin(), e.end(), x.begin(), 3, eng) == x.begin());
            CHECK(plastic::sample(a.begin(), a.end(), x.begin(), 3, eng) == x.begin() + 3);
        }

        SUBCASE("shuffle") {
            std::mt19937 eng{ std::random_device{}() };
            std::vector<int> x;
            plastic::shuffle(x.begin(), x.end(), eng);
            CHECK(format(e) == "[]");
            x = a;
            plastic::shuffle(x.begin(), x.end(), eng);
            REQUIRE(std::ranges::is_permutation(x, a));
            x = d;
            plastic::shuffle(x.begin(), x.end(), eng);
            REQUIRE(std::ranges::is_permutation(x, d));
        }

        SUBCASE("shift_left") {
            std::vector<int> x;
            plastic::shift_left(x.begin(), x.end(), 2);
            REQUIRE(format(x) == "[]");
            x = a;
            plastic::shift_left(x.begin(), x.end(), 2);
            CHECK(format(x.begin(), x.end() - 2) == "[5, 7, 9]");
            x = c;
            plastic::shift_left(x.begin(), x.end(), 1);
            CHECK(format(x.begin(), x.end() - 1) == "[2, 3, 2, 1]");
        }

        SUBCASE("shift_right") {
            std::vector<int> x;
            plastic::shift_right(x.begin(), x.end(), 1);
            CHECK(format(x) == "[]");
            x = a;
            plastic::shift_right(x.begin(), x.end(), 1);
            CHECK(format(x.begin() + 1, x.end()) == "[1, 3, 5, 7]");
            x = c;
            plastic::shift_right(x.begin(), x.end(), 2);
            CHECK(format(x.begin() + 2, x.end()) == "[1, 2, 3]");
        }
    }

    TEST_CASE("sorting") {
        std::vector<int> e, a{ 1, 2, 3, 4, 5 }, b{ 3, 4, 1, 5, 2 }, c(100);
        std::ranges::iota(c, 1);
        std::ranges::shuffle(c, std::mt19937{ std::random_device{}() });

        SUBCASE("sort") {
            std::vector<int> x;
            plastic::sort(x.begin(), x.end());
            CHECK(format(x) == "[]");
            x = b;
            plastic::sort(x.begin(), x.end());
            CHECK(std::ranges::is_sorted(x));
            x = c;
            plastic::sort(x.begin(), x.end());
            CHECK(std::ranges::is_sorted(x));
        }

        SUBCASE("stable_sort") {
            std::vector<int> x;
            plastic::stable_sort(x.begin(), x.end());
            CHECK(format(x) == "[]");
            x = b;
            plastic::stable_sort(x.begin(), x.end());
            CHECK(std::ranges::is_sorted(x));
            x = c;
            plastic::stable_sort(x.begin(), x.end());
            CHECK(std::ranges::is_sorted(x));
        }

        SUBCASE("partial_sort") {
            std::vector<int> x;
            plastic::partial_sort(x.begin(), x.begin(), x.end());
            CHECK(format(x) == "[]");
            x = b;
            plastic::partial_sort(x.begin(), x.begin() + 3, x.end());
            CHECK(format(x.begin(), x.begin() + 3) == "[1, 2, 3]");
            x = b;
            plastic::partial_sort(x.begin(), x.begin() + 4, x.end());
            CHECK(format(x.begin(), x.begin() + 4) == "[1, 2, 3, 4]");
        }

        SUBCASE("partial_sort_copy") {
            std::vector x{ 0, 0, 0, 0 };
            plastic::partial_sort_copy(e.begin(), e.end(), x.begin(), x.end());
            CHECK(format(x) == "[0, 0, 0, 0]");
            plastic::partial_sort_copy(a.begin(), a.end(), x.begin(), x.begin() + 3);
            CHECK(format(x) == "[1, 2, 3, 0]");
            plastic::partial_sort_copy(b.begin(), b.end(), x.begin(), x.begin() + 4);
            CHECK(format(x) == "[1, 2, 3, 4]");
        }

        SUBCASE("is_sorted") {
            CHECK(plastic::is_sorted(e.begin(), e.end()) == true);
            CHECK(plastic::is_sorted(a.begin(), a.end()) == true);
            CHECK(plastic::is_sorted(b.begin(), b.end()) == false);
        }

        SUBCASE("is_sorted_until") {
            CHECK(plastic::is_sorted_until(e.begin(), e.end()) == e.end());
            CHECK(plastic::is_sorted_until(a.begin(), a.end()) == a.end());
            CHECK(plastic::is_sorted_until(b.begin(), b.end()) == b.begin() + 2);
        }
    }

    TEST_CASE("nth_element") {
        std::vector<int> e, a{ 1, 2, 3, 4, 5 }, b{ 3, 4, 1, 5, 2 }, c(100);
        std::ranges::iota(c, 1);
        std::ranges::shuffle(c, std::mt19937{ std::random_device{}() });

        SUBCASE("nth_element") {
            std::vector<int> x;
            plastic::nth_element(x.begin(), x.begin(), x.end());
            CHECK(format(x) == "[]");
            x = b;
            plastic::nth_element(x.begin(), x.begin() + 2, x.end());
            CHECK(x[2] == 3);
            x = c;
            plastic::nth_element(x.begin(), x.begin() + 50, x.end());
            CHECK(x[50] == 51);
        }
    }

    TEST_CASE("binary_search") {
        std::vector<int> e, a{ 2, 4, 6, 8, 10 }, b{ 1, 2, 2, 3, 3 }, c{ 5, 5, 5, 5 };

        SUBCASE("lower_bound") {
            CHECK(plastic::lower_bound(e.begin(), e.end(), 3) == e.end());
            CHECK(plastic::lower_bound(a.begin(), a.end(), 5) == a.begin() + 2);
            CHECK(plastic::lower_bound(b.begin(), b.end(), 2) == b.begin() + 1);
            CHECK(plastic::lower_bound(c.begin(), c.end(), 5) == c.begin());
        }

        SUBCASE("upper_bound") {
            CHECK(plastic::upper_bound(e.begin(), e.end(), 3) == e.end());
            CHECK(plastic::upper_bound(a.begin(), a.end(), 5) == a.begin() + 2);
            CHECK(plastic::upper_bound(b.begin(), b.end(), 2) == b.begin() + 3);
            CHECK(plastic::upper_bound(c.begin(), c.end(), 5) == c.end());
        }

        SUBCASE("equal_range") {
            auto res1{ plastic::equal_range(e.begin(), e.end(), 3) };
            CHECK((res1.begin() == e.end() && res1.end() == e.end()));
            auto res2{ plastic::equal_range(a.begin(), a.end(), 5) };
            CHECK((res2.begin() == a.begin() + 2 && res2.end() == a.begin() + 2));
            auto res3{ plastic::equal_range(b.begin(), b.end(), 2) };
            CHECK((res3.begin() == b.begin() + 1 && res3.end() == b.begin() + 3));
            auto res4{ plastic::equal_range(c.begin(), c.end(), 5) };
            CHECK((res4.begin() == c.begin() && res4.end() == c.end()));
        }

        SUBCASE("binary_search") {
            CHECK(plastic::binary_search(e.begin(), e.end(), 3) == false);
            CHECK(plastic::binary_search(a.begin(), a.end(), 10) == true);
            CHECK(plastic::binary_search(b.begin(), b.end(), 2) == true);
            CHECK(plastic::binary_search(c.begin(), c.end(), 6) == false);
        }
    }

    TEST_CASE("partitioning") {
        std::vector<int> e, a{ 1, 3, 5, 7, 9 }, b{ 2, 4, 6, 8, 10 }, c{ 1, 2, 3, 2, 1 }, d{ 5, 5, 5, 5 };

        SUBCASE("is_partitioned") {
            CHECK(plastic::is_partitioned(e.begin(), e.end(), [](int x) { return x % 2 == 0; }) == true);
            CHECK(plastic::is_partitioned(a.begin(), a.end(), [](int x) { return x % 2 == 1; }) == true);
            CHECK(plastic::is_partitioned(b.begin(), b.end(), [](int x) { return x % 4 == 2; }) == false);
            CHECK(plastic::is_partitioned(c.begin(), c.end(), [](int x) { return x < 3; }) == false);
        }

        SUBCASE("partition") {
            std::vector<int> x;
            plastic::partition(x.begin(), x.end(), [](int x) { return x > 0; });
            CHECK(format(x) == "[]");
            x = a;
            plastic::partition(x.begin(), x.end(), [](int x) { return x < 5; });
            CHECK(std::ranges::is_partitioned(x, [](int x) { return x < 5; }));
            x = c;
            plastic::partition(x.begin(), x.end(), [](int x) { return x % 2 == 0; });
            CHECK(std::ranges::is_partitioned(x, [](int x) { return x % 2 == 0; }));
        }

        SUBCASE("stable_partition") {
            std::vector<int> x;
            plastic::stable_partition(x.begin(), x.end(), [](int x) { return x > 0; });
            CHECK(format(x) == "[]");
            x = a;
            plastic::stable_partition(x.begin(), x.end(), [](int x) { return x % 3 == 1; });
            CHECK(format(x) == "[1, 7, 3, 5, 9]");
            x = c;
            plastic::stable_partition(x.begin(), x.end(), [](int x) { return x % 2 == 0; });
            CHECK(format(x) == "[2, 2, 1, 3, 1]");
        }

        SUBCASE("partition_copy") {
            std::vector x{ 0, 0, 0, 0, 0 }, y{ x };
            plastic::partition_copy(e.begin(), e.end(), x.begin(), y.begin(), [](int x) { return x > 0; });
            CHECK(format(x) == "[0, 0, 0, 0, 0]");
            CHECK(format(y) == "[0, 0, 0, 0, 0]");
            plastic::partition_copy(a.begin(), a.end(), x.begin(), y.begin(), [](int x) { return x > 5; });
            CHECK(format(x) == "[7, 9, 0, 0, 0]");
            CHECK(format(y) == "[1, 3, 5, 0, 0]");
            plastic::partition_copy(c.begin(), c.end(), x.begin(), y.begin(), [](int x) { return x % 2 == 1; });
            CHECK(format(x) == "[1, 3, 1, 0, 0]");
            CHECK(format(y) == "[2, 2, 5, 0, 0]");
        }

        SUBCASE("partition_point") {
            CHECK(plastic::partition_point(e.begin(), e.end(), [](int x) { return x > 0; }) == e.begin());
            CHECK(plastic::partition_point(a.begin(), a.end(), [](int x) { return x < 10; }) == a.end());
            CHECK(plastic::partition_point(b.begin(), b.end(), [](int x) { return x <= 6; }) == b.begin() + 3);
        }
    }

    TEST_CASE("merge") {
        std::vector<int> e, a{ 1, 3, 5 }, b{ 2, 4, 6 }, c{ 1, 2, 3 };

        SUBCASE("merge") {
            std::vector x{ 0, 0, 0, 0, 0, 0 };
            plastic::merge(e.begin(), e.end(), e.begin(), e.end(), x.begin());
            CHECK(format(x) == "[0, 0, 0, 0, 0, 0]");
            plastic::merge(a.begin(), a.end(), b.begin(), b.end(), x.begin());
            CHECK(format(x) == "[1, 2, 3, 4, 5, 6]");
            plastic::merge(c.begin(), c.end(), c.begin(), c.end(), x.begin());
            CHECK(format(x) == "[1, 1, 2, 2, 3, 3]");
        }

        SUBCASE("inplace_merge") {
            std::vector<int> x;
            plastic::inplace_merge(e.begin(), e.begin(), e.end());
            CHECK(format(e) == "[]");
            x = { 1, 3, 5, 2, 4, 6 };
            plastic::inplace_merge(x.begin(), x.begin() + 3, x.end());
            CHECK(format(x) == "[1, 2, 3, 4, 5, 6]");
            x = { 1, 2, 3, 1, 2, 3 };
            plastic::inplace_merge(x.begin(), x.begin() + 3, x.end());
            CHECK(format(x) == "[1, 1, 2, 2, 3, 3]");
        }
    }

    TEST_CASE("set") {
        std::vector<int> e, a{ 1, 2, 3, 5, 7 }, b{ 1, 3, 4, 6, 7 };

        SUBCASE("includes") {
            CHECK(plastic::includes(e.begin(), e.end(), e.begin(), e.end()) == true);
            CHECK(plastic::includes(a.begin(), a.end(), e.begin(), e.end()) == true);
            CHECK(plastic::includes(e.begin(), e.end(), a.begin(), a.end()) == false);
            CHECK(plastic::includes(a.begin(), a.end(), b.begin(), b.end()) == false);
            CHECK(plastic::includes(a.begin(), a.end(), b.begin(), b.begin() + 2) == true);
        }

        SUBCASE("set_union") {
            std::vector x{ 0, 0, 0, 0, 0, 0, 0 };
            plastic::set_union(e.begin(), e.end(), e.begin(), e.end(), x.begin());
            CHECK(format(x) == "[0, 0, 0, 0, 0, 0, 0]");
            plastic::set_union(a.begin(), a.end(), e.begin(), e.end(), x.begin());
            CHECK(format(x) == "[1, 2, 3, 5, 7, 0, 0]");
            plastic::set_union(a.begin(), a.end(), b.begin(), b.end(), x.begin());
            CHECK(format(x) == "[1, 2, 3, 4, 5, 6, 7]");
            plastic::set_union(b.begin(), b.end(), a.begin(), a.end(), x.begin());
            CHECK(format(x) == "[1, 2, 3, 4, 5, 6, 7]");
        }

        SUBCASE("set_intersection") {
            std::vector x{ 0, 0, 0 };
            plastic::set_intersection(e.begin(), e.end(), e.begin(), e.end(), x.begin());
            CHECK(format(x) == "[0, 0, 0]");
            plastic::set_intersection(a.begin(), a.end(), e.begin(), e.end(), x.begin());
            CHECK(format(x) == "[0, 0, 0]");
            plastic::set_intersection(a.begin(), a.end(), b.begin(), b.end(), x.begin());
            CHECK(format(x) == "[1, 3, 7]");
            plastic::set_intersection(b.begin(), b.end(), a.begin(), a.end(), x.begin());
            CHECK(format(x) == "[1, 3, 7]");
        }

        SUBCASE("set_difference") {
            std::vector x{ 0, 0, 0, 0, 0 };
            plastic::set_difference(e.begin(), e.end(), e.begin(), e.end(), x.begin());
            CHECK(format(x) == "[0, 0, 0, 0, 0]");
            plastic::set_difference(a.begin(), a.end(), e.begin(), e.end(), x.begin());
            CHECK(format(x) == "[1, 2, 3, 5, 7]");
            plastic::set_difference(a.begin(), a.end(), b.begin(), b.end(), x.begin());
            CHECK(format(x) == "[2, 5, 3, 5, 7]");
            plastic::set_difference(b.begin(), b.end(), a.begin(), a.end(), x.begin());
            CHECK(format(x) == "[4, 6, 3, 5, 7]");
        }

        SUBCASE("set_symmetric_difference") {
            std::vector x{ 0, 0, 0, 0, 0 };
            plastic::set_symmetric_difference(e.begin(), e.end(), e.begin(), e.end(), x.begin());
            CHECK(format(x) == "[0, 0, 0, 0, 0]");
            plastic::set_symmetric_difference(a.begin(), a.end(), e.begin(), e.end(), x.begin());
            CHECK(format(x) == "[1, 2, 3, 5, 7]");
            plastic::set_symmetric_difference(a.begin(), a.end(), b.begin(), b.end(), x.begin());
            CHECK(format(x) == "[2, 4, 5, 6, 7]");
            plastic::set_symmetric_difference(b.begin(), b.end(), a.begin(), a.end(), x.begin());
            CHECK(format(x) == "[2, 4, 5, 6, 7]");
        }
    }

    TEST_CASE("heap") {
        std::vector<int> e, a{ 1, 2, 3, 4, 5 }, b{ 5, 4, 3, 2, 1 };

        SUBCASE("push_heap") {
            std::vector<int> x;
            plastic::push_heap(x.begin(), x.end());
            CHECK(format(x) == "[]");
            x = { 4, 1, 2, 0, 3 };
            plastic::push_heap(x.begin(), x.end());
            CHECK((std::ranges::is_heap(x) && std::ranges::contains(x, 3)));
            x = { 5, 4, 3, 2, 6 };
            plastic::push_heap(x.begin(), x.end());
            CHECK((std::ranges::is_heap(x) && std::ranges::contains(x, 6)));
        }

        SUBCASE("pop_heap") {
            std::vector<int> x;
            plastic::pop_heap(x.begin(), x.end());
            CHECK(format(x) == "[]");
            x = { 4, 3, 1, 2, 0 };
            plastic::pop_heap(x.begin(), x.end());
            x.pop_back();
            CHECK((std::ranges::is_heap(x) && !std::ranges::contains(x, 4)));
            x = { 5, 4, 3, 2, 1 };
            plastic::pop_heap(x.begin(), x.end());
            x.pop_back();
            CHECK((std::ranges::is_heap(x) && !std::ranges::contains(x, 5)));
        }

        SUBCASE("make_heap") {
            std::vector<int> x;
            plastic::make_heap(x.begin(), x.end());
            CHECK(format(x) == "[]");
            x = a;
            plastic::make_heap(x.begin(), x.end());
            CHECK(std::ranges::is_heap(x));
            x = b;
            plastic::make_heap(x.begin(), x.end());
            CHECK(std::ranges::is_heap(x));
        }

        SUBCASE("sort_heap") {
            std::vector<int> x;
            plastic::sort_heap(x.begin(), x.end());
            CHECK(format(x) == "[]");
            x = { 4, 3, 1, 2, 0 };
            plastic::sort_heap(x.begin(), x.end());
            CHECK(format(x) == "[0, 1, 2, 3, 4]");
            x = { 5, 4, 3, 2, 1 };
            plastic::sort_heap(x.begin(), x.end());
            CHECK(format(x) == "[1, 2, 3, 4, 5]");
        }

        SUBCASE("is_heap") {
            CHECK(plastic::is_heap(e.begin(), e.end()) == true);
            CHECK(plastic::is_heap(a.begin(), a.end()) == false);
            CHECK(plastic::is_heap(b.begin(), b.end()) == true);
        }

        SUBCASE("is_heap_until") {
            CHECK(plastic::is_heap_until(e.begin(), e.end()) == e.end());
            CHECK(plastic::is_heap_until(a.begin(), a.end()) == a.begin() + 1);
            CHECK(plastic::is_heap_until(b.begin(), b.end()) == b.end());
        }
    }

    TEST_CASE("minimun_and_maximum") {
        std::vector<int> e, a{ 1, 3, 5, 7, 9 }, b{ 10, 8, 6, 4, 2 };

        SUBCASE("min") {
            CHECK(plastic::min(0, 0) == 0);
            CHECK(plastic::min(5, 1) == 1);
            CHECK(plastic::min({ 0, 0, 0, 0, 0 }) == 0);
            CHECK(plastic::min({ 5, 3, 2, 4, 1 }) == 1);
        }

        SUBCASE("max") {
            CHECK(plastic::max(0, 0) == 0);
            CHECK(plastic::max(5, 1) == 5);
            CHECK(plastic::max({ 0, 0, 0, 0, 0 }) == 0);
            CHECK(plastic::max({ 5, 3, 2, 4, 1 }) == 5);
        }

        SUBCASE("minmax") {
            auto res1{ plastic::minmax(0, 0) };
            CHECK((res1.min == 0 && res1.max == 0));
            auto res2{ plastic::minmax(5, 1) };
            CHECK((res2.min == 1 && res2.max == 5));
            auto res3{ plastic::minmax({ 0, 0, 0, 0, 0 }) };
            CHECK((res3.min == 0 && res3.max == 0));
            auto res4{ plastic::minmax({ 5, 3, 2, 4, 1 }) };
            CHECK((res4.min == 1 && res4.max == 5));
        }

        SUBCASE("min_element") {
            CHECK(plastic::min_element(e.begin(), e.end()) == e.end());
            CHECK(plastic::min_element(a.begin(), a.end()) == a.begin());
            CHECK(plastic::min_element(b.begin(), b.end()) == b.end() - 1);
        }

        SUBCASE("max_element") {
            CHECK(plastic::max_element(e.begin(), e.end()) == e.end());
            CHECK(plastic::max_element(a.begin(), a.end()) == a.end() - 1);
            CHECK(plastic::max_element(b.begin(), b.end()) == b.begin());
        }

        SUBCASE("minmax_element") {
            auto res1{ plastic::minmax_element(e.begin(), e.end()) };
            CHECK((res1.min == e.end() && res1.max == e.end()));
            auto res2{ plastic::minmax_element(a.begin(), a.end()) };
            CHECK((res2.min == a.begin() && res2.max == a.end() - 1));
            auto res3{ plastic::minmax_element(b.begin(), b.end()) };
            CHECK((res3.min == b.end() - 1 && res3.max == b.begin()));
        }
    }

    TEST_CASE("bounded_value") {
        SUBCASE("clamp") {
            CHECK(plastic::clamp(0, 1, 5) == 1);
            CHECK(plastic::clamp(3, 1, 5) == 3);
            CHECK(plastic::clamp(6, 1, 5) == 5);
            CHECK(plastic::clamp(2, 2, 2) == 2);
        }
    }

    TEST_CASE("lexicographical_comparison") {
        SUBCASE("lexicographical_compare") {
            std::vector<int> e, a{ 1, 2, 3, 4, 5 }, b{ 2, 4, 6, 8, 10 };
            CHECK(plastic::lexicographical_compare(e.begin(), e.end(), a.begin(), a.end()) == true);
            CHECK(plastic::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end()) == true);
            CHECK(plastic::lexicographical_compare(b.begin(), b.end(), a.begin(), a.end()) == false);
            CHECK(
                plastic::lexicographical_compare(
                    a.begin(), a.end(), b.begin(), b.end(), {}, [](int x) { return x * 3; }
                )
                == false
            );
        }
    }

    TEST_CASE("permutation") {
        SUBCASE("next_permutation") {
            std::string x{ "abc" };
            plastic::next_permutation(x.begin(), x.end());
            REQUIRE(x == "acb");
            plastic::next_permutation(x.begin(), x.end());
            REQUIRE(x == "bac");
            plastic::next_permutation(x.begin(), x.end());
            REQUIRE(x == "bca");
            plastic::next_permutation(x.begin(), x.end());
            REQUIRE(x == "cab");
            plastic::next_permutation(x.begin(), x.end());
            REQUIRE(x == "cba");
            plastic::next_permutation(x.begin(), x.end());
            REQUIRE(x == "abc");
        }

        SUBCASE("prev_permutation") {
            std::string x{ "abc" };
            plastic::prev_permutation(x.begin(), x.end());
            REQUIRE(x == "cba");
            plastic::prev_permutation(x.begin(), x.end());
            REQUIRE(x == "cab");
            plastic::prev_permutation(x.begin(), x.end());
            REQUIRE(x == "bca");
            plastic::prev_permutation(x.begin(), x.end());
            REQUIRE(x == "bac");
            plastic::prev_permutation(x.begin(), x.end());
            REQUIRE(x == "acb");
            plastic::prev_permutation(x.begin(), x.end());
            REQUIRE(x == "abc");
        }
    }
}
