#include "SdKiCadFootprint.h"


SdKiCadFootprint::SdKiCadFootprint(SdProject *prj)
  {

  }

void SdKiCadFootprint::prolog(SdScanerSe *)
  {

  }

void SdKiCadFootprint::epilog(SdScanerSe *)
  {

  }

void SdKiCadFootprint::fp_text(SdScanerSe *scaner)
  {
  SdKiCadFpText fpText;
  fpText.scan( scaner );
  }

void SdKiCadFootprint::fp_line(SdScanerSe *scaner)
  {
  SdKiCadFpLine fpLine;
  fpLine.scan( scaner );
  }

void SdKiCadFootprint::fp_rect(SdScanerSe *scaner)
  {

  }

void SdKiCadFootprint::fp_circle(SdScanerSe *scaner)
  {

  }

void SdKiCadFootprint::fp_arc(SdScanerSe *scaner)
  {

  }

void SdKiCadFootprint::fp_poly(SdScanerSe *scaner)
  {

  }

void SdKiCadFootprint::fp_pad(SdScanerSe *scaner)
  {

  }
