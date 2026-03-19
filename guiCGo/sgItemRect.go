package main


// SgItemRect - rectangle widget
type SgItemRect struct {
  SgItem
  Radius  int
  Color   uint32
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
func NewSgItemRect(x, y, w, h int, color uint32) *SgItemRect {
  ir := &SgItemRect{
    SgItem: *NewSgItem(x, y, w, h),
    Radius:  0,
    Color:   color,
    }

  return ir
  }




//!
//! \brief Draw renders the rectangle widget at specified coordinates
//! \param x            Base X coordinate from parent
//! \param y            Base Y coordinate from parent
//!
//! Draws a filled rectangle with optional rounded corners (if Radius > 0),
//! then recursively draws all child items.
func (ir *SgItemRect) Draw(x int, y int) {
  // Check visibility - access through ir
  if !ir.Visible {
    return
    }

  // Calculate absolute coordinates including parent offsets
  absX := x + ir.PosX
  absY := y + ir.PosY

  // Draw the rectangle with current dimensions, radius and color
  SgDrawRoundRectangle( absX, absY, ir.Width, ir.Height, ir.Radius, ir.Color )

  // Draw all child elements recursively
  ir.DrawChild( absX, absY )
  }
