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

    CompressionAlgosncompress();
    
    /// Function where compression is performed
    void ApplyCompression(const std::vector<unsigned short> &waveform, const int mode, const UInt_t ch);

    /// Function that determines if we passed the threshold. Per plane
    bool PassThreshold(double thisADC, double base);

    void EndProcess(TFile* fout);

    void SetBlockSize(int b) { _block = b; }
    void SetBaselineThresh(double b) { _deltaB = b*b; }
    void SetVarianceThresh(double v) { _deltaV = v*v; }
    void SetCompressThresh(double tU, double tV, double tY) { _thresh[0] = tU; _thresh[1] = tV; _thresh[2] = tY; }
    void SetMaxADC(int ADC) { _maxADC = ADC; }
    void SetUplaneBuffer(int pre, int post) { _buffer[0][0] = pre; _buffer[0][1] = post; }
    void SetVplaneBuffer(int pre, int post) { _buffer[1][0] = pre; _buffer[1][1] = post; }
    void SetYplaneBuffer(int pre, int post) { _buffer[2][0] = pre; _buffer[2][1] = post; }
    void SetUVYplaneBuffer(int upre, int upost, int vpre, int vpost, int ypre, int ypost);

    // Decide if to fill tree with info or not
    void SetFillTree(bool on) { _fillTree = on; }

  protected:

    /// Merge overlapping regions - return is whether anything was merged
    bool MergeOverlaps();

    int _block;

    // Value of Baseline Threshold for initial stage of compression
    double _deltaB; // max difference for Baseline tolerared
    double _deltaV; // max difference for Variance tolerated
    std::vector<double> _thresh; // threshold to trigger waveform readout (per plane)
    
    // Buffer Tick number for various planes
    // structure: [ [Upre, Upost], [Vpre, Vpost], [Ypre, Ypost] ]
    std::vector<std::vector<int> > _buffer;

    int _maxADC;

    // Keep track of the per-channel baseline in a map
    // The baseline will be re-set per run.
    // If the baseline is not found in the first 3 blocks
    // (because noisy) then no output can be saved.
    // this will go on until a quiet region in the run is found
    std::map<int, double> _baselineMap;

    //TTree where to store stuff
    TTree *_algo_tree;
    // boolean to decide if to fill tree
    bool _fillTree;
    int _pl;
    double _v1, _v2, _v3;
    double _b1, _b2, _b3;
    double _max;
    int _interesting;
    int _save;
  };

}

#endif
/** @} */ // end of doxygen group 

