/**
 * \file MCShowerMatchAlg.hh
 *
 * \ingroup CMToolAna
 * 
 * \brief Class def header for a class MCShowerMatchAlg
 *
 * @author kazuhiro
 */

/** \addtogroup CMToolAna

    @{*/
#ifndef MCSHOWERMATCHALG_HH
#define MCSHOWERMATCHALG_HH

#include <iostream>
#include "larlight_base.hh"
#include "McshowerLookback.hh"
#include "CMatchManager.hh"
#include "CRUHelper.hh"
#include "TStopwatch.h"
#include "CMToolAnaException.hh"
namespace larlight {
  /**
     \class MCShowerMatchAlg
     User defined class MCShowerMatchAlg ... these comments are used to generate
     doxygen documentation!
  */
  class MCShowerMatchAlg : public larlight_base{
    
  public:
    
    /// Default constructor
    MCShowerMatchAlg();
    
    /// Default destructor
    virtual ~MCShowerMatchAlg(){};
    
    /** Set Cluster Type to be read in: ClusterMerger makes Cluster
	instead of FuzzyCluster
    */
    void SetClusterType(DATA::DATA_TYPE type) {_cluster_type = type;}
    
    bool Prepare(::larlight::storage_manager* storage,
		 const DATA::DATA_TYPE &cluster_type);

    void Match(const std::vector<unsigned int> &cluster_indices,
	       size_t &mcshower_index,
	       double &correctness) const;
    
  protected:
    
    DATA::DATA_TYPE _cluster_type;
    
    McshowerLookback fBTAlgo;
    
    /// Charge-based efficiency histogram: entry per matched pair
    TH1D* hMatchQEff;
    
    /// Charge-based efficiency histogram: entry per event
    TH1D* hMatchQEffEvent;
    
    /// # MCShower / # matched cluster pairs (kinda efficiency)
    TH1D* hMatchNumEff;
    
    /// Internal timer
    TStopwatch fWatch;
    
    std::vector<UChar_t> _view_to_plane;

    std::vector<std::vector<float> > _qfrac_v;
    std::vector<double> _cluster_charge;
    std::vector<unsigned char> _cluster_plane_id;
    std::vector<std::vector<double> > _bmatch_q;
    std::vector<std::vector<size_t> > _bmatch_id;
    std::vector<unsigned int> _mcshower_id;
  };
}
#endif
/** @} */ // end of doxygen group 

