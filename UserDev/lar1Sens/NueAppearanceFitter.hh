/**
 * \file NueAppearanceFitter.hh
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

      int Prepare();
      int ReadData();
      int Loop();
      int MakePlots();
      int MakeEventRatePlots();


      // Functions to switch the general configuration variables on and off:
      void setFileSource(std::string s){ fileSource = s;}
      void setVerbose(bool v){verbose = v;}
      void setDebug(bool d){debug = d;}
      void setSpecialNameText       (std::string s){specialNameText        = s;}
      void setSpecialNameTextOsc    (std::string s){specialNameTextOsc     = s;}
      void setSpecialNameText_far   (std::string s){specialNameText_far    = s;}
      void setSpecialNameTextOsc_far(std::string s){specialNameTextOsc_far = s;}
      void setFlatSystematicError(double d){flatSystematicError = d;}
      void setMode(std::string s){mode = s;}
      void setUse100m(bool b){use100m = b;}
      void setUse100mLong(bool b){use100mLong = b;}
      void setUse470m(bool b){use470m = b;}
      void setUse700m(bool b){use700m = b;}
      void setUseT600(bool b){useT600 = b;}
      void setUbooneScale(double d){ubooneScale = d;}
      void setLAr1NDScale(double d){LAr1NDScale = d;}
      void setLAr1FDScale(double d){LAr1FDScale = d;}
      void setEnergyType(std::string s){energyType = s;}

      void setForceRemake(bool b){forceRemake = b;}

      void setNpoints(int n)
      {
            npoints = n;
            // // for LSND BF:
            // dm2FittingPoint = 0.52*npoints;
            // sin22thFittingPoint = 0.375*npoints;
            // for Kopp BF:
            dm2FittingPoint = 0.41*npoints;
            sin22thFittingPoint = 0.53*npoints;

      }
      void setUseNearDetStats(bool b){useNearDetStats = b;}
      void setShapeOnlyFit(bool b){shapeOnlyFit = b;}
      void setNearDetSystematicError(double d){nearDetSystematicError = d;}

      void setElectContainedDist(double d){ElectContainedDist = d;}

      void setUseInfiniteStatistics(bool b){useInfiniteStatistics = b;}

      void setSystematicInflationAmount(double d){systematicInflationAmount =d;}
      void setInflateSystematics(bool b){inflateSystematics = b;}

    private:

      SensUtils utils;
      PlotUtils plotUtils;

      // All of the variables below are settable.
      //Self explanatory, this is where the ntuples you want to use are located:
      std::string fileSource;
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

      TString fileNameRoot;
      double flatSystematicError;  // Only used if nearDetStats = false.

      std::string mode;  //beam mode to run in
      bool use100m;      //Include the detector at 100m?
      bool use100mLong;
      bool use470m;      //Include the detector at 470m?
      bool use700m;     //Include the detector at 700m?
      bool useT600;
       
      bool forceRemake;

      //Note: there is no infrastructure to handle more than 3 baselines.
      //So, try at your own risk!
      
      double ubooneScale;       //Scale the event rates (uniformly across all events), uboone
      double LAr1NDScale; //Scale the event rates (uniformly across all events), near det
      double LAr1FDScale;     //Scale the event rates (uniformly across all events), far det

      //Histogram definitions
      const double emin = 0.2;        //GeV
      const double emax = 3.0;        //GeV
      const Int_t nbinsE = 10;    //number of energy bins in each baseline, for each distribution (fosc, nue, or numu)
      
      std::string energyType;
      // Options are etrue, eccqe, ecalo1, ecalo2;

      //How many points in the final graph do you want?  (symmetric in x and y)
      //More points means longer run time but better graphs
      //Note: most of the run time is in looping over ntuples, which only takes awhile
      //on the very first pass!  (subsequent runs are much faster)
      Int_t npoints;
      
      //grid boundaries
      const double dm2min = 0.01;                       //eV**2
      const double dm2max = 100.;                       //eV**2
      const double sin22thmin = 0.0001;
      const double sin22thmax = 1.0;
      
      bool   useNearDetStats;           // Only matters if the covariance matrix vector is empty.
      bool   shapeOnlyFit;              // Only matters with near detd stats = true
      double nearDetSystematicError;  // Only matters if useNearDetStats = true
      std::vector<std::string>  cov_max_name;
      
      // The rest of the variables are not settable
      std::vector<int> baselines;
      std::vector<double> scales;
      std::vector<std::string> names;
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
      const double deltachisq90=1.64, deltachisq3s=9.00, deltachisq5s=25.00;


      std::vector < std::vector< float> > eventsnLVec;      //has osc in it
      std::vector < std::vector< float> > eventsnLfitVec; //holds collapsed eventsnLVec spectrum, for fit
      std::vector <float> eventsnLnullVec;                  //has no osc in it
      std::vector <float> eventsnLcvVec;                    //has fullosc*flat0.3% osc prob in it

      bool useInfiniteStatistics;
      double systematicInflationAmount;
      bool inflateSystematics;
      
      //except for oscVev, its name[b_line][energy_bin]
      std::vector< std::vector <float> >    eventsnumuVec;
      std::vector< std::vector <float> >     eventsnueVec;
      std::vector<std::vector< std::vector <float> > >    eventsnueoscVec;
      //scheme is: eventsnueoscVec[b_line][point][energybin]
      std::vector< std::vector <float> >    eventsnuefoscVec;
      std::vector< std::vector <float> >    eventsSignalBestFitNuVec;
      std::vector< std::vector <float> >    eventsSignalBestFitNubarVec;

      std::vector< std::vector <float> >    shapeCorrection;

      std::vector< std::vector <float> >    fittingSignal;
      std::vector< std::vector <float> >    fittingBackgr;
      std::vector< std::vector <float> >    fittingErrors;
      int dm2FittingPoint;
      int sin22thFittingPoint;

      //Input fractional systematics covariance matrix
      //This is going to come from a txt file, from Georgia
      std::vector<std::vector<float> > fracentries;
      std::vector<float> nearDetStats;
      std::vector<float> nullVec;

      std::vector<std::vector<float> > systematicErrors;
      std::vector<std::vector<float> > statisticalErrors;
      std::vector<std::vector<float> > systematicErrorsPlotting;
      std::vector<std::vector<float> > statisticalErrorsPlotting;

      // output ntuple with chi2 values and sensitivity contour
      TNtuple * chi2;
      //sensitivity contours
      double * x90;
      double * y90;
      double * x3s;
      double * y3s;
      double * x5s;
      double * y5s;


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



      double ElectContainedDist;

  };

} //namespace lar1

#endif
/** @} */ // end of doxygen group 

