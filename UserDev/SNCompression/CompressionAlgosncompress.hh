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

  protected:

  };

}

#endif
/** @} */ // end of doxygen group 

