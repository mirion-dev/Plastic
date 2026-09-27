#show link: set text(fill: blue);

#title[Plastic]

My implementations of common data structures and algorithms for study.

= Data Structures

Reference the #link("https://eel.is/c++draft/#containers")[standard containers], but are not identical, as the goal is to implement different structures rather than complete standard conformance.

#table(
    columns: 6,
    align: center + horizon,
    table.header[][*`ArrayList`*][*`LinkedList`*][*`Deque`*][*Trees*][*Heaps*],
    [*`begin`*], `begin`, `begin`, `begin`, `begin`, `apex`,
    [*`end`*], `end`, `end`, `end`, `end`, [],
    [*`rbegin`*], `rbegin`, `rbegin`, `rbegin`, `rbegin`, [],
    [*`rend`*], `rend`, `rend`, `rend`, `rend`, [],
    [*`cbegin`*], `cbegin`, `cbegin`, `cbegin`, `cbegin`, `capex`,
    [*`cend`*], `cend`, `cend`, `cend`, `cend`, [],
    [*`crbegin`*], `crbegin`, `crbegin`, `crbegin`, `crbegin`, [],
    [*`crend`*], `crend`, `crend`, `crend`, `crend`, [],
    [*`empty`*], `empty`, `empty`, `empty`, `empty`, `empty`,
    [*`size`*], `size`, `size`, `size`, `size`, `size`,
    [*`max_size`*], `max_size`, `max_size`, `max_size`, `max_size`, `max_size`,
    [*`capacity`*], `capacity`, [], `capacity`, [], [],
    [*`resize`*], `resize`, `resize`, `resize`, [], [],
    [*`reserve`*], `reserve`, [], `reserve`, [], [],
    [*`operator[]`*], `operator[]`, [], `operator[]`, [], [],
    [*`front`*], `front`, `front`, `front`, `front`, `top`,
    [*`back`*], `back`, `back`, `back`, `back`, [],
    [*`data`*], `data`, [], [], [], [],
    [*`push_front`*], [], `push_front`, `push_front`, [], [],
    [*`pop_front`*], [], `pop_front`, `pop_front`, [], `pop`,
    [*`push_back`*], `push_back`, `push_back`, `push_back`, [], [],
    [*`pop_back`*], `pop_back`, `pop_back`, `pop_back`, [], [],
    [*`insert`*], `insert`, `insert`, `insert`, `insert`, `push`,
    [*`erase`*], `erase`, `erase`, `erase`, `erase`, `erase`,
    [*`swap`*], `swap`, `swap`, `swap`, `swap`, `swap`,
    [*`clear`*], `clear`, `clear`, `clear`, `clear`, `clear`,
    [*`merge`*], [], [], [], `merge`, `merge`,
    [*`find`*], [], [], [], `find`, [],
    [*`count`*], [], [], [], `count`, [],
    [*`contains`*], [], [], [], `contains`, [],
    [*`lower_bound`*], [], [], [], `lower_bound`, [],
    [*`upper_bound`*], [], [], [], `upper_bound`, [],
    [*`equal_range`*], [], [], [], `equal_range`, [],
    [*`operator==`*], `operator==`, `operator==`, `operator==`, `operator==`, [],
    [*`operator<=>`*], `operator<=>`, `operator<=>`, `operator<=>`, `operator<=>`, [],
)

Removed structures: `BigInt`, `FixedArrayList`, `DeArrayList`, `SinglyLinkedList`, `FixedDeque`.

= Algorithms

Same as the #link("https://eel.is/c++draft/#algorithms")[standard algorithms] in namespace `ranges`.

== Non-modifying Sequence

=== `all_of`

Time Complexity: `O()`.

=== `any_of`

Time Complexity: `O()`.

=== `none_of`

Time Complexity: `O()`.

=== `contains`

Time Complexity: `O()`.

=== `contains_subrange`

Time Complexity: `O()`.

=== `for_each`

Time Complexity: `O()`.

=== `for_each_n`

Time Complexity: `O()`.

=== `find`

Time Complexity: `O()`.

=== `find_if`

Time Complexity: `O()`.

=== `find_if_not`

Time Complexity: `O()`.

=== `find_last`

Time Complexity: `O()`.

=== `find_last_if`

Time Complexity: `O()`.

=== `find_last_if_not`

Time Complexity: `O()`.

=== `find_end`

Time Complexity: `O()`.

=== `find_first_of`

Time Complexity: `O()`.

=== `adjacent_find`

Time Complexity: `O()`.

=== `count`

Time Complexity: `O()`.

=== `count_if`

Time Complexity: `O()`.

=== `mismatch`

Time Complexity: `O()`.

=== `equal`

Time Complexity: `O()`.

=== `is_permutation`

Time Complexity: `O()`.

=== `search`

Time Complexity: `O()`.

=== `search_n`

Time Complexity: `O()`.

=== `starts_with`

Time Complexity: `O()`.

=== `ends_with`

Time Complexity: `O()`.

=== `fold_left`

Time Complexity: `O()`.

=== `fold_left_first`

Time Complexity: `O()`.

=== `fold_right`

Time Complexity: `O()`.

=== `fold_right_last`

Time Complexity: `O()`.

=== `fold_left_with_iter`

Time Complexity: `O()`.

=== `fold_left_first_with_iter`

Time Complexity: `O()`.

== Mutating Sequence

=== `copy`

Time Complexity: `O()`.

=== `copy_n`

Time Complexity: `O()`.

=== `copy_if`

Time Complexity: `O()`.

=== `copy_backward`

Time Complexity: `O()`.

=== `move`

Time Complexity: `O()`.

=== `move_backward`

Time Complexity: `O()`.

=== `swap_ranges`

Time Complexity: `O()`.

=== `transform`

Time Complexity: `O()`.

=== `replace`

Time Complexity: `O()`.

=== `replace_if`

Time Complexity: `O()`.

=== `replace_copy`

Time Complexity: `O()`.

=== `replace_copy_if`

Time Complexity: `O()`.

=== `fill`

Time Complexity: `O()`.

=== `fill_n`

Time Complexity: `O()`.

=== `generate`

Time Complexity: `O()`.

=== `generate_n`

Time Complexity: `O()`.

=== `remove`

Time Complexity: `O()`.

=== `remove_if`

Time Complexity: `O()`.

=== `remove_copy`

Time Complexity: `O()`.

=== `remove_copy_if`

Time Complexity: `O()`.

=== `unique`

Time Complexity: `O()`.

=== `unique_copy`

Time Complexity: `O()`.

=== `reverse`

Time Complexity: `O()`.

=== `reverse_copy`

Time Complexity: `O()`.

=== `rotate`

Time Complexity: `O()`.

=== `rotate_copy`

Time Complexity: `O()`.

=== `sample`

Time Complexity: `O()`.

=== `shuffle`

Time Complexity: `O()`.

=== `shift_left`

Time Complexity: `O()`.

=== `shift_right`

Time Complexity: `O()`.

== Sorting

=== `sort`

Time Complexity: `O()`.

=== `stable_sort`

Time Complexity: `O()`.

=== `partial_sort`

Time Complexity: `O()`.

=== `partial_sort_copy`

Time Complexity: `O()`.

=== `is_sorted`

Time Complexity: `O()`.

=== `is_sorted_until`

Time Complexity: `O()`.

== N-th Element

=== `nth_element`

Time Complexity: `O()`.

== Binary Search

=== `lower_bound`

Time Complexity: `O()`.

=== `upper_bound`

Time Complexity: `O()`.

=== `equal_range`

Time Complexity: `O()`.

=== `binary_search`

Time Complexity: `O()`.

== Partitioning

=== `is_partitioned`

Time Complexity: `O()`.

=== `partition`

Time Complexity: `O()`.

=== `stable_partition`

Time Complexity: `O()`.

=== `partition_copy`

Time Complexity: `O()`.

=== `partition_point`

Time Complexity: `O()`.

== Merge

=== `merge`

Time Complexity: `O()`.

=== `inplace_merge`

Time Complexity: `O()`.

== Set

=== `includes`

Time Complexity: `O()`.

=== `set_union`

Time Complexity: `O()`.

=== `set_intersection`

Time Complexity: `O()`.

=== `set_difference`

Time Complexity: `O()`.

=== `set_symmetric_difference`

Time Complexity: `O()`.

== Heap

=== `push_heap`

Time Complexity: `O()`.

=== `pop_heap`

Time Complexity: `O()`.

=== `make_heap`

Time Complexity: `O()`.

=== `sort_heap`

Time Complexity: `O()`.

=== `is_heap`

Time Complexity: `O()`.

=== `is_heap_until`

Time Complexity: `O()`.

== Minimum and Maximum

=== `min`

Time Complexity: `O()`.

=== `max`

Time Complexity: `O()`.

=== `minmax`

Time Complexity: `O()`.

=== `min_element`

Time Complexity: `O()`.

=== `max_element`

Time Complexity: `O()`.

=== `minmax_element`

Time Complexity: `O()`.

== Bounded Value

=== `clamp`

Time Complexity: `O()`.

== Lexicographical Comparison

=== `lexicographical_compare`

Time Complexity: `O()`.

== Permutation

=== `next_permutation`

Time Complexity: `O()`.

=== `prev_permutation`

Time Complexity: `O()`.
