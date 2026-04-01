/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Open project list widget
*/

#ifndef SDWPROJECTLIST_H
#define SDWPROJECTLIST_H

#include "objects/SdProject.h"
#include <QWidget>
#include <QList>
#include <QStackedWidget>
#include <QComboBox>
#include <QToolButton>

class SdWProjectTree;

class SdWProjectList : public QWidget
  {
    Q_OBJECT

    QStackedWidget   *mWProjectStack; //Изображение деревьев открытых проектов
    QComboBox        *mProjectTitles; //Список открытых проектов
    QToolButton      *mCloseProject;  //Кнопка закрытия активного проекта
  public:
    explicit SdWProjectList(QWidget *parent = nullptr);

    bool isEmpty() { return mWProjectStack->count() == 0; }

    SdWProjectTree* activeProject();
    SdWProjectTree* project( int index );
    SdProject*      projectByName( const QString &title ) const;

  signals:
    //Name of active project changed or other project selected
    void projectNameChanged( SdProject *project );

  public slots:
    void            snapshotSave( const QString &path, int index, QStringList &list, int &current );

    void            snapshotLoad( const QString &path, const QStringList &list, int current );


    //Close all projects
    void            fileCloseAll();

    //!
    //! \brief fileOpen      Can load project from local file, create new empty project, or open given project
    //! \param fname         File name for project
    //! \param project       Project for open or nullptr if need to be created new project or project mast be loaded from file
    //! \return              Tree window for project
    //!
    SdWProjectTree *fileOpen(const QString fname , SdProject *project = nullptr);

    //File import PasCAD
    void            fileImportPis( const QString fname );

    //Active file close
    bool            cmFileClose();

    //On project index activated
    void            onProjectActivated( int index );

    //On rename project
    void            onRenameProject(SdProject *prj , const QString shortName);

    //On activate item. We brings item's project up
    void            onItemActivated( SdProjectItem *item );

    //On project highlighted
    void            onProjectHighlighted(SdProject *project );

  private:
    //Bring project up
    void            bringProjectUp( int index );
  };

#endif // SDWPROJECTLIST_H
