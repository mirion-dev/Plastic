export module utils;

import std;

export template <class T>
std::string format(const T& value) {
    return std::format("{}", value);
}

export template <std::input_iterator It>
std::string format(It first, It last) {
    return ::format(std::ranges::subrange{ first, last });
}

export template <std::contiguous_iterator It>
std::string format(It first, std::size_t size) {
    return ::format(std::span{ first, size });
}
