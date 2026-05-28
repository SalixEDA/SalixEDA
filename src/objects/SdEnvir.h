/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Work environment for individual user
*/

#ifndef SDENVIR_H
#define SDENVIR_H

#include "SdConfig.h"
#include "SdLayer.h"
#include "SdLayerCache.h"
#include "SdSnapInfo.h"
#include "SdUtil.h"
#include "SdPadAssociation.h"
#include "SdRuleBlock.h"
#include "SvLib/SvSingleton.h"

#include <QColor>
#include <QMap>
#include <QPointF>
#include <QList>
#include <functional>


#define scPureBlack     -2 //Pure black color [Чисто черный цвет]
#define scUnvisible     -1 //Always invisible elements [Всегда невидимые элементы]
#define scLocked         0 //Color for locked elements [Цвет заблокированных элементов]
#define scSelected       1 //Color for selected elements [Цвет выбранных элементов]
#define scEnter          2 //Enter color [Цвет ввода]
#define scSmart          3 //Smart mark color [Цвет разумного режима]
#define scGraphBack      4 //Graphics editor background color [Цвет фона графического редактора]
#define scRatNet         5 //Rat net color [Цвет резинок]
#define scCursor         6 //Cursor color [Цвет курсора]
#define scTraseNet       7 //Color for currently traced net pads [Цвет трассируемой цепи]
#define scGrid           8 //Grid color [Цвет сетки]
#define scRuleErrors     9 //Rule errors color
#define scCatchPoint    10 //Catch point color in road enter mode. Catch point is point nearest current cursor postion for best next enter.
#define sc3dPadTop      11 //Color of top pad
#define sc3dPadBot      12 //Color of bottom pad
#define sc3dPadHole     13 //Color of pad holes
#define sc3dPcb         14 //Color of pcb
#define scLast          15


//Вид перекрестья курсора
#define dcvNone          0  //Cursor is invisible [Курсор не виден]
#define dcvSmall         1  //Small cursor crosshair [Малый курсор]
#define dcvFull          2  //Crosshair to fill view [Полный курсор]
#define dcvSmall45       3  //Small cursor crosshair with angle 45 [Малый под углом 45]
#define dcvFull45        4  //Crosshair to fill view with angle 45 [Полный под углом 45]
#define dcvLast          5

//Версия SdEnvir
#define SdEnvirVersion  (44 + FONT_COUNT)


class SdEnvir
  {
    QColor            mSysColors[scLast];    //!< System color table
    QString           mFonts[FONT_COUNT];    //!< System font table
    SdLayerPtrMap     mLayerTable;           //!< Layers table [Таблица слоев]
    double            mSchPPM;               //!< Conversion factor to physical quantity in the schematic editor [Коэффициент преобразования в физическую величину в схемном редакторе]
    double            mPrtPPM;               //!< Conversion factor to physical quantity in the pcb editor [Коэффициент преобразования в физическую величину в конструкциях]

    QList<QPointF>    mGridHistory;          //!< Previous grid history
    SdRuleBlock       mDefaultRules;         //!< Default rules for pcb

    //Not saved
    //Cashed layers for stratum
    SdLayerCache      mCacheForPad;
    SdLayerCache      mCacheForMask;
    SdLayerCache      mCacheForStencil;
    SdLayerCache      mCacheForHole;
    SdLayerCache      mCacheForRoad;
    SdLayerCache      mCacheForPolygon;
    SdLayerCache      mCacheForBoundary;
    SdLayerCache      mCacheForKeepout;
    SdPadAssociation *mPadStack;

    SdEnvir();
  public:
    ~SdEnvir();

    SV_SINGLETON( SdEnvir )


    int             mDotSize;              //!< Size of the connection point of circuit segments [Размер точки соединений сегментов цепи]
    int             mDotWidth;             //!< Line thickness of the connection point of circuit segments [Толщина линии точки соединения сегментов цепи]
    int             mSymPinSize;           //!< Size of the symbol pin crosshair [Размер перекрестья ножки символа]
    int             mPartPinSize;          //!< Size of the part pin circle [Размер окружности ножки корпуса]
    int             mSmartSize;            //!< Size of the smart point [Размер разумной точки]
    int             mSmartWidth;           //!< Line thickness of the smart point [Толщина линии разумной точки]
    bool            mIsSmart;              //!< Enable smart mode [Включение разумного режима]
    bool            mIsWireSmart;          //!< Second level of smart mode for nets in schematic [Второй уровень разумного режима для цепей в схеме]
    SdSnapMask      mSmartMask;            //!< Mask of allowed smart point search types [Маска разрешенных для поиска разумных точек]
    bool            mNetUnionOn;           //!< Show union window every time [Выводить окно объединения каждый раз]
    int             mNetUnionMode;         //!< Net union mode 0-none, 1-XN, 2-all [Режим объединения цепей 0-нет, 1-XN, 2-все]
    bool            mEnableComp;           //!< Allow or block component operations [Разрешить операции с компонентами или заблокировать]
    bool            mEnableNet;            //!< Allow net operations [Разрешить операции с цепями]
    bool            mEnablePic;            //!< Allow picture operations [Разрешить операции с картинкой]
    QString         mViaType;              //!< Default via type [Тип переходного отверстия по умолчанию]
    bool            mShowRatNet;           //!< Show rubber bands [Показывать резинки]
    bool            mShowRemark;           //!< Show tooltips over components [Показывать всплывающие подсказки над компонентами]
    bool            mShowMessageRemark;    //!< Show information in status bar [Показывать информацию в строке состояния]
    int             mPlaceMode;            //!< Component placement mode [Режим размещения компонентов]
    int             mMinViewGrid;          //!< Minimum visible grid size [Минимальный видимый размер сетки]
    int             mUndoSize;             //!< Undo buffer size [Размер буфера отмены]
    int             mCursorView;           //!< Cursor crosshair type [Вид перекрестья курсора]
    int             mSmallCursorSize;      //!< Small cursor size in pixels [Размер малого курсора в пикселах]
    bool            mCursorShow;           //!< Show cursor [Показывать курсор]
    int             mTraseDotSize;         //!< Size of the point showing the net [Размер точки, показывающей цепь]
                                           //!< PPM shows how much physical quantity corresponds to one logical unit [PPM показывает сколько физической величины приходится на одну логическую единицу]
    bool            mGridSyncXY;           //!< Synchronized edition x and grid steps
    bool            mGridShow;             //!< Enable grid [Включение сетки]
    bool            mCursorAlignGrid;      //!< Enable cursor movement by grid [Включение движения курсора по сетке]
    bool            mCenterCursor;         //!< Center cursor when zooming in and out [Центровать курсор при увеличении и уменьшении]

    bool            mShowRuleErrors;       //!< If true then over pcb shows rule error indicators as rectangles
    bool            mShowFields;           //!< If true then draw fields as fields names else draw fields as values
    bool            mShowPads;             //!< If true then draw pads in part editor
    double          mPolygonOpacity;       //!< Polygons draws with this opacity
    bool            mAutoRemoveRoadLoop;   //!< Automatic detect and remove loops when enter roads

    bool            mGuiderEnabled;        //!< Flag to enable/disable the guide [Флаг разрешения/запрещения путеводителя]
    int             mGuiderPosition;       //!< Position of the guide splitter [Положение разделителя путеводителя]
    bool            mProjectEnabled;       //!< Flag to enable/disable the project [Флаг разрешения/запрещения проекта]
    int             mProjectPosition;      //!< Position of the project splitter [Позиция разделителя проекта]
    bool            mProjectRemoveEnabled; //!< Allow automatic project disabling [Разрешение автоматического запрещения проекта]
    int             mProjectRemoveTime;    //!< Project hold time [Время удержания проекта]
    double          mWidthStep;            //!< Width change step [Шаг изменения толщины]
    double          mTextSizeStep;         //!< Text height change step [Шаг изменения высоты текста]
    bool            mCreateBack;           //!< Create BAK file when saving [Создавать BAK файл при сохранении]

    //This path depresate
    QString mPatternPath;

    //!
    //! \brief getSysColor Returns system color by its id
    //! \param colorId     Id of color
    //! \return            System color
    //!
    QColor   getSysColor( int colorId );

    //!
    //! \brief setSysColor Sets system color
    //! \param colorId     Id of system color
    //! \param color       New color value
    //!
    void     setSysColor( int colorId, QColor color );



    //!
    //! \brief getSysFont Returns name of system font by its id
    //! \param fontId     System font id
    //! \return           Name of system font
    //!
    QString  getSysFont( int fontId );

    //!
    //! \brief setSysFont Setup new name of system font
    //! \param fontId     System font id whous name is need to setup
    //! \param fontName   New font name
    //!
    void     setSysFont( int fontId, const QString fontName );

    //!
    //! \brief ppmGet  Returns ppm in accordance classId
    //! \param classId Class of object for which editor return ppm
    //! \return        ppm
    //!
    double   ppmGet( SdClass classId ) const;



    //!
    //! \brief loadEnvir Loads environment from user settings
    //!
    void     loadEnvir();

    //!
    //! \brief saveEnvir Saves environment to user settings
    //!
    void     saveEnvir();

    //!
    //! \brief defaultEnvir Creates environment with default values
    //!
    void     defaultEnvir();



    //!
    //! \brief layerGet Get existing layer, if it is not exist - then it's created as default
    //! \param id       Id of needed layer
    //! \return         Layer pointer
    //!
    SdLayer *layerGet( QString id );

    /*[] (SdLayer *layer) -> bool {

       }
       */
    //!
    //! \brief layerForEach - Iterates through all registered layers matching the specified class
    //! \param classMask   - Layer class filter (dctConstruct, dctSchematic, dctCommon or combination)
    //! \param fun1        - Callback function for each layer object. Return true to continue iteration, false to stop
    //!
    void     layerForEach(quint64 classMask, std::function<bool (SdLayer *)> fun1 );
    void     layerForEachConst( quint64 classMask, std::function<bool(SdLayer*)> fun1 ) const;

    //!
    //! \brief layerRemove - Removes a layer by its identifier
    //! \param layerId     - Unique identifier of the layer to remove
    //!
    void     layerRemove( const QString &layerId ) { if( mLayerTable.contains(layerId) ) { delete mLayerTable[layerId]; mLayerTable.remove( layerId ); } }

    //!
    //! \brief layerCount - Returns the total number of registered layers
    //! \return           - Number of layers currently in the layer table
    //!
    int      layerCount() const { return mLayerTable.count(); }

    //!
    //! \brief layerVisibleSet  Sets visibility for multiple layers
    //! \param layerIdTable     List of layer identifiers to set visible (others become invisible)
    //! \param editLayerIdTable List of layer identifiers to set editable
    //!
    void     layerVisibleSet( const QStringList &layerIdTable, const QStringList &editLayerIdTable = QStringList{} );

    //!
    //! \brief layerVisibleGet  Gets visibility for multiple layers
    //! \param layerIdTable     List of layer identifiers for visible
    //! \param editLayerIdTable List of layer identifiers for editable
    //!
    void     layerVisibleGet( QStringList &layerIdTable, QStringList &editLayerIdTable ) const;

    //!
    //! \brief gridForEach Executes function fun1 to index of grid table
    //!                    If intex < 0 then for all table
    //! \param index       Index of row for which need to be executed fun1, if < 0 then for all rows
    //! \param fun1        Function to execute
    //!
    void     gridForEach( int index, std::function<bool (QPointF)> fun1 ) const;

    //!
    //! \brief gridAppend Appends new record to grid table
    //! \param p          Record to append
    //!
    void     gridAppend( QPointF p );

    //Clear stratum layer association cashe
    void     resetForCache();

    SdLayer *layerVisibleForPad( SdPvStratum stratum )      { return mCacheForPad.getVisibleLayer( stratum ); }
    SdLayer *layerVisibleForMask( SdPvStratum stratum )     { return mCacheForMask.getVisibleLayer( stratum ); }
    SdLayer *layerVisibleForPolygon( SdPvStratum stratum )  { return mCacheForPolygon.getVisibleLayer( stratum ); }
    SdLayer *layerVisibleForStencil( SdPvStratum stratum )  { return mCacheForStencil.getVisibleLayer( stratum ); }
    SdLayer *layerVisibleForHole( SdPvStratum stratum )     { return mCacheForHole.getVisibleLayer( stratum ); }
    SdLayer *layerVisibleForRoad( SdPvStratum stratum )     { return mCacheForRoad.getVisibleLayer( stratum ); }
    SdLayer *layerVisibleForBoundary( SdPvStratum stratum ) { return mCacheForBoundary.getVisibleLayer( stratum ); }
    SdLayer *layerVisibleForKeepout( SdPvStratum stratum )  { return mCacheForKeepout.getVisibleLayer( stratum ); }

    //!
    //! \brief layerSetPair - Sets a pair of layers (top and bottom) for operations
    //! \param idTop        - Identifier of the top layer
    //! \param idBot        - Identifier of the bottom layer
    //!
    void     layerSetPair( QString idTop, QString idBot );

    //!
    //! \brief resetLayerUsage - Resets the "usage" flag for all layers
    //!
    void     resetLayerUsage();

    //!
    //! \brief setLayerUsage - Sets the "usage" flag for stratum layers from caches
    //! \param stratumCount  - Number of stratum layers to mark as used
    //!
    void     setLayerUsage( int stratumCount );

    //!
    //! \brief toPhisSchematic - Converts a logical value to a physical string in schematic units
    //! \param val            - Logical value to convert
    //! \return               - Physical string representation (e.g., "1.5 mm")
    //!
    QString  toPhisSchematic( int val ) const { return SdUtil::log2physStr(val,mSchPPM); }

    //!
    //! \brief fromPhisSchematic - Converts a physical string to a logical value in schematic units
    //! \param str              - Physical string to convert (e.g., "1.5 mm")
    //! \return                 - Logical integer value
    //!
    int      fromPhisSchematic( const QString str ) const { return SdUtil::phys2log(str,mSchPPM); }

    //!
    //! \brief toPhisPcb - Converts a logical value to a physical string in PCB units
    //! \param val       - Logical value to convert
    //! \return          - Physical string representation (e.g., "2.54 mm")
    //!
    QString  toPhisPcb( int val ) const { return SdUtil::log2physStr(val,mPrtPPM); }

    //!
    //! \brief fromPhisPcb - Converts a physical string to a logical value in PCB units
    //! \param str         - Physical string to convert (e.g., "2.54 mm")
    //! \return            - Logical integer value
    //!
    int      fromPhisPcb( const QString str ) const { return SdUtil::phys2log(str,mPrtPPM); }

    //!
    //! \brief getPad - Retrieves a pad by its pin type
    //! \param pinType - Type identifier of the pin (e.g., "smd", "thru")
    //! \return        - SdPad object corresponding to the specified pin type
    //!
    SdPad    getPad( const QString pinType );

    //!
    //! \brief resetPads - Resets all pad definitions to their default values
    //!
    void     resetPads();

    //!
    //! \brief defaultRules - Returns the default design rule block
    //! \return             - SdRuleBlock containing default clearance, width, etc.
    //!
    SdRuleBlock defaultRules() const { return mDefaultRules; }

    //Return current language settings
    static   QString languageGet();
  private:
    //!
    //! \brief deleteLayers - Deletes all layers and clears the layer table
    //!
    void deleteLayers();

    //!
    //! \brief addLayer - Adds a layer object to the layer management system
    //! \param layer    - Pointer to the SdLayer object to add
    //!
    void addLayer( SdLayer *layer );

    //!
    //! \brief addLayerId - Adds a layer identifier with description
    //! \param descr      - Layer descriptor containing layer properties
    //!
    void addLayerId(const SdLayerDescr &descr);

    //!
    //! \brief addLayerId - Adds a complete layer definition
    //! \param layerId    - Unique layer identifier
    //! \param name       - Native language name of the layer
    //! \param englishName - English name of the layer
    //! \param descr      - Layer descriptor with additional properties
    //! \param layerIndex - Index position for the layer
    //!
    void addLayerId(const QString &layerId, const QString &name, const QString &englishName, const SdLayerDescr &descr , int layerIndex);

    //!
    //! \brief layerSetPairFor - Sets a layer pair for a specific layer
    //! \param lid0           - Identifier of the reference layer to set pair for
    //!
    void layerSetPairFor( const QString &lid0 );
  };





#endif // SDENVIR_H
