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
    TTree* GetMCShowerTree() const {return _mcshower_tree;}

    void SetUseRefineDirection(bool flag) {_refine_direction = flag;}
   
    protected:

    void PrepareTTree();

    void ClearTTreeVars();

    bool RefineDirectionTest(storage_manager* storage, event_cluster* ev_cluster, event_mcshower* ev_mcshower);
    
    TTree* _mcshower_tree;


    //this i should get from geometry service but didn't find immediately
    double _wire_2_cm;
    double _time_2_cm;

    //whether you want to refine_direction before calculating figure of merit,
    //or whether you want to disable refine_direction (to compare to... like a control group)
    //default is false
    bool _refine_direction = false;

    // variables that go into the analysis ttree
    std::vector<double> _refinestartend_FOM;

    
    
    //stuff temporarily necessary for refine_direction test
    void RefineStartPointsShowerShape(Double_t &start_wire, Double_t &start_time, Double_t &end_wire, Double_t &end_time, const std::vector<larlight::hit> &in_hit_v);

    TH1F *_hit_angles_forwards;
    TH1F *_hit_angles_backwards;

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
