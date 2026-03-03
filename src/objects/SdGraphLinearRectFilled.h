/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Graphical object filled rectangle.
*/
#ifndef SDGRAPHLINEARRECTFILLED_H
#define SDGRAPHLINEARRECTFILLED_H

#include "SdGraphLinearRect.h"

#define SD_TYPE_RECT_FILLED "RectFilled"

class SdGraphLinearRectFilled : public SdGraphLinearRect
  {
  public:
    SdGraphLinearRectFilled();
    SdGraphLinearRectFilled( SdPoint p1, SdPoint p2, const SdPropLine &propLine );

    // SdObject interface
    virtual QString getType() const override { return QStringLiteral(SD_TYPE_RECT_FILLED); }

    // SdGraph interface
    virtual void    selectByPoint(const SdPoint p, SdSelector *selector) override;
//    virtual void    selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual void    draw(SdContext *dc) override;
    //!
    //! \brief draw3d Draws object in 3d space
    //! \param f      3d draw functions with predefined 3d context
    //!
    virtual void    draw3d( QOpenGLFunctions_2_0 *f ) const override;
    virtual int     behindCursor(SdPoint p) override;
  };

#endif // SDGRAPHLINEARRECTFILLED_H
