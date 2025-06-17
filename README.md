# Plastic

My implementations of common data structures and algorithms for study.

## Data Structures

The class designs are primarily based on [MSVC STL](https://github.com/microsoft/STL), though not identical. For instance, the underlying structure of `deque` is implemented as a bidirectional dynamic array instead of blocks.

### Linear Structures

![](./img/linear.svg)

\* Amortized complexity.

| | **`inplace_vector`<br>`vector`** | **`inplace_deque`<br>`deque`** | **`forward_list`** | **`list`** |
| :--: | :--: | :--: | :--: | :--: |
| **`empty`** | `empty` | `empty` | `empty` | `empty` |
| **`size`** | `size` | `size` | `size` | `size` |
| **`max_size`** | `max_size` | `max_size` | `max_size` | `max_size` |
| **`clear`** | `clear` | `clear` | `clear` | `clear` |
| **`resize`** | `resize` | `resize` | `resize` | `resize` |
| **`capacity`** | `capacity` | `capacity` | | |
| **`reserve`** | `reserve` | `reserve` | | |
| **`begin`** | `begin` | `begin` | `begin` | `begin` |
| **`end`** | `end` | `end` | `end` | `end` |
| **`cbegin`** | `cbegin` | `cbegin` | `cbegin` | `cbegin` |
| **`cend`** | `cend` | `cend` | `cend` | `cend` |
| **`rbegin`** | `rbegin` | `rbegin` | | `rbegin` |
| **`rend`** | `rend` | `rend` | | `rend` |
| **`crbegin`** | `crbegin` | `crbegin` | | `crbegin` |
| **`crend`** | `crend` | `crend` | | `crend` |
| **`front`** | `front` | `front` | `front` | `front` |
| **`back`** | `back` | `back` | | `back` |
| **`data`** | `data` | `deque::data` | | |
| **`operator[]`** | `operator[]` | `operator[]` | | |
| **`push_front`** | | `push_front` | `push_front` | `push_front` |
| **`pop_front`** | | `pop_front` | `pop_front` | `pop_front` |
| **`push_back`** | `push_back` | `push_back` | | `push_back` |
| **`pop_back`** | `pop_back` | `pop_back` | | `pop_back` |
| **`insert`** | `insert` | `insert` | `insert_after` | `insert` |
| **`erase`** | `erase` | `erase` | `erase_after` | `erase` |
| **`swap`** | `swap` | `swap` | `swap` | `swap` |
| **`operator==`** | `operator==` | `operator==` | `operator==` | `operator==` |
| **`operator<=>`** | `operator<=>` | `operator<=>` | `operator<=>` | `operator<=>` |

### Search Trees

![](./img/trees.svg)

\* Amortized complexity.

Member functions: `empty`, `size`, `max_size`, `clear`, `begin`, `end`, `cbegin`, `cend`, `rbegin`, `rend`, `crbegin`, `crend`, `lower_bound`, `upper_bound`, `equal_range`, `find`, `contains`, `count`, `insert`, `erase`, `swap`, `operator==`, `operator<=>`.

### Addressable Heaps

![](./img/heaps.svg)

\* Amortized complexity.

Member functions: `empty`, `size`, `max_size`, `clear`, `top_handle`, `top`, `push`, `pop`, `merge`, `update`, `erase`.

## Algorithms

Same as the [constrained algorithms](https://en.cppreference.com/w/cpp/header/algorithm#Function-like_entities_.28C.2B.2B20.29) in `<algorithm>`, but only meet the standard's minimum requirements.

| **Group** | **Functions** |
| -- | -- |
| Non-modifying Sequence | `all_of`, `any_of`, `none_of`, `for_each`, `for_each_n`, `count`, `count_if`, `mismatch`, `find`, `find_if`, `find_if_not`, `find_end`, `find_first_of`, `adjacent_find`, `search`, `search_n`, `contains`, `contains_subrange`, `starts_with`, `ends_with` |
| Fold | `fold_left`, `fold_left_first`, `fold_right`, `fold_right_last`, `fold_left_with_iter`, `fold_left_first_with_iter` |
| Modifying Sequence | `copy`, `copy_if`, `copy_n`, `copy_backward`, `move`, `move_backward`, `fill`, `fill_n`, `transform`, `generate`, `generate_n`, `remove`, `remove_if`, `remove_copy`, `remove_copy_if`, `replace`, `replace_if`, `replace_copy`, `replace_copy_if`, `swap_ranges`, `reverse`, `reverse_copy`, `rotate`, `rotate_copy`, `shift_left`, `shift_right`, `sample`, `shuffle`, `unique`, `unique_copy` |
| Partitioning | `is_partitioned`, `partition`, `partition_copy`, `stable_partition`, `partition_point` |
| Sorting | `is_sorted`, `is_sorted_until`, `sort`, `partial_sort`, `partial_sort_copy`, `stable_sort`, `nth_element` |
| Binary Search | `lower_bound`, `upper_bound`, `binary_search`, `equal_range` |
| Merge | `merge`, `inplace_merge` |
| Set | `includes`, `set_difference`, `set_intersection`, `set_symmetric_difference`, `set_union` |
| Heap | `is_heap`, `is_heap_until`, `make_heap`, `push_heap`, `pop_heap`, `sort_heap` |
| Minimum/Maximum | `max`, `max_element`, `min`, `min_element`, `minmax`, `minmax_element`, `clamp` |
| Comparison | `equal`, `lexicographical_compare` |
| Permutation | `is_permutation`, `next_permutation`, `prev_permutation` |