#include "SdNetPinRef.h"
#include "SdGraphPartImp.h"



//!
//! \brief json Overloaded function to write object content into json writer
//! \param js   Json writer
//!
void SdNetPinRef::json(SdJsonWriter &js) const
  {
  js.jsonObjectPtr( "PartImp", mPartImp );
  js.jsonString( "PinNumber", mPinNumber );
  js.jsonString( "PinFunction", mPinFunction );
  }




//!
//! \brief json Overloaded function to read object content from json reader
//! \param js   Json reader
//!
void SdNetPinRef::json(const SdJsonReader &js)
  {
  js.jsonObjectPtr( "PartImp", mPartImp );
  js.jsonString( "PinNumber", mPinNumber );
  js.jsonString( "PinFunction", mPinFunction );
  }
