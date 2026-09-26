#include <doctest/doctest.h>

import std;
import plastic;
import utils;

TEST_SUITE("sequence") {
    TEST_CASE("vector") {
        std::array arr{ 5, 4, 3, 2, 1 };

        plastic::Vector<int> v1;
        REQUIRE(format(v1) == "[]");
        plastic::Vector<int> v2(3);
        REQUIRE(format(v2) == "[0, 0, 0]");
        plastic::Vector v3(4, 4);
        REQUIRE(format(v3) == "[4, 4, 4, 4]");
        plastic::Vector v4(arr.begin(), arr.end());
        REQUIRE(format(v4) == "[5, 4, 3, 2, 1]");
        plastic::Vector v5(v2);
        REQUIRE(format(v5) == "[0, 0, 0]");
        plastic::Vector v6(std::move(v3));
        REQUIRE(format(v6) == "[4, 4, 4, 4]");
        plastic::Vector v7{ 5, 4, 3, 2, 1 };
        REQUIRE(format(v7) == "[5, 4, 3, 2, 1]");

        plastic::Vector<int> e, a(3), b(4, 4), c{ 5, 4, 3, 2, 1 }, x, y{ 3, 2, 1 };

        x = y;
        REQUIRE(format(x) == "[3, 2, 1]");
        x = std::move(y);
        REQUIRE(format(x) == "[3, 2, 1]");
        x = { 1, 2, 3 };
        REQUIRE(format(x) == "[1, 2, 3]");

        REQUIRE(format(c) == "[5, 4, 3, 2, 1]");
        REQUIRE(format(c.rbegin(), c.rend()) == "[1, 2, 3, 4, 5]");
        REQUIRE(format(c.cbegin(), c.cend()) == "[5, 4, 3, 2, 1]");
        REQUIRE(format(c.crbegin(), c.crend()) == "[1, 2, 3, 4, 5]");

        REQUIRE(e.empty());
        REQUIRE(!a.empty());
        REQUIRE(!b.empty());
        REQUIRE(!c.empty());

        REQUIRE(e.size() == 0);
        REQUIRE(a.size() == 3);
        REQUIRE(b.size() == 4);
        REQUIRE(c.size() == 5);

        REQUIRE(e.max_size() >= 1'000'000);
        REQUIRE(a.max_size() >= 1'000'000);
        REQUIRE(b.max_size() >= 1'000'000);
        REQUIRE(c.max_size() >= 1'000'000);

        REQUIRE(e.capacity() == 0);
        REQUIRE(a.capacity() == 3);
        REQUIRE(b.capacity() == 4);
        REQUIRE(c.capacity() == 5);

        x = { 3, 2, 1 };
        x.resize(2);
        REQUIRE(format(x) == "[3, 2]");
        x.resize(3);
        REQUIRE(format(x) == "[3, 2, 0]");
        x.resize(5, 1);
        REQUIRE(format(x) == "[3, 2, 0, 1, 1]");

        x = { 3, 2, 1 };
        x.reserve(10);
        REQUIRE(x.capacity() >= 10);
        x.reserve(5);
        REQUIRE(x.capacity() >= 10);

        REQUIRE(a[0] == 0);
        REQUIRE(b[1] == 4);
        REQUIRE(c[2] == 3);

        REQUIRE(a.front() == 0);
        REQUIRE(b.front() == 4);
        REQUIRE(c.front() == 5);

        REQUIRE(a.back() == 0);
        REQUIRE(b.back() == 4);
        REQUIRE(c.back() == 1);

        REQUIRE(format(e.data(), e.size()) == "[]");
        REQUIRE(format(a.data(), a.size()) == "[0, 0, 0]");
        REQUIRE(format(b.data(), b.size()) == "[4, 4, 4, 4]");
        REQUIRE(format(c.data(), c.size()) == "[5, 4, 3, 2, 1]");

        x = { 3, 2, 1 };
        x.push_back(4);
        REQUIRE(format(x) == "[3, 2, 1, 4]");
        x.pop_back();
        REQUIRE(format(x) == "[3, 2, 1]");
        x.push_back(5);
        REQUIRE(format(x) == "[3, 2, 1, 5]");
        x.push_back(6);
        REQUIRE(format(x) == "[3, 2, 1, 5, 6]");
        x.pop_back();
        REQUIRE(format(x) == "[3, 2, 1, 5]");

        x = { 3, 2, 1 };
        y = { 0, 1, 2 };
        x.insert(x.begin() + 1, 3);
        REQUIRE(format(x) == "[3, 3, 2, 1]");
        x.insert(x.begin() + 1, 2, 4);
        REQUIRE(format(x) == "[3, 4, 4, 3, 2, 1]");
        x.insert(x.begin(), y.begin(), y.end());
        REQUIRE(format(x) == "[0, 1, 2, 3, 4, 4, 3, 2, 1]");
        x.insert(x.end(), { 0 });
        REQUIRE(format(x) == "[0, 1, 2, 3, 4, 4, 3, 2, 1, 0]");

        x = { 5, 4, 3, 2, 1 };
        x.erase(x.begin() + 1);
        REQUIRE(format(x) == "[5, 3, 2, 1]");
        x.erase(x.begin() + 1, x.end() - 1);
        REQUIRE(format(x) == "[5, 1]");

        x = { 3, 2, 1 };
        y = { 0, 1 };
        x.swap(y);
        REQUIRE(format(x) == "[0, 1]");
        REQUIRE(format(y) == "[3, 2, 1]");
        swap(x, y);
        REQUIRE(format(x) == "[3, 2, 1]");
        REQUIRE(format(y) == "[0, 1]");

        x = { 3, 2, 1 };
        x.clear();
        REQUIRE(format(x) == "[]");

        x = { 1, 2 };
        y = { 1, 2, 3 };
        REQUIRE(x == x);
        REQUIRE(x != y);
        REQUIRE(x < y);
        REQUIRE(y > x);
        REQUIRE(x <= y);
        REQUIRE(y >= x);
    }

    TEST_CASE("deque") {
        std::array arr{ 5, 4, 3, 2, 1 };

        plastic::Deque<int> d1;
        REQUIRE(format(d1) == "[]");
        plastic::Deque<int> d2(3);
        REQUIRE(format(d2) == "[0, 0, 0]");
        plastic::Deque d3(4, 4);
        REQUIRE(format(d3) == "[4, 4, 4, 4]");
        plastic::Deque d4(arr.begin(), arr.end());
        REQUIRE(format(d4) == "[5, 4, 3, 2, 1]");
        plastic::Deque d5(d2);
        REQUIRE(format(d5) == "[0, 0, 0]");
        plastic::Deque d6(std::move(d3));
        REQUIRE(format(d6) == "[4, 4, 4, 4]");
        plastic::Deque d7{ 5, 4, 3, 2, 1 };
        REQUIRE(format(d7) == "[5, 4, 3, 2, 1]");

        plastic::Deque<int> e, a(3), b(4, 4), c{ 5, 4, 3, 2, 1 }, x, y{ 3, 2, 1 };

        x = y;
        REQUIRE(format(x) == "[3, 2, 1]");
        x = std::move(y);
        REQUIRE(format(x) == "[3, 2, 1]");
        x = { 1, 2, 3 };
        REQUIRE(format(x) == "[1, 2, 3]");

        REQUIRE(format(c) == "[5, 4, 3, 2, 1]");
        REQUIRE(format(c.rbegin(), c.rend()) == "[1, 2, 3, 4, 5]");
        REQUIRE(format(c.cbegin(), c.cend()) == "[5, 4, 3, 2, 1]");
        REQUIRE(format(c.crbegin(), c.crend()) == "[1, 2, 3, 4, 5]");

        REQUIRE(e.empty());
        REQUIRE(!a.empty());
        REQUIRE(!b.empty());
        REQUIRE(!c.empty());

        REQUIRE(e.size() == 0);
        REQUIRE(a.size() == 3);
        REQUIRE(b.size() == 4);
        REQUIRE(c.size() == 5);

        REQUIRE(e.max_size() >= 1'000'000);
        REQUIRE(a.max_size() >= 1'000'000);
        REQUIRE(b.max_size() >= 1'000'000);
        REQUIRE(c.max_size() >= 1'000'000);

        REQUIRE(e.capacity() == 0);
        REQUIRE(a.capacity() == 3);
        REQUIRE(b.capacity() == 4);
        REQUIRE(c.capacity() == 5);

        x = { 3, 2, 1 };
        x.resize(2);
        REQUIRE(format(x) == "[3, 2]");
        x.resize(3);
        REQUIRE(format(x) == "[3, 2, 0]");
        x.resize(5, 1);
        REQUIRE(format(x) == "[3, 2, 0, 1, 1]");

        x = { 3, 2, 1 };
        x.reserve(10);
        REQUIRE(x.capacity() >= 10);
        x.reserve(5);
        REQUIRE(x.capacity() >= 10);

        REQUIRE(a[0] == 0);
        REQUIRE(b[1] == 4);
        REQUIRE(c[2] == 3);

        REQUIRE(a.front() == 0);
        REQUIRE(b.front() == 4);
        REQUIRE(c.front() == 5);

        REQUIRE(a.back() == 0);
        REQUIRE(b.back() == 4);
        REQUIRE(c.back() == 1);

        x = { 3, 2, 1 };
        x.push_front(4);
        REQUIRE(format(x) == "[4, 3, 2, 1]");
        x.pop_front();
        REQUIRE(format(x) == "[3, 2, 1]");
        x.push_front(5);
        REQUIRE(format(x) == "[5, 3, 2, 1]");
        x.push_front(6);
        REQUIRE(format(x) == "[6, 5, 3, 2, 1]");
        x.pop_front();
        REQUIRE(format(x) == "[5, 3, 2, 1]");

        x = { 3, 2, 1 };
        x.push_back(4);
        REQUIRE(format(x) == "[3, 2, 1, 4]");
        x.pop_back();
        REQUIRE(format(x) == "[3, 2, 1]");
        x.push_back(5);
        REQUIRE(format(x) == "[3, 2, 1, 5]");
        x.push_back(6);
        REQUIRE(format(x) == "[3, 2, 1, 5, 6]");
        x.pop_back();
        REQUIRE(format(x) == "[3, 2, 1, 5]");

        x = { 3, 2, 1 };
        y = { 0, 1, 2 };
        x.insert(x.begin() + 1, 3);
        REQUIRE(format(x) == "[3, 3, 2, 1]");
        x.insert(x.begin() + 1, 2, 4);
        REQUIRE(format(x) == "[3, 4, 4, 3, 2, 1]");
        x.insert(x.begin(), y.begin(), y.end());
        REQUIRE(format(x) == "[0, 1, 2, 3, 4, 4, 3, 2, 1]");
        x.insert(x.end(), { 0 });
        REQUIRE(format(x) == "[0, 1, 2, 3, 4, 4, 3, 2, 1, 0]");

        x = { 5, 4, 3, 2, 1 };
        x.erase(x.begin() + 1);
        REQUIRE(format(x) == "[5, 3, 2, 1]");
        x.erase(x.begin() + 1, x.end() - 1);
        REQUIRE(format(x) == "[5, 1]");

        x = { 3, 2, 1 };
        y = { 0, 1 };
        x.swap(y);
        REQUIRE(format(x) == "[0, 1]");
        REQUIRE(format(y) == "[3, 2, 1]");
        swap(x, y);
        REQUIRE(format(x) == "[3, 2, 1]");
        REQUIRE(format(y) == "[0, 1]");

        x = { 3, 2, 1 };
        x.clear();
        REQUIRE(format(x) == "[]");

        x = { 1, 2 };
        y = { 1, 2, 3 };
        REQUIRE(x == x);
        REQUIRE(x != y);
        REQUIRE(x < y);
        REQUIRE(y > x);
        REQUIRE(x <= y);
        REQUIRE(y >= x);
    }

    TEST_CASE("list") {
        std::array arr{ 5, 4, 3, 2, 1 };

        plastic::List<int> l1;
        REQUIRE(format(l1) == "[]");
        plastic::List<int> l2(3);
        REQUIRE(format(l2) == "[0, 0, 0]");
        plastic::List l3(4, 4);
        REQUIRE(format(l3) == "[4, 4, 4, 4]");
        plastic::List l4(arr.begin(), arr.end());
        REQUIRE(format(l4) == "[5, 4, 3, 2, 1]");
        plastic::List l5(l2);
        REQUIRE(format(l5) == "[0, 0, 0]");
        plastic::List l6(std::move(l3));
        REQUIRE(format(l6) == "[4, 4, 4, 4]");
        plastic::List l7{ 5, 4, 3, 2, 1 };
        REQUIRE(format(l7) == "[5, 4, 3, 2, 1]");

        plastic::List<int> e, a(3), b(4, 4), c{ 5, 4, 3, 2, 1 }, x, y{ 3, 2, 1 };

        x = y;
        REQUIRE(format(x) == "[3, 2, 1]");
        x = std::move(y);
        REQUIRE(format(x) == "[3, 2, 1]");
        x = { 1, 2, 3 };
        REQUIRE(format(x) == "[1, 2, 3]");

        REQUIRE(format(c) == "[5, 4, 3, 2, 1]");
        REQUIRE(format(c.rbegin(), c.rend()) == "[1, 2, 3, 4, 5]");
        REQUIRE(format(c.cbegin(), c.cend()) == "[5, 4, 3, 2, 1]");
        REQUIRE(format(c.crbegin(), c.crend()) == "[1, 2, 3, 4, 5]");

        REQUIRE(e.empty());
        REQUIRE(!a.empty());
        REQUIRE(!b.empty());
        REQUIRE(!c.empty());

        REQUIRE(e.size() == 0);
        REQUIRE(a.size() == 3);
        REQUIRE(b.size() == 4);
        REQUIRE(c.size() == 5);

        REQUIRE(e.max_size() >= 1'000'000);
        REQUIRE(a.max_size() >= 1'000'000);
        REQUIRE(b.max_size() >= 1'000'000);
        REQUIRE(c.max_size() >= 1'000'000);

        x = { 3, 2, 1 };
        x.resize(2);
        REQUIRE(format(x) == "[3, 2]");
        x.resize(3);
        REQUIRE(format(x) == "[3, 2, 0]");
        x.resize(5, 1);
        REQUIRE(format(x) == "[3, 2, 0, 1, 1]");

        REQUIRE(a.front() == 0);
        REQUIRE(b.front() == 4);
        REQUIRE(c.front() == 5);

        REQUIRE(a.back() == 0);
        REQUIRE(b.back() == 4);
        REQUIRE(c.back() == 1);

        x = { 3, 2, 1 };
        x.push_front(4);
        REQUIRE(format(x) == "[4, 3, 2, 1]");
        x.pop_front();
        REQUIRE(format(x) == "[3, 2, 1]");
        x.push_front(5);
        REQUIRE(format(x) == "[5, 3, 2, 1]");
        x.push_front(6);
        REQUIRE(format(x) == "[6, 5, 3, 2, 1]");
        x.pop_front();
        REQUIRE(format(x) == "[5, 3, 2, 1]");

        x = { 3, 2, 1 };
        x.push_back(4);
        REQUIRE(format(x) == "[3, 2, 1, 4]");
        x.pop_back();
        REQUIRE(format(x) == "[3, 2, 1]");
        x.push_back(5);
        REQUIRE(format(x) == "[3, 2, 1, 5]");
        x.push_back(6);
        REQUIRE(format(x) == "[3, 2, 1, 5, 6]");
        x.pop_back();
        REQUIRE(format(x) == "[3, 2, 1, 5]");

        x = { 3, 2, 1 };
        y = { 0, 1, 2 };
        x.insert(std::ranges::next(x.begin(), 1), 3);
        REQUIRE(format(x) == "[3, 3, 2, 1]");
        x.insert(std::ranges::next(x.begin(), 1), 2, 4);
        REQUIRE(format(x) == "[3, 4, 4, 3, 2, 1]");
        x.insert(x.begin(), y.begin(), y.end());
        REQUIRE(format(x) == "[0, 1, 2, 3, 4, 4, 3, 2, 1]");
        x.insert(x.end(), { 0 });
        REQUIRE(format(x) == "[0, 1, 2, 3, 4, 4, 3, 2, 1, 0]");

        x = { 5, 4, 3, 2, 1 };
        x.erase(std::ranges::next(x.begin(), 1));
        REQUIRE(format(x) == "[5, 3, 2, 1]");
        x.erase(std::ranges::next(x.begin(), 1), std::ranges::prev(x.end(), 1));
        REQUIRE(format(x) == "[5, 1]");

        x = { 3, 2, 1 };
        y = { 0, 1 };
        x.swap(y);
        REQUIRE(format(x) == "[0, 1]");
        REQUIRE(format(y) == "[3, 2, 1]");
        swap(x, y);
        REQUIRE(format(x) == "[3, 2, 1]");
        REQUIRE(format(y) == "[0, 1]");

        x = { 3, 2, 1 };
        x.clear();
        REQUIRE(format(x) == "[]");

        x = { 1, 2 };
        y = { 1, 2, 3 };
        REQUIRE(x == x);
        REQUIRE(x != y);
        REQUIRE(x < y);
        REQUIRE(y > x);
        REQUIRE(x <= y);
        REQUIRE(y >= x);
    }
}
