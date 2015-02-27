/**
 * \file CompressionStudyBase.hh
 *
 * \ingroup DavidAna
 * 
 * @author David Caratelli
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef COMPRESSIONSTUDYBASE_HH
#define COMPRESSIONSTUDYBASE_HH

#include <iostream>
#include "Geometry.hh"
#include <vector>

namespace compress {

  /**
     \class CMAlgoBase
     ...
  */
  class CompressionStudyBase {
    
  public:
    
    /// Default constructor
    CompressionStudyBase();

    /// Default destructor
    virtual ~CompressionStudyBase(){}

    /// Open (maybe to write trees & such)
    virtual void ProcessBegin() {}

    /// Close Algorithm (maybe to write trees & such)
    virtual void EndProcess(TFile* fout=nullptr) {}
 
    /// Setter function for verbosity
    virtual void SetVerbose(bool doit=true) { _verbose = doit; }

    /// Setter function for debug mode
    virtual void SetDebug(bool doit=true) { _debug = doit; }

    /// Function where study is performed
    virtual void StudyCompression(const std::vector<unsigned short> &in,
				  const std::vector<std::vector<unsigned short> > &out,
				  const std::vector<int> &times,
				  const int pl);

  protected:

    /// Boolean to choose verbose mode.
    bool _verbose;

    /// Boolean to choose debug mode.
    bool _debug;

  };

}

#endif
/** @} */ // end of doxygen group 

