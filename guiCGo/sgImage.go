package main

import (
  "bytes"
  "errors"
  "fmt"
  "image"
  "image/jpeg"
  "image/png"
)


//!
//! \brief SgImage represents an image stored as an int32 array in ARGB format
//! \param Pixels Array of pixels in ARGB format (AARRGGBB, alpha is most significant)
//! \param Width Image width in pixels
//! \param Height Image height in pixels
//!
type SgImage struct {
  Pixels []uint32
  Width  int
  Height int
}


//!
//! \brief NewSgImage creates a new empty SgImage instance
//! \return Pointer to initialized SgImage with empty pixel array
//!
func NewSgImage() *SgImage {
  return &SgImage{
    Pixels: make([]uint32, 0),
    Width:  0,
    Height: 0,
  }
}




//!
//! \brief Load         loads an image from a resource path
//! \param resourcePath Path to the image resource
//! \return error       Returns nil on success, error message on failure
//! \note Supports two formats:
//!       - Regular OS files: "path/to/image.jpg"
//!       - Embedded files: "qrc:/images/photo.png" or "qrc:images/photo.png"
//!
func (img *SgImage) Load(resourcePath string) error {
  data, err := SgFileRead(resourcePath)
  if err != nil {
    return fmt.Errorf("failed to read resource: %w", err)
    }

  return img.ParseFromBytes(data)
  }




//!
//! \brief ParseFromBytes parses an image from a byte array (PNG or JPG format)
//! \param data           Byte array containing the image data
//! \return error         Returns nil on success, error message on failure
//! \note Format is automatically detected using magic numbers
//!
func (img *SgImage) ParseFromBytes(data []byte) error {
  if len(data) == 0 {
    return errors.New("empty byte array")
    }

  // Detect image format using magic numbers
  format := detectFormat(data)
  if format == "unknown" {
    return errors.New("unsupported or unknown image format")
    }

  // Decode image based on detected format
  var (
    srcImage image.Image
    err      error
    )

  switch format {
    case "png":
      srcImage, err = png.Decode(bytes.NewReader(data))
    case "jpg", "jpeg":
      srcImage, err = jpeg.Decode(bytes.NewReader(data))
    default:
      return fmt.Errorf("unsupported format: %s", format)
    }

  if err != nil {
    return fmt.Errorf("failed to decode %s: %w", format, err)
    }

  // Get image dimensions
  bounds := srcImage.Bounds()
  img.Width = bounds.Dx()
  img.Height = bounds.Dy()

  // Create pixel array
  img.Pixels = make([]uint32, img.Width*img.Height)

  // Fill pixel array in ARGB format
  idx := 0
  for y := bounds.Min.Y; y < bounds.Max.Y; y++ {
    for x := bounds.Min.X; x < bounds.Max.X; x++ {
      c := srcImage.At(x, y)
      r, g, b, a := c.RGBA()

      // Convert from 16-bit to 8-bit range and create ARGB
      argb := (uint32(a>>8) << 24) | (uint32(r>>8) << 16) | (uint32(g>>8) << 8) | uint32(b>>8)
      img.Pixels[idx] = argb
      idx++
      }
    }

  return nil
  }




// detectFormat determines image format using magic numbers
// Supports PNG and JPEG formats
func detectFormat(data []byte) string {
  if len(data) < 12 {
    return "unknown"
    }

  // PNG signature: 89 50 4E 47 0D 0A 1A 0A
  if len(data) >= 8 && bytes.Equal(data[:8], []byte{0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A}) {
    return "png"
    }

  // JPEG signature: SOI marker (FF D8)
  if len(data) >= 2 && data[0] == 0xFF && data[1] == 0xD8 {
    return "jpg"
    }

  return "unknown"
  }

