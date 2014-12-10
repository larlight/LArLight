/**
 * \file compressionalgosncompress.hh
 *
 * \ingroup SNCompression
 * 
 * @author csc2168
 */

/** \addtogroup SNCompression

    @{*/
#ifndef COMPRESSIONALGOSNCOMPRESS_HH
#define COMPRESSIONALGOSNCOMPRESS_HH

#include "CompressionAlgoBase.hh"
#include <math.h>

namespace compress {

  /**
     \class compressionalgosncompress
     A Class where to write a compressiona algorithm for TPC wire signals.
  */
  class CompressionAlgosncompress : public CompressionAlgoBase {
    
  public:
    
    /// Function where compression is performed
    void ApplyCompression(const std::vector<unsigned short> &waveform, int mode);

    void SetBlockSize(int b) { _block = b; }

    void Debug(bool a) {debug = a;}

    void SetBaselineThresh(int b) { _deltaB = b; }
    void SetVarianceThresh(int v) { _deltaV = v; }
    void SetCompressThresh(int c) { _c = c; }

  protected:
    int _block;
    bool debug;

    // Value of Baseline Threshold for initial stage of compression
    int _deltaB;
    int _deltaV;
    int _c;

  };

}

#endif
/** @} */ // end of doxygen group 

