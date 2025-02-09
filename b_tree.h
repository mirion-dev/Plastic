#pragma once

#include <cassert>
#include <functional>

namespace plastic {

    template<class T, class Cmp = std::less<T>, std::size_t order = 5>
    class b_tree {
        static constexpr Cmp _cmp{};
        static constexpr std::size_t _min_size{ order - 1 };
        static constexpr std::size_t _max_size{ 2 * order - 1 };

        struct node {
            T value[_max_size];
            node* children[_max_size + 1];
            std::size_t size;
        };

        node* _root;
        std::size_t _size;

    public:
        b_tree() noexcept :
            _root{},
            _size{} {}
    };

}