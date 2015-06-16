/**
 * \file CompressionAlgoOneThresh.hh
 *
 * \ingroup SNCompression
 * 
 * @author Yun-Tse Tsai
 */

/** \addtogroup SNCompression

    @{*/
#ifndef COMPRESSIONALGOONETHRESH_HH
#define COMPRESSIONALGOONETHRESH_HH

#include "CompressionAlgoBase.hh"
#include <math.h>

namespace compress {

  /**
     \class CompressionAlgoOneThresh
     A Class where to write a compressiona algorithm for TPC wire signals.
  */
  class CompressionAlgoOneThresh : public CompressionAlgoBase {
    
  public:
    
    /// Function where compression is performed
    void ApplyCompression(const std::vector<unsigned short> &waveform, int mode);

    /// Function to set input waveform to be compressed
    void SetInWF(std::vector<unsigned short> waveform) { _InWF = waveform; }

    /// Function to be run at beginning of compression algo
    void PrepareAlgo() { _InWF.clear(); _OutWF.clear(); _tmpWF.clear(); _OutWFStartTick.clear(); }

    // Function to find WF baseline...
    void FindBaseline();

    /// Function to set Threshold
    void SetThreshold( int thr0, int thr1, int thr2 ) { _Threshold[0] = thr0; _Threshold[1] = thr1; _Threshold[2] = thr2; }

  protected:

    /// Temporary holder for a cut that passes
    std::vector<unsigned short> _tmpWF;

    /// Baseline value
    double _Baseline;
    /// Threshold for cut
    double _Threshold[3];

  };

}

#endif
/** @} */ // end of doxygen group 

