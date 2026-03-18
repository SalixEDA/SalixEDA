package main


/*
#cgo linux LDFLAGS: -L/usr/lib/X11 -lX11 -lm
#include "guiGraphics.h"
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
  if len == 0 || str == "" {
    return 0
    }
  runes := []rune(str)           // 1. Создается срез runes
                                 //    Память выделена в Go
  //println("goTextWidth ", size, " ", str, " ", len )
  return int( C.textWidth( C.int(size), (*C.int)(unsafe.Pointer(&runes[0])), C.int(len) ) )
  }


func goTextLimit( width int, size int, str string, len int ) int {
  if len == 0 || str == "" {
    return 0
    }
  runes := []rune(str)           // 1. Создается срез runes
                                 //    Память выделена в Go

  return int( C.textLimit( C.int(width), C.int(size), (*C.int)(unsafe.Pointer(&runes[0])), C.int(len) ) )
  }



// ItemInterface - базовый интерфейс для всех виджетов
type ItemInterface interface {
  draw(x int, y int)
  resize()
  isHit(localX int, localY int) bool
  mouseClick(localX int, localY int) bool
  isHoverEnabled() bool
  hover( enter bool )
  keyDown(code int)
  keyChar(code int)
  setParent(parent ItemInterface)
  setFocus(focus bool)

  left(base ItemInterface) int
  right(base ItemInterface) int
  top(base ItemInterface) int
  bottom(base ItemInterface) int
  hCenter(base ItemInterface) int
  vCenter(base ItemInterface) int
  }




type Item struct {
  x,y     int
  w,h     int
  visible bool
  focus   bool
  parent  ItemInterface
  child []ItemInterface

  onResizeW  func(item *Item)
  onResizeH  func(item *Item)
  onClick    func(item *Item, localX int, localY int )
  }



func NewItem(x, y, w, h int) *Item {
  return &Item{
    x:      x,
    y:      y,
    w:      w,
    h:      h,
    visible: true,
    focus:   false,
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


func (i *Item) resize() {
  if !i.visible {
    return
    }
  if i.onResizeW != nil {
    i.onResizeW( i )
    }
  if i.onResizeH != nil {
    i.onResizeH( i )
    }
  for _, child := range i.child {
    child.resize()
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
  println( "Item key down ", code )
  }

func (i *Item) keyChar(code int) {
  println( "Item key char ", code )
  }


var focusItem ItemInterface
func setFocus( item ItemInterface ) {
  if focusItem != nil {
    focusItem.setFocus(false)
    }
  focusItem = item
  focusItem.setFocus(true)
  }


func (i *Item) setFocus(focus bool) {
  i.focus = focus
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
    return i.h
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





func (i *Item) horzEdges(base ItemInterface) (int, int) {
  return i.left(base), i.right(base)
  }

func (i *Item) vertEdges(base ItemInterface) (int, int) {
  return i.top(base), i.bottom(base)
  }




func (i *Item) leftEdge() func(base ItemInterface) int {
  return func(base ItemInterface) int {
    return i.left(base)
    }
  }

func (i *Item) rightEdge() func(base ItemInterface) int {
  return func(base ItemInterface) int {
    return i.right(base)
    }
  }

func (i *Item) hCenterEdge() func(base ItemInterface) int {
  return func(base ItemInterface) int {
    return i.hCenter(base)
    }
  }





func (i *Item) topEdge() func(base ItemInterface) int {
  return func(base ItemInterface) int {
    return i.top(base)
    }
  }

func (i *Item) bottomEdge() func(base ItemInterface) int {
  return func(base ItemInterface) int {
    return i.bottom(base)
    }
  }

func (i *Item) vCenterEdge() func(base ItemInterface) int {
  return func(base ItemInterface) int {
    return i.vCenter(base)
    }
  }






func (i *Item) anchorHorzLeft( anchor func(base ItemInterface) int, gap int ) {
  i.onResizeW = func( item *Item ) {
    item.x = anchor(item.parent) + gap
    }
  }

func (i *Item) anchorHorzRight( anchor func(base ItemInterface) int, gap int ) {
  i.onResizeW = func( item *Item ) {
    item.x = anchor(item.parent) - gap - item.w
    }
  }

func (i *Item) anchorHorzCenter( anchor func(base ItemInterface) int, gap int ) {
  i.onResizeW = func( item *Item ) {
    item.x = anchor(item.parent) + gap - item.w/2
    }
  }

func (i *Item) anchorHorzLeftRight( anchorLeft func(base ItemInterface) int, gapLeft int, anchorRight func(base ItemInterface) int, gapRight int ) {
  i.onResizeW = func( item *Item ) {
    item.x = anchorLeft(item.parent) + gapLeft
    item.w = anchorRight(item.parent) - gapRight - item.x
    }
  }

func (i *Item) anchorHorzFill( ref *Item, gapLeft int, gapRight int ) {
  i.onResizeW = func( item *Item ) {
    left,right := ref.horzEdges(item.parent)
    item.x = left + gapLeft
    item.w = right - gapRight - item.x
    }
  }



func (i *Item) anchorVertTop( anchor func(base ItemInterface) int, gap int ) {
  i.onResizeH = func( item *Item ) {
    item.y = anchor(item.parent) + gap
    }
  }

func (i *Item) anchorVertBottom( anchor func(base ItemInterface) int, gap int ) {
  i.onResizeH = func( item *Item ) {
    item.y = anchor(item.parent) - gap - item.h
    }
  }

func (i *Item) anchorVertCenter( anchor func(base ItemInterface) int, gap int ) {
  i.onResizeH = func( item *Item ) {
    item.y = anchor(item.parent) + gap - item.h/2
    }
  }

func (i *Item) anchorVertTopBottom( anchorTop func(base ItemInterface) int, gapTop int, anchorBottom func(base ItemInterface) int, gapBottom int ) {
  i.onResizeH = func( item *Item ) {
    item.y = anchorTop(item.parent) + gapTop
    item.w = anchorBottom(item.parent) - gapBottom - item.y
    }
  }

func (i *Item) anchorVertFill( ref *Item, gapTop int, gapBottom int ) {
  i.onResizeH = func( item *Item ) {
    top,bottom := ref.vertEdges(item.parent)
    item.y = top + gapTop
    item.h = bottom - gapBottom - item.y
    }
  }




func (i *Item) anchorFillGap( ref *Item, gapLeft int, gapRight int, gapTop int, gapBottom int ) {
  i.anchorHorzFill( ref, gapLeft, gapRight )
  i.anchorVertFill( ref, gapTop, gapBottom )
  }

func (i *Item) anchorFill( ref *Item, gap int ) {
  i.anchorFillGap( ref, gap, gap, gap, gap )
  }

func (i *Item) anchorFillDef( ref *Item ) {
  i.anchorFillGap( ref, 0, 0, 0, 0 )
  }


func (i *Item) anchorCenterIn( ref *Item, gapHorz int, gapVert int ) {
  i.anchorHorzCenter( ref.hCenterEdge(), gapHorz )
  i.anchorVertCenter( ref.vCenterEdge(), gapVert )
  }

func (i *Item) anchorCenterInDef( ref *Item ) {
  i.anchorHorzCenter( ref.hCenterEdge(), 0 )
  i.anchorVertCenter( ref.vCenterEdge(), 0 )
  }







var screen = Item {
  x: 0,
  y: 0,
  w: 0,
  h: 0,
  visible: true,
  child: make( []ItemInterface, 0 ),
  }



//export goPaint
func goPaint( width C.int, height C.int ) {
  screen.w = int(width)
  screen.h = int(height)
  screen.resize()
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

/*
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

  C.winStart()
}

*/