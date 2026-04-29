#include <CppUnitTest.h>

#define ASSERT Microsoft::VisualStudio::CppUnitTestFramework::Assert::IsTrue

import std;
import plastic;
import utils;

namespace tests {

    template <class Hp>
    void test_addressable_heap() {
        Hp a{ 0, 0, 0 }, b{ 4, 4, 4, 4 }, c{ 3, 2, 1 }, x;
        ASSERT(tests::format(x) == "[]");
        ASSERT(tests::format(a) == "[0, 0, 0]");
        ASSERT(tests::format(b) == "[4, 4, 4, 4]");
        ASSERT(tests::format(c) == "[1, 2, 3]");

        x = c;
        ASSERT(tests::format(x) == "[1, 2, 3]");
        c = std::move(x);
        ASSERT(tests::format(c) == "[1, 2, 3]");
        x = {};
        ASSERT(tests::format(x) == "[]");

        ASSERT(x.empty() == true);
        ASSERT(a.empty() == false);
        ASSERT(b.empty() == false);
        ASSERT(c.empty() == false);

        ASSERT(x.size() == 0);
        ASSERT(a.size() == 3);
        ASSERT(b.size() == 4);
        ASSERT(c.size() == 3);

        x = c;
        x.clear();
        ASSERT(tests::format(x) == "[]");

        ASSERT(*a.apex() == 0);
        ASSERT(*b.apex() == 4);
        ASSERT(*c.apex() == 3);

        ASSERT(a.top() == 0);
        ASSERT(b.top() == 4);
        ASSERT(c.top() == 3);

        x = { 3, 2, 0, 1, 1 };
        ASSERT(tests::format(x) == "[0, 1, 1, 2, 3]");
        auto h1{ x.push(0) };
        ASSERT(tests::format(x) == "[0, 0, 1, 1, 2, 3]");
        x.push(4);
        ASSERT(tests::format(x) == "[0, 0, 1, 1, 2, 3, 4]");
        x.push(3);
        ASSERT(tests::format(x) == "[0, 0, 1, 1, 2, 3, 3, 4]");
        auto h2{ x.push(1) };
        ASSERT(tests::format(x) == "[0, 0, 1, 1, 1, 2, 3, 3, 4]");

        x.pop();
        ASSERT(tests::format(x) == "[0, 0, 1, 1, 1, 2, 3, 3]");
        x.pop();
        ASSERT(tests::format(x) == "[0, 0, 1, 1, 1, 2, 3]");
        x.pop();
        ASSERT(tests::format(x) == "[0, 0, 1, 1, 1, 2]");

        *h1 = 5;
        ASSERT(tests::format(x) == "[0, 1, 1, 1, 2, 5]");
        *h2 = 0;
        ASSERT(tests::format(x) == "[0, 0, 1, 1, 2, 5]");
        *x.apex() = 3;
        ASSERT(tests::format(x) == "[0, 0, 1, 1, 2, 3]");

        x.erase(h1);
        ASSERT(tests::format(x) == "[0, 0, 1, 1, 2]");
        x.erase(h2);
        ASSERT(tests::format(x) == "[0, 1, 1, 2]");

        x.merge(x);
        ASSERT(tests::format(x) == "[0, 1, 1, 2]");
        x.merge(b);
        ASSERT(tests::format(x) == "[0, 1, 1, 2, 4, 4, 4, 4]");
        ASSERT(tests::format(b) == "[]");
        x.merge(c);
        ASSERT(tests::format(x) == "[0, 1, 1, 1, 2, 2, 3, 4, 4, 4, 4]");
        ASSERT(tests::format(c) == "[]");
    }

    TEST_CLASS(heap) {
    public:
        TEST_METHOD(binary_heap) {
            test_addressable_heap<plastic::BinaryHeap<int>>();
        }
    };

}
