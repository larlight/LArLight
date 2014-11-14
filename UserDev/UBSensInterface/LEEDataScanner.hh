/**
 * \file LEEDataScanner.hh
 *
 * \ingroup LEEDataScanner
 * 
 * \brief This class loops over larlight events, fills a data container (defined in the
 * UBSensitivity code) with relevant parameters, uses UBSens code to write
 * those data to an output file. This output file is later read in and turned
 * into scaled histograms by UBSensivity code.
 * Note event selection should be done by separate filter modules attached
 * to ana_processor before the LEEDataScanner module.
 *
 * @author davidkaleko
 */

/** \addtogroup LEEDataScanner
    
    @{*/

#ifndef LEEDATASCANNER_HH
#define LEEDATASCANNER_HH

#include "ana_base.hh"
#include "DataHandle/DataManager.hh"

namespace larlight {
  /**
     \class LEEDataScanner
     User custom analysis class made by davidkaleko
  */
  class LEEDataScanner : public ana_base{
    
  public:
    
    /// Default constructor
    LEEDataScanner() : ana_base() { 
      _name="LEEDataScanner"; 
      _include_reco_showers = false;
    }
    
    /// Default destructor
    virtual ~LEEDataScanner(){};
    
    /** IMPLEMENT in LEEDataScanner.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();
    
    /** IMPLEMENT in LEEDataScanner.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);
    
    /** IMPLEMENT in LEEDataScanner.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();
    
    /// Setter to include reconstructed showers... if set to false,
    /// then only MCShower parameters are used for analysis
    void SetIncludeRecoShowers(bool flag) { _include_reco_showers = flag; }

  protected:
    
    bool _include_reco_showers;

    ::ubsens::data::DataManager _mgr;

    ::ubsens::data::TruthShower _truthshower;

    ::ubsens::data::RecoShower _recoshower;

    
  };
}
#endif

//**************************************************************************
// 
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group 
