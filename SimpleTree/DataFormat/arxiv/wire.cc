#ifndef WIRE_CC
#define WIRE_CC

#include "wire.hh"

//######################################################################
void wire::clear_data()
//######################################################################
{
  fSignal.clear();
  fView       = GEO::kUnknown;
  fSignalType = GEO::kMysteryType;
  fChannel    = 0xffffffff;
}

#endif
