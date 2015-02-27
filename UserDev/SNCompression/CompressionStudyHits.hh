/**
 * \file compressionstudyhits.hh
 *
 * \ingroup SNCompression
 * 
 * @author david
 */

/** \addtogroup SNCompression

    @{*/
#ifndef COMPRESSIONSTUDYHITS_HH
#define COMPRESSIONSTUDYHITS_HH

#include "CompressionStudyBase.hh"
#include <math.h>

namespace compress {

  /**
     \class compressionstudyhits
     A Class where to write a compressiona algorithm for TPC wire signals.
  */
  class CompressionStudyHits : public CompressionStudyBase {
    
  public:

    CompressionStudyHits();

    /// Function where compression is performed
    void StudyCompression(const std::vector<unsigned short> &in,
			  const std::vector<std::vector<unsigned short> > &out,
			  const std::vector<int> &times,
			  const int pl);

    /// Close Algorithm (maybe to write trees & such)
    void EndProcess(TFile* fout=nullptr);

  protected:


    void findHits(const std::vector<unsigned short> &wf, bool in);

    bool PassThreshold(double thisADC, double base);

    double _threshold;
    
    // TTree and variable for TTree
    TTree *_in_study_tree;
    TTree *_out_study_tree;
    double _hit_peak; // hit max Q for waveforms
    double _hit_area; // hit area Q for waveforms
    int    _pl; // Plane

  };

}

#endif
/** @} */ // end of doxygen group 

