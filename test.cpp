#include <CppUnitTest.h>

#define assert Microsoft::VisualStudio::CppUnitTestFramework::Assert::IsTrue

import std;
import plastic;

template<class T>
std::string format(const T& arg) {
    return std::format("{}", arg);
}

template<std::contiguous_iterator It>
std::string format(It first, std::size_t size) {
    return format(std::span{ first, size });
}

template<std::input_iterator It>
std::string format(It first, It last) {
    return format(std::ranges::subrange{ first, last });
}

template<class T, class Cmp>
std::string format(const plastic::binary_heap<T, Cmp>& heap) {
    std::vector<T> temp(heap.data(), heap.data() + heap.size());
    std::ranges::sort(temp, Cmp{});
    return format(temp);
}

TEST_CLASS(data_structure) {
public:

    TEST_METHOD(vector) {
        plastic::vector<int> a, b(4, 4), c{ 3, 2, 1 };
        assert(format(a) == "[]");
        assert(format(b) == "[4, 4, 4, 4]");
        assert(format(c) == "[3, 2, 1]");

        assert(format(c.data(), c.size()) == "[3, 2, 1]");
        assert(format(c.cbegin(), c.cend()) == "[3, 2, 1]");
        assert(format(c.rbegin(), c.rend()) == "[1, 2, 3]");
        assert(format(c.crbegin(), c.crend()) == "[1, 2, 3]");

        c = c;
        assert(format(c) == "[3, 2, 1]");
        c = b;
        assert(format(c) == "[4, 4, 4, 4]");

        assert(a.empty() == true);
        assert(b.empty() == false);

        assert(a.size() == 0);
        assert(b.size() == 4);

        c.clear();
        assert(format(c) == "[]");

        c.resize(2);
        assert(format(c) == "[0, 0]");
        c.resize(1);
        assert(format(c) == "[0]");
        c.resize(5, 1);
        assert(format(c) == "[0, 1, 1, 1, 1]");

        c.reserve(10);
        assert(c.capacity() == 10);
        c.reserve(5);
        assert(c.capacity() == 10);

        assert(b[1] == 4);
        assert(c[1] == 1);

        assert(b.front() == 4);
        assert(c.front() == 0);

        assert(b.back() == 4);
        assert(c.back() == 1);

        c.push_back(2);
        assert(format(c) == "[0, 1, 1, 1, 1, 2]");

        c.pop_back();
        c.pop_back();
        assert(format(c) == "[0, 1, 1, 1]");

        c.insert(c.begin() + 2, 3, 2);
        assert(format(c) == "[0, 1, 2, 2, 2, 1, 1]");
        c.insert(c.begin() + 3, { 3, 4, 5 });
        assert(format(c) == "[0, 1, 2, 3, 4, 5, 2, 2, 1, 1]");

        c.erase(c.begin() + 5);
        assert(format(c) == "[0, 1, 2, 3, 4, 2, 2, 1, 1]");
        c.erase(c.begin() + 5, c.end() - 1);
        assert(format(c) == "[0, 1, 2, 3, 4, 1]");

        plastic::vector d{ 1, 2 }, e{ 1, 2, 2 }, f{ 1, 2, 3 };
        assert(d == d);
        assert(d != e);
        assert(d < e);
        assert(d <= f);
    }

    TEST_METHOD(deque) {
        plastic::deque<int> a, b(4, 4), c{ 3, 2, 1 };
        assert(format(a) == "[]");
        assert(format(b) == "[4, 4, 4, 4]");
        assert(format(c) == "[3, 2, 1]");

        assert(format(c.cbegin(), c.cend()) == "[3, 2, 1]");
        assert(format(c.rbegin(), c.rend()) == "[1, 2, 3]");
        assert(format(c.crbegin(), c.crend()) == "[1, 2, 3]");

        c = c;
        assert(format(c) == "[3, 2, 1]");
        c = b;
        assert(format(c) == "[4, 4, 4, 4]");

        assert(a.empty() == true);
        assert(b.empty() == false);

        assert(a.size() == 0);
        assert(b.size() == 4);

        c.clear();
        assert(format(c) == "[]");

        c.resize(2);
        assert(format(c) == "[0, 0]");
        c.resize(1);
        assert(format(c) == "[0]");
        c.resize(5, 1);
        assert(format(c) == "[0, 1, 1, 1, 1]");

        c.reserve(10);
        assert(c.capacity() == 10);
        c.reserve(5);
        assert(c.capacity() == 10);

        assert(b[1] == 4);
        assert(c[1] == 1);

        assert(b.front() == 4);
        assert(c.front() == 0);

        assert(b.back() == 4);
        assert(c.back() == 1);

        c.push_front(-1);
        c.push_front(-1);
        assert(format(c) == "[-1, -1, 0, 1, 1, 1, 1]");

        c.pop_front();
        assert(format(c) == "[-1, 0, 1, 1, 1, 1]");

        c.push_back(2);
        assert(format(c) == "[-1, 0, 1, 1, 1, 1, 2]");

        c.pop_back();
        c.pop_back();
        assert(format(c) == "[-1, 0, 1, 1, 1]");

        c.insert(c.begin() + 3, 2, 2);
        assert(format(c) == "[-1, 0, 1, 2, 2, 1, 1]");
        c.insert(c.begin() + 4, { 3, 4, 5 });
        assert(format(c) == "[-1, 0, 1, 2, 3, 4, 5, 2, 1, 1]");

        c.erase(c.begin());
        assert(format(c) == "[0, 1, 2, 3, 4, 5, 2, 1, 1]");
        c.erase(c.begin() + 5, c.end() - 1);
        assert(format(c) == "[0, 1, 2, 3, 4, 1]");

        plastic::deque d{ 1, 2 }, e{ 1, 2, 2 }, f{ 1, 2, 3 };
        assert(d == d);
        assert(d != e);
        assert(d < e);
        assert(d <= f);
    }

    TEST_METHOD(forward_list) {
        plastic::forward_list<int> a, b(4, 4), c{ 3, 2, 1 };
        assert(format(a) == "[]");
        assert(format(b) == "[4, 4, 4, 4]");
        assert(format(c) == "[3, 2, 1]");

        assert(format(c.cbegin(), c.cend()) == "[3, 2, 1]");

        c = c;
        assert(format(c) == "[3, 2, 1]");
        c = b;
        assert(format(c) == "[4, 4, 4, 4]");

        assert(a.empty() == true);
        assert(b.empty() == false);

        assert(a.size() == 0);
        assert(b.size() == 4);

        c.clear();
        assert(format(c) == "[]");

        c.resize(2);
        assert(format(c) == "[0, 0]");
        c.resize(1);
        assert(format(c) == "[0]");
        c.resize(5, 1);
        assert(format(c) == "[0, 1, 1, 1, 1]");

        assert(b.front() == 4);
        assert(c.front() == 0);

        c.push_front(-1);
        c.push_front(-1);
        assert(format(c) == "[-1, -1, 0, 1, 1, 1, 1]");

        c.pop_front();
        assert(format(c) == "[-1, 0, 1, 1, 1, 1]");

        c.insert_after(std::next(c.begin(), 2), 2, 2);
        assert(format(c) == "[-1, 0, 1, 2, 2, 1, 1, 1]");
        c.insert_after(std::next(c.begin(), 3), { 3, 4, 5 });
        assert(format(c) == "[-1, 0, 1, 2, 3, 4, 5, 2, 1, 1, 1]");

        c.erase_after(c.end());
        assert(format(c) == "[0, 1, 2, 3, 4, 5, 2, 1, 1, 1]");
        c.erase_after(std::next(c.begin(), 4), c.end());
        assert(format(c) == "[0, 1, 2, 3, 4]");

        plastic::forward_list d{ 1, 2 }, e{ 1, 2, 2 }, f{ 1, 2, 3 };
        assert(d == d);
        assert(d != e);
        assert(d < e);
        assert(d <= f);
    }

    TEST_METHOD(list) {
        plastic::list<int> a, b(4, 4), c{ 3, 2, 1 };
        assert(format(a) == "[]");
        assert(format(b) == "[4, 4, 4, 4]");
        assert(format(c) == "[3, 2, 1]");

        assert(format(c.cbegin(), c.cend()) == "[3, 2, 1]");
        assert(format(c.rbegin(), c.rend()) == "[1, 2, 3]");
        assert(format(c.crbegin(), c.crend()) == "[1, 2, 3]");

        c = c;
        assert(format(c) == "[3, 2, 1]");
        c = b;
        assert(format(c) == "[4, 4, 4, 4]");

        assert(a.empty() == true);
        assert(b.empty() == false);

        assert(a.size() == 0);
        assert(b.size() == 4);

        c.clear();
        assert(format(c) == "[]");

        c.resize(2);
        assert(format(c) == "[0, 0]");
        c.resize(1);
        assert(format(c) == "[0]");
        c.resize(5, 1);
        assert(format(c) == "[0, 1, 1, 1, 1]");

        assert(b.front() == 4);
        assert(c.front() == 0);

        assert(b.back() == 4);
        assert(c.back() == 1);

        c.push_back(2);
        assert(format(c) == "[0, 1, 1, 1, 1, 2]");

        c.pop_back();
        c.pop_back();
        assert(format(c) == "[0, 1, 1, 1]");

        c.insert(std::next(c.begin(), 2), 3, 2);
        assert(format(c) == "[0, 1, 2, 2, 2, 1, 1]");
        c.insert(std::next(c.begin(), 3), { 3, 4, 5 });
        assert(format(c) == "[0, 1, 2, 3, 4, 5, 2, 2, 1, 1]");

        c.erase(std::next(c.begin(), 5));
        assert(format(c) == "[0, 1, 2, 3, 4, 2, 2, 1, 1]");
        c.erase(std::next(c.begin(), 5), std::prev(c.end()));
        assert(format(c) == "[0, 1, 2, 3, 4, 1]");

        plastic::list d{ 1, 2 }, e{ 1, 2, 2 }, f{ 1, 2, 3 };
        assert(d == d);
        assert(d != e);
        assert(d < e);
        assert(d <= f);
    }

    TEST_METHOD(binary_search_tree) {
        plastic::binary_search_tree<int> a, b{ 1, 2, 2, 2, 3 }, c{ 6, 4, 8, 2 };
        assert(format(a) == "[]");
        assert(format(b) == "[1, 2, 2, 2, 3]");
        assert(format(c) == "[2, 4, 6, 8]");

        assert(format(c.cbegin(), c.cend()) == "[2, 4, 6, 8]");
        assert(format(c.rbegin(), c.rend()) == "[8, 6, 4, 2]");
        assert(format(c.crbegin(), c.crend()) == "[8, 6, 4, 2]");

        c = c;
        assert(format(c) == "[2, 4, 6, 8]");
        c = b;
        assert(format(c) == "[1, 2, 2, 2, 3]");

        assert(a.empty() == true);
        assert(b.empty() == false);

        assert(a.size() == 0);
        assert(b.size() == 5);

        c.clear();
        assert(format(c) == "[]");

        assert(c.front() == 1);

        assert(c.back() == 3);

        assert(std::distance(c.begin(), c.lower_bound(1)) == 0);
        assert(std::distance(c.begin(), c.lower_bound(2)) == 1);
        assert(std::distance(c.begin(), c.lower_bound(3)) == 4);
        assert(std::distance(c.begin(), c.lower_bound(4)) == 5);

        assert(std::distance(c.begin(), c.upper_bound(1)) == 1);
        assert(std::distance(c.begin(), c.upper_bound(2)) == 4);
        assert(std::distance(c.begin(), c.upper_bound(3)) == 5);
        assert(std::distance(c.begin(), c.upper_bound(4)) == 5);

        assert(std::distance(c.begin(), c.find(1)) == 0);
        assert(std::distance(c.begin(), c.find(2)) == 1);
        assert(std::distance(c.begin(), c.find(3)) == 4);
        assert(std::distance(c.begin(), c.find(4)) == 5);

        assert(c.count(1) == 1);
        assert(c.count(2) == 3);
        assert(c.count(3) == 1);
        assert(c.count(4) == 0);

        assert(c.contains(1) == true);
        assert(c.contains(2) == true);
        assert(c.contains(3) == true);
        assert(c.contains(4) == false);

        c.insert(2);
        assert(format(c) == "[1, 2, 2, 2, 2, 3]");
        c.insert({ 3, 0, 4 });
        assert(format(c) == "[0, 1, 2, 2, 2, 2, 3, 3, 4]");

        c.erase(std::next(c.begin(), 5));
        assert(format(c) == "[0, 1, 2, 2, 2, 3, 3, 4]");
        c.erase(std::next(c.begin(), 5), std::prev(c.end()));
        assert(format(c) == "[0, 1, 2, 2, 2, 4]");

        plastic::binary_search_tree d{ 1, 2 }, e{ 1, 2, 2 }, f{ 1, 2, 3 };
        assert(d == d);
        assert(d != e);
        assert(d < e);
        assert(d <= f);
    }

    TEST_METHOD(binary_heap) {
        plastic::binary_heap<int> a, b{ 4, 4, 4, 4 }, c{ 3, 2, 1 };
        assert(format(a) == "[]");
        assert(format(b) == "[4, 4, 4, 4]");
        assert(format(c) == "[1, 2, 3]");

        c = c;
        assert(format(c) == "[1, 2, 3]");
        c = b;
        assert(format(c) == "[4, 4, 4, 4]");

        assert(a.empty() == true);
        assert(b.empty() == false);

        assert(a.size() == 0);
        assert(b.size() == 4);

        c.clear();
        assert(format(c) == "[]");

        assert(b.top() == 4);

        c.push(4);
        c.push(6);
        c.push(2);
        c.push(8);
        assert(format(c) == "[2, 4, 6, 8]");

        c.pop();
        assert(format(c) == "[2, 4, 6]");

        c.merge(plastic::binary_heap{ 3, 1, 7 });
        assert(format(c) == "[1, 2, 3, 4, 6, 7]");

        c.assign(c.data(), 5);
        assert(format(c) == "[1, 2, 3, 4, 5, 6]");

        c.erase(c.data());
        assert(format(c) == "[1, 2, 3, 4, 5]");
    }

};

TEST_CLASS(algorithm) {
public:

    TEST_METHOD(non_modification) {
        std::vector<int> e, a{ 1, 3, 5, 7, 9 }, b{ 2, 4, 6, 8, 10 }, c{ 1, 2, 3, 2, 1 }, d{ 5, 5, 5, 5 }, x;

        assert(plastic::all_of(e.begin(), e.end(), [](int x) { return x % 2 == 0; }) == true);
        assert(plastic::all_of(a.begin(), a.end(), [](int x) { return x % 2 == 1; }) == true);
        assert(plastic::all_of(b.begin(), b.end(), [](int x) { return x % 2 == 1; }) == false);
        assert(plastic::all_of(c.begin(), c.end(), [](int x) { return x % 2 == 1; }) == false);

        assert(plastic::any_of(e.begin(), e.end(), [](int x) { return x > 0; }) == false);
        assert(plastic::any_of(a.begin(), a.end(), [](int x) { return x > 5; }) == true);
        assert(plastic::any_of(b.begin(), b.end(), [](int x) { return x % 2 == 0; }) == true);
        assert(plastic::any_of(c.begin(), c.end(), [](int x) { return x == 4; }) == false);

        assert(plastic::none_of(e.begin(), e.end(), [](int x) { return x < 0; }) == true);
        assert(plastic::none_of(a.begin(), a.end(), [](int x) { return x % 2 == 0; }) == true);
        assert(plastic::none_of(b.begin(), b.end(), [](int x) { return x > 2; }) == false);
        assert(plastic::none_of(c.begin(), c.end(), [](int x) { return x == 2; }) == false);

        x = { 1, 2, 3 };
        plastic::for_each(e.begin(), e.end(), [](int& x) { x += 10; });
        plastic::for_each(x.begin(), x.end(), [](int& x) { x += 10; });
        assert(format(e) == "[]");
        assert(format(x) == "[11, 12, 13]");

        x = { 1, 2, 3, 4, 5 };
        plastic::for_each_n(e.begin(), 0, [](int& x) { x += 10; });
        plastic::for_each_n(x.begin(), 3, [](int& x) { x *= 2; });
        assert(format(e) == "[]");
        assert(format(x) == "[2, 4, 6, 4, 5]");

        assert(plastic::count(e.begin(), e.end(), 0) == 0);
        assert(plastic::count(c.begin(), c.end(), 2) == 2);
        assert(plastic::count(d.begin(), d.end(), 5) == 4);

        assert(plastic::count_if(e.begin(), e.end(), [](int x) { return x == 0; }) == 0);
        assert(plastic::count_if(a.begin(), a.end(), [](int x) { return x > 5; }) == 2);
        assert(plastic::count_if(b.begin(), b.end(), [](int x) { return x % 4 == 2; }) == 3);

        assert(plastic::mismatch(e.begin(), e.end(), e.begin(), e.end()).in1 == e.begin());
        assert(plastic::mismatch(a.begin(), a.end(), c.begin(), c.end()).in1 == a.begin() + 1);
        assert(plastic::mismatch(b.begin(), b.end(), b.begin(), b.end()).in2 == b.end());

        assert(plastic::find(e.begin(), e.end(), 0) == e.end());
        assert(plastic::find(a.begin(), a.end(), 7) == a.begin() + 3);
        assert(plastic::find(b.begin(), b.end(), 5) == b.end());

        assert(plastic::find_if(e.begin(), e.end(), [](int x) { return x == 0; }) == e.end());
        assert(plastic::find_if(a.begin(), a.end(), [](int x) { return x < 0; }) == a.end());
        assert(plastic::find_if(b.begin(), b.end(), [](int x) { return x > 5; }) == b.begin() + 2);

        assert(plastic::find_if_not(e.begin(), e.end(), [](int x) { return x != 0; }) == e.end());
        assert(plastic::find_if_not(c.begin(), c.end(), [](int x) { return x < 3; }) == c.begin() + 2);
        assert(plastic::find_if_not(d.begin(), d.end(), [](int x) { return x == 5; }) == d.end());

        x = { 5, 5 };
        assert(plastic::find_end(a.begin(), a.end(), e.begin(), e.end()).begin() == a.end());
        assert(plastic::find_end(e.begin(), e.end(), a.begin(), a.end()).begin() == e.end());
        assert(plastic::find_end(c.begin(), c.end(), x.begin(), x.end()).begin() == c.end());
        assert(plastic::find_end(d.begin(), d.end(), x.begin(), x.end()).begin() == d.begin() + 2);

        x = { 0, 10 };
        assert(plastic::find_first_of(a.begin(), a.end(), e.begin(), e.end()) == a.end());
        assert(plastic::find_first_of(e.begin(), e.end(), a.begin(), a.end()) == e.end());
        assert(plastic::find_first_of(a.begin(), a.end(), x.begin(), x.end()) == a.end());
        assert(plastic::find_first_of(b.begin(), b.end(), x.begin(), x.end()) == b.begin() + 4);

        assert(plastic::adjacent_find(e.begin(), e.end()) == e.end());
        assert(plastic::adjacent_find(a.begin(), a.end()) == a.end());
        assert(plastic::adjacent_find(d.begin(), d.end()) == d.begin());

        x = { 3, 5, 7 };
        assert(plastic::search(a.begin(), a.end(), e.begin(), e.end()).begin() == a.begin());
        assert(plastic::search(e.begin(), e.end(), a.begin(), a.end()).begin() == e.end());
        assert(plastic::search(a.begin(), a.end(), x.begin(), x.end()).begin() == a.begin() + 1);
        assert(plastic::search(b.begin(), b.end(), x.begin(), x.end()).begin() == b.end());

        assert(plastic::search_n(e.begin(), e.end(), 0, 10).begin() == e.begin());
        assert(plastic::search_n(e.begin(), e.end(), 10, 0).begin() == e.end());
        assert(plastic::search_n(c.begin(), c.end(), 2, 1, std::ranges::greater{}).begin() == c.begin() + 1);
        assert(plastic::search_n(d.begin(), d.end(), 4, 5).begin() == d.begin());

        assert(plastic::contains(e.begin(), e.end(), 3) == false);
        assert(plastic::contains(a.begin(), a.end(), 3) == true);
        assert(plastic::contains(b.begin(), b.end(), 3) == false);
        assert(plastic::contains(c.begin(), c.end(), 3) == true);

        x = { 3, 2, 1 };
        assert(plastic::contains_subrange(e.begin(), e.end(), x.begin(), x.end()) == false);
        assert(plastic::contains_subrange(a.begin(), a.end(), e.begin(), e.end()) == true);
        assert(plastic::contains_subrange(a.begin(), a.end(), x.begin(), x.end()) == false);
        assert(plastic::contains_subrange(b.begin(), b.end(), x.begin(), x.end()) == false);
        assert(plastic::contains_subrange(c.begin(), c.end(), x.begin(), x.end()) == true);

        x = { 1, 3, 5 };
        assert(plastic::starts_with(e.begin(), e.end(), x.begin(), x.end()) == false);
        assert(plastic::starts_with(a.begin(), a.end(), e.begin(), e.end()) == true);
        assert(plastic::starts_with(a.begin(), a.end(), x.begin(), x.end()) == true);
        assert(plastic::starts_with(b.begin(), b.end(), x.begin(), x.end()) == false);
        assert(plastic::starts_with(c.begin(), c.end(), x.begin(), x.end()) == false);

        x = { 3, 2, 1 };
        assert(plastic::ends_with(e.begin(), e.end(), x.begin(), x.end()) == false);
        assert(plastic::ends_with(a.begin(), a.end(), e.begin(), e.end()) == true);
        assert(plastic::ends_with(a.begin(), a.end(), x.begin(), x.end()) == false);
        assert(plastic::ends_with(b.begin(), b.end(), x.begin(), x.end()) == false);
        assert(plastic::ends_with(c.begin(), c.end(), x.begin(), x.end()) == true);
    }

    TEST_METHOD(fold) {
        std::vector<int> e, a{ 1, 2, 3, 4, 5 }, b{ 2, 4, 6, 8, 10 };

        assert(plastic::fold_left(e.begin(), e.end(), 10, std::plus{}) == 10);
        assert(plastic::fold_left(a.begin(), a.end(), 0, std::plus{}) == 15);
        assert(plastic::fold_left(a.begin(), a.end(), 10, std::minus{}) == -5);
        assert(plastic::fold_left(b.begin(), b.end(), 1, std::multiplies{}) == 3840);

        assert(!plastic::fold_left_first(e.begin(), e.end(), std::plus{}));
        assert(*plastic::fold_left_first(a.begin(), a.end(), std::plus{}) == 15);
        assert(*plastic::fold_left_first(b.begin(), b.end(), std::multiplies{}) == 3840);

        assert(plastic::fold_right(e.begin(), e.end(), 100, std::plus{}) == 100);
        assert(plastic::fold_right(a.begin(), a.end(), 0, std::plus{}) == 15);
        assert(plastic::fold_right(b.begin(), b.end(), 2, std::minus{}) == 4);

        assert(!plastic::fold_right_last(e.begin(), e.end(), std::plus{}));
        assert(*plastic::fold_right_last(a.begin(), a.end(), std::plus{}) == 15);
        assert(*plastic::fold_right_last(b.begin(), b.end(), std::minus{}) == 6);

        auto res1{ plastic::fold_left_with_iter(a.begin(), a.end(), 0, std::plus{}) };
        assert(res1.value == 15 && res1.in == a.end());
        auto res2{ plastic::fold_left_with_iter(a.begin(), a.begin() + 3, 10, std::multiplies{}) };
        assert(res2.value == 60 && res2.in == a.begin() + 3);

        auto res3{ plastic::fold_left_first_with_iter(e.begin(), e.end(), std::plus{}) };
        assert(!res3.value && res3.in == e.end());
        auto res4{ plastic::fold_left_first_with_iter(b.begin(), b.end(), std::multiplies{}) };
        assert(*res4.value == 3840 && res4.in == b.end());
    }

    TEST_METHOD(modification) {
        std::vector<int> e, a{ 1, 3, 5, 7, 9 }, b{ 2, 4, 6, 8, 10 }, c{ 1, 2, 3, 2, 1 }, d{ 5, 5, 5, 5 }, x, y;

        x = { 0, 0, 0, 0, 0 };
        plastic::copy(e.begin(), e.end(), x.begin());
        assert(format(x) == "[0, 0, 0, 0, 0]");
        plastic::copy(a.begin(), a.end(), x.begin());
        assert(format(x) == "[1, 3, 5, 7, 9]");
        plastic::copy(d.begin(), d.end(), x.begin());
        assert(format(x) == "[5, 5, 5, 5, 9]");

        x = { 0, 0, 0, 0, 0 };
        plastic::copy_if(e.begin(), e.end(), x.begin(), [](int x) { return x > 0; });
        assert(format(x) == "[0, 0, 0, 0, 0]");
        plastic::copy_if(a.begin(), a.end(), x.begin(), [](int x) { return x > 3; });
        assert(format(x) == "[5, 7, 9, 0, 0]");
        plastic::copy_if(d.begin(), d.end(), x.begin(), [](int x) { return x <= 5; });
        assert(format(x) == "[5, 5, 5, 5, 0]");

        x = { 0, 0, 0, 0, 0 };
        plastic::copy_n(e.begin(), 0, x.begin());
        assert(format(x) == "[0, 0, 0, 0, 0]");
        plastic::copy_n(a.begin(), 3, x.begin());
        assert(format(x) == "[1, 3, 5, 0, 0]");
        plastic::copy_n(b.begin(), 4, x.begin());
        assert(format(x) == "[2, 4, 6, 8, 0]");

        x = { 0, 0, 0, 0, 0 };
        plastic::copy_backward(e.begin(), e.end(), x.end());
        assert(format(x) == "[0, 0, 0, 0, 0]");
        plastic::copy_backward(a.begin(), a.end(), x.end());
        assert(format(x) == "[1, 3, 5, 7, 9]");
        plastic::copy_backward(d.begin(), d.end(), x.end());
        assert(format(x) == "[1, 5, 5, 5, 5]");

        x = { 0, 0, 0, 0, 0 };
        y = { 1, 2, 3, 4, 5 };
        plastic::move(e.begin(), e.end(), x.begin());
        assert(format(x) == "[0, 0, 0, 0, 0]");
        plastic::move(y.begin(), y.end(), x.begin());
        assert(format(x) == "[1, 2, 3, 4, 5]");

        x = { 0, 0, 0, 0, 0 };
        y = { 1, 2, 3, 4, 5 };
        plastic::move_backward(e.begin(), e.end(), x.end());
        assert(format(x) == "[0, 0, 0, 0, 0]");
        plastic::move_backward(y.begin(), y.end(), x.end());
        assert(format(x) == "[1, 2, 3, 4, 5]");

        x = { 0, 0, 0, 0, 0 };
        plastic::fill(e.begin(), e.end(), 3);
        plastic::fill(x.begin(), x.end(), 3);
        assert(format(e) == "[]");
        assert(format(x) == "[3, 3, 3, 3, 3]");

        x = { 0, 0, 0, 0, 0 };
        plastic::fill_n(e.begin(), 0, 7);
        plastic::fill_n(x.begin(), 3, 7);
        assert(format(e) == "[]");
        assert(format(x) == "[7, 7, 7, 0, 0]");

        x = { 0, 0, 0, 0, 0 };
        plastic::transform(e.begin(), e.end(), x.begin(), [](int x) { return x * 2; });
        assert(format(x) == "[0, 0, 0, 0, 0]");
        plastic::transform(a.begin(), a.end(), x.begin(), [](int x) { return x * 2; });
        assert(format(x) == "[2, 6, 10, 14, 18]");
        plastic::transform(d.begin(), d.end(), x.begin(), [](int x) { return x + 1; });
        assert(format(x) == "[6, 6, 6, 6, 18]");
        plastic::transform(e.begin(), e.end(), a.begin(), a.end(), x.begin(), [](int x, int y) { return x + y; });
        assert(format(x) == "[6, 6, 6, 6, 18]");
        plastic::transform(a.begin(), a.end(), b.begin(), b.end(), x.begin(), [](int x, int y) { return y - x; });
        assert(format(x) == "[1, 1, 1, 1, 1]");
        plastic::transform(a.begin(), a.end(), d.begin(), d.end(), x.begin(), [](int x, int y) { return y * 2 - x - 1; });
        assert(format(x) == "[8, 6, 4, 2, 1]");

        x = { 0, 0, 0, 0, 0 };
        plastic::generate(x.begin(), x.end(), [v{ 0 }] mutable { return v += 2; });
        assert(format(x) == "[2, 4, 6, 8, 10]");
        plastic::generate(x.begin(), x.end(), [i{ a.begin() }] mutable { return *i++; });
        assert(format(x) == "[1, 3, 5, 7, 9]");

        x = { 0, 0, 0, 0, 0 };
        plastic::generate_n(x.begin(), 4, [v{ 0 }] mutable { return v += 2; });
        assert(format(x) == "[2, 4, 6, 8, 0]");
        plastic::generate_n(x.begin(), 5, [i{ a.begin() }] mutable { return *i++; });
        assert(format(x) == "[1, 3, 5, 7, 9]");

        x = { 1, 2, 2, 2, 5 };
        x.erase(plastic::remove(x.begin(), x.end(), 2).begin(), x.end());
        assert(format(x) == "[1, 5]");
        x.erase(plastic::remove(x.begin(), x.end(), 1).begin(), x.end());
        assert(format(x) == "[5]");
        x.erase(plastic::remove(x.begin(), x.end(), 5).begin(), x.end());
        assert(format(x) == "[]");
        x.erase(plastic::remove(x.begin(), x.end(), 0).begin(), x.end());
        assert(format(x) == "[]");

        x = { 1, 2, 3, 2, 1 };
        x.erase(plastic::remove_if(x.begin(), x.end(), [](int x) { return x % 2 == 0; }).begin(), x.end());
        assert(format(x) == "[1, 3, 1]");
        x.erase(plastic::remove_if(x.begin(), x.end(), [](int x) { return x >= 2; }).begin(), x.end());
        assert(format(x) == "[1, 1]");
        x.erase(plastic::remove_if(x.begin(), x.end(), [](int x) { return x == 1; }).begin(), x.end());
        assert(format(x) == "[]");
        x.erase(plastic::remove_if(x.begin(), x.end(), [](int x) { return true; }).begin(), x.end());
        assert(format(x) == "[]");

        x = { 0, 0, 0, 0, 0 };
        plastic::remove_copy(e.begin(), e.end(), x.begin(), 2);
        assert(format(x) == "[0, 0, 0, 0, 0]");
        plastic::remove_copy(c.begin(), c.end(), x.begin(), 2);
        assert(format(x) == "[1, 3, 1, 0, 0]");
        plastic::remove_copy(d.begin(), d.end(), x.begin(), 5);
        assert(format(x) == "[1, 3, 1, 0, 0]");

        x = { 0, 0, 0, 0, 0 };
        plastic::remove_copy_if(e.begin(), e.end(), x.begin(), [](int x) { return x > 0; });
        assert(format(x) == "[0, 0, 0, 0, 0]");
        plastic::remove_copy_if(a.begin(), a.end(), x.begin(), [](int x) { return x < 5; });
        assert(format(x) == "[5, 7, 9, 0, 0]");
        plastic::remove_copy_if(b.begin(), b.end(), x.begin(), [](int x) { return x > 5; });
        assert(format(x) == "[2, 4, 9, 0, 0]");

        x = e;
        plastic::replace(x.begin(), x.end(), 2, 9);
        assert(format(x) == "[]");
        x = c;
        plastic::replace(x.begin(), x.end(), 2, 9);
        assert(format(x) == "[1, 9, 3, 9, 1]");
        x = d;
        plastic::replace(x.begin(), x.end(), 5, 0);
        assert(format(x) == "[0, 0, 0, 0]");

        x = e;
        plastic::replace_if(x.begin(), x.end(), [](int x) { return x > 0; }, 0);
        assert(format(x) == "[]");
        x = a;
        plastic::replace_if(x.begin(), x.end(), [](int x) { return x % 3 == 0; }, 0);
        assert(format(x) == "[1, 0, 5, 7, 0]");
        x = b;
        plastic::replace_if(x.begin(), x.end(), [](int x) { return x < 6; }, 1);
        assert(format(x) == "[1, 1, 6, 8, 10]");

        x = { 0, 0, 0, 0, 0 };
        plastic::replace_copy(e.begin(), e.end(), x.begin(), 2, 8);
        assert(format(x) == "[0, 0, 0, 0, 0]");
        plastic::replace_copy(c.begin(), c.end(), x.begin(), 2, 8);
        assert(format(x) == "[1, 8, 3, 8, 1]");
        plastic::replace_copy(d.begin(), d.end(), x.begin(), 5, 2);
        assert(format(x) == "[2, 2, 2, 2, 1]");

        x = { 0, 0, 0, 0, 0 };
        plastic::replace_copy_if(e.begin(), e.end(), x.begin(), [](int x) { return x > 0; }, 0);
        assert(format(x) == "[0, 0, 0, 0, 0]");
        plastic::replace_copy_if(a.begin(), a.end(), x.begin(), [](int x) { return x > 5; }, 1);
        assert(format(x) == "[1, 3, 5, 1, 1]");
        plastic::replace_copy_if(b.begin(), b.end(), x.begin(), [](int x) { return x % 2 == 0; }, 7);
        assert(format(x) == "[7, 7, 7, 7, 7]");

        x = { 0, 0, 0, 0, 0 };
        y = e;
        plastic::swap_ranges(x.begin(), x.end(), y.begin(), y.end());
        assert(format(x) == "[0, 0, 0, 0, 0]");
        assert(format(y) == "[]");
        y = b;
        plastic::swap_ranges(x.begin(), x.end(), y.begin(), y.end());
        assert(format(x) == "[2, 4, 6, 8, 10]");
        assert(format(y) == "[0, 0, 0, 0, 0]");
        y = d;
        plastic::swap_ranges(x.begin(), x.end(), y.begin(), y.end());
        assert(format(x) == "[5, 5, 5, 5, 10]");
        assert(format(y) == "[2, 4, 6, 8]");

        x = e;
        plastic::reverse(x.begin(), x.end());
        assert(format(x) == "[]");
        x = a;
        plastic::reverse(x.begin(), x.end());
        assert(format(x) == "[9, 7, 5, 3, 1]");
        x = c;
        plastic::reverse(x.begin(), x.begin() + 3);
        assert(format(x) == "[3, 2, 1, 2, 1]");

        x = { 0, 0, 0, 0, 0 };
        plastic::reverse_copy(e.begin(), e.end(), x.begin());
        assert(format(x) == "[0, 0, 0, 0, 0]");
        plastic::reverse_copy(a.begin(), a.end(), x.begin());
        assert(format(x) == "[9, 7, 5, 3, 1]");
        plastic::reverse_copy(c.begin(), c.begin() + 3, x.begin());
        assert(format(x) == "[3, 2, 1, 3, 1]");

        x = e;
        plastic::rotate(x.begin(), x.begin(), x.end());
        assert(format(x) == "[]");
        x = a;
        plastic::rotate(x.begin(), x.begin() + 2, x.end());
        assert(format(x) == "[5, 7, 9, 1, 3]");
        x = c;
        plastic::rotate(x.begin(), x.begin() + 1, x.end());
        assert(format(x) == "[2, 3, 2, 1, 1]");

        x = { 0, 0, 0, 0, 0 };
        plastic::rotate_copy(e.begin(), e.begin(), e.end(), x.begin());
        assert(format(x) == "[0, 0, 0, 0, 0]");
        plastic::rotate_copy(a.begin(), a.begin() + 1, a.end(), x.begin());
        assert(format(x) == "[3, 5, 7, 9, 1]");
        plastic::rotate_copy(c.begin(), c.begin() + 2, c.end(), x.begin());
        assert(format(x) == "[3, 2, 1, 1, 2]");

        x = e;
        plastic::shift_left(x.begin(), x.end(), 2);
        assert(format(x) == "[]");
        x = a;
        plastic::shift_left(x.begin(), x.end(), 2);
        assert(format(x.begin(), x.end() - 2) == "[5, 7, 9]");
        x = c;
        plastic::shift_left(x.begin(), x.end(), 1);
        assert(format(x.begin(), x.end() - 1) == "[2, 3, 2, 1]");

        x = e;
        plastic::shift_right(x.begin(), x.end(), 1);
        assert(format(x) == "[]");
        x = a;
        plastic::shift_right(x.begin(), x.end(), 1);
        assert(format(x.begin() + 1, x.end()) == "[1, 3, 5, 7]");
        x = c;
        plastic::shift_right(x.begin(), x.end(), 2);
        assert(format(x.begin() + 2, x.end()) == "[1, 2, 3]");

        std::mt19937 eng{ std::random_device{}() };
        x = { 0, 0, 0, 0, 0 };
        assert(plastic::sample(e.begin(), e.end(), x.begin(), 3, eng) == x.begin());
        assert(plastic::sample(a.begin(), a.end(), x.begin(), 3, eng) == x.begin() + 3);

        x = e;
        plastic::shuffle(x.begin(), x.end(), eng);
        assert(std::ranges::is_permutation(x.begin(), x.end(), e.begin(), e.end()));
        x = a;
        plastic::shuffle(x.begin(), x.end(), eng);
        assert(std::ranges::is_permutation(x.begin(), x.end(), a.begin(), a.end()));
        x = d;
        plastic::shuffle(x.begin(), x.end(), eng);
        assert(std::ranges::is_permutation(x.begin(), x.end(), d.begin(), d.end()));

        x = e;
        x.erase(plastic::unique(x.begin(), x.end()).begin(), x.end());
        assert(format(x) == "[]");
        x = d;
        x.erase(plastic::unique(x.begin(), x.end()).begin(), x.end());
        assert(format(x) == "[5]");
        x = { 1, 1, 2, 2, 3 };
        x.erase(plastic::unique(x.begin(), x.end()).begin(), x.end());
        assert(format(x) == "[1, 2, 3]");

        x = { 0, 0, 0, 0, 0 };
        plastic::unique_copy(e.begin(), e.end(), x.begin());
        assert(format(x) == "[0, 0, 0, 0, 0]");
        plastic::unique_copy(c.begin(), c.end(), x.begin());
        assert(format(x) == "[1, 2, 3, 2, 1]");
        plastic::unique_copy(d.begin(), d.end(), x.begin());
        assert(format(x) == "[5, 2, 3, 2, 1]");
    }

    TEST_METHOD(partition) {
        std::vector<int> e, a{ 1, 3, 5, 7, 9 }, b{ 2, 4, 6, 8, 10 }, c{ 1, 2, 3, 2, 1 }, d{ 5, 5, 5, 5 }, x, y;

        assert(plastic::is_partitioned(e.begin(), e.end(), [](int x) { return x % 2 == 0; }) == true);
        assert(plastic::is_partitioned(a.begin(), a.end(), [](int x) { return x % 2 == 1; }) == true);
        assert(plastic::is_partitioned(b.begin(), b.end(), [](int x) { return x % 4 == 2; }) == false);
        assert(plastic::is_partitioned(c.begin(), c.end(), [](int x) { return x < 3; }) == false);

        x = e;
        plastic::partition(x.begin(), x.end(), [](int x) { return x > 0; });
        assert(format(x) == "[]");
        x = a;
        plastic::partition(x.begin(), x.end(), [](int x) { return x < 5; });
        assert(std::ranges::is_partitioned(x.begin(), x.end(), [](int x) { return x < 5; }));
        x = c;
        plastic::partition(x.begin(), x.end(), [](int x) { return x % 2 == 0; });
        assert(std::ranges::is_partitioned(x.begin(), x.end(), [](int x) { return x % 2 == 0; }));

        x = y = { 0, 0, 0, 0, 0 };
        plastic::partition_copy(e.begin(), e.end(), x.begin(), y.begin(), [](int x) { return x > 0; });
        assert(format(x) == "[0, 0, 0, 0, 0]");
        assert(format(y) == "[0, 0, 0, 0, 0]");
        plastic::partition_copy(a.begin(), a.end(), x.begin(), y.begin(), [](int x) { return x > 5; });
        assert(format(x) == "[7, 9, 0, 0, 0]");
        assert(format(y) == "[1, 3, 5, 0, 0]");
        plastic::partition_copy(c.begin(), c.end(), x.begin(), y.begin(), [](int x) { return x % 2 == 1; });
        assert(format(x) == "[1, 3, 1, 0, 0]");
        assert(format(y) == "[2, 2, 5, 0, 0]");

        x = e;
        plastic::stable_partition(x.begin(), x.end(), [](int x) { return x > 0; });
        assert(format(x) == "[]");
        x = a;
        plastic::stable_partition(x.begin(), x.end(), [](int x) { return x % 3 == 1; });
        assert(format(x) == "[1, 7, 3, 5, 9]");
        x = c;
        plastic::stable_partition(x.begin(), x.end(), [](int x) { return x % 2 == 0; });
        assert(format(x) == "[2, 2, 1, 3, 1]");

        assert(plastic::partition_point(e.begin(), e.end(), [](int x) { return x > 0; }) == e.begin());
        assert(plastic::partition_point(a.begin(), a.end(), [](int x) { return x < 10; }) == a.end());
        assert(plastic::partition_point(b.begin(), b.end(), [](int x) { return x <= 6; }) == b.begin() + 3);
    }

    TEST_METHOD(sorting) {
        assert(false);
    }

    TEST_METHOD(binary_search) {
        std::vector<int> e, a{ 2, 4, 6, 8, 10 }, b{ 1, 2, 2, 3, 3 }, c{ 5, 5, 5, 5 };

        assert(plastic::lower_bound(e.begin(), e.end(), 3) == e.end());
        assert(plastic::lower_bound(a.begin(), a.end(), 5) == a.begin() + 2);
        assert(plastic::lower_bound(b.begin(), b.end(), 2) == b.begin() + 1);
        assert(plastic::lower_bound(c.begin(), c.end(), 5) == c.begin());

        assert(plastic::upper_bound(e.begin(), e.end(), 3) == e.end());
        assert(plastic::upper_bound(a.begin(), a.end(), 5) == a.begin() + 2);
        assert(plastic::upper_bound(b.begin(), b.end(), 2) == b.begin() + 3);
        assert(plastic::upper_bound(c.begin(), c.end(), 5) == c.end());

        assert(plastic::binary_search(e.begin(), e.end(), 3) == false);
        assert(plastic::binary_search(a.begin(), a.end(), 10) == true);
        assert(plastic::binary_search(b.begin(), b.end(), 2) == true);
        assert(plastic::binary_search(c.begin(), c.end(), 6) == false);

        auto res1{ plastic::equal_range(e.begin(), e.end(), 3) };
        assert(res1.begin() == e.end() && res1.end() == e.end());
        auto res2{ plastic::equal_range(a.begin(), a.end(), 5) };
        assert(res2.begin() == a.begin() + 2 && res2.end() == a.begin() + 2);
        auto res3{ plastic::equal_range(b.begin(), b.end(), 2) };
        assert(res3.begin() == b.begin() + 1 && res3.end() == b.begin() + 3);
        auto res4{ plastic::equal_range(c.begin(), c.end(), 5) };
        assert(res4.begin() == c.begin() && res4.end() == c.end());
    }

    TEST_METHOD(merging) {
        std::vector<int> e, a{ 1, 3, 5 }, b{ 2, 4, 6 }, c{ 1, 2, 3 }, x;

        x = { 0, 0, 0, 0, 0, 0 };
        plastic::merge(e.begin(), e.end(), e.begin(), e.end(), x.begin());
        assert(format(x) == "[0, 0, 0, 0, 0, 0]");
        plastic::merge(a.begin(), a.end(), b.begin(), b.end(), x.begin());
        assert(format(x) == "[1, 2, 3, 4, 5, 6]");
        plastic::merge(c.begin(), c.end(), c.begin(), c.end(), x.begin());
        assert(format(x) == "[1, 1, 2, 2, 3, 3]");

        x = e;
        plastic::inplace_merge(x.begin(), x.begin(), x.end());
        assert(format(x) == "[]");
        x = { 1, 3, 5, 2, 4, 6 };
        plastic::inplace_merge(x.begin(), x.begin() + 3, x.end());
        assert(format(x) == "[1, 2, 3, 4, 5, 6]");
        x = { 1, 2, 3, 1, 2, 3 };
        plastic::inplace_merge(x.begin(), x.begin() + 3, x.end());
        assert(format(x) == "[1, 1, 2, 2, 3, 3]");
    }

    TEST_METHOD(set) {
        std::vector<int> e, a{ 1, 2, 3, 5, 7 }, b{ 1, 3, 4, 6, 7 }, x;

        assert(plastic::includes(e.begin(), e.end(), e.begin(), e.end()) == true);
        assert(plastic::includes(a.begin(), a.end(), e.begin(), e.end()) == true);
        assert(plastic::includes(e.begin(), e.end(), a.begin(), a.end()) == false);
        assert(plastic::includes(a.begin(), a.end(), b.begin(), b.end()) == false);
        assert(plastic::includes(a.begin(), a.end(), b.begin() + 1, b.begin() + 2) == true);

        x = { 0, 0, 0, 0, 0 };
        plastic::set_difference(e.begin(), e.end(), e.begin(), e.end(), x.begin());
        assert(format(x) == "[0, 0, 0, 0, 0]");
        plastic::set_difference(a.begin(), a.end(), e.begin(), e.end(), x.begin());
        assert(format(x) == "[1, 2, 3, 5, 7]");
        plastic::set_difference(a.begin(), a.end(), b.begin(), b.end(), x.begin());
        assert(format(x) == "[2, 5, 3, 5, 7]");
        plastic::set_difference(b.begin(), b.end(), a.begin(), a.end(), x.begin());
        assert(format(x) == "[4, 6, 3, 5, 7]");

        x = { 0, 0, 0 };
        plastic::set_intersection(e.begin(), e.end(), e.begin(), e.end(), x.begin());
        assert(format(x) == "[0, 0, 0]");
        plastic::set_intersection(a.begin(), a.end(), e.begin(), e.end(), x.begin());
        assert(format(x) == "[0, 0, 0]");
        plastic::set_intersection(a.begin(), a.end(), b.begin(), b.end(), x.begin());
        assert(format(x) == "[1, 3, 7]");
        plastic::set_intersection(b.begin(), b.end(), a.begin(), a.end(), x.begin());
        assert(format(x) == "[1, 3, 7]");

        x = { 0, 0, 0, 0, 0 };
        plastic::set_symmetric_difference(e.begin(), e.end(), e.begin(), e.end(), x.begin());
        assert(format(x) == "[0, 0, 0, 0, 0]");
        plastic::set_symmetric_difference(a.begin(), a.end(), e.begin(), e.end(), x.begin());
        assert(format(x) == "[1, 2, 3, 5, 7]");
        plastic::set_symmetric_difference(a.begin(), a.end(), b.begin(), b.end(), x.begin());
        assert(format(x) == "[2, 4, 5, 6, 7]");
        plastic::set_symmetric_difference(b.begin(), b.end(), a.begin(), a.end(), x.begin());
        assert(format(x) == "[2, 4, 5, 6, 7]");

        x = { 0, 0, 0, 0, 0, 0, 0 };
        plastic::set_union(e.begin(), e.end(), e.begin(), e.end(), x.begin());
        assert(format(x) == "[0, 0, 0, 0, 0, 0, 0]");
        plastic::set_union(a.begin(), a.end(), e.begin(), e.end(), x.begin());
        assert(format(x) == "[1, 2, 3, 5, 7, 0, 0]");
        plastic::set_union(a.begin(), a.end(), b.begin(), b.end(), x.begin());
        assert(format(x) == "[1, 2, 3, 4, 5, 6, 7]");
        plastic::set_union(b.begin(), b.end(), a.begin(), a.end(), x.begin());
        assert(format(x) == "[1, 2, 3, 4, 5, 6, 7]");
    }

    TEST_METHOD(heap) {
        assert(false);
    }

    TEST_METHOD(minimun_maximum) {
        std::vector<int> e, a{ 1, 3, 5, 7, 9 }, b{ 10, 8, 6, 4, 2 };

        assert(plastic::max(0, 0) == 0);
        assert(plastic::max(5, 1) == 5);
        assert(plastic::max({ 0, 0, 0, 0, 0 }) == 0);
        assert(plastic::max({ 5, 3, 2, 4, 1 }) == 5);

        assert(plastic::max_element(e.begin(), e.end()) == e.end());
        assert(plastic::max_element(a.begin(), a.end()) == a.end() - 1);
        assert(plastic::max_element(b.begin(), b.end()) == b.begin());

        assert(plastic::min(0, 0) == 0);
        assert(plastic::min(5, 1) == 1);
        assert(plastic::min({ 0, 0, 0, 0, 0 }) == 0);
        assert(plastic::min({ 5, 3, 2, 4, 1 }) == 1);

        assert(plastic::min_element(e.begin(), e.end()) == e.end());
        assert(plastic::min_element(a.begin(), a.end()) == a.begin());
        assert(plastic::min_element(b.begin(), b.end()) == b.end() - 1);

        auto res1{ plastic::minmax(0, 0) };
        assert(res1.min == 0 && res1.max == 0);
        auto res2{ plastic::minmax(5, 1) };
        assert(res2.min == 1 && res2.max == 5);
        auto res3{ plastic::minmax({ 0, 0, 0, 0, 0 }) };
        assert(res3.min == 0 && res3.max == 0);
        auto res4{ plastic::minmax({ 5, 3, 2, 4, 1 }) };
        assert(res4.min == 1 && res4.max == 5);

        auto res5{ plastic::minmax_element(e.begin(), e.end()) };
        assert(res5.min == e.end() && res5.max == e.end());
        auto res6{ plastic::minmax_element(a.begin(), a.end()) };
        assert(res6.min == a.begin() && res6.max == a.end() - 1);
        auto res7{ plastic::minmax_element(b.begin(), b.end()) };
        assert(res7.min == b.end() - 1 && res7.max == b.begin());

        assert(plastic::clamp(0, 1, 5) == 1);
        assert(plastic::clamp(3, 1, 5) == 3);
        assert(plastic::clamp(6, 1, 5) == 5);
        assert(plastic::clamp(2, 2, 2) == 2);
    }

    TEST_METHOD(comparison) {
        std::vector<int> e, a{ 1, 2, 3, 4, 5 }, b{ 2, 4, 6, 8, 10 };

        assert(plastic::equal(e.begin(), e.end(), e.begin(), e.end()) == true);
        assert(plastic::equal(e.begin(), e.end(), a.begin(), a.end()) == false);
        assert(plastic::equal(a.begin(), a.end(), b.begin(), b.end()) == false);
        assert(plastic::equal(a.begin(), a.end(), b.begin(), b.end(), {}, [](int x) { return x * 2; }) == true);

        assert(plastic::lexicographical_compare(e.begin(), e.end(), a.begin(), a.end()) == true);
        assert(plastic::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end()) == true);
        assert(plastic::lexicographical_compare(b.begin(), b.end(), a.begin(), a.end()) == false);
        assert(plastic::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end(), {}, [](int x) { return x * 3; }) == false);
    }

    TEST_METHOD(permutation) {
        std::string e, a{ "abc" }, b{ "cba" }, c{ "aab" }, d{ "abcd" };

        assert(plastic::is_permutation(e.begin(), e.end(), e.begin(), e.end()) == true);
        assert(plastic::is_permutation(a.begin(), a.end(), b.begin(), b.end()) == true);
        assert(plastic::is_permutation(a.begin(), a.end(), c.begin(), c.end()) == false);
        assert(plastic::is_permutation(a.begin(), a.end(), d.begin(), d.end()) == false);

        plastic::next_permutation(a.begin(), a.end());
        assert(a == "acb");
        plastic::next_permutation(a.begin(), a.end());
        assert(a == "bac");
        plastic::next_permutation(a.begin(), a.end());
        assert(a == "bca");
        plastic::next_permutation(a.begin(), a.end());
        assert(a == "cab");
        plastic::next_permutation(a.begin(), a.end());
        assert(a == "cba");
        plastic::next_permutation(a.begin(), a.end());
        assert(a == "abc");

        plastic::prev_permutation(a.begin(), a.end());
        assert(a == "cba");
        plastic::prev_permutation(a.begin(), a.end());
        assert(a == "cab");
        plastic::prev_permutation(a.begin(), a.end());
        assert(a == "bca");
        plastic::prev_permutation(a.begin(), a.end());
        assert(a == "bac");
        plastic::prev_permutation(a.begin(), a.end());
        assert(a == "acb");
        plastic::prev_permutation(a.begin(), a.end());
        assert(a == "abc");
    }

};