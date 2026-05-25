/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Base tool bar for property bars with layers
*/
#ifndef SDPROPBARLAY_H
#define SDPROPBARLAY_H

#include "objects/SdLayer.h"
#include "objects/SdProjectItem.h"
#include "SdPropBar.h"

#include <QToolBar>
#include <QComboBox>

class SdPropBarLay : public SdPropBar
  {
    Q_OBJECT

    SdClass     mEditObjectClass; //!< Class of edit object (one of derived from SdProjectItem)
    SdPvStratum mStratum;         //!< Plate stratum otherwise stmThrough
  protected:
    QComboBox    *mLayer;         //!< Visual list of available layers

    void     setSelectedLayer( SdLayer *layer );
    SdLayer *getSelectedLayer();
  public:
    SdPropBarLay(const QString title);

  signals:
    //On property changed by user
    void propChanged();

  public slots:
    virtual void updateViewedLayers( SdLayer *currentLayer );

    virtual void setEditObject(SdProjectItem *pitem) override;

    void         setEditObject(SdClass theClass, SdPvStratum stratum);

  private:
    //!
    //! \brief refillLayers Refill list of available layers and correct ability selection of required layer
    //! \param required     Layer required selected
    //! \return             Available selected layer
    //!
    SdLayer     *refillLayers( SdLayer *required );
  };

#endif // SDPROPBARLAY_H
