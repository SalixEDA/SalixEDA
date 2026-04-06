/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Script language is C-style hi-level language for programming 3d models, 2d parametric graphics and
  schematic calculations.

  Embedded function.

  Script language not supported definition of functions. Available only embedded functions. This structure
  represents souch functions.
*/
#ifndef SDSCRIPTVALUEFUNCTION_H
#define SDSCRIPTVALUEFUNCTION_H

#include "SdScriptValue.h"

#define SDSCRIPT_FUN_MAX_PARAM 10 //!< Maximal parameters available for functions

class SdScriptValueFunction : public SdScriptValue
  {
    char mResultType;                         //!< Result type of function
    int  mParamCount;                         //!< Param count for function 0 - (SDSCRIPT_FUN_MAX_PARAM-1)
    char mParamTypes[SDSCRIPT_FUN_MAX_PARAM]; //!< Type of each param
  protected:
    SdScriptValuePtr mParamList[SDSCRIPT_FUN_MAX_PARAM];

    void             setParamTypes( const char *types, int count )
      {
      for( mParamCount = 0; mParamCount < count && mParamCount < SDSCRIPT_FUN_MAX_PARAM; ++mParamCount )
        mParamTypes[mParamCount] = types[mParamCount];
      }
  public:
    SdScriptValueFunction( char resultType, char paramType0 );
    SdScriptValueFunction( char resultType, char paramType0, char paramType1 );
    SdScriptValueFunction( char resultType, char paramType0, char paramType1, char paramType2 );
    SdScriptValueFunction( char resultType, char paramType0, char paramType1, char paramType2, char paramType3 );
    SdScriptValueFunction( char resultType, char paramType0, char paramType1, char paramType2, char paramType3, char paramType4 );
    SdScriptValueFunction( char resultType, char paramType0, char paramType1, char paramType2, char paramType3, char paramType4, char paramType5 );
    SdScriptValueFunction( char resultType, char paramType0, char paramType1, char paramType2, char paramType3, char paramType4, char paramType5, char paramType6 );
    SdScriptValueFunction( char resultType, char paramType0, char paramType1, char paramType2, char paramType3, char paramType4, char paramType5, char paramType6, char paramType7 );
    SdScriptValueFunction( char resultType ) :
      mResultType(resultType),
      mParamCount( 0 )
      {
      clearParamList();
      }
    ~SdScriptValueFunction();

    SdScriptValuePtr param(int i) const { return mParamList[i]; }

    template<typename T>
    T paramAs(int i) const;

    //!
    //! \brief paramSet Set param for function
    //! \param index    Index of param
    //! \param param    Param value ptr
    //!
    void paramSet( int index, SdScriptValuePtr param ) { mParamList[index] = param; }

    //!
    //! \brief paramCount Return function param count
    //! \return           Function param count
    //!
    int  paramCount() const { return mParamCount; }

    //!
    //! \brief paramType Return param type
    //! \param index     Index of param
    //! \return          Type of param with index
    //!
    char paramType( int index ) const { return mParamTypes[index]; }


    // SdM3dValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char type() const override { return mResultType; }

  private:
    void clearParamList();
  };

template<> inline float         SdScriptValueFunction::paramAs<float>(int i) const { return mParamList[i]->toFloat(); }

template<> inline bool          SdScriptValueFunction::paramAs<bool>(int i) const { return mParamList[i]->toBool(); }

template<> inline QColor        SdScriptValueFunction::paramAs<QColor>(int i) const { return mParamList[i]->toColor(); }

template<> inline QMatrix4x4    SdScriptValueFunction::paramAs<QMatrix4x4>(int i) const { return mParamList[i]->toMatrix(); }

template<> inline Sd3drFace     SdScriptValueFunction::paramAs<Sd3drFace>(int i) const { return mParamList[i]->toFace(); }

template<> inline Sd3drFaceList SdScriptValueFunction::paramAs<Sd3drFaceList>(int i) const { return mParamList[i]->toFaceList(); }

template<> inline Sd2dRegion    SdScriptValueFunction::paramAs<Sd2dRegion>(int i) const { return mParamList[i]->to2dRegion(); }

template<> inline QList<float>  SdScriptValueFunction::paramAs<QList<float> >(int i) const { return mParamList[i]->toFloatList(); }

#endif // SDSCRIPTVALUEFUNCTION_H
