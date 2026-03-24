package main

// Text alignment constants
const (
  // Horizontal alignment
  AlignLeft    = 0
  AlignHCenter = 1
  AlignRight   = 2

  // Vertical alignment
  AlignBottom  = 0
  AlignVCenter = 4
  AlignTop     = 8
)




// SgItemText - text widget
type SgItemText struct {
  SgItem
  Text  SgString
  Size  int
  Align int
  Color uint32
  }



//!
//! \brief NewSgItemText creates a new text widget
//! \param x            X coordinate of text position (affected by alignment)
//! \param y            Y coordinate of text position (affected by alignment)
//! \param text         UTF-8 string to display
//! \param size         Font size in points or pixels
//! \param color        Text color value (e.g., 0xRRGGBB or 0xAARRGGBB)
//! \return             Pointer to new SgItemText instance
//!
//! Creates a text widget with default alignment (AlignLeft|AlignBottom).
//! Width and height are initially set to 0 and should be calculated based on text metrics.
func NewSgItemText(x, y int, text string, size int, color uint32) *SgItemText {
  it := &SgItemText{
    SgItem: *NewSgItem(x, y, 0, size), // Size will be calculated later
    Text:   SgStringFromUtf8(text),
    Size:   size,
    Align:  AlignLeft | AlignBottom, // Default alignment
    Color:  color,
    }

  return it
  }


func (it *SgItemText) AlignSet( align int ) {
  it.Align = align
  }


func (it *SgItemText) TextSet( str string ) {
  it.Text = SgStringFromUtf8(str)
  }



//!
//! \brief Draw renders the text widget at specified coordinates
//! \param x            Base X coordinate from parent
//! \param y            Base Y coordinate from parent
//!
//! Draws text with specified alignment, font size and color,
//! then recursively draws all child items.
func (it *SgItemText) Draw(x int, y int) {
  // Check visibility - access through it
  if !it.Visible {
    return
    }

  // Calculate absolute coordinates including parent offsets
  absX := x + it.PosX
  absY := y + it.PosY

  vAlign := it.Align & 0x0C // берем только вертикальную часть (4,8)

  switch vAlign {
    case AlignTop:
      // По умолчанию строки идут сверху - ничего не меняем

    case AlignVCenter:
      // Центрируем по вертикали относительно it.h
      absY += it.Height / 2

    case AlignBottom:
      // Прижимаем к низу
      absY += it.Height
    }


  // Draw text with current settings
  if it.Text.Length() > 0 {
    SgDrawText( it.Align, absX, absY, it.Size, it.Text, it.Color )
    }

  // Draw all child elements recursively
  it.DrawChild( absX, absY )
  }
