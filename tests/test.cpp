#include <CppUnitTest.h>

#define ASSERT Microsoft::VisualStudio::CppUnitTestFramework::Assert::IsTrue

import std;
import plastic;

std::string format(const auto& arg) {
    return std::format("{}", arg);
}

std::string format(std::contiguous_iterator auto first, std::size_t size) {
    return format(std::span{ first, size });
}

template <std::input_iterator It>
std::string format(It first, It last) {
    return format(std::ranges::subrange{ first, last });
}

template <class Hp>
    requires requires(Hp heap) {
        heap.empty();
        heap.top();
        heap.pop();
    }
std::string format(const Hp& heap) {
    Hp clone{ heap };
    std::vector<typename Hp::value_type> temp;
    while (!clone.empty()) {
        temp.push_back(clone.top());
        clone.pop();
    }
    return format(temp | std::views::reverse);
}

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
    ASSERT(d == d); // NOLINT
    ASSERT(d != e);
    ASSERT(e != d);
    ASSERT(d < e);
    ASSERT(e > d);
    ASSERT(d <= f);
    ASSERT(f >= d);
}

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

    ASSERT(a.top() == 0);
    ASSERT(b.top() == 4);
    ASSERT(c.top() == 3);

    ASSERT(*a.top_handle() == 0);
    ASSERT(*b.top_handle() == 4);
    ASSERT(*c.top_handle() == 3);

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

    x.merge({ 3, 1, 4 });
    ASSERT(format(x) == "[0, 0, 1, 1, 1, 1, 2, 3, 4]");

    x.update(h1, 5);
    ASSERT(format(x) == "[0, 1, 1, 1, 1, 2, 3, 4, 5]");
    x.update(h2, 0);
    ASSERT(format(x) == "[0, 0, 1, 1, 1, 2, 3, 4, 5]");
    x.update(x.top_handle(), 3);
    ASSERT(format(x) == "[0, 0, 1, 1, 1, 2, 3, 3, 4]");

    x.erase(h1);
    ASSERT(format(x) == "[0, 0, 1, 1, 1, 2, 3, 4]");
    x.erase(h2);
    ASSERT(format(x) == "[0, 1, 1, 1, 2, 3, 4]");
}

TEST_CLASS(data_structure) {

public:
    TEST_METHOD(inplace_vector) {
        plastic::inplace_vector<int, 12> a(3), b(4, 4), c{ 3, 2, 1 }, x;
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

        ASSERT(x.max_size() == 12);

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
        ASSERT(x.capacity() == 12);
        x.reserve(6);
        ASSERT(x.capacity() == 12);

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

        plastic::inplace_vector<int, 12> d{ 1, 2 }, e{ 1, 2, 2 }, f{ 1, 2, 3 };
        ASSERT(d == d); // NOLINT
        ASSERT(d != e);
        ASSERT(e != d);
        ASSERT(d < e);
        ASSERT(e > d);
        ASSERT(d <= f);
        ASSERT(f >= d);
    }

    TEST_METHOD(vector) {
        plastic::vector<int> a(3), b(4, 4), c{ 3, 2, 1 }, x;
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

        ASSERT(x.max_size() == std::numeric_limits<std::size_t>::max());

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
        ASSERT(x.capacity() == 10);
        x.reserve(5);
        ASSERT(x.capacity() == 10);

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

        plastic::vector d{ 1, 2 }, e{ 1, 2, 2 }, f{ 1, 2, 3 };
        ASSERT(d == d); // NOLINT
        ASSERT(d != e);
        ASSERT(e != d);
        ASSERT(d < e);
        ASSERT(e > d);
        ASSERT(d <= f);
        ASSERT(f >= d);
    }

    TEST_METHOD(inplace_deque) {
        plastic::inplace_deque<int, 12> a(3), b(4, 4), c{ 3, 2, 1 }, x;
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

        ASSERT(x.max_size() == 12);

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
        ASSERT(x.capacity() == 12);
        x.reserve(6);
        ASSERT(x.capacity() == 12);

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

        plastic::inplace_deque<int, 12> d{ 1, 2 }, e{ 1, 2, 2 }, f{ 1, 2, 3 };
        ASSERT(d == d); // NOLINT
        ASSERT(d != e);
        ASSERT(e != d);
        ASSERT(d < e);
        ASSERT(e > d);
        ASSERT(d <= f);
        ASSERT(f >= d);
    }

    TEST_METHOD(deque) {
        plastic::deque<int> a(3), b(4, 4), c{ 3, 2, 1 }, x;
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

        ASSERT(x.max_size() == std::numeric_limits<std::size_t>::max());

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
        ASSERT(x.capacity() == 12);
        x.reserve(6);
        ASSERT(x.capacity() == 12);

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

        plastic::deque<int> d{ 1, 2 }, e{ 1, 2, 2 }, f{ 1, 2, 3 };
        ASSERT(d == d); // NOLINT
        ASSERT(d != e);
        ASSERT(e != d);
        ASSERT(d < e);
        ASSERT(e > d);
        ASSERT(d <= f);
        ASSERT(f >= d);
    }

    TEST_METHOD(forward_list) {
        plastic::forward_list<int> a(3), b(4, 4), c{ 3, 2, 1 }, x;
        ASSERT(format(x) == "[]");
        ASSERT(format(a) == "[0, 0, 0]");
        ASSERT(format(b) == "[4, 4, 4, 4]");
        ASSERT(format(c) == "[3, 2, 1]");

        ASSERT(format(c.cbegin(), c.cend()) == "[3, 2, 1]");

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

        ASSERT(x.max_size() == std::numeric_limits<std::size_t>::max());

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

        x.push_front(1);
        ASSERT(format(x) == "[1, 3, 2, 0, 1, 1]");
        x.push_front(2);
        ASSERT(format(x) == "[2, 1, 3, 2, 0, 1, 1]");
        x.push_front(3);
        ASSERT(format(x) == "[3, 2, 1, 3, 2, 0, 1, 1]");

        x.pop_front();
        ASSERT(format(x) == "[2, 1, 3, 2, 0, 1, 1]");
        x.pop_front();
        ASSERT(format(x) == "[1, 3, 2, 0, 1, 1]");
        x.pop_front();
        ASSERT(format(x) == "[3, 2, 0, 1, 1]");

        x.insert_after(++x.begin(), 1);
        ASSERT(format(x) == "[3, 2, 1, 0, 1, 1]");
        x.insert_after(std::next(x.begin(), 3), 2, 1);
        ASSERT(format(x) == "[3, 2, 1, 0, 1, 1, 1, 1]");
        x.insert_after(std::next(x.begin(), 4), { 2, 3, 2 });
        ASSERT(format(x) == "[3, 2, 1, 0, 1, 2, 3, 2, 1, 1, 1]");

        x.erase_after(x.begin());
        ASSERT(format(x) == "[3, 1, 0, 1, 2, 3, 2, 1, 1, 1]");
        x.erase_after(++x.begin(), std::next(x.begin(), 8));
        ASSERT(format(x) == "[3, 1, 1, 1]");

        plastic::forward_list d{ 1, 2 }, e{ 1, 2, 2 }, f{ 1, 2, 3 };
        ASSERT(d == d); // NOLINT
        ASSERT(d != e);
        ASSERT(e != d);
        ASSERT(d < e);
        ASSERT(e > d);
        ASSERT(d <= f);
        ASSERT(f >= d);
    }

    TEST_METHOD(list) {
        plastic::list<int> a(3), b(4, 4), c{ 3, 2, 1 }, x;
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

        ASSERT(x.max_size() == std::numeric_limits<std::size_t>::max());

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

        x.insert(std::next(x.begin(), 2), 1);
        ASSERT(format(x) == "[3, 2, 1, 0, 1, 1]");
        x.insert(--x.end(), 2, 2);
        ASSERT(format(x) == "[3, 2, 1, 0, 1, 2, 2, 1]");
        x.insert(std::next(x.begin(), 5), { 2, 3, 2 });
        ASSERT(format(x) == "[3, 2, 1, 0, 1, 2, 3, 2, 2, 2, 1]");

        x.erase(++x.begin());
        ASSERT(format(x) == "[3, 1, 0, 1, 2, 3, 2, 2, 2, 1]");
        x.erase(std::next(x.begin(), 2), std::prev(x.end(), 2));
        ASSERT(format(x) == "[3, 1, 2, 1]");

        plastic::list<int> d{ 1, 2 }, e{ 1, 2, 2 }, f{ 1, 2, 3 };
        ASSERT(d == d); // NOLINT
        ASSERT(d != e);
        ASSERT(e != d);
        ASSERT(d < e);
        ASSERT(e > d);
        ASSERT(d <= f);
        ASSERT(f >= d);
    }

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

    TEST_METHOD(binary_heap) {
        test_addressable_heap<plastic::binary_heap<int>>();
    }

};

TEST_CLASS(algorithm) {

public:
    TEST_METHOD(non_modifying_sequence) {
        std::vector<int> e, a{ 1, 3, 5, 7, 9 }, b{ 2, 4, 6, 8, 10 }, c{ 1, 2, 3, 2, 1 }, d{ 5, 5, 5, 5 }, x;

        ASSERT(plastic::all_of(e.begin(), e.end(), [](int x) { return x % 2 == 0; }) == true);
        ASSERT(plastic::all_of(a.begin(), a.end(), [](int x) { return x % 2 == 1; }) == true);
        ASSERT(plastic::all_of(b.begin(), b.end(), [](int x) { return x % 2 == 1; }) == false);
        ASSERT(plastic::all_of(c.begin(), c.end(), [](int x) { return x % 2 == 1; }) == false);

        ASSERT(plastic::any_of(e.begin(), e.end(), [](int x) { return x > 0; }) == false);
        ASSERT(plastic::any_of(a.begin(), a.end(), [](int x) { return x > 5; }) == true);
        ASSERT(plastic::any_of(b.begin(), b.end(), [](int x) { return x % 2 == 0; }) == true);
        ASSERT(plastic::any_of(c.begin(), c.end(), [](int x) { return x == 4; }) == false);

        ASSERT(plastic::none_of(e.begin(), e.end(), [](int x) { return x < 0; }) == true);
        ASSERT(plastic::none_of(a.begin(), a.end(), [](int x) { return x % 2 == 0; }) == true);
        ASSERT(plastic::none_of(b.begin(), b.end(), [](int x) { return x > 2; }) == false);
        ASSERT(plastic::none_of(c.begin(), c.end(), [](int x) { return x == 2; }) == false);

        x = { 1, 2, 3 };
        plastic::for_each(e.begin(), e.end(), [](int& x) { x += 10; });
        plastic::for_each(x.begin(), x.end(), [](int& x) { x += 10; });
        ASSERT(format(e) == "[]");
        ASSERT(format(x) == "[11, 12, 13]");

        x = { 1, 2, 3, 4, 5 };
        plastic::for_each_n(e.begin(), 0, [](int& x) { x += 10; });
        plastic::for_each_n(x.begin(), 3, [](int& x) { x *= 2; });
        ASSERT(format(e) == "[]");
        ASSERT(format(x) == "[2, 4, 6, 4, 5]");

        ASSERT(plastic::count(e.begin(), e.end(), 0) == 0);
        ASSERT(plastic::count(c.begin(), c.end(), 2) == 2);
        ASSERT(plastic::count(d.begin(), d.end(), 5) == 4);

        ASSERT(plastic::count_if(e.begin(), e.end(), [](int x) { return x == 0; }) == 0);
        ASSERT(plastic::count_if(a.begin(), a.end(), [](int x) { return x > 5; }) == 2);
        ASSERT(plastic::count_if(b.begin(), b.end(), [](int x) { return x % 4 == 2; }) == 3);

        ASSERT(plastic::mismatch(e.begin(), e.end(), e.begin(), e.end()).in1 == e.begin());
        ASSERT(plastic::mismatch(a.begin(), a.end(), c.begin(), c.end()).in1 == a.begin() + 1);
        ASSERT(plastic::mismatch(b.begin(), b.end(), b.begin(), b.end()).in2 == b.end());

        ASSERT(plastic::find(e.begin(), e.end(), 0) == e.end());
        ASSERT(plastic::find(a.begin(), a.end(), 7) == a.begin() + 3);
        ASSERT(plastic::find(b.begin(), b.end(), 5) == b.end());

        ASSERT(plastic::find_if(e.begin(), e.end(), [](int x) { return x == 0; }) == e.end());
        ASSERT(plastic::find_if(a.begin(), a.end(), [](int x) { return x < 0; }) == a.end());
        ASSERT(plastic::find_if(b.begin(), b.end(), [](int x) { return x > 5; }) == b.begin() + 2);

        ASSERT(plastic::find_if_not(e.begin(), e.end(), [](int x) { return x != 0; }) == e.end());
        ASSERT(plastic::find_if_not(c.begin(), c.end(), [](int x) { return x < 3; }) == c.begin() + 2);
        ASSERT(plastic::find_if_not(d.begin(), d.end(), [](int x) { return x == 5; }) == d.end());

        x = { 5, 5 };
        ASSERT(plastic::find_end(a.begin(), a.end(), e.begin(), e.end()).begin() == a.end());
        ASSERT(plastic::find_end(e.begin(), e.end(), a.begin(), a.end()).begin() == e.end());
        ASSERT(plastic::find_end(c.begin(), c.end(), x.begin(), x.end()).begin() == c.end());
        ASSERT(plastic::find_end(d.begin(), d.end(), x.begin(), x.end()).begin() == d.begin() + 2);

        x = { 0, 10 };
        ASSERT(plastic::find_first_of(a.begin(), a.end(), e.begin(), e.end()) == a.end());
        ASSERT(plastic::find_first_of(e.begin(), e.end(), a.begin(), a.end()) == e.end());
        ASSERT(plastic::find_first_of(a.begin(), a.end(), x.begin(), x.end()) == a.end());
        ASSERT(plastic::find_first_of(b.begin(), b.end(), x.begin(), x.end()) == b.begin() + 4);

        ASSERT(plastic::adjacent_find(e.begin(), e.end()) == e.end());
        ASSERT(plastic::adjacent_find(a.begin(), a.end()) == a.end());
        ASSERT(plastic::adjacent_find(d.begin(), d.end()) == d.begin());

        x = { 3, 5, 7 };
        ASSERT(plastic::search(a.begin(), a.end(), e.begin(), e.end()).begin() == a.begin());
        ASSERT(plastic::search(e.begin(), e.end(), a.begin(), a.end()).begin() == e.end());
        ASSERT(plastic::search(a.begin(), a.end(), x.begin(), x.end()).begin() == a.begin() + 1);
        ASSERT(plastic::search(b.begin(), b.end(), x.begin(), x.end()).begin() == b.end());

        ASSERT(plastic::search_n(e.begin(), e.end(), 0, 10).begin() == e.begin());
        ASSERT(plastic::search_n(e.begin(), e.end(), 10, 0).begin() == e.end());
        ASSERT(plastic::search_n(c.begin(), c.end(), 2, 1, std::ranges::greater{}).begin() == c.begin() + 1);
        ASSERT(plastic::search_n(d.begin(), d.end(), 4, 5).begin() == d.begin());

        ASSERT(plastic::contains(e.begin(), e.end(), 3) == false);
        ASSERT(plastic::contains(a.begin(), a.end(), 3) == true);
        ASSERT(plastic::contains(b.begin(), b.end(), 3) == false);
        ASSERT(plastic::contains(c.begin(), c.end(), 3) == true);

        x = { 3, 2, 1 };
        ASSERT(plastic::contains_subrange(e.begin(), e.end(), x.begin(), x.end()) == false);
        ASSERT(plastic::contains_subrange(a.begin(), a.end(), e.begin(), e.end()) == true);
        ASSERT(plastic::contains_subrange(a.begin(), a.end(), x.begin(), x.end()) == false);
        ASSERT(plastic::contains_subrange(b.begin(), b.end(), x.begin(), x.end()) == false);
        ASSERT(plastic::contains_subrange(c.begin(), c.end(), x.begin(), x.end()) == true);

        x = { 1, 3, 5 };
        ASSERT(plastic::starts_with(e.begin(), e.end(), x.begin(), x.end()) == false);
        ASSERT(plastic::starts_with(a.begin(), a.end(), e.begin(), e.end()) == true);
        ASSERT(plastic::starts_with(a.begin(), a.end(), x.begin(), x.end()) == true);
        ASSERT(plastic::starts_with(b.begin(), b.end(), x.begin(), x.end()) == false);
        ASSERT(plastic::starts_with(c.begin(), c.end(), x.begin(), x.end()) == false);

        x = { 3, 2, 1 };
        ASSERT(plastic::ends_with(e.begin(), e.end(), x.begin(), x.end()) == false);
        ASSERT(plastic::ends_with(a.begin(), a.end(), e.begin(), e.end()) == true);
        ASSERT(plastic::ends_with(a.begin(), a.end(), x.begin(), x.end()) == false);
        ASSERT(plastic::ends_with(b.begin(), b.end(), x.begin(), x.end()) == false);
        ASSERT(plastic::ends_with(c.begin(), c.end(), x.begin(), x.end()) == true);
    }

    TEST_METHOD(fold) {
        std::vector<int> e, a{ 1, 2, 3, 4, 5 }, b{ 2, 4, 6, 8, 10 };

        ASSERT(plastic::fold_left(e.begin(), e.end(), 10, std::plus{}) == 10);
        ASSERT(plastic::fold_left(a.begin(), a.end(), 0, std::plus{}) == 15);
        ASSERT(plastic::fold_left(a.begin(), a.end(), 10, std::minus{}) == -5);
        ASSERT(plastic::fold_left(b.begin(), b.end(), 1, std::multiplies{}) == 3840);

        ASSERT(!plastic::fold_left_first(e.begin(), e.end(), std::plus{}));
        ASSERT(*plastic::fold_left_first(a.begin(), a.end(), std::plus{}) == 15);
        ASSERT(*plastic::fold_left_first(b.begin(), b.end(), std::multiplies{}) == 3840);

        ASSERT(plastic::fold_right(e.begin(), e.end(), 100, std::plus{}) == 100);
        ASSERT(plastic::fold_right(a.begin(), a.end(), 0, std::plus{}) == 15);
        ASSERT(plastic::fold_right(b.begin(), b.end(), 2, std::minus{}) == 4);

        ASSERT(!plastic::fold_right_last(e.begin(), e.end(), std::plus{}));
        ASSERT(*plastic::fold_right_last(a.begin(), a.end(), std::plus{}) == 15);
        ASSERT(*plastic::fold_right_last(b.begin(), b.end(), std::minus{}) == 6);

        auto res1{ plastic::fold_left_with_iter(a.begin(), a.end(), 0, std::plus{}) };
        ASSERT(res1.value == 15 && res1.in == a.end());
        auto res2{ plastic::fold_left_with_iter(a.begin(), a.begin() + 3, 10, std::multiplies{}) };
        ASSERT(res2.value == 60 && res2.in == a.begin() + 3);

        auto res3{ plastic::fold_left_first_with_iter(e.begin(), e.end(), std::plus{}) };
        ASSERT(!res3.value && res3.in == e.end());
        auto res4{ plastic::fold_left_first_with_iter(b.begin(), b.end(), std::multiplies{}) };
        ASSERT(*res4.value == 3840 && res4.in == b.end());
    }

    TEST_METHOD(modifying_sequence) {
        std::vector<int> e, a{ 1, 3, 5, 7, 9 }, b{ 2, 4, 6, 8, 10 }, c{ 1, 2, 3, 2, 1 }, d{ 5, 5, 5, 5 }, x, y;

        x = { 0, 0, 0, 0, 0 };
        plastic::copy(e.begin(), e.end(), x.begin());
        ASSERT(format(x) == "[0, 0, 0, 0, 0]");
        plastic::copy(a.begin(), a.end(), x.begin());
        ASSERT(format(x) == "[1, 3, 5, 7, 9]");
        plastic::copy(d.begin(), d.end(), x.begin());
        ASSERT(format(x) == "[5, 5, 5, 5, 9]");

        x = { 0, 0, 0, 0, 0 };
        plastic::copy_if(e.begin(), e.end(), x.begin(), [](int x) { return x > 0; });
        ASSERT(format(x) == "[0, 0, 0, 0, 0]");
        plastic::copy_if(a.begin(), a.end(), x.begin(), [](int x) { return x > 3; });
        ASSERT(format(x) == "[5, 7, 9, 0, 0]");
        plastic::copy_if(d.begin(), d.end(), x.begin(), [](int x) { return x <= 5; });
        ASSERT(format(x) == "[5, 5, 5, 5, 0]");

        x = { 0, 0, 0, 0, 0 };
        plastic::copy_n(e.begin(), 0, x.begin());
        ASSERT(format(x) == "[0, 0, 0, 0, 0]");
        plastic::copy_n(a.begin(), 3, x.begin());
        ASSERT(format(x) == "[1, 3, 5, 0, 0]");
        plastic::copy_n(b.begin(), 4, x.begin());
        ASSERT(format(x) == "[2, 4, 6, 8, 0]");

        x = { 0, 0, 0, 0, 0 };
        plastic::copy_backward(e.begin(), e.end(), x.end());
        ASSERT(format(x) == "[0, 0, 0, 0, 0]");
        plastic::copy_backward(a.begin(), a.end(), x.end());
        ASSERT(format(x) == "[1, 3, 5, 7, 9]");
        plastic::copy_backward(d.begin(), d.end(), x.end());
        ASSERT(format(x) == "[1, 5, 5, 5, 5]");

        x = { 0, 0, 0, 0, 0 };
        y = { 1, 2, 3, 4, 5 };
        plastic::move(e.begin(), e.end(), x.begin());
        ASSERT(format(x) == "[0, 0, 0, 0, 0]");
        plastic::move(y.begin(), y.end(), x.begin());
        ASSERT(format(x) == "[1, 2, 3, 4, 5]");

        x = { 0, 0, 0, 0, 0 };
        y = { 1, 2, 3, 4, 5 };
        plastic::move_backward(e.begin(), e.end(), x.end());
        ASSERT(format(x) == "[0, 0, 0, 0, 0]");
        plastic::move_backward(y.begin(), y.end(), x.end());
        ASSERT(format(x) == "[1, 2, 3, 4, 5]");

        x = { 0, 0, 0, 0, 0 };
        plastic::fill(e.begin(), e.end(), 3);
        plastic::fill(x.begin(), x.end(), 3);
        ASSERT(format(e) == "[]");
        ASSERT(format(x) == "[3, 3, 3, 3, 3]");

        x = { 0, 0, 0, 0, 0 };
        plastic::fill_n(e.begin(), 0, 7);
        plastic::fill_n(x.begin(), 3, 7);
        ASSERT(format(e) == "[]");
        ASSERT(format(x) == "[7, 7, 7, 0, 0]");

        x = { 0, 0, 0, 0, 0 };
        plastic::transform(e.begin(), e.end(), x.begin(), [](int x) { return x * 2; });
        ASSERT(format(x) == "[0, 0, 0, 0, 0]");
        plastic::transform(a.begin(), a.end(), x.begin(), [](int x) { return x * 2; });
        ASSERT(format(x) == "[2, 6, 10, 14, 18]");
        plastic::transform(d.begin(), d.end(), x.begin(), [](int x) { return x + 1; });
        ASSERT(format(x) == "[6, 6, 6, 6, 18]");
        plastic::transform(e.begin(), e.end(), a.begin(), a.end(), x.begin(), [](int x, int y) { return x + y; });
        ASSERT(format(x) == "[6, 6, 6, 6, 18]");
        plastic::transform(a.begin(), a.end(), b.begin(), b.end(), x.begin(), [](int x, int y) { return y - x; });
        ASSERT(format(x) == "[1, 1, 1, 1, 1]");
        plastic::transform(
            a.begin(),
            a.end(),
            d.begin(),
            d.end(),
            x.begin(),
            [](int x, int y) { return y * 2 - x - 1; }
        );
        ASSERT(format(x) == "[8, 6, 4, 2, 1]");

        x = { 0, 0, 0, 0, 0 };
        plastic::generate(x.begin(), x.end(), [v{ 0 }] mutable { return v += 2; });
        ASSERT(format(x) == "[2, 4, 6, 8, 10]");
        plastic::generate(x.begin(), x.end(), [i{ a.begin() }] mutable { return *i++; });
        ASSERT(format(x) == "[1, 3, 5, 7, 9]");

        x = { 0, 0, 0, 0, 0 };
        plastic::generate_n(x.begin(), 4, [v{ 0 }] mutable { return v += 2; });
        ASSERT(format(x) == "[2, 4, 6, 8, 0]");
        plastic::generate_n(x.begin(), 5, [i{ a.begin() }] mutable { return *i++; });
        ASSERT(format(x) == "[1, 3, 5, 7, 9]");

        x = { 1, 2, 2, 2, 5 };
        x.erase(plastic::remove(x.begin(), x.end(), 2).begin(), x.end());
        ASSERT(format(x) == "[1, 5]");
        x.erase(plastic::remove(x.begin(), x.end(), 1).begin(), x.end());
        ASSERT(format(x) == "[5]");
        x.erase(plastic::remove(x.begin(), x.end(), 5).begin(), x.end());
        ASSERT(format(x) == "[]");
        x.erase(plastic::remove(x.begin(), x.end(), 0).begin(), x.end());
        ASSERT(format(x) == "[]");

        x = { 1, 2, 3, 2, 1 };
        x.erase(plastic::remove_if(x.begin(), x.end(), [](int x) { return x % 2 == 0; }).begin(), x.end());
        ASSERT(format(x) == "[1, 3, 1]");
        x.erase(plastic::remove_if(x.begin(), x.end(), [](int x) { return x >= 2; }).begin(), x.end());
        ASSERT(format(x) == "[1, 1]");
        x.erase(plastic::remove_if(x.begin(), x.end(), [](int x) { return x == 1; }).begin(), x.end());
        ASSERT(format(x) == "[]");
        x.erase(plastic::remove_if(x.begin(), x.end(), [](int x) { return true; }).begin(), x.end());
        ASSERT(format(x) == "[]");

        x = { 0, 0, 0, 0, 0 };
        plastic::remove_copy(e.begin(), e.end(), x.begin(), 2);
        ASSERT(format(x) == "[0, 0, 0, 0, 0]");
        plastic::remove_copy(c.begin(), c.end(), x.begin(), 2);
        ASSERT(format(x) == "[1, 3, 1, 0, 0]");
        plastic::remove_copy(d.begin(), d.end(), x.begin(), 5);
        ASSERT(format(x) == "[1, 3, 1, 0, 0]");

        x = { 0, 0, 0, 0, 0 };
        plastic::remove_copy_if(e.begin(), e.end(), x.begin(), [](int x) { return x > 0; });
        ASSERT(format(x) == "[0, 0, 0, 0, 0]");
        plastic::remove_copy_if(a.begin(), a.end(), x.begin(), [](int x) { return x < 5; });
        ASSERT(format(x) == "[5, 7, 9, 0, 0]");
        plastic::remove_copy_if(b.begin(), b.end(), x.begin(), [](int x) { return x > 5; });
        ASSERT(format(x) == "[2, 4, 9, 0, 0]");

        x = e;
        plastic::replace(x.begin(), x.end(), 2, 9);
        ASSERT(format(x) == "[]");
        x = c;
        plastic::replace(x.begin(), x.end(), 2, 9);
        ASSERT(format(x) == "[1, 9, 3, 9, 1]");
        x = d;
        plastic::replace(x.begin(), x.end(), 5, 0);
        ASSERT(format(x) == "[0, 0, 0, 0]");

        x = e;
        plastic::replace_if(x.begin(), x.end(), [](int x) { return x > 0; }, 0);
        ASSERT(format(x) == "[]");
        x = a;
        plastic::replace_if(x.begin(), x.end(), [](int x) { return x % 3 == 0; }, 0);
        ASSERT(format(x) == "[1, 0, 5, 7, 0]");
        x = b;
        plastic::replace_if(x.begin(), x.end(), [](int x) { return x < 6; }, 1);
        ASSERT(format(x) == "[1, 1, 6, 8, 10]");

        x = { 0, 0, 0, 0, 0 };
        plastic::replace_copy(e.begin(), e.end(), x.begin(), 2, 8);
        ASSERT(format(x) == "[0, 0, 0, 0, 0]");
        plastic::replace_copy(c.begin(), c.end(), x.begin(), 2, 8);
        ASSERT(format(x) == "[1, 8, 3, 8, 1]");
        plastic::replace_copy(d.begin(), d.end(), x.begin(), 5, 2);
        ASSERT(format(x) == "[2, 2, 2, 2, 1]");

        x = { 0, 0, 0, 0, 0 };
        plastic::replace_copy_if(e.begin(), e.end(), x.begin(), [](int x) { return x > 0; }, 0);
        ASSERT(format(x) == "[0, 0, 0, 0, 0]");
        plastic::replace_copy_if(a.begin(), a.end(), x.begin(), [](int x) { return x > 5; }, 1);
        ASSERT(format(x) == "[1, 3, 5, 1, 1]");
        plastic::replace_copy_if(b.begin(), b.end(), x.begin(), [](int x) { return x % 2 == 0; }, 7);
        ASSERT(format(x) == "[7, 7, 7, 7, 7]");

        x = { 0, 0, 0, 0, 0 };
        y = e;
        plastic::swap_ranges(x.begin(), x.end(), y.begin(), y.end());
        ASSERT(format(x) == "[0, 0, 0, 0, 0]");
        ASSERT(format(y) == "[]");
        y = b;
        plastic::swap_ranges(x.begin(), x.end(), y.begin(), y.end());
        ASSERT(format(x) == "[2, 4, 6, 8, 10]");
        ASSERT(format(y) == "[0, 0, 0, 0, 0]");
        y = d;
        plastic::swap_ranges(x.begin(), x.end(), y.begin(), y.end());
        ASSERT(format(x) == "[5, 5, 5, 5, 10]");
        ASSERT(format(y) == "[2, 4, 6, 8]");

        x = e;
        plastic::reverse(x.begin(), x.end());
        ASSERT(format(x) == "[]");
        x = a;
        plastic::reverse(x.begin(), x.end());
        ASSERT(format(x) == "[9, 7, 5, 3, 1]");
        x = c;
        plastic::reverse(x.begin(), x.begin() + 3);
        ASSERT(format(x) == "[3, 2, 1, 2, 1]");

        x = { 0, 0, 0, 0, 0 };
        plastic::reverse_copy(e.begin(), e.end(), x.begin());
        ASSERT(format(x) == "[0, 0, 0, 0, 0]");
        plastic::reverse_copy(a.begin(), a.end(), x.begin());
        ASSERT(format(x) == "[9, 7, 5, 3, 1]");
        plastic::reverse_copy(c.begin(), c.begin() + 3, x.begin());
        ASSERT(format(x) == "[3, 2, 1, 3, 1]");

        x = e;
        plastic::rotate(x.begin(), x.begin(), x.end());
        ASSERT(format(x) == "[]");
        x = a;
        plastic::rotate(x.begin(), x.begin() + 2, x.end());
        ASSERT(format(x) == "[5, 7, 9, 1, 3]");
        x = c;
        plastic::rotate(x.begin(), x.begin() + 1, x.end());
        ASSERT(format(x) == "[2, 3, 2, 1, 1]");

        x = { 0, 0, 0, 0, 0 };
        plastic::rotate_copy(e.begin(), e.begin(), e.end(), x.begin());
        ASSERT(format(x) == "[0, 0, 0, 0, 0]");
        plastic::rotate_copy(a.begin(), a.begin() + 1, a.end(), x.begin());
        ASSERT(format(x) == "[3, 5, 7, 9, 1]");
        plastic::rotate_copy(c.begin(), c.begin() + 2, c.end(), x.begin());
        ASSERT(format(x) == "[3, 2, 1, 1, 2]");

        x = e;
        plastic::shift_left(x.begin(), x.end(), 2);
        ASSERT(format(x) == "[]");
        x = a;
        plastic::shift_left(x.begin(), x.end(), 2);
        ASSERT(format(x.begin(), x.end() - 2) == "[5, 7, 9]");
        x = c;
        plastic::shift_left(x.begin(), x.end(), 1);
        ASSERT(format(x.begin(), x.end() - 1) == "[2, 3, 2, 1]");

        x = e;
        plastic::shift_right(x.begin(), x.end(), 1);
        ASSERT(format(x) == "[]");
        x = a;
        plastic::shift_right(x.begin(), x.end(), 1);
        ASSERT(format(x.begin() + 1, x.end()) == "[1, 3, 5, 7]");
        x = c;
        plastic::shift_right(x.begin(), x.end(), 2);
        ASSERT(format(x.begin() + 2, x.end()) == "[1, 2, 3]");

        std::mt19937 eng{ std::random_device{}() };
        x = { 0, 0, 0, 0, 0 };
        ASSERT(plastic::sample(e.begin(), e.end(), x.begin(), 3, eng) == x.begin());
        ASSERT(plastic::sample(a.begin(), a.end(), x.begin(), 3, eng) == x.begin() + 3);

        x = e;
        plastic::shuffle(x.begin(), x.end(), eng);
        ASSERT(format(e) == "[]");
        x = a;
        plastic::shuffle(x.begin(), x.end(), eng);
        ASSERT(std::ranges::is_permutation(x, a));
        x = d;
        plastic::shuffle(x.begin(), x.end(), eng);
        ASSERT(std::ranges::is_permutation(x, d));

        x = e;
        x.erase(plastic::unique(x.begin(), x.end()).begin(), x.end());
        ASSERT(format(x) == "[]");
        x = d;
        x.erase(plastic::unique(x.begin(), x.end()).begin(), x.end());
        ASSERT(format(x) == "[5]");
        x = { 1, 1, 2, 2, 3 };
        x.erase(plastic::unique(x.begin(), x.end()).begin(), x.end());
        ASSERT(format(x) == "[1, 2, 3]");

        x = { 0, 0, 0, 0, 0 };
        plastic::unique_copy(e.begin(), e.end(), x.begin());
        ASSERT(format(x) == "[0, 0, 0, 0, 0]");
        plastic::unique_copy(c.begin(), c.end(), x.begin());
        ASSERT(format(x) == "[1, 2, 3, 2, 1]");
        plastic::unique_copy(d.begin(), d.end(), x.begin());
        ASSERT(format(x) == "[5, 2, 3, 2, 1]");
    }

    TEST_METHOD(partitioning) {
        std::vector<int> e, a{ 1, 3, 5, 7, 9 }, b{ 2, 4, 6, 8, 10 }, c{ 1, 2, 3, 2, 1 }, d{ 5, 5, 5, 5 }, x, y;

        ASSERT(plastic::is_partitioned(e.begin(), e.end(), [](int x) { return x % 2 == 0; }) == true);
        ASSERT(plastic::is_partitioned(a.begin(), a.end(), [](int x) { return x % 2 == 1; }) == true);
        ASSERT(plastic::is_partitioned(b.begin(), b.end(), [](int x) { return x % 4 == 2; }) == false);
        ASSERT(plastic::is_partitioned(c.begin(), c.end(), [](int x) { return x < 3; }) == false);

        x = e;
        plastic::partition(x.begin(), x.end(), [](int x) { return x > 0; });
        ASSERT(format(x) == "[]");
        x = a;
        plastic::partition(x.begin(), x.end(), [](int x) { return x < 5; });
        ASSERT(std::ranges::is_partitioned(x, [](int x) { return x < 5; }));
        x = c;
        plastic::partition(x.begin(), x.end(), [](int x) { return x % 2 == 0; });
        ASSERT(std::ranges::is_partitioned(x, [](int x) { return x % 2 == 0; }));

        x = y = { 0, 0, 0, 0, 0 };
        plastic::partition_copy(e.begin(), e.end(), x.begin(), y.begin(), [](int x) { return x > 0; });
        ASSERT(format(x) == "[0, 0, 0, 0, 0]");
        ASSERT(format(y) == "[0, 0, 0, 0, 0]");
        plastic::partition_copy(a.begin(), a.end(), x.begin(), y.begin(), [](int x) { return x > 5; });
        ASSERT(format(x) == "[7, 9, 0, 0, 0]");
        ASSERT(format(y) == "[1, 3, 5, 0, 0]");
        plastic::partition_copy(c.begin(), c.end(), x.begin(), y.begin(), [](int x) { return x % 2 == 1; });
        ASSERT(format(x) == "[1, 3, 1, 0, 0]");
        ASSERT(format(y) == "[2, 2, 5, 0, 0]");

        x = e;
        plastic::stable_partition(x.begin(), x.end(), [](int x) { return x > 0; });
        ASSERT(format(x) == "[]");
        x = a;
        plastic::stable_partition(x.begin(), x.end(), [](int x) { return x % 3 == 1; });
        ASSERT(format(x) == "[1, 7, 3, 5, 9]");
        x = c;
        plastic::stable_partition(x.begin(), x.end(), [](int x) { return x % 2 == 0; });
        ASSERT(format(x) == "[2, 2, 1, 3, 1]");

        ASSERT(plastic::partition_point(e.begin(), e.end(), [](int x) { return x > 0; }) == e.begin());
        ASSERT(plastic::partition_point(a.begin(), a.end(), [](int x) { return x < 10; }) == a.end());
        ASSERT(plastic::partition_point(b.begin(), b.end(), [](int x) { return x <= 6; }) == b.begin() + 3);
    }

    TEST_METHOD(sorting) {
        std::vector<int> e, a{ 1, 2, 3, 4, 5 }, b{ 3, 4, 1, 5, 2 }, c(100), x;
        std::ranges::iota(c, 1);
        std::ranges::shuffle(c, std::mt19937{ std::random_device{}() });

        ASSERT(plastic::is_sorted(e.begin(), e.end()) == true);
        ASSERT(plastic::is_sorted(a.begin(), a.end()) == true);
        ASSERT(plastic::is_sorted(b.begin(), b.end()) == false);

        ASSERT(plastic::is_sorted_until(e.begin(), e.end()) == e.end());
        ASSERT(plastic::is_sorted_until(a.begin(), a.end()) == a.end());
        ASSERT(plastic::is_sorted_until(b.begin(), b.end()) == b.begin() + 2);

        x = e;
        plastic::sort(x.begin(), x.end());
        ASSERT(format(x) == "[]");
        x = b;
        plastic::sort(x.begin(), x.end());
        ASSERT(std::ranges::is_sorted(x));
        x = c;
        plastic::sort(x.begin(), x.end());
        ASSERT(std::ranges::is_sorted(x));

        x = e;
        plastic::partial_sort(x.begin(), x.begin(), x.end());
        ASSERT(format(x) == "[]");
        x = b;
        plastic::partial_sort(x.begin(), x.begin() + 3, x.end());
        ASSERT(format(x.begin(), x.begin() + 3) == "[1, 2, 3]");
        x = b;
        plastic::partial_sort(x.begin(), x.begin() + 4, x.end());
        ASSERT(format(x.begin(), x.begin() + 4) == "[1, 2, 3, 4]");

        x = { 0, 0, 0, 0 };
        plastic::partial_sort_copy(e.begin(), e.end(), x.begin(), x.end());
        ASSERT(format(x) == "[0, 0, 0, 0]");
        plastic::partial_sort_copy(a.begin(), a.end(), x.begin(), x.begin() + 3);
        ASSERT(format(x) == "[1, 2, 3, 0]");
        plastic::partial_sort_copy(b.begin(), b.end(), x.begin(), x.begin() + 4);
        ASSERT(format(x) == "[1, 2, 3, 4]");

        x = e;
        plastic::stable_sort(x.begin(), x.end());
        ASSERT(format(x) == "[]");
        x = b;
        plastic::stable_sort(x.begin(), x.end());
        ASSERT(std::ranges::is_sorted(x));
        x = c;
        plastic::stable_sort(x.begin(), x.end());
        ASSERT(std::ranges::is_sorted(x));

        x = e;
        plastic::nth_element(x.begin(), x.begin(), x.end());
        ASSERT(format(x) == "[]");
        x = b;
        plastic::nth_element(x.begin(), x.begin() + 2, x.end());
        ASSERT(x[2] == 3);
        x = c;
        plastic::nth_element(x.begin(), x.begin() + 50, x.end());
        ASSERT(x[50] == 51);
    }

    TEST_METHOD(binary_search) {
        std::vector<int> e, a{ 2, 4, 6, 8, 10 }, b{ 1, 2, 2, 3, 3 }, c{ 5, 5, 5, 5 };

        ASSERT(plastic::lower_bound(e.begin(), e.end(), 3) == e.end());
        ASSERT(plastic::lower_bound(a.begin(), a.end(), 5) == a.begin() + 2);
        ASSERT(plastic::lower_bound(b.begin(), b.end(), 2) == b.begin() + 1);
        ASSERT(plastic::lower_bound(c.begin(), c.end(), 5) == c.begin());

        ASSERT(plastic::upper_bound(e.begin(), e.end(), 3) == e.end());
        ASSERT(plastic::upper_bound(a.begin(), a.end(), 5) == a.begin() + 2);
        ASSERT(plastic::upper_bound(b.begin(), b.end(), 2) == b.begin() + 3);
        ASSERT(plastic::upper_bound(c.begin(), c.end(), 5) == c.end());

        ASSERT(plastic::binary_search(e.begin(), e.end(), 3) == false);
        ASSERT(plastic::binary_search(a.begin(), a.end(), 10) == true);
        ASSERT(plastic::binary_search(b.begin(), b.end(), 2) == true);
        ASSERT(plastic::binary_search(c.begin(), c.end(), 6) == false);

        auto res1{ plastic::equal_range(e.begin(), e.end(), 3) };
        ASSERT(res1.begin() == e.end() && res1.end() == e.end());
        auto res2{ plastic::equal_range(a.begin(), a.end(), 5) };
        ASSERT(res2.begin() == a.begin() + 2 && res2.end() == a.begin() + 2);
        auto res3{ plastic::equal_range(b.begin(), b.end(), 2) };
        ASSERT(res3.begin() == b.begin() + 1 && res3.end() == b.begin() + 3);
        auto res4{ plastic::equal_range(c.begin(), c.end(), 5) };
        ASSERT(res4.begin() == c.begin() && res4.end() == c.end());
    }

    TEST_METHOD(merge) {
        std::vector<int> e, a{ 1, 3, 5 }, b{ 2, 4, 6 }, c{ 1, 2, 3 }, x;

        x = { 0, 0, 0, 0, 0, 0 };
        plastic::merge(e.begin(), e.end(), e.begin(), e.end(), x.begin());
        ASSERT(format(x) == "[0, 0, 0, 0, 0, 0]");
        plastic::merge(a.begin(), a.end(), b.begin(), b.end(), x.begin());
        ASSERT(format(x) == "[1, 2, 3, 4, 5, 6]");
        plastic::merge(c.begin(), c.end(), c.begin(), c.end(), x.begin());
        ASSERT(format(x) == "[1, 1, 2, 2, 3, 3]");

        x = e;
        plastic::inplace_merge(x.begin(), x.begin(), x.end());
        ASSERT(format(x) == "[]");
        x = { 1, 3, 5, 2, 4, 6 };
        plastic::inplace_merge(x.begin(), x.begin() + 3, x.end());
        ASSERT(format(x) == "[1, 2, 3, 4, 5, 6]");
        x = { 1, 2, 3, 1, 2, 3 };
        plastic::inplace_merge(x.begin(), x.begin() + 3, x.end());
        ASSERT(format(x) == "[1, 1, 2, 2, 3, 3]");
    }

    TEST_METHOD(set) {
        std::vector<int> e, a{ 1, 2, 3, 5, 7 }, b{ 1, 3, 4, 6, 7 }, x;

        ASSERT(plastic::includes(e.begin(), e.end(), e.begin(), e.end()) == true);
        ASSERT(plastic::includes(a.begin(), a.end(), e.begin(), e.end()) == true);
        ASSERT(plastic::includes(e.begin(), e.end(), a.begin(), a.end()) == false);
        ASSERT(plastic::includes(a.begin(), a.end(), b.begin(), b.end()) == false);
        ASSERT(plastic::includes(a.begin(), a.end(), b.begin(), b.begin() + 2) == true);

        x = { 0, 0, 0, 0, 0 };
        plastic::set_difference(e.begin(), e.end(), e.begin(), e.end(), x.begin());
        ASSERT(format(x) == "[0, 0, 0, 0, 0]");
        plastic::set_difference(a.begin(), a.end(), e.begin(), e.end(), x.begin());
        ASSERT(format(x) == "[1, 2, 3, 5, 7]");
        plastic::set_difference(a.begin(), a.end(), b.begin(), b.end(), x.begin());
        ASSERT(format(x) == "[2, 5, 3, 5, 7]");
        plastic::set_difference(b.begin(), b.end(), a.begin(), a.end(), x.begin());
        ASSERT(format(x) == "[4, 6, 3, 5, 7]");

        x = { 0, 0, 0 };
        plastic::set_intersection(e.begin(), e.end(), e.begin(), e.end(), x.begin());
        ASSERT(format(x) == "[0, 0, 0]");
        plastic::set_intersection(a.begin(), a.end(), e.begin(), e.end(), x.begin());
        ASSERT(format(x) == "[0, 0, 0]");
        plastic::set_intersection(a.begin(), a.end(), b.begin(), b.end(), x.begin());
        ASSERT(format(x) == "[1, 3, 7]");
        plastic::set_intersection(b.begin(), b.end(), a.begin(), a.end(), x.begin());
        ASSERT(format(x) == "[1, 3, 7]");

        x = { 0, 0, 0, 0, 0 };
        plastic::set_symmetric_difference(e.begin(), e.end(), e.begin(), e.end(), x.begin());
        ASSERT(format(x) == "[0, 0, 0, 0, 0]");
        plastic::set_symmetric_difference(a.begin(), a.end(), e.begin(), e.end(), x.begin());
        ASSERT(format(x) == "[1, 2, 3, 5, 7]");
        plastic::set_symmetric_difference(a.begin(), a.end(), b.begin(), b.end(), x.begin());
        ASSERT(format(x) == "[2, 4, 5, 6, 7]");
        plastic::set_symmetric_difference(b.begin(), b.end(), a.begin(), a.end(), x.begin());
        ASSERT(format(x) == "[2, 4, 5, 6, 7]");

        x = { 0, 0, 0, 0, 0, 0, 0 };
        plastic::set_union(e.begin(), e.end(), e.begin(), e.end(), x.begin());
        ASSERT(format(x) == "[0, 0, 0, 0, 0, 0, 0]");
        plastic::set_union(a.begin(), a.end(), e.begin(), e.end(), x.begin());
        ASSERT(format(x) == "[1, 2, 3, 5, 7, 0, 0]");
        plastic::set_union(a.begin(), a.end(), b.begin(), b.end(), x.begin());
        ASSERT(format(x) == "[1, 2, 3, 4, 5, 6, 7]");
        plastic::set_union(b.begin(), b.end(), a.begin(), a.end(), x.begin());
        ASSERT(format(x) == "[1, 2, 3, 4, 5, 6, 7]");
    }

    TEST_METHOD(heap) {
        std::vector<int> e, a{ 1, 2, 3, 4, 5 }, b{ 5, 4, 3, 2, 1 }, x;

        ASSERT(plastic::is_heap(e.begin(), e.end()) == true);
        ASSERT(plastic::is_heap(a.begin(), a.end()) == false);
        ASSERT(plastic::is_heap(b.begin(), b.end()) == true);

        ASSERT(plastic::is_heap_until(e.begin(), e.end()) == e.end());
        ASSERT(plastic::is_heap_until(a.begin(), a.end()) == a.begin() + 1);
        ASSERT(plastic::is_heap_until(b.begin(), b.end()) == b.end());

        x = e;
        plastic::make_heap(x.begin(), x.end());
        ASSERT(format(x) == "[]");
        x = a;
        plastic::make_heap(x.begin(), x.end());
        ASSERT(std::ranges::is_heap(x));
        x = b;
        plastic::make_heap(x.begin(), x.end());
        ASSERT(std::ranges::is_heap(x));

        x = e;
        plastic::push_heap(x.begin(), x.end());
        ASSERT(format(x) == "[]");
        x = { 4, 1, 2, 0, 3 };
        plastic::push_heap(x.begin(), x.end());
        ASSERT(std::ranges::is_heap(x) && std::ranges::contains(x, 3));
        x = { 5, 4, 3, 2, 6 };
        plastic::push_heap(x.begin(), x.end());
        ASSERT(std::ranges::is_heap(x) && std::ranges::contains(x, 6));

        x = e;
        plastic::pop_heap(x.begin(), x.end());
        ASSERT(format(x) == "[]");
        x = { 4, 3, 1, 2, 0 };
        plastic::pop_heap(x.begin(), x.end());
        x.pop_back();
        ASSERT(std::ranges::is_heap(x) && !std::ranges::contains(x, 4));
        x = { 5, 4, 3, 2, 1 };
        plastic::pop_heap(x.begin(), x.end());
        x.pop_back();
        ASSERT(std::ranges::is_heap(x) && !std::ranges::contains(x, 5));

        x = e;
        plastic::sort_heap(x.begin(), x.end());
        ASSERT(format(x) == "[]");
        x = { 4, 3, 1, 2, 0 };
        plastic::sort_heap(x.begin(), x.end());
        ASSERT(format(x) == "[0, 1, 2, 3, 4]");
        x = { 5, 4, 3, 2, 1 };
        plastic::sort_heap(x.begin(), x.end());
        ASSERT(format(x) == "[1, 2, 3, 4, 5]");
    }

    TEST_METHOD(minimun_maximum) {
        std::vector<int> e, a{ 1, 3, 5, 7, 9 }, b{ 10, 8, 6, 4, 2 };

        ASSERT(plastic::max(0, 0) == 0);
        ASSERT(plastic::max(5, 1) == 5);
        ASSERT(plastic::max({ 0, 0, 0, 0, 0 }) == 0);
        ASSERT(plastic::max({ 5, 3, 2, 4, 1 }) == 5);

        ASSERT(plastic::max_element(e.begin(), e.end()) == e.end());
        ASSERT(plastic::max_element(a.begin(), a.end()) == a.end() - 1);
        ASSERT(plastic::max_element(b.begin(), b.end()) == b.begin());

        ASSERT(plastic::min(0, 0) == 0);
        ASSERT(plastic::min(5, 1) == 1);
        ASSERT(plastic::min({ 0, 0, 0, 0, 0 }) == 0);
        ASSERT(plastic::min({ 5, 3, 2, 4, 1 }) == 1);

        ASSERT(plastic::min_element(e.begin(), e.end()) == e.end());
        ASSERT(plastic::min_element(a.begin(), a.end()) == a.begin());
        ASSERT(plastic::min_element(b.begin(), b.end()) == b.end() - 1);

        auto res1{ plastic::minmax(0, 0) };
        ASSERT(res1.min == 0 && res1.max == 0);
        auto res2{ plastic::minmax(5, 1) };
        ASSERT(res2.min == 1 && res2.max == 5);
        auto res3{ plastic::minmax({ 0, 0, 0, 0, 0 }) };
        ASSERT(res3.min == 0 && res3.max == 0);
        auto res4{ plastic::minmax({ 5, 3, 2, 4, 1 }) };
        ASSERT(res4.min == 1 && res4.max == 5);

        auto res5{ plastic::minmax_element(e.begin(), e.end()) };
        ASSERT(res5.min == e.end() && res5.max == e.end());
        auto res6{ plastic::minmax_element(a.begin(), a.end()) };
        ASSERT(res6.min == a.begin() && res6.max == a.end() - 1);
        auto res7{ plastic::minmax_element(b.begin(), b.end()) };
        ASSERT(res7.min == b.end() - 1 && res7.max == b.begin());

        ASSERT(plastic::clamp(0, 1, 5) == 1);
        ASSERT(plastic::clamp(3, 1, 5) == 3);
        ASSERT(plastic::clamp(6, 1, 5) == 5);
        ASSERT(plastic::clamp(2, 2, 2) == 2);
    }

    TEST_METHOD(comparison) {
        std::vector<int> e, a{ 1, 2, 3, 4, 5 }, b{ 2, 4, 6, 8, 10 };

        ASSERT(plastic::equal(e.begin(), e.end(), e.begin(), e.end()) == true);
        ASSERT(plastic::equal(e.begin(), e.end(), a.begin(), a.end()) == false);
        ASSERT(plastic::equal(a.begin(), a.end(), b.begin(), b.end()) == false);
        ASSERT(plastic::equal(a.begin(), a.end(), b.begin(), b.end(), {}, [](int x) { return x * 2; }) == true);

        ASSERT(plastic::lexicographical_compare(e.begin(), e.end(), a.begin(), a.end()) == true);
        ASSERT(plastic::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end()) == true);
        ASSERT(plastic::lexicographical_compare(b.begin(), b.end(), a.begin(), a.end()) == false);
        ASSERT(
            plastic::lexicographical_compare(
                a.begin(),
                a.end(),
                b.begin(),
                b.end(),
                {},
                [](int x) { return x * 3; }
            ) == false
        );
    }

    TEST_METHOD(permutation) {
        std::string e, a{ "abc" }, b{ "cba" }, c{ "aab" }, d{ "abcd" };

        ASSERT(plastic::is_permutation(e.begin(), e.end(), e.begin(), e.end()) == true);
        ASSERT(plastic::is_permutation(a.begin(), a.end(), b.begin(), b.end()) == true);
        ASSERT(plastic::is_permutation(a.begin(), a.end(), c.begin(), c.end()) == false);
        ASSERT(plastic::is_permutation(a.begin(), a.end(), d.begin(), d.end()) == false);

        plastic::next_permutation(a.begin(), a.end());
        ASSERT(a == "acb");
        plastic::next_permutation(a.begin(), a.end());
        ASSERT(a == "bac");
        plastic::next_permutation(a.begin(), a.end());
        ASSERT(a == "bca");
        plastic::next_permutation(a.begin(), a.end());
        ASSERT(a == "cab");
        plastic::next_permutation(a.begin(), a.end());
        ASSERT(a == "cba");
        plastic::next_permutation(a.begin(), a.end());
        ASSERT(a == "abc");

        plastic::prev_permutation(a.begin(), a.end());
        ASSERT(a == "cba");
        plastic::prev_permutation(a.begin(), a.end());
        ASSERT(a == "cab");
        plastic::prev_permutation(a.begin(), a.end());
        ASSERT(a == "bca");
        plastic::prev_permutation(a.begin(), a.end());
        ASSERT(a == "bac");
        plastic::prev_permutation(a.begin(), a.end());
        ASSERT(a == "acb");
        plastic::prev_permutation(a.begin(), a.end());
        ASSERT(a == "abc");
    }

};
