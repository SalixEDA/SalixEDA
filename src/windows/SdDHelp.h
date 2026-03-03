/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Dialog with help page. Used to display help to dialogs.
*/
#ifndef SDDHELP_H
#define SDDHELP_H

#include <QDialog>

//Previous declaration of help widget class
class SdWHelp;

class SdDHelp : public QDialog
  {
    Q_OBJECT

    //Help browser
    SdWHelp *mWHelp;
  public:
    SdDHelp( const QString &helpPage, QWidget *parent );

    static void help( const QString &helpPage, QWidget *parent );
  };

#endif // SDDHELP_H
