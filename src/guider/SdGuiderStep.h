/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  SdGuiderStep represents a single step in the guider scenario
*/
#ifndef SDGUIDERSTEP_H
#define SDGUIDERSTEP_H

#include <QString>
#include <QList>

//! \brief Represents a single step in the guider scenario
struct SdGuiderStep
  {
    int     mDuration = 3; //!< Step duration in seconds. Default value is 3 sec
    QString mTitle;        //!< Step title, displayed below the guider movie
  };

//! \brief Step list for a single scene
using SdGuiderStepList = QList<SdGuiderStep>;

#endif // SDGUIDERSTEP_H
