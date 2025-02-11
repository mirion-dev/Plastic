#include <assert.h>

import std;
import plastic;

template<class P>
concept binary_tree_node_ptr = requires(P ptr) {
    ptr->left;
    ptr->right;
    ptr->value;
    ptr->is_head;
};

template<binary_tree_node_ptr P>
struct std::formatter<P> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    template<class Ctx>
    auto format(P ptr, Ctx& ctx) const {
        if (ptr->is_head) {
            return std::format_to(ctx.out(), "*");
        }
        return std::format_to(ctx.out(), "{}", ptr->value);
    }
};

template<binary_tree_node_ptr P>
class level_order_traversal {
    P _head;
    std::vector<std::vector<P>> _result;

    void _dfs(P ptr, std::size_t depth) {
        if (depth >= _result.size()) {
            _result.push_back({ ptr });
        }
        else {
            _result[depth].push_back(ptr);
        }
        if (!ptr->is_head) {
            _dfs(ptr->left, depth + 1);
            _dfs(ptr->right, depth + 1);
        }
    }

public:
    level_order_traversal(P head) :
        _head{ head } {

        _dfs(head->parent, 0);
    }

    const auto& operator()() const {
        return _result;
    }
};

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

std::string format(const auto& tree) requires binary_tree_node_ptr<decltype(tree.data())> {
    return format(level_order_traversal{ tree.data() }());
}

template<class T, class Cmp>
std::string format(const plastic::binary_heap<T, Cmp>& heap) {
    std::vector<T> temp(heap.data(), heap.data() + heap.size());
    std::ranges::sort(temp, Cmp{});
    return format(temp);
}

int main() {
    {
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
    {
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
    {
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

        assert(*std::next(b.begin()) == 4);
        assert(*std::next(c.begin()) == 1);

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
    {
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

        assert(*std::next(b.begin()) == 4);
        assert(*std::next(c.begin()) == 1);

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
    {

    }
    {
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
}