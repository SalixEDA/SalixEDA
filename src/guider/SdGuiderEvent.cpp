/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  SdGuiderEvent Stores complete information about mouse and keyboard input events,
  including position, button states, modifier keys, and key codes.
  Used for capturing, replaying, and simulating user interactions
  with the graphical editor.
*/
#include "guider/SdGuiderEvent.h"

#include <QApplication>
#include <QWidget>
#include <QMouseEvent>

//!
//! \brief Serializes event data to JSON writer
//! \param js JSON writer object
//!
void SdGuiderEvent::json(SvJsonWriter &js) const
  {
  js.jsonInt( "StepIndex", mStepIndex );
  js.jsonInt( "PosX",      mMousePosX );
  js.jsonInt( "PosY",      mMousePosY );
  js.jsonInt( "Buttons",   mMouseButtons );
  js.jsonInt( "Modifier",  mKeyModifier );
  js.jsonInt( "KeyCode",   mKeyCode );
  js.jsonInt( "KeyChar",   mKeyChar );
  js.jsonInt( "WheelX",    mWheelX );
  js.jsonInt( "WheelY",    mWheelY );
  }




//!
//! \brief Deserializes event data from JSON reader
//! \param js JSON reader object
//!
void SdGuiderEvent::json(const SvJsonReader &js)
  {
  js.jsonInt( "StepIndex", mStepIndex );
  js.jsonInt( "PosX",      mMousePosX );
  js.jsonInt( "PosY",      mMousePosY );
  js.jsonInt( "Buttons",   mMouseButtons );
  js.jsonInt( "Modifier",  mKeyModifier );
  js.jsonInt( "KeyCode",   mKeyCode );
  js.jsonInt( "KeyChar",   mKeyChar );
  js.jsonInt( "WheelX",    mWheelX );
  js.jsonInt( "WheelY",    mWheelY );
  }




//!
//! \brief   Injects event of difference between current and next event into main application queue
//! \details Computes the difference between current event and the next event,
//!          and injects the transition into the application's event queue.
//!          Used for replaying recorded sequences.
//! \param next Next event to transition to
//!
void SdGuiderEvent::inject( const SdGuiderEvent &next, QPoint windowPos )
  {
  mStepIndex = next.mStepIndex;

  QPoint global( next.pos() + windowPos );

  QWidget *w = QApplication::widgetAt( global );

  QPoint local;
  if( w != nullptr ) {
    local = w->mapFromGlobal( global );

    if( mMousePosX != next.mMousePosX || mMousePosY != next.mMousePosY ) {
      //Mouse move event
      QCoreApplication::postEvent( w, new QMouseEvent( QEvent::MouseMove,
                                                       local,
                                                       global,
                                                       Qt::NoButton,
                                                       (Qt::MouseButton)mMouseButtons,
                                                       (Qt::KeyboardModifier)mKeyModifier ) );
      mMousePosX = next.mMousePosX;
      mMousePosY = next.mMousePosY;
      }

    if( mMouseButtons != next.mMouseButtons ) {
      mouseEvent( w, global, local, next, Qt::LeftButton );
      mouseEvent( w, global, local, next, Qt::RightButton );
      mouseEvent( w, global, local, next, Qt::MiddleButton );
      }

    if( mWheelX != next.mWheelX || mWheelY != next.mWheelY ) {
      //Mouse wheel event
      QCoreApplication::postEvent( w, new QWheelEvent( local,
                                                       global,
                                                       QPoint(0,0), //pixel data
                                                       QPoint(next.mWheelX-mWheelX, next.mWheelY-mWheelY),
                                                       (Qt::MouseButton)mMouseButtons,
                                                       (Qt::KeyboardModifier)mKeyModifier,
                                                       Qt::NoScrollPhase,
                                                       false) );

      mWheelX       = next.mWheelX;
      mWheelY       = next.mWheelY;
      }
    }
  else {
    mMousePosX    = next.mMousePosX;
    mMousePosY    = next.mMousePosY;
    mMouseButtons = next.mMouseButtons;
    mWheelX       = next.mWheelX;
    mWheelY       = next.mWheelY;
    }

  if( mKeyCode != next.mKeyCode || mKeyChar != next.mKeyChar ) {
    w = QApplication::focusWidget();
    if( w != nullptr ) {
      if( (mKeyCode | mKeyChar) ) {
        //Key release
        QCoreApplication::postEvent( w, new QKeyEvent( QEvent::KeyRelease,
                                                       mKeyCode,
                                                       (Qt::KeyboardModifier)mKeyModifier,
                                                       mKeyChar ? QString( QChar(mKeyChar) ) : QString{}
                                                       ));
        mKeyCode = next.mKeyCode;
        mKeyChar = next.mKeyChar;
        }
      else {
        //Key press
        mKeyCode = next.mKeyCode;
        mKeyChar = next.mKeyChar;
        QCoreApplication::postEvent( w, new QKeyEvent( QEvent::KeyPress,
                                                       mKeyCode,
                                                       (Qt::KeyboardModifier)mKeyModifier,
                                                       mKeyChar ? QString( QChar(mKeyChar) ) : QString{}
                                                       ));
        }
      }
    else {
      mKeyCode = next.mKeyCode;
      mKeyChar = next.mKeyChar;
      }
    }
  }


void SdGuiderEvent::mouseEvent(QWidget *w, QPoint global, QPoint local, const SdGuiderEvent &next, int buttonMask)
  {
  if( (mMouseButtons & buttonMask) != (next.mMouseButtons & buttonMask) ) {
    if( (mMouseButtons & buttonMask) == 0 ) {
      //Button pressed
      mMouseButtons |= buttonMask;
      QCoreApplication::postEvent( w, new QMouseEvent( QEvent::MouseButtonPress,
                                                       local,
                                                       global,
                                                       (Qt::MouseButton)buttonMask,
                                                       (Qt::MouseButton)mMouseButtons,
                                                       (Qt::KeyboardModifier)mKeyModifier ) );
      }
    else {
      //Mouse released
      mMouseButtons &= ~buttonMask;
      QCoreApplication::postEvent( w, new QMouseEvent( QEvent::MouseButtonRelease,
                                                       local,
                                                       global,
                                                       (Qt::MouseButton)buttonMask,
                                                       (Qt::MouseButton)mMouseButtons,
                                                       (Qt::KeyboardModifier)mKeyModifier ) );
      }
    }
  }
