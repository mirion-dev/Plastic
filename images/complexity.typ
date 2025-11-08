#import "@preview/cetz:0.4.2": *

#set page(width: auto, height: auto, margin: 2em)
#set grid(inset: .5em)
#show: align.with(center + horizon)

#let O(complexity, fill: none, amortized: false) = grid.cell(
    inset: (left: 1em, right: 1em),
    fill: fill.lighten(50%),
    if amortized {
        $O(#complexity)^*$
    } else {
        $O(#complexity)$
    },
)

#let O1 = O($1$, fill: lime)
#let O1a = O($1$, fill: lime, amortized: true)
#let Ologn = O($log n$, fill: yellow)
#let Ologna = O($log n$, fill: yellow, amortized: true)
#let On = O($n$, fill: red)
#let Ona = O($n$, fill: red, amortized: true)

#let diag = canvas.with(length: 1em)
#let box = draw.rect.with(stroke: .5pt)
#let tile = box.with(stroke: none, fill: lime)
#let line = draw.line.with(stroke: .5pt)
#let arrow = line.with(mark: (end: "stealth", stroke: 0pt, fill: black))

#let diag-inplace-vector = diag({
    tile((0, 0), (4, 1))
    for i in range(6) {
        box((i, 0), (i + 1, 1))
    }
})

#let diag-vector = diag(
    padding: (left: .5),
    {
        tile((0, 0), (4, 1))
        for x in range(6) {
            box((x, 0), (x + 1, 1))
        }
        for y in range(2) {
            line((6, y), (6.5, y))
        }
    },
)

#let diag-inplace-deque = diag({
    tile((0, 0), (1, 1))
    tile((3, 0), (6, 1))
    tile((2, 0), (3, 1), fill: gray)
    for x in range(6) {
        box((x, 0), (x + 1, 1))
    }
})

#let diag-deque = diag({
    tile((1, 0), (5, 1))
    for x in range(6) {
        box((x, 0), (x + 1, 1))
    }
    for y in range(2) {
        line((-.5, y), (6.5, y))
    }
})

#let diag-forward-list = diag({
    box((0, 0), (1, 1), fill: gray)
    for x in (2, 4) {
        box((x, 0), (x + 1, 1), fill: lime)
        arrow((x - 1, .5), (x, .5))
    }
    arrow((5, .5), (5.75, .5), (5.75, -.3), (-.75, -.3), (-.75, .5), (0, .5))
})

#let diag-list = diag({
    box((0, 0), (1, 1), fill: gray)
    for x in (2, 4) {
        box((x, 0), (x + 1, 1), fill: lime)
        arrow((x - 1, .3), (x, .3))
        arrow((x, .7), (x - 1, .7))
    }
    arrow((5, .3), (5.75, .3), (5.75, -.3), (-.75, -.3), (-.75, .3), (0, .3))
    arrow((0, .7), (-.75, .7), (-.75, 1.3), (5.75, 1.3), (5.75, .7), (5, .7))
})

== Linear Structure

#grid(
    columns: 12,
    column-gutter: (0em, 1em, 0em, 0em, 1em, 0em, 0em, 1em, 0em, 0em, 1em),
    grid.cell(rowspan: 3)[*Structure*],
    grid.cell(rowspan: 3)[*Diagram*],
    grid.cell(colspan: 10)[*Time Complexity*],
    grid.cell(colspan: 3)[*Access / Update*],
    grid.cell(colspan: 3)[*Insertion*],
    grid.cell(colspan: 3)[*Deletion*],
    grid.cell(rowspan: 2)[Search],
    ..([Begin], [Middle], [End]) * 3,
    `inplace_vector`, diag-inplace-vector, O1, O1, O1, On, On, O1, On, On, O1, On,
    `vector`, diag-vector, O1, O1, O1, On, On, O1a, On, On, O1, On,
    `inplace_deque`, diag-inplace-deque, O1, O1, O1, O1, On, O1, O1, On, O1, On,
    `deque`, diag-deque, O1, O1, O1, O1a, On, O1a, O1, On, O1, On,
    `forward_list`, diag-forward-list, O1, On, On, O1, On, On, O1, On, On, On,
    `list`, diag-list, O1, On, O1, O1, On, O1, O1, On, O1, On,
)

== Search Tree

#grid(
    columns: 5,
    grid.cell(rowspan: 2)[*Structure*],
    grid.cell(colspan: 4)[*Time Complexity*],
    [Begin / End Access], [Insertion], [Deletion], [Search],
    `binary_search_tree`, O1, On, On, On,
    `red_black_tree`, O1, Ologn, Ologn, Ologn,
    `avl_tree`, O1, Ologn, Ologn, Ologn,
    `b_tree`, O1, Ologn, Ologn, Ologn,
)

== *Addressable Heap*

#grid(
    columns: 7,
    column-gutter: (0em, 0em, 0em, 1em, 1em, 0em),
    grid.cell(rowspan: 2)[*Structure*],
    grid.cell(colspan: 6)[*Time Complexity*],
    [Apex Access], [Insertion], [Apex Deletion], [Merge], [Handle Update], [Handle Deletion],
    `binary_heap`, O1, Ologna, Ologn, On, Ologn, Ologn,
)

#align(right)[\* Amortized complexity.]
