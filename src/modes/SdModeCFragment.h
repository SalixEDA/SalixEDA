/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Insertion fragment from library
*/
#ifndef SDMODECFRAGMENT_H
#define SDMODECFRAGMENT_H

#include "SdModeCommon.h"
#include "objects/SdSelector.h"
#include "objects/SdProject.h"


class SdModeCFragment : public SdModeCommon
  {
  protected:
    SdSelector      mPaste;        //!< Fragment for insertion [Фрагмент для вставки (копирования)]
    SdProject      *mPastePrj;     //!< Project, from which insertion [Проект, из которого производится вставка]
    SdPoint         mFirst;        //!< First point [Начальная точка]
    SdPoint         mPrevMove;     //!< Previous point [Предыдущая точка]
    SdPoint         mCurPoint;     //!< Current point [Текущая точка]
    QString         mObjectName;   //!< Object name for insertion
  public:
    SdModeCFragment( SdWEditorGraph *editor, SdProjectItem *obj, SdProject *project = nullptr, const QString &objectName = QString{} );
    ~SdModeCFragment() override;

    // SdMode interface
  public:
    virtual void    activate() override;
    virtual void    drawDynamic(SdContext *ctx) override;
    virtual int     getPropBarId() const override;
    virtual void    enterPoint(SdPoint point) override;
    virtual void    cancelPoint(SdPoint) override;
    virtual void    movePoint(SdPoint p) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getCursor() const override;
    virtual int     getIndex() const override;

  private:
    void getFragment();
    void clear();
  };

#endif // SDMODECFRAGMENT_H
