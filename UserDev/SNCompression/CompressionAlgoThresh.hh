/**
 * \file CompressionAlgoThresh.hh
 *
 * \ingroup DavidAna
 * 
 * @author David Caratelli
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef COMPRESSIONALGOTHRESH_HH
#define COMPRESSIONALGOTHRESH_HH

#include "CompressionAlgoBase.hh"
#include <math.h>

namespace compress {

  /**
     \class CMAlgoBase
     An abstract base class for merging/mathcing algorithm. Having this base class helps
     to have a better overall design of various merging for iterative approach.
     The algorithms are run through CMergeManager.
  */
  class CompressionAlgoThresh : public CompressionAlgoBase {
    
  public:
    
    /// Function where compression is performed
    void ApplyCompression(const std::vector<unsigned short> &waveform);
    
    /// Function to set input waveform to be compressed
    void SetInWF(std::vector<unsigned short> waveform) { _InWF = waveform; }

    /// Function to be run at beginning of compression algo
    void PrepareAlgo() { _InWF.clear(); _OutWF.clear(); _tmpWF.clear(); _OutWFStartTick.clear(); _Threshold=5;}

    // Function to find WF baseline...
    void FindBaseline();

  protected:

    /// Temporary holder for a cut that passes
    std::vector<unsigned short> _tmpWF;

    /// Baseline value
    double _Baseline;
    /// Threshold for cut
    double _Threshold;

  };

}

#endif
/** @} */ // end of doxygen group 

