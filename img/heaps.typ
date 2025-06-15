#set page(width: auto, height: auto, margin: 1em)

#let O1 = grid.cell(fill: lime.lighten(50%), $O(1)$)
#let O1a = grid.cell(fill: lime.lighten(50%), $O(1)^*$)
#let Ologn = grid.cell(fill: yellow.lighten(50%), $O(log n)$)
#let Ologna = grid.cell(fill: yellow.lighten(50%), $O(log n)^*$)
#let On = grid.cell(fill: red.lighten(50%), $O(n)$)

#grid(
    align: center + horizon,
    inset: (left: 1em, right: 1em, top: .5em, bottom: .5em),
    columns: 7,
    column-gutter: (0em, 0em, 0em, 1em, 1em, 0em),
    grid.cell(rowspan: 2)[*Structure*],
    grid.cell(colspan: 6)[*Time Complexity*],
    [Top Access], [Insertion], [Top Deletion], [Merge], [Update], [Deletion],
    `binary_heap`, O1, Ologna, Ologn, On, Ologn, Ologn
)
