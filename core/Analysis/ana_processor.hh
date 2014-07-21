/**
 * \file ana_processor.hh
 *
 * \ingroup Analysis
 * 
 * \brief A batch processor class for arbitrary number of analysis modules
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Analysis

    @{*/
#ifndef ANA_PROCESSOR_HH
#define ANA_PROCESSOR_HH

#include <TFile.h>
#include <vector>
#include "ana_base.hh"
#include "storage_manager.hh"

namespace larlight {
  /**
     \class ana_processor
     A class to be loaded with arbitrary number of ana_base inherited analysis
     modules. This class process data by applying loaded analysis modules in
     consecutive order.
  */
  class ana_processor : public larlight_base{
    
  public:
    
    /// Process flag enum
    enum PROCESS_FLAG{
      INIT,       ///< Process is @ the initialization stage
      READY,      ///< Process is ready to start data processing
      PROCESSING, ///< Process is processing data
      FINISHED    ///< Process has finished processing allevents.
    };
    
    /// Default constructor
    ana_processor();
    
    /// Default destructor
    ~ana_processor(){}
    
    /// Override a method to set verbosity
    /**
       It changes the verbosity level of not just this class but also owned
       other class instances.
    */
    virtual void set_verbosity(MSG::Level level);
    
    /// Setter for an input DATA file name
    void add_input_file(std::string name){_storage->add_in_filename(name);}

    /// Setter for an input ROOT TDirectory name
    void set_input_rootdir(std::string name) {_storage->set_in_rootdir(name);}

    /// Setter for an input ROOT TDirectory name
    void set_output_rootdir(std::string name) {_storage->set_out_rootdir(name);}

    /// Setter for an input ROOT TDirectory name
    void set_rootdir(std::string name) {_storage->set_rootdir(name);}
    
    /// Setter for an output DATA file name
    void set_output_file(std::string name){_storage->set_out_filename(name);}
    
    /// Setter for an analysis output root file name
    void set_ana_output_file(std::string name){_ofile_name=name;}
    
    /// Setter for I/O mode ... READ or WRITE or BOTH 
    void set_io_mode(storage_manager::MODE mode){_storage->set_io_mode(mode);}
    
    /// Setter to specify a certain data class to be read from input
    void set_data_to_read(DATA::DATA_TYPE type,Bool_t read=true)
    {_storage->set_data_to_read(type,read);}
    
    /// Getter of running analysis status
    Bool_t get_ana_status(ana_base* ptr) const;
    
    /// A method to run a batch process 
    Bool_t run(UInt_t start_index=0, UInt_t nevents=0);
    
    /// A method to process just one event.
    Bool_t process_event(UInt_t index=0);
    
    /// A method to append analysis class instance. Returns index number.
    Size_t add_process(ana_base* ana){_analyzers.push_back(ana); return _analyzers.size()-1;}
    
    /// A method to inquir attached analysis class instance.
    ana_base* get_process(Size_t loc){return (_analyzers.size() > loc) ? _analyzers[loc] : 0;}
    
    /// A method to inquire the process status
    PROCESS_FLAG get_process_status() {return _process;}
    
  private:
    
    /// A method to reset members
    void reset();
    
    /// A method to initialize and prepare for running analysis
    Bool_t initialize();
    
    /// A method to finalize data processing
    Bool_t finalize();
    
    std::vector<ana_base*>   _analyzers;  ///< A vector of analysis modules
    std::map<ana_base*,Bool_t> _ana_status; ///< A map of analysis module status
    
    PROCESS_FLAG _process;       ///< Processing status flag
    UInt_t _nevents;           ///< Number of events being processed
    UInt_t _index;             ///< Index of currently processing event
    std::string _ofile_name;     ///< Output file name
    TFile*   _fout;              ///< Output file pointer
    storage_manager* _storage;   ///< Storage manager pointer
    
  };
}
#endif

/** @} */ // end of doxygen group 
