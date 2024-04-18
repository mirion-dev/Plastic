# Plastic

by Mirion

## Data Structure

| | **`vector`** | **`stack`** | **`queue`** | **`priority_queue`** | **`forward_list`** | **`list`** | **`binary_search_tree`** | **`red_black_tree`** | **`avl_tree`** | **`b_tree`** | |
| :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: |
| **`empty`** | `empty` | `empty` | `empty` | `empty` | `empty` | `empty` | `empty` | `empty` | `empty` | `empty` | **`empty`** |
| **`size`** | `size` | `size` | `size` | `size` | `size` | `size` | `size` | `size` | `size` | `size` | **`size`** |
| **`reserve`** | `reserve` | `reserve` | -- | -- | -- | -- | -- | -- | -- | -- | -- |
| **`resize`** | `resize` | | -- | -- | -- | -- | -- | -- | -- | -- | -- |
| **`begin`** | `begin` | | | | `begin` | `begin` | | | | | **`begin`** |
| **`end`** | `end` | | | | `end` | `end` | | | | | **`end`** |
| **`front`** | `front` | | `front` | `top` | | | | | | | **`front`** |
| **`back`** | `back` | `top` | `back` | | | | | | | | **`back`** |
| **`push_front`** | | | | | `push_front` | `push_front` | | | | | **`push_front`** |
| **`pop_front`** | | | `pop` | `pop` | `pop_front` | `pop_front` | | | | | **`pop_front`** |
| **`push_back`** | `push_back` | `push` | `push` | `push` | | `push_back` | | | | | **`push_back`** |
| **`pop_back`** | `pop_back` | `pop` | | | | `pop_back` | | | | | **`pop_back`** |
| **`insert`** | `insert` | | | | `insert_after` | `insert` | `insert` | `insert` | `insert` | `insert` | **`insert`** |
| **`erase`** | `erase` | | | | `erase_after` | `erase` | `erase` | `erase` | `erase` | `erase` | **`erase`** |
| **`count`** | | | | | | | `count` | `count` | `count` | `count` | **`count`** |
| **`contains`** | | | | | | | `contains` | `contains` | `contains` | `contains` | **`contains`** |
| **`max`** | | | | | | | `max` | `max` | `max` | `max` | **`max`** |
| **`min`** | | | | | | | `min` | `min` | `min` | `min` | **`min`** |

## Algorithm

| | `operator` | `member` | `non-member` |
| :--: | :--: | :--: | :--: |
| **`sort`** | | |`selection_sort`<br>`insertion_sort`<br>`bubble_sort`<br>`heap_sort`<br>`merge_sort`<br>`quick_sort` |
| **`bigint`** | `+n`<br>`-n`<br>`++n`<br>`--n`<br>`n++`<br>`n--`<br>`+`<br>`-`<br>`*`<br>`/`<br>`%`<br>`+=`<br>`-=`<br>`*=`<br>`/=`<br>`%=`<br>`==`<br>`!=`<br>`<`<br>`>`<br>`<=`<br>`>=`<br>`<=>`<br>`int64_t` | `sign`<br>`data`<br>`negate` | `to_string` |