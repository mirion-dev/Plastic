#import "@preview/cetz:0.4.0": *
#import "common.typ": *

#show: init

#let diag = canvas.with(length: 1em)
#let frame = draw.rect.with(stroke: .5pt)
#let box = draw.rect.with(stroke: none)
#let line = draw.line.with(stroke: .5pt)
#let arrow = draw.line.with(stroke: .5pt, mark: (end: "stealth", stroke: 0pt, fill: black))

#let diag-inplace-vector = diag({
    box((0, 0), (4, 1), fill: lime)
    for i in range(6) {
        frame((i, 0), (i + 1, 1))
    }
})

#let diag-vector = diag(
    padding: (left: .5),
    {
        box((0, 0), (4, 1), fill: lime)
        for x in range(6) {
            frame((x, 0), (x + 1, 1))
        }
        for y in range(2) {
            line((6, y), (6.5, y))
        }
    },
)

#let diag-inplace-deque = diag({
    box((0, 0), (1, 1), fill: lime)
    box((3, 0), (6, 1), fill: lime)
    box((2, 0), (3, 1), fill: gray)
    for x in range(6) {
        frame((x, 0), (x + 1, 1))
    }
})

#let diag-deque = diag({
    box((1, 0), (5, 1), fill: lime)
    for x in range(6) {
        frame((x, 0), (x + 1, 1))
    }
    for y in range(2) {
        line((-.5, y), (0, y))
        line((6, y), (6.5, y))
    }
})

#let diag-forward-list = diag({
    frame((0, 0), (1, 1), fill: gray)
    for x in (2, 4) {
        frame((x, 0), (x + 1, 1), fill: lime)
        arrow((x - 1, .5), (x, .5))
    }
    arrow((5, .5), (5.75, .5), (5.75, -.3), (-.75, -.3), (-.75, .5), (0, .5))
})

#let diag-list = diag({
    frame((0, 0), (1, 1), fill: gray)
    for x in (2, 4) {
        frame((x, 0), (x + 1, 1), fill: lime)
        arrow((x - 1, .3), (x, .3))
        arrow((x, .7), (x - 1, .7))
    }
    arrow((5, .3), (5.75, .3), (5.75, -.3), (-.75, -.3), (-.75, .3), (0, .3))
    arrow((0, .7), (-.75, .7), (-.75, 1.3), (5.75, 1.3), (5.75, .7), (5, .7))
})

#grid(
    columns: 12,
    column-gutter: (-1em, 0em, 0em, 0em, 1em, 0em, 0em, 1em, 0em, 0em, 1em),
    grid.cell(rowspan: 3)[*Structure*],
    grid.cell(rowspan: 3)[*Diagram*],
    grid.cell(colspan: 10)[*Time Complexity*],
    grid.cell(colspan: 3)[*Access / Update*],
    grid.cell(colspan: 3)[*Insertion*],
    grid.cell(colspan: 3)[*Deletion*],
    grid.cell(rowspan: 2)[*Search*],
    ..([Begin], [Middle], [End]) * 3,
    `inplace_vector`, diag-inplace-vector, O1, O1, O1, On, On, O1, On, On, O1, On,
    `vector`, diag-vector, O1, O1, O1, On, On, O1a, On, On, O1, On,
    `inplace_deque`, diag-inplace-deque, O1, O1, O1, O1, On, O1, O1, On, O1, On,
    `deque`, diag-deque, O1, O1, O1, O1a, On, O1a, O1, On, O1, On,
    `forward_list`, diag-forward-list, O1, On, On, O1, On, On, O1, On, On, On,
    `list`, diag-list, O1, On, O1, O1, On, O1, O1, On, O1, On,
)
