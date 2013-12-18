/**
 * \file ClusterAnaPrep.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class ClusterAnaPrep
 *
 * @author kazuhiro
 */

/** \addtogroup Analysis

    @{*/

#ifndef CLUSTERMERGER_HH
#define CLUSTERMERGER_HH

#include "ana_base.hh"
#include "ana_info_struct.hh"

#include <TH2D.h>
#include <TH1D.h>
#include <map>

namespace larlight {

  /**
     \class ClusterAnaPrep
     A utility class to loop over cluster & associated hits to extract cluster-wise parametes.
     It makes some histograms as well, but the main purpose is to fill & hold cluster_ana_info
     C++ struct data which holds cluster-wise information. 
     The class is a singleton and can be called from anywhere through ClusterAnaPrep::get() function.
     Running ClusterAnaPrep::analyze() records processed run/subrun/event-ID in the local variable.
     If ClusterAnaPrep::analyze() is called again for the same event, it returns w/o doing anything.
     Example) Say we have analysis unit A, B, and C attached to ana_processor, and they all want to
     use cluster_ana_info filled by ClusterAnaPrep module. In each of A, B, and C analyze() method,
     they can call ClusterAnaPrep::get()->analyze(storage). Among those modules, whichever calls 
     this method first actually fill the cluster_ana_info, and other modules' call won't do anything.
     This avoids re-filling the cluster_ana_info per call, and allows all A, B and C to work independently.
     Ask Kazu for details if this explanation is not clear.
   */
  class ClusterAnaPrep : public ana_base{
  
  public:

    /// Static method to get a self pointer
    static ClusterAnaPrep* get(){
      if(!_me) _me = new ClusterAnaPrep;
      return _me;
    }

    /// Default destructor
    virtual ~ClusterAnaPrep(){};

    /** 
	Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** 
        Analyze a data event-by-event ... this fills a vector of cluster_ana_info
    */
    virtual bool analyze(storage_manager* storage);

    /** 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    /**
       IMPORTANT
       Setter for a charge fraction cut used to exclude some hits from processing.
       Note cluster_ana_info is computed by looping over hits. However this loop may not
       include all clusters. The loop starts from a hit with the highest charge, and it moves
       to a hit with the lowest charge. The loop terminats, however, when the fraction of the 
       summed charge of processed hits to all hits exceed the set fraction value through this 
       function. If you wish to include all hits in processing, set this to 1.
     */
    void set_q_frac_cut(Double_t f){ clear_event_info(); _q_frac_cut = f; }

    /**
       IMPORTANT
       Setter for the cut value used to exclude clusters with a large start wire number error.
       A cluster with the error of start wire exceeding the set limit will be ignored from
       processing. 
     */
    void set_sigma_cut(Double_t s){ clear_event_info(); _sigma_cut = s; }

    /// A getter method for a total skipped cluster count
    Int_t get_skipped_cluster_cnt() const { return _skipped_cluster_cnt; }

    /// A method to force clear the event-wise information ... CALL IT IF YOU REALLY NEED TO!
    void clear_event_info();

    /// A method to obtain cluster_ana_info vector for a specified wire plane.
    const std::vector<larlight::cluster_ana_info>* get_cluster_info(GEO::View_t v) const;

  protected:

    /// Default constructor
    ClusterAnaPrep();

    /// A utility function to check if the provided event is already processed or not.
    bool same_event(data_base* ptr)
    { return (_run == ptr->run() && _subrun == ptr->subrun() && _event == ptr->event_id()); }

    /// Static self-pointer
    static ClusterAnaPrep* _me;

    /// Boolean to specify the status of initialization & finalization
    bool _initialized, _finalized;

    /// integers used to identify an event
    Int_t _run, _subrun, _event;
    
    /// Cluster multiplicity histograms for U/V/W planes
    TH1D *_hMultU, *_hMultV, *_hMultW;

    /// 2D angle histograms for U/V/W planes
    TH1D *_h2DAngleU, *_h2DAngleV, *_h2DAngleW;

    /// Start wire vs. time histograms for U/V/W planes
    TH2D *_hStartPosU, *_hStartPosV, *_hStartPosW;

    /// End wire vs. time histograms for U/V/W planes
    TH2D *_hEndPosU, *_hEndPosV, *_hEndPosW;

    
    /// Charge fraction cut used to determine which hits to be included in the computation.
    Double_t _q_frac_cut;

    /// Uncertainty cut used to determine which cluster to be included in the computation.
    Double_t _sigma_cut;
    
    /// A counter for skipped clusters that did not satisfy _sigma_cut.
    Int_t _skipped_cluster_cnt;

    std::vector<cluster_ana_info> _uclusters; ///< vector of cluster_ana_info for U plane
    std::vector<cluster_ana_info> _vclusters; ///< vector of cluster_ana_info for V plane
    std::vector<cluster_ana_info> _wclusters; ///< vector of cluster_ana_info for W plane

  };
}
#endif

/** @} */ // end of doxygen group 
