#pragma once

#include <cassert>
#include <functional>

namespace plastic {

    template<class T, size_t order, class Cmp = std::less<T>>
    class b_tree {
        static constexpr size_t _minSize{ order - 1 };
        static constexpr size_t _maxSize{ 2 * order - 1 };

        struct node {
            T value[_maxSize];
            node* children[_maxSize + 1];
            size_t size;
        };

        node* _root;
        size_t _size;

    public:
        b_tree() {
            _root = nullptr;
            _size = 0;
        }
    };

}