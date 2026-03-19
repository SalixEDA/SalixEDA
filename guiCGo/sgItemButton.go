package main


// SgItemButton - button widget
type SgItemButton struct {
  SgItem
  Text         SgString
  TextSize     int
  TextColor    uint32
  BgColor      uint32
  BorderColor  uint32
  HoverColor   uint32
  PressedColor uint32
  Radius       int
  State        int // 0 - normal, 1 - hover, 2 - pressed
  }

const (
  ButtonStateNormal = iota
  ButtonStateHover
  ButtonStatePressed
  )


//!
//! \brief NewSgItemButton creates a new button widget
//! \param x            X coordinate of top-left corner
//! \param y            Y coordinate of top-left corner
//! \param w            Width of the button
//! \param h            Height of the button
//! \param text         UTF-8 text to display on the button
//! \return             Pointer to new SgItemButton instance
//!
//! Creates a button with default colors and 5px corner radius.
//! Default colors: Text=white, Background=dark gray, Border=gray,
//! Hover=lighter gray, Pressed=darker gray.
func NewSgItemButton(x, y, w, h int, text string) *SgItemButton {
  btn := &SgItemButton{
    SgItem:       *NewSgItem(x, y, w, h),
    Text:         SgStringFromUtf8(text),
    TextSize:     16,
    TextColor:    0xFFFFFF,
    BgColor:      0x404040,
    BorderColor:  0x808080,
    HoverColor:   0x505050,
    PressedColor: 0x303030,
    Radius:       5,
    State:        ButtonStateNormal,
    }

  return btn
  }


//!
//! \brief Draw renders the button widget at specified coordinates
//! \param x            Base X coordinate from parent
//! \param y            Base Y coordinate from parent
//!
//! Draws button with background color based on current state (normal, hover, pressed),
//! optional border, and centered text. Then recursively draws all child items.
func (b *SgItemButton) Draw(x, y int) {
  if !b.Visible {
    return
    }

  absX := x + b.PosX
  absY := y + b.PosY

  // Select background color based on button state
  var bgColor uint32
  switch b.State {
    case ButtonStateHover:
      bgColor = b.HoverColor
    case ButtonStatePressed:
      bgColor = b.PressedColor
    default:
      bgColor = b.BgColor
    }

  // Draw background with rounded corners
  SgDrawRoundRectangle( absX, absY, b.Width, b.Height, b.Radius, bgColor )

  // Draw border if color is specified
  if b.BorderColor != 0 {
    SgDrawRoundRect( absX, absY, b.Width, b.Height, b.Radius, b.BorderColor )
    }

  // Draw centered text if not empty
  if !b.Text.IsEmpty() {
    textX := absX + b.Width / 2
    textY := absY + b.Height / 2
    SgDrawText( AlignHCenter|AlignVCenter, textX, textY, b.TextSize, b.Text, b.TextColor )
    }

  b.DrawChild(absX, absY)
  }

