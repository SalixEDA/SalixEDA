package main

import (
  "sync/atomic"
  )


// SgItemInterface - базовый интерфейс для всех виджетов
type SgItemInterface interface {
  Add(items ...SgItemInterface)
  ClearChild()
  SetPos(x int, y int)
  GetPos() (x int, y int)
  SetSize(w int, h int)
  GetSize() (w int, h int)
  SetVisible(vis bool)
  IsVisible() bool
  DrawChild( bx int, by int )
  Draw(x int, y int)
  Resize()
  ResizeChild()
  IsHit(localX int, localY int) bool
  MouseClick(localX int, localY int) bool
  IsHoverEnabled() bool
  Hover( enter bool )
  KeyDown(code int)
  KeyChar(code int)
  setFocus(focus bool)
  ParentSet(parent SgItemInterface)
  ParentGet() SgItemInterface
  Ident() int

  Left(base SgItemInterface) int
  Right(base SgItemInterface) int
  Top(base SgItemInterface) int
  Bottom(base SgItemInterface) int
  HCenter(base SgItemInterface) int
  VCenter(base SgItemInterface) int

  HorzEdges(base SgItemInterface) (int, int)
  VertEdges(base SgItemInterface) (int, int)

  LeftEdge() func(base SgItemInterface) int
  RightEdge() func(base SgItemInterface) int
  HCenterEdge() func(base SgItemInterface) int
  TopEdge() func(base SgItemInterface) int
  BottomEdge() func(base SgItemInterface) int
  VCenterEdge() func(base SgItemInterface) int

  AnchorHorzLeft( anchor func(base SgItemInterface) int, gap int )
  AnchorHorzRight( anchor func(base SgItemInterface) int, gap int )
  AnchorHorzCenter( anchor func(base SgItemInterface) int, gap int )
  AnchorHorzCenterIn( ref SgItemInterface, gap int )
  AnchorHorzLeftRight( anchorLeft func(base SgItemInterface) int, gapLeft int, anchorRight func(base SgItemInterface) int, gapRight int )
  AnchorHorzFill( ref SgItemInterface, gapLeft int, gapRight int )

  AnchorVertTop( anchor func(base SgItemInterface) int, gap int )
  AnchorVertBottom( anchor func(base SgItemInterface) int, gap int )
  AnchorVertCenter( anchor func(base SgItemInterface) int, gap int )
  AnchorVertCenterIn( ref SgItemInterface, gap int )
  AnchorVertTopBottom( anchorTop func(base SgItemInterface) int, gapTop int, anchorBottom func(base SgItemInterface) int, gapBottom int )
  AnchorVertFill( ref SgItemInterface, gapTop int, gapBottom int )

  AnchorFillGap( ref SgItemInterface, gapLeft int, gapRight int, gapTop int, gapBottom int )
  AnchorFill( ref SgItemInterface, gap int )
  AnchorFillDef( ref SgItemInterface )

  AnchorCenterIn( ref SgItemInterface, gapHorz int, gapVert int )
  AnchorCenterInDef( ref SgItemInterface )
  }




type SgItem struct {
  ident        int
  PosX,PosY    int
  Width,Height int
  Visible      bool
  Focus        bool
  Parent       SgItemInterface
  Child      []SgItemInterface

  OnResizeW    func(item *SgItem)
  OnResizeH    func(item *SgItem)
  OnClick      func(item *SgItem, localX int, localY int )
  }


var globalIdCounter int32

func NewSgItem(x, y, w, h int) *SgItem {
  return &SgItem {
    ident:    int(atomic.AddInt32( &globalIdCounter, 1 )),
    PosX:     x,
    PosY:     y,
    Width:    w,
    Height:   h,
    Visible:  true,
    Focus:    false,
    Child:    make([]SgItemInterface, 0),
    }
  }



func (i *SgItem) Ident() int {
  return i.ident
  }



func (i *SgItem) Add(items ...SgItemInterface) {
  // Iterate through all provided items
  for _, item := range items {
    item.ParentSet( i )
    i.Child = append(i.Child, item)
    }
  }




func (i *SgItem) ClearChild() {
  i.Child = make([]SgItemInterface, 0)
  }




func (i *SgItem) SetPos(x int, y int) {
  i.PosX = x
  i.PosY = y
  }


func (i *SgItem) GetPos() (x int, y int) {
  return i.PosX, i.PosY
  }



func (i *SgItem) SetSize(w int, h int) {
  i.Width = w
  i.Height = h
  }




func (i *SgItem) GetSize() (w int, h int) {
  return i.Width, i.Height
  }




func (i *SgItem) SetVisible(vis bool) {
  i.Visible = vis
  }


func (i *SgItem) IsVisible() bool {
  return i.Visible
  }




func (i *SgItem) DrawChild( bx int, by int ) {
  if i.Visible {
    for _, child := range i.Child {
      child.Draw( bx, by )
      }
    }
  }


func (i *SgItem) Draw(x int, y int) {
  i.DrawChild( x + i.PosX, y + i.PosY )
  }


func (i *SgItem) Resize() {
  if !i.Visible {
    return
    }
  i.ResizeBind()
  i.ResizeChild()
  }



func (i *SgItem) ResizeBind() {
  if i.OnResizeW != nil {
    i.OnResizeW( i )
    }
  if i.OnResizeH != nil {
    i.OnResizeH( i )
    }
  }



func (i *SgItem) ResizeChild() {
  for _, child := range i.Child {
    child.Resize()
    }
  }




var mouseItem SgItemInterface

func (i *SgItem) IsHit(localX int, localY int) bool {
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
    // Execute own mouse function
    if i.IsHoverEnabled() {
      if mouseItem == nil || mouseItem.Ident() != i.Ident() {
        if mouseItem != nil {
          mouseItem.Hover( false )
          }
        mouseItem = i
        mouseItem.Hover( true )
        }
      return true
      }
    }
  return false
  }



func (i *SgItem) MouseClick(localX int, localY int) bool {
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
    // Execute own mouse function
    if i.OnClick != nil {
      i.OnClick( i, localX, localY )
      return true
      }
    }
  return false
  }


func (i *SgItem) IsHoverEnabled() bool {
  return false
  }


func (i *SgItem) Hover( enter bool ) {
  }


func (i *SgItem) KeyDown(code int) {
  }

func (i *SgItem) KeyChar(code int) {
  }


var focusItem SgItemInterface
func SetFocus( item SgItemInterface ) {
  if focusItem != nil {
    focusItem.setFocus(false)
    }
  focusItem = item
  focusItem.setFocus(true)
  }


func (i *SgItem) setFocus(focus bool) {
  i.Focus = focus
  }



func (i *SgItem) ParentSet(parent SgItemInterface) {
  i.Parent = parent
  }


func (i *SgItem) ParentGet() SgItemInterface {
  return i.Parent
  }









//!
//! \brief Left returns the left edge position in base item coordinates
//! \param base         Base item for coordinate reference
//! \return             Left edge X coordinate relative to base
//!
//! If current item equals base, returns 0.
//! Otherwise recursively calculates position through parent chain.
func (i *SgItem) Left(base SgItemInterface) int {
  // If current item is the base, return 0
  if( i.Ident() == base.Ident() ) {
    return 0
    }
  return i.Parent.Left(base) + i.PosX
  }



//!
//! \brief Right returns the right edge position in base item coordinates
//! \param base         Base item for coordinate reference
//! \return             Right edge X coordinate relative to base
//!
//! If current item equals base, returns item width.
//! Otherwise recursively calculates position through parent chain.
func (i *SgItem) Right(base SgItemInterface) int {
  // If current item is the base, return i.Width
  if( i.Ident() == base.Ident() ) {
    return i.Width
    }
  return i.Parent.Left(base) + i.PosX + i.Width
  }




//!
//! \brief Top returns the top edge position in base item coordinates
//! \param base         Base item for coordinate reference
//! \return             Top edge Y coordinate relative to base
//!
//! If current item equals base, returns 0.
//! Otherwise recursively calculates position through parent chain.
func (i *SgItem) Top(base SgItemInterface) int {
  // If current item is the base, return 0
  if( i.Ident() == base.Ident() ) {
    return 0
    }
  return i.Parent.Top(base) + i.PosY
  }




//!
//! \brief Bottom returns the bottom edge position in base item coordinates
//! \param base         Base item for coordinate reference
//! \return             Bottom edge Y coordinate relative to base
//!
//! If current item equals base, returns item height.
//! Otherwise recursively calculates position through parent chain.
func (i *SgItem) Bottom(base SgItemInterface) int {
  // If current item is the base, return 0
  if( i.Ident() == base.Ident() ) {
    return i.Height
    }
  return i.Parent.Top(base) + i.PosY + i.Height
  }



//!
//! \brief HCenter returns the horizontal center position in base item coordinates
//! \param base         Base item for coordinate reference
//! \return             Center X coordinate relative to base
//!
//! If current item equals base, returns half of width.
//! Otherwise recursively calculates position through parent chain.
func (i *SgItem) HCenter(base SgItemInterface) int {
  // If current item is the base, return i.Width / 2
  if( i.Ident() == base.Ident() ) {
    return i.Width / 2
    }
  return i.Parent.Left(base) + i.PosX + i.Width / 2
  }



//!
//! \brief VCenter returns the vertical center position in base item coordinates
//! \param base         Base item for coordinate reference
//! \return             Center Y coordinate relative to base
//!
//! If current item equals base, returns half of height.
//! Otherwise recursively calculates position through parent chain.
func (i *SgItem) VCenter(base SgItemInterface) int {
  // If current item is the base, return i.Height / 2
  if( i.Ident() == base.Ident() ) {
    return i.Height / 2
    }
  return i.Parent.Top(base) + i.PosY + i.Height / 2
  }





func (i *SgItem) HorzEdges(base SgItemInterface) (int, int) {
  return i.Left(base), i.Right(base)
  }

func (i *SgItem) VertEdges(base SgItemInterface) (int, int) {
  return i.Top(base), i.Bottom(base)
  }




func (i *SgItem) LeftEdge() func(base SgItemInterface) int {
  return func(base SgItemInterface) int {
    return i.Left(base)
    }
  }

func (i *SgItem) RightEdge() func(base SgItemInterface) int {
  return func(base SgItemInterface) int {
    return i.Right(base)
    }
  }

func (i *SgItem) HCenterEdge() func(base SgItemInterface) int {
  return func(base SgItemInterface) int {
    return i.HCenter(base)
    }
  }





func (i *SgItem) TopEdge() func(base SgItemInterface) int {
  return func(base SgItemInterface) int {
    return i.Top(base)
    }
  }

func (i *SgItem) BottomEdge() func(base SgItemInterface) int {
  return func(base SgItemInterface) int {
    return i.Bottom(base)
    }
  }

func (i *SgItem) VCenterEdge() func(base SgItemInterface) int {
  return func(base SgItemInterface) int {
    return i.VCenter(base)
    }
  }









//!
//! \brief AnchorHorzLeft anchors left edge to an anchor function with gap
//! \param anchor       Function returning anchor position
//! \param gap          Gap from anchor to left edge
//!
//! Sets OnResizeW to position item's left edge relative to anchor.
func (i *SgItem) AnchorHorzLeft( anchor func(base SgItemInterface) int, gap int ) {
  i.OnResizeW = func( item *SgItem ) {
    item.PosX = anchor(item.ParentGet()) + gap
    }
  }




//!
//! \brief AnchorHorzRight anchors right edge to an anchor function with gap
//! \param anchor       Function returning anchor position
//! \param gap          Gap from right edge to anchor
//!
//! Sets OnResizeW to position item's right edge relative to anchor.
func (i *SgItem) AnchorHorzRight( anchor func(base SgItemInterface) int, gap int ) {
  i.OnResizeW = func( item *SgItem ) {
    item.PosX = anchor(item.ParentGet()) - gap - item.Width
    }
  }




//!
//! \brief AnchorHorzCenter anchors horizontal center to an anchor function with gap
//! \param anchor       Function returning anchor position
//! \param gap          Gap from center to anchor
//!
//! Sets OnResizeW to center item relative to anchor.
func (i *SgItem) AnchorHorzCenter( anchor func(base SgItemInterface) int, gap int ) {
  i.OnResizeW = func( item *SgItem ) {
    item.PosX = anchor(item.ParentGet()) + gap - item.Width/2
    }
  }



func (i *SgItem) AnchorHorzCenterIn( ref SgItemInterface, gap int ) {
  i.AnchorHorzCenter( ref.HCenterEdge(), gap )
  }





//!
//! \brief AnchorHorzLeftRight anchors both left and right edges
//! \param anchorLeft   Function returning left anchor position
//! \param gapLeft      Gap from left anchor to left edge
//! \param anchorRight  Function returning right anchor position
//! \param gapRight     Gap from right edge to right anchor
//!
//! Sets OnResizeW to stretch item between two anchors.
func (i *SgItem) AnchorHorzLeftRight( anchorLeft func(base SgItemInterface) int, gapLeft int, anchorRight func(base SgItemInterface) int, gapRight int ) {
  i.OnResizeW = func( item *SgItem ) {
    item.PosX = anchorLeft(item.ParentGet()) + gapLeft
    item.Width = anchorRight(item.ParentGet()) - gapRight - item.PosX
    }
  }




//!
//! \brief AnchorHorzFill fills horizontally relative to reference item
//! \param ref          Reference item for boundaries
//! \param gapLeft      Gap from left edge of ref to left edge
//! \param gapRight     Gap from right edge to right edge of ref
//!
//! Sets OnResizeW to fill between reference item's left and right edges.
func (i *SgItem) AnchorHorzFill( ref SgItemInterface, gapLeft int, gapRight int ) {
  i.OnResizeW = func( item *SgItem ) {
    left,right := ref.HorzEdges(item.ParentGet())
    item.PosX = left + gapLeft
    item.Width = right - gapRight - item.PosX
    }
  }







//!
//! \brief AnchorVertTop anchors top edge to an anchor function with gap
//! \param anchor       Function returning anchor position
//! \param gap          Gap from anchor to top edge
//!
//! Sets OnResizeH to position item's top edge relative to anchor.
func (i *SgItem) AnchorVertTop( anchor func(base SgItemInterface) int, gap int ) {
  i.OnResizeH = func( item *SgItem ) {
    item.PosY = anchor(item.ParentGet()) + gap
    }
  }




//!
//! \brief AnchorVertBottom anchors bottom edge to an anchor function with gap
//! \param anchor       Function returning anchor position
//! \param gap          Gap from bottom edge to anchor
//!
//! Sets OnResizeH to position item's bottom edge relative to anchor.
func (i *SgItem) AnchorVertBottom( anchor func(base SgItemInterface) int, gap int ) {
  i.OnResizeH = func( item *SgItem ) {
    item.PosY = anchor(item.ParentGet()) - gap - item.Height
    }
  }




//!
//! \brief AnchorVertCenter anchors vertical center to an anchor function with gap
//! \param anchor       Function returning anchor position
//! \param gap          Gap from center to anchor
//!
//! Sets OnResizeH to center item vertically relative to anchor.
func (i *SgItem) AnchorVertCenter( anchor func(base SgItemInterface) int, gap int ) {
  i.OnResizeH = func( item *SgItem ) {
    item.PosY = anchor(item.ParentGet()) + gap - item.Height/2
    }
  }



func (i *SgItem) AnchorVertCenterIn( ref SgItemInterface, gap int ) {
  i.AnchorVertCenter( ref.VCenterEdge(), gap )
  }




//!
//! \brief AnchorVertTopBottom anchors both top and bottom edges
//! \param anchorTop    Function returning top anchor position
//! \param gapTop       Gap from top anchor to top edge
//! \param anchorBottom Function returning bottom anchor position
//! \param gapBottom    Gap from bottom edge to bottom anchor
//!
//! Sets OnResizeH to stretch item between two anchors.
func (i *SgItem) AnchorVertTopBottom( anchorTop func(base SgItemInterface) int, gapTop int, anchorBottom func(base SgItemInterface) int, gapBottom int ) {
  i.OnResizeH = func( item *SgItem ) {
    item.PosY = anchorTop(item.ParentGet()) + gapTop
    item.Height = anchorBottom(item.ParentGet()) - gapBottom - item.PosY
    }
  }




//!
//! \brief AnchorVertFill fills vertically relative to reference item
//! \param ref          Reference item for boundaries
//! \param gapTop       Gap from top edge of ref to top edge
//! \param gapBottom    Gap from bottom edge to bottom edge of ref
//!
//! Sets OnResizeH to fill between reference item's top and bottom edges.
func (i *SgItem) AnchorVertFill( ref SgItemInterface, gapTop int, gapBottom int ) {
  i.OnResizeH = func( item *SgItem ) {
    top,bottom := ref.VertEdges(item.ParentGet())
    item.PosY = top + gapTop
    item.Height = bottom - gapBottom - item.PosY
    }
  }




//!
//! \brief AnchorFillGap fills both horizontally and vertically with specified gaps
//! \param ref          Reference item for boundaries
//! \param gapLeft      Left gap
//! \param gapRight     Right gap
//! \param gapTop       Top gap
//! \param gapBottom    Bottom gap
//!
//! Combines horizontal and vertical fill with individual gaps.
func (i *SgItem) AnchorFillGap( ref SgItemInterface, gapLeft int, gapRight int, gapTop int, gapBottom int ) {
  i.AnchorHorzFill( ref, gapLeft, gapRight )
  i.AnchorVertFill( ref, gapTop, gapBottom )
  }



//!
//! \brief AnchorFill fills both horizontally and vertically with uniform gap
//! \param ref          Reference item for boundaries
//! \param gap          Uniform gap on all sides
//!
//! Combines horizontal and vertical fill with same gap on all sides.
func (i *SgItem) AnchorFill( ref SgItemInterface, gap int ) {
  i.AnchorFillGap( ref, gap, gap, gap, gap )
  }



//!
//! \brief AnchorFillDef fills both horizontally and vertically with no gaps
//! \param ref          Reference item for boundaries
//!
//! Default fill without any gaps on all sides.
func (i *SgItem) AnchorFillDef( ref SgItemInterface ) {
  i.AnchorFillGap( ref, 0, 0, 0, 0 )
  }




//!
//! \brief AnchorCenterIn centers item within reference item with gaps
//! \param ref          Reference item to center in
//! \param gapHorz      Horizontal gap from center
//! \param gapVert      Vertical gap from center
//!
//! Centers item horizontally and vertically within reference item.
func (i *SgItem) AnchorCenterIn( ref SgItemInterface, gapHorz int, gapVert int ) {
  i.AnchorHorzCenter( ref.HCenterEdge(), gapHorz )
  i.AnchorVertCenter( ref.VCenterEdge(), gapVert )
  }




//!
//! \brief AnchorCenterInDef centers item within reference item with no gaps
//! \param ref          Reference item to center in
//!
//! Default centering without any gaps.
func (i *SgItem) AnchorCenterInDef( ref SgItemInterface ) {
  i.AnchorHorzCenter( ref.HCenterEdge(), 0 )
  i.AnchorVertCenter( ref.VCenterEdge(), 0 )
  }







var SgScreen = &SgItem {
  ident:  -1,
  PosX:    0,
  PosY:    0,
  Width:   0,
  Height:  0,
  Visible: true,
  Child:   make( []SgItemInterface, 0 ),
  }

