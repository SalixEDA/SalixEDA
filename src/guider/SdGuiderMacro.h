#ifndef SDGUIDERMACRO_H
#define SDGUIDERMACRO_H

#include "objects/SdJsonIO.h"

struct SdGuiderMacro
  {
    SdPoint mPosition;    //!< Mouse position
    int     mButtons;     //!< Mouse buttons

    //!
    //! \brief json Function to write object into json writer
    //! \param js   Json writer
    //!
    void json( SdJsonWriter &js ) const;

    //!
    //! \brief json Function to read object from json reader
    //! \param js   Json reader
    //!
    void json( const SdJsonReader &js );
  };


#endif // SDGUIDERMACRO_H
