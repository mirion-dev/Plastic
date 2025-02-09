#include <assert.h>

import std;
import plastic;

template<std::input_iterator It>
std::string to_string(It first, It last) {
    if (first == last) {
        return "[]";
    }

    std::string res{ std::format("[{}", *first++) };
    while (first != last) {
        res += std::format(", {}", *first++);
    }

    return res += ']';
}

std::string to_string(std::random_access_iterator auto first, std::size_t size) {
    return to_string(first, first + size);
}

std::string to_string(std::ranges::input_range auto&& range) {
    return to_string(range.begin(), range.end());
}

template<class T, class Cmp>
std::string to_string(plastic::binary_heap<T, Cmp> heap) {
    std::sort_heap(heap.data(), heap.data() + heap.size(), Cmp{});
    return to_string(heap.data(), heap.size());
}

int main() {
    {
        plastic::vector<int> a, b(4, 4), c{ 3, 2, 1 };
        assert(to_string(a) == "[]");
        assert(to_string(b) == "[4, 4, 4, 4]");
        assert(to_string(c) == "[3, 2, 1]");

        assert(to_string(c.data(), c.size()) == "[3, 2, 1]");
        assert(to_string(c.cbegin(), c.cend()) == "[3, 2, 1]");
        assert(to_string(c.rbegin(), c.rend()) == "[1, 2, 3]");
        assert(to_string(c.crbegin(), c.crend()) == "[1, 2, 3]");

        c = c;
        assert(to_string(c) == "[3, 2, 1]");
        c = b;
        assert(to_string(c) == "[4, 4, 4, 4]");

        assert(a.empty() == true);
        assert(b.empty() == false);

        assert(a.size() == 0);
        assert(b.size() == 4);

        c.clear();
        assert(to_string(c) == "[]");

        c.resize(2);
        assert(to_string(c) == "[0, 0]");
        c.resize(1);
        assert(to_string(c) == "[0]");
        c.resize(5, 1);
        assert(to_string(c) == "[0, 1, 1, 1, 1]");

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
        assert(to_string(c) == "[0, 1, 1, 1, 1, 2]");

        c.pop_back();
        c.pop_back();
        assert(to_string(c) == "[0, 1, 1, 1]");

        c.insert(c.begin() + 2, 3, 2);
        assert(to_string(c) == "[0, 1, 2, 2, 2, 1, 1]");
        c.insert(c.begin() + 3, { 3, 4, 5 });
        assert(to_string(c) == "[0, 1, 2, 3, 4, 5, 2, 2, 1, 1]");

        c.erase(c.begin() + 5);
        assert(to_string(c) == "[0, 1, 2, 3, 4, 2, 2, 1, 1]");
        c.erase(c.begin() + 5, c.end() - 1);
        assert(to_string(c) == "[0, 1, 2, 3, 4, 1]");

        plastic::vector d{ 1, 2 }, e{ 1, 2, 2 }, f{ 1, 2, 3 };
        assert(d == d);
        assert(d != e);
        assert(d < e);
        assert(d <= f);
    }
    {
        plastic::deque<int> a, b(4, 4), c{ 3, 2, 1 };
        assert(to_string(a) == "[]");
        assert(to_string(b) == "[4, 4, 4, 4]");
        assert(to_string(c) == "[3, 2, 1]");

        assert(to_string(c.cbegin(), c.cend()) == "[3, 2, 1]");
        assert(to_string(c.rbegin(), c.rend()) == "[1, 2, 3]");
        assert(to_string(c.crbegin(), c.crend()) == "[1, 2, 3]");

        c = c;
        assert(to_string(c) == "[3, 2, 1]");
        c = b;
        assert(to_string(c) == "[4, 4, 4, 4]");

        assert(a.empty() == true);
        assert(b.empty() == false);

        assert(a.size() == 0);
        assert(b.size() == 4);

        c.clear();
        assert(to_string(c) == "[]");

        c.resize(2);
        assert(to_string(c) == "[0, 0]");
        c.resize(1);
        assert(to_string(c) == "[0]");
        c.resize(5, 1);
        assert(to_string(c) == "[0, 1, 1, 1, 1]");

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
        assert(to_string(c) == "[-1, -1, 0, 1, 1, 1, 1]");

        c.pop_front();
        assert(to_string(c) == "[-1, 0, 1, 1, 1, 1]");

        c.push_back(2);
        assert(to_string(c) == "[-1, 0, 1, 1, 1, 1, 2]");

        c.pop_back();
        c.pop_back();
        assert(to_string(c) == "[-1, 0, 1, 1, 1]");

        c.insert(c.begin() + 3, 2, 2);
        assert(to_string(c) == "[-1, 0, 1, 2, 2, 1, 1]");
        c.insert(c.begin() + 4, { 3, 4, 5 });
        assert(to_string(c) == "[-1, 0, 1, 2, 3, 4, 5, 2, 1, 1]");

        c.erase(c.begin());
        assert(to_string(c) == "[0, 1, 2, 3, 4, 5, 2, 1, 1]");
        c.erase(c.begin() + 5, c.end() - 1);
        assert(to_string(c) == "[0, 1, 2, 3, 4, 1]");

        plastic::deque d{ 1, 2 }, e{ 1, 2, 2 }, f{ 1, 2, 3 };
        assert(d == d);
        assert(d != e);
        assert(d < e);
        assert(d <= f);
    }
    {
        plastic::forward_list<int> a, b(4, 4), c{ 3, 2, 1 };
        assert(to_string(a) == "[]");
        assert(to_string(b) == "[4, 4, 4, 4]");
        assert(to_string(c) == "[3, 2, 1]");

        assert(to_string(c.cbegin(), c.cend()) == "[3, 2, 1]");

        c = c;
        assert(to_string(c) == "[3, 2, 1]");
        c = b;
        assert(to_string(c) == "[4, 4, 4, 4]");

        assert(a.empty() == true);
        assert(b.empty() == false);

        assert(a.size() == 0);
        assert(b.size() == 4);

        c.clear();
        assert(to_string(c) == "[]");

        c.resize(2);
        assert(to_string(c) == "[0, 0]");
        c.resize(1);
        assert(to_string(c) == "[0]");
        c.resize(5, 1);
        assert(to_string(c) == "[0, 1, 1, 1, 1]");

        assert(*std::next(b.begin()) == 4);
        assert(*std::next(c.begin()) == 1);

        assert(b.front() == 4);
        assert(c.front() == 0);

        c.push_front(-1);
        c.push_front(-1);
        assert(to_string(c) == "[-1, -1, 0, 1, 1, 1, 1]");

        c.pop_front();
        assert(to_string(c) == "[-1, 0, 1, 1, 1, 1]");

        c.insert_after(std::next(c.begin(), 2), 2, 2);
        assert(to_string(c) == "[-1, 0, 1, 2, 2, 1, 1, 1]");
        c.insert_after(std::next(c.begin(), 3), { 3, 4, 5 });
        assert(to_string(c) == "[-1, 0, 1, 2, 3, 4, 5, 2, 1, 1, 1]");

        c.erase_after(c.end());
        assert(to_string(c) == "[0, 1, 2, 3, 4, 5, 2, 1, 1, 1]");
        c.erase_after(std::next(c.begin(), 4), c.end());
        assert(to_string(c) == "[0, 1, 2, 3, 4]");

        plastic::forward_list d{ 1, 2 }, e{ 1, 2, 2 }, f{ 1, 2, 3 };
        assert(d == d);
        assert(d != e);
        assert(d < e);
        assert(d <= f);
    }
    {
        plastic::list<int> a, b(4, 4), c{ 3, 2, 1 };
        assert(to_string(a) == "[]");
        assert(to_string(b) == "[4, 4, 4, 4]");
        assert(to_string(c) == "[3, 2, 1]");

        assert(to_string(c.cbegin(), c.cend()) == "[3, 2, 1]");
        assert(to_string(c.rbegin(), c.rend()) == "[1, 2, 3]");
        assert(to_string(c.crbegin(), c.crend()) == "[1, 2, 3]");

        c = c;
        assert(to_string(c) == "[3, 2, 1]");
        c = b;
        assert(to_string(c) == "[4, 4, 4, 4]");

        assert(a.empty() == true);
        assert(b.empty() == false);

        assert(a.size() == 0);
        assert(b.size() == 4);

        c.clear();
        assert(to_string(c) == "[]");

        c.resize(2);
        assert(to_string(c) == "[0, 0]");
        c.resize(1);
        assert(to_string(c) == "[0]");
        c.resize(5, 1);
        assert(to_string(c) == "[0, 1, 1, 1, 1]");

        assert(*std::next(b.begin()) == 4);
        assert(*std::next(c.begin()) == 1);

        assert(b.front() == 4);
        assert(c.front() == 0);

        assert(b.back() == 4);
        assert(c.back() == 1);

        c.push_back(2);
        assert(to_string(c) == "[0, 1, 1, 1, 1, 2]");

        c.pop_back();
        c.pop_back();
        assert(to_string(c) == "[0, 1, 1, 1]");

        c.insert(std::next(c.begin(), 2), 3, 2);
        assert(to_string(c) == "[0, 1, 2, 2, 2, 1, 1]");
        c.insert(std::next(c.begin(), 3), { 3, 4, 5 });
        assert(to_string(c) == "[0, 1, 2, 3, 4, 5, 2, 2, 1, 1]");

        c.erase(std::next(c.begin(), 5));
        assert(to_string(c) == "[0, 1, 2, 3, 4, 2, 2, 1, 1]");
        c.erase(std::next(c.begin(), 5), std::prev(c.end()));
        assert(to_string(c) == "[0, 1, 2, 3, 4, 1]");

        plastic::list d{ 1, 2 }, e{ 1, 2, 2 }, f{ 1, 2, 3 };
        assert(d == d);
        assert(d != e);
        assert(d < e);
        assert(d <= f);
    }
    {
        plastic::binary_heap<int> a, b{ 4, 4, 4, 4 }, c{ 3, 2, 1 };

        assert(to_string(a) == "[]");
        assert(to_string(b) == "[4, 4, 4, 4]");
        assert(to_string(c) == "[1, 2, 3]");

        c = c;
        assert(to_string(c) == "[1, 2, 3]");
        c = b;
        assert(to_string(c) == "[4, 4, 4, 4]");

        assert(a.empty() == true);
        assert(b.empty() == false);

        assert(a.size() == 0);
        assert(b.size() == 4);

        c.clear();
        assert(to_string(c) == "[]");

        assert(b.top() == 4);

        c.push(4);
        c.push(6);
        c.push(2);
        c.push(8);
        assert(to_string(c) == "[2, 4, 6, 8]");

        c.pop();
        assert(to_string(c) == "[2, 4, 6]");

        c.merge(plastic::binary_heap{ 3, 1, 7 });
        assert(to_string(c) == "[1, 2, 3, 4, 6, 7]");

        c.assign(c.data(), 5);
        assert(to_string(c) == "[1, 2, 3, 4, 5, 6]");

        c.erase(c.data());
        assert(to_string(c) == "[1, 2, 3, 4, 5]");
    }
}