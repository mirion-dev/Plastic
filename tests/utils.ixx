export module utils;

import std;

export std::string format(const auto& arg) {
    return std::format("{}", arg);
}

export std::string format(std::contiguous_iterator auto first, std::size_t size) {
    return format(std::span{ first, size });
}

export template <std::input_iterator It>
std::string format(It first, It last) {
    return format(std::ranges::subrange{ first, last });
}

export template <class Hp>
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
