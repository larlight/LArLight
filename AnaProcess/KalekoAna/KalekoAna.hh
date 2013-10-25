/**
 * \file KalekoAna.hh
 *
 * \ingroup KalekoAna
 * 
 * \brief Class def header for a class KalekoAna
 *
 * @author davidkaleko
 */

/** \addtogroup KalekoAna

    @{*/
#ifndef KALEKOANA_HH
#define KALEKOANA_HH


#include <iostream>
#include <TSystem.h>
#include <TH1.h>
#include <TGraph.h>
#include <TVector3.h>
#include <TMath.h>

#include <storage_manager.hh>
#include <constants.hh>
#include "KalekoUtilFunc.hh"



/**
   \class KalekoAna
   User defined class KalekoAna ... these comments are used to generate
   doxygen documentation!
 */
class KalekoAna{

public:

  /// Default constructor
  KalekoAna();

  /// Default destructor
  virtual ~KalekoAna(){};

  /// Read in the datascanner output file, -1 max_evts scans all events
  /// Also create a histogram of track ThetaYZ values
  int EventLoop(std::string filename, const int MCTheta, const int max_evts, const int n_bins_histo);

  /// Functions to get pointer to private histograms
  TH1D* GetRecoTrackAngleHist() {return RecoTrackAngleHist;}
  TH1D* GetMCTrackAngleHist() {return MCTrackAngleHist;}
  TH1D* GetNRecoTracksHist() {return NRecoTracksHist;}

private:

  /// Function to initalize histograms with bins, etc
  /// Private b/c called by EventLoop, not by the python macro
  void InitializeHistograms(const int MCTheta, const int n_bins_histo);

  /// Reconstructed ThetaYZ histogram
  TH1D *RecoTrackAngleHist;

  /// MC ThetaYZ histogram
  TH1D *MCTrackAngleHist;
 
  /// Number reconstructed tracks per event histogram
  TH1D *NRecoTracksHist;
  
};

#endif
/** @} */ // end of doxygen group 

