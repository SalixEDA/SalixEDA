/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  SalixEDA configuration and history

History
  01.01.2026 v3.00 Rebrending from SaliCAD
  03.03.2026 v3.1  Fix: crash in registration form. Not available to deal with ui from separate thread
  13.03.2026 v3.2  Append visual object group id, which allow to group many visual objects into single object
  27.03.2026 v3.3  Completely redesigned guider. Works with simple text scripts. Produces standard mp4 video.
  08.04.2026 v3.4  Append board builder master
  06.05.2026 v3.5  Append base for textual net connection list
*/

#ifndef SDCONFIG
#define SDCONFIG

//Do'nt change this name
#define SD_AUTHOR                    "Alexander Sibilev"
#define SD_NAME                      "SalixEDA"

//Version definition
#define SD_VERSION_MAJOR             3
#define SD_VERSION_MINOR             5

//Some defaults
#define SD_DEFAULT_WEB               "www.SalixEDA.org"
#define SD_UPGRADE_WEB               "http:/SalixEDA.org/downloads/"


//When defined disable registration with sali name
#define SD_DISABLE_SALI_AUTHOR

//Project naming definition
#define SD_BASE_EXTENSION            ".salixeda-ex"
#define SD_DEFAULT_FILE_NAME         "default"

//Binary project naming definition
#define SD_BINARY_EXTENSION          ".salixeda"
#define SD_BINARY_EXTENSION_LENGTH   9

//SaliCAD compatibility
#define SD_SALICAD_EXTENSION         ".salicad"
#define SD_SALICAD_BIN_EXTENSION     ".salicadx"

//Layer list naming definition
#define SD_LAYER_LIST_EXTENSION      ".layersSet"

//Pads association naming definition
#define SD_PAD_ASSOCIATION_EXTENSION ".salixpads"

//Export to assembler file extension
#define SD_ASSEMBLER_EXTENSION       ".salixasm"

//Gerber file extension
#define SD_GERBER_EXTENSION          ".gbr"

//SalixEDA database version
//Must be compatible with previous versions
#define SD_BASE_VERSION_1            1
#define SD_BASE_VERSION_2            2
#define SD_BASE_VERSION_3            3

//Current SalixEDA database version
#define SD_BASE_VERSION              SD_BASE_VERSION_3

//SalixEDA json file type description
#define SD_BASE_TYPE                 "SalixEDA file"


//Previous file count in menu
#define PREVIOUS_FILES_COUNT         10

//Grid history count
//User can enter any grid size, but only GRID_HISTORY_SIZE grid sizes save in system for fast selection
#define GRID_HISTORY_SIZE            12

//Maximum open editors
//When open editor which index more than this maximum
// then last resently editor removed
#define SD_MAX_EDITORS               10

//Maximum font count
//In project fonts referenced by index in system font table
//This count define size of this table. So only FONT_COUNT different
//fonts may be used in project.
#define FONT_COUNT                   5

//Undo history count
#define UNDO_HISTORY_SIZE            1000

//Clipboard objects format
#define SD_CLIP_FORMAT_PITEM         "bin/salixeda-SdProjectItem"    //Partial salicad SdProjectItem
#define SD_CLIP_FORMAT_SELECTOR      "bin/salixeda-SdSelector"       //Project and selection SdSelector

//Size of picture to draw fragment to clipboard
#define CLIP_IMAGE_WIDTH             (1920*2)
#define CLIP_IMAGE_HEIGHT            (1280*2)

//Max default delivered limit elements
#define SD_DEFAULT_DELIVERED_LIMIT   100

//Max object in find list of GetObject dialog
#define SD_GET_OBJECT_MAX_FIND_LIST  300

//Default IP for global storage and private cloud
#define SD_DEFAULT_GLOBAL_STORAGE_IP "178.212.13.179"
#define SD_DEFAULT_PRIVATE_CLOUD_IP  "178.212.13.179"

//Settings key names
#define SDK_WMAIN_MAX                "WMainMax"
#define SDK_LANGUAGE                 "Language"
#define SDK_MAIN_SPLITTER            "MainSplitter"
#define SDK_PREVIOUS_FILES           "PreviousFiles"
#define SDK_PRIVATE_KEY              "PrivateKey"
#define SDK_LOCAL_LIBRARY_PATH       "LocalLibraryPath"
#define SDK_GLOBAL_STORAGE_IP        "GlobalStorageIp"
#define SDK_PRIVATE_CLOUD_IP         "PrivateCloudIp"
#define SDK_PRIVATE_CLOUD_NAME       "PrivateCloudName"
#define SDK_ENVIR_VERSION            "Envir version"
#define SDK_ENVIR                    "Envir"
#define SDK_GET_OBJECT_DLG_CFG       "DlgGetObjectCfg2_8"
#define SDK_PROP                     "Prop"
#define SDK_PROP_VERSION             "Prop version"


#define MODE_HELP                    //Prefix to mode help files

#define MAX64_MASK                   0xffffffffffffffffL
#define MAX32_MASK                   0xffffffff

#define defSymScale                  20.0    //Default scale for schematic and symbol
#define defPrtScale                  200.0   //Default scale for PCB and part
#define minScale                     0.0001  //Minimum scale
#define maxScale                     10000.0 //Maximum scale
#define maxBusNumber                 1000    //Max net number in bus when enter bus mode
#define pcb3dZLevel                  -0.05   //Vertical offset of pcb
#define pcb3dThickness               1.2

#define defNetNamePrefix             "XN"    //Default net name prefix

//Standard param names
#define stdParamBom                  QStringLiteral("bom")
#define stdParamArticle              QStringLiteral("article")
#define stdParamTitle                QStringLiteral("title")
#define stdParamValue                QStringLiteral("value")
#define stdParamValueMin             QStringLiteral("valueMin")
#define stdParamValueMax             QStringLiteral("valueMax")
#define stdParamValueRow             QStringLiteral("valueRow")
#define stdParamPrefix               QStringLiteral("prefix")
#define stdParamValueSelector        QStringLiteral("valueSelector")
#define stdParamManufacturer         QStringLiteral("manufacturer")
#define stdParamTolerance            QStringLiteral("tolerance")
#define stdParamDatasheet            QStringLiteral("datasheet")
#define stdParamWww                  QStringLiteral("www")
#define stdParamLogNumber            QStringLiteral("logNumber")
#define stdParamItemId               QStringLiteral("itemId")
#define stdParamItemIdList           QStringLiteral("itemIdList")
#define stdParamCompCount            QStringLiteral("compCount")
#define stdParamTotalPageCount       QStringLiteral("totalPageCount")
#define stdParamPageIndex            QStringLiteral("pageIndex")
#define stdParamDeviceMark           QStringLiteral("device mark")
//Capacitor specific
#define stdParamDielectric           QStringLiteral("dielectric")
#define stdParamVoltage              QStringLiteral("voltage")
//Diod specific
#define stdParamForvardVoltage       QStringLiteral("forward voltage")
#define stdParamCurent               QStringLiteral("current")
#define stdParamReverceVoltage       QStringLiteral("reverce voltage")
//LED specific
#define stdParamAngle                QStringLiteral("angle")
#define stdParamBrightness           QStringLiteral("brightness")
#define stdParamColor                QStringLiteral("color")
#define stdParamWavelength           QStringLiteral("wavelength")
//Inductor specific
#define stdParamSatCurrent           QStringLiteral("saturation current")
#define stdParamResistance           QStringLiteral("resistance")
#define stdParamSRF                  QStringLiteral("SRF")
//MOSFET specific
#define stdParamId                   QStringLiteral("Id")
#define stdParamPd                   QStringLiteral("Pd")
#define stdParamRdsOn                QStringLiteral("Rds on")
#define stdParamVdss                 QStringLiteral("Vdss")
#define stdParamVgs                  QStringLiteral("Vgs")
//Transistor specific
#define stdParamIc                   QStringLiteral("Ic")
#define stdParamVcbo                 QStringLiteral("Vcbo")
#define stdParamVceo                 QStringLiteral("Vceo")
#define stdParamHFE                  QStringLiteral("hFE")
//3d model programm
#define stdParam3dModelProgramm      QStringLiteral("3dModelProgramm")
#define stdParam3dModelDescription   QStringLiteral("3dModelDescription")

#endif // SDCONFIG

