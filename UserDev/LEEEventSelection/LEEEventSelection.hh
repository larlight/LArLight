/**
 * \file LEEEventSelection.hh
 *
 * \ingroup LEEEventSelection
 * 
 * \brief Class def header for a class LEEEventSelection
 *
 * @author davidkaleko
 */

/** \addtogroup LEEEventSelection
    
    @{*/

#ifndef LEEEVENTSELECTION_HH
#define LEEEVENTSELECTION_HH

#include "ana_base.hh"
//#include "/Users/davidkaleko/UBSensitivity/core/DataHandle/DataManager.hh"
#include "DataManager.hh"

namespace larlight {
  /**
     \class LEEEventSelection
     User custom analysis class made by davidkaleko
  */
  class LEEEventSelection : public ana_base{
    
  public:
    
    /// Default constructor
    LEEEventSelection() : ana_base() { _name="LEEEventSelection"; }
    
    /// Default destructor
    virtual ~LEEEventSelection(){};
    
    /** IMPLEMENT in LEEEventSelection.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();
    
    /** IMPLEMENT in LEEEventSelection.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);
    
    /** IMPLEMENT in LEEEventSelection.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();
    
  protected:
    
    ::ubsens::data::DataManager _mgr;
    
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
