/**
 * \file comptmp.hh
 *
 * \ingroup SNCompression
 * 
 * @author user
 */

/** \addtogroup SNCompression

    @{*/
#ifndef COMPTMP_HH
#define COMPTMP_HH

#include "CompressionAlgoBase.hh"
#include <math.h>

namespace compress {

  /**
     \class comptmp
     A Class where to write a compressiona algorithm for TPC wire signals.
  */
  class CompTmp : public CompressionAlgoBase {
    
  public:
    
    /// Function where compression is performed
    void ApplyCompression(const std::vector<unsigned short> &waveform);

  protected:

  };

}

#endif
/** @} */ // end of doxygen group 

