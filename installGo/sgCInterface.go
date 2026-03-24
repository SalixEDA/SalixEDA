package main


/*
#cgo linux LDFLAGS: -L/usr/lib/X11 -lX11 -lm
#include "sgCGraphics.h"
#include "stdlib.h"
*/
import "C"

import (
  "unsafe"
  )




// Коды клавиш (можно расширить)
type KeyCode int

const (
  KeyNone KeyCode = iota
  KeyBackspace = 0x0e
  KeyEnter     = 0x1c
  KeyEscape    = 0x01
  KeyTab       = 0x0f
  KeyLeft      = 0x4b
  KeyRight     = 0x4d
  KeyUp        = 0x48
  KeyPgUp      = 0x49
  KeyDown      = 0x50
  KeyPgDn      = 0x51
  KeyHome      = 0x47
  KeyEnd       = 0x4f
  KeyDelete    = 0x53

)




//===============================================
//  Main window functions

func SgWinStart( posx int, posy int, width int, height int, title string ) {
  // Преобразуем Go строку в C строку
  cTitle := C.CString(title)

  // Освобождаем память после использования
  defer C.free(unsafe.Pointer(cTitle))

  // Вызываем C-функцию с преобразованными параметрами
  C.winStart( C.int(posx), C.int(posy), C.int(width), C.int(height), cTitle )
  }



func SgWinRepaint() {
  C.winRepaint()
  }


func SgWinClose() {
  C.winClose()
  }







//===============================================
//  Drawing functions

//!
//! \brief SgDrawText calls C.drawText to render UTF-8 text at specified position with given color and alignment
//! \param align        Alignment flags combination (horizontal and vertical AlignXXX flags)
//! \param x            X coordinate for text position
//! \param y            Y coordinate for text position
//! \param size         Font size in points or pixels
//! \param str          UNICODE string to render
//! \param color        Color value (e.g., 0xRRGGBB or 0xAARRGGBB)
//!
//! \note Memory is managed by Go and automatically freed after function returns.
//!
func SgDrawText(align int, x int, y int, size int, str SgString, color uint32) {
  // Call C function with pointer to first rune
  // C function receives runes as int array (Unicode code points)
  C.drawText( C.int(align), C.int(x), C.int(y), C.int(size), (*C.int)(unsafe.Pointer(&str[0])), C.int(str.Length()), C.uint32_t(color) )
  // C function executes and blocks
  // runes slice goes out of scope and becomes eligible for GC
  }



//!
//! \brief SgTextWidth calls C.textWidth to calculate text width in pixels
//! \param size         Font size in points or pixels
//! \param str          UNICODE string to measure
//! \param len          Length in Unicode characters, if len < 0 then full string
//! \return             Text width in pixels, or 0 if string is empty
//!
//! \note Memory is managed by Go and automatically freed after function returns.
//!
func SgTextWidth(size int, str SgString, len int) int {
  // Handle empty string case
  if len == 0 || str.Length() == 0 {
    return 0
    }

  if len < 0 {
    len = str.Length()
    }

  // Call C function to calculate width
  return int(C.textWidth( C.int(size), (*C.int)(unsafe.Pointer(&str[0])), C.int(len) ) )
  }



//!
//! \brief SgTextLimit calls C.textLimit to calculate how many Unicode16 characters fit within given width
//! \param width        Maximum width in pixels
//! \param size         Font size in points or pixels
//! \param str          UNICODE string to measure
//! \param len          Length in Unicode characters (should match string length)
//! \return             Number of characters that fit within width, or 0 if string is empty
//!
//! \note  Memory is managed by Go and automatically freed after function returns.
//!
func SgTextLimit(width int, size int, str SgString, len int) int {
  // Handle empty string case
  if len == 0 || str.Length() == 0 {
    return 0
    }

  if len < 0 {
    len = str.Length()
    }

  // Call C function to calculate character limit
  return int(C.textLimit( C.int(width), C.int(size), (*C.int)(unsafe.Pointer(&str[0])), C.int(len) ))
  }



//!
//! \brief SgDrawHLine2 calls C.drawHLine2 which draw two horizontal lines from x1 to x2
//!                     at yTop and yBottom
//! \param x1           x1 coordinate starting for two lines
//! \param x2           x2 coordinate ending for two lines
//! \param yTop         y position of line 1
//! \param yBottom      y position of line 2
//! \param color        Color value (e.g., 0xRRGGBB or 0xAARRGGBB)
//!
func SgDrawHLine2( x1 int, x2 int, yTop int, yBottom int, color uint32 ) {
  C.drawHLine2( C.int(x1), C.int(x2), C.int(yTop), C.int(yBottom), C.uint32_t(color) )
  }



//!
//! \brief SgDrawVLine2 calls C.drawVLine2 which draws two vertical lines from y1 to y2 at positions xl and xr
//! \param xl           Left X coordinate for vertical line
//! \param xr           Right X coordinate for vertical line
//! \param y1           Starting Y coordinate for both lines
//! \param y2           Ending Y coordinate for both lines
//! \param color        Color value (e.g., 0xRRGGBB or 0xAARRGGBB)
//!
func SgDrawVLine2(xl int, xr int, y1 int, y2 int, color uint32) {
  C.drawVLine2(C.int(xl), C.int(xr), C.int(y1), C.int(y2), C.uint32_t(color))
  }



//!
//! \brief SgDrawRectangle calls C.drawRectangle which draws a filled rectangle
//! \param x            X coordinate of top-left corner
//! \param y            Y coordinate of top-left corner
//! \param w            Width of the rectangle
//! \param h            Height of the rectangle
//! \param color        Color value (e.g., 0xRRGGBB or 0xAARRGGBB)
//!
func SgDrawRectangle(x int, y int, w int, h int, color uint32) {
  C.drawRectangle(C.int(x), C.int(y), C.int(w), C.int(h), C.uint32_t(color))
  }




//!
//! \brief SgDrawRect calls C.drawRect which draws a 1-pixel wide rectangle outline
//! \param x            X coordinate of top-left corner
//! \param y            Y coordinate of top-left corner
//! \param w            Width of the rectangle
//! \param h            Height of the rectangle
//! \param color        Color value (e.g., 0xRRGGBB or 0xAARRGGBB)
//!
func SgDrawRect(x int, y int, w int, h int, color uint32) {
  C.drawRect(C.int(x), C.int(y), C.int(w), C.int(h), C.uint32_t(color))
  }



//!
//! \brief SgDrawRoundRectangle calls C.drawRoundRectangle which draws a filled rectangle with rounded corners
//! \param px           X coordinate of top-left corner
//! \param py           Y coordinate of top-left corner
//! \param w            Width of the rectangle
//! \param h            Height of the rectangle
//! \param r            Corner radius in pixels
//! \param color        Color value (e.g., 0xRRGGBB or 0xAARRGGBB)
//!
func SgDrawRoundRectangle(px int, py int, w int, h int, r int, color uint32) {
  C.drawRoundRectangle(C.int(px), C.int(py), C.int(w), C.int(h), C.int(r), C.uint32_t(color))
  }



//!
//! \brief SgDrawRoundRect calls C.drawRoundRect which draws a 1-pixel wide outline of a rectangle with rounded corners
//! \param px           X coordinate of top-left corner
//! \param py           Y coordinate of top-left corner
//! \param w            Width of the rectangle
//! \param h            Height of the rectangle
//! \param r            Corner radius in pixels
//! \param color        Color value (e.g., 0xRRGGBB or 0xAARRGGBB)
//!
func SgDrawRoundRect(px int, py int, w int, h int, r int, color uint32) {
  C.drawRoundRect(C.int(px), C.int(py), C.int(w), C.int(h), C.int(r), C.uint32_t(color))
  }






//!
//! \brief SgDrawUpRoundRectangle calls C.drawRoundRectangle which draws a filled rectangle with rounded corners
//! \param px           X coordinate of top-left corner
//! \param py           Y coordinate of top-left corner
//! \param w            Width of the rectangle
//! \param h            Height of the rectangle
//! \param r            Corner radius in pixels
//! \param color        Color value (e.g., 0xRRGGBB or 0xAARRGGBB)
//!
func SgDrawUpRoundRectangle(px int, py int, w int, h int, r int, color uint32) {
  C.drawUpRoundRectangle(C.int(px), C.int(py), C.int(w), C.int(h), C.int(r), C.uint32_t(color))
  }



//!
//! \brief SgDrawUpRoundRect calls C.drawRoundRect which draws a 1-pixel wide outline of a rectangle with rounded corners
//! \param px           X coordinate of top-left corner
//! \param py           Y coordinate of top-left corner
//! \param w            Width of the rectangle
//! \param h            Height of the rectangle
//! \param r            Corner radius in pixels
//! \param color        Color value (e.g., 0xRRGGBB or 0xAARRGGBB)
//!
func SgDrawUpRoundRect(px int, py int, w int, h int, r int, color uint32) {
  C.drawUpRoundRect(C.int(px), C.int(py), C.int(w), C.int(h), C.int(r), C.uint32_t(color))
  }


//!
//! \brief SgDrawImage  calls C.drawImage which draws a image
//! \param px           X coordinate of top-left corner
//! \param py           Y coordinate of top-left corner
//! \param w            Width of the rectangle
//! \param h            Height of the rectangle
//! \param image        Image data
//! \param imgWidth     Image width in pixel
//! \param imgHeight    Image height in pixel
//!
func SgDrawImage( px int, py int, w int, h int, image []uint32, imgWidth int, imgHeight int ) {
  C.drawImage( C.int(px), C.int(py), C.int(w), C.int(h), (*C.int)(unsafe.Pointer(&image[0])), C.int(0), C.int(0), C.int(imgWidth), C.int(imgHeight) )
  }

//===============================================
//  Go callback functions



//export goPaint
func goPaint( width C.int, height C.int ) {
  SgScreen.Width = int(width)
  SgScreen.Height = int(height)
  SgScreen.Resize()
  SgScreen.DrawChild( 0, 0 )
  }




var mousePrevLeftButton bool

//export goMouse
func goMouse( x C.int, y C.int, leftButton C.int, rightButton C.int ) {
  //Move mouse
  SgScreen.IsHit( int(x), int(y) )
  if !mousePrevLeftButton && bool(leftButton != 0) {
    SgScreen.MouseClick( int(x), int(y) )
    }
  mousePrevLeftButton = bool(leftButton != 0)
  }




//export goKeyDown
func goKeyDown( code C.int ) {
  if focusItem != nil {
    focusItem.KeyDown( int(code) )
    }
  }



//export goKeyUp
func goKeyUp( code C.int ) {
  }




//export goKeyChar
func goKeyChar( code C.int ) {
  if focusItem != nil {
    focusItem.KeyChar( int(code) )
    }
  }


