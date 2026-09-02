/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Symbol implementation properties bar for component insertion and edit in sheet editor
*/
#ifndef SDPROPBARSYMIMP_H
#define SDPROPBARSYMIMP_H

#include "objects/SdPropSymImp.h"
#include "SdPropBar.h"

struct SdPropSymImp;

class SdPropBarSymImp : public SdPropBar
  {
    Q_OBJECT

    //Symbol direction
    QAction   *mDir0;
    QAction   *mDir90;
    QAction   *mDir180;
    QAction   *mDir270;

    //Mirror
    QAction   *mMirror;
  public:
    SdPropBarSymImp( const QString title );

    void setPropSymImp( const SdPropSymImp &propSymImp );
    void getPropSymImp( SdPropSymImp &propSymImp );

    void setPropSymImp( const SdPropComposerSymImp &propSymImp );
    void getPropSymImp( SdPropComposerSymImp &propSymImp );

    //!
    //! \brief mBarId Prop Bar id
    //!
    static constexpr int mBarId = PB_SYM_IMP;
  private:
    void setDirection( int dir );
  };

using SdPropBarSymImpPtr = SdPropBarPointer<SdPropBarSymImp>;

#endif // SDPROPBARSYMIMP_H
