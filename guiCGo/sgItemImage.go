package main

// SgItemImage - image widget
type SgItemImage struct {
  SgItem
  image SgImage
  }



//!
//! \brief NewSgItemImage creates a new rectangle widget
//! \param x            X coordinate of top-left corner
//! \param y            Y coordinate of top-left corner
//! \param w            Width of the rectangle
//! \param h            Height of the rectangle
//! \param imagePath    Path to image file
//! \return             Pointer to new SgItemImage instance
//!
func NewSgItemImage(x, y int, imagePath string) *SgItemImage {
  im := &SgItemImage{
    SgItem: *NewSgItem(x, y, 0, 0),
    image: *NewSgImage(),
    }
  im.image.Load( imagePath )
  im.Width = im.image.Width
  im.Height = im.image.Height
  return im
  }




//!
//! \brief Draw renders the image widget at specified coordinates
//! \param x            Base X coordinate from parent
//! \param y            Base Y coordinate from parent
//!
func (im *SgItemImage) Draw(x int, y int) {
  // Check visibility - access through ir
  if !im.Visible {
    return
    }

  // Calculate absolute coordinates including parent offsets
  absX := x + im.PosX
  absY := y + im.PosY

  // Draw the rectangle with current dimensions, radius and color
  SgDrawImage( absX, absY, im.Width, im.Height, im.image.Pixels, im.image.Width, im.image.Height )

  // Draw all child elements recursively
  im.DrawChild( absX, absY )
  }


func (im *SgItemImage) Resize() {
  if !im.Visible {
    return
    }
  im.Width = im.image.Width
  im.Height = im.image.Height
  im.ResizeBind()
  im.Width = im.image.Width
  im.Height = im.image.Height
  im.ResizeChild()
  }
