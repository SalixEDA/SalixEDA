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
#include <QMenuBar>
#include <QMainWindow>

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
  js.jsonInt( "KeyEvent",  mKeyEventType );
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
  js.jsonInt( "KeyEvent",  mKeyEventType );
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

  if( next.mKeyEventType ) {
    w = QApplication::focusWidget();
    if( w != nullptr ) {
      if( next.mKeyEventType == 2 ) {
        //Key release
        if( next.mKeyCode == Qt::Key_F8 ) {
          //Special case. We treat F8 release as F8 press
          QMainWindow *win = dynamic_cast<QMainWindow*>(QApplication::activeWindow());
          if( win != nullptr ) {
            QMenuBar *bar = win->menuBar();
            bar->setFocus();
            bar->setActiveAction(bar->actions().first());
            }
          }
        else {
          QCoreApplication::postEvent( w, new QKeyEvent( QEvent::KeyRelease,
                                                         next.mKeyCode,
                                                         (Qt::KeyboardModifier)next.mKeyModifier,
                                                         next.mKeyChar ? QString( QChar(next.mKeyChar) ) : QString{}
                                                         ));
          }
        }
      else {
        //Special cases for menu selection
        if( next.mKeyCode == Qt::Key_Down ) {
          //Special case. We treat F8 release as F8 press
          QMainWindow *win = dynamic_cast<QMainWindow*>(QApplication::activeWindow());
          if( win != nullptr ) {
            QMenuBar *bar = win->menuBar();
            if( w == bar ) {
              QAction *act = bar->activeAction();
              if( act != nullptr ) {
                QMenu *menu = act->menu();
                if( menu != nullptr )
                  w = menu;
                }
              }
            }
          }
        //Key press
        QCoreApplication::postEvent( w, new QKeyEvent( QEvent::KeyPress,
                                                       next.mKeyCode,
                                                       (Qt::KeyboardModifier)next.mKeyModifier,
                                                       next.mKeyChar ? QString( QChar(next.mKeyChar) ) : QString{}
                                                                       ));
        }
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
      if( (buttonMask & Qt::LeftButton) && w != nullptr )
        w->setFocus();
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
