#import "@preview/cetz:0.4.2"

#set page(width: auto, height: auto, margin: 2em)
#set grid(align: horizon, inset: .5em)
#show heading: align.with(center)
#show grid: align.with(center)

#let diag = cetz.canvas.with(length: 1em)

#let box((x, y), ..args) = cetz.draw.rect((x - .5, y - .5), (x + .5, y + .5), stroke: .5pt, ..args)
#let tile = box.with(stroke: none, fill: gray)

#let cbox = cetz.draw.circle.with(radius: .5, stroke: .5pt)
#let ctile = cbox.with(stroke: none, fill: gray)

#let line = cetz.draw.line.with(stroke: .5pt)
#let curve = cetz.draw.bezier.with(stroke: .5pt)
#let arrow = line.with(mark: (end: "stealth", stroke: 0pt, fill: black))

#let diag-vector = diag({
    for x in range(6) {
        box((x, 0), fill: if x < 4 { lime })
    }
})

#let diag-deque = diag({
    for x in range(6) {
        box((x, 0), fill: if x == 0 or x > 2 { lime })
    }
})

#let diag-list = diag({
    let y1 = .2
    let y2 = .7
    cbox((0, 0), fill: gray)
    for x in (2, 4) {
        arrow((x - .5, y1), (x - 1.5, y1))
        arrow((x - 1.5, -y1), (x - .5, -y1))
        cbox((x, 0), fill: lime)
    }
    arrow((4.5, -y1), (5.25, -y1), (5.25, -y2), (-1.25, -y2), (-1.25, -y1), (-.5, -y1))
    arrow((-.5, y1), (-1.25, y1), (-1.25, y2), (5.25, y2), (5.25, y1), (4.5, y1))
})

#let diag-red-black-tree = diag({
    line((0, 0), (0, 1.5))
    for x in (-1, 1) {
        line((0, 0), (x, -1))
        curve((0, 1.5), (x, -1), (x * -4.5, -3.5))
        cbox((x, -1), fill: red)
    }
    cbox((0, 0), fill: black)
    cbox((0, 1.5), fill: gray)
})

#let O-cell(complexity, fill: red, label: none, ..args) = grid.cell(
    inset: (left: 1em, right: 1em),
    fill: fill.lighten(50%),
    ..args,
    {
        $O(#complexity)$

        if label != none {
            show regex("[^*]"): super
            link(label, context query(label).first().term)
        }
    },
)

#let O1-cell = O-cell.with($1$, fill: lime)
#let Ologn-cell = O-cell.with($log n$, fill: yellow)
#let On-cell = O-cell.with($n$)

#let O1 = O1-cell()
#let Ologn = Ologn-cell()
#let On = On-cell()
#let O1a = O1-cell(label: <amortized>)
#let Ologna = Ologn-cell(label: <amortized>)
#let Ona = On-cell(label: <amortized>)

= Time Complexity

== Linear Structures & Search Trees

#grid(
    columns: 12,
    column-gutter: (0em, 1em, 0em, 0em, 0em, 1em, 0em, 0em, 1em, 0em, 0em),
    grid.cell(rowspan: 2)[*Structure*],
    grid.cell(rowspan: 2)[*Diagram*],
    grid.cell(colspan: 4)[*Get Iterator*],
    grid.cell(colspan: 3)[*Insertion*],
    grid.cell(colspan: 3)[*Deletion*],
    [First], [Middle], [Last], [By Search], [First], [Middle], [Last], [First], [Middle], [Last],
    `Vector`, diag-vector, O1, O1, O1, On, On, On, O1a, On, On, O1,
    `Deque`, diag-deque, O1, O1, O1, On, O1a, On, O1a, O1, On, O1,
    `List`, diag-list, O1, On, O1, On, O1, O1, O1, O1, O1, O1,
    [Self-balancing\ Search Trees], diag-red-black-tree, O1, On-cell(label: <search-tree>), O1, Ologn, Ologn-cell(colspan: 3), Ologn-cell(colspan: 3),
)

== Addressable Heaps

#grid(
    columns: 7,
    column-gutter: (0em, 0em, 0em, 1em, 1em, 0em),
    grid.cell(rowspan: 2)[*Structure*],
    grid.cell(colspan: 3)[*Basic*],
    [*Mergable*],
    grid.cell(colspan: 2)[*Addressable*],
    [Get Apex], [Insertion], [Deletion at Apex], [Merge], [Update], [Deletion],
    `BinaryHeap`, O1, Ologna, Ologn, On, Ologn, Ologn,
)

/ \*: Amortized complexity.
<amortized>
/ 1: Can be $O(log n)$ if store the subtree size in the node.
<search-tree>
