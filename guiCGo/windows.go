package main


/*
#cgo linux LDFLAGS: -L/usr/lib/X11 -lX11 -lm
#include "graphics.h"
*/
import "C"

import (
  "unsafe"
  )



// Коды клавиш (можно расширить)
type KeyCode int

const (
  KeyNone KeyCode = iota
  KeyBackspace = 0x0e
  KeyEnter     = 0x1c
  KeyEscape    = 0x01
  KeyTab       = 0x0f
  KeyLeft      = 0x4b
  KeyRight     = 0x4d
  KeyUp        = 0x48
  KeyPgUp      = 0x49
  KeyDown      = 0x50
  KeyPgDn      = 0x51
  KeyHome      = 0x47
  KeyEnd       = 0x4f
  KeyDelete    = 0x53

)




func goDrawText( align C.int, x C.int, y C.int, size C.int, str string, color C.uint32_t ) {
  runes := []rune(str)           // 1. Создается срез runes
                                 //    Память выделена в Go

  C.drawText( align, x, y, size,
        (*C.int)(unsafe.Pointer(&runes[0])),  // 2. Передаем указатель в C
        C.int(len(runes)),
        color,
    )                              // 3. drawText выполняется (блокирует)
                                   // 4. Функция завершается
}                                  // 5. runes выходит из области видимости
                                   //    и становится доступен для GC



func goTextWidth( size int, str string, len int ) int {
  runes := []rune(str)           // 1. Создается срез runes
                                 //    Память выделена в Go

  return int( C.textWidth( C.int(size), (*C.int)(unsafe.Pointer(&runes[0])), C.int(len) ) )
  }



// ItemInterface - базовый интерфейс для всех виджетов
type ItemInterface interface {
  draw(x int, y int)
  resizeW(parentWidth int)
  resizeH(parentHeight int)
  isHit(localX int, localY int) bool
  mouseClick(localX int, localY int) bool
  isHoverEnabled() bool
  hover( enter bool )
  keyDown(code int)
  keyChar(code int)
  setParent(parent ItemInterface)

  left(base ItemInterface) int
  right(base ItemInterface) int
  top(base ItemInterface) int
  bottom(base ItemInterface) int
  hCenter(base ItemInterface) int
  vCenter(base ItemInterface) int
  }




type Item struct {
  x,y int
  w,h int
  visible bool
  parent  ItemInterface
  child []ItemInterface

  onResizeW  func(item *Item, parentWidth int )
  onResizeH  func(item *Item, parentHeight int )
  onClick    func(item *Item, localX int, localY int )
  }



func NewItem(x, y, w, h int) *Item {
  return &Item{
    x:      x,
    y:      y,
    w:      w,
    h:      h,
    visible: true,
    child:   make([]ItemInterface, 0),
    }
  }




func (i *Item) add(item ItemInterface) {
  item.setParent( i )
  i.child = append(i.child, item)
  }




func (i *Item) setPos(x int, y int) {
  i.x = x
  i.y = y
  }




func (i *Item) setSize(w int, h int) {
  i.w = w
  i.h = h
  }




func (i *Item) setVisible(vis bool) {
  i.visible = vis
  }




func (i *Item) drawChild( bx int, by int ) {
  if i.visible {
    for _, child := range i.child {
      child.draw( bx, by )
      }
    }
  }


func (i *Item) draw(x int, y int) {
  i.drawChild( x + i.x, y + i.y )
  }


func (i *Item) resizeW(parentWidth int) {
  if i.onResizeW != nil {
    i.onResizeW( i, parentWidth )
    //println("x:", i.x, "y:", i.y, "w:", i.w, "h:", i.h)
    }
  for _, child := range i.child {
    child.resizeW( i.w )
    }
  }

func (i *Item) resizeH(parentHeight int) {
  if i.onResizeH != nil {
    i.onResizeH( i, parentHeight )
    //println("x:", i.x, "y:", i.y, "w:", i.w, "h:", i.h)
    }
  for _, child := range i.child {
    child.resizeH( i.h )
    }
  }


var mouseItem ItemInterface

func (i *Item) isHit(localX int, localY int) bool {
  // Проверяем, попадает ли мышь в область элемента
  localX -= i.x
  localY -= i.y
  if i.visible && localX >= 0 && localX < i.w && localY >= 0 && localY < i.h {
    // Мышь внутри элемента
    for j := len(i.child) - 1; j >= 0; j-- {
      if i.child[j].isHit( localX, localY ) {
        return true
        }
      }
    //Execute own mouse function
    if i.isHoverEnabled() {
      if mouseItem != i {
        if mouseItem != nil {
          mouseItem.hover( false )
          }
        mouseItem = i
        mouseItem.hover( true )
        }
      return true
      }
    }
  return false
  }



func (i *Item) mouseClick(localX int, localY int) bool {
  // Проверяем, попадает ли мышь в область элемента
  localX -= i.x
  localY -= i.y
  if i.visible && localX >= 0 && localX < i.w && localY >= 0 && localY < i.h {
    // Мышь внутри элемента
    for j := len(i.child) - 1; j >= 0; j-- {
      if i.child[j].mouseClick( localX, localY ) {
        return true
        }
      }
    //Execute own mouse function
    if i.onClick != nil {
      i.onClick( i, localX, localY )
      return true
      }
    }
  return false
  }


func (i *Item) isHoverEnabled() bool {
  return false
  }


func (i *Item) hover( enter bool ) {
  }


func (i *Item) keyDown(code int) {
  }

func (i *Item) keyChar(code int) {
  }


var focusItem ItemInterface

func (i *Item) setFocus(focus bool) {
  if focus {
    focusItem = i
    } else {
    focusItem = nil
    }
  }


func (i *Item) setParent(parent ItemInterface) {
  i.parent = parent
  }







//Вернуть левую позицию в координатах объекта base
func (i *Item) left(base ItemInterface) int {
  //Если текущий объект и есть базовый, то возвращаем 0
  if( i == base ) {
    return 0
    }
  return i.parent.left(base) + i.x
  }



//Вернуть правую позицию в координатах объекта base
func (i *Item) right(base ItemInterface) int {
  //Если текущий объект и есть базовый, то возвращаем i.w
  if( i == base ) {
    return i.w
    }
  return i.parent.left(base) + i.x + i.w
  }




//Вернуть верхнюю позицию в координатах объекта base
func (i *Item) top(base ItemInterface) int {
  //Если текущий объект и есть базовый, то возвращаем 0
  if( i == base ) {
    return 0
    }
  return i.parent.top(base) + i.y
  }




//Вернуть нижнюю позицию в координатах объекта base
func (i *Item) bottom(base ItemInterface) int {
  //Если текущий объект и есть базовый, то возвращаем 0
  if( i == base ) {
    return 0
    }
  return i.parent.top(base) + i.y + i.h
  }



//Вернуть центр по горизонтали
func (i *Item) hCenter(base ItemInterface) int {
  //Если текущий объект и есть базовый, то возвращаем 0
  if( i == base ) {
    return i.w / 2
    }
  return i.parent.left(base) + i.x + i.w / 2
  }



//Вернуть центр по вертикали
func (i *Item) vCenter(base ItemInterface) int {
  //Если текущий объект и есть базовый, то возвращаем 0
  if( i == base ) {
    return i.h / 2
    }
  return i.parent.top(base) + i.y + i.h / 2
  }


func (i *Item) fillParent() {
  i.onResizeW = fillParentW
  i.onResizeH = fillParentH
  }


func (i *Item) centerInParent() {
  i.onResizeW = centerInParentW
  i.onResizeH = centerInParentH
  }





func fillParentW( item *Item, parentWidth int ) {
  item.x = 0
  item.w = parentWidth
  }

func fillParentH( item *Item, parentHeight int ) {
  item.y = 0
  item.h = parentHeight
  }

func centerInParentW( item *Item, parentWidth int ) {
  item.x = (parentWidth - item.w) / 2
  }

func centerInParentH( item *Item, parentHeight int ) {
  item.y = (parentHeight - item.h) / 2
  }






var screen = Item {
  x: 0,
  y: 0,
  w: 0,
  h: 0,
  visible: true,
  child: make( []ItemInterface, 0 ),
  onResizeW: fillParentW,
  onResizeH: fillParentH,
  }



//export goPaint
func goPaint( width C.int, height C.int ) {
  screen.resizeW( int(width) )
  screen.resizeH( int(height) )
  screen.drawChild( 0, 0 )
  }




var mousePrevLeftButton bool

//export goMouse
func goMouse( x C.int, y C.int, leftButton C.int, rightButton C.int ) {
  //Move mouse
  screen.isHit( int(x), int(y) )
  if !mousePrevLeftButton && bool(leftButton != 0) {
    screen.mouseClick( int(x), int(y) )
    }
  mousePrevLeftButton = bool(leftButton != 0)
  }




//export goKeyDown
func goKeyDown( code C.int ) {
  if focusItem != nil {
    focusItem.keyDown( int(code) )
    }
  }



//export goKeyUp
func goKeyUp( code C.int ) {
}




//export goKeyChar
func goKeyChar( code C.int ) {
  if focusItem != nil {
    focusItem.keyChar( int(code) )
    }
  }


func main() {
 // back := NewItemRect( 0, 0, 200, 70, 0xf000 )
 // back.onResizeW = centerInParentW
 // back.onResizeH = centerInParentH
 // screen.add( back )

// Создаем фон
  background := NewItemRect(0, 0, 0, 0, 0x202020)
  background.fillParent()
  screen.add(background)

  // Создаем панель в центре
  panel := NewItemRect(0, 0, 400, 300, 0x303030)
  panel.centerInParent()
  panel.r = 10
  screen.add(panel)
                       /*
  rc := NewItemRect( 0, 0, 150, 20, 0x606060 )
  rc.centerInParent()
  rc.r = 5
  panel.add( rc )
                     */
  // Заголовок
  title := NewItemText(0, 20, "Настройка системы", 24, 0xFFFFFF)
  title.align = AlignHCenter
  title.onResizeW = func( item *Item, parentWidth int ) {
    item.x = parentWidth / 2
    }
  panel.add(title)

  // Поле ввода
  input := NewItemInputLine(0, 70, 360, 30)
  input.onResizeW = centerInParentW
  panel.add(input)

  // Полоса прогресса
  progress := NewItemProgressBar(0, 120, 360, 20)
  progress.onResizeW = centerInParentW
  progress.SetValue(0.66)
  panel.add(progress)

  // Кнопки
  okButton := NewItemButton(0, 160, 100, 30, "OK")
  okButton.onResizeW = func(item *Item, pw int) {
    item.x = pw - item.w - 10
    }
  okButton.onResizeH = func(item *Item, ph int) {
    item.y = ph - item.h - 10
    }
  okButton.onClick = func(item *Item, localX int, localY int) {
    println("OK clicked!")
    }
  panel.add(okButton)

  cancelButton := NewItemButton(10, 160, 100, 30, "Отмена")
  cancelButton.onResizeH = func(item *Item, ph int) {
    item.y = ph - item.h - 10
    }
  cancelButton.onClick = func(item *Item, localX int, localY int) {
    println("Cancel clicked!")
    }
  panel.add(cancelButton)

  C.winStart()
}

