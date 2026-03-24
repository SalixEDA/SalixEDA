package main

type SgItemModal struct {
  SgItemRect
  BorderColor uint32
  Result      chan int
  }



//!
//! \brief NewSgItemRect creates a new rectangle widget
//! \param x            X coordinate of top-left corner
//! \param y            Y coordinate of top-left corner
//! \param w            Width of the rectangle
//! \param h            Height of the rectangle
//! \param color        Fill color value (e.g., 0xRRGGBB or 0xAARRGGBB)
//! \return             Pointer to new SgItemRect instance
//!
//! Creates a rectangle widget with default radius 0 (no rounding).
func NewSgItemModal(x, y, w, h, r int, color uint32, borderColor uint32) *SgItemModal {
  im := &SgItemModal{
    SgItemRect:  *NewSgItemRect(x, y, w, h, color),
    BorderColor:  borderColor,
    Result:       make(chan int),
    }
  im.Radius = r
  im.Visible = false

  return im
  }



//!
//! \brief Draw renders the rectangle widget at specified coordinates
//! \param x            Base X coordinate from parent
//! \param y            Base Y coordinate from parent
//!
//! Draws a filled rectangle with optional rounded corners (if Radius > 0),
//! then recursively draws all child items.
func (im *SgItemModal) Draw(x int, y int) {
  // Check visibility - access through im
  if !im.Visible {
    return
    }

  // Calculate absolute coordinates including parent offsets
  absX := x + im.PosX
  absY := y + im.PosY

  // Draw the rectangle with current dimensions, radius and color
  SgDrawRoundRectangle( absX, absY, im.Width, im.Height, im.Radius, im.Color )

  //Draw border
  SgDrawRoundRect( absX, absY, im.Width, im.Height, im.Radius, im.BorderColor )

  // Draw all child elements recursively
  im.DrawChild( absX, absY )
  }


func (i *SgItemModal) IsHit(localX int, localY int) bool {
  // Check if mouse is within item's area
  localX -= i.PosX
  localY -= i.PosY
  if i.Visible && localX >= 0 && localX < i.Width && localY >= 0 && localY < i.Height {
    // Mouse inside item
    for j := len(i.Child) - 1; j >= 0; j-- {
      if i.Child[j].IsHit( localX, localY ) {
        return true
        }
      }
    return true
    }
  return false
  }



func (i *SgItemModal) MouseClick(localX int, localY int) bool {
  // Check if mouse is within item's area
  localX -= i.PosX
  localY -= i.PosY
  if i.Visible && localX >= 0 && localX < i.Width && localY >= 0 && localY < i.Height {
    // Mouse inside item
    for j := len(i.Child) - 1; j >= 0; j-- {
      if i.Child[j].MouseClick( localX, localY ) {
        return true
        }
      }
    }
  if i.Visible {
    return true
    }
  return false
  }


func (i *SgItemModal) Exec() int {
  i.Visible = true
  SgWinRepaint()
  res := <- i.Result
  i.Visible = false
  SgWinRepaint()
  return res
  }


func (i *SgItemModal) Done( res int ) {
  i.Result <- res
  }
