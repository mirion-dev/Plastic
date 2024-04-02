# Plastic

算法竞赛复健期写的一些东西. 

## 数据结构

| | **`stack`** | **`queue`** | **`priority_queue`** | **`deque`** | **`forward_list`** | **`list`** | **`binary_search_tree`** | **`red_black_tree`** | **`avl_tree`** | **`b_tree`** | |
| :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: |
| **`empty`** | `empty` | `empty` | `empty` | `empty` | `empty` | `empty` | `empty` | `empty` | `empty` | `empty` | **`empty`** |
| **`size`** | `size` | `size` | `size` | `size` | `size` | `size` | `size` | `size` | `size` | `size` | **`size`** |
| **`begin`** | | | | | `begin` | `begin` | | | | | **`begin`** |
| **`end`** | | | | | `end` | `end` | | | | | **`end`** |
| **`push_back`** | `push` | `push` | `push` | `push_back` | | `push_back` | | | | | **`push_back`** |
| **`pop_back`** | `pop` | | | `pop_back` | | `pop_back` | | | | | **`pop_back`** |
| **`push_front`** | | | | `push_front` | `push_front` | `push_front` | | | | | **`push_front`** |
| **`pop_front`** | | `pop` | `pop` | `pop_front` | `pop_front` | `pop_front` | | | | | **`pop_front`** |
| **`insert`** | | | | | `insert_after` | `insert` | `insert` | `insert` | `insert` | `insert` | **`insert`** |
| **`erase`** | | | | | `erase_after` | `erase` | `erase` | `erase` | `erase` | `erase` | **`erase`** |
| **`count`** | | | | | | | `count` | `count` | `count` | `count` | **`count`** |
| **`contains`** | | | | | | | `contains` | `contains` | `contains` | `contains` | **`contains`** |
| **`max`** | | | | | | | `max` | `max` | `max` | `max` | **`max`** |
| **`min`** | | | | | | | `min` | `min` | `min` | `min` | **`min`** |

## 算法

| | 运算符重载 | 成员函数 | 非成员函数 |
| :--: | :--: | :--: | :--: |
| **`sort`** | | |`selection_sort`<br>`insertion_sort`<br>`bubble_sort`<br>`heap_sort`<br>`merge_sort`<br>`quick_sort` |
| **`bigint`** | `+n`<br>`-n`<br>`++n`<br>`--n`<br>`n++`<br>`n--`<br>`+`<br>`-`<br>`*`<br>`/`<br>`%`<br>`+=`<br>`-=`<br>`*=`<br>`/=`<br>`%=`<br>`==`<br>`!=`<br>`<`<br>`>`<br>`<=`<br>`>=`<br>`<=>`<br>`int64_t` | `sign`<br>`data`<br>`negate` | `to_string` |