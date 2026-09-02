/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Base mode for operation on objects.
*/

#ifndef SDMODE_H
#define SDMODE_H

#include "objects/SdProjectItem.h"
#include "objects/SdContext.h"
#include "SdCursorFactory.h"
#include "SdModeIdents.h"

#include <QChar>
#include <QTransform>
#include <QImage>


class SdWEditorGraph;

//Base class for mode
//Each changes to project made through modes. With modes we append new objects, edit them and delete them.
//Specific operation depends on mode.
//Graphic editor - is source of events and display server
//All events which receiv editor it sends to active mode. Mode decides how to handle this event.
//Not all event must be handle. Not handled events processed by default with this base class.
//Display process consist of two etaps: static draw and dynamic draw. For example, when we enter line
// all currently existing graphics are static draw and dynamic draw is only entered line. This do for
// accelerate paint process because not need to redraw static elements every time as changed only small count of elements.
//For this mech mode has two memebers drawStatic and drawDynamic. Both with default implementation.
//By default drawStatic draws all graphics of container. By default drawDynamic draws nothing.
class SdMode
  {
    int             mStep;       //!< Mode step
  protected:
    SdProjectItem  *mObject;     //!< Object is mode working on
    SdWEditorGraph *mEditor;     //!< Editor window is mode working in
    SdUndo         *mUndo;       //!< Undo for this project

   //Исполняемые через обозреватель
   void           update();          //Обновить изображение
   void           cancelMode();      //Прекращение режима
   SdPoint        getGrid() const;
   double         getPPM() const;

   //Service
   //Set step to index
   void           setStep( int stp );

   //Get current step. Return current step index
   int            getStep() const { return mStep; }

   //Declare that object is changed and project must be save
   void           setDirty();

   //Declare that static draw must be redrawed
   void           setDirtyCashe();

   //If container is SdPlate object then set dirty rat net for its rebuild
   void           setDirtyRatNet();

   //Add object to container
   void           addPic(SdObject *obj , QString title);

   //!
   //! \brief setStatusMessage Sets message to status bar
   //! \param msg              Message to set
   //!
   void           setStatusMessage( const QString &msg ) const;

   //!
   //! \brief setStepStatusMessage Sets step help message to status bar
   //!
   void           setStepStatusMessage() const { setStatusMessage( getStepHelp() ); }

   //!
   //! \brief setCursor Sets editor current cursor
   //! \param cursor    Cursor id
   //!
   void           setCursor( int cursor ) const;

   //!
   //! \brief setStepCursor Sets step cursor as current editor cursor
   //!
   void           setStepCursor() const { return setCursor( getCursor() ); }

  public:
    SdMode( SdWEditorGraph *editor, SdProjectItem *obj );
    virtual ~SdMode();

            void    restore();                              //Обновляет все, что касается текущего состояния режима

    virtual void    activate();                             //Вызывается первой, после того как режим делается текущим
    virtual void    deactivate();                           //Calling before mode set to inactive state
    virtual void    reset();                                //Сбрасывает режим в исходное состояние

    virtual void    drawStatic( SdContext *ctx );           //Рисует постоянную часть картинки
    virtual void    drawDynamic( SdContext *ctx );          //Рисует переменную часть картинки

    virtual int     getPropBarId() const { return -1; }     //Получить идентификатор панели свойств
    virtual void    propGetFromBar() {}                     //Извещение об изменении свойств
    virtual void    propSetToBar() {}                       //Установить свойства в активный бар

    virtual void    enterPoint( SdPoint ) {}                //Ввод точки (левая кнопка)
    virtual void    clickPoint( SdPoint ) {}                //Двойное нажатие левой кнопки

    //!
    //! \brief cancelPoint Process point rejection/cancellation (right mouse button click) [Точка прекращения (правая кнопка)]
    //! \param p           Point coordinates at cancellation moment
    //!
    virtual void    cancelPoint( SdPoint ) {}
    virtual void    movePoint( SdPoint ) {}                 //Перемещение точки
    virtual bool    wheel( SdPoint ) { return false; }      //Вращение колеса мыши
    virtual void    keyDown( int key, QChar ch );           //Нажатие кнопки
    virtual void    keyUp( int key, QChar ch );             //Отпускание клавиши
    virtual SdPoint enterPrev() { return SdPoint(); }       //Повторение ввода предыдущего сеанса (разумный ввод)
    virtual void    beginDrag( SdPoint ) {}                 //Начало перетаскивания
    virtual void    dragPoint( SdPoint ) {}                 //Перетаскивание
    virtual void    stopDrag( SdPoint ) {}                  //Конец перетаскивания

    //!
    //! \brief showMenu Shows context menu at point
    //!
    virtual QMenu  *contextMenu() const { return nullptr; }

    //!
    //! \brief contextCommand Execute context menu command
    //!
    virtual void    contextCommand( int ) {}

    virtual bool    enableCopy() const;                     //Режим имеет объекты для копирования
    virtual bool    enablePaste( quint64 pasteMask ) const; //Режим разрешает вставку объектов с заданной маской

    virtual bool    getInfo( SdPoint p, QString &info );    //Получить всплывающую информацию в заданной точке

    virtual QString getStepHelp() const = 0;                //Краткая помощь по текущему шагу режима
    virtual QString getModeThema() const = 0;               //Help page address Адрес страницы помощи
    virtual QString getStepThema() const = 0;               //Help page address and link for current step Адрес страницы помощи для текущего шага
    virtual int     getCursor() const = 0;                  //Номер курсора для режима
    virtual int     getIndex() const = 0;                   //Номер режима в списке режимов

    virtual void    partSelect( QStringList ) {}            //Select parts by list

  };

#endif // SDMODE_H
