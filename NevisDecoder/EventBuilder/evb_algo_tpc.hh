/**
 * \file evb_algo_tpc.hh
 *
 * \ingroup EventBuilder
 * 
 * \brief Event builder algorithm class def for TPC data stream
 *
 * @author Kazu - Nevis 2014
 */

/** \addtogroup EventBuilder

    @{*/

#ifndef EVB_ALGO_TPC_HH
#define EVB_ALGO_TPC_HH

#include "evb_algo_base.hh"
#include <TGraph.h>
#include <TH1D.h>

namespace larlight {

  class evb_algo_tpc : public evb_algo_base {

  public:

    evb_algo_tpc() : evb_algo_base(DATA::TPCFIFO) { _name="evb_algo_tpc";}

    virtual ~evb_algo_tpc(){}

    virtual bool initialize();

    virtual bool finalize();

    virtual bool check_event_quality();

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
