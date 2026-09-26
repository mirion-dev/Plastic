#include <doctest/doctest.h>

import std;
import plastic;
import utils;

template <class Tr>
void test_search_tree() {
    Tr a{ 0, 0, 0 }, b{ 4, 4, 4, 4 }, c{ 2, 3, 1 }, x;
    REQUIRE(::format(x) == "[]");
    REQUIRE(::format(a) == "[0, 0, 0]");
    REQUIRE(::format(b) == "[4, 4, 4, 4]");
    REQUIRE(::format(c) == "[1, 2, 3]");

    REQUIRE(::format(c.cbegin(), c.cend()) == "[1, 2, 3]");
    REQUIRE(::format(c.rbegin(), c.rend()) == "[3, 2, 1]");
    REQUIRE(::format(c.crbegin(), c.crend()) == "[3, 2, 1]");

    x = c;
    REQUIRE(::format(x) == "[1, 2, 3]");
    c = std::move(x);
    REQUIRE(::format(c) == "[1, 2, 3]");
    x = {};
    REQUIRE(::format(x) == "[]");

    REQUIRE(x.empty() == true);
    REQUIRE(a.empty() == false);
    REQUIRE(b.empty() == false);
    REQUIRE(c.empty() == false);

    REQUIRE(x.size() == 0);
    REQUIRE(a.size() == 3);
    REQUIRE(b.size() == 4);
    REQUIRE(c.size() == 3);

    x = c;
    x.clear();
    REQUIRE(::format(x) == "[]");

    REQUIRE(a.front() == 0);
    REQUIRE(b.front() == 4);
    REQUIRE(c.front() == 1);

    REQUIRE(a.back() == 0);
    REQUIRE(b.back() == 4);
    REQUIRE(c.back() == 3);

    x = { 1, 2, 2, 2, 3 };
    REQUIRE(std::ranges::distance(x.begin(), x.lower_bound(1)) == 0);
    REQUIRE(std::ranges::distance(x.begin(), x.lower_bound(2)) == 1);
    REQUIRE(std::ranges::distance(x.begin(), x.lower_bound(3)) == 4);
    REQUIRE(std::ranges::distance(x.begin(), x.lower_bound(4)) == 5);

    REQUIRE(std::ranges::distance(x.begin(), x.upper_bound(1)) == 1);
    REQUIRE(std::ranges::distance(x.begin(), x.upper_bound(2)) == 4);
    REQUIRE(std::ranges::distance(x.begin(), x.upper_bound(3)) == 5);
    REQUIRE(std::ranges::distance(x.begin(), x.upper_bound(4)) == 5);

    REQUIRE(std::ranges::distance(x.begin(), x.find(1)) == 0);
    REQUIRE(std::ranges::distance(x.begin(), x.find(2)) == 1);
    REQUIRE(std::ranges::distance(x.begin(), x.find(3)) == 4);
    REQUIRE(std::ranges::distance(x.begin(), x.find(4)) == 5);

    REQUIRE(x.contains(1) == true);
    REQUIRE(x.contains(2) == true);
    REQUIRE(x.contains(3) == true);
    REQUIRE(x.contains(4) == false);

    REQUIRE(x.count(1) == 1);
    REQUIRE(x.count(2) == 3);
    REQUIRE(x.count(3) == 1);
    REQUIRE(x.count(4) == 0);

    x.insert(2);
    REQUIRE(::format(x) == "[1, 2, 2, 2, 2, 3]");
    x.insert(4);
    REQUIRE(::format(x) == "[1, 2, 2, 2, 2, 3, 4]");
    x.insert(0);
    REQUIRE(::format(x) == "[0, 1, 2, 2, 2, 2, 3, 4]");
    x.insert({ 5, 3, 1, 0, 2 });
    REQUIRE(::format(x) == "[0, 0, 1, 1, 2, 2, 2, 2, 2, 3, 3, 4, 5]");

    x.erase(2);
    REQUIRE(::format(x) == "[0, 0, 1, 1, 3, 3, 4, 5]");
    x.erase(1);
    REQUIRE(::format(x) == "[0, 0, 3, 3, 4, 5]");
    x.erase(++x.begin());
    REQUIRE(::format(x) == "[0, 3, 3, 4, 5]");
    x.erase(std::ranges::next(x.begin(), 2), std::ranges::prev(x.end(), 2));
    REQUIRE(::format(x) == "[0, 3, 4, 5]");

    x.merge(x);
    REQUIRE(::format(x) == "[0, 3, 4, 5]");
    x.merge(a);
    REQUIRE(::format(x) == "[0, 0, 0, 0, 3, 4, 5]");
    REQUIRE(::format(a) == "[]");
    x.merge(c);
    REQUIRE(::format(x) == "[0, 0, 0, 0, 1, 2, 3, 3, 4, 5]");
    REQUIRE(::format(c) == "[]");

    Tr d{ 1, 2 }, e{ 1, 2, 2 }, f{ 1, 2, 3 };
    REQUIRE(d == d);
    REQUIRE(d != e);
    REQUIRE(e != d);
    REQUIRE(d < e);
    REQUIRE(e > d);
    REQUIRE(d <= f);
    REQUIRE(f >= d);
}

TEST_SUITE("tree") {
    TEST_CASE("red_black_tree") {
        test_search_tree<plastic::RedBlackTree<int>>();
    }

    TEST_CASE("avl_tree" * doctest::skip{}) {
        test_search_tree<plastic::AvlTree<int>>();
    }
}
