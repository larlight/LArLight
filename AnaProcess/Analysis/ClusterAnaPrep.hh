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

#include <TH2D.h>
#include <TH1D.h>
#include <map>

namespace larlight {

  struct cluster_ana_info {

    double start_time_max;
    double start_time_min;
    double peak_time_max;
    double peak_time_min;
    double end_time_max;
    double end_time_min;
    double sum_charge;
    GEO::View_t view;
    size_t cluster_index;

    cluster_ana_info(){
      start_time_max = start_time_min = -1;
      peak_time_max = peak_time_min = -1;
      end_time_max = end_time_min = -1;
      sum_charge = 0;
      view = GEO::kUnknown;
      cluster_index = 0xffff;
    };

  };

  /**
     \class ClusterAnaPrep
     User custom analysis class made by kazuhiro
   */
  class ClusterAnaPrep : public ana_base{
  
  public:

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
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    const std::vector<larlight::cluster_ana_info>* get_cluster_info(GEO::View_t v) const;

  protected:

    /// Default constructor
    ClusterAnaPrep();

    bool same_event(data_base* ptr)
    { return (_run == ptr->run() && _subrun == ptr->subrun() && _event == ptr->event_id()); }

    static ClusterAnaPrep* _me;

    Int_t _run, _subrun, _event;
    TH1D *_hMultU, *_hMultV, *_hMultW;
    TH1D *_h2DAngleU, *_h2DAngleV, *_h2DAngleW;
    TH2D *_hStartPosU, *_hStartPosV, *_hStartPosW;
    TH2D *_hEndPosU, *_hEndPosV, *_hEndPosW;

    Double_t _q_frac_cut;

    Double_t _sigma_cut;

    Int_t _skipped_cluster_cnt;

    std::vector<cluster_ana_info> _uclusters;
    std::vector<cluster_ana_info> _vclusters;
    std::vector<cluster_ana_info> _wclusters;

  };
}
#endif

/** @} */ // end of doxygen group 
