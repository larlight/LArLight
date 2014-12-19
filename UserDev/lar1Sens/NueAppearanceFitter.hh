 /* \file NueAppearanceFitter.hh
 *
 * \ingroup NueAppearanceFitter
 * 
 * \brief Class def header for a class NueAppearanceFitter
 *
 * @author cja33
 */

/** \addtogroup NueAppearanceFitter

    @{*/
#ifndef NUEAPPEARANCEFITTER_HH
#define NUEAPPEARANCEFITTER_HH

#include "TString.h"
#include "TROOT.h" //for gROOT
#include "TStyle.h" //for gStyle
#include "TGraph.h"
#include "TNtuple.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TMatrix.h"
#include "TMarker.h"
#include "TH2D.h"
#include "TImage.h"
#include "THStack.h"
#include "TPad.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <ostream>

#include "NtupleReader.hh"
#include "SensUtils.hh"
#include "PlotUtils.hh"

/**
   \class NueAppearanceFitter
   User defined class NueAppearanceFitter ... these comments are used to generate
   doxygen documentation!
 */
namespace lar1{

  class NueAppearanceFitter{

    public:

      /// Default constructor
      NueAppearanceFitter();

      /// Default destructor
      virtual ~NueAppearanceFitter(){};

      int Run();

      void setBuildCovarianceMatrix(bool b){fBuildCovarianceMatrix=b;}
      void setMakeRatioPlots(bool b){fMakeRatioPlots=b;}
      void setLoop(bool b){fLoop = b;}
      void setMakeSimplePlot(bool b){fMakeSimplePlot = b;}
      void setMakeEventRatePlots(bool b){fMakeEventRatePlots = b;}
      void setMakeAltSensPlot(bool b){fMakeAltSensPlot = b;}

      // Functions to switch the general configuration variables on and off:
      void setFileSource(std::string s){ fileSource = s;}
      void setVerbose(bool v){verbose = v;}
      void setDebug(bool d){debug = d;}


      void setSpecialNameText       (std::string s){specialNameText        = s;}
      void setSpecialNameTextOsc    (std::string s){specialNameTextOsc     = s;}
      void setSpecialNameText_far   (std::string s){specialNameText_far    = s;}
      void setSpecialNameTextOsc_far(std::string s){specialNameTextOsc_far = s;}

      // Configure beam and detectors:
      void setMode(std::string s){mode = s;}
      void setUse100m(bool b){use100m = b;}
      void setUse470m(bool b){use470m = b;}
      void setUseT600_onaxis(bool b){useT600_onaxis = b;}

      // Can scale the event rates up or down
      void setUbooneScale(double d){ubooneScale = d;}
      void setLAr1NDScale(double d){LAr1NDScale = d;}
      void setIcarusScale(double d){icarusScale = d;}
      void setUboonePOT(double d){uboonePOT = d;}
      void setLAr1NDPOT(double d){LAr1NDPOT = d;}
      void setIcarusPOT(double d){icarusPOT = d;}

      // Can scale the cosmic rates up or down independantly:
      void setUbooneCosmicScale(double d){ubooneCosmicScale = d;}
      void setLAr1NDCosmicScale(double d){LAr1NDCosmicScale = d;}
      void setIcarusCosmicScale(double d){icarusCosmicScale = d;}
      // Physics switches
      void setEnergyType(std::string s){energyType = s;}
      void setNueBins (std::vector<float> bins){nueBins  = bins; nbins_nue = nueBins.size()-1;}
      void setNumuBins(std::vector<float> bins){numuBins = bins; nbins_numu = numuBins.size()-1;}
      void setIncludeCosmics(bool b = false){includeCosmics = b;}
      void setIncludeDirt(bool b = false){includeDirt = b;}
      void setIncludeDetSys(bool b = false){includeDetSyst = b;}
      void setCosmicsFile(std::string s){cosmicsFile = s;}
      void setElectContainedDist(double d){ElectContainedDist = d;}
      void setMinDistanceToStart(double d){minDistanceToStart = d;}
      void setShapeOnlyFit(bool b){shapeOnlyFit = b;}
      void setTopologyCut(double MinVertexEnergyPhoton= 0.05, double MinShowerGap=1.5){
            minVertexEnergyPhoton = MinVertexEnergyPhoton;
            minShowerGap = MinShowerGap;
      }
      void setMinVertexEnergySignal(double MinVertexEnergySignal){
            minVertexEnergySignal = MinVertexEnergySignal;
      }

      // Runtime switches
      void setForceRemake(bool b){forceRemake = b;}
      void setSavePlots(bool b){savePlots=b;}


      void extendFileName(std::string s){fileNameExtenstion = s;}

      void setNpoints(int n)
      {
            npoints = n;
            // for LSND BF:
            // dm2FittingPoint = 0.52*npoints;
            // sin22thFittingPoint = 0.375*npoints;
            // // // for Kopp BF:
            // // dm2FittingPoint = 0.41*npoints;
            // // sin22thFittingPoint = 0.53*npoints;
            // // for high dm2 point:
            // // dm2FittingPoint = 0.9*npoints;
            // // sin22thFittingPoint = 0.375*npoints;

      }

      // Ways to configure the errors used
      void setUseMultiWeights(bool b){useMultiWeights = b;}
      void setUseInfiniteStatistics(bool b){useInfiniteStatistics = b;}
      void setSystematicInflationAmount(double d){systematicInflationAmount =d;}
      void setInflateSystematics(bool b){inflateSystematics = b;}

      // For the multiweights, pick which to use
      void setMultiWeightSource(int source){multiWeightSource = source;}
      void setAbsolute_MWSource(bool b){absolute_MWSource = b;}
      void setNWeights(int n){nWeights = n;}
      void setUseXSecWeights(bool b){useXSecWeights = b;}
      void setUseFluxWeights(bool b){useFluxWeights = b;}

      void setIncludeNumus(bool b){includeNumus = b;}
      void setIncludeFosc(bool b){includeFosc = b;}

      void setCovMatList(std::vector<std::string> s){covMatrixList = s;}
      void setCovMatListSource(std::vector<int> s){covMatrixListSource = s;}

    private:

      SensUtils utils;
      PlotUtils plotUtils;

      // These variables help control which files are read in, and which functions
      // get called.
      bool includeNumus;
      bool includeFosc;

      // These variables handle which functions get run.
      // Some are necessary to others and get called first.
      bool fBuildCovarianceMatrix;
      bool fMakeRatioPlots;
      bool fLoop;
      bool fMakeSimplePlot;
      bool fMakeEventRatePlots;
      bool fMakeAltSensPlot;

      // These are the functions that actually run the code
      // They all get called from inside run(), and aren't public.
      int Prepare();
      int ReadData();
      int BuildCovarianceMatrix();
      int MakeRatioPlots();
      int Loop();
      int MakeSimplePlot();
      int MakeEventRatePlots();
      int MakeAltSensPlot();


      // All of the variables below are settable.
      //Self explanatory, this is where the ntuples you want to use are located:
      std::string fileSource;
      TString fileNameRoot;
      TString fileNameExtenstion;
      TString detNamesString;
      //Note: parts of this routine will write to that same folder, make sure its writable!

      //Set up the bins for the vector:

      bool verbose;  //verbose causes print outs of information that is good for checking
      bool debug  ;  //debug prints out info to make sure root is behaving...

      // These strings can control special setups, like turning off oscillations
      std::string specialNameText;
      std::string specialNameTextOsc;
      std::string specialNameText_far;
      std::string specialNameTextOsc_far;

      bool useHighDm;
      bool useGlobBF;

      int multiWeightSource;
      bool absolute_MWSource;
      bool useXSecWeights;
      bool useFluxWeights;
      

      std::string mode;  //beam mode to run in
      bool use100m;      //Include the detector at 100m?
      bool use150m;
      bool use200m;
      bool use100mLong;
      bool use470m;      //Include the detector at 470m?
      bool use700m;      //Include the detector at 700m?
      bool useT600_onaxis;
      bool useT600_offaxis;
       
      bool forceRemake;

      //Note: there is no infrastructure to handle more than 3 baselines.
      //So, try at your own risk!
      
      double ubooneScale;     //Scale the event rates (uniformly across all events), uboone
      double LAr1NDScale;     //Scale the event rates (uniformly across all events), near det
      double icarusScale;     //Scale the event rates (uniformly across all events), far det

      double ubooneCosmicScale;     //Scale the event rates (uniformly across all events), uboone
      double LAr1NDCosmicScale;     //Scale the event rates (uniformly across all events), near det
      double icarusCosmicScale;     //Scale the event rates (uniformly across all events), far det

      double uboonePOT;     //Scale the event rates (uniformly across all events), uboone
      double LAr1NDPOT;     //Scale the event rates (uniformly across all events), near det
      double icarusPOT;     //Scale the event rates (uniformly across all events), far det

      //Histogram definitions
      double emin;        //GeV
      double emax;        //GeV
      // const Int_t nbinsE = 10;    //number of energy bins in each baseline, for each distribution (fosc, nue, or numu)
      
      std::string energyType;
      // Options are etrue, eccqe, ecalo1, ecalo2;

      //How many points in the final graph do you want?  (symmetric in x and y)
      //More points means longer run time but better graphs
      //Note: most of the run time is in looping over ntuples, which only takes awhile
      //on the very first pass!  (subsequent runs are much faster)
      Int_t npoints;
      Int_t nWeights;
      
      //grid boundaries
      const double dm2min = 0.01;                       //eV**2
      const double dm2max = 100.;                       //eV**2
      const double sin22thmin = 0.0001;
      const double sin22thmax = 1.0;
      
      bool   savePlots;

      bool   useMultiWeights;           // Use the multiweight samples to build the covariance matrix
      bool   shapeOnlyFit;              // Perform the fit with shape analysis

      
      // The rest of the variables are not settable
      std::vector<std::string> baselines;
      std::vector<double> scales;
      std::vector<double> POT;
      std::vector<double> cosmicScales;
      std::vector<std::string> names;
      std::vector<std::string> baselinesFancy;
      std::vector<double> volume;
      std::vector<NtupleReader> readerNue;
      std::vector<NtupleReader> readerNumu;

      // Used for the labels in the plots
      char  label1[200], label2[200];
      int nL;
      int nbins;    //total number of energy bins for side-by-side distributions

      //Fitting variables
      double dm2,sin22th;
      //const double dm2BF=1.2, sin22thBF=0.003; //The LSND best fit values of the parameters
      double chisq;
      const double deltachisq90=1.64, deltachisq3s=7.74, deltachisq5s=23.40; //deltachisq99=5.41;

      // These vectors hold the bin scheme for the nue and numu samples.
      // The default is going to be 200 MeV wide bins from 200 MeV to 3 GeV
      // These bins can be different between nue and numu.
      std::vector<float> nueBins;
      std::vector<float> numuBins;
      int nbins_nue;
      int nbins_numu;
      int nbins_null;


      std::vector < std::vector< float> > oscVector;        //has osc in it
      std::vector <float> nullBackgrounds;                  //has no osc in it

      bool useInfiniteStatistics;
      double systematicInflationAmount;
      bool inflateSystematics;
      

      //except for oscVev, its name[b_line][energy_bin]
      std::vector< std::vector <float> >    eventsnumuVec;
      std::vector< std::vector <float> >    eventsnueVec;
      std::vector<std::vector< std::vector <float> > >    
                                            eventsnueoscVec;

      //scheme is: eventsnueoscVec[b_line][point][energybin]
      std::vector< std::vector <float> >    eventsnuefoscVec;
      std::vector< std::vector <float> >    eventsSignalBestFitNuVec;
      std::vector< std::vector <float> >    eventsSignalBestFitNubarVec;

      std::vector< std::vector <float> >    eventsNueMCStats;
      std::vector< std::vector <float> >    eventsNumuMCStats;

      std::vector< std::vector <float> >    shapeCorrection;

      // //Input fractional systematics covariance matrix
      // //This is going to come from a txt file, from Georgia
      // std::vector<std::vector<float> > fracentries;
      std::vector<float> nullVec;


      // output ntuple with chi2 values and sensitivity contour
      TNtuple * chi2;
      //sensitivity contours
      std::vector<float> x90;
      std::vector<float> y90;
      std::vector<float> x3s;
      std::vector<float> y3s;
      std::vector<float> x5s;
      std::vector<float> y5s;


      // This vector controls which covariance matrices get used in
      // the raster scan.
      std::vector<std::string> covMatrixList;
      std::vector<    int    > covMatrixListSource;

      // For making event rate plots:
      // Vectors for reading in the backgrounds
      std::vector< std::vector<float> >  NueFromNueCC_muonVec;
      std::vector< std::vector<float> >  NueFromNueCC_chargeKaonVec;
      std::vector< std::vector<float> >  NueFromNueCC_neutKaonVec;
      std::vector< std::vector<float> >  NueFromEScatterVec;
      std::vector< std::vector<float> >  NueFromNC_pi0Vec;
      std::vector< std::vector<float> >  NueFromNC_delta0Vec;
      std::vector< std::vector<float> >  NueFromNumuCCVec;
      std::vector< std::vector<float> >  DirtVec;
      std::vector< std::vector<float> >  OtherVec;
      std::vector< std::vector<float> >  NueFromCosmicsVec;

      // This vector is the null vector for the multiweight stuff.
      // if is long enough to accomodate *all* samples (fosc, nue, numu)xnL 
      // but the fosc sample is empty.      
      std::vector<std::vector<float> >  eventsNullVecMultiWeight;



      bool includeCosmics;
      bool includeDirt;
      bool includeDetSyst;
      std::string cosmicsFile;

      double ElectContainedDist;
      double minDistanceToStart;
      double minShowerGap;
      double minVertexEnergyPhoton;
      double minVertexEnergySignal;

  };

} //namespace lar1

#endif
/** @} */ // end of doxygen group 

