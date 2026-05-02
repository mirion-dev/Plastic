#include <CppUnitTest.h>

#define ASSERT Microsoft::VisualStudio::CppUnitTestFramework::Assert::IsTrue

import std;
import plastic;
import utils;

namespace tests {

    TEST_CLASS(sequence) {
        TEST_METHOD(vector) {
            std::array arr{ 5, 4, 3, 2, 1 };

            plastic::Vector<int> v1;
            ASSERT(format(v1) == "[]");
            plastic::Vector<int> v2(3);
            ASSERT(format(v2) == "[0, 0, 0]");
            plastic::Vector v3(4, 4);
            ASSERT(format(v3) == "[4, 4, 4, 4]");
            plastic::Vector v4(arr.begin(), arr.end());
            ASSERT(format(v4) == "[5, 4, 3, 2, 1]");
            plastic::Vector v5(v2);
            ASSERT(format(v5) == "[0, 0, 0]");
            plastic::Vector v6(std::move(v3));
            ASSERT(format(v6) == "[4, 4, 4, 4]");
            plastic::Vector v7{ 5, 4, 3, 2, 1 };
            ASSERT(format(v7) == "[5, 4, 3, 2, 1]");

            plastic::Vector<int> e, a(3), b(4, 4), c{ 5, 4, 3, 2, 1 }, x, y{ 3, 2, 1 };

            x = y;
            ASSERT(format(x) == "[3, 2, 1]");
            x = std::move(y);
            ASSERT(format(x) == "[3, 2, 1]");
            x = { 1, 2, 3 };
            ASSERT(format(x) == "[1, 2, 3]");

            ASSERT(format(c) == "[5, 4, 3, 2, 1]");
            ASSERT(format(c.rbegin(), c.rend()) == "[1, 2, 3, 4, 5]");
            ASSERT(format(c.cbegin(), c.cend()) == "[5, 4, 3, 2, 1]");
            ASSERT(format(c.crbegin(), c.crend()) == "[1, 2, 3, 4, 5]");

            ASSERT(e.empty());
            ASSERT(!a.empty());
            ASSERT(!b.empty());
            ASSERT(!c.empty());

            ASSERT(e.size() == 0);
            ASSERT(a.size() == 3);
            ASSERT(b.size() == 4);
            ASSERT(c.size() == 5);

            ASSERT(e.max_size() >= 1'000'000);
            ASSERT(a.max_size() >= 1'000'000);
            ASSERT(b.max_size() >= 1'000'000);
            ASSERT(c.max_size() >= 1'000'000);

            ASSERT(e.capacity() == 0);
            ASSERT(a.capacity() == 3);
            ASSERT(b.capacity() == 4);
            ASSERT(c.capacity() == 5);

            x = { 3, 2, 1 };
            x.resize(2);
            ASSERT(format(x) == "[3, 2]");
            x.resize(3);
            ASSERT(format(x) == "[3, 2, 0]");
            x.resize(5, 1);
            ASSERT(format(x) == "[3, 2, 0, 1, 1]");

            x = { 3, 2, 1 };
            x.reserve(10);
            ASSERT(x.capacity() >= 10);
            x.reserve(5);
            ASSERT(x.capacity() >= 10);

            ASSERT(a[0] == 0);
            ASSERT(b[1] == 4);
            ASSERT(c[2] == 3);

            ASSERT(a.front() == 0);
            ASSERT(b.front() == 4);
            ASSERT(c.front() == 5);

            ASSERT(a.back() == 0);
            ASSERT(b.back() == 4);
            ASSERT(c.back() == 1);

            ASSERT(format(e.data(), e.size()) == "[]");
            ASSERT(format(a.data(), a.size()) == "[0, 0, 0]");
            ASSERT(format(b.data(), b.size()) == "[4, 4, 4, 4]");
            ASSERT(format(c.data(), c.size()) == "[5, 4, 3, 2, 1]");

            x = { 3, 2, 1 };
            x.push_back(4);
            ASSERT(format(x) == "[3, 2, 1, 4]");
            x.pop_back();
            ASSERT(format(x) == "[3, 2, 1]");
            x.push_back(5);
            ASSERT(format(x) == "[3, 2, 1, 5]");
            x.push_back(6);
            ASSERT(format(x) == "[3, 2, 1, 5, 6]");
            x.pop_back();
            ASSERT(format(x) == "[3, 2, 1, 5]");

            x = { 3, 2, 1 };
            y = { 0, 1, 2 };
            x.insert(x.begin() + 1, 3);
            ASSERT(format(x) == "[3, 3, 2, 1]");
            x.insert(x.begin() + 1, 2, 4);
            ASSERT(format(x) == "[3, 4, 4, 3, 2, 1]");
            x.insert(x.begin(), y.begin(), y.end());
            ASSERT(format(x) == "[0, 1, 2, 3, 4, 4, 3, 2, 1]");
            x.insert(x.end(), { 0 });
            ASSERT(format(x) == "[0, 1, 2, 3, 4, 4, 3, 2, 1, 0]");

            x = { 5, 4, 3, 2, 1 };
            x.erase(x.begin() + 1);
            ASSERT(format(x) == "[5, 3, 2, 1]");
            x.erase(x.begin() + 1, x.end() - 1);
            ASSERT(format(x) == "[5, 1]");

            x = { 3, 2, 1 };
            y = { 0, 1 };
            x.swap(y);
            ASSERT(format(x) == "[0, 1]");
            ASSERT(format(y) == "[3, 2, 1]");
            swap(x, y);
            ASSERT(format(x) == "[3, 2, 1]");
            ASSERT(format(y) == "[0, 1]");

            x = { 3, 2, 1 };
            x.clear();
            ASSERT(format(x) == "[]");

            x = { 1, 2 };
            y = { 1, 2, 3 };
            ASSERT(x == x);
            ASSERT(x != y);
            ASSERT(x < y);
            ASSERT(y > x);
            ASSERT(x <= y);
            ASSERT(y >= x);
        }

        TEST_METHOD(deque) {
            std::array arr{ 5, 4, 3, 2, 1 };

            plastic::Deque<int> d1;
            ASSERT(format(d1) == "[]");
            plastic::Deque<int> d2(3);
            ASSERT(format(d2) == "[0, 0, 0]");
            plastic::Deque d3(4, 4);
            ASSERT(format(d3) == "[4, 4, 4, 4]");
            plastic::Deque d4(arr.begin(), arr.end());
            ASSERT(format(d4) == "[5, 4, 3, 2, 1]");
            plastic::Deque d5(d2);
            ASSERT(format(d5) == "[0, 0, 0]");
            plastic::Deque d6(std::move(d3));
            ASSERT(format(d6) == "[4, 4, 4, 4]");
            plastic::Deque d7{ 5, 4, 3, 2, 1 };
            ASSERT(format(d7) == "[5, 4, 3, 2, 1]");

            plastic::Deque<int> e, a(3), b(4, 4), c{ 5, 4, 3, 2, 1 }, x, y{ 3, 2, 1 };

            x = y;
            ASSERT(format(x) == "[3, 2, 1]");
            x = std::move(y);
            ASSERT(format(x) == "[3, 2, 1]");
            x = { 1, 2, 3 };
            ASSERT(format(x) == "[1, 2, 3]");

            ASSERT(format(c) == "[5, 4, 3, 2, 1]");
            ASSERT(format(c.rbegin(), c.rend()) == "[1, 2, 3, 4, 5]");
            ASSERT(format(c.cbegin(), c.cend()) == "[5, 4, 3, 2, 1]");
            ASSERT(format(c.crbegin(), c.crend()) == "[1, 2, 3, 4, 5]");

            ASSERT(e.empty());
            ASSERT(!a.empty());
            ASSERT(!b.empty());
            ASSERT(!c.empty());

            ASSERT(e.size() == 0);
            ASSERT(a.size() == 3);
            ASSERT(b.size() == 4);
            ASSERT(c.size() == 5);

            ASSERT(e.max_size() >= 1'000'000);
            ASSERT(a.max_size() >= 1'000'000);
            ASSERT(b.max_size() >= 1'000'000);
            ASSERT(c.max_size() >= 1'000'000);

            ASSERT(e.capacity() == 0);
            ASSERT(a.capacity() == 3);
            ASSERT(b.capacity() == 4);
            ASSERT(c.capacity() == 5);

            x = { 3, 2, 1 };
            x.resize(2);
            ASSERT(format(x) == "[3, 2]");
            x.resize(3);
            ASSERT(format(x) == "[3, 2, 0]");
            x.resize(5, 1);
            ASSERT(format(x) == "[3, 2, 0, 1, 1]");

            x = { 3, 2, 1 };
            x.reserve(10);
            ASSERT(x.capacity() >= 10);
            x.reserve(5);
            ASSERT(x.capacity() >= 10);

            ASSERT(a[0] == 0);
            ASSERT(b[1] == 4);
            ASSERT(c[2] == 3);

            ASSERT(a.front() == 0);
            ASSERT(b.front() == 4);
            ASSERT(c.front() == 5);

            ASSERT(a.back() == 0);
            ASSERT(b.back() == 4);
            ASSERT(c.back() == 1);

            x = { 3, 2, 1 };
            x.push_front(4);
            ASSERT(format(x) == "[4, 3, 2, 1]");
            x.pop_front();
            ASSERT(format(x) == "[3, 2, 1]");
            x.push_front(5);
            ASSERT(format(x) == "[5, 3, 2, 1]");
            x.push_front(6);
            ASSERT(format(x) == "[6, 5, 3, 2, 1]");
            x.pop_front();
            ASSERT(format(x) == "[5, 3, 2, 1]");

            x = { 3, 2, 1 };
            x.push_back(4);
            ASSERT(format(x) == "[3, 2, 1, 4]");
            x.pop_back();
            ASSERT(format(x) == "[3, 2, 1]");
            x.push_back(5);
            ASSERT(format(x) == "[3, 2, 1, 5]");
            x.push_back(6);
            ASSERT(format(x) == "[3, 2, 1, 5, 6]");
            x.pop_back();
            ASSERT(format(x) == "[3, 2, 1, 5]");

            x = { 3, 2, 1 };
            y = { 0, 1, 2 };
            x.insert(x.begin() + 1, 3);
            ASSERT(format(x) == "[3, 3, 2, 1]");
            x.insert(x.begin() + 1, 2, 4);
            ASSERT(format(x) == "[3, 4, 4, 3, 2, 1]");
            x.insert(x.begin(), y.begin(), y.end());
            ASSERT(format(x) == "[0, 1, 2, 3, 4, 4, 3, 2, 1]");
            x.insert(x.end(), { 0 });
            ASSERT(format(x) == "[0, 1, 2, 3, 4, 4, 3, 2, 1, 0]");

            x = { 5, 4, 3, 2, 1 };
            x.erase(x.begin() + 1);
            ASSERT(format(x) == "[5, 3, 2, 1]");
            x.erase(x.begin() + 1, x.end() - 1);
            ASSERT(format(x) == "[5, 1]");

            x = { 3, 2, 1 };
            y = { 0, 1 };
            x.swap(y);
            ASSERT(format(x) == "[0, 1]");
            ASSERT(format(y) == "[3, 2, 1]");
            swap(x, y);
            ASSERT(format(x) == "[3, 2, 1]");
            ASSERT(format(y) == "[0, 1]");

            x = { 3, 2, 1 };
            x.clear();
            ASSERT(format(x) == "[]");

            x = { 1, 2 };
            y = { 1, 2, 3 };
            ASSERT(x == x);
            ASSERT(x != y);
            ASSERT(x < y);
            ASSERT(y > x);
            ASSERT(x <= y);
            ASSERT(y >= x);
        }

        TEST_METHOD(list) {
            std::array arr{ 5, 4, 3, 2, 1 };

            plastic::List<int> l1;
            ASSERT(format(l1) == "[]");
            plastic::List<int> l2(3);
            ASSERT(format(l2) == "[0, 0, 0]");
            plastic::List l3(4, 4);
            ASSERT(format(l3) == "[4, 4, 4, 4]");
            plastic::List l4(arr.begin(), arr.end());
            ASSERT(format(l4) == "[5, 4, 3, 2, 1]");
            plastic::List l5(l2);
            ASSERT(format(l5) == "[0, 0, 0]");
            plastic::List l6(std::move(l3));
            ASSERT(format(l6) == "[4, 4, 4, 4]");
            plastic::List l7{ 5, 4, 3, 2, 1 };
            ASSERT(format(l7) == "[5, 4, 3, 2, 1]");

            plastic::List<int> e, a(3), b(4, 4), c{ 5, 4, 3, 2, 1 }, x, y{ 3, 2, 1 };

            x = y;
            ASSERT(format(x) == "[3, 2, 1]");
            x = std::move(y);
            ASSERT(format(x) == "[3, 2, 1]");
            x = { 1, 2, 3 };
            ASSERT(format(x) == "[1, 2, 3]");

            ASSERT(format(c) == "[5, 4, 3, 2, 1]");
            ASSERT(format(c.rbegin(), c.rend()) == "[1, 2, 3, 4, 5]");
            ASSERT(format(c.cbegin(), c.cend()) == "[5, 4, 3, 2, 1]");
            ASSERT(format(c.crbegin(), c.crend()) == "[1, 2, 3, 4, 5]");

            ASSERT(e.empty());
            ASSERT(!a.empty());
            ASSERT(!b.empty());
            ASSERT(!c.empty());

            ASSERT(e.size() == 0);
            ASSERT(a.size() == 3);
            ASSERT(b.size() == 4);
            ASSERT(c.size() == 5);

            ASSERT(e.max_size() >= 1'000'000);
            ASSERT(a.max_size() >= 1'000'000);
            ASSERT(b.max_size() >= 1'000'000);
            ASSERT(c.max_size() >= 1'000'000);

            x = { 3, 2, 1 };
            x.resize(2);
            ASSERT(format(x) == "[3, 2]");
            x.resize(3);
            ASSERT(format(x) == "[3, 2, 0]");
            x.resize(5, 1);
            ASSERT(format(x) == "[3, 2, 0, 1, 1]");

            ASSERT(a.front() == 0);
            ASSERT(b.front() == 4);
            ASSERT(c.front() == 5);

            ASSERT(a.back() == 0);
            ASSERT(b.back() == 4);
            ASSERT(c.back() == 1);

            x = { 3, 2, 1 };
            x.push_front(4);
            ASSERT(format(x) == "[4, 3, 2, 1]");
            x.pop_front();
            ASSERT(format(x) == "[3, 2, 1]");
            x.push_front(5);
            ASSERT(format(x) == "[5, 3, 2, 1]");
            x.push_front(6);
            ASSERT(format(x) == "[6, 5, 3, 2, 1]");
            x.pop_front();
            ASSERT(format(x) == "[5, 3, 2, 1]");

            x = { 3, 2, 1 };
            x.push_back(4);
            ASSERT(format(x) == "[3, 2, 1, 4]");
            x.pop_back();
            ASSERT(format(x) == "[3, 2, 1]");
            x.push_back(5);
            ASSERT(format(x) == "[3, 2, 1, 5]");
            x.push_back(6);
            ASSERT(format(x) == "[3, 2, 1, 5, 6]");
            x.pop_back();
            ASSERT(format(x) == "[3, 2, 1, 5]");

            x = { 3, 2, 1 };
            y = { 0, 1, 2 };
            x.insert(std::ranges::next(x.begin(), 1), 3);
            ASSERT(format(x) == "[3, 3, 2, 1]");
            x.insert(std::ranges::next(x.begin(), 1), 2, 4);
            ASSERT(format(x) == "[3, 4, 4, 3, 2, 1]");
            x.insert(x.begin(), y.begin(), y.end());
            ASSERT(format(x) == "[0, 1, 2, 3, 4, 4, 3, 2, 1]");
            x.insert(x.end(), { 0 });
            ASSERT(format(x) == "[0, 1, 2, 3, 4, 4, 3, 2, 1, 0]");

            x = { 5, 4, 3, 2, 1 };
            x.erase(std::ranges::next(x.begin(), 1));
            ASSERT(format(x) == "[5, 3, 2, 1]");
            x.erase(std::ranges::next(x.begin(), 1), std::ranges::prev(x.end(), 1));
            ASSERT(format(x) == "[5, 1]");

            x = { 3, 2, 1 };
            y = { 0, 1 };
            x.swap(y);
            ASSERT(format(x) == "[0, 1]");
            ASSERT(format(y) == "[3, 2, 1]");
            swap(x, y);
            ASSERT(format(x) == "[3, 2, 1]");
            ASSERT(format(y) == "[0, 1]");

            x = { 3, 2, 1 };
            x.clear();
            ASSERT(format(x) == "[]");

            x = { 1, 2 };
            y = { 1, 2, 3 };
            ASSERT(x == x);
            ASSERT(x != y);
            ASSERT(x < y);
            ASSERT(y > x);
            ASSERT(x <= y);
            ASSERT(y >= x);
        }
    };

}
