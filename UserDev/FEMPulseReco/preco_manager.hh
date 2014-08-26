/**
 * \file preco_manager.hh
 *
 * \ingroup PulseReco
 * 
 * \brief Class definition file of preco_manager
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup PulseReco

    @{*/

#ifndef PRECO_MANAGER_HH
#define PRECO_MANAGER_HH

#include "ana_base.hh"
#include "preco_algo_base.hh"
#include "ped_estimator.hh"

#include <TH1D.h>
#include <TH2D.h>

namespace optreco {

  /**
     \class preco_manager
     A manager class of pulse reconstruction: it runs pedestal estimation and pulse reconstruction algorithm(s).
     This class executes various pulse reconstruction algorithm which inherits from preco_algo_base.
   */
  class preco_manager {

  public:

    /// enum to define ped_estimator algorithm options
    enum PED_METHOD{
      kHEAD = 0, ///< Use first N samples
      kTAIL,     ///< Use last N samples
      kBOTH      ///< Calculate both and use the one with smaller RMS
    };

  public:

    /// Default constructor 
    preco_manager();

    /// Default destructor
    virtual ~preco_manager(){};

    /// A method to set pulse reconstruction algorithm
    void add_reco_algo          (preco_algo_base* algo) { _reco_algo_v.push_back(algo); };

    /// Getter for a number of algorithms
    size_t algo_count() const { return _reco_algo_v.size(); }

    /// Getter for a specific algorithm via index
    const preco_algo_base& get_algo(size_t i) const { return (*(_reco_algo_v.at(i))); }

    /// A method to set a choice of pedestal estimation method
    void set_ped_algo           (PED_METHOD type)       { _ped_method = type;           };

    /// A method to set # of ADC samples to be used for a cosmic readout
    void set_ped_nsample_cosmic (size_t n)              { _ped_nsample_cosmic = n;      };

    /// A method to set # of ADC samples to be used for a beam readout
    void set_ped_nsample_beam   (size_t n)              { _ped_nsample_beam   = n;      };

    bool run_reco(const std::vector<unsigned short>& wf);

  private:

    /// pulse reconstruction algorithm pointer
    std::vector<preco_algo_base*> _reco_algo_v;

    /// ped_estimator object
    ped_estimator _ped_algo;

    /// # samples to be used for pedestal estimation of cosmic readout
    size_t _ped_nsample_cosmic;

    /// # samples to be used for pedestal estimation of beam readout
    size_t _ped_nsample_beam;

    /// Enum value to be set by a user for the pedestal estimation method.
    PED_METHOD _ped_method;

    /// A method to judge whether the subject waveform corresponds to a beam readout or not
    inline bool is_beam(const std::vector<unsigned short>& wf){return (wf.size() > 1000);};

  };
}

#endif

/** @} */ // end of doxygen group
