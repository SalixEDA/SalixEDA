package main



func main() {
 // back := NewItemRect( 0, 0, 200, 70, 0xf000 )
 // back.onResizeW = centerInParentW
 // back.onResizeH = centerInParentH
 // screen.add( back )

// Создаем фон
  background := NewSgItemRect(0, 0, 0, 0, 0x202020)
  background.AnchorFill( &SgScreen, 0 )
  SgScreen.Add(background)

  // Создаем панель в центре
  panel := NewSgItemRect(0, 0, 400, 300, 0x303030)
  panel.AnchorFill( &SgScreen, 50 )
  panel.Radius = 10
  SgScreen.Add(panel)

  // Заголовок
  title := NewSgItemText(0, 20, "Настройка системы", 24, 0xFFFFFF)
  title.Align = AlignHCenter
  title.AnchorHorzCenter( panel.HCenterEdge(), 0 )
  panel.Add(title)

  // Поле ввода
  input := NewSgItemInputLine(0, 70, 360, 30)
  input.AnchorHorzCenter( panel.HCenterEdge(), 0 )
  input.TextSet( "Primer" )
  panel.Add(input)
  SetFocus(input)

  // Полоса прогресса
  progress := NewSgItemProgressBar(0, 120, 360, 20)
  progress.AnchorHorzCenter( panel.HCenterEdge(), 0 )
  progress.SetValue(0.66)
  panel.Add(progress)

  // Кнопки
  okButton := NewSgItemButton(0, 160, 100, 30, "OK")
  okButton.AnchorHorzRight( panel.RightEdge(), 10 )
  okButton.AnchorVertBottom( panel.BottomEdge(), 10 )
  okButton.OnClick = func(item *SgItem, localX int, localY int) {
    println("OK clicked!")
    SgWinClose()
    }
  panel.Add(okButton)

  //Dialog
  dialog := NewSgItemModal( 0, 0, 400, 300, 8, 0x808030, 0xa0a050 )

  cancelButton := NewSgItemButton(10, 160, 100, 30, "Отмена")
  cancelButton.AnchorVertBottom( okButton.BottomEdge(), 0 )
  cancelButton.OnClick = func(item *SgItem, localX int, localY int) {
    println("Cancel clicked!")
    go func() {
      res := dialog.Exec()
      println("Dialog completed with ", res )
      } ()
    }
  panel.Add(cancelButton)

  testImage := NewSgItemImage( 5, 5, "3dHorzMove.png" )
  panel.Add( testImage )

  dialog.AnchorCenterIn( &SgScreen, 0, 0 )
  SgScreen.Add( dialog )

  dialogButton := NewSgItemButton( 50, 40, 100, 30, "Закрыть" )
  dialogButton.OnClick = func( item *SgItem, localX int, localY int ) {
    println("Dialog cancel")
    dialog.Done( 5 )
    }
  dialog.Add( dialogButton )

  SgWinStart( 300, 600, 800, 500, "SalixEDA setup program" )
  }



