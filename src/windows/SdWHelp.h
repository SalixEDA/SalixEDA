/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Widget to display help
*/
#ifndef SDWHELP_H
#define SDWHELP_H

#include <QTextBrowser>

class SdWMain;

class SdWHelp : public QTextBrowser
  {
    Q_OBJECT

    SdWMain *mMain; //Main window pointer
  public:
    //Common constructor for help widgets
    SdWHelp();
    //Constructor for intro help page widget
    SdWHelp( SdWMain *main );


    //!
    //! \brief helpPath Returns internal help path (from resources)
    //! \return         Internal help path
    //!
    static QString helpPath();

    //!
    //! \brief externHelpPath Returns external help path (applicationPath/help)
    //! \return               External help path
    //!
    static QString externHelpPath();

    //Conversion page name with fragment point in page to URL with current language
    //Преобразование названия страницы в фактический URL в соответствии с выбранным языком
    static QUrl    pageConvert( const QString &page , const QString &fragment);

    //Return page with reference error
    //Вернуть страницу помощи с ошибкой ссылки
    static QUrl    pageError();

  signals:

  public slots:
    //Show help contents
    void contents();

    //Show help topic
    void helpTopic( const QString topic );

  private:

    //Show intro topic
    void helpIntro();

    //!
    //! \brief openGuider Opens guider player dialog
    //! \param topic      Topic name
    //!
    void openGuider( const QString &topic );
  };

#endif // SDWHELP_H
