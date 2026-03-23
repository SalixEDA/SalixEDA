package main

type SgItemContainer struct {
  SgItem
  }



func NewSgItemContainer(x, y, w, h int) *SgItemContainer {
  return &SgItemContainer{
    SgItem:  *NewSgItem(x, y, w, h),
    }
  }




func (i *SgItemContainer) Add(items ...SgItemInterface) {
  // Проходим по всем
  for _, item := range items {
    item.ParentSet( i )
    i.Child = append(i.Child, item)
    item.AnchorFillDef( i )
    if len(i.Child) == 1 {
      item.SetVisible( true )
     } else {
      item.SetVisible( false )
      }
    }
  }



func (i *SgItemContainer) SetCurrent( index int ) {
  //Scan all childs and switch off their visible flag except only with equal index
  for _, child := range i.Child {
    child.SetVisible( index == 0 )
    index--
    }
  }


