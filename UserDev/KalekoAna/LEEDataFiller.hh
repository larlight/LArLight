/**
 * \file LEEDataFiller.hh
 *
 * \ingroup KalekoAna
 * 
 * \brief Class def header for a class LEEDataFiller
 *
 * @author davidkaleko
 */

/** \addtogroup KalekoAna
    
    @{*/

#ifndef LEEDATAFILLER_HH
#define LEEDATAFILLER_HH

#include "ana_base.hh"
#include "/Users/davidkaleko/UBSensitivity/core/DataHandle/DataManager.hh"
//#include "DataHandle/DataManager.hh"

namespace kaleko {
  /**
     \class LEEDataFiller
     User custom analysis class made by davidkaleko
  */
  class LEEDataFiller : public larlight::ana_base{
    
  public:
    
    LEEDataFiller(){ _name="LEEDataFiller"; _fout=0;};

    virtual ~LEEDataFiller(){};
    
    virtual bool initialize();

    virtual bool analyze(larlight::storage_manager* storage);

    virtual bool finalize();
    
  protected:
    
    ::ubsens::data::DataManager _mgr;
    
  };
}
#endif

/** @} */ // end of doxygen group 
