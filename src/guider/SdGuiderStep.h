#ifndef SDGUIDERSTEP_H
#define SDGUIDERSTEP_H

#include <QString>
#include <QList>

struct SdGuiderStep
  {
    int     mDuration = 3; //!< Step duration in sec. Default value is 3 sec
    QString mTiter;        //!< Step titer
  };

using SdGuiderStepList = QList<SdGuiderStep>;

#endif // SDGUIDERSTEP_H
