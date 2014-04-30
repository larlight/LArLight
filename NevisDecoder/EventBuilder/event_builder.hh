/**
 * \file event_builder.hh
 *
 * \ingroup EventBuilder
 * 
 * \brief Event builder class def 
 *
 * @author Kazu - Nevis 2014
 */

/** \addtogroup EventBuilder

    @{*/

#ifndef EVENT_BUILDER_HH
#define EVENT_BUILDER_HH

#include <algorithm>
#include <iterator>
#include <vector>
#include <time.h>
#include <TStopwatch.h>

#include "Base-TypeDef.hh"
#include "evb_algo_pmt.hh"
#include "evb_algo_tpc.hh"
#include "evb_algo_trigger.hh"

namespace larlight {

  /**
    \class event_builder
    
   */
  class event_builder : public larlight_base {

  public:

    /// Default constructor
    event_builder();

    /// Default destructor
    virtual ~event_builder(){};

    /// Method to set the output file name
    void set_output_filename(std::string name){ _out_storage->set_out_filename(name);}

    /// Method to set the analysis output file (users' plots/data product)
    void set_ana_output_filename(std::string name) { _ana_filename = name; }
    
    /// Method to add an algorithm/input file
    bool add_input(DATA::DATA_TYPE type, std::string fname);

    /// Method to execute the event building algorithms
    bool run();

    /// Method to set the reference (main) readout stream. Default=trigger.
    void set_master_stream(DATA::DATA_TYPE type);
    
    /// Method to run in debug mode
    void debug_mode(bool debug_mode){ _debug_mode = debug_mode;}

  protected:

    bool supported_stream(DATA::DATA_TYPE type);

    std::vector<int> _algo_index;

    std::vector<larlight::evb_algo_base*> _evb_algo_v;

    DATA::DATA_TYPE _ref_data;

    storage_manager* _out_storage;

    std::string _ana_filename;

    TFile* _fout;

    bool _debug_mode;

  };
}
#endif
/** @} */ // end of doxygen group 
