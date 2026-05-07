/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Graphical base object for symbol pins map which support pins manipulation
  and net connection to pins
*/
#ifndef SDGRAPHSYMPINSMAP_H
#define SDGRAPHSYMPINSMAP_H

#include "SdGraphParam.h"

class SdGraphSymPinsMap : public SdGraphParam
  {
  public:
    SdGraphSymPinsMap() : SdGraphParam() {}
    SdGraphSymPinsMap(const SdStringMap &param) : SdGraphParam(param) {}

    //!
    //! \brief isPinConnected Returns the connection status of a pin.
    //! \param pinName        Name of the pin to be checked.
    //! \return               true if a net is connected to the pin, false otherwise.
    //!
    virtual bool      isPinConnected( const QString pinName ) const = 0;

    //!
    //! \brief pinNetName Returns the name of the net connected to the pin.
    //! \param pinName    Name of the pin to be checked.
    //! \return           Net name connected to the pin, or an empty string if none.
    //!
    virtual QString   pinNetName( const QString pinName ) const = 0;

    //!
    //! \brief unLinkPart Unlink part impelement from symbol implement
    //! \param partImp    Part implement
    //! \param undo       Undo operation
    //!
    virtual void      unLinkPart( SdGraphPartImp *partImp, SdUndo *undo ) = 0;

  };

#endif // SDGRAPHSYMPINSMAP_H
