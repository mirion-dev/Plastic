#include <doctest/doctest.h>

import std;
import plastic;
import utils;

template <class Hp>
std::string format_heap(const Hp& heap) {
    Hp clone{ heap };
    std::vector<typename Hp::value_type> res;
    while (!clone.empty()) {
        res.push_back(clone.top());
        clone.pop();
    }
    return std::format("{}", res | std::views::reverse);
}

template <class Hp>
void test_addressable_heap() {
    Hp a{ 0, 0, 0 }, b{ 4, 4, 4, 4 }, c{ 3, 2, 1 }, x;
    REQUIRE(::format_heap(x) == "[]");
    REQUIRE(::format_heap(a) == "[0, 0, 0]");
    REQUIRE(::format_heap(b) == "[4, 4, 4, 4]");
    REQUIRE(::format_heap(c) == "[1, 2, 3]");

    x = c;
    REQUIRE(::format_heap(x) == "[1, 2, 3]");
    c = std::move(x);
    REQUIRE(::format_heap(c) == "[1, 2, 3]");
    x = {};
    REQUIRE(::format_heap(x) == "[]");

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
    REQUIRE(::format_heap(x) == "[]");

    REQUIRE(*a.apex() == 0);
    REQUIRE(*b.apex() == 4);
    REQUIRE(*c.apex() == 3);

    REQUIRE(a.top() == 0);
    REQUIRE(b.top() == 4);
    REQUIRE(c.top() == 3);

    x = { 3, 2, 0, 1, 1 };
    REQUIRE(::format_heap(x) == "[0, 1, 1, 2, 3]");
    auto h1{ x.push(0) };
    REQUIRE(::format_heap(x) == "[0, 0, 1, 1, 2, 3]");
    x.push(4);
    REQUIRE(::format_heap(x) == "[0, 0, 1, 1, 2, 3, 4]");
    x.push(3);
    REQUIRE(::format_heap(x) == "[0, 0, 1, 1, 2, 3, 3, 4]");
    auto h2{ x.push(1) };
    REQUIRE(::format_heap(x) == "[0, 0, 1, 1, 1, 2, 3, 3, 4]");

    x.pop();
    REQUIRE(::format_heap(x) == "[0, 0, 1, 1, 1, 2, 3, 3]");
    x.pop();
    REQUIRE(::format_heap(x) == "[0, 0, 1, 1, 1, 2, 3]");
    x.pop();
    REQUIRE(::format_heap(x) == "[0, 0, 1, 1, 1, 2]");

    *h1 = 5;
    REQUIRE(::format_heap(x) == "[0, 1, 1, 1, 2, 5]");
    *h2 = 0;
    REQUIRE(::format_heap(x) == "[0, 0, 1, 1, 2, 5]");
    *x.apex() = 3;
    REQUIRE(::format_heap(x) == "[0, 0, 1, 1, 2, 3]");

    x.erase(h1);
    REQUIRE(::format_heap(x) == "[0, 0, 1, 1, 2]");
    x.erase(h2);
    REQUIRE(::format_heap(x) == "[0, 1, 1, 2]");

    x.merge(x);
    REQUIRE(::format_heap(x) == "[0, 1, 1, 2]");
    x.merge(b);
    REQUIRE(::format_heap(x) == "[0, 1, 1, 2, 4, 4, 4, 4]");
    REQUIRE(::format_heap(b) == "[]");
    x.merge(c);
    REQUIRE(::format_heap(x) == "[0, 1, 1, 1, 2, 2, 3, 4, 4, 4, 4]");
    REQUIRE(::format_heap(c) == "[]");
}

TEST_SUITE("heap") {
    TEST_CASE("binary_heap") {
        test_addressable_heap<plastic::BinaryHeap<int>>();
    }
}
