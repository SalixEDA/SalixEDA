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

  Collection of solidXXX functions
*/
#ifndef SDSCRIPTVALUEFUNSOLIDXXX_H
#define SDSCRIPTVALUEFUNSOLIDXXX_H

#include "SdScriptValueFunction.h"


template <typename T>
struct SdScriptMethodTraits;

template <typename R, typename... Args>
struct SdScriptMethodTraits<R(Sd3drModel::*)(Args...)>
  {
  using Result = R;
  using ArgsTuple = std::tuple<Args...>;
  static constexpr int ArgCount = sizeof ...(Args);
  };

template <typename T>
struct SdScriptTypeMap;

//float
template<>
struct SdScriptTypeMap<float>
  {
    static constexpr char type = SD_SCRIPT_TYPE_FLOAT;
    static float get(const SdScriptValue *v) { return v->toFloat(); }
  };

//bool
template<>
struct SdScriptTypeMap<bool>
  {
    static constexpr char type = SD_SCRIPT_TYPE_BOOL;
    static bool get(const SdScriptValue *v) { return v->toBool(); }
  };

//QColor
template<>
struct SdScriptTypeMap<QColor>
  {
    static constexpr char type = SD_SCRIPT_TYPE_COLOR;
    static QColor get(const SdScriptValue *v) { return v->toColor(); }
  };

//QMatrix4x4
template<>
struct SdScriptTypeMap<QMatrix4x4>
  {
    static constexpr char type = SD_SCRIPT_TYPE_MATRIX;
    static QMatrix4x4 get(const SdScriptValue *v) { return v->toMatrix(); }
  };

//const QMatrix4x4&
template<>
struct SdScriptTypeMap<const QMatrix4x4&> :
    SdScriptTypeMap<QMatrix4x4> {};



//Sd3drFace
template<>
struct SdScriptTypeMap<Sd3drFace>
  {
    static constexpr char type = SD_SCRIPT_TYPE_FACE;
    static Sd3drFace get(const SdScriptValue *v) { return v->toFace(); }
  };

//const Sd3drFace&
template<>
struct SdScriptTypeMap<const Sd3drFace&> :
    SdScriptTypeMap<Sd3drFace> {};

//Sd3drFaceList
template<>
struct SdScriptTypeMap<Sd3drFaceList>
  {
    static constexpr char type = SD_SCRIPT_TYPE_FACE_LIST;
    static Sd3drFaceList get(const SdScriptValue *v) { return v->toFaceList(); }
  };

//const Sd3drFaceList&
template<>
struct SdScriptTypeMap<const Sd3drFaceList&> :
    SdScriptTypeMap<Sd3drFaceList> {};

//Sd2dRegion
template<>
struct SdScriptTypeMap<Sd2dRegion>
  {
    static constexpr char type = SD_SCRIPT_TYPE_2D_REGION;
    static Sd2dRegion get(const SdScriptValue *v) { return v->to2dRegion(); }
  };

//const Sd2dRegion&
template<>
struct SdScriptTypeMap<const Sd2dRegion&> :
    SdScriptTypeMap<Sd2dRegion> {};


//QList<float>
template<>
struct SdScriptTypeMap<QList<float> >
  {
    static constexpr char type = SD_SCRIPT_TYPE_AFLOAT;
    static QList<float> get(const SdScriptValue *v) { return v->toFloatList(); }
  };

//const QList<float>&
template<>
struct SdScriptTypeMap<const QList<float>&> :
    SdScriptTypeMap<QList<float> > {};





template <typename T>
struct SdScriptResultInvoker;

//Sd3drFace
template<>
struct SdScriptResultInvoker<Sd3drFace>
  {
    static constexpr char type = SD_SCRIPT_TYPE_FACE;

    template <typename F>
    static Sd3drFace call( F&& f ) { return f(); }
  };

//Sd3drFaceList
template<>
struct SdScriptResultInvoker<Sd3drFaceList>
  {
    static constexpr char type = SD_SCRIPT_TYPE_FACE_LIST;

    template <typename F>
    static Sd3drFaceList call( F&& f ) { return f(); }
  };

//QMatrix4x4
template<>
struct SdScriptResultInvoker<QMatrix4x4>
  {
    static constexpr char type = SD_SCRIPT_TYPE_MATRIX;

    template <typename F>
    static QMatrix4x4 call( F&& f ) { return f(); }
  };

//Sd2dRegion
template<>
struct SdScriptResultInvoker<Sd2dRegion>
  {
    static constexpr char type = SD_SCRIPT_TYPE_2D_REGION;

    template <typename F>
    static Sd2dRegion call( F&& f ) { return f(); }
  };

//QColor
template<>
struct SdScriptResultInvoker<QColor>
  {
    static constexpr char type = SD_SCRIPT_TYPE_COLOR;

    template <typename F>
    static QColor call( F&& f ) { return f(); }
  };


//QList<float>
template<>
struct SdScriptResultInvoker<QList<float> >
  {
    static constexpr char type = SD_SCRIPT_TYPE_AFLOAT;

    template <typename F>
    static QList<float> call( F&& f ) { return f(); }
  };




template <typename Method, size_t... I>
auto callHelper( Method method, Sd3drModel *model, const SdScriptValueFunction *self, std::index_sequence<I...> )
  {
  return (model->*method)( self->template paramAs< std::remove_cvref_t<std::tuple_element_t<I, typename SdScriptMethodTraits<Method>::ArgsTuple>> >(I)... );
  }



template <typename Method>
class SdScriptValueMethod : public SdScriptValueFunction
  {
    using Traits = SdScriptMethodTraits<Method>;
    using Result = typename Traits::Result;

    Sd3drModel *mModel;
    Method      mMethod;
  public:
    SdScriptValueMethod( Sd3drModel *model, Method method )
      : SdScriptValueFunction( SdScriptResultInvoker<Result>::type )
      , mModel(model)
      , mMethod(method)
      {
      initTypes( std::make_index_sequence<Traits::ArgCount>{} );
      }

  private:

    template<size_t... I>
    void initTypes( std::index_sequence<I...>)
      {
      char types[] = {SdScriptTypeMap<std::remove_cvref_t<std::tuple_element_t<I, typename Traits::ArgsTuple> > >::type...};
      setParamTypes( types, sizeof...(I) );
      }

    Result invoke() const
      {
      return callHelper( mMethod, mModel, this, std::make_index_sequence<Traits::ArgCount>{} );
      }

  public:
    Sd3drFaceList toFaceList() const override
      {
      if constexpr (std::is_same_v<Result,Sd3drFaceList>)
        return invoke();
      return SdScriptValueFunction::toFaceList();
      }

    Sd3drFace     toFace() const override
      {
      if constexpr (std::is_same_v<Result,Sd3drFace>)
        return invoke();
      return SdScriptValueFunction::toFace();
      }

    QMatrix4x4    toMatrix() const override
      {
      if constexpr (std::is_same_v<Result,QMatrix4x4>)
        return invoke();
      return SdScriptValueFunction::toMatrix();
      }

    Sd2dRegion    to2dRegion() const override
      {
      if constexpr (std::is_same_v<Result,Sd2dRegion>)
        return invoke();
      return SdScriptValueFunction::to2dRegion();
      }

    QColor        toColor() const override
      {
      if constexpr (std::is_same_v<Result,QColor>)
        return invoke();
      return SdScriptValueFunction::toColor();
      }

  };




#endif // SDSCRIPTVALUEFUNSOLIDXXX_H
