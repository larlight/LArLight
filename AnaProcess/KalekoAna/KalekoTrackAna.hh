/**
 * \file KalekoTrackAna.hh
 *
 * \ingroup KalekoTrackAna
 * 
 * \brief Class def header for a class KalekoTrackAna
 *
 * @author davidkaleko
 */

/** \addtogroup KalekoTrackAna

    @{*/
#ifndef KALEKOTRACKANA_HH
#define KALEKOTRACKANA_HH


#include <iostream>
#include <TSystem.h>
#include <TH1.h>
#include <TGraph.h>
#include <TVector3.h>
#include <TMath.h>

#include <storage_manager.hh>
#include <constants.hh>
#include "KalekoUtilFunc.hh"


namespace kaleko {
  /**
     \class KalekoTrackAna
     User defined class KalekoTrackAna ... these comments are used to generate
     doxygen documentation!
  */
  class KalekoTrackAna{
    
  public:
    
    /// Default constructor
    KalekoTrackAna();
    
    /// Default destructor
    virtual ~KalekoTrackAna(){};

    /// Set the data type
    larlight::DATA::DATA_TYPE fDataType;
    void set_data_type(larlight::DATA::DATA_TYPE type) { fDataType = type;};
    
    /// Read in the datascanner output file, -1 max_evts scans all events
    /// Also create a histogram of track ThetaYZ values
    int EventLoop(std::string filename, const int MCTheta, const int max_evts, const int n_bins_histo);
    
    /// Functions to get pointer to private histograms
    TH1D* GetRecoTrackAngleHist()         {return RecoTrackAngleHist;}
    TH1D* GetMCTrackAngleHist()           {return MCTrackAngleHist;}
    TH1D* GetMCTrackBoxAngleHist()        {return MCTrackBoxAngleHist;}
    TH1D* GetNRecoTracksHist()            {return NRecoTracksHist;}
    TTree* GetDataTree()                  {return DataTree;}
    
  private:
    
    /// Function to initalize histograms with bins, etc
    /// Private b/c called by EventLoop, not by the python macro
    void InitializeHistograms(const int MCTheta, const int n_bins_histo);
    
    /// Function to initialize the data tree
    void PrepareDataTree();
    
    /// Function to loop over data tree and make the histos I want
    void LoopOverTree();
    
    /// Reconstructed initial ThetaYZ histogram
    TH1D *RecoTrackAngleHist;
    
    /// MC initial ThetaYZ histogram
    TH1D *MCTrackAngleHist;
    
    /// MC initial inside of reco boundary box ThetaYZ histogram
    TH1D *MCTrackBoxAngleHist;
    
    /// Number reconstructed tracks per event histogram
    TH1D *NRecoTracksHist;
    
    /// TTree that I fill in the first event loop, then I'll draw histos w/ TTree::Draw (fastest)
    TTree *DataTree;
    
    /// Now the variables that go into the DataTree
    /// ThetaYZ the muon was generated at
    double myMCTheta;
    /// First-Step MC ThetaYZ for muon once it's inside a box defined by boundaries of reconstructed track
    /// vector because there can be multiple reconstructed tracks in one event
    std::vector<double> mydMCBoxTheta;
    /// The first-step thetaYZ for the reconstructed track
    /// vector because there can be multiple reconstructed tracks in one event
    /// MINUS the MC track angle (d stands for difference)
    std::vector<double> mydRecoTrackAngle;
    
    ///The actual reco track angle, just adding to tree for debugging
    std::vector<double> myRecoTrackAngle;
    
    std::vector<double> myAverageRecoTrackAngle;
    
    std::vector<double> myRecoStartingZ;
    
    ///length of the reco track, adding together distances b/t consecutive points
    std::vector<double> crude_reco_tracklengths;
    
    ///length of the MC track portion inside the detector, adding dist b/t consec points
    ///doesn't have to be a vector, each element is the same length
    std::vector<double> crude_MC_tracklengths;
    
    /// track length estimate based on dist b/t first and last reconstructed point
    std::vector<double> straightline_reco_tracklengths;
    
    /// for each muon, pair of (Energy,dE/dx)
    std::vector< std::pair<double,double> > E_dEdx_pair;
    
    std::vector<int> nTrackPoints;
    
    ///max and min difference in angles b/t reco and MC, over ALL tracks in all events
    ///to be used to set range of histograms
    //  double max_mydMCBoxTheta,     min_mydMCBoxTheta;
    double max_mydRecoTrackAngle, min_mydRecoTrackAngle;
    int max_nRecoTracks;
    int nRecoTracks;
    
    int myEventNumber;
    
    int nSpacePoints;
    
  };
}
#endif
/** @} */ // end of doxygen group 

