#ifndef PMTFIFO_CC
#define PMTFIFO_CC

#include "pmtfifo.hh"

void pmtfifo::clear_data()

{
  data_base::clear_data();
  fWaveform.clear();
  fCh     = 0xffff;
  fFrame  = 0xffffffff;
  fSample = 0xffffffff;
  fDiscID = PMT::DISC_MAX;
}

#endif
