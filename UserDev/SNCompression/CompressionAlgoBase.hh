/**
 * \file CompressionAlgoBase.hh
 *
 * \ingroup DavidAna
 * 
 * @author David Caratelli
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef COMPRESSIONALGOBASE_HH
#define COMPRESSIONALGOBASE_HH

#include <iostream>
#include <vector>

namespace compress {

  /**
     \class CMAlgoBase
     ...
  */
  class CompressionAlgoBase {
    
  public:
    
    /// Default constructor
    CompressionAlgoBase(){ _verbose = false; _InWF.clear(); _OutWF.clear(); _OutWFStartTick.clear(); _OutWFStartTick.clear(); }
    
    /// Default destructor
    virtual ~CompressionAlgoBase(){}

    /// Function to reset the algorithm instance 
    virtual void Reset() { _InWF.clear(); _OutWF.clear(); _OutWFStartTick.clear(); }

    /// Function where compression is performed
    virtual void ApplyCompression(const std::vector<unsigned short> &waveform);

    /// Get Compression Output Vector
    virtual std::vector<std::vector<unsigned short> > GetOutputWFs() { return _OutWF; }

    /// Get Compression Output Times Vector
    virtual std::vector<int> GetOutputWFTimes() { return _OutWFStartTick; }
 
    /// Setter function for verbosity
    virtual void SetVerbose(bool doit=true) { _verbose = doit; }

    /// Setter function for debug mode
    virtual void SetDebug(bool doit=true) { _debug = doit; }

  protected:

    /// Boolean to choose verbose mode.
    bool _verbose;

    /// Boolean to choose debug mode.
    bool _debug;

    /// Waveform read in by algorithm...to be scanned and compressed
    std::vector<unsigned short> _InWF;

    /// Vector of output waveforms to be returned
    std::vector<std::vector<unsigned short> > _OutWF;
    /// Vector of start-tick values for the output waveforms -> allows to get timing information when saving a new waveform
    std::vector<int> _OutWFStartTick;

  };

}

#endif
/** @} */ // end of doxygen group 

