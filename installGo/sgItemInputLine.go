package main

import (
  "unicode"
  )


// ItemInputLine - строчный редактор с UTF-16 внутренним представлением
type SgItemInputLine struct {
  SgItem
  text           SgString  // Внутреннее представление в UTF-32
  cursorPos      int       // Позиция курсора в символах UTF-16
  scrollOffset   int       // Индекс первого отображаемого символа
  selectionStart int
  selectionEnd   int
  maxLength      int
  TextSize       int
  TextColor      uint32
  BgColor        uint32
  BorderColor    uint32
  FocusColor     uint32
  Radius         int
}


// NewItemInputLine создает новый элемент ввода
func NewSgItemInputLine(x, y, w, h int) *SgItemInputLine {
  in := &SgItemInputLine{
    SgItem:        *NewSgItem(x, y, w, h),
    text:           SgStringFromUtf8(""),
    cursorPos:      0,
    scrollOffset:   0,
    TextSize:       16,
    TextColor:      0xFFFFFF,
    BgColor:        0x202020,
    BorderColor:    0x808080,
    FocusColor:     0x00A0FF,
    Radius:         5,
    }

  return in
  }


func (in *SgItemInputLine) TextGet() string {
  return in.text.ToUtf8()
  }

func (in *SgItemInputLine) TextSet( str string ) {
  in.text = SgStringFromUtf8(str)

  // Корректируем позицию курсора
  if in.cursorPos > in.text.Length() {
    in.cursorPos = in.text.Length()
    }

  // Корректируем скролл
  in.adjustScrollOffset()

  SgWinRepaint()
  }










// getDisplaySymbolPos вычисляет позицию символа с учетом скролла
func (in *SgItemInputLine) getDisplaySymbolPos(index int) int {
  if index == in.scrollOffset {
    return 0
    }
  if index < in.scrollOffset {
    return -10
    }

  // Получаем ширину текста от scrollOffset до index
  scrollText := in.text.Mid( in.scrollOffset, -1 )

  // Измеряем ширину текста до index и вычитаем ширину скроллированной части
  scrollWidth := SgTextWidth( in.TextSize, scrollText, index - in.scrollOffset )

  return scrollWidth
  }



// getCursorX вычисляет X-координату курсора в пикселях (с учетом скролла)
func (in *SgItemInputLine) getCursorX() int {
  return in.getDisplaySymbolPos(in.cursorPos)
  }




// getVisibleText возвращает текст, который помещается в видимой области
func (in *SgItemInputLine) getVisibleText() (SgString, int) {
  if in.text.Length() == 0 {
    return NewSgString(), 0
    }

  // Получаем ширину доступной области для текста (ширина поля минус отступы)
  availableWidth := in.Width - 10 // 5 пикселей отступ с каждой стороны

  // Получаем текст от scrollOffset до конца
  textFromOffset := in.text.Mid( in.scrollOffset, -1 )

  // Определяем, сколько символов помещается в доступную ширину
  visibleChars := SgTextLimit(availableWidth, in.TextSize, textFromOffset, textFromOffset.Length() )

  if visibleChars <= 0 {
    return NewSgString(), 0
    }

  // Возвращаем видимую часть текста
  return in.text.Mid( in.scrollOffset, visibleChars ), visibleChars
  }




// adjustScrollOffset корректирует scrollOffset после изменений
func (in *SgItemInputLine) adjustScrollOffset() {
  if in.text.IsEmpty() {
    in.scrollOffset = 0
    return
    }

  // Получаем ширину доступной области
  availableWidth := in.Width - 10

  // Если курсор левее видимой области, скроллим влево
  if in.cursorPos < in.scrollOffset {
    in.scrollOffset = in.cursorPos
  } else {
    // Проверяем, не правее ли курсор видимой области
    cursorScreenX := in.getDisplaySymbolPos(in.cursorPos)

    if cursorScreenX >= availableWidth {
      // Курсор за правым краем, нужно скроллить
      // Находим минимальный scrollOffset, при котором курсор виден
      for in.scrollOffset < in.cursorPos {
        testCursorX := in.getDisplaySymbolPos(in.cursorPos)
        if testCursorX < availableWidth {
          break
          }
        in.scrollOffset++
      }
    }
  }

  // Дополнительно проверяем, что scrollOffset не выходит за пределы
  if in.scrollOffset < 0 {
    in.scrollOffset = 0
    }
  if in.scrollOffset >= in.text.Length() {
    in.scrollOffset = max(0, in.text.Length()-1)
    }
  }




func max(a, b int) int {
  if a > b {
    return a
    }
  return b
  }




// mouseClick обработка клика мыши
func (in *SgItemInputLine) MouseClick(localX, localY int) bool {
  // Проверяем, попадает ли мышь в область элемента
  localX -= in.PosX
  localY -= in.PosY

  if in.Visible && localX >= 0 && localX < in.Width && localY >= 0 && localY < in.Height {
    // Устанавливаем фокус
    SetFocus(in)

    if !in.text.IsEmpty() {
      clickX := localX - 5 // отступ

      // Учитываем скролл при определении позиции курсора
      // Сначала ищем среди видимых символов
      visibleText, visibleChars := in.getVisibleText()
      if visibleChars > 0 {
        for index := 0; index < visibleChars; index++ {
          // Вычисляем экранную позицию символа
          symbolPos := SgTextWidth( in.TextSize, visibleText[:index+1], index+1 )
          if symbolPos > clickX {
            in.cursorPos = in.scrollOffset + index
            in.adjustScrollOffset()
            SgWinRepaint()
            return true
            }
          }
        }

      // Если клик после последнего видимого символа, ставим курсор в конец видимой области
      in.cursorPos = in.scrollOffset + visibleChars
      if in.cursorPos > in.text.Length() {
        in.cursorPos = in.text.Length()
        }
     } else {
      in.cursorPos = 0
      }

    in.adjustScrollOffset()
    SgWinRepaint()
    return true
    }

  return false
  }




// keyDown обработка нажатий клавиш
func (in *SgItemInputLine) KeyDown(code int) {
  if in.Focus {
    switch code {
      case KeyLeft:
        if in.cursorPos > 0 {
          in.cursorPos--
          in.adjustScrollOffset()
          }

      case KeyRight:
        if in.cursorPos < in.text.Length() {
          in.cursorPos++
          in.adjustScrollOffset()
          }

      case KeyBackspace:
        if in.cursorPos > 0 && in.text.Length() > 0 {
          in.text.Remove( in.cursorPos-1, 1 )
          in.cursorPos--
          in.adjustScrollOffset()
          }

      case KeyDelete:
        if in.cursorPos < in.text.Length() {
          in.text.Remove( in.cursorPos, 1 )
          in.adjustScrollOffset()
          }

      case KeyHome:
        in.cursorPos = 0
        in.scrollOffset = 0

      case KeyEnd:
        in.cursorPos = in.text.Length()
        in.adjustScrollOffset()

      default:
        return
      }
    SgWinRepaint()
    }
  }




// keyChar обработка ввода символов
func (in *SgItemInputLine) KeyChar(code int) {
  if in.Focus {
    // Проверяем, что это печатный символ
    r := rune(code)
    if unicode.IsPrint(r) {
      // Вставляем символ в текущую позицию курсора
      in.text.InsertUChar( in.cursorPos, code )
      in.cursorPos++
      in.adjustScrollOffset()
      SgWinRepaint()
      }
    }
  }




// draw отрисовка элемента
func (in *SgItemInputLine) Draw(x, y int) {
  if in.Visible {
    absX := x + in.PosX
    absY := y + in.PosY

    // Фон
    SgDrawRoundRectangle( absX, absY, in.Width, in.Height, in.Radius, in.BgColor )

    // Рамка
    borderColor := in.BorderColor
    if in.Focus {
      borderColor = in.FocusColor
      }
    SgDrawRoundRect( absX, absY, in.Width, in.Height, in.Radius, borderColor )

    // Текст
    padding := 5
    textX := absX + padding
    textY := absY + in.Height / 2

    if in.text.Length() > 0 {
      // Получаем видимую часть текста
      visibleText, _ := in.getVisibleText()
      if !visibleText.IsEmpty() {
        //println( visibleText.ToUtf8() )
        SgDrawText( AlignLeft|AlignVCenter, textX, textY, in.TextSize, visibleText, in.TextColor )
        }
      }

    // Курсор
    if in.Focus {
      cursorX := in.getCursorX()
      // Рисуем курсор только если он в видимой области
      if cursorX >= 0 && cursorX < in.Width - padding*2 {
        SgDrawRectangle( textX+cursorX, textY-in.TextSize/2, 2, in.TextSize, in.TextColor )
        }
      }

    in.DrawChild(absX, absY)
    }
  }


 /*

// SelectAll выделяет весь текст
func (in *ItemInputLine) SelectAll() {
  in.selectionStart = 0
  in.selectionEnd = in.textLength()
  }

// ClearSelection снимает выделение
func (in *ItemInputLine) ClearSelection() {
  in.selectionStart = 0
  in.selectionEnd = 0
  }

// Copy копирует выделенный текст в буфер обмена
func (in *ItemInputLine) Copy() string {
  if in.selectionStart < in.selectionEnd {
    return in.textGet(in.selectionEnd)[in.selectionStart:in.selectionEnd]
    }
  return ""
  }

// Cut вырезает выделенный текст
func (in *ItemInputLine) Cut() string {
  if in.selectionStart < in.selectionEnd {
    selected := in.Copy()
    in.deleteRange(in.selectionStart, in.selectionEnd)
    in.cursorPos = in.selectionStart
    in.ClearSelection()
    return selected
    }
  return ""
  }

// Paste вставляет текст из буфера обмена
func (in *ItemInputLine) Paste(text string) {
  if in.selectionStart < in.selectionEnd {
    // Заменяем выделенное
    in.deleteRange(in.selectionStart, in.selectionEnd)
    in.cursorPos = in.selectionStart
    in.ClearSelection()
    }

  in.insertAt(in.cursorPos, text)
  in.cursorPos += len([]rune(text))
  }
*/
