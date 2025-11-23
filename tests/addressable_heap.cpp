#include <CppUnitTest.h>

#define ASSERT Microsoft::VisualStudio::CppUnitTestFramework::Assert::IsTrue

import std;
import plastic;
import utils;

using namespace plastic;

template <class Hp>
void test_addressable_heap() {
    Hp a{ 0, 0, 0 }, b{ 4, 4, 4, 4 }, c{ 3, 2, 1 }, x;
    ASSERT(format(x) == "[]");
    ASSERT(format(a) == "[0, 0, 0]");
    ASSERT(format(b) == "[4, 4, 4, 4]");
    ASSERT(format(c) == "[1, 2, 3]");

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

    ASSERT(*a.apex() == 0);
    ASSERT(*b.apex() == 4);
    ASSERT(*c.apex() == 3);

    ASSERT(a.top() == 0);
    ASSERT(b.top() == 4);
    ASSERT(c.top() == 3);

    x = { 3, 2, 0, 1, 1 };
    ASSERT(format(x) == "[0, 1, 1, 2, 3]");
    auto h1{ x.push(0) };
    ASSERT(format(x) == "[0, 0, 1, 1, 2, 3]");
    x.push(4);
    ASSERT(format(x) == "[0, 0, 1, 1, 2, 3, 4]");
    x.push(3);
    ASSERT(format(x) == "[0, 0, 1, 1, 2, 3, 3, 4]");
    auto h2{ x.push(1) };
    ASSERT(format(x) == "[0, 0, 1, 1, 1, 2, 3, 3, 4]");

    x.pop();
    ASSERT(format(x) == "[0, 0, 1, 1, 1, 2, 3, 3]");
    x.pop();
    ASSERT(format(x) == "[0, 0, 1, 1, 1, 2, 3]");
    x.pop();
    ASSERT(format(x) == "[0, 0, 1, 1, 1, 2]");

    *h1 = 5;
    ASSERT(format(x) == "[0, 1, 1, 1, 2, 5]");
    *h2 = 0;
    ASSERT(format(x) == "[0, 0, 1, 1, 2, 5]");
    *x.apex() = 3;
    ASSERT(format(x) == "[0, 0, 1, 1, 2, 3]");

    x.erase(h1);
    ASSERT(format(x) == "[0, 0, 1, 1, 2]");
    x.erase(h2);
    ASSERT(format(x) == "[0, 1, 1, 2]");

    x.merge(x);
    ASSERT(format(x) == "[0, 1, 1, 2]");
    x.merge(b);
    ASSERT(format(x) == "[0, 1, 1, 2, 4, 4, 4, 4]");
    ASSERT(format(b) == "[]");
    x.merge(c);
    ASSERT(format(x) == "[0, 1, 1, 1, 2, 2, 3, 4, 4, 4, 4]");
    ASSERT(format(c) == "[]");
}

TEST_CLASS(addressable_heap) {

public:
    TEST_METHOD(binary_heap) {
        test_addressable_heap<plastic::binary_heap<int>>();
    }

};
