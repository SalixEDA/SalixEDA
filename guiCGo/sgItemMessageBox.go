package main

type SgItemMessageBox struct {
  SgItemModal
  title  SgItemText
  text   SgItemTextMulty
  button SgItemButton
  }


func NewSgItemMessageBox( w int, h int ) *SgItemMessageBox {
  mb := &SgItemMessageBox {
    SgItemModal: *NewSgItemModal( 0, 0, w, h, 10, 0xF0F0F0, 0x969696 ),
    title:       *NewSgItemText( 0, 0, "", 20, 0x242934 ),
    text:        *NewSgItemTextMulty( 0, 0, w - 10, "", 18, 0x202020 ),
    button:      *NewSgItemButton( 0, 0, 100, 30, "Ok" ),
    }

  //Title background
  titleBackground := NewSgItemRect( 1, 1, w-2, 40, 0xE8E8E8 )
  titleBackground.Radius = 9
  titleBackground.UpRound = true
  titleBackground.AnchorHorzFill( mb, 1, 1 )
  mb.Add( titleBackground )

  //Title
  mb.title.AnchorCenterIn( titleBackground, 0, 0 )
  mb.title.Align = AlignHCenter | AlignVCenter
  titleBackground.Add( &mb.title )

  mb.text.AnchorHorzFill( mb, 10, 10 )
  mb.text.AnchorVertTop( titleBackground.BottomEdge(), 0 )
  mb.text.Align = AlignLeft | AlignTop
  mb.Add( &mb.text )

  //Button
  mb.button.AnchorHorzCenter( mb.HCenterEdge(), 0 )
  mb.button.AnchorVertBottom( mb.BottomEdge(), 10 )
  mb.Add( &mb.button )

  mb.button.OnClick = func(item *SgItem, localX int, localY int) {
    mb.Done(1)
    }

  return mb
  }


func (mb *SgItemMessageBox) Message ( title string, text string ) int {
  mb.title.TextSet( title )
  mb.text.TextSet( text )
  return mb.Exec()
  }
