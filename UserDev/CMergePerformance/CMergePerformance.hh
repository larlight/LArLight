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
#include "CMAlgoShortestDist.hh"

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
    CMergePerformance() : ana_base() { _name="CMergePerformance"; _fout=0;}

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

    protected:

    void PrepareHistos();
    
    void FillClusterParamsVector(event_cluster* ev_cluster,
				 event_hit* ev_hits);

    void FillFOMHistos(bool before_merging,
		       event_mcshower* ev_mcshower,
		       const std::vector<::cluster::ClusterParamsAlgNew> &_clusterparams);


    void RunMerging(event_cluster* ev_cluster,
		    event_hit* ev_hits);

    std::vector<::cluster::ClusterParamsAlgNew> _clusterparams;

    //declare a member of CMergeManager class called _mgr
    ::cluster::CMergeManager _mgr;

    //declare a member of CBoolAlgoBase class called _shortest_dist_algo
    ::cluster::CMAlgoShortestDist _shortest_dist_algo;
    



    Double_t w2cm, t2cm;
    
    //neural net path needed
    std::string myNeuralNetPath;

    //[0] is before merging, [1] is after merging
    std::vector< TH1D* > hPurity;
    std::vector< TH1D* > hNClusOverNMCShowers;




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
