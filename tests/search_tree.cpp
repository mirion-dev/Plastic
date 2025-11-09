#include <CppUnitTest.h>

#define ASSERT Microsoft::VisualStudio::CppUnitTestFramework::Assert::IsTrue

import std;
import plastic;
import utils;

using namespace plastic;

template <class Tr>
void test_search_tree() {
    Tr a{ 0, 0, 0 }, b{ 4, 4, 4, 4 }, c{ 2, 3, 1 }, x;
    ASSERT(format(x) == "[]");
    ASSERT(format(a) == "[0, 0, 0]");
    ASSERT(format(b) == "[4, 4, 4, 4]");
    ASSERT(format(c) == "[1, 2, 3]");

    ASSERT(format(c.cbegin(), c.cend()) == "[1, 2, 3]");
    ASSERT(format(c.rbegin(), c.rend()) == "[3, 2, 1]");
    ASSERT(format(c.crbegin(), c.crend()) == "[3, 2, 1]");

    x = c;
    ASSERT(format(x) == "[1, 2, 3]");
    c = std::move(x);
    ASSERT(format(c) == "[1, 2, 3]");
    x = {};
    ASSERT(format(x) == "[]");

    ASSERT(x.empty() == true);
    ASSERT(a.empty() == false);
    ASSERT(b.empty() == false);
    ASSERT(c.empty() == false);

    ASSERT(x.size() == 0);
    ASSERT(a.size() == 3);
    ASSERT(b.size() == 4);
    ASSERT(c.size() == 3);

    ASSERT(x.max_size() == std::numeric_limits<std::size_t>::max());

    x = c;
    x.clear();
    ASSERT(format(x) == "[]");

    ASSERT(a.front() == 0);
    ASSERT(b.front() == 4);
    ASSERT(c.front() == 1);

    ASSERT(a.back() == 0);
    ASSERT(b.back() == 4);
    ASSERT(c.back() == 3);

    x = { 1, 2, 2, 2, 3 };
    ASSERT(std::distance(x.begin(), x.lower_bound(1)) == 0);
    ASSERT(std::distance(x.begin(), x.lower_bound(2)) == 1);
    ASSERT(std::distance(x.begin(), x.lower_bound(3)) == 4);
    ASSERT(std::distance(x.begin(), x.lower_bound(4)) == 5);

    ASSERT(std::distance(x.begin(), x.upper_bound(1)) == 1);
    ASSERT(std::distance(x.begin(), x.upper_bound(2)) == 4);
    ASSERT(std::distance(x.begin(), x.upper_bound(3)) == 5);
    ASSERT(std::distance(x.begin(), x.upper_bound(4)) == 5);

    ASSERT(std::distance(x.begin(), x.find(1)) == 0);
    ASSERT(std::distance(x.begin(), x.find(2)) == 1);
    ASSERT(std::distance(x.begin(), x.find(3)) == 4);
    ASSERT(std::distance(x.begin(), x.find(4)) == 5);

    ASSERT(x.contains(1) == true);
    ASSERT(x.contains(2) == true);
    ASSERT(x.contains(3) == true);
    ASSERT(x.contains(4) == false);

    ASSERT(x.count(1) == 1);
    ASSERT(x.count(2) == 3);
    ASSERT(x.count(3) == 1);
    ASSERT(x.count(4) == 0);

    x.insert(2);
    ASSERT(format(x) == "[1, 2, 2, 2, 2, 3]");
    x.insert(4);
    ASSERT(format(x) == "[1, 2, 2, 2, 2, 3, 4]");
    x.insert(0);
    ASSERT(format(x) == "[0, 1, 2, 2, 2, 2, 3, 4]");
    x.insert({ 5, 3, 1, 0, 2 });
    ASSERT(format(x) == "[0, 0, 1, 1, 2, 2, 2, 2, 2, 3, 3, 4, 5]");

    x.erase(2);
    ASSERT(format(x) == "[0, 0, 1, 1, 3, 3, 4, 5]");
    x.erase(1);
    ASSERT(format(x) == "[0, 0, 3, 3, 4, 5]");
    x.erase(++x.begin());
    ASSERT(format(x) == "[0, 3, 3, 4, 5]");
    x.erase(std::next(x.begin(), 2), std::prev(x.end(), 2));
    ASSERT(format(x) == "[0, 3, 4, 5]");

    Tr d{ 1, 2 }, e{ 1, 2, 2 }, f{ 1, 2, 3 };
    ASSERT(d == d);
    ASSERT(d != e);
    ASSERT(e != d);
    ASSERT(d < e);
    ASSERT(e > d);
    ASSERT(d <= f);
    ASSERT(f >= d);
}

TEST_CLASS(search_tree) {

public:
    TEST_METHOD(binary_search_tree) {
        test_search_tree<plastic::binary_search_tree<int>>();
    }

    TEST_METHOD(red_black_tree) {
        test_search_tree<plastic::red_black_tree<int>>();
    }

    TEST_METHOD(avl_tree) {
        ASSERT(false);
        test_search_tree<plastic::avl_tree<int>>();
    }

};
