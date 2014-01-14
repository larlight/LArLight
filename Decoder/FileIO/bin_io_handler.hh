/**
 * \file bin_io_handler.hh
 *
 * \ingroup FileIO
 * 
 * \brief Class def header for binary/ascii file I/O
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup FileIO

    @{*/
#ifndef BIN_IO_HANDLER_HH
#define BIN_IO_HANDLER_HH

#include "Base-TypeDef.hh"
#include <cstdio>

namespace larlight {

  /**
     \class bin_io_handler
     Operates on bin/ASCII file and read-out 32-bit words consecutively.   
  */
  class bin_io_handler : public larlight_base {

  public:
    
    /// I/O mode enum
    enum MODE{
      READ, ///< READ mode
      WRITE ///< WRITE mode
    };
    
    /// Process status enum
    enum STATUS{
      INIT,      ///< Status initialized, before opening a file
      OPENED,    ///< File opened, before start operation
      OPERATING, ///< Operating I/O, before closing file
      CLOSED     ///< Final status: closed a file.
    };
    
    /// Default constructor
    bin_io_handler(FORMAT::INPUT_FILE type=FORMAT::UNDEFINED);

    /// Default destructor
    ~bin_io_handler(){};
    
    /// Send a word to this method to write out in WRITE mode.
    Bool_t write_word(const UInt_t word);
    
    /// Send an array of words to this method to write out in WRITE mode.
    Bool_t write_multi_word(const UInt_t *words, const size_t entries);
    
    /// Receive 32-bit word from this method in READ mode.
    UInt_t read_word();
    
    /// Setter for the input file format defined in FORMAT::INPUT_FILE
    void set_format(FORMAT::INPUT_FILE f) {_format=f;};
    
    /// Setter for I/O mode defined in bin_io_handler::MODE
    void set_mode(MODE mode)              {_mode=mode;};
    
    /// Setter for input/output file name
    void set_filename(std::string name)   {_filename=name;};
    
    /// A method to return if eof is reached or not.
    Bool_t eof() const {return _eof;};
    
    /// A method to open input/output file
    Bool_t open();

    /// A method to check if a file is still in open state.
    Bool_t is_open() const;
    
    /// A method to close input/output file
    void close();
    
    /// A method to initialize the class instance.
    void reset();
    
    UInt_t read_multi_word(size_t length=0);
    
  private:
    
    std::string _filename;    ///< Input/Output filename
    
    Bool_t   _eof;                  ///< EOF boolean
    UShort_t _nwords_in_file;   ///< Number of words read or written per file
    UInt_t   _checksum;         ///< checksum of operated 32-bit words
    size_t   _file_suffix;      ///< A counter for file suffix in case output is too big.
    
    UInt_t _single_word[1];///< A place holder variable to process a single word
    UInt_t _word;          ///< A word set to be read/write-out
    UInt_t _nchars;        ///< Number of chars read in case of ASCII file
    FORMAT::INPUT_FILE _format; ///< Input file format
    STATUS _status;             ///< Process status flag
    MODE   _mode;               ///< I/O mode
    FILE* _handler;             ///< File handler pointer
    
    
    std::vector<UInt_t> _read_word_buffer;
    size_t _multi_word_index;
  };
}  
#endif
/** @} */ // end of doxygen group 
