#ifndef COMPRESSIONSTUDYBASE_CXX
#define COMPRESSIONSTUDYBASE_CXX

#include "CompressionStudyBase.hh"

namespace compress {

  CompressionStudyBase::CompressionStudyBase(){

    _verbose = false; 
  }
  
  void CompressionStudyBase::StudyCompression(const std::vector<unsigned short> &in,
					      const std::vector<std::vector<unsigned short> > &out,
					      const std::vector<int> &times,
					      const int pl)
  {
    
    return;
  }
  
}

#endif
