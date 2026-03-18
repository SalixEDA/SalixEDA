package main

/*
#include "guiGraphics.h"
*/
import "C"

import (
  "unicode"
  "unicode/utf16"
  )


// Константы для выравнивания текста
const (
  //Горизонтальное выравнивание
  AlignLeft    = 0
  AlignHCenter = 1
  AlignRight   = 2

  //Вертикальное выравнивание
  AlignBottom  = 0
  AlignVCenter = 4
  AlignTop     = 8
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
  //println("draw x:", absX, "y:", absY, "w:", ir.w, "h:", ir.h)
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
    textY := absY + b.h/2
    goDrawText(C.int(AlignHCenter|AlignVCenter), C.int(textX), C.int(textY), C.int(b.textSize), b.text, b.textColor)
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
    radius:      5,
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
    textY := absY + p.h/2
    goDrawText(C.int(AlignHCenter|AlignVCenter), C.int(textX), C.int(textY), C.int(p.textSize), text, p.textColor)
    }

  p.drawChild(absX, absY)
  }














// ItemInputLine - строчный редактор с UTF-16 внутренним представлением
type ItemInputLine struct {
  Item
  textUTF16     []uint16  // Внутреннее представление в UTF-16
  cursorPos     int       // Позиция курсора в символах UTF-16
  scrollOffset  int       // Индекс первого отображаемого символа
  selectionStart int
  selectionEnd   int
  maxLength     int
  textSize      int
  textColor     C.uint32_t
  bgColor       C.uint32_t
  borderColor   C.uint32_t
  focusColor    C.uint32_t
  radius        int
}


// NewItemInputLine создает новый элемент ввода
func NewItemInputLine(x, y, w, h int) *ItemInputLine {
  in := &ItemInputLine{
    Item:      *NewItem(x, y, w, h),
    textUTF16: make([]uint16, 0),
    cursorPos: 0,
    scrollOffset: 0,
    selectionStart: 0,
    selectionEnd: 0,
    maxLength: 256,
    textSize:  16,
    textColor: 0xFFFFFF,
    bgColor:   0x202020,
    borderColor: 0x808080,
    focusColor:  0x00A0FF,
    radius:    3,
    }

  return in
  }





// textGet возвращает строку Go из внутреннего UTF-16 представления
// len - количество символов UTF-16, которые нужно получить (не байт!)
// При len == -1 возвращается вся строка
func (in *ItemInputLine) textGet(length int) string {
  if in.textUTF16 == nil {
    return ""
  }

  if length == -1 || length >= len(in.textUTF16) {
    // Возвращаем всю строку
    return string(utf16.Decode(in.textUTF16))
  }

  if length <= 0 {
    return ""
  }

  // Возвращаем только первые length символов
  return string(utf16.Decode(in.textUTF16[:length]))
}


func (in *ItemInputLine) textGetMid( firstInclude int, lastExclude int ) string {
  if lastExclude < 0 {
    return string( utf16.Decode(in.textUTF16[firstInclude:]) )
    }
  return string( utf16.Decode(in.textUTF16[firstInclude:lastExclude]) )
  }





// setText устанавливает текст из строки Go
func (in *ItemInputLine) textSet(text string) {
  // Конвертируем UTF-8 в UTF-16
  utf16Data := utf16.Encode([]rune(text))

  // Ограничиваем длину
  if len(utf16Data) > in.maxLength {
    utf16Data = utf16Data[:in.maxLength]
  }

  in.textUTF16 = utf16Data

  // Корректируем позицию курсора
  if in.cursorPos > len(in.textUTF16) {
    in.cursorPos = len(in.textUTF16)
  }

  // Корректируем скролл
  in.adjustScrollOffset()

  C.winRepaint()
}





// textLength возвращает длину текста в символах UTF-16
func (in *ItemInputLine) textLength() int {
  return len(in.textUTF16)
}




// insertAt вставляет строку в указанную позицию
func (in *ItemInputLine) insertAt(pos int, s string) {
  if pos < 0 || pos > len(in.textUTF16) {
    return
  }

  // Конвертируем вставляемый текст в UTF-16
  insertRunes := []rune(s)
  insertUTF16 := utf16.Encode(insertRunes)

  // Проверяем ограничение длины
  newLength := len(in.textUTF16) + len(insertUTF16)
  if newLength > in.maxLength {
    // Обрезаем вставляемый текст
    available := in.maxLength - len(in.textUTF16)
    if available <= 0 {
      return
    }
    if available < len(insertUTF16) {
      insertUTF16 = insertUTF16[:available]
    }
  }

  // Вставляем
  newText := make([]uint16, len(in.textUTF16)+len(insertUTF16))
  copy(newText, in.textUTF16[:pos])
  copy(newText[pos:], insertUTF16)
  copy(newText[pos+len(insertUTF16):], in.textUTF16[pos:])

  in.textUTF16 = newText
}




// deleteRange удаляет диапазон символов
func (in *ItemInputLine) deleteRange(start, end int) {
  if start < 0 || end > len(in.textUTF16) || start >= end {
    return
  }

  in.textUTF16 = append(in.textUTF16[:start], in.textUTF16[end:]...)
}




// getSymbolPos вычисляет позицию символа с индексом index в пикселях
// (относительно начала текста, без учета скролла)
func (in *ItemInputLine) getSymbolPos(index int) int {
  if index <= 0 {
    return 0
  }

  if index >= len(in.textUTF16) {
    index = len(in.textUTF16)
  }

  // Получаем текст до индекса
  beforeCursor := in.textGet(index)

  // Используем C-функцию для измерения ширины текста
  return goTextWidth(in.textSize, beforeCursor, index)
}





// getDisplaySymbolPos вычисляет позицию символа с учетом скролла
func (in *ItemInputLine) getDisplaySymbolPos(index int) int {
  if index <= 0 {
    return 0
    }

  // Получаем ширину текста от scrollOffset до index
  text := in.textGet(index)
  scrollText := in.textGet(in.scrollOffset)

  // Измеряем ширину текста до index и вычитаем ширину скроллированной части
  fullWidth := goTextWidth(in.textSize, text, index)
  scrollWidth := goTextWidth(in.textSize, scrollText, in.scrollOffset)

  return fullWidth - scrollWidth
}



// getCursorX вычисляет X-координату курсора в пикселях (с учетом скролла)
func (in *ItemInputLine) getCursorX() int {
  return in.getDisplaySymbolPos(in.cursorPos)
}




// getVisibleText возвращает текст, который помещается в видимой области
func (in *ItemInputLine) getVisibleText() (string, int) {
  if in.textLength() == 0 {
    return "", 0
  }

  // Получаем ширину доступной области для текста (ширина поля минус отступы)
  availableWidth := in.w - 10 // 5 пикселей отступ с каждой стороны

  // Получаем текст от scrollOffset до конца
  textFromOffset := in.textGetMid( in.scrollOffset, -1 )

  // Определяем, сколько символов помещается в доступную ширину
  visibleChars := goTextLimit(availableWidth, in.textSize, textFromOffset, -1)

  if visibleChars <= 0 {
    return "", 0
  }

  // Возвращаем видимую часть текста
  return in.textGetMid( in.scrollOffset, in.scrollOffset + visibleChars ), visibleChars
}




// adjustScrollOffset корректирует scrollOffset после изменений
func (in *ItemInputLine) adjustScrollOffset() {
  if in.textLength() == 0 {
    in.scrollOffset = 0
    return
  }

  // Получаем ширину доступной области
  availableWidth := in.w - 10

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
  if in.scrollOffset >= in.textLength() {
    in.scrollOffset = max(0, in.textLength()-1)
  }
}




func max(a, b int) int {
  if a > b {
    return a
  }
  return b
}




// mouseClick обработка клика мыши
func (in *ItemInputLine) mouseClick(localX, localY int) bool {
  // Проверяем, попадает ли мышь в область элемента
  localX -= in.x
  localY -= in.y

  if in.visible && localX >= 0 && localX < in.w && localY >= 0 && localY < in.h {
    // Устанавливаем фокус
    setFocus(in)

    if len(in.textUTF16) > 0 {
      clickX := localX - 5 // отступ

      // Учитываем скролл при определении позиции курсора
      // Сначала ищем среди видимых символов
      visibleText, visibleChars := in.getVisibleText()
      if visibleChars > 0 {
        for index := 0; index < visibleChars; index++ {
          // Вычисляем экранную позицию символа
          symbolPos := goTextWidth(in.textSize, visibleText[:index+1], index+1)
          if symbolPos > clickX {
            in.cursorPos = in.scrollOffset + index
            in.adjustScrollOffset()
            C.winRepaint()
            return true
          }
        }
      }

      // Если клик после последнего видимого символа, ставим курсор в конец видимой области
      in.cursorPos = in.scrollOffset + visibleChars
      if in.cursorPos > in.textLength() {
        in.cursorPos = in.textLength()
      }
    } else {
      in.cursorPos = 0
    }

    in.adjustScrollOffset()
    C.winRepaint()
    return true
  }

  return false
}




// keyDown обработка нажатий клавиш
func (in *ItemInputLine) keyDown(code int) {
  if in.focus {
    switch code {
    case KeyLeft:
      if in.cursorPos > 0 {
        in.cursorPos--
        in.adjustScrollOffset()
      }

    case KeyRight:
      if in.cursorPos < in.textLength() {
        in.cursorPos++
        in.adjustScrollOffset()
      }

    case KeyBackspace:
      if in.cursorPos > 0 && in.textLength() > 0 {
        in.deleteRange(in.cursorPos-1, in.cursorPos)
        in.cursorPos--
        in.adjustScrollOffset()
      }

    case KeyDelete:
      if in.cursorPos < in.textLength() {
        in.deleteRange(in.cursorPos, in.cursorPos+1)
        in.adjustScrollOffset()
      }

    case KeyHome:
      in.cursorPos = 0
      in.scrollOffset = 0

    case KeyEnd:
      in.cursorPos = in.textLength()
      in.adjustScrollOffset()

    default:
      return
    }
    C.winRepaint()
  }
}




// keyChar обработка ввода символов
func (in *ItemInputLine) keyChar(code int) {
  if in.focus {
    // Проверяем, что это печатный символ
    r := rune(code)
    if unicode.IsPrint(r) && in.textLength() < in.maxLength {
      // Вставляем символ в текущую позицию курсора
      in.insertAt(in.cursorPos, string(r))
      in.cursorPos++
      in.adjustScrollOffset()
      C.winRepaint()
    }
  }
}




// draw отрисовка элемента
func (in *ItemInputLine) draw(x, y int) {
  if in.visible {
    absX := x + in.x
    absY := y + in.y

    // Фон
    C.drawRoundRectangle(C.int(absX), C.int(absY), C.int(in.w), C.int(in.h), C.int(in.radius), in.bgColor)

    // Рамка
    borderColor := in.borderColor
    if in.focus {
      borderColor = in.focusColor
    }
    C.drawRoundRect(C.int(absX), C.int(absY), C.int(in.w), C.int(in.h), C.int(in.radius), borderColor)

    // Текст
    padding := 5
    textX := absX + padding
    textY := absY + in.h/2

    if in.textLength() > 0 {
      // Получаем видимую часть текста
      visibleText, _ := in.getVisibleText()
      if visibleText != "" {
        goDrawText(C.int(AlignLeft|AlignVCenter), C.int(textX), C.int(textY),
          C.int(in.textSize), visibleText, in.textColor)
      }
    }

    // Курсор
    if in.focus {
      cursorX := in.getCursorX()
      // Рисуем курсор только если он в видимой области
      if cursorX >= 0 && cursorX < in.w-padding*2 {
        C.drawRectangle(C.int(textX+cursorX), C.int(textY-in.textSize/2),
          C.int(2), C.int(in.textSize), in.textColor)
      }
    }

    in.drawChild(absX, absY)
  }
}




// Дополнительные полезные методы

// SetMaxLength устанавливает максимальную длину текста
func (in *ItemInputLine) SetMaxLength(max int) {
  in.maxLength = max
  if len(in.textUTF16) > max {
    in.textUTF16 = in.textUTF16[:max]
    if in.cursorPos > max {
      in.cursorPos = max
      }
    }
  }



// GetText возвращает текущий текст
func (in *ItemInputLine) GetText() string {
  return in.textGet(-1)
  }

// SetText устанавливает текст
func (in *ItemInputLine) SetText(text string) {
  in.textSet(text)
  }

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












// ItemTextMulty - многострочный текст с переносом по словам
type ItemTextMulty struct {
  Item
  text      string      // исходный текст
  size      int
  align     int
  color     C.uint32_t
  lines     []textLine  // разобранные строки
  lineHeight int        // высота строки с интервалом
  }




// textLine - структура для хранения одной строки
type textLine struct {
  text string
  x    int
  y    int
  width int
  }




// NewItemTextMulty - создание многострочного текста
func NewItemTextMulty(x, y, w int, text string, size int, color C.uint32_t) *ItemTextMulty {
  it := &ItemTextMulty{
    Item:     *NewItem(x, y, w, 0),
    text:     text,
    size:     size,
    align:    AlignTop | AlignLeft,
    color:    color,
    lines:    []textLine{},
    lineHeight: size + size/4, // +25%
    }

  // Сразу разбираем текст на строки
  it.textSet(text)

  return it
  }




// textSet - разбор текста на строки с переносом по словам
func (it *ItemTextMulty) textSet(str string) {
  it.text = str
  it.lines = []textLine{}

  if str == "" || it.w <= 0 {
    it.h = 0
    return
    }

  // Разбиваем на слова
  words := splitIntoWords(str)
  if len(words) == 0 {
    return
    }

  currentLine := ""
  currentWidth := 0

  for _, word := range words {
    // Измеряем слово
    wordWidth := goTextWidth(it.size, word, -1)

    // Для первого слова в строке добавляем без пробела
    if currentLine == "" {
      currentLine = word
      currentWidth = wordWidth
      continue
      }

    // Пробуем добавить слово с пробелом
    testLine := currentLine + " " + word
    testWidth := goTextWidth(it.size, testLine, -1)

    if testWidth <= it.w {
      // Помещается
      currentLine = testLine
      currentWidth = testWidth
    } else {
      // Не помещается - сохраняем текущую строку и начинаем новую
      it.addLine(currentLine, currentWidth)
      currentLine = word
      currentWidth = wordWidth
      }
    }

  // Добавляем последнюю строку
  if currentLine != "" {
    it.addLine(currentLine, currentWidth)
    }

  // Вычисляем общую высоту
  it.calcHeight()
  }





// addLine - добавление строки с учетом выравнивания
func (it *ItemTextMulty) addLine(lineText string, lineWidth int) {
  x := it.calcX(lineWidth)
  y := len(it.lines) * it.lineHeight

  it.lines = append(it.lines, textLine{
    text:  lineText,
    x:     x,
    y:     y,
    width: lineWidth,
    })
  }





// calcX - вычисление X координаты строки по горизонтальному выравниванию
func (it *ItemTextMulty) calcX(lineWidth int) int {
  hAlign := it.align & 0x03 // берем только горизонтальную часть (0,1,2)

  switch hAlign {
    case AlignLeft:
      return 0
    case AlignRight:
      return it.w - lineWidth
    case AlignHCenter:
      return (it.w - lineWidth) / 2
    default:
      return 0
    }
  }




// calcHeight - вычисление высоты и корректировка Y по вертикальному выравниванию
func (it *ItemTextMulty) calcHeight() {
  if len(it.lines) == 0 {
    it.h = 0
    return
    }

  // Общая высота блока текста
  textHeight := len(it.lines) * it.lineHeight
  vAlign := it.align & 0x0C // берем только вертикальную часть (4,8)

  switch vAlign {
    case AlignTop:
      // По умолчанию строки идут сверху - ничего не меняем
      it.h = textHeight

    case AlignVCenter:
      // Центрируем по вертикали относительно it.h
      if textHeight < it.h {
        offset := (it.h - textHeight) / 2
        for i := range it.lines {
          it.lines[i].y += offset
          }
        }

    case AlignBottom:
      // Прижимаем к низу
      if textHeight < it.h {
        offset := it.h - textHeight
        for i := range it.lines {
          it.lines[i].y += offset
          }
        }
    }
  }





// resize - обработка изменения размеров
func (it *ItemTextMulty) resize() {
  if !it.visible {
    return
    }

  oldW := it.w
  oldH := it.h

  if it.onResizeW != nil {
    it.onResizeW(&it.Item)
    }
  if it.onResizeH != nil {
    it.onResizeH(&it.Item)
    }

  // Если ширина изменилась - перестраиваем строки заново
  if it.w != oldW {
    it.textSet(it.text)
  } else if it.h != oldH {
    // Если изменилась только высота - пересчитываем вертикальное выравнивание
    it.calcHeight()
    }

  for _, child := range it.child {
    child.resize()
    }
  }




// draw - отрисовка многострочного текста
func (it *ItemTextMulty) draw(x int, y int) {
  if !it.visible {
    return
    }

  absX := x + it.x
  absY := y + it.y

  // Рисуем все строки
  for _, line := range it.lines {
    goDrawText(
      C.int(it.align),           // align
      C.int(absX+line.x),        // x
      C.int(absY+line.y),        // y
      C.int(it.size),            // size
      line.text,                 // text
      it.color,                  // color
      )
    }

  // Рисуем дочерние элементы
  it.drawChild(absX, absY)
  }



// textGet - получение исходного текста
func (it *ItemTextMulty) textGet() string {
  return it.text
  }



// setAlign - установка выравнивания
func (it *ItemTextMulty) setAlign(align int) {
  if it.align != align {
    it.align = align
    // Пересчитываем позиции строк
    for i, line := range it.lines {
      it.lines[i].x = it.calcX(line.width)
      }
    it.calcHeight()
    }
  }




// splitIntoWords - вспомогательная функция разбивки на слова
func splitIntoWords(text string) []string {
  words := []string{}
  currentWord := ""

  for _, r := range text {
    if r == ' ' || r == '\t' || r == '\n' {
      if currentWord != "" {
        words = append(words, currentWord)
        currentWord = ""
        }
     } else {
      currentWord += string(r)
      }
    }

  if currentWord != "" {
    words = append(words, currentWord)
    }

  return words
  }




// ItemTextMulty - многострочный текст с переносом по словам
type ItemContainer struct {
  Item
  }



func NewItemContainer(x, y, w, h int) *ItemContainer {
  return &ItemContainer{
    Item:  *NewItem(x, y, w, h),
    }
  }




func (i *ItemContainer) add(items ...ItemInterface) {
  // Проходим по всем
  for _, item := range items {
    item.setParent( i )
    i.child = append(i.child, item)
    item.anchorFillDef( i )
    if len(i.child) == 1 {
      item.setVisible( true )
     } else {
      item.setVisible( false )
      }
    }
  }



func (i *ItemContainer) setCurrent( index int ) {
  //Scan all childs and switch off their visible flag except only with equal index
  for _, child := range i.child {
    child.setVisible( index == 0 )
    index--
    }
  }




