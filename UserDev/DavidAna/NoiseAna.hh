/**
 * \file NoiseAna.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class NoiseAna
 *
 * @author SHELL_USER_NAME
 */

/** \addtogroup Analysis

    @{*/

#ifndef NOISEANA_HH
#define NOISEANA_HH

#include "ana_base.hh"


#include "TVirtualFFT.h"
#include "TF1.h"
#include "TMath.h"
#include "TRandom.h"

namespace larlight {
  /**
     \class NoiseAna
     User custom analysis class made by SHELL_USER_NAME
   */
  class NoiseAna : public ana_base{
  
  public:

    /// Default constructor
    NoiseAna(){ _name="NoiseAna"; _fout=0;};

    /// Default destructor
    virtual ~NoiseAna(){};

    /** IMPLEMENT in NoiseAna.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in NoiseAna.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in NoiseAna.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    protected:

    //Crates, Boards and Channels being used
    short _NCrates;
    short _NBoards;
    short _NChans;
    int _TotChans;

    //Width in seconds of each bin
    double _BinTime;
    //minimum channel number used
    int _ChMin;
    //maximum channel number used
    int _ChMax;
    int event_num;
    int maxevt;
    TH1D *AllRMS;
    TH1D *ChannelRMS;
    TH1D *ChannelBaseline;
    TH1D *RMSperChanNum[64];
    TH1D *RMSperBoard[20];
    double BoardVsChanRMSArray[20][64];
    TH2D *BoardVsChanRMS;
    //ChanRMS[Chan#][evt#] = RMS for that chan for that event
    double ChanRMS[20000][100];
    //RMS MAP FOR ALL CHANNELS
    double RMSNoiseMap[1][20][64][100];//limit to 1 crate and 100 evts now
    double BaselineMap[1][20][64][100];
    int ChanEntries[1][20][64];
    //ChanBaseline[Chan#][evt#] = Baseline for that chan for that event
    double ChanBaseline[20000][100];
    int BaselineTrend[20000];
    int RMSTrend[20000];
    double BaselineMean[20000];
    double BaselineRMS[20000];
    TH1D *AllChan;
    TH1D *NoiseSpec;
    TH1D *SineExmpl;
    TH1D *sine;
    TH1D *RandExmpl;
    TH1D *rand;
    TH1D *BaseTrendHisto;
    TH1D *RMSTrendHisto;
    TH1D *BaseMeanHisto;
    TH1D *BaseRMSHisto;
    TH1D *MaxADCsHisto;

  };
}
#endif

/** @} */ // end of doxygen group 
