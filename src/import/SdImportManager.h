/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  The SdImportManager class is used to create and manage a unified list of importers.
  This list is utilized to display available importers in the main menu. When an
  importer is selected from the list, a unified import function is invoked.
*/
#ifndef SDIMPORTMANAGER_H
#define SDIMPORTMANAGER_H

#include "SvLib/SvSingleton.h"

#include <QString>
#include <QList>
#include <functional>

class SdProject;
class QWidget;

using SdImportFunction = std::function<bool(QWidget*, const QStringList&, SdProject*, bool)>;



//!
//! \brief The SdImportManager class is used to create and manage a unified list of importers.
//!
//! This list is utilized to display available importers in the main menu.
//! When an importer is selected from the list, a unified import function is invoked.
//!
class SdImportManager
  {
    struct SdImportItem
      {
        QString          mMenuTitle;          //!< Menu title for menu item in main menu
        QString          mIconTitle;          //!< Icon name for menu item in main menu
        QString          mDialogTitle;        //!< Title for import file selection dialog
        QString          mDialogFilter;       //!< File filter for import file selection dialog
        bool             mUseExistingProject; //!< When true we try import into current existing project, otherwise we create new one
        SdImportFunction mImportFunctor;      //!< Perform import
      };

    QList<SdImportItem>  mImportItemList;     //!< List of imports

    //Private constructor for disable unauthorizable creation
    SdImportManager() {}
  public:
    SV_SINGLETON( SdImportManager )

    //!
    //! \brief count Returns count of registered imports
    //! \return      Count of registered imports
    //!
    int count() const { return mImportItemList.count(); }

    //!
    //! \brief menuTitle Returns menu title for the registered import item by index
    //! \param index     Index of the registered import item
    //! \return          Menu title string
    //!
    QString menuTitle( int index ) const { return mImportItemList.at(index).mMenuTitle; }

    //!
    //! \brief iconName Returns icon name for the registered import item by index
    //! \param index    Index of the registered import item
    //! \return         Icon name string
    //!
    QString iconName( int index ) const { return mImportItemList.at(index).mIconTitle; }

    //!
    //! \brief dialogTitle Returns title for import file selection dialog by index
    //! \param index       Index of the registered import item
    //! \return            Dialog title string
    //!
    QString dialogTitle( int index ) const { return mImportItemList.at(index).mDialogTitle; }

    //!
    //! \brief dialogFilter Returns file filter for import file selection dialog by index
    //! \param index        Index of the registered import item
    //! \return             Dialog file filter string
    //!
    QString dialogFilter( int index ) const { return mImportItemList.at(index).mDialogFilter; }

    //!
    //! \brief useExistingProject Returns flag whether to import into the current existing project
    //! \param index              Index of the registered import item
    //! \return                   True if import should be made into existing project, false otherwise
    //!
    bool    useExistingProject( int index ) const { return mImportItemList.at(index).mUseExistingProject; }

    //!
    //! \brief import       Execute the file import operation using the registered functor
    //! \param index        Index of the registered import item
    //! \param parent       Parent widget for dialogs and error messages
    //! \param fileList     List of files to be imported
    //! \param project      Target project for the import operation
    //! \param usedExisting Flag indicating if an existing project was used
    //! \return             True if the import was successful, false otherwise
    //!
    bool    import( int index, QWidget *parent, const QStringList &fileList, SdProject *project, bool usedExisting )
      {
      return mImportItemList.at(index).mImportFunctor( parent, fileList, project, usedExisting );
      }

    //!
    //! \brief registerImport Add a new import type definition to the manager
    //! \param menuTitle      Title for the main menu item
    //! \param iconName       Name of the icon associated with the import
    //! \param dialogTitle    Title for the file selection dialog
    //! \param dialogFilter   File extensions filter for the dialog
    //! \param useExisting    When true we import into current existing project
    //! \param fun            Functor that performs the specific import logic
    //!
    void    registerImport( const QString &menuTitle, const QString &iconName, const QString &dialogTitle, const QString &dialogFilter, bool useExisting, SdImportFunction fun )
      {
      //Fill import item
      SdImportItem ii;
      ii.mMenuTitle          = menuTitle;
      ii.mIconTitle          = iconName;
      ii.mDialogTitle        = dialogTitle;
      ii.mDialogFilter       = dialogFilter;
      ii.mUseExistingProject = useExisting;
      ii.mImportFunctor      = fun;

      //Append import to list
      mImportItemList.append( ii );
      }
  };

#endif // SDIMPORTMANAGER_H
