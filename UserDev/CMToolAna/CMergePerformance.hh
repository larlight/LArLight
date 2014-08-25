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
#include "CMergeManager.hh"
#include "CRUHelper.hh"
#include "McshowerLookback.hh"

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
  class CMergePerformance : public ana_base {
  
  public:

    /// Default constructor
    CMergePerformance() : ana_base() { 
      _name="CMergePerformance"; 
      _fout=0;
      _run_before_merging=true;
      _run_merging=true;
      _MCShower_mother_energy_cutoff = 0.02;
      _cluster_type = DATA::FuzzyCluster;
    }

    /// Default destructor
    virtual ~CMergePerformance(){}

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

    /** Set Cluster Type to be read in: ClusterMerger makes Cluster
	instead of FuzzyCluster
    */
    void SetClusterType(DATA::DATA_TYPE type) {_cluster_type = type;}

    /// Getter for the CMergeManager so user can configure merge algos etc
    ::cmtool::CMergeManager& GetManager() { return _mgr; }

    /// Option to not bother with fillin FOM histos with raw input data
    /// This is useful if you are repeatedly running over the same file w/
    /// different merge algos and remaking the same raw plots is redundant
    void SetRunBeforeMerging(bool flag) { _run_before_merging = flag; }
    
    /// Option to not bother with cluster merging and just make FOM plots
    /// for raw input data. The after-merge FOM histos
    /// will just be left blank.
    /// This is useful because merging can take the most time to run
    void SetRunMerging(bool flag) { _run_merging = flag; }
    
    void SetDebug(bool flag) { _debug = flag; }


    protected:

    void PrepareHistos();
    
    void PrepareTTree();

    void FillFOMHistos(bool before_merging,
		       const event_mcshower* ev_mcshower,
		       const event_cluster* ev_cluster,
		       const event_hit* ev_hits);


    ///vector of CPAN objects (before, and overwritten after merging)
    std::vector< ::cluster::ClusterParamsAlg> _clusterparams;

    /// vector of vector of cluster indices for looping over _clusterparmas
    /// before merging it is just {{1},{2},{3},{4},...} since 
    /// _clusterparams has 1:1 mapping to ev_cluster
    /// after merging it is something like {{1,2,3},{4},...} and will be
    /// retreived from a CBookKeeper function
    std::vector<std::vector<unsigned short> > clus_idx_vec;

    //declare a member of CMergeManager class called _mgr
    ::cmtool::CMergeManager _mgr;

    /// CRU helper
    ::cluster::CRUHelper _cru_helper;
    
    bool _run_before_merging, _run_merging;
    double _MCShower_mother_energy_cutoff;

    //Cluster type to be read in
    DATA::DATA_TYPE _cluster_type;

    //declare mcshowerlookback object only once, not once per event
    larlight::McshowerLookback _mcslb;

    //some maps that mcshowerlookback fills once per event
    std::map<UInt_t,UInt_t> _shower_idmap;
    std::map<UShort_t,larlight::simch> _simch_map;
    
    //neural net path needed
    std::string myNeuralNetPath;

    //hit type used in lots of functions, putting it here to make it more global
    larlight::DATA::DATA_TYPE hit_type;

    //.at(0) is before merging, .at(1) is after merging
    //.at(0).at(0) is before merging, plane 0
    //.at(0).at(1) is before merging, plane 1
    std::vector<std::vector<TH1D*> > hPurity;
    std::vector<std::vector<TH1D*> > hPurityPerMCShower;
    std::vector<std::vector<TH1D*> > hEff;
    std::vector<std::vector<TH1D*> > hEffPerMCShower;
    std::vector<std::vector<TH1D*> > hClusQoverMCQ;
    std::vector<std::vector<TH1D*> > hQFracInBigClusters;
    std::vector<std::vector<TH2D*> > hPi0_photonanglediff_vs_Eff;
    //might as well save stuff in a TTree too
    TTree* ana_tree;

    //TTree variables
    //double _purity;
    double _clusQfrac_over_totclusQ;
    double _clusQ_over_MCQ;
    double _tot_clus_charge;
    double _tot_clus_charge_fromKnownMCS;
    double _frac_clusQ;
    double _dom_MCS_Q;
    double _mother_energy;
    int _nhits;
    double _opening_angle;
    double _clusQfrac_from_unknown;
    GEO::View_t _plane;
    bool _after_merging;

    bool _debug;

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
