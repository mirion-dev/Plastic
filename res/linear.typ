#import "@preview/cetz:0.3.3": *

#set page(width: auto, height: auto, margin: 1em)

#let title = (grid.cell(rowspan: 3)[*Structure*], grid.cell(rowspan: 3)[*Diagram*], grid.cell(colspan: 10)[*Time Complexity*], grid.cell(colspan: 3)[*Insertion*], grid.cell(colspan: 3)[*Deletion*], grid.cell(colspan: 3)[*Access*], grid.cell(rowspan: 2)[*Search*], ..([Begin], [Middle], [End]) * 3)

#let O1 = grid.cell(fill: lime.lighten(50%), $O(1)$)
#let On = grid.cell(fill: red.lighten(50%), $O(n)$)
#let rowspan(n) = i=>grid.cell(rowspan: n, fill: i.fill, i.body)

#let line = draw.line.with(stroke: .5pt)
#let rect = draw.rect.with(stroke: .5pt)
#let crect = draw.rect.with(stroke: none)
#let arrow = draw.line.with(stroke: .5pt, mark: (end: "stealth", stroke: 0pt, fill: black))

#let inplace-vector-diag = canvas({
  crect((0, 0), (4, 1), fill: lime)
  for i in range(6){
    rect((i, 0), (i + 1, 1))
  }
}, length: 1em)

#let vector-diag = canvas({
  crect((0, 0), (4, 1), fill: lime)
  for x in range(6){
    rect((x, 0), (x + 1, 1))
  }
  for y in range(2){
    line((6, y), (6.5, y))
  }
}, length: 1em, padding: (left: .5))

#let inplace-deque-diag = canvas({
  crect((0, 0), (1, 1), fill: lime)
  crect((3, 0), (6, 1), fill: lime)
  crect((2, 0), (3, 1), fill: gray)
  for x in range(6){
    rect((x, 0), (x + 1, 1))
  }
}, length: 1em)

#let deque-diag = canvas({
  crect((1, 0), (5, 1), fill: lime)
  for x in range(6){
    rect((x, 0), (x + 1, 1))
  }
  for y in range(2){
    line((-.5, y), (0, y))
    line((6, y), (6.5, y))
  }
}, length: 1em)

#let forward-list-diag = canvas({
  rect((0, 0), (1, 1), fill: gray)
  for x in (1, 3){
    arrow((x, .5), (x + 1, .5))
    rect((x + 1, 0), (x + 2, 1), fill: lime)
  }
  arrow((5, .5), (5.75, .5), (5.75, -.3), (-.75, -.3), (-.75, .5), (0, .5))
}, length: 1em)

#let list-diag = canvas({
  rect((0, 0), (1, 1), fill: gray)
  for x in (1, 3){
    arrow((x, .3), (x + 1, .3))
    arrow((x + 1, .7), (x, .7))
    rect((x + 1, 0), (x + 2, 1), fill: lime)
  }
  arrow((5, .3), (5.75, .3), (5.75, -.3), (-.75, -.3), (-.75, .3), (0, .3))
  arrow((0, .7), (-.75, .7), (-.75, 1.3), (5.75, 1.3), (5.75, .7), (5, .7))
}, length: 1em)

#let vector = (`inplace_vector`, inplace-vector-diag, ..(On, On, O1, On, On, O1, O1, O1, O1, On).map(rowspan(2)), `vector`, vector-diag)
#let deque = (`inplace_deque`, inplace-deque-diag, ..(O1, On, O1, O1, On, O1, O1, O1, O1, On).map(rowspan(2)), `deque`, deque-diag)
#let forward-list = (`forward_list`, forward-list-diag, O1, On, On, O1, On, On, O1, On, On, On)
#let list = (`list`, list-diag, O1, On, O1, O1, On, O1, O1, On, O1, On)

#grid(align: center + horizon, columns: 12, inset: (left: 1em, right: 1em, top: .5em, bottom: .5em), column-gutter: (-1em, 0em, ..(0em, 0em, 1em) * 3), ..title, ..vector, ..deque, ..forward-list, ..list)