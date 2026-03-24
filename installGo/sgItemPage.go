package main

type SgItemPage struct {
  SgItemRect
  Title      *SgItemText
  //HeaderLine SgItemRect
  Footer     *SgItemRow
  //FooterLine SgItemRect
  Central    *SgItem
  }


func NewSgItemPage( pageColor uint32, title string, titleSize int, titleColor uint32) *SgItemPage {
  ip := &SgItemPage{
    SgItemRect:  *NewSgItemRect(0, 0, 10, 10, pageColor ),
    Title:        NewSgItemText( 0, 0, title, titleSize, titleColor ),
    Footer:       NewSgItemRow( 0, 0, 10, 10, 10 ),
    Central:      NewSgItem(0, 0, 10, 10),
    }

  ip.Title.Align = AlignHCenter | AlignTop
  ip.Title.AnchorHorzCenterIn( ip, 0 )
  ip.Title.AnchorVertTop( ip.TopEdge(), 20 )
  ip.Add( ip.Title )

  headerLine := NewSgItemRect( 0, 60, 10, 2, 0x404040 )
  headerLine.AnchorHorzFill( ip, 5, 5 )
  ip.Add( headerLine )

  ip.Footer.AnchorHorzRight( ip.RightEdge(), 10 )
  ip.Footer.AnchorVertBottom( ip.BottomEdge(), 10 )
  ip.Add( ip.Footer )

  footerLine := NewSgItemRect( 0, 60, 10, 2, 0x404040 )
  footerLine.AnchorHorzFill( ip, 5, 5 )
  footerLine.AnchorVertBottom( ip.Footer.TopEdge(), 10 )
  ip.Add( footerLine )

  ip.Central.AnchorHorzFill( ip, 5, 5 )
  ip.Central.AnchorVertTopBottom( headerLine.BottomEdge(), 5, footerLine.TopEdge(), 5 )
  ip.Add( ip.Central )

  return ip
  }




func (i *SgItemPage) ParentSet(parent SgItemInterface) {
  i.Parent = parent
  i.AnchorFillDef( parent )
  }



func (i *SgItemPage) CentralSet( item SgItemInterface ) {
  i.Central.ClearChild()
  i.Central.Add( item )
  item.AnchorFillDef( i.Central )
  }


func (i *SgItemPage) AddFooter(items ...SgItemInterface) {
  i.Footer.Add( items... )
  }

