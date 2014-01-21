/**
 * \file decoder_manager.hh
 *
 * \ingroup Decoder
 * 
 * \brief Main decoder task manager class def 
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Decoder

    @{*/

#ifndef DECODER_MANAGER_HH
#define DECODER_MANAGER_HH

#include <algorithm>
#include <iterator>
#include <vector>
#include <time.h>

#include "Base-TypeDef.hh"
#include "storage_manager.hh"
#include "bin_io_handler.hh"
#include "algo_base.hh"

namespace larlight {

  /**
    \class decoder_manager
    A handler class for decoding task. It operates (1) binary/ascii file
    I/O, (2) decoding of data word, (3) creation & maintaining of data
    storage manager. 
   */
  class decoder_manager : public larlight_base {

  public:

    /// Default constructor
    decoder_manager();

    /// Default destructor
    virtual ~decoder_manager(){};

    /// Setter for the input filename
    void add_input_filename(std::string name){ _bin_files.push_back(name);};

    /// Setter for the output filename
    void set_output_filename(std::string name){ _storage->set_out_filename(name);};

    /// Setter for the input file format
    void set_format(FORMAT::INPUT_FILE fmt){_fin.set_format(fmt);};

    /// Setter for the decoding algorithm
    void set_decoder(algo_base* algo){_decoder=algo;};

    /// Setter for the debug mode
    void debug_mode(bool debug){_debug_mode=debug;};

    /// A method to start a batch decoding process with specified # of events to be decoded
    bool run(UInt_t limit=0);

    /// A method to initialize members for new decoding process.
    void reset();

    /// A method to enable block-read for an input binary/ascii file
    void set_read_by_block(bool doit=true){_read_by_block=doit;};

    /** 
	A method to set the size of block-read (in terms of number of 32-bit words.
	Note that one does not have to set this variable. If unset (=default value of 0),
	then it assumes the input file holds a 32-bit word at the beginning of each
	block that should represent # of 32-bit words to be read in that particular block.
    */
    void set_read_block_size(size_t n){_read_block_size=n;};

  private:

    /// A method to be called before run() to initialize variables
    bool initialize();

    /// A method 
    bool decode(UInt_t limit=0);

    /// A method to finalize decoding process, to be called after all is done.
    bool finalize();

    /// A method to open input/output files.
    bool open_file();

    //
    // Run control variables
    //
    bool _debug_mode;   ///< Debug mode boolean

    //
    // File IO variables
    //
    std::string _input_file;    ///< Input file name
    std::vector<std::string> _bin_files;
    std::vector<std::string>::iterator _bin_file_iter;
    bin_io_handler _fin;        ///< Input file I/O handler
    /** 
	Read a chunk of data sample from the input file if set to true.
	A chunk can be specified as number of 32-bit words through a parameter
	_read_block_size. If _read_block_size is 0, then it assumes the
	input file holds a block size at the beginning of each I/O block.
    */
    bool _read_by_block;        

    /// Number of words to be read as a chunk. See _read_by_block comment for details.
    size_t _read_block_size;

    // storage I/O
    storage_manager* _storage;  ///< Output file I/O handler
    algo_base* _decoder;        ///< Decoding algorithm

  };
}
#endif
/** @} */ // end of doxygen group 
