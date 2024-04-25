# Plastic

by Mirion

## Data Structure

| | **`vector`** | **`deque`** | **`priority_queue`** | **`forward_list`** | **`list`** | **`binary_search_tree`** | **`red_black_tree`** | **`avl_tree`** | |
| :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: |
| **`empty`** | `empty` | `empty` | `empty` | `empty` | `empty` | `empty` | `empty` | `empty` | **`empty`** |
| **`size`** | `size` | `size` | `size` | | | `size` | `size` | `size` | **`size`** |
| **`clear`** | `clear` | `clear` | `clear` | | | | | | **`clear`** |
| **`resize`** | `resize` | `resize` | | `resize` | `resize` | | | | **`resize`** |
| **`capacity`** | `capacity` | `capacity` | `capacity` | | | | | | **`capacity`** |
| **`reserve`** | `reserve` | `reserve` | `reserve` | | | | | | **`reserve`** |
| **`begin`** | `begin` | `begin` | | `begin` | `begin` | | | | **`begin`** |
| **`end`** | `end` | `end` | | `end` | `end` | | | | **`end`** |
| **`front`** | `front` | `front` | `top` | `front` | `front` | | | | **`front`** |
| **`back`** | `back` | `back` | | | `back` | | | | **`back`** |
| **`operator[]`** | `operator[]` | `operator[]` | | | | | | | **`operator[]`** |
| **`push_front`** | | `push_front` | | `push_front` | `push_front` | | | | **`push_front`** |
| **`pop_front`** | | `pop_front` | `pop` | `pop_front` | `pop_front` | | | | **`pop_front`** |
| **`push_back`** | `push_back` | `push_back` | `push` | | `push_back` | | | | **`push_back`** |
| **`pop_back`** | `pop_back` | `pop_back` | | | `pop_back` | | | | **`pop_back`** |
| **`insert`** | `insert` | `insert` | | `insert_after` | `insert` | `insert` | `insert` | `insert` | **`insert`** |
| **`erase`** | `erase` | `erase` | | `erase_after` | `erase` | `erase` | `erase` | `erase` | **`erase`** |
| **`swap`** | `swap` | `swap` | -- | -- | -- | -- | -- | -- | **`swap`** |
| **`operator<=>`** | `operator<=>` | `operator<=>` | -- | -- | -- | -- | -- | -- | **`operator<=>`** |
| | **`vector`** | **`deque`** | **`priority_queue`** | **`forward_list`** | **`list`** | **`binary_search_tree`** | **`red_black_tree`** | **`avl_tree`** | |

## Algorithm

| | `operator` | `member` | `non-member` |
| :--: | :--: | :--: | :--: |
| **`sort`** | | |`selection_sort`<br>`insertion_sort`<br>`bubble_sort`<br>`heap_sort`<br>`merge_sort`<br>`quick_sort` |
| **`bigint`** | `+n`<br>`-n`<br>`++n`<br>`--n`<br>`n++`<br>`n--`<br>`+`<br>`-`<br>`*`<br>`/`<br>`%`<br>`+=`<br>`-=`<br>`*=`<br>`/=`<br>`%=`<br>`==`<br>`!=`<br>`<`<br>`>`<br>`<=`<br>`>=`<br>`<=>`<br>`int64_t` | `sign`<br>`data`<br>`negate` | `to_string` |