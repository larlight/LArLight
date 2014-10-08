#ifndef PI0SELECTION_CC
#define PI0SELECTION_CC

#include "Pi0Selection.hh"

namespace larlight {

  Pi0Selection::Pi0Selection()
  { 
    _name="Pi0Selection"; 
    _fout=0;

    _vtx_error_cut = 10;
    _pi0_ke_range.first  = 50;
    _pi0_ke_range.second = 300;
    _vtx_dist_cut = 3;
    _data_type = DATA::MCShower;

  }

  bool Pi0Selection::initialize() {

    return true;
  }

  void Pi0Selection::SetDataType(DATA::DATA_TYPE type) {

    switch(type) {

    case DATA::Shower:
    case DATA::MCShower:
    case DATA::RyanShower:
      _data_type = type;
      break;
      
    default:

      throw ::showerana::ShowerAnaException(Form("Data type %s not supported by Pi0Selection!",
						 DATA::DATA_TREE_NAME[type].c_str())
					    );
      
    }
  }
  
  bool Pi0Selection::analyze(storage_manager* storage) {

    return true;
  }

  bool Pi0Selection::finalize() {

    return true;
  }

}
#endif
