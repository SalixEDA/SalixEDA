package main



func main() {
 // back := NewItemRect( 0, 0, 200, 70, 0xf000 )
 // back.onResizeW = centerInParentW
 // back.onResizeH = centerInParentH
 // screen.add( back )

// Создаем фон
  background := NewItemRect(0, 0, 0, 0, 0x202020)
  background.anchorFill( &screen, 0 )
  screen.add(background)

  // Создаем панель в центре
  panel := NewItemRect(0, 0, 400, 300, 0x303030)
  panel.anchorFill( &screen, 50 )
  panel.r = 10
  screen.add(panel)

  // Заголовок
  title := NewItemText(0, 20, "Настройка системы", 24, 0xFFFFFF)
  title.align = AlignHCenter
  title.anchorHorzCenter( panel.hCenterEdge(), 0 )
  panel.add(title)

  // Поле ввода
  input := NewItemInputLine(0, 70, 360, 30)
  input.anchorHorzCenter( panel.hCenterEdge(), 0 )
  input.textSet( "Primer" )
  panel.add(input)
  setFocus(input)

  // Полоса прогресса
  progress := NewItemProgressBar(0, 120, 360, 20)
  progress.anchorHorzCenter( panel.hCenterEdge(), 0 )
  progress.SetValue(0.66)
  panel.add(progress)

  // Кнопки
  okButton := NewItemButton(0, 160, 100, 30, "OK")
  okButton.anchorHorzRight( panel.rightEdge(), 10 )
  okButton.anchorVertBottom( panel.bottomEdge(), 10 )
  okButton.onClick = func(item *Item, localX int, localY int) {
    println("OK clicked!")
    }
  panel.add(okButton)

  cancelButton := NewItemButton(10, 160, 100, 30, "Отмена")
  cancelButton.anchorVertBottom( okButton.bottomEdge(), 0 )
  cancelButton.onClick = func(item *Item, localX int, localY int) {
    println("Cancel clicked!")
    }
  panel.add(cancelButton)

  goWinStart()
}



