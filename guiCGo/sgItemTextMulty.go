package main


// ItemTextMulty - многострочный текст с переносом по словам
type SgItemTextMulty struct {
  SgItem
  text        string      // исходный текст
  Size        int
  Align       int
  Color       uint32
  lines     []textLine  // разобранные строки
  lineHeight  int        // высота строки с интервалом
  }




// textLine - структура для хранения одной строки
type textLine struct {
  text  SgString
  x     int
  y     int
  width int
  }




// NewItemTextMulty - создание многострочного текста
func NewSgItemTextMulty(x, y, w int, text string, size int, color uint32) *SgItemTextMulty {
  it := &SgItemTextMulty{
    SgItem:  *NewSgItem(x, y, w, 0),
    text:     text,
    Size:     size,
    Align:    AlignTop | AlignLeft,
    Color:    color,
    lines:    []textLine{},
    lineHeight: size + size/4, // +25%
    }

  // Сразу разбираем текст на строки
  it.TextSet(text)

  return it
  }




// textSet - разбор текста на строки с переносом по словам
func (it *SgItemTextMulty) TextSet(str string) {
  it.text = str
  it.lines = []textLine{}

  if str == "" || it.Width <= 0 {
    it.Height = 0
    return
    }

  // Разбиваем на слова
  sgString := SgStringFromUtf8(str)
  words :=  sgString.SplitUChar( 0x20 )
  if len(words) == 0 {
    return
    }

  currentLine := NewSgString()
  currentWidth := 0

  for _, word := range words {
    // Измеряем слово
    wordWidth := SgTextWidth( it.Size, word, -1 )

    if currentWidth + wordWidth < it.Width {
      //This word filled into line
      currentLine.AppendSgString( word )
      currentWidth += wordWidth
      } else {
      //Word not filled into line
      it.addLine( currentLine, currentWidth )
      currentLine = word
      currentWidth = wordWidth
      }
    }

  // Добавляем последнюю строку
  if !currentLine.IsEmpty() {
    it.addLine( currentLine, currentWidth )
    }

  // Вычисляем общую высоту
  it.calcHeight()
  }





// addLine - добавление строки с учетом выравнивания
func (it *SgItemTextMulty) addLine(lineText SgString, lineWidth int) {
  x := it.calcX(lineWidth)
  y := (len(it.lines) + 1) * it.lineHeight

  it.lines = append(it.lines, textLine{
    text:  lineText,
    x:     x,
    y:     y,
    width: lineWidth,
    })
  }





// calcX - вычисление X координаты строки по горизонтальному выравниванию
func (it *SgItemTextMulty) calcX(lineWidth int) int {
  hAlign := it.Align & 0x03 // берем только горизонтальную часть (0,1,2)

  switch hAlign {
    case AlignLeft:
      return 0
    case AlignRight:
      return it.Width - lineWidth
    case AlignHCenter:
      return (it.Width - lineWidth) / 2
    default:
      return 0
    }
  }




// calcHeight - вычисление высоты и корректировка Y по вертикальному выравниванию
func (it *SgItemTextMulty) calcHeight() {
  if len(it.lines) == 0 {
    it.Height = 0
    return
    }

  // Общая высота блока текста
  textHeight := len(it.lines) * it.lineHeight
  vAlign := it.Align & 0x0C // берем только вертикальную часть (4,8)

  switch vAlign {
    case AlignTop:
      // По умолчанию строки идут сверху - ничего не меняем
      it.Height = textHeight

    case AlignVCenter:
      // Центрируем по вертикали относительно it.h
      if textHeight < it.Height {
        offset := (it.Height - textHeight) / 2
        for i := range it.lines {
          it.lines[i].y += offset
          }
        }

    case AlignBottom:
      // Прижимаем к низу
      if textHeight < it.Height {
        offset := it.Height - textHeight
        for i := range it.lines {
          it.lines[i].y += offset
          }
        }
    }
  }





// resize - обработка изменения размеров
func (it *SgItemTextMulty) Resize() {
  if !it.Visible {
    return
    }

  oldW := it.Width
  oldH := it.Height

  if it.OnResizeW != nil {
    it.OnResizeW(&it.SgItem)
    }
  if it.OnResizeH != nil {
    it.OnResizeH(&it.SgItem)
    }

  // Если ширина изменилась - перестраиваем строки заново
  if it.Width != oldW {
    it.TextSet(it.text)
  } else if it.Height != oldH {
    // Если изменилась только высота - пересчитываем вертикальное выравнивание
    it.calcHeight()
    }

  for _, child := range it.Child {
    child.Resize()
    }
  }




// draw - отрисовка многострочного текста
func (it *SgItemTextMulty) Draw(x int, y int) {
  if !it.Visible {
    return
    }

  absX := x + it.PosX
  absY := y + it.PosY

  // Рисуем все строки
  for _, line := range it.lines {
    SgDrawText( it.Align, absX+line.x, absY+line.y, it.Size, line.text, it.Color )
    }

  // Рисуем дочерние элементы
  it.DrawChild(absX, absY)
  }



// textGet - получение исходного текста
func (it *SgItemTextMulty) SgTextGet() string {
  return it.text
  }



// setAlign - установка выравнивания
func (it *SgItemTextMulty) SetAlign(align int) {
  if it.Align != align {
    it.Align = align
    // Пересчитываем позиции строк
    for i, line := range it.lines {
      it.lines[i].x = it.calcX(line.width)
      }
    it.calcHeight()
    SgWinRepaint()
    }
  }




