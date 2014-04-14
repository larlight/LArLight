/**
 * \file algo_base.hh
 *
 * \ingroup Algorithm
 * 
 * \brief Algorithm base class, algo_base definition header file
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Algorithm

    @{*/

#ifndef ALGO_BASE
#define ALGO_BASE

#include "storage_manager.hh"
#include <sstream>
#include <deque>
#include "decode_algo_exception.hh"
namespace larlight {

  /**
     \class algo_base
     Decoder algorithm base class. 
     May need a little make up to be a base for encdoer algorithms.
  */
  class algo_base : public larlight_base {
    
  public:
    
    /// Default constructor 
    algo_base() { 
      _name="algo_base"; 
      _storage=0; 
      _debug_mode=false; 
      _bt_mode=false;
      _bt_nwords=0;
      _bt_nwords_filled=false;
    };
    
    /// Default destructor 
    virtual ~algo_base(){}; 

    /// Core function: expects a stream (ordered) of 32-bit words from PMT readout data. 
    /** A parent process to run this decoder should pass a stream of words to this method. 
	In this framework, a parent class should be decoder_manager for decoding, for instance.
	Children should implement to perform (1) decoding and (2) storing event data upon
	this function call.
    */
    virtual bool process_word(const UInt_t word)=0;
    
    /// Function called just after processing word.
    virtual void finalize(){};

    /// Virtual member: this should return word type defined in FEM::FEM_WORD.
    virtual inline FEM::FEM_WORD get_word_class(const UInt_t word) const
    {if(word) return FEM::UNDEFINED_WORD; else return FEM::UNDEFINED_WORD;};

    /// Run utility: set debug mode. Children class defines what-to-do in debug mode.
    void set_debug_mode(bool doit){_debug_mode=doit;};

    /// Expects a storage pointer to be provided from a parent class. 
    void set_storage_ptr(storage_manager* storage){_storage=storage;};

    /// Run utility: set back-trace mode. One should provide number of words to be stored.
    virtual void set_backtrace_mode(size_t nwords=0){
      _bt_nwords=nwords;
      _bt_mode=(bool)(nwords);
    };

    /// Getter for backtrace mode (returns # words specified to store. 0=no backtrace)
    size_t backtrace_mode() const {return _bt_nwords;};

    /** A method to check event quality upon saving in the output file.
	Children should implement.
     */
    virtual bool check_event_quality() {return false;};

    /// Function to reset variables for new usage
    virtual void reset(){ init_checker_info(); _storage=0;};

    /// Function to print out stored words in buffer for back-trace purpose.
    inline void backtrace() const
    {
      Message::send(MSG::WARNING,__FUNCTION__,"Dumping a backtrace...");
      int ctr=1;
      for(std::deque<UInt_t>::const_iterator iter(_bt_words.begin());
	  iter!=_bt_words.end();
	  ++iter) {
	printf("%08x ",(*iter));
	if(ctr%8==0 && ctr) std::cout<<std::endl;
	ctr++;
      }
      std::cout<<std::endl<<std::endl;
    };

    /// A function to inquire if the local event data holder is empty or not
    virtual bool is_event_empty()=0;

  protected:

    /// Function to initialize checker information
    virtual void init_checker_info();

  protected:
    //
    // Run control 
    //
    bool    _debug_mode;  ///< Debug mode boolean holder
    bool    _bt_mode;     ///< Back-trace mode boolean holder

    //
    // Constants
    //
    static const size_t BEAM_REF_CHANNEL=39; ///< Defines a reference channel for beamgate.
    static const size_t BEAM_NWORDS=21;      ///< Defines # of minimum words to identify beamgate event.
    static const size_t FRAME_WIDTH=8192;    ///< Defines # of ADC samples per readout frame.

    //
    // Data
    // 
    storage_manager* _storage;  ///< Data storage pointer to be provided by a parent process.

    //
    // Checker variables
    //
    UInt_t _checksum;  ///< data checksum
    UInt_t _nwords;    ///< number of data word counts processed

    //
    // Back-trace tools
    //
    std::deque<UInt_t> _bt_words; ///< processed words to be stored event-wise for back-trace purpose  
    size_t _bt_nwords;            ///< user defined # of words to be temporarily stored in the buffer
    bool   _bt_nwords_filled;     ///< run utility boolean to keep a recored of filled buffer

  };
}
#endif

/** @} */ // end of doxygen group
