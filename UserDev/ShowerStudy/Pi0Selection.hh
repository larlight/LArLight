/**
 * \file Pi0Selection.hh
 *
 * \ingroup ShowerStudy
 * 
 * \brief Class def header for a class Pi0Selection
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerStudy

    @{*/

#ifndef PI0SELECTION_HH
#define PI0SELECTION_HH

#include "ana_base.hh"
#include "Pi0ProfileCut.hh"

namespace larlight {

  /**
     \class Pi0Selection
     User custom analysis class made by kazuhiro
   */
  class Pi0Selection : public ana_base{

  public:

    /// Default constructor
    Pi0Selection();

    /// Default destructor
    virtual ~Pi0Selection(){};

    /**
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /**
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    /// Setter for what type of data to look at
    void SetDataType(DATA::DATA_TYPE type);

  protected:

    /// Cut on pi0 vtx error
    double _vtx_error_cut;

    /// Cut on pi0 kinetic energy range (pair of min & max)
    std::pair<double,double> _pi0_ke_range;

    /// Cut on pi0 vtx to daughter shower start distance
    double _vtx_dist_cut;
    
    /// Data type
    DATA::DATA_TYPE _data_type;

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
