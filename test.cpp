// linear structure
#include "deque.h"
#include "forward_list.h"
#include "list.h"
#include "vector.h"

// search tree
#include "binary_search_tree.h"

// heap
#include "binary_heap.h"

#include <algorithm>
#include <format>

using namespace plastic;

template<std::input_iterator It>
std::string str(It first, It last) {
    if (first == last) {
        return "[]";
    }

    std::string res{ std::format("[{}", *first++) };
    while (first != last) {
        res += std::format(", {}", *first++);
    }

    return res += ']';
}

std::string str(std::random_access_iterator auto first, std::size_t size) {
    return str(first, first + size);
}

std::string str(std::ranges::input_range auto&& range) {
    return str(range.begin(), range.end());
}

template<class T, class Cmp>
std::string str(binary_heap<T, Cmp> heap) {
    std::sort_heap(heap.data(), heap.data() + heap.size(), Cmp{});
    return str(heap.data(), heap.size());
}

int main() {
    // linear structure
    {
        vector<int> a, b(4, 4), c{ 3, 2, 1 };
        assert(str(a) == "[]");
        assert(str(b) == "[4, 4, 4, 4]");
        assert(str(c) == "[3, 2, 1]");

        assert(str(c.data(), c.size()) == "[3, 2, 1]");
        assert(str(c.cbegin(), c.cend()) == "[3, 2, 1]");
        assert(str(c.rbegin(), c.rend()) == "[1, 2, 3]");
        assert(str(c.crbegin(), c.crend()) == "[1, 2, 3]");

        c = c;
        assert(str(c) == "[3, 2, 1]");
        c = b;
        assert(str(c) == "[4, 4, 4, 4]");

        assert(a.empty() == true);
        assert(b.empty() == false);

        assert(a.size() == 0);
        assert(b.size() == 4);

        c.clear();
        assert(str(c) == "[]");

        c.resize(2);
        assert(str(c) == "[0, 0]");
        c.resize(1);
        assert(str(c) == "[0]");
        c.resize(5, 1);
        assert(str(c) == "[0, 1, 1, 1, 1]");

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
        assert(str(c) == "[0, 1, 1, 1, 1, 2]");

        c.pop_back();
        c.pop_back();
        assert(str(c) == "[0, 1, 1, 1]");

        c.insert(c.begin() + 2, 3, 2);
        assert(str(c) == "[0, 1, 2, 2, 2, 1, 1]");
        c.insert(c.begin() + 3, { 3, 4, 5 });
        assert(str(c) == "[0, 1, 2, 3, 4, 5, 2, 2, 1, 1]");

        c.erase(c.begin() + 5);
        assert(str(c) == "[0, 1, 2, 3, 4, 2, 2, 1, 1]");
        c.erase(c.begin() + 5, c.end() - 1);
        assert(str(c) == "[0, 1, 2, 3, 4, 1]");

        vector d{ 1, 2 }, e{ 1, 2, 2 }, f{ 1, 2, 3 };
        assert(d == d);
        assert(d != e);
        assert(d < e);
        assert(d <= f);
    }
    {
        deque<int> a, b(4, 4), c{ 3, 2, 1 };
        assert(str(a) == "[]");
        assert(str(b) == "[4, 4, 4, 4]");
        assert(str(c) == "[3, 2, 1]");

        assert(str(c.cbegin(), c.cend()) == "[3, 2, 1]");
        assert(str(c.rbegin(), c.rend()) == "[1, 2, 3]");
        assert(str(c.crbegin(), c.crend()) == "[1, 2, 3]");

        c = c;
        assert(str(c) == "[3, 2, 1]");
        c = b;
        assert(str(c) == "[4, 4, 4, 4]");

        assert(a.empty() == true);
        assert(b.empty() == false);

        assert(a.size() == 0);
        assert(b.size() == 4);

        c.clear();
        assert(str(c) == "[]");

        c.resize(2);
        assert(str(c) == "[0, 0]");
        c.resize(1);
        assert(str(c) == "[0]");
        c.resize(5, 1);
        assert(str(c) == "[0, 1, 1, 1, 1]");

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
        assert(str(c) == "[-1, -1, 0, 1, 1, 1, 1]");

        c.pop_front();
        assert(str(c) == "[-1, 0, 1, 1, 1, 1]");

        c.push_back(2);
        assert(str(c) == "[-1, 0, 1, 1, 1, 1, 2]");

        c.pop_back();
        c.pop_back();
        assert(str(c) == "[-1, 0, 1, 1, 1]");

        c.insert(c.begin() + 3, 2, 2);
        assert(str(c) == "[-1, 0, 1, 2, 2, 1, 1]");
        c.insert(c.begin() + 4, { 3, 4, 5 });
        assert(str(c) == "[-1, 0, 1, 2, 3, 4, 5, 2, 1, 1]");

        c.erase(c.begin());
        assert(str(c) == "[0, 1, 2, 3, 4, 5, 2, 1, 1]");
        c.erase(c.begin() + 5, c.end() - 1);
        assert(str(c) == "[0, 1, 2, 3, 4, 1]");

        deque d{ 1, 2 }, e{ 1, 2, 2 }, f{ 1, 2, 3 };
        assert(d == d);
        assert(d != e);
        assert(d < e);
        assert(d <= f);
    }
    {
        forward_list<int> a, b(4, 4), c{ 3, 2, 1 };
        assert(str(a) == "[]");
        assert(str(b) == "[4, 4, 4, 4]");
        assert(str(c) == "[3, 2, 1]");

        assert(str(c.cbegin(), c.cend()) == "[3, 2, 1]");

        c = c;
        assert(str(c) == "[3, 2, 1]");
        c = b;
        assert(str(c) == "[4, 4, 4, 4]");

        assert(a.empty() == true);
        assert(b.empty() == false);

        assert(a.size() == 0);
        assert(b.size() == 4);

        c.clear();
        assert(str(c) == "[]");

        c.resize(2);
        assert(str(c) == "[0, 0]");
        c.resize(1);
        assert(str(c) == "[0]");
        c.resize(5, 1);
        assert(str(c) == "[0, 1, 1, 1, 1]");

        assert(*std::next(b.begin()) == 4);
        assert(*std::next(c.begin()) == 1);

        assert(b.front() == 4);
        assert(c.front() == 0);

        c.push_front(-1);
        c.push_front(-1);
        assert(str(c) == "[-1, -1, 0, 1, 1, 1, 1]");

        c.pop_front();
        assert(str(c) == "[-1, 0, 1, 1, 1, 1]");

        c.insert_after(std::next(c.begin(), 2), 2, 2);
        assert(str(c) == "[-1, 0, 1, 2, 2, 1, 1, 1]");
        c.insert_after(std::next(c.begin(), 3), { 3, 4, 5 });
        assert(str(c) == "[-1, 0, 1, 2, 3, 4, 5, 2, 1, 1, 1]");

        c.erase_after(c.end());
        assert(str(c) == "[0, 1, 2, 3, 4, 5, 2, 1, 1, 1]");
        c.erase_after(std::next(c.begin(), 4), c.end());
        assert(str(c) == "[0, 1, 2, 3, 4]");

        forward_list d{ 1, 2 }, e{ 1, 2, 2 }, f{ 1, 2, 3 };
        assert(d == d);
        assert(d != e);
        assert(d < e);
        assert(d <= f);
    }
    {
        list<int> a, b(4, 4), c{ 3, 2, 1 };
        assert(str(a) == "[]");
        assert(str(b) == "[4, 4, 4, 4]");
        assert(str(c) == "[3, 2, 1]");

        assert(str(c.cbegin(), c.cend()) == "[3, 2, 1]");
        assert(str(c.rbegin(), c.rend()) == "[1, 2, 3]");
        assert(str(c.crbegin(), c.crend()) == "[1, 2, 3]");

        c = c;
        assert(str(c) == "[3, 2, 1]");
        c = b;
        assert(str(c) == "[4, 4, 4, 4]");

        assert(a.empty() == true);
        assert(b.empty() == false);

        assert(a.size() == 0);
        assert(b.size() == 4);

        c.clear();
        assert(str(c) == "[]");

        c.resize(2);
        assert(str(c) == "[0, 0]");
        c.resize(1);
        assert(str(c) == "[0]");
        c.resize(5, 1);
        assert(str(c) == "[0, 1, 1, 1, 1]");

        assert(*std::next(b.begin()) == 4);
        assert(*std::next(c.begin()) == 1);

        assert(b.front() == 4);
        assert(c.front() == 0);

        assert(b.back() == 4);
        assert(c.back() == 1);

        c.push_back(2);
        assert(str(c) == "[0, 1, 1, 1, 1, 2]");

        c.pop_back();
        c.pop_back();
        assert(str(c) == "[0, 1, 1, 1]");

        c.insert(std::next(c.begin(), 2), 3, 2);
        assert(str(c) == "[0, 1, 2, 2, 2, 1, 1]");
        c.insert(std::next(c.begin(), 3), { 3, 4, 5 });
        assert(str(c) == "[0, 1, 2, 3, 4, 5, 2, 2, 1, 1]");

        c.erase(std::next(c.begin(), 5));
        assert(str(c) == "[0, 1, 2, 3, 4, 2, 2, 1, 1]");
        c.erase(std::next(c.begin(), 5), std::prev(c.end()));
        assert(str(c) == "[0, 1, 2, 3, 4, 1]");

        list d{ 1, 2 }, e{ 1, 2, 2 }, f{ 1, 2, 3 };
        assert(d == d);
        assert(d != e);
        assert(d < e);
        assert(d <= f);
    }

    // search tree
    {
        binary_search_tree<int> a;
    }

    // heap
    {
        binary_heap<int> a, b{ 4, 4, 4, 4 }, c{ 3, 2, 1 };

        assert(str(a) == "[]");
        assert(str(b) == "[4, 4, 4, 4]");
        assert(str(c) == "[1, 2, 3]");

        c = c;
        assert(str(c) == "[1, 2, 3]");
        c = b;
        assert(str(c) == "[4, 4, 4, 4]");

        assert(a.empty() == true);
        assert(b.empty() == false);

        assert(a.size() == 0);
        assert(b.size() == 4);

        c.clear();
        assert(str(c) == "[]");

        assert(b.top() == 4);

        c.push(4);
        c.push(6);
        c.push(2);
        c.push(8);
        assert(str(c) == "[2, 4, 6, 8]");

        c.pop();
        assert(str(c) == "[2, 4, 6]");

        c.merge(binary_heap{ 3, 1, 7 });
        assert(str(c) == "[1, 2, 3, 4, 6, 7]");

        c.assign(c.data(), 5);
        assert(str(c) == "[1, 2, 3, 4, 5, 6]");

        c.erase(c.data());
        assert(str(c) == "[1, 2, 3, 4, 5]");
    }
}