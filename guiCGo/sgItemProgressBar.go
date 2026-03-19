package main


// SgItemProgressBar - progress bar widget
type SgItemProgressBar struct {
  SgItem
  Value       float64 // 0.0 - 1.0
  BgColor     uint32
  FgColor     uint32
  BorderColor uint32
  Radius      int
  ShowText    bool
  TextSize    int
  TextColor   uint32
  }


//!
//! \brief NewSgItemProgressBar creates a new progress bar widget
//! \param x            X coordinate of top-left corner
//! \param y            Y coordinate of top-left corner
//! \param w            Width of the progress bar
//! \param h            Height of the progress bar
//! \return             Pointer to new SgItemProgressBar instance
//!
//! Creates a progress bar with default colors: dark gray background,
//! green foreground, gray border, and 5px corner radius.
//! Text display is enabled by default with 12px white text.
func NewSgItemProgressBar(x, y, w, h int) *SgItemProgressBar {
  pb := &SgItemProgressBar{
    SgItem:      *NewSgItem(x, y, w, h),
    Value:       0.0,
    BgColor:     0x404040,
    FgColor:     0x00A000,
    BorderColor: 0x808080,
    Radius:      5,
    ShowText:    true,
    TextSize:    12,
    TextColor:   0xFFFFFF,
    }

  return pb
  }


//!
//! \brief SetValue sets the progress bar value and triggers a repaint
//! \param value        Progress value between 0.0 and 1.0
//!
//! Clamps the input value to the valid range [0.0, 1.0] and updates
//! the progress bar. Automatically requests a window repaint.
func (p *SgItemProgressBar) SetValue(value float64) {
  if value < 0 {
    value = 0
    }
  if value > 1 {
    value = 1
    }
  p.Value = value
  SgWinRepaint()
  }




//!
//! \brief Draw renders the progress bar widget at specified coordinates
//! \param x            Base X coordinate from parent
//! \param y            Base Y coordinate from parent
//!
//! Draws background, border, filled progress portion based on current value,
//! and optional percentage text. Then recursively draws all child items.
func (p *SgItemProgressBar) Draw(x, y int) {
  if !p.Visible {
    return
    }

  absX := x + p.PosX
  absY := y + p.PosY

  // Draw background
  SgDrawRoundRectangle( absX, absY, p.Width, p.Height, p.Radius, p.BgColor )
  // Draw border
  SgDrawRoundRect( absX, absY, p.Width, p.Height, p.Radius, p.BorderColor )

  // Draw progress fill
  if p.Value > 0 {
    progressW := int(float64(p.Width-4) * p.Value)
    if progressW > 0 {
      SgDrawRoundRectangle( absX+2, absY+2, progressW, p.Height-4, p.Radius-1, p.FgColor )
      }
    }

  // Draw percentage text if enabled
  if p.ShowText {
    percent := int(p.Value * 100)
    text := itoa(percent) + "%"
    textX := absX + p.Width / 2
    textY := absY + p.Height / 2
    SgDrawText( AlignHCenter|AlignVCenter, textX, textY, p.TextSize, SgStringFromUtf8(text), p.TextColor )
    }

  p.DrawChild(absX, absY)
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

