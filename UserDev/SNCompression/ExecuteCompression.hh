/**
 * \file ExecuteCompression.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class ExecuteCompression
 *
 * @author David Caratelli
 */

/** \addtogroup Analysis

    @{*/

#ifndef EXECUTECOMPRESSION_HH
#define EXECUTECOMPRESSION_HH

#include "ana_base.hh"
#include "CompressionAlgoBase.hh"
#include "Geometry.hh"

namespace larlight {
  /**
     \class ExecuteCompression
     User custom analysis class made by SHELL_USER_NAME
   */
  class ExecuteCompression : public ana_base{
  
  public:

    /// Default constructor
    ExecuteCompression(){ _name="ExecuteCompression"; _fout=0; _compress_algo = 0; };

    /// Default destructor
    virtual ~ExecuteCompression(){};

    /** IMPLEMENT in ExecuteCompression.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ExecuteCompression.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ExecuteCompression.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    /// Set Compression Algorithm
    void SetCompressAlgo(compress::CompressionAlgoBase* algo) { _compress_algo = algo; }

    /// Function to extract ADC vector of shorts from tpcfifo data-type
    std::vector<unsigned short> getADCs(larlight::tpcfifo* tpc_data);

    /// Now filll output WFs information into larlight data product so that we can write to output
    void SwapData(larlight::tpcfifo* tpc_data, std::vector<std::vector<unsigned short> > outputWFs);

    /// Calculate compression: keep holders for number of ticks in original waveform and number of ticks in compressed waveforms
    void CalculateCompression(std::vector<unsigned short> beforeADCs, std::vector<std::vector<unsigned short> > afterADCs);

    protected:
    
    /// Compression Algorithm Object...performs compression
    compress::CompressionAlgoBase* _compress_algo;
    
    /// event_tpcfifo: temporary holder for output WFs
    larlight::event_tpcfifo _out_event_wf;

    /// holder for the # of ticks in the original waveforms
    double _inTicks;
    /// holder for the # of ticks in the compressed waveforms
    double _outTicks;


  };
}
#endif

/** @} */ // end of doxygen group 
