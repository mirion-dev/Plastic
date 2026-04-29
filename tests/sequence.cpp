#include <CppUnitTest.h>

#define ASSERT Microsoft::VisualStudio::CppUnitTestFramework::Assert::IsTrue

import std;
import plastic;
import utils;

namespace tests {

    TEST_CLASS(sequence) {
        TEST_METHOD(vector) {
            plastic::Vector<int> a(3), b(4, 4), c{ 3, 2, 1 }, x;
            ASSERT(format(x) == "[]");
            ASSERT(format(a) == "[0, 0, 0]");
            ASSERT(format(b) == "[4, 4, 4, 4]");
            ASSERT(format(c) == "[3, 2, 1]");

            ASSERT(format(c.data(), c.size()) == "[3, 2, 1]");
            ASSERT(format(c.cbegin(), c.cend()) == "[3, 2, 1]");
            ASSERT(format(c.rbegin(), c.rend()) == "[1, 2, 3]");
            ASSERT(format(c.crbegin(), c.crend()) == "[1, 2, 3]");

            x = c;
            ASSERT(format(x) == "[3, 2, 1]");
            c = std::move(x);
            ASSERT(format(c) == "[3, 2, 1]");
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

            x = c;
            x.clear();
            ASSERT(format(x) == "[]");

            x = c;
            x.resize(2);
            ASSERT(format(x) == "[3, 2]");
            x.resize(3);
            ASSERT(format(x) == "[3, 2, 0]");
            x.resize(5, 1);
            ASSERT(format(x) == "[3, 2, 0, 1, 1]");

            x.reserve(10);
            ASSERT(x.capacity() >= 10);
            x.reserve(5);
            ASSERT(x.capacity() >= 10);

            ASSERT(x[0] == 3);
            ASSERT(x[4] == 1);
            ASSERT(a[0] == 0);
            ASSERT(b[1] == 4);
            ASSERT(c[2] == 1);

            ASSERT(x.front() == 3);
            ASSERT(a.front() == 0);
            ASSERT(b.front() == 4);
            ASSERT(c.front() == 3);

            ASSERT(x.back() == 1);
            ASSERT(a.back() == 0);
            ASSERT(b.back() == 4);
            ASSERT(c.back() == 1);

            x = { 3, 2, 0, 1, 1 };
            x.data()[3] = 2;
            ASSERT(format(x) == "[3, 2, 0, 2, 1]");

            x.push_back(1);
            ASSERT(format(x) == "[3, 2, 0, 2, 1, 1]");
            x.push_back(2);
            ASSERT(format(x) == "[3, 2, 0, 2, 1, 1, 2]");
            x.push_back(3);
            ASSERT(format(x) == "[3, 2, 0, 2, 1, 1, 2, 3]");

            x.pop_back();
            ASSERT(format(x) == "[3, 2, 0, 2, 1, 1, 2]");
            x.pop_back();
            ASSERT(format(x) == "[3, 2, 0, 2, 1, 1]");
            x.pop_back();
            ASSERT(format(x) == "[3, 2, 0, 2, 1]");

            x.insert(x.begin() + 2, 1);
            ASSERT(format(x) == "[3, 2, 1, 0, 2, 1]");
            x.insert(x.end() - 2, 2, 1);
            ASSERT(format(x) == "[3, 2, 1, 0, 1, 1, 2, 1]");
            x.insert(x.begin() + 5, { 2, 3, 2 });
            ASSERT(format(x) == "[3, 2, 1, 0, 1, 2, 3, 2, 1, 2, 1]");

            x.erase(x.begin() + 1);
            ASSERT(format(x) == "[3, 1, 0, 1, 2, 3, 2, 1, 2, 1]");
            x.erase(x.begin() + 2, x.end() - 2);
            ASSERT(format(x) == "[3, 1, 2, 1]");

            plastic::Vector d{ 1, 2 }, e{ 1, 2, 2 }, f{ 1, 2, 3 };
            ASSERT(d == d);
            ASSERT(d != e);
            ASSERT(e != d);
            ASSERT(d < e);
            ASSERT(e > d);
            ASSERT(d <= f);
            ASSERT(f >= d);
        }

        TEST_METHOD(deque) {
            plastic::Deque<int> a(3), b(4, 4), c{ 3, 2, 1 }, x;
            ASSERT(format(x) == "[]");
            ASSERT(format(a) == "[0, 0, 0]");
            ASSERT(format(b) == "[4, 4, 4, 4]");
            ASSERT(format(c) == "[3, 2, 1]");

            ASSERT(format(c.data(), c.size()) == "[3, 2, 1]");
            ASSERT(format(c.cbegin(), c.cend()) == "[3, 2, 1]");
            ASSERT(format(c.rbegin(), c.rend()) == "[1, 2, 3]");
            ASSERT(format(c.crbegin(), c.crend()) == "[1, 2, 3]");

            x = c;
            ASSERT(format(x) == "[3, 2, 1]");
            c = std::move(x);
            ASSERT(format(c) == "[3, 2, 1]");
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

            x = c;
            x.clear();
            ASSERT(format(x) == "[]");

            x = c;
            x.resize(2);
            ASSERT(format(x) == "[3, 2]");
            x.resize(3);
            ASSERT(format(x) == "[3, 2, 0]");
            x.resize(5, 1);
            ASSERT(format(x) == "[3, 2, 0, 1, 1]");

            x.reserve(12);
            ASSERT(x.capacity() >= 12);
            x.reserve(6);
            ASSERT(x.capacity() >= 12);

            ASSERT(x[0] == 3);
            ASSERT(x[4] == 1);
            ASSERT(a[0] == 0);
            ASSERT(b[1] == 4);
            ASSERT(c[2] == 1);

            ASSERT(x.front() == 3);
            ASSERT(a.front() == 0);
            ASSERT(b.front() == 4);
            ASSERT(c.front() == 3);

            ASSERT(x.back() == 1);
            ASSERT(a.back() == 0);
            ASSERT(b.back() == 4);
            ASSERT(c.back() == 1);

            x = { 3, 2, 0, 1, 1 };
            x.push_back(1);
            ASSERT(format(x) == "[3, 2, 0, 1, 1, 1]");
            x.push_front(1);
            ASSERT(format(x) == "[1, 3, 2, 0, 1, 1, 1]");
            x.push_back(2);
            ASSERT(format(x) == "[1, 3, 2, 0, 1, 1, 1, 2]");
            x.push_front(2);
            ASSERT(format(x) == "[2, 1, 3, 2, 0, 1, 1, 1, 2]");
            x.push_back(3);
            ASSERT(format(x) == "[2, 1, 3, 2, 0, 1, 1, 1, 2, 3]");
            x.push_front(3);
            ASSERT(format(x) == "[3, 2, 1, 3, 2, 0, 1, 1, 1, 2, 3]");

            x.pop_back();
            ASSERT(format(x) == "[3, 2, 1, 3, 2, 0, 1, 1, 1, 2]");
            x.pop_front();
            ASSERT(format(x) == "[2, 1, 3, 2, 0, 1, 1, 1, 2]");
            x.pop_back();
            ASSERT(format(x) == "[2, 1, 3, 2, 0, 1, 1, 1]");
            x.pop_front();
            ASSERT(format(x) == "[1, 3, 2, 0, 1, 1, 1]");
            x.pop_back();
            ASSERT(format(x) == "[1, 3, 2, 0, 1, 1]");
            x.pop_front();
            ASSERT(format(x) == "[3, 2, 0, 1, 1]");

            x.insert(x.begin() + 2, 1);
            ASSERT(format(x) == "[3, 2, 1, 0, 1, 1]");
            x.insert(x.end() - 1, 2, 2);
            ASSERT(format(x) == "[3, 2, 1, 0, 1, 2, 2, 1]");
            x.insert(x.begin() + 5, { 2, 3, 2 });
            ASSERT(format(x) == "[3, 2, 1, 0, 1, 2, 3, 2, 2, 2, 1]");

            x.erase(x.begin() + 1);
            ASSERT(format(x) == "[3, 1, 0, 1, 2, 3, 2, 2, 2, 1]");
            x.erase(x.begin() + 2, x.end() - 2);
            ASSERT(format(x) == "[3, 1, 2, 1]");

            plastic::Deque d{ 1, 2 }, e{ 1, 2, 2 }, f{ 1, 2, 3 };
            ASSERT(d == d);
            ASSERT(d != e);
            ASSERT(e != d);
            ASSERT(d < e);
            ASSERT(e > d);
            ASSERT(d <= f);
            ASSERT(f >= d);
        }

        TEST_METHOD(list) {
            plastic::List<int> a(3), b(4, 4), c{ 3, 2, 1 }, x;
            ASSERT(format(x) == "[]");
            ASSERT(format(a) == "[0, 0, 0]");
            ASSERT(format(b) == "[4, 4, 4, 4]");
            ASSERT(format(c) == "[3, 2, 1]");

            ASSERT(format(c.cbegin(), c.cend()) == "[3, 2, 1]");
            ASSERT(format(c.rbegin(), c.rend()) == "[1, 2, 3]");
            ASSERT(format(c.crbegin(), c.crend()) == "[1, 2, 3]");

            x = c;
            ASSERT(format(x) == "[3, 2, 1]");
            c = std::move(x);
            ASSERT(format(c) == "[3, 2, 1]");
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

            x = c;
            x.clear();
            ASSERT(format(x) == "[]");

            x = c;
            x.resize(2);
            ASSERT(format(x) == "[3, 2]");
            x.resize(3);
            ASSERT(format(x) == "[3, 2, 0]");
            x.resize(5, 1);
            ASSERT(format(x) == "[3, 2, 0, 1, 1]");

            ASSERT(x.front() == 3);
            ASSERT(a.front() == 0);
            ASSERT(b.front() == 4);
            ASSERT(c.front() == 3);

            ASSERT(x.back() == 1);
            ASSERT(a.back() == 0);
            ASSERT(b.back() == 4);
            ASSERT(c.back() == 1);

            x = { 3, 2, 0, 1, 1 };
            x.push_back(1);
            ASSERT(format(x) == "[3, 2, 0, 1, 1, 1]");
            x.push_front(1);
            ASSERT(format(x) == "[1, 3, 2, 0, 1, 1, 1]");
            x.push_back(2);
            ASSERT(format(x) == "[1, 3, 2, 0, 1, 1, 1, 2]");
            x.push_front(2);
            ASSERT(format(x) == "[2, 1, 3, 2, 0, 1, 1, 1, 2]");
            x.push_back(3);
            ASSERT(format(x) == "[2, 1, 3, 2, 0, 1, 1, 1, 2, 3]");
            x.push_front(3);
            ASSERT(format(x) == "[3, 2, 1, 3, 2, 0, 1, 1, 1, 2, 3]");

            x.pop_back();
            ASSERT(format(x) == "[3, 2, 1, 3, 2, 0, 1, 1, 1, 2]");
            x.pop_front();
            ASSERT(format(x) == "[2, 1, 3, 2, 0, 1, 1, 1, 2]");
            x.pop_back();
            ASSERT(format(x) == "[2, 1, 3, 2, 0, 1, 1, 1]");
            x.pop_front();
            ASSERT(format(x) == "[1, 3, 2, 0, 1, 1, 1]");
            x.pop_back();
            ASSERT(format(x) == "[1, 3, 2, 0, 1, 1]");
            x.pop_front();
            ASSERT(format(x) == "[3, 2, 0, 1, 1]");

            x.insert(std::ranges::next(x.begin(), 2), 1);
            ASSERT(format(x) == "[3, 2, 1, 0, 1, 1]");
            x.insert(--x.end(), 2, 2);
            ASSERT(format(x) == "[3, 2, 1, 0, 1, 2, 2, 1]");
            x.insert(std::ranges::next(x.begin(), 5), { 2, 3, 2 });
            ASSERT(format(x) == "[3, 2, 1, 0, 1, 2, 3, 2, 2, 2, 1]");

            x.erase(++x.begin());
            ASSERT(format(x) == "[3, 1, 0, 1, 2, 3, 2, 2, 2, 1]");
            x.erase(std::ranges::next(x.begin(), 2), std::ranges::prev(x.end(), 2));
            ASSERT(format(x) == "[3, 1, 2, 1]");

            plastic::List d{ 1, 2 }, e{ 1, 2, 2 }, f{ 1, 2, 3 };
            ASSERT(d == d);
            ASSERT(d != e);
            ASSERT(e != d);
            ASSERT(d < e);
            ASSERT(e > d);
            ASSERT(d <= f);
            ASSERT(f >= d);
        }
    };

}
