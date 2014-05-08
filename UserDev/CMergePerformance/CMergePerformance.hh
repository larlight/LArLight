/**
 * \file CMergePerformance.hh
 *
 * \ingroup CMergePerformance
 * 
 * \brief Class def header for a class CMergePerformance
 *
 * @author davidkaleko
 */

/** \addtogroup CMergePerformance

    @{*/

#ifndef CMERGEPERFORMANCE_HH
#define CMERGEPERFORMANCE_HH

#include "ana_base.hh"
#include "ClusterParamsAlgNew.hh"
#include "CMergeManager.hh"
#include "McshowerLookback.hh"

#include "CMAlgoShortestDist.hh"


#include <TStopwatch.h>

namespace larlight {
  /**
     \class CMergePerformance
     User custom analysis class made by davidkaleko
     The purpose of this class is to study cluster merging performance
     with various algorithms and parameters for those algorithms.

     This class will 
     1) start with an events unmerged clusters 
     2) make figure of merit plots that depend on information from MC showers
     3) run cluster merging with whatever algos/parameters
     4) make those same FOM plots to compare to the original
   */
  class CMergePerformance : public ana_base{
  
  public:

    /// Default constructor
    CMergePerformance() : ana_base() { 
      _name="CMergePerformance"; 
      _fout=0;
      _run_before_merging=true;
      _run_merging=true;
    }

    /// Default destructor
    virtual ~CMergePerformance(){};

    /** IMPLEMENT in CMergePerformance.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in CMergePerformance.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in CMergePerformance.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    /// Getter for the CMergeManager so user can configure merge algos etc
    ::cluster::CMergeManager& GetManager() { return _mgr; }

    /// Option to not bother with fillin FOM histos with raw fuzzyclus data
    /// This is useful if you are repeatedly running over the same file w/
    /// different merge algos and remaking the same raw plots is redundant
    void SetRunBeforeMerging(bool flag) { _run_before_merging = flag; }
    
    /// Option to not bother with cluster merging and just make FOM plots
    /// for raw fuzzycluster data. The after-merge FOM histos
    /// will just be left blank.
    /// This is useful because merging can take the most time to run
    void SetRunMerging(bool flag) { _run_merging = flag; }

    protected:

    void PrepareHistos();
    
    void PrepareTTree();

    void FillClusterParamsVector(event_cluster* ev_cluster,
				 event_hit* ev_hits);

    void FillFOMHistos(bool before_merging,
		       event_mcshower* ev_mcshower,
		       event_cluster* ev_cluster,
		       event_hit* ev_hits,
		       const std::vector< ::cluster::ClusterParamsAlgNew> &_clusterparams);


    void RunMerging(event_cluster* ev_cluster,
		    event_hit* ev_hits);

    ///vector of CPAN objects (before, and overwritten after merging)
    std::vector< ::cluster::ClusterParamsAlgNew> _clusterparams;

    /// vector of vector of cluster indices for looping over _clusterparmas
    /// before merging it is just {{1},{2},{3},{4},...} since 
    /// _clusterparams has 1:1 mapping to ev_cluster
    /// after merging it is something like {{1,2,3},{4},...} and will be
    /// retreived from a CBookKeeper function
    std::vector<std::vector<unsigned short> > clus_idx_vec;

    //declare a member of CMergeManager class called _mgr
    ::cluster::CMergeManager _mgr;
    
    bool _run_before_merging, _run_merging;
    

    //declare mcshowerlookback object only once, not once per event
    larlight::McshowerLookback _mcslb;

    //some maps that mcshowerlookback fills once per event
    std::map<UInt_t,UInt_t> _shower_idmap;
    std::map<UShort_t,larlight::simch> _simch_map;

    Double_t w2cm, t2cm;
    
    //neural net path needed
    std::string myNeuralNetPath;

    //hit type used in lots of functions, putting it here to make it more global
    larlight::DATA::DATA_TYPE hit_type;

    //.at(0) is before merging, .at(1) is after merging
    //.at(0).at(0) is before merging, plane 0
    //.at(0).at(1) is before merging, plane 1
    std::vector<std::vector<TH1D*> > hPurity;
    std::vector<std::vector<TH1D*> > hEff;
    std::vector<std::vector<TH1D*> > hClusQoverMCQ;

    //might as well save stuff in a TTree too
    TTree* ana_tree;

    //TTree variables
    //double _purity;
    double _clusQfrac_over_totclusQ;
    double _clusQ_over_MCQ;
    double _tot_clus_charge;
    double _frac_clusQ;
    double _dom_MCS_Q;
    double _mother_energy;
    int _nhits;
    double _opening_angle;
    GEO::View_t _plane;
    bool _after_merging;


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
