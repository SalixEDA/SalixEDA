package main

type SgItemColumn struct {
  SgItem
  Spacing int
  }



func NewSgItemColumn(x, y, w, h, spacing int) *SgItemColumn {
  return &SgItemColumn{
    SgItem:  *NewSgItem(x, y, w, h),
    Spacing: spacing,
    }
  }




func (ic *SgItemColumn) Resize() {
  if !ic.Visible {
    return
    }

  w := int(0)
  h := ic.Spacing
  for _, child := range ic.Child {
    child.Resize()
    child.SetPos( 0, h )
    width, height := child.GetSize()
    h += height + ic.Spacing
    if w < width {
      w = width
      }
    }
  ic.Width = w
  ic.Height = h
  ic.ResizeBind()
  ic.ResizeChild()
  }

