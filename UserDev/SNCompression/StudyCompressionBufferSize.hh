/**
 * \file StudyCompressionBufferSize.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class StudyCompressionBufferSize
 *
 * @author David Caratelli
 */

/** \addtogroup Analysis

    @{*/

#ifndef STUDYCOMPRESSIONBUFFERSIZE_HH
#define STUDYCOMPRESSIONBUFFERSIZE_HH

#include "ana_base.hh"
#include "CompressionAlgoBase.hh"
#include "Geometry.hh"
#include "TH1D.h"
#include "TF1.h"
#include "TTree.h"
#include "TGraph.h"

namespace larlight {
  /**
     \class StudyCompressionBufferSize
     User custom analysis class made by SHELL_USER_NAME
   */
  class StudyCompressionBufferSize : public ana_base{
  
  public:

    /// Default constructor
    StudyCompressionBufferSize();

    /// Default destructor
    virtual ~StudyCompressionBufferSize(){};

    /** IMPLEMENT in StudyCompressionBufferSize.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in StudyCompressionBufferSize.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in StudyCompressionBufferSize.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    /// Study baseline effects due to compression
    void StudyBaseline(const std::vector<unsigned short> &wf);

    /// Setup model for baseline fitting
    void PrepareLinearModel();

    /// Set Compression Algorithm
    void SetCompressAlgo(compress::CompressionAlgoBase* algo) { _compress_algo = algo; }

    /// Function to extract ADC vector of shorts from tpcfifo data-type
    std::vector<unsigned short> getADCs(larlight::tpcfifo* tpc_data);

    /// Now filll output WFs information into larlight data product so that we can write to output
    void SwapData(larlight::tpcfifo* tpc_data, std::vector<std::vector<unsigned short> > outputWFs,
		  std::vector<int> outTimes);

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

    // Histogram for per-event compression factor
    TTree *_compress_tree;
    double _compression;
    int    _evt;

    // Values for baseline fitting
    TTree *_base_tree;
    double _base_pre;
    double _base_post;
    double _slope_pre;
    double _slope_post;
    double _baseChan;
    double _varChan;
    int  _ch;
    int  _sygType;
    int  _plane;

    // Model for fitting
    TF1 *Model;


  };
}
#endif

/** @} */ // end of doxygen group 
