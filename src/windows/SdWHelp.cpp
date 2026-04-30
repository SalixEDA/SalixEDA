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
#include "SdConfig.h"
#include "SdWHelp.h"
#include "SdWMain.h"
#include "SdDGuiderPlayer.h"
#include "objects/SdEnvir.h"
#include "SvLib/SvDir.h"

#include <QUrl>
#include <QSettings>
#include <QFile>
#include <QCoreApplication>
#include <QStringList>
#include <QFileInfo>
#include <QDebug>
#include <QMessageBox>
#include <QDesktopServices>



//Common constructor for help widgets
SdWHelp::SdWHelp() :
  QTextBrowser( nullptr ),
  mMain(nullptr)
  {
  zoomIn(2);

  setOpenLinks(false);
  //Replace anchor clicked
  connect( this, &SdWHelp::anchorClicked, this, [this] ( QUrl url) {
    //Test special case for intro page
    //In intro page we can open project, create new project or open previously file
    QString path = url.fileName();
    if( path.endsWith( QStringLiteral(".mp4")) )
      openGuider( path );
    else if( path.contains( QString("://") ) )
      QDesktopServices::openUrl( url );
    else {
      if( url.hasFragment() )
        setSource( pageConvert( url.fileName(), url.fragment() ) );
      else
        setSource( pageConvert( url.fileName(), QString() ) );
      }
    });
  }





//Constructor for intro help page widget
SdWHelp::SdWHelp(SdWMain *main) :
  QTextBrowser( nullptr ),
  mMain(main)
  {
  zoomIn(2);

  setOpenLinks(false);
  //Replace anchor clicked
  connect( this, &SdWHelp::anchorClicked, this, [this] ( QUrl url) {
    //Test special case for intro page
    //In intro page we can open project, create new project or open previously file
    QString path = url.toString();
    if( mMain ) {
      qDebug() << "help" << path;
      if( path.startsWith("open:") ) { mMain->cmFileOpen(); return; }
      else if( path.startsWith("new:") ) { mMain->cmFileNew(); return; }
      else if( path.startsWith("load:") ) { mMain->cmFileOpenFile( path.mid(5) ); return; }
      else if( path.startsWith("library:") ) { mMain->cmFileLoadUid( path.mid(8) ); return; }
      }
    if( path.endsWith( QStringLiteral(".mp4")) )
      openGuider( path );
    else if( path.contains( QString("://") ) )
      QDesktopServices::openUrl( url );
    else if( mMain != nullptr ) {
      if( url.hasFragment() )
        mMain->cmHelpPage( url.fileName() + QStringLiteral("#") + url.fragment() );
      else
        mMain->cmHelpPage( url.fileName() );
      }
    });
  helpIntro();
  }





QUrl SdWHelp::pageConvert(const QString &page, const QString &fragment)
  {
  //Interface language
  //Язык интерфейса
  QString lang = SdEnvir::languageGet();
  QString core;
  QString ext;

  int pointIndex = page.lastIndexOf(QChar('.'));
  if( pointIndex > 0 ) {
    core = page.left( pointIndex );
    ext  = page.mid( pointIndex );

    if( core.contains(QChar('-')) ) {
      pointIndex = core.lastIndexOf(QChar('-'));
      lang = core.mid( pointIndex + 1 );
      core = core.left( pointIndex );
      }

    //We use external or internal path for help in case of start page with "hx" prefix
    QString dirPath( page.startsWith("hx") ? externHelpPath() : helpPath() );

    //Test if file exist with language lang
    //Проверить наличие файла с языком
    QString path( dirPath + core + "-" + lang + ext );
    if( QFile::exists( path ) ) {
      //File exist. Build url
      QUrl url = QUrl::fromLocalFile(path);
      if( !fragment.isEmpty() )
        url.setFragment( fragment );
      return url;
      }
    //Test if file exist with english language
    //Проверить наличие файла с анлийским языком
    else if( QFile::exists( dirPath + core + "-en" + ext ) ) {
      //File exist. Build url
      QUrl url = QUrl::fromLocalFile( dirPath + core + "-en" + ext);
      if( !fragment.isEmpty() )
        url.setFragment( fragment );
      return url;
      }
    }


  //No file exist, return error file
  //Никакого файла нету, выдать файл с ошибкой
  return pageError();
  }





QUrl SdWHelp::pageError()
  {
  //Interface language
  //Язык интерфейса
  QString lang = SdEnvir::languageGet();
  QString path( helpPath() + "errorNoPage-" + lang + ".md" );
  //Test if exist error page with current language
  //Проверить наличие страницы с ошибкой на языке пользователя
  if( QFile::exists( path ) )
    //Page exist, build url and return it
    return QUrl::fromLocalFile( path );

  //Return url with english error page
  //Вернуть страницу с ошибкой на английском
  return QUrl::fromLocalFile( helpPath() + "errorNoPage-en.md" );
  }








//Show help contents
void SdWHelp::contents()
  {
  //Setup home page
  //Выдать начальную страницу
  setSource( pageConvert( "contents.htm", QString() ) );
  }





//Show help topic
void SdWHelp::helpTopic(const QString topic)
  {
  //qDebug() << "help topic" << topic;
  if( topic.endsWith( QStringLiteral(".mp4")) )
    openGuider( topic );
  else if( topic.contains(QChar('#')) ) {
    //Topic contains local position
    int i = topic.indexOf( QChar('#') );
    //File part
    QString file = topic.left(i);
    //Local point part
    QString local = topic.mid( i + 1 );
    setSource( pageConvert( file, local ) );
    }
  else
    setSource( pageConvert( topic, QString() ) );
  }





//Show intro topic
//Here we injecting into html page previous file list
void SdWHelp::helpIntro()
  {
  //Interface language
  //Язык интерфейса
  QString lang = SdEnvir::languageGet();

  QString fname( QString(":/startupPage/%1-startup.htm").arg(lang) );
  //Test if file exist with language lang
  //Проверить наличие файла с языком
  if( !QFile::exists( fname ) )
    fname = QString(":/startupPage/en-startup.htm");

  QFile file(fname);
  if( file.open(QIODevice::ReadOnly) ) {
    //Load intro file contents
    QString html = QString::fromUtf8( file.readAll() );

    //Get previously file list
    QSettings settings;
    QStringList files = settings.value(SDK_PREVIOUS_FILES).toStringList();

    //Create html view previously file list
    QString prev;
    for( const QString &str : std::as_const(files) ) {
      QFileInfo info(str);
      prev.append("<p><a href=\"load:").append(str).append("\">").append( info.completeBaseName() ).append("</a><br>(").append(str).append(")</p>");
      }

    //Show intro with injected previously file list
    setSearchPaths( {helpPath(),externHelpPath()} );
    setHtml( html.arg(prev) );
    }
  }





//!
//! \brief openGuider Opens guider player dialog
//! \param topic      Topic name
//!
void SdWHelp::openGuider(const QString &topic)
  {
  //Remove .mp4 from end
  QString path( topic.left( topic.length() - 4 ) );
  if( SdDGuiderPlayer::guiderExist( path ) ) {
    //Show guide player dialog
    SdDGuiderPlayer player( path, this );
    player.exec();
    }
  else
    QMessageBox::warning( this, tr("Error!"), tr("Guider file '%1' not exist. Try reinstall SalixEDA").arg(topic) );
  }





//!
//! \brief helpPath Returns internal help path (from resources)
//! \return         Internal help path
//!
QString SdWHelp::helpPath()
  {
  return QStringLiteral(":/help/");
  }




//!
//! \brief externHelpPath Returns external help path (applicationPath/help)
//! \return               External help path
//!
QString SdWHelp::externHelpPath()
  {
  static QString helpPath;
  if( helpPath.isEmpty() ) {
    SvDir dir( QCoreApplication::applicationDirPath() );
    dir.cd( QStringLiteral("help/") );
    helpPath = dir.slashedPath();
    }
  return helpPath;
  }



