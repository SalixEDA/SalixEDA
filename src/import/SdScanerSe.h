/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  S-expression format scaner
*/
#ifndef SDSCANERSE_H
#define SDSCANERSE_H

#include "SdScanerMultyline.h"

#include <QMap>
#include <QPoint>
#include <QPointF>
#include <functional>

//Previous declarations
class SdScanerSe;



// The SdSeTokenMap type represents an identifier-to-functor map. This map is used to invoke
// the functor when a string identifier is encountered.
template <typename SdSeObject>
using SdSeTokenMap = QMap<QString,std::function<void( SdScanerSe*, SdSeObject* )> >;


// SdSeSerialList представляет собой последовательный список сканирования позиционных элементов
template <typename SdSeObject>
using SdSeSerialList = QList<std::function<bool( SdScanerSe*, SdSeObject* )> >;


//!
//! \brief The SdScanerSe class Represents an S-expression scanner. The main function
//! is the parse template, which accepts a map of handlers for possible record types.
//!
class SdScanerSe : public SdScanerMultyline
  {
  public:
    double mScaleFactor = 1000.0;

    //!
    //! \brief parse    Template function for element scanning
    //! \param tokenMap Map of named elements included in the element
    //! \param obj      Helper object containing information about the scanning results
    //!
    //! Sequentially scans named elements. For each named element,
    //! the corresponding handler from the map is called. Unknown elements (missing from the map) are skipped,
    //! including all nested elements.
    template <class SdSeObject>
    void parse( const SdSeTokenMap<SdSeObject> &tokenMap, SdSeObject *obj )
      {
      // Scan until a closing parenthesis is found or an error occurs
      // matchToken consumes the parenthesis and scans the next token, so after
      // the function completes, the current token is ready to continue analysis
      while( !matchToken(')') && !isError() )
        parseContent( tokenMap, obj );
      }



    //!
    //! \brief parseContent Performs scanning of a single named element
    //! \param tokenMap     Map of named elements and corresponding handlers
    //! \param obj          Helper object containing information about the scanning results
    //!
    template <class SdSeObject>
    void parseContent( const SdSeTokenMap<SdSeObject> &tokenMap, SdSeObject *obj )
      {
      // Token name
      QString tokenName;

      if( token() == 'n' ) {
        tokenNeedValue( 'n', tokenName, QObject::tr("Need token name") );

        if( tokenMap.contains( tokenName ) )
          tokenMap.value(tokenName)( this, obj );

        return;
        }

      // Expect an opening parenthesis followed by a name
      if( tokenNeed( '(' ) && tokenNeedValue( 'n', tokenName, QObject::tr("Need token name") ) ) {
        // Look up the name in the map
        if( tokenMap.contains(tokenName) )
          // Call the handler for the found name
          tokenMap.value(tokenName)( this, obj );
        else {
          // The name is not in the map. Skip the element and all its nested elements
          int level = 0;
          while( token() != ')' || level != 0 ) {
            if( token() == '(' ) level++;
            if( token() == ')' ) level--;
            tokenNext();
            if( isEndOfScanOrError() ) return;
            }
          tokenNext();
          }
        }
      }

    // SdScaner interface
  public:
    virtual void tokenNext() override;
  };




//!
//! \brief sdSeScan Helper handler template (for the handler map). Invokes the scan function for a class member
//! \param scaner   Scanner
//! \param obj      Class object for whose member 'Member' the scan function is called
//!
template<typename SdSeObject, auto Member>
void sdSeParse( SdScanerSe* scaner, SdSeObject* obj )
  {
  std::invoke(Member, obj).scan(scaner);
  }



template <typename SdSeObject, auto Member>
void sdSeParseFlag( SdScanerSe*, SdSeObject *obj )
  {
  using SdMemberType = decltype(obj->*Member);

  static_assert(std::is_same_v<SdMemberType, bool&>, "Member must be a reference to bool" );

  obj->*Member = true;
  }




//!
//! \brief sdSeScanHandler Helper handler template (for the handler map). Invokes the member function 'Member' for scanning
//! \param scaner          Scanner
//! \param obj             Class object for which the member function 'Member' is called as a scanner handler
//!
template<typename SdSeObject, auto Member>
void sdSeParseHandler( SdScanerSe* scaner, SdSeObject* obj )
  {
  (obj->*Member)( scaner );
  }



inline constexpr bool SdSeOptional = true;
inline constexpr bool SdSeRequired = false;



//!
//! \brief sdSeScanInt Helper handler template (for sequential list).
//!                    Performs scanning of an integer into the Member of the object
//! \param scaner      Scanner
//! \param obj         Class object into whose member 'Member' the scanned integer is written
//! \return            true when scanning is successful
//!
template <typename SdSeObject, auto Member, bool optional = SdSeRequired>
bool sdSeScanInt( SdScanerSe *scaner, SdSeObject *obj )
  {
  using SdMemberType = decltype(obj->*Member);

  static_assert(std::is_same_v<SdMemberType, int&>, "Member must be a reference to int" );

  if constexpr (optional) {
    if( scaner->token() != 'd' ) return true;
    }
  return scaner->tokenNeedValueInt( 'd', obj->*Member, "Need integer" );
  }



//!
//! \brief sdSeScanIntScaled Вспомогательный шаблон-обработчик (для последовательного списка).
//!                          Выполняет сканирование double, умножает его на коэффициент масштабирования scale и
//!                          записывает его в Member член объекта
//! \param scaner            Сканер
//! \param obj               Объект класса, в член Member которого записывается сканированное и масштабированное double
//! \return                  true когда сканирование успешно
//!
template <typename SdSeObject, auto Member, int scale, bool optional = SdSeRequired>
bool sdSeScanIntScaled( SdScanerSe *scaner, SdSeObject *obj )
  {
  using SdMemberType = decltype(obj->*Member);

  static_assert(std::is_same_v<SdMemberType, int&>, "Member must be a reference to int" );

  if constexpr (optional) {
    if( scaner->token() != 'd' && scaner->token() != 'f' ) return true;
    }

  double v;
  if( scaner->tokenNeedValueDouble( 'f', v, "Need double" ) ) {
    obj->*Member = static_cast<int>( v * static_cast<double>(scale) );
    return true;
    }

  return false;
  }




//!
//! \brief sdSeScanIntDynScaled Вспомогательный шаблон-обработчик (для последовательного списка).
//!                             Выполняет сканирование double, умножает его на коэффициент масштабирования scaner->mScaleFactor и
//!                             записывает его в Member член объекта
//! \param scaner               Сканер
//! \param obj                  Объект класса, в член Member которого записывается сканированное и масштабированное double
//! \return                     true когда сканирование успешно
//!
template <typename SdSeObject, auto Member, bool optional = SdSeRequired>
bool sdSeScanIntDynScaled( SdScanerSe *scaner, SdSeObject *obj )
  {
  using SdMemberType = decltype(obj->*Member);

  static_assert(std::is_same_v<SdMemberType, int&>, "Member must be a reference to int" );

  if constexpr (optional) {
    if( scaner->token() != 'd' && scaner->token() != 'f' ) return true;
    }

  double v;
  if( scaner->tokenNeedValueDouble( 'f', v, "Need double" ) ) {
    obj->*Member = static_cast<int>( v * scaner->mScaleFactor );
    return true;
    }

  return false;
  }




//!
//! \brief sdSeScanFloat Helper handler template (for sequential list).
//!                      Performs scanning of a float into the Member of the object
//! \param scaner        Scanner
//! \param obj           Class object into whose member 'Member' the scanned float is written
//! \return              true when scanning is successful
//!
template <typename SdSeObject, auto Member, bool optional = SdSeRequired>
bool sdSeScanFloat( SdScanerSe *scaner, SdSeObject *obj )
  {
  using SdMemberType = decltype(obj->*Member);

  static_assert(std::is_same_v<SdMemberType, float&>, "Member must be a reference to float" );

  if constexpr (optional) {
    if( scaner->token() != 'd' && scaner->token() != 'f' ) return true;
    }

  return scaner->tokenNeedValueFloat( 'f', obj->*Member, "Need float" );
  }




//!
//! \brief sdSeScanDouble Helper handler template (for sequential list).
//!                       Performs scanning of a double into the Member of the object
//! \param scaner         Scanner
//! \param obj            Class object into whose member 'Member' the scanned double is written
//! \return               true when scanning is successful
//!
template <typename SdSeObject, auto Member, bool optional = SdSeRequired>
bool sdSeScanDouble( SdScanerSe *scaner, SdSeObject *obj )
  {
  using SdMemberType = decltype(obj->*Member);

  static_assert(std::is_same_v<SdMemberType, double&>, "Member must be a reference to double" );

  if constexpr (optional) {
    if( scaner->token() != 'd' && scaner->token() != 'f' ) return true;
    }

  return scaner->tokenNeedValueDouble( 'f', obj->*Member, "Need double" );
  }




//!
//! \brief sdSeScanString Helper handler template (for sequential list).
//!                       Performs scanning of a quoted string into the Member QString of the object
//! \param scaner         Scanner
//! \param obj            Class object into whose member 'Member' the scanned string is written
//! \return               true when scanning is successful
//!
template <typename SdSeObject, auto Member, bool optional = SdSeRequired>
bool sdSeScanString( SdScanerSe *scaner, SdSeObject *obj )
  {
  using SdMemberType = decltype(obj->*Member);

  static_assert(std::is_same_v<SdMemberType, QString&>, "Member must be a reference to QString" );

  if constexpr (optional) {
    if( scaner->token() != 's' ) return true;
    }

  return scaner->tokenNeedValue( 's', obj->*Member, "Need string" );
  }




//!
//! \brief sdSeScanName   Helper handler template (for sequential list).
//!                       Performs scanning of a name into the Member QString of the object
//! \param scaner         Scanner
//! \param obj            Class object into whose member 'Member' the scanned name is written
//! \return               true when scanning is successful
//!
template <typename SdSeObject, auto Member, bool optional = SdSeRequired>
bool sdSeScanName( SdScanerSe *scaner, SdSeObject *obj )
  {
  using SdMemberType = decltype(obj->*Member);

  static_assert(std::is_same_v<SdMemberType, QString&>, "Member must be a reference to QString" );

  if constexpr (optional) {
    if( scaner->token() != 'n' ) return true;
    }

  return scaner->tokenNeedValue( 'n', obj->*Member, "Need name" );
  }




//!
//! \brief sdSeScanStringOrName Helper handler template (for sequential list).
//!                             Performs scanning of a quoted string or name into the Member QString of the object
//! \param scaner               Scanner
//! \param obj                  Class object into whose member 'Member' the scanned name is written
//! \return                     true when scanning is successful
//!
template <typename SdSeObject, auto Member, bool optional = SdSeRequired>
bool sdSeScanStringOrName( SdScanerSe *scaner, SdSeObject *obj )
  {
  using SdMemberType = decltype(obj->*Member);

  static_assert(std::is_same_v<SdMemberType, QString&>, "Member must be a reference to QString" );

  if constexpr (optional) {
    if( scaner->token() != 's' && scaner->token() != 'n' ) return true;
    }

  return scaner->tokenNeedValue( 's', 'n', obj->*Member, "Need string or name" );
  }




//!
//! \brief sdSeScanSelector Creates a selector that chooses from the list of provided values
//! \param tokens           List of possible selector values
//!
template <typename SdSeObject, auto Member, bool optional = SdSeRequired>
auto sdSeScanSelector( QStringList tokens )
  {
  using SdMemberType = decltype(((SdSeObject*)nullptr)->*Member);
  static_assert(std::is_same_v<SdMemberType, int&>, "Member must be a reference to int");

  // Create a QMap once when creating the functor
  // Key — token, value — its index
  QMap<QString, int> selectorMap;
  int index = 0;
  for( const auto &token : std::as_const(tokens) )
    selectorMap.insert( token, index++ );


  // Return a lambda functor that captures selectorMap by value
  return [selectorMap]( SdScanerSe *scaner, SdSeObject *obj ) -> bool {
    if constexpr (optional) {
      if( scaner->token() != 'n' ) return true;
      }
    QString str = scaner->tokenNeedValue('n');
    obj->*Member = selectorMap.value( str, obj->*Member );
    return true;
    };
  }






//!
//! \brief The SdSeGroup class Template for a group element. It serves for declarative structure definition
//! with the capability of generation by the recursive descent method.
//!
template <typename SdSeObject >
struct SdSeGroup
  {
    //!
    //! \brief prolog Base implementation of prolog. This function is called before scanning starts
    //!
    void prolog( SdScanerSe* ) {}

    //!
    //! \brief epilog Base implementation of epilog. This function is called after scanning completes
    //!
    void epilog( SdScanerSe* ) {}


    //!
    //! \brief scan   Main scanning function
    //! \param scaner Scanner
    //!
    //! Before starting the scan, we call prolog to perform preparatory operations.
    //! Then we scan positional elements (if present). After the positional elements, we perform
    //! scanning of named elements if they are present, and if they are absent, we require a
    //! closing parenthesis. When scanning named elements, the closing parenthesis is detected there.
    //! In conclusion, we call epilog to perform final operations.
    void scan(  SdScanerSe *scaner  )
      {
      // Derived class object
      SdSeObject *obj = static_cast<SdSeObject*>(this);

      // Prolog for the derived class; if it is not present there, do nothing
      obj->prolog( scaner );

      // Scan positional elements if they exist. For this,
      // the serialList must be declared in the derived class
      if constexpr (requiresSerialList<SdSeObject>()) {

        // The sequential scanning itself
        for( auto fun : std::as_const(SdSeObject::serialList) )
          // In case of an error in a sequential element, stop scanning
          if( !fun( scaner, obj )  ) return;

        }


      // Scan named elements if they exist. For this,
      // the tokenMap must be declared in the derived class
      if constexpr (requiresTokenMap<SdSeObject>()) {
        // The named elements scanning itself. The closing parenthesis is checked automatically.
        scaner->parse( SdSeObject::tokenMap, obj );
        }
      else {
        // Explicitly check for the closing parenthesis
        scaner->tokenNeed( ')' );
        }

      // Epilog for the derived class; if it is not present there, do nothing
      obj->epilog( scaner );
      }

  private:
    // Helper objects for conditional compilation

    // To determine the presence of serialList
    template <typename T> static auto requiresSerialList() -> decltype(T::serialList, std::true_type{}) { return {}; }
    template <typename T> static auto requiresSerialList() -> std::false_type { return {}; }

    // To determine the presence of tokenMap
    template <typename T> static auto requiresTokenMap() -> decltype(T::tokenMap, std::true_type{}) { return {}; }
    template <typename T> static auto requiresTokenMap() -> std::false_type { return {}; }
  };



template <typename SdSeObject >
class SdSeGroupValidable : SdSeGroup<SdSeObject>
  {
    bool mPresent = false; //!< Flag, true if this field present in file
  public:
    void epilog( SdScanerSe* ) { mPresent = true; }

    bool isValid() const { return mPresent; }
  };



class SdSeDouble : public SdSeGroupValidable<SdSeDouble>
  {
    double mValue;     //!< Value
  public:
    SdSeDouble( double d = 0.0 ) : mValue(d) {}

    inline double value() const { return mValue; }

    inline static SdSeSerialList<SdSeDouble> serialList = { sdSeScanDouble<SdSeDouble,&SdSeDouble::mValue> };
  };



template <int scale>
class SdSeIntScaled : public SdSeGroupValidable<SdSeIntScaled<scale> >
  {
    int  mValue   = 0;     //!< Value
  public:
    SdSeIntScaled( int v = 0 ) : mValue(v) {}

    inline int value() const { return mValue; }

    inline static SdSeSerialList<SdSeIntScaled<scale> > serialList = { sdSeScanIntScaled<SdSeIntScaled<scale>,&SdSeIntScaled<scale>::mValue, scale> };
  };



class SdSePoint : public SdSeGroupValidable<SdSePoint>
  {
    double mX; //! 2d coordinates
    double mY;
  public:
    inline QPointF point() const { return QPointF(mX,mY); }

    inline static SdSeSerialList<SdSePoint> serialList = {
      sdSeScanDouble<SdSePoint,&SdSePoint::mX>,
      sdSeScanDouble<SdSePoint,&SdSePoint::mY>
      };
  };



template <int scale>
class SdSeSdPoint : public SdSeGroupValidable<SdSeSdPoint<scale> >
  {
    int mX = 0; //!< 2d scaled coordinates
    int mY = 0;
  public:
    inline QPoint point() const { return QPoint(mX,mY); }

    inline static SdSeSerialList<SdSeSdPoint<scale>> serialList = {
      sdSeScanIntScaled<SdSeSdPoint<scale>,&SdSeSdPoint<scale>::mX,scale>,
      sdSeScanIntScaled<SdSeSdPoint<scale>,&SdSeSdPoint<scale>::mY,scale>
      };
  };



class SdSeString : public SdSeGroupValidable<SdSeString>
  {
    QString mValue;           //!< String value
  public:
    inline QString string() const { return mValue; }

    inline static SdSeSerialList<SdSeString> serialList = {
      sdSeScanString<SdSeString,&SdSeString::mValue>
      };
  };




class SdSeStringOrName : public SdSeGroupValidable<SdSeStringOrName>
  {
    QString mValue;           //!< String value
  public:
    inline QString string() const { return mValue; }

    inline static SdSeSerialList<SdSeStringOrName> serialList = {
      sdSeScanStringOrName<SdSeStringOrName,&SdSeStringOrName::mValue>
      };
  };




class SdSeStringList : public SdSeGroupValidable<SdSeStringList>
  {
    QStringList mList;            //!< List of strings
  public:
    inline const QStringList &stringList() const { return mList; }

    void scan( SdScanerSe *scaner )
      {
      while( scaner->token() != ')' )
        mList.append( scaner->tokenNeedValue('s') );
      scaner->tokenNeed( ')' );
      epilog( scaner );
      }
  };



template <typename SdSeObject>
class SdSeSelector : public SdSeGroupValidable<SdSeObject>
  {
    int  mSelector;
  public:
    SdSeSelector( int v = -1 ) : mSelector(v) {}

    inline int selector() const { return mSelector; }

    void scan( SdScanerSe *scaner )
      {
      QString str = scaner->tokenNeedValue('n');
      scaner->tokenNeed( ')' );
      mSelector =  SdSeObject::selectorMap.value(str, mSelector);
      SdSeSelector<SdSeObject>::epilog( scaner );
      }
  };





#endif // SDSCANERSE_H
