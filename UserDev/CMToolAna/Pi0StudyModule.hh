/**
 * \file Pi0StudyModule.hh
 *
 * \ingroup CMergePerformance
 * 
 * \brief Class def header for a class Pi0StudyModule
 *
 * @author davidkaleko
 */

/** \addtogroup CMergePerformance

    @{*/

#ifndef PI0STUDYMODULE_HH
#define PI0STUDYMODULE_HH

#include "ana_base.hh"
#include "ComputePi0Mass.hh"
#include "TwoShower3DIntxn.hh"

namespace larlight {
  /**
     \class Pi0StudyModule
     User custom analysis class made by davidkaleko
   */
  class Pi0StudyModule : public ana_base{
  
  public:

    /// Default constructor
    Pi0StudyModule(){ _name="Pi0StudyModule"; _fout=0;};

    /// Default destructor
    virtual ~Pi0StudyModule(){};

    /** IMPLEMENT in Pi0StudyModule.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in Pi0StudyModule.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in Pi0StudyModule.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:

    void PrepareAnaTree();

    TTree* ana_tree;

    float _mass;
    float _MC_pi0_energy;
    float _dist_reco3Dlineintxn_MCorigin;
    float _reco3Dlineintxn_uncert;
    
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
