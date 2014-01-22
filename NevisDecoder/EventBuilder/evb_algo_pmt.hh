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

namespace larlight {

  class evb_algo_pmt : public evb_algo_base {

  public:

    evb_algo_pmt() : evb_algo_base(DATA::PMTFIFO) { _name = "evb_algo_pmt";}

    virtual ~evb_algo_pmt(){}

    virtual bool initialize();

    virtual bool finalize();

  protected:

    virtual bool build(storage_manager *out_storage, UInt_t id);

  };
}
#endif
/** @} */ // end of doxygen group 
