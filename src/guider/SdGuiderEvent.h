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
#ifndef SDGUIDEREVENT_H
#define SDGUIDEREVENT_H

#include "SvLib/SvJsonIO.h"

//!
//! \brief The SdGuiderEvent struct Represents a user input event in the guiding system
//! \details Stores complete information about mouse and keyboard input events,
//!          including position, button states, modifier keys, and key codes.
//!          Used for capturing, replaying, and simulating user interactions
//!          with the graphical editor.
//!
struct SdGuiderEvent
  {
    int mStepIndex;    //!< Current step index for title generation
    int mMousePosX;    //!< Mouse position X coordinate in screen space
    int mMousePosY;    //!< Mouse position Y coordinate in screen space
    int mMouseButtons; //!< Mouse buttons state. Bits 0,1,2 for left, right and middle buttons
    int mKeyModifier;  //!< Keyboard modifier key code (Qt::KeyboardModifier). Ctrl, Shift, Alt and so on
    int mKeyCode;      //!< Pressed key code (Qt::Key). One of Qt::Key_...
    int mKeyChar;      //!< Unicode character of pressed key
    int mWheelX;
    int mWheelY;

    //!
    //! \brief Serializes event data to JSON writer
    //! \param js JSON writer object
    //!
    void   json( SvJsonWriter &js ) const;

    //!
    //! \brief Deserializes event data from JSON reader
    //! \param js JSON reader object
    //!
    void   json( const SvJsonReader &js );

    //!
    //! \brief   Injects event of difference between current and next event into main application queue
    //! \details Computes the difference between current event and the next event,
    //!          and injects the transition into the application's event queue.
    //!          Used for replaying recorded sequences.
    //! \param next Next event to transition to
    //! \param windowPos root application window position
    //!
    void   inject( const SdGuiderEvent &next, QPoint windowPos );

    //!
    //! \brief pos Returns global mouse position
    //! \return    Global mouse position
    //!
    QPoint pos() const { return QPoint(mMousePosX,mMousePosY); }

  private:
    void mouseEvent( QWidget *w, QPoint global, QPoint local, const SdGuiderEvent &next, int buttonMask );
  };

using SdGuiderEventList = QList<SdGuiderEvent>;

#endif // SDGUIDEREVENT_H
