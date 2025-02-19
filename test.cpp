#include <assert.h>

import std;
import plastic;

using namespace plastic;

std::string format(const auto& arg) {
    return std::format("{}", arg);
}

std::string format(std::random_access_iterator auto first, std::size_t size) {
    return format(std::span{ first, size });
}

template<std::input_iterator It>
std::string format(It first, It last) {
    return format(std::ranges::subrange{ first, last });
}

template<class T, class Cmp>
std::string format(const binary_heap<T, Cmp>& heap) {
    std::vector<T> temp(heap.data(), heap.data() + heap.size());
    std::ranges::sort(temp, Cmp{});
    return format(temp);
}

int main() {
    {
        vector<int> a, b(4, 4), c{ 3, 2, 1 };
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

        vector d{ 1, 2 }, e{ 1, 2, 2 }, f{ 1, 2, 3 };
        assert(d == d);
        assert(d != e);
        assert(d < e);
        assert(d <= f);
    }
    {
        deque<int> a, b(4, 4), c{ 3, 2, 1 };
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

        deque d{ 1, 2 }, e{ 1, 2, 2 }, f{ 1, 2, 3 };
        assert(d == d);
        assert(d != e);
        assert(d < e);
        assert(d <= f);
    }
    {
        forward_list<int> a, b(4, 4), c{ 3, 2, 1 };
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

        forward_list d{ 1, 2 }, e{ 1, 2, 2 }, f{ 1, 2, 3 };
        assert(d == d);
        assert(d != e);
        assert(d < e);
        assert(d <= f);
    }
    {
        list<int> a, b(4, 4), c{ 3, 2, 1 };
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

        list d{ 1, 2 }, e{ 1, 2, 2 }, f{ 1, 2, 3 };
        assert(d == d);
        assert(d != e);
        assert(d < e);
        assert(d <= f);
    }
    {
        binary_search_tree<int> a, b{ 1, 2, 2, 2, 3 }, c{ 6, 4, 8, 2 };
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

        binary_search_tree d{ 1, 2 }, e{ 1, 2, 2 }, f{ 1, 2, 3 };
        assert(d == d);
        assert(d != e);
        assert(d < e);
        assert(d <= f);
    }
    {
        binary_heap<int> a, b{ 4, 4, 4, 4 }, c{ 3, 2, 1 };
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

        c.merge(binary_heap{ 3, 1, 7 });
        assert(format(c) == "[1, 2, 3, 4, 6, 7]");

        c.assign(c.data(), 5);
        assert(format(c) == "[1, 2, 3, 4, 5, 6]");

        c.erase(c.data());
        assert(format(c) == "[1, 2, 3, 4, 5]");
    }
}