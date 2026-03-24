package main

type SgItemRow struct {
  SgItem
  Spacing int
  }



func NewSgItemRow(x, y, w, h, spacing int) *SgItemRow {
  return &SgItemRow{
    SgItem:  *NewSgItem(x, y, w, h),
    Spacing: spacing,
    }
  }




func (ir *SgItemRow) Resize() {
  if !ir.Visible {
    return
    }

  w := ir.Spacing
  h := int(0)
  for _, child := range ir.Child {
    child.Resize()
    child.SetPos( w, 0 )
    width, height := child.GetSize()
    w += width + ir.Spacing
    if h < height {
      h = height
      }
    }
  ir.Width = w
  ir.Height = h
  ir.ResizeBind()
  ir.ResizeChild()
  }

