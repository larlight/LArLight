/**
 * \file MCShowerStudy.hh
 *
 * \ingroup MCShowerStudy
 * 
 * \brief This is a generic module to compare parameters from reconstructed showers to MC showers.
 *        It calculates whatever figures of merit you want to calculate, and saves it to an output TTree.
 *
 * @author davidkaleko
 */

/** \addtogroup MCShowerStudy

    @{*/

#ifndef MCSHOWERSTUDY_HH
#define MCSHOWERSTUDY_HH

#include "ana_base.hh"
#include <sstream>

//this include works because I modified the GNUMakeFile to add LArUtil
// to INCFLAGS, so compiler can find this hh file
#include "GeometryUtilities.hh"

namespace larlight {
  /**
     \class MCShowerStudy
     User custom analysis class made by davidkaleko
   */
  class MCShowerStudy : public ana_base{
  
  public:

    /// Default constructor
    MCShowerStudy() : ana_base() { _name="MCShowerStudy"; }

    /// Default destructor
    virtual ~MCShowerStudy(){};

    /** IMPLEMENT in MCShowerStudy.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in MCShowerStudy.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in MCShowerStudy.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    //getter for the tree holding figures of merit
    TTree* GetMCClusTree() const {return _mcclus_tree;}

    protected:

    void PrepareTTree();

    void ClearTTreeVars();

    bool RefineDirectionTest(event_cluster* ev_cluster, event_mcshower* ev_mcshower);
    


    TTree* _mcclus_tree;


    //this i should get from geometry service but didn't find immediately
    double _wire_2_cm;
    double _time_2_cm;

    // variables that go into the analysis ttree
    std::vector<double> _refinestartend_FOM;

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
