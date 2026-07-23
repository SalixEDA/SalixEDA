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

class SdWHelp : public QTextBrowser
  {
    Q_OBJECT
  public:
    //Common constructor for help widgets
    SdWHelp( bool isIntro );


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

    //!
    //! \brief examplesPath Returns path for examples (applicationPath/examples)
    //! \return             Examples path
    //!
    static QString examplesPath();

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
