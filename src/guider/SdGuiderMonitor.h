#ifndef SDGUIDERMONITOR_H
#define SDGUIDERMONITOR_H

#include <QObject>

class SdGuiderMonitor : public QObject
  {
    Q_OBJECT

  public:

    // QObject interface
  public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
  };

#endif // SDGUIDERMONITOR_H
