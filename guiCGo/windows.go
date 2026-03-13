package main


/*
#include "graphics.h"
*/
import "C"

import (
  "unsafe"
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

// ItemInterface - базовый интерфейс для всех виджетов
type ItemInterface interface {
  draw(x int, y int)
  resizeW(parentWidth int)
  resizeH(parentHeight int)
  isHit(localX int, localY int) bool
  mouse(localX int, localY int, leftButton bool, rightButton bool)
  keyDown(code int) bool
  keyUp(code int) bool
  keyChar(code int) bool
  setFocus(focus bool)
  hasFocus() bool
  getRect() (x, y, w, h int)
  setParent(parent Item)
  }

type Item struct {
  x,y int
  w,h int
  visible bool
  focused bool
  hover   bool
  parent  ItemInterface
  child []ItemInterface

  onResizeW  func(item *Item, parentWidth int )
  onResizeH  func(item *Item, parentHeight int )
  onClick    func(item *Item, localX int, localY int )
  onKeyDown  func(item *Item, code int) bool
  onKeyChar  func(item *Item, code int) bool
  onFocus    func(item *Item, focused bool)
  }

func NewItem(x, y, w, h int) *Item {
  return &ItemImpl{
    x:      x,
    y:      y,
    w:      w,
    h:      h,
    visible: true,
    child:   make([]ItemInterface, 0),
    }
  }

func (i *ItemImpl) add(item Item) {
  item.parent = i
  i.child = append(i.child, item)
  }

func (i *ItemImpl) setPos(x int, y int) {
  i.x = x
  i.y = y
  }

func (i *ItemImpl) setSize(w int, h int) {
  i.w = w
  i.h = h
  }

func (i *ItemImpl) setVisible(vis bool) {
  i.visible = vis
  }

func (i *ItemImpl) drawChild( bx int, by int ) {
  if i.visible {
    bx += i.x
    by += i.y
    for _, child := range i.child {
      child.draw( bx, by )
      }
    }
  }

func (i *ItemImpl) resize( parentWidth int, parentHeight int ) {
  if i.resizeFunc != nil {
    oldW := i.w
    oldH := i.h
    i.resizeFunc( i, parentWidth, parentHeight )

    if oldW != i.w || oldH != i.h {
      for _, child := range i.child {
        child.resize( i.w, i.h )
        }
      }
    }
  }

func (i *ItemImpl) mouse(localX int, localY int, leftButton bool, rightButton bool) bool {
  // Проверяем, попадает ли мышь в область элемента
  localX -= i.x
  localY -= i.y
  if localX >= 0 && localX < i.w && localY >= 0 && localY < i.h {
    // Мышь внутри элемента
    for j := len(i.child) - 1; j >= 0; j-- {
      if i.child[j].mouse( localX, localY, leftButton, rightButton ) {
        return true
        }
      }
    //Execute own mouse function
    if mouseFunc != nil {
      mouseFunc( localX, localY, leftButton, rightButton )
      return true
      }
    }
  return false
  }

//Вернуть левую позицию в координатах объекта base
func (i *ItemImpl) Left( base *ItemImpl) int {
  //Если текущий объект и есть базовый, то возвращаем 0
  if( i == base ) {
    return 0
    }
  return i.parent.Left(base) + i.x
  }

//Вернуть правую позицию в координатах объекта base
func (i *ItemImpl) Right( base *ItemImpl) int {
  //Если текущий объект и есть базовый, то возвращаем i.w
  if( i == base ) {
    return i.w
    }
  return i.parent.Left(base) + i.x + i.w
  }

//Вернуть верхнюю позицию в координатах объекта base
func (i *ItemImpl) Top( base *ItemImpl) int {
  //Если текущий объект и есть базовый, то возвращаем 0
  if( i == base ) {
    return 0
    }
  return i.parent.Top(base) + i.y
  }

//Вернуть нижнюю позицию в координатах объекта base
func (i *ItemImpl) Bottom( base *ItemImpl) int {
  //Если текущий объект и есть базовый, то возвращаем 0
  if( i == base ) {
    return 0
    }
  return i.parent.Top(base) + i.y + i.h
  }

//Вернуть центр по горизонтали
func (i *ItemImpl) HCenter( base *ItemImpl) int {
  //Если текущий объект и есть базовый, то возвращаем 0
  if( i == base ) {
    return i.w / 2
    }
  return i.parent.Left(base) + i.x + i.w / 2
  }

//Вернуть центр по вертикали
func (i *ItemImpl) VCenter( base *ItemImpl) int {
  //Если текущий объект и есть базовый, то возвращаем 0
  if( i == base ) {
    return i.h / 2
    }
  return i.parent.Top(base) + i.y + i.h / 2
  }




type ItemRect struct {
  ItemImpl
  r     int
  color C.uint32_t
  }

func (ir *ItemRect) draw(x int, y int) {
  // Проверка видимости - обращаемся через ir
  if !ir.visible {
    return
    }

  // Вычисляем абсолютные координаты с учетом родительских
  absX := x + ir.x
  absY := y + ir.y

  // Рисуем прямоугольник
  C.drawRoundRectangle(C.int(absX), C.int(absY), C.int(ir.w), C.int(ir.h), C.int(ir.r), ir.color )

  // Рисуем все дочерние элементы
  ir.drawChild( absX, absY )
  }



type ItemText struct {
  ItemImpl
  text  string
  size  int
  align int
  color C.uint32_t
  }


func (it *ItemText) draw(x int, y int) {
  // Проверка видимости - обращаемся через it
  if !it.visible {
    return
    }

  // Вычисляем абсолютные координаты с учетом родительских
  absX := x + it.x
  absY := y + it.y

  // Рисуем
  goDrawText( C.int(it.align), C.int(absX), C.int(absY), C.int(it.size), it.text, it.color )

  // Рисуем все дочерние элементы
  it.drawChild( absX, absY )
  }


func fillParent( item *ItemImpl, parentWidth int, parentHeight int ) {
  item.w = parentWidth
  item.h = parentHeight
  }

func centerInParent( item *ItemImpl, parentWidth int, parentHeight int ) {
  item.x = (parentWidth - item.w) / 2
  item.y = (parentHeight - item.h) / 2
  }



var screen = ItemImpl {
  x: 0,
  y: 0,
  w: 0,
  h: 0,
  visible: true,
  child: make( []Item, 0 ),
  resizeFunc: fillParent,
  }



//export goPaint
func goPaint( width C.int, height C.int ) {
  screen.resize( int(width), int(height) )
  screen.drawChild( 0, 0 )
  }

//export goMouse
func goMouse( x C.int, y C.int, leftButton C.int, rightButton C.int ) {
  screen.mouse( int(x), int(y), bool(leftButton), bool(rightButton) )
  }

//export goKeyDown
func goKeyDown( code C.int ) {
}

//export goKeyUp
func goKeyUp( code C.int ) {
}

//export goKeyChar
func goKeyChar( code C.int ) {
}


func main() {
  back := &ItemRect {
    ItemImpl: ItemImpl {
      w: 200,
      h: 70,
      resizeFunc: centerInParent,
      child: make( []Item, 0 ),
      visible: true,
      },
    color: 0xf00000,
    }
  screen.Add( back )

  C.winStart()
}

