#let init = el => {
    set page(width: auto, height: auto, margin: 1em)
    set grid(align: center + horizon, inset: (left: 1em, right: 1em, top: .5em, bottom: .5em))
    el
}

#let O1 = grid.cell(fill: lime.lighten(50%), $O(1)$)
#let O1a = grid.cell(fill: lime.lighten(50%), $O(1)^*$)
#let Ologn = grid.cell(fill: yellow.lighten(50%), $O(log n)$)
#let Ologna = grid.cell(fill: yellow.lighten(50%), $O(log n)^*$)
#let On = grid.cell(fill: red.lighten(50%), $O(n)$)
#let Ona = grid.cell(fill: red.lighten(50%), $O(n)^*$)
