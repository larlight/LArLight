/**
 * \file ViewCompression.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class ViewCompression
 *
 * @author David Caratelli
 */

/** \addtogroup Analysis

    @{*/

#ifndef VIEWCOMPRESSION_HH
#define VIEWCOMPRESSION_HH

#include "ana_base.hh"
#include "CompressionAlgoBase.hh"
#include "Geometry.hh"
#include <TH1D.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TPad.h>

namespace larlight {
  /**
     \class ViewCompression
     User custom analysis class made by David Caratelli
   */
  class ViewCompression : public ana_base{
  
  public:

    /// Default constructor
    ViewCompression(){ _name="ViewCompression"; _fout=0; _compress_algo=0; _c1 = nullptr; _p1 = nullptr; _hInWF = nullptr; _hOutWF = nullptr; _hInBase = nullptr; _hInVar = nullptr; _baseline = false; };

    /// Default destructor
    virtual ~ViewCompression(){};

    /** IMPLEMENT in ViewCompression.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ViewCompression.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ViewCompression.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    /// PrepareCanvas
    void PrepareCanvas() { _c1 = new TCanvas("c1", "canvas", 900, 900); _c1->Divide(1,2); }

    void UpdateCanvas() { _c1->cd(1); _hInWF->Draw(); _c1->cd(2); _hOutWF->Draw(); }

    /// Set Compression Algorithm
    void SetCompressAlgo(compress::CompressionAlgoBase* algo) { _compress_algo = algo; }

    /// Set boolean whether to use baseline or not
    void suppressBaseline(bool on) { _baseline = on; }

    /// Function to extract ADC vector of shorts from tpcfifo data-type
    std::vector<unsigned short> getADCs(larlight::tpcfifo* tpc_data);

    /// Function to process single waveform and return Canvas
    void processWF();

    /// Get Number of WFs in event
    int GetNumWFs() { return _numWFs; }

    /// Get Number of output waveforms
    int GetNumOutWFs() { return _NumOutWFs; }

    //// Clear Histograms
    //    void ClearHistograms() { _hInWF=0; _hOutWF=0; }
    void ClearHistograms() { delete _hInWF; delete _hOutWF; delete _hInBase; delete _hInVar; }

    /// Fill Histograms with new and old waveforms
    void FillHistograms(std::vector<unsigned short> ADCwaveform,
			std::vector<std::vector<unsigned short> > compressOutput,
			std::vector<int> outputTimes,
			UShort_t ch,
			UChar_t pl);

  void FillBaseVarHistos(std::vector<double> base,
			 std::vector<double> var,
			 UShort_t ch,
			 UChar_t pl);

    /// Get Histograms for python script
    const TH1D* GetHistos(int which) const {
      if (which == 1)
	return _hInWF;
      else if (which == 2)
	return _hOutWF;
      else{
	std::cout << "not valid input...returning _hInWF" << std::endl;
	return _hInWF;
      }

    }

    /// Get baseline histo
    const TH1D* GetBaseHisto() const { return _hInBase; }

    /// Get variance histo
    const TH1D* GetVarHisto() const { return _hInVar; }

    protected:

    /// Event Number
    int _evtNum;

    /// bool to suppress baseline or not
    bool _baseline;
    /// approximate baseline value
    double _base;

    /// Compression Algorithm Object...performs compression
    compress::CompressionAlgoBase* _compress_algo;

    /// event_wf for this event
    larlight::event_tpcfifo *_current_event_wf;

    /// Main Canvas
    TCanvas* _c1;
    /// Main Pad
    TPad* _p1;
    /// Original WF Histo
    TH1D* _hInWF;
    /// Original Baseline Histo (in blocks of 64)
    TH1D* _hInBase;
    /// Original Variance Histo (in blocks of 64)
    TH1D* _hInVar;
    /// Output WF Histo
    TH1D* _hOutWF;

    /// Keep track of which waveform we are looking at
    int _currentWF;

    /// Number of WFs in this event
    int _numWFs;

    /// Number of output waveforms from original input waveform
    int _NumOutWFs;

    TH1S *_hMaxADC;

  };
}
#endif

/** @} */ // end of doxygen group 
