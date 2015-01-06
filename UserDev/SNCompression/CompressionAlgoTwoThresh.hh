/**
 * \file compressionalgotwothresh.hh
 *
 * \ingroup SNCompression
 * 
 * @author yuntse
 */

/** \addtogroup SNCompression

    @{*/
#ifndef COMPRESSIONALGOTWOTHRESH_HH
#define COMPRESSIONALGOTWOTHRESH_HH

#include "CompressionAlgoBase.hh"
#include "CompressionAlgoOneThresh.hh"
#include <math.h>

namespace compress {

  /**
     \class compressionalgotwothresh
     A Class where to write a compressiona algorithm for TPC wire signals.
  */
  class CompressionAlgoTwoThresh : public CompressionAlgoOneThresh {
    
  public:
    
    /// Function where compression is performed; override
    void ApplyCompression( const std::vector<unsigned short> &waveform, int mode );

    /// Override the function to set Threshold (CompressionAlgoThresh::SetThreshold)
    void SetThreshold( int lthr0, int tthr0, int lthr1, int tthr1, int lthr2, int tthr2, int mt ) { _LeadingThreshold[0] = lthr0; _TrailingThreshold[0] = tthr0; _LeadingThreshold[1] = lthr1; _TrailingThreshold[1] = tthr1; _LeadingThreshold[2] = lthr2; _TrailingThreshold[2] = tthr2; _mt = mt; }

  protected:
    /// Threshold for cut
    double _LeadingThreshold[3];
    double _TrailingThreshold[3];
    /// Number of samples below the trailing threshold to consider done
    int    _mt;

  };

}

#endif
/** @} */ // end of doxygen group 

