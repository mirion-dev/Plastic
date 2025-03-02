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
        /*
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
        */
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
        std::vector<int> empty, a{ 1, 3, 5, 7, 9 }, b{ 2, 4, 6, 8, 10 }, c{ 1, 2, 3, 2, 1 }, d{ 5, 5, 5, 5 }, x;

        assert(plastic::all_of(empty.begin(), empty.end(), [](int x) { return x % 2 == 0; }) == true);
        assert(plastic::all_of(a.begin(), a.end(), [](int x) { return x % 2 == 1; }) == true);
        assert(plastic::all_of(b.begin(), b.end(), [](int x) { return x % 2 == 1; }) == false);
        assert(plastic::all_of(c.begin(), c.end(), [](int x) { return x % 2 == 1; }) == false);

        assert(plastic::any_of(empty.begin(), empty.end(), [](int x) { return x > 0; }) == false);
        assert(plastic::any_of(a.begin(), a.end(), [](int x) { return x > 5; }) == true);
        assert(plastic::any_of(b.begin(), b.end(), [](int x) { return x % 2 == 0; }) == true);
        assert(plastic::any_of(c.begin(), c.end(), [](int x) { return x == 4; }) == false);

        assert(plastic::none_of(empty.begin(), empty.end(), [](int x) { return x < 0; }) == true);
        assert(plastic::none_of(a.begin(), a.end(), [](int x) { return x % 2 == 0; }) == true);
        assert(plastic::none_of(b.begin(), b.end(), [](int x) { return x > 2; }) == false);
        assert(plastic::none_of(c.begin(), c.end(), [](int x) { return x == 2; }) == false);

        x = { 1, 2, 3 };
        plastic::for_each(x.begin(), x.end(), [](int& x) { x += 10; });
        assert(format(x) == "[11, 12, 13]");

        x = { 1, 2, 3, 4, 5 };
        plastic::for_each_n(x.begin(), 3, [](int& x) { x *= 2; });
        assert(format(x) == "[2, 4, 6, 4, 5]");

        assert(plastic::count(empty.begin(), empty.end(), 0) == 0);
        assert(plastic::count(c.begin(), c.end(), 2) == 2);
        assert(plastic::count(d.begin(), d.end(), 5) == 4);

        assert(plastic::count_if(a.begin(), a.end(), [](int x) { return x > 5; }) == 2);
        assert(plastic::count_if(b.begin(), b.end(), [](int x) { return x % 4 == 2; }) == 3);

        assert(plastic::mismatch(a.begin(), a.end(), b.begin(), b.end()).in1 == a.begin());
        assert(plastic::mismatch(b.begin(), b.end(), b.begin(), b.end()).in2 == b.end());

        assert(plastic::find(a.begin(), a.end(), 7) == a.begin() + 3);
        assert(plastic::find(b.begin(), b.end(), 5) == b.end());

        assert(plastic::find_if(a.begin(), a.end(), [](int x) { return x < 0; }) == a.end());
        assert(plastic::find_if(b.begin(), b.end(), [](int x) { return x > 5; }) == b.begin() + 2);

        assert(plastic::find_if_not(c.begin(), c.end(), [](int x) { return x < 3; }) == c.begin() + 2);
        assert(plastic::find_if_not(d.begin(), d.end(), [](int x) { return x == 5; }) == d.end());

        x = { 5, 5 };
        assert(plastic::find_end(c.begin(), c.end(), x.begin(), x.end()).begin() == c.end());
        assert(plastic::find_end(d.begin(), d.end(), x.begin(), x.end()).begin() == d.begin() + 2);

        x = { 0, 10 };
        assert(plastic::find_first_of(a.begin(), a.end(), x.begin(), x.end()) == a.end());
        assert(plastic::find_first_of(b.begin(), b.end(), x.begin(), x.end()) == b.begin() + 4);

        assert(plastic::adjacent_find(a.begin(), a.end()) == a.end());
        assert(plastic::adjacent_find(d.begin(), d.end()) == d.begin());

        x = { 3, 5, 7 };
        assert(plastic::search(a.begin(), a.end(), x.begin(), x.end()).begin() == a.begin() + 1);
        assert(plastic::search(b.begin(), b.end(), x.begin(), x.end()).begin() == b.end());

        assert(plastic::search_n(d.begin(), d.end(), 4, 5).begin() == d.begin());
        assert(plastic::search_n(c.begin(), c.end(), 2, 2, [](int x, int v) { return x >= v; }).begin() == c.begin() + 1);

        assert(plastic::contains(a.begin(), a.end(), 3) == true);
        assert(plastic::contains(b.begin(), b.end(), 3) == false);
        assert(plastic::contains(c.begin(), c.end(), 3) == true);

        x = { 3, 2, 1 };
        assert(plastic::contains_subrange(a.begin(), a.end(), x.begin(), x.end()) == false);
        assert(plastic::contains_subrange(b.begin(), b.end(), x.begin(), x.end()) == false);
        assert(plastic::contains_subrange(c.begin(), c.end(), x.begin(), x.end()) == true);

        x = { 1, 3, 5 };
        assert(plastic::starts_with(a.begin(), a.end(), x.begin(), x.end()) == true);
        assert(plastic::starts_with(b.begin(), b.end(), x.begin(), x.end()) == false);
        assert(plastic::starts_with(c.begin(), c.end(), x.begin(), x.end()) == false);

        x = { 3, 2, 1 };
        assert(plastic::ends_with(a.begin(), a.end(), x.begin(), x.end()) == false);
        assert(plastic::ends_with(b.begin(), b.end(), x.begin(), x.end()) == false);
        assert(plastic::ends_with(c.begin(), c.end(), x.begin(), x.end()) == true);
    }

    TEST_METHOD(fold) {
        std::vector<int> empty, a{ 1, 2, 3, 4, 5 }, b{ 2, 4, 6, 8, 10 };

        assert(plastic::fold_left(empty.begin(), empty.end(), 10, std::plus{}) == 10);
        assert(plastic::fold_left(a.begin(), a.end(), 0, std::plus{}) == 15);
        assert(plastic::fold_left(a.begin(), a.end(), 10, std::minus{}) == -5);
        assert(plastic::fold_left(b.begin(), b.end(), 1, std::multiplies{}) == 3840);

        assert(!plastic::fold_left_first(empty.begin(), empty.end(), std::plus{}));
        assert(plastic::fold_left_first(a.begin(), a.end(), std::plus{}).value() == 15);
        assert(*plastic::fold_left_first(b.begin(), b.end(), std::multiplies{}) == 3840);

        assert(plastic::fold_right(empty.begin(), empty.end(), 100, std::plus{}) == 100);
        assert(plastic::fold_right(a.begin(), a.end(), 0, std::plus{}) == 15);
        assert(plastic::fold_right(b.begin(), b.end(), 2, std::minus{}) == 4);

        assert(!plastic::fold_right_last(empty.begin(), empty.end(), std::plus{}));
        assert(*plastic::fold_right_last(a.begin(), a.end(), std::plus{}) == 15);
        assert(*plastic::fold_right_last(b.begin(), b.end(), std::minus{}) == 6);

        auto res1{ plastic::fold_left_with_iter(a.begin(), a.end(), 0, std::plus{}) };
        assert(res1.value == 15 && res1.in == a.end());
        auto res2{ plastic::fold_left_with_iter(a.begin(), a.begin() + 3, 10, std::multiplies{}) };
        assert(res2.value == 60 && res2.in == a.begin() + 3);

        auto res3{ plastic::fold_left_first_with_iter(empty.begin(), empty.end(), std::plus{}) };
        assert(!res3.value && res3.in == empty.end());
        auto res4{ plastic::fold_left_first_with_iter(b.begin(), b.end(), std::multiplies{}) };
        assert(*res4.value == 3840 && res4.in == b.end());
    }

    TEST_METHOD(modification) {

    }

    TEST_METHOD(partition) {

    }

    TEST_METHOD(sorting) {

    }

    TEST_METHOD(binary_search) {

    }

    TEST_METHOD(merging) {

    }

    TEST_METHOD(set) {

    }

    TEST_METHOD(heap) {

    }

    TEST_METHOD(minimun_maximum) {

    }

    TEST_METHOD(comparison) {
        std::vector<int> empty, a{ 1, 2, 3, 4, 5 }, b{ 2, 4, 6, 8, 10 };

        assert(plastic::equal(empty.begin(), empty.end(), empty.begin(), empty.end()) == true);
        assert(plastic::equal(empty.begin(), empty.end(), a.begin(), a.end()) == false);
        assert(plastic::equal(a.begin(), a.end(), b.begin(), b.end()) == false);
        assert(plastic::equal(a.begin(), a.end(), b.begin(), b.end(), {}, [](int x) { return x * 2; }) == true);

        assert(plastic::lexicographical_compare(empty.begin(), empty.end(), a.begin(), a.end()) == true);
        assert(plastic::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end()) == true);
        assert(plastic::lexicographical_compare(b.begin(), b.end(), a.begin(), a.end()) == false);
        assert(plastic::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end(), {}, [](int x) { return x * 3; }) == false);
    }

    TEST_METHOD(permutation) {

    }

};