# Plastic

My implementations of common data structures and algorithms for study.

## Data Structures

The class designs are primarily based on the standard library, but not identical. For example, the underlying structure of `deque` is implemented as a bidirectional dynamic array instead of blocks.

| | **`inplace_vector`<br>`vector`** | **`inplace_deque`<br>`deque`** | **`forward_list`** | **`list`** | Search Tree | Addressable Heap |
| :--: | :--: | :--: | :--: | :--: | :--: | :--: |
| **`empty`** | `empty` | `empty` | `empty` | `empty` | `empty` | `empty` |
| **`size`** | `size` | `size` | `size` | `size` | `size` | `size` |
| **`max_size`** | `max_size` | `max_size` | `max_size` | `max_size` | `max_size` | `max_size` |
| **`clear`** | `clear` | `clear` | `clear` | `clear` | `clear` | `clear` |
| **`resize`** | `resize` | `resize` | `resize` | `resize` | | |
| **`capacity`** | `capacity` | `capacity` | | | | |
| **`reserve`** | `reserve` | `reserve` | | | | |
| **`before_begin`** | | | `before_begin` | | | |
| **`begin`** | `begin` | `begin` | `begin` | `begin` | `begin` | `top_handle` |
| **`end`** | `end` | `end` | `end` | `end` | `end` | |
| **`cbefore_begin`** | | | `cbefore_begin` | | | |
| **`cbegin`** | `cbegin` | `cbegin` | `cbegin` | `cbegin` | `cbegin` | |
| **`cend`** | `cend` | `cend` | `cend` | `cend` | `cend` | |
| **`rbegin`** | `rbegin` | `rbegin` | | `rbegin` | `rbegin` | |
| **`rend`** | `rend` | `rend` | | `rend` | `rend` | |
| **`crbegin`** | `crbegin` | `crbegin` | | `crbegin` | `crbegin` | |
| **`crend`** | `crend` | `crend` | | `crend` | `crend` | |
| **`front`** | `front` | `front` | `front` | `front` | | `top` |
| **`back`** | `back` | `back` | | `back` | | |
| **`update`** | | | | | | `update` |
| **`data`** | `data` | `deque::data` | | | | |
| **`operator[]`** | `operator[]` | `operator[]` | | | | |
| **`contains`** | | | | | `contains` | |
| **`find`** | | | | | `find` | |
| **`count`** | | | | | `count` | |
| **`lower_bound`** | | | | | `lower_bound` | |
| **`upper_bound`** | | | | | `upper_bound` | |
| **`equal_range`** | | | | | `equal_range` | |
| **`push_front`** | | `push_front` | `push_front` | `push_front` | | `push` |
| **`pop_front`** | | `pop_front` | `pop_front` | `pop_front` | | `pop` |
| **`push_back`** | `push_back` | `push_back` | | `push_back` | | |
| **`pop_back`** | `pop_back` | `pop_back` | | `pop_back` | | |
| **`insert`** | `insert` | `insert` | `insert_after` | `insert` | `insert` | |
| **`erase`** | `erase` | `erase` | `erase_after` | `erase` | `erase` | `erase` |
| **`swap`** | `swap` | `swap` | `swap` | `swap` | `swap` | `swap` |
| **`merge`** | | | | | | `merge` |
| **`operator==`** | `operator==` | `operator==` | `operator==` | `operator==` | `operator==` | |
| **`operator<=>`** | `operator<=>` | `operator<=>` | `operator<=>` | `operator<=>` | `operator<=>` | |

<img src="./images/linear_structure.svg" width="100%" />
<img src="./images/search_tree.svg" width="100%" />
z<img src="./images/addressable_heap.svg" width="100%" />

## Algorithms

Same as the [constrained algorithms](https://en.cppreference.com/w/cpp/header/algorithm#Function-like_entities_.28C.2B.2B20.29) in `<algorithm>`, but only meet the standard's minimum requirements.

| **Category** | **Operations** |
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