/**
 * \file pulse_reco.hh
 *
 * \ingroup PulseReco
 * 
 * \brief Class definition file of pulse_reco
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup PulseReco

    @{*/

#ifndef PULSE_RECO_HH
#define PULSE_RECO_HH

#include "ana_base.hh"
#include "preco_manager.hh"

#include <TH1D.h>
#include <TH2D.h>

namespace larlight {

  /**
     \class pulse_reco
     Analysis unit to run preco_manager on larlight data products.
   */
  class pulse_reco : public ana_base {

  public:

    /// Default constructor 
    pulse_reco();

    /// Default destructor
    ~pulse_reco(){}

    /// Implementation of ana_base::initialize method
    virtual bool initialize();

    /// Implementation of ana_base::analyze method
    virtual bool analyze(storage_manager* storage);

    /// Implementation of ana_base::finalize method
    virtual bool finalize();

    /// Manager getter
    ::optreco::preco_manager& get_manager() { return _mgr; }

  private:

    /// Analyzing waveform data type
    DATA::DATA_TYPE _input_type;

    /// Pulse-Reco manager instance
    ::optreco::preco_manager _mgr;

    /// Vector of output data type
    std::vector<larlight::DATA::DATA_TYPE> _output_data_type;
    
  private:

    void store_reco(storage_manager* storage,
		    const fifo* wf,
		    FEM::DISCRIMINATOR disc_id);
    
  };
}

#endif

/** @} */ // end of doxygen group
