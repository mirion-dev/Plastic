#import "common.typ": *

#show: init

#grid(
    columns: 7,
    column-gutter: (0em, 0em, 0em, 1em, 1em, 0em),
    grid.cell(colspan: 7)[*Addressable Heap*],
    grid.cell(rowspan: 2)[*Structure*],
    grid.cell(colspan: 6)[*Time Complexity*],
    [Top Access], [Insertion], [Top Deletion], [Merge], [Update], [Deletion],
    `binary_heap`, O1, Ologna, Ologn, On, Ologn, Ologn,
)
