#include "deque.h"
#include "vector.h"

#include <format>

using namespace plastic;

template<std::input_iterator It>
std::string format(It first, It last) {
    if (first == last) {
        return "[]";
    }

    std::string res{ std::format("[{}", *first++) };
    while (first != last) {
        res += std::format(", {}", *first++);
    }

    return res += ']';
}

std::string format(std::random_access_iterator auto first, std::size_t size) {
    return format(first, first + size);
}

std::string format(std::ranges::input_range auto&& range) {
    return format(range.begin(), range.end());
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


}