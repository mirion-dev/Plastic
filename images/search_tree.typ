#import "common.typ": *

#show: init

#grid(
    columns: 5,
    grid.cell(colspan: 5)[*Search Tree*],
    grid.cell(rowspan: 2)[*Structure*],
    grid.cell(colspan: 4)[*Time Complexity*],
    [Iteration], [Insertion], [Deletion], [Search],
    `binary_search_tree`, O1a, On, On, On,
    `red_black_tree`, O1a, Ologn, Ologn, Ologn,
    `avl_tree`, O1a, Ologn, Ologn, Ologn,
    `b_tree`, O1a, Ologn, Ologn, Ologn,
)
