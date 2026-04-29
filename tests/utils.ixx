export module utils;

import std;

namespace tests {

    export template <class T>
    std::string format(const T& value) {
        return std::format("{}", value);
    }

    export template <std::contiguous_iterator It>
    std::string format(It first, std::size_t size) {
        return tests::format(std::span{ first, size });
    }

    export template <std::input_iterator It>
    std::string format(It first, It last) {
        return tests::format(std::ranges::subrange{ first, last });
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
        return tests::format(temp | std::views::reverse);
    }

}
