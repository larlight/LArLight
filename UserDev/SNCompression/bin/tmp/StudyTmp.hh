/**
 * \file studytmp.hh
 *
 * \ingroup SNCompression
 * 
 * @author user
 */

/** \addtogroup SNCompression

    @{*/
#ifndef STUDYTMP_HH
#define STUDYTMP_HH

#include "CompressionStudyBase.hh"
#include <math.h>

namespace compress {

  /**
     \class studytmp
     A Class where to write a compressiona algorithm for TPC wire signals.
  */
  class StudyTmp : public CompressionStudyBase {
    
  public:

    StudyTmp();
    
    /// Function where compression is performed
    void StudyCompression(const std::vector<unsigned short> &in,
			  const std::vector<std::vector<unsigned short> > &out,
			  const std::vector<int> &times,
			  const int pl);

    /// Close Algorithm (maybe to write trees & such)
    void EndProcess(TFile* fout=nullptr);

  protected:

  };

}

#endif
/** @} */ // end of doxygen group 

