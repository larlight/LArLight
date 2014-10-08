#ifndef LEEDATAFILLER_CC
#define LEEDATAFILLER_CC

#include "LEEDataFiller.hh"

namespace kaleko {

  bool LEEDataFiller::initialize() {

    _mgr = ::ubsens::data::DataManager();

    return true;
  }
  
  bool LEEDataFiller::analyze(larlight::storage_manager* storage) {
  
      
    return true;
  }

  bool LEEDataFiller::finalize() {

    // if(_fout) { _fout->cd(); h1->Write(); }
    //
    // else 
    //   print(larlight::MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    //
  
    return true;
  }

}
#endif
