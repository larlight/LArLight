#ifndef STUDYTMP_CC
#define STUDYTMP_CC

#include "StudyTmp.hh"

namespace compress {

  StudyTmp::StudyTmp()
    : CompressionStudyBase()
  {

  }


  void StudyTmp::StudyCompression(const std::vector<unsigned short> &in,
				  const std::vector<std::vector<unsigned short> > &out,
				  const std::vector<int> &times,
				  const int pl)
  {
    
    return;
  }
  

  void StudyTmp::EndProcess(TFile *fout){

    return;
  }

}

#endif
