#set page(width: auto, height: auto, margin: 1em)

#let O1a = grid.cell(fill: lime.lighten(50%), $O(1)^*$)
#let Ologn = grid.cell(fill: yellow.lighten(50%), $O(log n)$)
#let On = grid.cell(fill: red.lighten(50%), $O(n)$)

#grid(
    align: center + horizon,
    inset: (left: 1em, right: 1em, top: .5em, bottom: .5em),
    columns: 5,
    grid.cell(rowspan: 2)[*Structure*],
    grid.cell(colspan: 4)[*Time Complexity*],
    [Iteration], [Insertion], [Deletion], [Search],
    `binary_search_tree`, O1a, On, On, On,
    `red_black_tree`, O1a, Ologn, Ologn, Ologn,
    `avl_tree`, O1a, Ologn, Ologn, Ologn,
    `b_tree`, O1a, Ologn, Ologn, Ologn,
)
