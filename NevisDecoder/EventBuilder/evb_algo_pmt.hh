/**
 * \file evb_algo_pmt.hh
 *
 * \ingroup EventBuilder
 * 
 * \brief Event builder algorithm class def for PMT data stream
 *
 * @author Kazu - Nevis 2014
 */

/** \addtogroup EventBuilder

    @{*/

#ifndef EVB_ALGO_PMT_HH
#define EVB_ALGO_PMT_HH

#include "evb_algo_base.hh"
#include <TGraph.h>
#include <TH1D.h>

namespace larlight {

  class evb_algo_pmt : public evb_algo_base {

  public:

    evb_algo_pmt() : evb_algo_base(DATA::PMTFIFO) { _name = "evb_algo_pmt";}

    virtual ~evb_algo_pmt(){}

    virtual bool initialize();

    virtual bool finalize();

  protected:

    virtual bool build(storage_manager *out_storage, UInt_t id);

    /// FEM's trigger frame number ... length = # events & # FEMs
    std::vector<std::vector<UInt_t> > _fem_trig_frame;

    /// FEM's event frame number ... length = # events & # FEMs
    std::vector<std::vector<UInt_t> > _fem_event_frame;

    /// FEM's trigger sample number ... length = # events & # FEMs
    std::vector<std::vector<UInt_t> > _fem_trig_sample;

    /// FEM module count
    UChar_t _num_fem;
    
    /// Last event number
    UInt_t _last_event_number;

    /// First event number
    UInt_t _first_event_number;

  };
}
#endif
/** @} */ // end of doxygen group 
