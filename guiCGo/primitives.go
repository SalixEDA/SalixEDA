package main

/*
#include "graphics.h"
*/
import "C"

import (
  "unicode"
  )


// Константы для выравнивания текста
const (
  //Горизонтальное выравнивание
  AlignLeft    = 0
  AlignRight   = 1
  AlignHCenter = 2

  //Вертикальное выравнивание
  AlignBottom  = 0
  AlignVCenter = 1
  AlignTop     = 2
)


// ItemRect - прямоугольник
type ItemRect struct {
  Item
  r     int
  color C.uint32_t
  }


func NewItemRect(x, y, w, h int, color C.uint32_t) *ItemRect {
  ir := &ItemRect{
    Item: *NewItem(x, y, w, h),
    r:     0,
    color: color,
    }

  return ir
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



// ItemText - текст
type ItemText struct {
  Item
  text  string
  size  int
  align int
  color C.uint32_t
  }

func NewItemText(x, y int, text string, size int, color C.uint32_t) *ItemText {
  it := &ItemText{
    Item: *NewItem(x, y, 0, 0), // Размер будет вычислен позже
    text:  text,
    size:     size,
    align:    AlignLeft,
    color:    color,
    }

  return it
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




// ItemButton - кнопка
type ItemButton struct {
  Item
  text         string
  textSize     int
  textColor    C.uint32_t
  bgColor      C.uint32_t
  borderColor  C.uint32_t
  hoverColor   C.uint32_t
  pressedColor C.uint32_t
  radius       int
  state        int // 0 - normal, 1 - hover, 2 - pressed
  onClick      func(button *ItemButton)
  }

const (
	ButtonStateNormal = iota
	ButtonStateHover
	ButtonStatePressed
)

func NewItemButton(x, y, w, h int, text string) *ItemButton {
  btn := &ItemButton{
    Item:    *NewItem(x, y, w, h),
    text:        text,
    textSize:    16,
    textColor:   0xFFFFFF,
    bgColor:     0x404040,
    borderColor: 0x808080,
    hoverColor:  0x505050,
    pressedColor: 0x303030,
    radius:      5,
    state:       ButtonStateNormal,
    }

  return btn
  }



func (b *ItemButton) draw(x, y int) {
  if !b.visible {
    return
    }

  absX := x + b.x
  absY := y + b.y

  // Выбираем цвет в зависимости от состояния
  var bgColor C.uint32_t
  switch b.state {
    case ButtonStateHover:
      bgColor = b.hoverColor
    case ButtonStatePressed:
      bgColor = b.pressedColor
    default:
      bgColor = b.bgColor
    }

  // Рисуем фон
  C.drawRoundRectangle(C.int(absX), C.int(absY), C.int(b.w), C.int(b.h), C.int(b.radius), bgColor)

  // Рисуем рамку
  if b.borderColor != 0 {
    // Здесь можно нарисовать рамку
    C.drawRoundRect(C.int(absX), C.int(absY), C.int(b.w), C.int(b.h), C.int(b.radius), b.borderColor)
    }

  // Рисуем текст
  if b.text != "" {
    textX := absX + b.w/2
    textY := absY + (b.h-b.textSize)/2
    goDrawText(C.int(AlignHCenter), C.int(textX), C.int(textY), C.int(b.textSize), b.text, b.textColor)
    }

  b.drawChild(absX, absY)
  }





// ItemProgressBar - полоса прогресса
type ItemProgressBar struct {
  Item
  value     float64 // 0.0 - 1.0
  bgColor   C.uint32_t
  fgColor   C.uint32_t
  borderColor C.uint32_t
  radius    int
  showText  bool
  textSize  int
  textColor C.uint32_t
  }

func NewItemProgressBar(x, y, w, h int) *ItemProgressBar {
  pb := &ItemProgressBar{
    Item:    *NewItem(x, y, w, h),
    value:       0.0,
    bgColor:     0x404040,
    fgColor:     0x00A000,
    borderColor: 0x808080,
    radius:      3,
    showText:    true,
    textSize:    12,
    textColor:   0xFFFFFF,
    }

  return pb
  }


func (p *ItemProgressBar) SetValue(value float64) {
  if value < 0 {
    value = 0
    }
  if value > 1 {
    value = 1
    }
  p.value = value
  C.winRepaint()
  }

func (p *ItemProgressBar) draw(x, y int) {
  if !p.visible {
    return
    }

  absX := x + p.x
  absY := y + p.y

  // Фон
  C.drawRoundRectangle(C.int(absX), C.int(absY), C.int(p.w), C.int(p.h), C.int(p.radius), p.bgColor)
  //Контур
  C.drawRoundRect(C.int(absX), C.int(absY), C.int(p.w), C.int(p.h), C.int(p.radius), p.borderColor)

  // Прогресс
  if p.value > 0 {
    progressW := int(float64(p.w-4) * p.value)
    if progressW > 0 {
      C.drawRoundRectangle(C.int(absX+2), C.int(absY+2), C.int(progressW), C.int(p.h-4), C.int(p.radius-1), p.fgColor)
      }
    }

  // Текст
  if p.showText {
    percent := int(p.value * 100)
    text := itoa(percent) + "%"
    textX := absX + p.w/2
    textY := absY + (p.h-p.textSize)/2
    goDrawText(C.int(AlignHCenter), C.int(textX), C.int(textY), C.int(p.textSize), text, p.textColor)
    }

  p.drawChild(absX, absY)
  }






// ItemInputLine - строчный редактор
type ItemInputLine struct {
  Item
  text        string
  cursorPos   int
  selectionStart int
  selectionEnd   int
  maxLength   int
  textSize    int
  textColor   C.uint32_t
  bgColor     C.uint32_t
  borderColor C.uint32_t
  focusColor  C.uint32_t
  radius      int
  }

func NewItemInputLine(x, y, w, h int) *ItemInputLine {
  inp := &ItemInputLine{
    Item:    *NewItem(x, y, w, h),
    text:        "",
    cursorPos:   0,
    maxLength:   256,
    textSize:    16,
    textColor:   0xFFFFFF,
    bgColor:     0x202020,
    borderColor: 0x808080,
    focusColor:  0x00A0FF,
    radius:      3,
    }

  return inp
  }


func (in *ItemInputLine) setText(text string) {
  if len(text) > in.maxLength {
    text = text[:in.maxLength]
    }
  in.text = text
  if in.cursorPos > len(text) {
    in.cursorPos = len(text)
    }
  C.winRepaint()
  }



func (in *ItemInputLine) mouseClick( localX, localY int ) bool {
  // Проверяем, попадает ли мышь в область элемента
  localX -= in.x
  localY -= in.y
  if in.visible && localX >= 0 && localX < in.w && localY >= 0 && localY < in.h {
    // Устанавливаем фокус
    in.setFocus(true)

    // Устанавливаем позицию курсора по клику
    // Здесь нужна более сложная логика для определения позиции по координатам
    in.cursorPos = len(in.text)

    C.winRepaint()
    return true
    }
  return false
  }


func (in *ItemInputLine) keyDown( code int ) {
  if focusItem == in {
    switch code {
      case KeyLeft:
        if in.cursorPos > 0 {
          in.cursorPos--
          }
      case KeyRight:
        if in.cursorPos < len(in.text) {
          in.cursorPos++
          }
      case KeyBackspace:
        if in.cursorPos > 0 && len(in.text) > 0 {
          in.text = in.text[:in.cursorPos-1] + in.text[in.cursorPos:]
          in.cursorPos--
          }
      }
    C.winRepaint()
    }
  }


func (in *ItemInputLine) keyChar( code int ) {
  if focusItem == in {
    // Проверяем, что это печатный символ
    if code >= 32 && code < 127 && len(in.text) < in.maxLength {
      // Вставляем символ
      r := rune(code)
      if unicode.IsPrint(r) {
        before := in.text[:in.cursorPos]
        after := in.text[in.cursorPos:]
        in.text = before + string(r) + after
        in.cursorPos++
        C.winRepaint()
        }
      }
    }
  }



func (in *ItemInputLine) draw(x, y int) {
  if !in.visible {
    return
    }

  absX := x + in.x
  absY := y + in.y

  // Фон
  C.drawRoundRectangle(C.int(absX), C.int(absY), C.int(in.w), C.int(in.h), C.int(in.radius), in.bgColor)

  // Рамка
  borderColor := in.borderColor
  if focusItem == in {
    borderColor = in.focusColor
    }
  // Здесь можно нарисовать рамку
  C.drawRoundRect(C.int(absX), C.int(absY), C.int(in.w), C.int(in.h), C.int(in.radius), C.uint32_t(borderColor) )

  // Текст
  padding := 5
  textX := absX + padding
  textY := absY + (in.h-in.textSize)/2

  if in.text != "" {
    goDrawText(C.int(AlignLeft), C.int(textX), C.int(textY), C.int(in.textSize), in.text, in.textColor)
    }

  // Курсор
  if focusItem == in {
    // Здесь нужна логика для вычисления позиции курсора
    // Для простоты ставим в конец
    cursorX := 0
    if in.cursorPos > 0 {
      cursorX = goTextWidth( in.textSize, in.text, in.cursorPos )
      }
    C.drawRectangle(C.int(cursorX), C.int(textY), C.int(2), C.int(in.textSize), in.textColor)
    }

  in.drawChild(absX, absY)
  }



// Вспомогательная функция для преобразования int в строку
func itoa(n int) string {
  if n == 0 {
    return "0"
    }

  negative := false
  if n < 0 {
    negative = true
    n = -n
    }

  var digits []byte
  for n > 0 {
    digits = append([]byte{byte('0' + n%10)}, digits...)
    n /= 10
    }

  if negative {
    digits = append([]byte{'-'}, digits...)
    }

  return string(digits)
  }

