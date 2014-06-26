//############################################################################
//Author: Georgia Karagiorgi (georgia@nevis.columbia.edu)
//Author: Corey Adams	(corey.adams@yale.edu)
//February 25, 2013
//
//This macro calculates nue appearance sensitivity for high-Dm2 oscillations
//by performing a chi2 calculation over reconstructed side-by-side nue+numu
//spectra. Up to three detector locations can be simulaneously fit, for
//constraining systematic uncertainties.
//
//Inputs:
//(1) ntuple of neutrinos with enutrue, enurec, baseline, weight, nutype, etc...
//   for each baseline
//(1) ntuple of fully oscillated (numu-->nue) neutrinos with enutrue, erec, baseline
//   weight, nutype, etc... for each baseline
//(2) full fractional systematics covariance matrix, including bin-to-bin and
//   sample-to-sample correlations within a certain baseline, as well as
//   baseline-to-baseline correlations
//
//  By default neutrino mode ntuples are normalized to 6.6e20 POT and antineutrino mode
//  is 10e20 POT.  You can effectively change this by adjusting the scaling of event rates
//  at baselines
//
//  Original code used histograms for vectors, updated to use std::vectors for several reasons
//  Histograms are a pain to deal with in root, since root does not manage them as expected
//  std::vectors are leaner and meaner, should improve speed too.
//
//
//
//The chi2 is calculated over a 2D grid of Dm2 vs sin22th_mue, and a raster scan
//is used to determine the 90% (one-sided), 3sigma (two-sided) and 5sigma(two-sided)
// 1-dof sensitivity contours.
//############################################################################

#include <sys/stat.h>
#include <unistd.h>
#include "TFile.h"
#include "TH1F.h"
#include "TString.h"
#include "TGraph.h"
#include "TNtuple.h"
#include "TChain.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TMatrix.h"
#include "TROOT.h" //for gROOT
#include "TStyle.h" //for gStyle
#include "TH2D.h"
#include "TLegend.h"
#include "THStack.h"
#include "TImage.h"
#include "TMarker.h"
#include "TLatex.h"
#include <iostream>
#include <ostream>
#include <sstream>
#include <fstream>
// #include "calcChiSquared.C"


//for filling the histograms:
#if defined(__CINT__) && !defined(__MAKECINT__)
#include "NtupleReader.cxx+"
#else
#include "NtupleReader.cxx"
#endif
//for collapsing the covariance matrix:
#if defined(__CINT__) && !defined(__MAKECINT__)
#include "collapseMatrix.C+"
#else
#include "collapseMatrix.C"
#endif
//For appending histograms to create
#if defined(__CINT__) && !defined(__MAKECINT__)
#include "appendHistograms.C+"
#else
#include "appendHistograms.C"
#endif



//Trying to make this a cleaned up version of the fitting code.  Breaking off pieces into 
//functions, making the inputs into vectors instead of arrays, etc.

    //Here's the order:
    /*
    
        Done: Know which ntuples to read.
    
        Done: Determine what baselines, modes, etc.
                        
        Done: Know which covariance matrices to use
            //If no covariance matrices, use sys=near det stats + stats
        
        Done: Using size of vector of baselines, read in the ntuples.  Maybe this is it's own script?
            -> Have a script that reads ntuples and fills histos, allows to change that
               script to change oscillation mechanism without changing fitting code
        
        Done: Next, build event rates and covariance matrices.  Each of these should get it's
            own function.
            
        Done: Calculate a sensitivity for a particular value of (dm2, sin22th).  This should
        be its own function to make things easy.  Not it's own function, but there you go.
        
        Done: Plot the results.
        
    
    */
    
//forward declaration of some functions:
std::vector<float> appendVectors(std::vector<float> , std::vector<float> );
std::vector<float> appendVectors(std::vector<float> , std::vector<float> , std::vector<float>);
int buildCovarianceMatrix(std::vector<std::vector<float> > &, std::vector<std::string> & , int );
TH1F* makeHistogram(std::vector<float> & input, double lowbin, double highbin);
std::vector<float> collapseVector(std::vector<float> input, int nbinsE, int nL);


#ifndef FILECHECK
#define FILECHECK
bool fileExists(const char *filename)
{
  ifstream ifile(filename);
  return ifile.is_open();
}
#endif

// adds a plot label to the current canvas
void add_plot_label( char* label, double x, double y, double size = 0.05, int color = 1, int font = 62, int align = 22 ){

  TLatex *latex = new TLatex( x, y, label );
  latex->SetNDC();
  latex->SetTextSize(size);
  latex->SetTextColor(color);
  latex->SetTextFont(font);
  latex->SetTextAlign(align);
  latex->Draw();
}



//Function to produce total covariance matrix
//(Only need one covariance matrix at a time!)
//Takes as input the vector of individual covariance matrices
//If the vector is empty, take second argument of fractional errors at near detector




//This function produces a value of chi squared, given an input covariance matrix and 
/*
//Some error codes I've whipped up:
1    //Error reading the ntuples, check read_ntuple.C or read_ntuple_fosc.C
2    //No baselines!
3    //Couldn't open file with histograms
*/
int multiple_detector_fit()
{
    //    gROOT->Reset();

//---------------Start of section you can alter to change the baselines:

    //Self explanatory, this is where the ntuples you want to use are located:
    char fileSource[200]="/Users/cja33/Desktop/LAr1ND/lar1Repos/lar1/MC2_proposal_ntuples/";
    //Note: parts of this routine will write to that same folder, make sure its writable!

    //Set up the bins for the vector:
    //coming soon...


    bool verbose = false;  //verbose causes print outs of information that is good for checking
    bool debug   = false;  //debug prints out info to make sure root is behaving...

    std::string specialNameText = "";
    std::string specialNameTextOsc = "";

    std::string specialNameText_far = "";
    std::string specialNameTextOsc_far = "";



    TString fileNameRoot = fileSource;
    fileNameRoot += "nue_appearance_";
    // fileNameRoot += "nue_appearance_uboone_";
    // fileNameRoot += "nue_appearance_uboone_20_";
    double flatSystematicError = 0.20;  // Only used if nearDetStats = false.

    std::string mode = "nu";  //beam mode to run in
    bool use100m = false;      //Include the detector at 100m?
    bool use100mLong=true;
    bool use470m = true;      //Include the detector at 470m?
    bool use700m = true;     //Include the detector at 700m?
    bool useT600 = false;
     
    bool forceRemake = false;

    //Note: there is no infrastructure to handle more than 3 baselines.
    //So, try at your own risk!
    
    Double_t ubooneScale = 1.0;       //Scale the event rates (uniformly across all events), uboone
    Double_t LAr1NDScale = (1.0); //Scale the event rates (uniformly across all events), near det
    Double_t LAr1FDScale = (1.0);     //Scale the event rates (uniformly across all events), far det

    //Histogram definitions
    // Double_t emin = 0.2;        //GeV
    // Double_t emax = 3.0;        //GeV
    const Int_t nbinsE = 10;    //number of energy bins in each baseline, for each distribution (fosc, nue, or numu)
    
    std::string energyType = "ecalo1";
    fileNameRoot += energyType;
    fileNameRoot += "_";
    // Options are etrue, eccqe, ecalo1, ecalo2;

    //How many points in the final graph do you want?  (symmetric in x and y)
    //More points means longer run time but better graphs
    //Note: most of the run time is in looping over ntuples, which only takes awhile
    //on the very first pass!  (subsequent runs are much faster)
    Int_t npoints = 250;
    
    //grid boundaries
    Double_t dm2min = 0.01;                       //eV**2
    Double_t dm2max = 100.;                       //eV**2
    Double_t sin22thmin = 0.0001;
    Double_t sin22thmax = 1.0;
    
    bool useNearDetStats = true;           // Only matters if the covariance matrix vector is empty.
    bool shapeOnlyFit = false;              // Only matters with near detd stats = true
    double nearDetSystematicError = 0.20;  // Only matters if userNearDetStats = true
    std::vector<std::string>  cov_max_name;
    //cov_max_name.push_back(mode+"_covariance_matrix/output_fraccovmx_xsec.out");
    

    
//---------------End of section you should alter without care
//
//

    if (use700m && useT600){
        std::cout << "Error: can't use both LAr1-FD and T600.";
        return -1;
    }
    if (use100m && use100mLong){
        std::cout << "Error: can't use both LAr1-ND and 2*LAr1-ND.";
        return -1;
    }


    char  label1[200], label2[200];

    if (mode == "nu"){
        if (energyType == "etrue")  sprintf(label1, "#nu mode, CC events");
        if (energyType == "eccqe")  sprintf(label1, "#nu mode, CCQE events");
        if (energyType == "ecalo1") sprintf(label1, "#nu mode, CC events");
        if (energyType == "ecalo2") sprintf(label1, "#nu mode, CC events");
    }
    else if (mode == "nubar"){
        if (energyType == "etrue")  sprintf(label1, "#bar{#nu} mode, CC events");
        if (energyType == "eccqe")  sprintf(label1, "#bar{#nu} mode, CCQE events");
        if (energyType == "ecalo1") sprintf(label1, "#bar{#nu} mode, CC events");
        if (energyType == "ecalo2") sprintf(label1, "#bar{#nu} mode, CC events");
    }

    if (energyType == "etrue")  sprintf(label2, "True Energy");
    if (energyType == "eccqe")  sprintf(label2, "CCQE Energy");
    if (energyType == "ecalo1") sprintf(label2, "Calo Energy (no neutrons)");
    if (energyType == "ecalo2") sprintf(label2, "Calo Energy (neutrons)");

    std::vector<int> baselines;
    std::vector<double> scales;
    std::vector<std::string> names;
    std::vector<double> volume;
    std::vector<NtupleReader> readerNue;
    std::vector<NtupleReader> readerNumu;
  
    //initialize all the baselines we'll use:
    if (use100m) baselines.push_back(100);
    if (use100mLong) baselines.push_back(100);
    if (use470m) baselines.push_back(470);
    if (use700m || useT600) baselines.push_back(700);
    //and they're corresponding scaling factors:
    if (use100m) scales.push_back(LAr1NDScale);
    if (use100mLong) scales.push_back(LAr1NDScale);
    if (use470m) scales.push_back(ubooneScale);
    if (use700m) scales.push_back(LAr1FDScale);
    if (useT600) scales.push_back(1.0);
    //some detector names just for fun:
    if (use100m) names.push_back("LAr1-ND");
    if (use100mLong) names.push_back("2*LAr1-ND");
    if (use470m) names.push_back("MicroBooNE");
    if (use700m) names.push_back("LAr1-FD");
    if (useT600) names.push_back("ICARUS");
    //fiducial volume FROM MC, also just for fun: (scale to get target volume)
    if (use100m) volume.push_back(40);
    if (use470m) volume.push_back(61.4);
    if (use700m) volume.push_back(1000);
    if (useT600) volume.push_back(476);
    if (use100m) fileNameRoot += "ND_";
    if (use100mLong) fileNameRoot += "2ND_";
    if (use470m) fileNameRoot += "uB_";
    if (use700m) fileNameRoot += "FD_";
    if (useT600) fileNameRoot += "IC_";
    
    if (use100m){
      NtupleReader a("nue",fileSource, 100, mode, energyType, npoints, forceRemake);
      a.setSpecialNameText(specialNameText);
      a.setSpecialNameTextOsc(specialNameTextOsc);
      readerNue.push_back(a);
      a.setSignal("numu");
      a.setSpecialNameText(specialNameText);
      // a.setSpecialNameText(specialNameTextOsc);
      readerNumu.push_back(a);
    }
    if (use100mLong){
      NtupleReader a("nue",fileSource, 100, mode, energyType, npoints, forceRemake);
      a.setSpecialNameText("long");
      a.setSpecialNameTextOsc("long");
      readerNue.push_back(a);
      a.setSignal("numu");
      // a.setSpecialNameText("");
      a.setSpecialNameTextOsc("long");
      // a.setSpecialNameText(specialNameTextOsc);
      readerNumu.push_back(a);
    }
    if (use470m){
      NtupleReader a("nue",fileSource, 470, mode, energyType, npoints, forceRemake);
      // a.setSpecialNameText(specialNameText);
      a.setSpecialNameTextOsc(specialNameTextOsc);
      readerNue.push_back(a);
      a.setSignal("numu");
      // a.setSpecialNameText("");
      a.setSpecialNameTextOsc("");
      // a.setSpecialNameText(specialNameTextOsc);
      readerNumu.push_back(a);
    }
    if (use700m) {
      NtupleReader a("nue",fileSource, 700, mode, energyType, npoints, forceRemake);
      a.setSpecialNameText(specialNameText);
      a.setSpecialNameTextOsc(specialNameTextOsc);
      readerNue.push_back(a);
      a.setSignal("numu");
      a.setSpecialNameText("");
      a.setSpecialNameTextOsc("");
      // a.setSpecialNameText(specialNameTextOsc);
      readerNumu.push_back(a);
    }
    if (useT600) {
      NtupleReader a("nue",fileSource, 700, mode, energyType, npoints, forceRemake);
      specialNameText_far.append("IC");
      specialNameTextOsc_far.append("IC");
      a.setSpecialNameText(specialNameText_far);
      a.setSpecialNameTextOsc(specialNameTextOsc_far);
      readerNue.push_back(a);
      a.setSignal("numu");
      a.setSpecialNameText("");
      a.setSpecialNameTextOsc("");
      // a.setSpecialNameText(specialNameTextOsc);
      readerNumu.push_back(a);
    }
    //This value is the number of baselines:
    Int_t nL = baselines.size();
    const Int_t nbins = nL*nbinsE*3;    //total number of energy bins for side-by-side distributions:
    //just in case, if the number of baselines is 1 switch off nearDetStats:
    if (nL == 1) {
        useNearDetStats=false;
        shapeOnlyFit = false;
        //flatSystematicError = nearDetSystematicError;	
    }
    if (!useNearDetStats) shapeOnlyFit = false;

    //The following section is purely informational, to let the user see what's happening...

    for (int i = 0; i < 10; i++) std::cout << "\n";
    std::cout << "Beginning sensitivity analysis...\n";
    std::cout << "Running in " << mode << " mode with the following detectors:\n";
    for (int i = 0; i < nL; i++){
        std::cout << "\t" << names[i] << " at " << baselines[i] << "m with " << volume[i]*scales[i] << " ton fiducial volume.\n";
    }
    std::cout << "\nLooking for input ntuples in this folder:\n";
    std::cout << "\t" << fileSource << std::endl;
    std::cout << "\nDebug is set to " << debug << std::endl;
    std::cout << "Verbose is set to " << verbose << std::endl;
    
    for (int i = 0; i < 10; i++) std::cout << "\n";
    
    //Make sure there is at least one detector...
    if (nL == 0){
        std::cout << "\nNo baselines selected!  What did you expect? Exiting.\n" << std::endl;
        return 2;
    }
    
    //Stuff for root, drawing options and such.
    gStyle->SetOptStat(0000);
    gStyle->SetOptFit(0000);
    gStyle->SetPadBorderMode(0);
    gStyle->SetPadBottomMargin(0.15);
    gStyle->SetPadLeftMargin(0.15);
    gStyle->SetPadRightMargin(0.05);
    gStyle->SetFrameBorderMode(0);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetPalette(0);
    gStyle->SetCanvasColor(0);
    gStyle->SetPadColor(0);
    
    TH1::AddDirectory(kFALSE);
    //This line is important.  But so stupid. 
    //If not included, when histos are read in they attach to the file they are from.
    //When the file goes out of scope, the histo dies and your code will seg fault.
    
    //Fitting variables
    Double_t dm2,sin22th;
    //Double_t dm2BF=1.2, sin22thBF=0.003; //The LSND best fit values of the parameters
    Double_t chisq;
    Double_t deltachisq90=1.64, deltachisq3s=9.00, deltachisq5s=25.00, deltachisq99=2.58;
    
    //output ntuple with chi2 values and sensitivity contour
    TNtuple* chi2 = new TNtuple("chi2","chi2","chisq:dm2:sin22th");
    //sensitivity contours
    TGraph *sens90; Double_t x90[npoints+1]; Double_t y90[npoints+1];
    // TGraph *sens99;
    Double_t x99[npoints+1]; Double_t y99[npoints+1];
    TGraph *sens3s; Double_t x3s[npoints+1]; Double_t y3s[npoints+1];
    TGraph *sens5s; Double_t x5s[npoints+1]; Double_t y5s[npoints+1];

    //This loop calls the routine to read all of the ntuples.  We then read them in after.
    //You could consider changing the oscillation model by making cuts in the program that
    //reads the ntuples, which is read_ntuple.C and read_ntuple_fosc.C.
    //It might be easier to reprocess the histograms though.
    for (int i = 0; i < nL; i++){
        Int_t L = baselines[i];
        Int_t returnVal;
        returnVal = readerNue[i].processData();
        if (returnVal) {  //only returns 0 on successful completion
            std::cout << "Error: failed to read the ntuple at " << L << " with error " << returnVal;
            std::cout << std::endl;
            return 1;
        }
        returnVal = readerNumu[i].processData();
        if (returnVal) {  //only returns 0 on successful completion
            std::cout << "Error: failed to read the ntuple at " << L << " with error " << returnVal;
            std::cout << std::endl;
            return 1;
        }
    }
    
        
    //Now we set up the histograms that we read the event rates into:
    //Each gets one histogram per baseline nL
    // TH1F *eventsnue[nL];                   //nueb
    // TH1F *eventsnumu[nL];                  //numu
    // TH1F *eventsnueosc[nL][npoints+1];     //fullosc weighted by Posc=1*sin2(1.27dm2L/E), at each dm2 point
    // TH1F *eventsnuefosc[nL];               //fullosc (Posc=1)
    //TH1F *eventsSignalBestFitNu[nL];        //Number of events expected at the best fit.
    //TH1F *eventsSignalBestFitNubar[nL];     //Number of events expected at the best fit.    
    
    //all of these vectors are exactly corresponding to the histograms above
    //except for oscVev, its name[b_line][energy_bin]
    std::vector< std::vector <float> >    eventsnumuVec;
    std::vector< std::vector <float> >     eventsnueVec;
    std::vector<std::vector< std::vector <float> > >    eventsnueoscVec;
    //scheme is: eventsnueoscVec[b_line][point][energybin]
    std::vector< std::vector <float> >    eventsnuefoscVec;
    std::vector< std::vector <float> >	eventsSignalBestFitNuVec;
    std::vector< std::vector <float> >	eventsSignalBestFitNubarVec;
    
    //Make sure the vectors are the right size!
    eventsnueVec.resize(nL);
    eventsnumuVec.resize(nL);
    eventsnueoscVec.resize(nL);
    eventsnuefoscVec.resize(nL);
    eventsSignalBestFitNuVec.resize(nL);
    eventsSignalBestFitNubarVec.resize(nL);

    //Now actually read in the histograms, which are in files generated by read_ntuple.C and _fosc.C
    //Sure, you could include this loop above, but it is easier logically to read all ntuples 
    //and then import all the results.  And that's what we're going for here.
    //You shouldn't have to change anything in this loop if you don't edit read_ntuple.C (or fosc)
    for(int b_line = 0; b_line < nL; b_line++){
        //For each detector, we read in 3 files of histograms, nuebackground, numubkg, fosc
        // eventsnueoscVec[b_line].resize(npoints+1); // make room for the osc vectors

        eventsnueVec[b_line]     = readerNue[b_line].GetData();
        eventsnueoscVec[b_line]  = readerNue[b_line].GetDataOsc();
        eventsnumuVec[b_line]    = readerNumu[b_line].GetData();
        eventsnuefoscVec[b_line] = readerNumu[b_line].GetData();
        std::transform( eventsnuefoscVec[b_line].begin(), eventsnuefoscVec[b_line].end(),
            eventsnuefoscVec[b_line].begin(), 
            std::bind2nd(std::multiplies<float>(), 0.003));
        // That last line is getting the nue background again but it should really be getting
        // the numu background!!
            
    }//end loop over baselines
    
    //Everything is read, now scale the vectors:
    for (int b_line = 0; b_line < nL; b_line ++){
        //using the stl to do the scaling.  
        //transform works on each element from the first arg to the second (takes iterators)
        //and stores the result in the range starting at 3rd arg.  In this case, it's an overwrite
        //bind2nd takes a binary operation (multiplies, in this case) and creates a unary op
        //by always using the same value as the second operand (scales[b_line] in this case)
        //If you're unsure, check out:
        //http://www.cplusplus.com/reference/algorithm/transform/
        //http://www.cplusplus.com/reference/functional/bind2nd/
        std::transform( eventsnueVec[b_line].begin(), eventsnueVec[b_line].end(),
                        eventsnueVec[b_line].begin(), 
                        std::bind2nd(std::multiplies<float>(), scales[b_line]));
        std::transform( eventsnumuVec[b_line].begin(), eventsnumuVec[b_line].end(),
                        eventsnumuVec[b_line].begin(), 
                        std::bind2nd(std::multiplies<float>(), scales[b_line]));
        std::transform( eventsnumuVec[b_line].begin(), eventsnumuVec[b_line].end(),
                        eventsnumuVec[b_line].begin(), 
                        std::bind2nd(std::plus<float>(), 100.0));
        std::transform( eventsnuefoscVec[b_line].begin(), eventsnuefoscVec[b_line].end(),
                        eventsnuefoscVec[b_line].begin(), 
                        std::bind2nd(std::multiplies<float>(), scales[b_line]));
        std::transform( eventsSignalBestFitNuVec[b_line].begin(), eventsSignalBestFitNuVec[b_line].end(),
                        eventsSignalBestFitNuVec[b_line].begin(), 
                        std::bind2nd(std::multiplies<float>(), scales[b_line]));
        std::transform( eventsSignalBestFitNubarVec[b_line].begin(), eventsSignalBestFitNubarVec[b_line].end(),
                        eventsSignalBestFitNubarVec[b_line].begin(), 
                        std::bind2nd(std::multiplies<float>(), scales[b_line]));
        for (int point = 0; point <= npoints; point ++){
            std::transform( eventsnueoscVec[b_line][point].begin(),
                            eventsnueoscVec[b_line][point].end(),
                            eventsnueoscVec[b_line][point].begin(), 
                            std::bind2nd(std::multiplies<float>(), scales[b_line]));		
		} //loop over points
	} //loop over baselines, ends scaling
	
	
	//Alright, we've got the histos for each baseline as well as the oscillated histos!
	//set up the histograms that hold the side by side events:
	std::vector < std::vector< float> > eventsnLVec;	//has osc in it
	std::vector < std::vector< float> > eventsnLfitVec; //holds collapsed eventsnLVec spectrum, for fit
	std::vector <float> eventsnLnullVec; 			//has no osc in it
	std::vector <float> eventsnLcvVec;				//has fullosc*flat0.3% osc prob in it
	eventsnLVec.resize(npoints+1);
	eventsnLfitVec.resize(npoints+1);
	
	
    TH1F *eventsnL[npoints+1];                    //has osc in it
    TH1F *eventsnLcv;                             //has fullosc*flat 0.3% osc prob in it
    TH1F *eventsnLnull;                           //has no osc in it
    TH1F *eventsnLfit[npoints+1];                 //holds collapsed eventsnL[] spectrum, for fit
	
	//Stitch together the histograms into the side by side versions (over npoints):
	//loop over npoints for eventsnL and eventsnLfit

    //=============================================
    //this histogram is as a blank histogram
    std::vector<float> blankVector(nbinsE, 0); 


	//Fill the combined histograms: 
	for (int point = 0; point <= npoints; point++){
		//this isn't elegant but it gets the job done:
		if (nL == 1){ //only one baseline
			eventsnLVec[point] 	= appendVectors(eventsnueoscVec[0][point], eventsnueVec[0], eventsnumuVec[0]);
			eventsnLfitVec[point] = eventsnLVec[point];
			eventsnL[point] 	= makeHistogram(eventsnLVec[point]		, 0	, nbins);
			eventsnLfit[point]	= makeHistogram(eventsnLfitVec[point]	, 0	, nbins);
			if (point == 0){
			  eventsnLcvVec 	= appendVectors(eventsnuefoscVec[0]	, eventsnueVec[0], eventsnumuVec[0]);
			  eventsnLnullVec = appendVectors(blankVector			, eventsnueVec[0], eventsnumuVec[0]);
			  eventsnLcv 		= makeHistogram(eventsnLcvVec		, 0	, nbins);
			  eventsnLnull 	= makeHistogram(eventsnLnullVec		, 0	, nbins);
			}
		}
		else if (nL == 2) { //two baselines
		  //append the vectors:
		  eventsnLVec[point] = appendVectors(	appendVectors(eventsnueoscVec[0][point], eventsnueVec[0], eventsnumuVec[0]),
							appendVectors(eventsnueoscVec[1][point], eventsnueVec[1], eventsnumuVec[1]));
		  eventsnLfitVec[point] = eventsnLVec[point];
		  //make them into histograms, just for display:
		  eventsnL[point] 	= makeHistogram(eventsnLVec[point]		, 0	, nbins);
		  eventsnLfit[point]	= makeHistogram(eventsnLfitVec[point]	, 0	, nbins);
		  if (point == 0){
		    eventsnLcvVec 	= appendVectors(appendVectors(eventsnuefoscVec[0]	, eventsnueVec[0], eventsnumuVec[0]),
							appendVectors(eventsnuefoscVec[1]	, eventsnueVec[1], eventsnumuVec[1]));
		    eventsnLnullVec = appendVectors(appendVectors(blankVector			, eventsnueVec[0], eventsnumuVec[0]),
						    appendVectors(blankVector			, eventsnueVec[1], eventsnumuVec[1]));
		    eventsnLcv 		= makeHistogram(eventsnLcvVec		, 0	, nbins);
		    eventsnLnull 	= makeHistogram(eventsnLnullVec		, 0	, nbins);
		  }
		}
		else if (nL == 3) { //three baselines
		  //append the vectors:
		  eventsnLVec[point] = appendVectors(	appendVectors(eventsnueoscVec[0][point], eventsnueVec[0], eventsnumuVec[0]),
							appendVectors(eventsnueoscVec[1][point], eventsnueVec[1], eventsnumuVec[1]),
							appendVectors(eventsnueoscVec[2][point], eventsnueVec[2], eventsnumuVec[2]));
		  eventsnLfitVec[point] = eventsnLVec[point];
		  //make them into histograms, just for display:
		  eventsnL[point] 	= makeHistogram(eventsnLVec[point]		, 0	, nbins);
		  eventsnLfit[point]	= makeHistogram(eventsnLfitVec[point]	, 0	, nbins);
		  if (point == 0){
		    eventsnLcvVec 	= appendVectors(appendVectors(eventsnuefoscVec[0]	, eventsnueVec[0], eventsnumuVec[0]),
												appendVectors(eventsnuefoscVec[1]	, eventsnueVec[1], eventsnumuVec[1]),
							appendVectors(eventsnuefoscVec[2]	, eventsnueVec[2], eventsnumuVec[2]));
		    eventsnLnullVec = appendVectors(appendVectors(blankVector			, eventsnueVec[0], eventsnumuVec[0]),
						    appendVectors(blankVector			, eventsnueVec[1], eventsnumuVec[1]),
						    appendVectors(blankVector			, eventsnueVec[2], eventsnumuVec[2]));
				eventsnLcv 		= makeHistogram(eventsnLcvVec		, 0	, nbins);
				eventsnLnull 	= makeHistogram(eventsnLnullVec		, 0	, nbins);
		  }	
		}
		else { //4+ baselines.  No way!
		  std::cout << "This macro doesn't handle more than 3 baselines." << std::endl;
		}
	}
	
	if (verbose){
		std::cout << "\n------------------------\nPrinting out P=0.3% values and P=0.0% values:\n";
		std::cout << "\tP=0.3%\t\tP=0\t\tosc"<<npoints/2<<"\n";
		for (unsigned int i = 0; i < eventsnLcvVec.size(); i++){
			std::cout << "\t"<<eventsnLcvVec[i] << "\t\t" << eventsnLnullVec[i] << "\t\t";
			std::cout << eventsnLfitVec[0][i] << "\n";
		}
		std::cout << "------------------------" << std::endl;
	}

    printf("\n====> Side-by-side Histogram[dm2] arrays have been filled\n");
    TCanvas *c2 = new TCanvas("c2","sidebyside",800,800);
    c2->Divide(1,2);
    eventsnLcv->SetTitle("Side-by-side distribution, P_{osc}=0.3%;Reconstructed Energy Bin;Events");
    c2->cd(1); gPad->SetLogy(); eventsnLcv->Draw();
    eventsnLnull->SetTitle("Side-by-side distribution, null;Reconstructed Energy Bin; Events");
    c2->cd(2); gPad->SetLogy(); eventsnLnull->Draw();    

// return 0;
    
		
	//Input fractional systematics covariance matrix
    //This is going to come from a txt file, from Georgia
    std::vector<std::vector<float> > fracentries( nbins, std::vector<float> ( nbins, 0.0 ) );
    //initialized to zero
	
	if (cov_max_name.size()) buildCovarianceMatrix(fracentries, cov_max_name, nbins);
	//if there is nothing in the covariance matrix vector, going to use near detector
	//statistics as the systematics downstream.  This involves going and finding
	//the near detector statistics in each case, and then populating the covariance matrix
	//Since we can't do this until we're reading in the event rates, there's no point to do
	//it here.  But we can read in the near detector stats:
	//Only have useful near det stats for nue and numu, nothing on signal.  However,
	//going to make an array 3*nbinsE long and just leave the signal entries at zero
	std::vector<float> nearDetStats;


    // For shape only fits, the near det stats get recalculated each time chi2 gets
    // calculate.  But we do them once out here too, for the shape+rate fit.
	if (!cov_max_name.size()){
		//by design, the nearest detector to the source is the first in the vector
		//of baselines.
		nearDetStats.resize(nbinsE*3);
		for (unsigned int i = nbinsE; i < nearDetStats.size(); i++){
			//leaving the first nbinsE entries at 0 for the signal bins
			//for bins nbinsE -> 2*nbinsE-1, use 1/sqrt(nue events in that bin)
			if (i < 2*nbinsE) nearDetStats[i] = 1/sqrt(eventsnueVec[0][i-nbinsE]);
			else nearDetStats[i] = 1/sqrt(eventsnumuVec[0][i-2*nbinsE]);
		}
		if(verbose){
			std::cout << "\nNo covariance matrix entries, calculating near det errors.\n";
			std::cout << "The fractional errors are: " << std::endl;
			for (unsigned int i = 0; i < nearDetStats.size(); i++){
				std::cout << "\tBin " << i << ":\t" << nearDetStats[i] << "\n";
			}
		}
		
	}
	std::cout << std::endl;
	//now either the covariance matrix is set, or the frac entries is filled.
	//nearDetStats is only nonezero size if the cov_max_name is empty
	
	//This vector holds the null oscillation collapsed signal (in other words,
	//it's the nue and numu appended).  It's used in the chisq calc.
	std::vector<float> nullVec(nbins-nL*nbinsE,0);
	// nullVec = collapseVector(eventsnLnullVec, nbinsE, nL);

	//This loop is actually doing the chisq calculation
	//It loops over npoints twice.  First over dm2, and second over sin22th
	for (int dm2point = 0; dm2point <= npoints; dm2point ++){
		std::cout << "dm^2: " << dm2point << " of " << npoints << std::endl;			
		for (int sin22thpoint = 0; sin22thpoint <= npoints; sin22thpoint++){
			chisq = 0.0; //reset the chisq!

			//First, figure out what dm2 and sin22th are for this iteration:
			dm2 = pow(10.,(TMath::Log10(dm2min)+(dm2point*1./npoints)*TMath::Log10(dm2max/dm2min)));
			sin22th = pow(10.,(TMath::Log10(sin22thmin)+(sin22thpoint*1./npoints)*TMath::Log10(sin22thmax/sin22thmin)));
			if (debug) std::cout << "---------------------------Begin dm2: " << dm2 << ",\tsin22th: " << sin22th << std::endl;

			//Now build the covariance matrix for this point on (dm2, sin22th)
			//The current matrix is just the fractional systematics.
			//This means scaling each entry by the central values
			//And also means adding statistical errors on the diagonal.
			//If cov_max_name.size() is zero, the use either flat systematics or nearDetStats
			

			
            // In the case of the shape only fit,
            // Copy the unscaled vector that will be used to build error matrix
            // Probably need to scale it as well
            std::vector< float > eventsnLVecTemp = eventsnLVec[dm2point];

            // Set up the predictive vectors:

            //Collapsed spectrum(dm2,s22th)
            //PredictionVec holds background + signal
            std::vector<float> predictionVec(nbins-nL*nbinsE,0);
            // Null vec holds the rates with no oscillation signal
            // But in the shape only analysis, we normalize this to account
            // for high dm2 signal affecting the "null" background
            nullVec = collapseVector(eventsnLnullVec, nbinsE, nL);
            //before collapsing the prediction, need to scale the oscillated signal prediction:
            std::vector<float> eventsFitVecTemp = eventsnLfitVec[dm2point]; //don't want to overwrite...
            for (int i = 0; i < nL; i ++){
                std::transform( eventsFitVecTemp.begin() + i*nbinsE*3, //start here
                                eventsFitVecTemp.begin() + i*nbinsE*3 + nbinsE, //end here
                                eventsFitVecTemp.begin() + i*nbinsE*3, //put it here (replace)
                                std::bind2nd(std::multiplies<double>(), sin22th)); //use this unary operation
                //starts at eventsnLfit
            }

            // Here's where the shape only stuff comes into play
            double intSignalND = 0.;    double intNooscND = 0.;
            std::vector<double> scalefac;

            predictionVec = collapseVector(eventsFitVecTemp, nbinsE, nL);
            // Now predictionVec and nullVec hold 
			if(shapeOnlyFit){
                // Loop over the near det and determine the ratio between 
                // background and background + signal
                for(int i = 0; i < nbinsE; i ++){
                    intSignalND = predictionVec[i];
                    intNooscND  = nullVec[i];
                    scalefac.push_back(intSignalND / intNooscND);
                }
                // if (verbose) for (auto & point : scalefac) std::cout << " Scalefac: " << point << "\n";
                
                // next scale the nue background for each det
                for(int n = 0; n < nL; n++)
                {
                    for (int i = 0; i < nbinsE; ++ i)
                    {
                        if (debug) std::cout << "\nScaled from: " << nullVec[2*n*nbinsE + i] << std::endl;
                        nullVec[2*n*nbinsE + i] *= scalefac[i];
                        if (debug) std::cout << "Scaled to: " << nullVec[2*n*nbinsE + i] << std::endl;
                    }
                }
            }
            // At this point, prediction vec is the 2*nbinsE*nL long vector that holds
            // the null+signal
            // nullVec is the (corrected, if necessary) background
            // That means that the central values need to be read out from nullVec
            // It also means that the nearDetStats need to come from nullVec
            // Therefore, dig those out:
            if (shapeOnlyFit)
            {
                for (int i = 0; i < nbinsE*2; ++i)
                {
                    nearDetStats[i] = 1/sqrt( nullVec.at(i+nbinsE) );
                }
            }

			//initialize the covariance matrix array:
			std::vector< std::vector<float> > entries( nbins, std::vector<float> ( nbins, 0.0 ) );

			
			if (cov_max_name.size() == 0){
				//Only dealing with diagonal entries, just loop over them:
				for (int ibin = 0; ibin < nbins; ibin++){
					//start by getting the central value for this entry
					// getting the central value depends on whether or not shape only
                    // if not shape only, it's easy:
                    double cvi = eventsnLVecTemp[ibin];
                    if (shapeOnlyFit)
                    {
                        if (ibin%(3*nbinsE) < nbinsE) //on a signal bin
                        {
                            int index = ibin - ((int) ibin/(3*nbinsE))*nbinsE;
                            // std::cout << "ibin is: " << ibin << " and accessing nullvec at "
                            // << index;
                            cvi = abs( predictionVec.at(index) - nullVec.at(index) );
                            // std::cout << ". cvi is prediction - nullvec: " << cvi << std::endl;
                            if (cvi > 10e10){
                                std::cout << "\tprediction is " << predictionVec[ibin] << std::endl;
                                std::cout << "\tnullVec is " << nullVec.at(ibin - ((int) ibin/(3*nbinsE))*nbinsE) << std::endl;                                
                            }
                             // Make the cvi the difference between prediction and null
                        }
                        else
                        {
                            // std::cout << "ibin is: " << ibin << " and accessing nullvec at "
                              // << ibin - nbinsE - ((int) ibin/(3*nbinsE))*nbinsE;
                            cvi = nullVec.at(ibin - nbinsE - ((int) ibin/(3*nbinsE))*nbinsE);
                            // std::cout << ". cvi is nullvec only: " << cvi << std::endl;
                            // else using the null results.
                        }
                    }
                    if (debug) std::cout << "ibin: " << ibin << ", cvi: "<< cvi << std::endl;
					//looking at the simulated signal for this if:
					if ( (ibin%(nbinsE*3)) < nbinsE ) cvi *= sin22th; //scale the oscillated signal
					
					if (useNearDetStats){ //using near detector stats
						if (ibin < 3*nbinsE){ //on the near detector
							entries[ibin][ibin] = cvi + pow(nearDetSystematicError*cvi, 2); 
							//statistical^2 + systematic^2 errors
						} // end if on "is near det"
						
						else{ //on far detectors
							entries[ibin][ibin] = cvi + pow(nearDetStats[ibin % (3*nbinsE)]*cvi, 2);
						} // end else, end section on far dets
					} //end if on useNearDetStats
					
					else{ //using flat systematics on everything
						entries[ibin][ibin] = cvi + pow(flatSystematicError*cvi, 2);
					} //end else on 5% flat stats
				
                    if (isinf(entries[ibin][ibin])) {
                        std::cout << "ibin is " << ibin << std::endl;
                        debug = true;
                        // return -1;
                    }

                } //end loop over bins, is really loop over diagonal entries
			} //end if on "cov_max_name.size() == 0"			
			else{ //use the full blown covariance matrix.  Loop over every entry.
				//Can't forget to add in the statistical errors on the diagonal!
				for (int ibin = 0; ibin < nbins; ibin++){
					for (int jbin = 0; jbin < nbins; jbin ++){
						double cvi = eventsnLVecTemp[ibin];
						double cvj = eventsnLVecTemp[jbin];
						if ( (ibin%(nbinsE*3)) <nbinsE ) cvi *= sin22th; //scale the oscillated signal
						if ( (jbin%(nbinsE*3)) <nbinsE ) cvj *= sin22th; //scale the oscillated signal

						//Now scale the entries:
						entries[ibin][jbin] = fracentries[ibin][jbin]*cvi*cvj;
						//add statistical errors on the diagonal:
						if (ibin == jbin){
							entries[ibin][jbin] += cvi; //cvi should equal cvj here
						}
					} //end loop on jbin
				} //end loop on ibin
			
			} //end section that is using full covariance matrix
			
			//At this point, the covariance matrix is built in full in "entries"
			//We need to collapse everything, though.
			
			//for debugging, can print out covariance matrix:
			if (debug){
				for (unsigned int i = 0; i < entries.size(); i ++){
					std::cout << i << "\t";
					for (unsigned int j = 0; j < entries[i].size(); j++){
						std::cout << entries[i][j] << " ";
					}
					std::cout << std::endl;
				}
			}

			Float_t * entriescollapsed = (collapseMatrix(entries,nbinsE, nL));
			//collapsed!
			//if debug is on, print out the collapsed matrix:
			if (debug){
				//the matrix should be nbins- nL*nbinsE on each side.  Print:
				std::cout << "\nPrinting collapsed covariance matrix:\n";
				unsigned int side = nbins-nL*nbinsE;
				for (unsigned int i = 0; i < side; i ++){
					std::cout << i << "\t";
					for (unsigned int j = 0; j < side; j++){
						std::cout << entriescollapsed[i*side + j] << " ";
					}
					std::cout << std::endl;
				}
			}
			
			//Now create the inverse covariance matrix.  Root can invert for us:
			TMatrix *M = new TMatrix(nbins-nL*nbinsE,nbins-nL*nbinsE,entriescollapsed);
            //inverse cov matrix, M^{-1}, used in chi2 calculation
            TMatrix *cov = new TMatrix(nbins-nL*nbinsE,nbins-nL*nbinsE);
            cov = &(M->Invert());	//this is used in chi2 calculation

if (debug == true) return 1;

			//Checking collapsing:
			if (debug){
				std::cout << "Pre\t\tPost\n";
				for (unsigned int i = 0; i < eventsnLfitVec[dm2point].size(); i++){
					std::cout << eventsnLfitVec[dm2point][i] << "\t\t";
					if (i < predictionVec.size())  std::cout << predictionVec[i];
					std::cout << "\n";
				}
			}




            // Make some plots:
            if (shapeOnlyFit && dm2point == (int) npoints*0.9 )
            {
                if (sin22thpoint == (int) npoints/2)
                {
                    std::cout << "Using dm2: " << dm2 << std::endl;
                    std::cout << "Using sin22th: " << sin22th << std::endl;
                    // std::cout << "Scalefac: " << scalefac << std::endl;
                    std::vector<float> tempVec = collapseVector(eventsnLnullVec, nbinsE, nL);
                    TH1F * unscaled = makeHistogram(tempVec, 0, nbinsE);
                    TH1F * background = makeHistogram(nullVec, 0, nbinsE);
                    TH1F * sig = makeHistogram(predictionVec, 0, nbinsE);
                    unscaled -> SetLineColor(1);
                    background -> SetLineColor(2);
                    sig -> SetLineColor(4);

                    TLegend * scaleLegend = new TLegend(0.65,0.55,0.82,0.75);
                    scaleLegend->SetFillStyle(0);
                    scaleLegend->SetFillColor(0);
                    scaleLegend->SetBorderSize(0);
                    scaleLegend->SetTextSize(0.025);
                
                    scaleLegend -> AddEntry(unscaled, "Unscaled events");
                    scaleLegend -> AddEntry(background, "background events");
                    scaleLegend -> AddEntry(sig, "scaled events");

                    TCanvas * c65 = new TCanvas("c65","Stupid",800,400);
                    c65 -> cd();
                    unscaled ->Draw();
                    background ->Draw("same");
                    sig -> Draw("same");
                    scaleLegend -> Draw();
                }
            }



            //This is the actual chi2 calculation
            //Energy spectrum fit
            //loop over collapsed bins
            for (int ibin=1; ibin<=nbins-nL*nbinsE; ibin++)
            {
                //loop over collapsed bins
                for (int jbin=1; jbin<=nbins-nL*nbinsE; jbin++)
                {

                    float cvi = nullVec[ibin-1];
                    float cvj = nullVec[jbin-1];

                    // The prediction doesn't change
                    float predictioni = predictionVec[ibin-1];
                    float predictionj = predictionVec[jbin-1];
                    
                    if (debug){
                        if (ibin == jbin){
                            std::cout << "ibin: "<< ibin << " Prediction: " << predictioni << " cvi: " << cvi;
                            std::cout << " M^-1: " << (*cov)(ibin-1,jbin-1) << std::endl;
                        }
                    }
                  
                    chisq += (predictioni-cvi)*(predictionj-cvj)* (*cov)(ibin-1,jbin-1);

                }
            }
            
            // return 0;
            //write model in file
            chi2->Fill(chisq,dm2,sin22th);

            if (chisq<=deltachisq5s)
            {
                x5s[dm2point] = sin22th;
                y5s[dm2point] = dm2;
            }
            if (chisq<=deltachisq3s)
            {
                x3s[dm2point] = sin22th;
                y3s[dm2point] = dm2;
            }
            if (chisq<=deltachisq90)
            {
                x90[dm2point] = sin22th;
                y90[dm2point] = dm2;
            }
            if (chisq<=deltachisq99)
            {
                x99[dm2point] = sin22th;
                y99[dm2point] = dm2;
            }
            
        if (verbose) std::cout << "dm2: " << dm2 << ",\tsin22th: " << sin22th << ",\tchisq: " << chisq << std::endl;    
        if (debug) std::cout << "\n\n";
        if (debug) std::cout << "---------------------------End dm2: " << dm2 << ",\tsin22th: " << sin22th << std::endl;
        }// end loop over sin22thpoints
    } // end loop over dm2points
    
    
    //Plot Results:
    sens90 = new TGraph(npoints+1,x90,y90); sens90->SetLineColor(1); sens90->SetLineWidth(2);
    //sens99 = new TGraph(npoints+1,x99,y99); sens99->SetLineColor(9); sens99->SetLineWidth(2);
    sens3s = new TGraph(npoints+1,x3s,y3s); sens3s->SetLineColor(9); sens3s->SetLineWidth(2);
    sens5s = new TGraph(npoints+1,x5s,y5s); sens5s->SetLineColor(9); sens5s->SetLineStyle(2); sens5s->SetLineWidth(1);
    
    TString chi2FileName = fileNameRoot+mode;
    if(shapeOnlyFit){
        if (useNearDetStats){
            chi2FileName = chi2FileName+"_nearDetStats_shapeOnly_chi2.root";
        }
        else{
            chi2FileName = chi2FileName+"_flatStats_shapeOnly_chi2.root";
        }
    }
    else{
        if (useNearDetStats){
            chi2FileName = chi2FileName+"_nearDetStats_chi2.root";
        }
        else{
            chi2FileName = chi2FileName+"_flatStats_chi2.root";
        }
    }

    std::cout << "Writing chi2 to " << chi2FileName << std::endl;

    //write the results to file:
    TFile *file1 = new TFile(chi2FileName,"RECREATE");
    chi2->Write();
    sens90->Write();
    // sens99->Write();
    sens3s->Write();
    sens5s->Write();
    file1->Close();
    
        
    //======================================================
    printf("\nDrawing sensitivity curves...\n");

    cout<<"Drawing LSND intervals...\n\n";

    TCanvas* c3 = new TCanvas("c3","Sensitivity",700,700);
    c3->SetLogx();
    c3->SetLogy();

    TH2D* hr1=new TH2D("hr1","hr1",500,sin22thmin,sin22thmax,500,dm2min,dm2max);
    hr1->Reset();
    hr1->SetFillColor(0);
    hr1->SetTitle(";sin^{2}2#theta_{#mue};#Deltam^{2} (eV^{2})");
    hr1->GetXaxis()->SetTitleOffset(1.1);
    hr1->GetYaxis()->SetTitleOffset(1.2);
    hr1->GetXaxis()->SetTitleSize(0.05);
    hr1->GetYaxis()->SetTitleSize(0.05);
    hr1->SetStats(kFALSE);
    hr1->Draw();
    gROOT->ProcessLine(".x ./lsnd_plot.c+(c3)");

    //======================================================

    TLegend* legt=new TLegend(0.68,0.55,0.86,0.72);
    legt->SetFillStyle(0);
    legt->SetFillColor(0);
    legt->SetBorderSize(0);
    legt->SetTextSize(0.025);
    //     legt->AddEntry("",running,"");
    //     legt->AddEntry("",datapot,"");
    //     
    //Determine the lines in the legend.  Print the detectors first:
    // TString detectors.
    // add_plot_label()
    //if (mode == "nu") legt ->AddEntry("", "#nu mode", "");
    //else if (mode == "nubar") legt ->AddEntry("", "#nubar mode", "");
    
    // char name[200];
    // char name2[200];
    // char name3[200];

    char name[3][200];

    for (int j=0; j<nL; j++) {
        if (baselines[j] == 100){
            if (use100m)
                sprintf(name[j], "LAr1-ND (%im)", baselines[j]);
            else if (use100mLong)
                sprintf(name[j], "2*LAr1-ND (%im)", baselines[j]);
        }
        else if (baselines[j] == 470){
            sprintf(name[j], "MicroBooNE (%im)", baselines[j]);
        }
        else if (baselines[j] == 700){
            if (use700m) sprintf(name[j], "LAr1-FD (%im)", baselines[j]);
            else if (useT600) sprintf(name[j], "T600 (%im)", baselines[j]);
        }
    }


    //legt->AddEntry("",name,"");
    
    for (int i = 0; i < nL;i++){
        std::cout << "Name is " << name[i] << std::endl;
        add_plot_label(name[i],  0.77, 0.87-i*0.04, 0.026, 1, 62);
    }  

    add_plot_label(label2, 0.77, 0.76, 0.023, 1, 62);
    add_plot_label(label1, 0.77, 0.73, 0.023, 1, 62);
    // legt->AddEntry("",label,"");

    sens90->Draw("l same");
    legt->AddEntry(sens90,"90\% CL","l");
    // sens99->Draw("l same");
    // legt->AddEntry(sens99,"99\% CL","l");
    sens3s->Draw("l same");
    legt->AddEntry(sens3s,"3#sigma CL","l");
    sens5s->Draw("l same");
    legt->AddEntry(sens5s,"5#sigma CL","l");

    legt->Draw();

    // if (specialNameText != ""){
    //     char tempChar[20];
    //     sprintf(tempChar, "%s",specialNameText.c_str());
    //     add_plot_label(tempChar, 0.75, 0.25);
    // }
    // if (specialNameTextOsc != "") {
    //     char tempChar[20];
    //     sprintf(tempChar, "%s",specialNameTextOsc.c_str());
    //     add_plot_label(tempChar, 0.75, 0.3);
    // }
    //add_plot_label(label, label_x, label_y, 0.05, 1, 62, 22 );

    TLegend* leg3=new TLegend(0.2,0.2,0.4,0.35);
    leg3->SetFillStyle(0);
    leg3->SetFillColor(0);
    leg3->SetBorderSize(0);
    leg3->SetTextSize(0.03);
    TGraph *gdummy1 = new TGraph();
    gdummy1->SetFillColor(29);
    TGraph *gdummy2 = new TGraph();
    gdummy2->SetFillColor(38);
    TMarker *gdummy3 = new TMarker();
    gdummy3 -> SetMarkerStyle(3);
    gdummy3 -> SetMarkerColor(1);
    TMarker *gdummy4 = new TMarker();
    gdummy4 -> SetMarkerStyle(34);
    gdummy4 -> SetMarkerColor(1);
    TGraph *gdummy0 = new TGraph();
    gdummy0 -> SetFillColor(1);
    gdummy0 -> SetFillStyle(3254);
    leg3->AddEntry(gdummy2,"LSND 90% CL","F");
    leg3->AddEntry(gdummy1,"LSND 99% CL","F");
    leg3->AddEntry(gdummy3,"LSND Best Fit","P*");
    leg3->AddEntry(gdummy4,"Global Best Fit","P*");
    leg3->AddEntry(gdummy0,"Global Fit 90% CL (J. Kopp et al. arXiv:1303.3011)");   
    leg3->Draw();
    
    TImage *img = TImage::Create();
    img -> FromPad(c3);
    if (specialNameText_far != "") fileNameRoot = fileNameRoot + specialNameText_far + "_";
    if (specialNameTextOsc_far != "") fileNameRoot = fileNameRoot + specialNameTextOsc_far + "_";
    if(shapeOnlyFit){
        if (useNearDetStats){
            c3 -> Print(fileNameRoot+mode+"_nearDetStats_shapeOnly.pdf", "pdf");
            c3 -> Print(fileNameRoot+mode+"_nearDetStats_shapeOnly.png", "png");
        }
        else{
            c3 -> Print(fileNameRoot+mode+"_flatStats_shapeOnly.pdf", "pdf");
            c3 -> Print(fileNameRoot+mode+"_flatStats_shapeOnly.png", "png");
        }
    }
    else{
    	if (useNearDetStats){
            c3 -> Print(fileNameRoot+mode+"_nearDetStats.pdf", "pdf");
            c3 -> Print(fileNameRoot+mode+"_nearDetStats.png", "png");
        }
        else{
            c3 -> Print(fileNameRoot+mode+"_flatStats.pdf", "pdf");
            c3 -> Print(fileNameRoot+mode+"_flatStats.png", "png");
        }
    }
    cout<<"\nEnd of routine.\n";
    
    return 0;
}

//make sure fracentries is initialized as [nbins][nbins] or this will definitely fall over
int buildCovarianceMatrix(std::vector<std::vector<float> > & fracentries, std::vector<std::string> & cov_max_name, int nbins){    
    for(std::vector<std::string>::iterator it = cov_max_name.begin(); it != cov_max_name.end(); it++){
        const char * name = (*it).c_str();
        double readInArray[nbins][nbins];
        std::cout << "\nReading in covariance matric from " << name << std::endl;
        if (fileExists(name) ){
            ifstream indata(name);
            for (int i = 0; i < nbins; i ++ ){
                std::string line;
                std::getline(indata, line);
                std::stringstream ss(line);
                for (int j = 0; j < nbins; j ++ ){
                    ss >> readInArray[i][j];
                }    
            }
            //Quick check on corner entries:
            //std::cout << "Covariance Matrix " << name << "is read, check entries...?" << std::endl;
            //std::cout << "Top left: " << readInArray[0][0] << std::endl;
            //std::cout << "Bottom left: " << readInArray[0][nbins-1] << std::endl;
            //std::cout << "Top right: " << readInArray[nbins-1][0] << std::endl;
            //std::cout << "Bottom right: " << readInArray[nbins-1][nbins-1] << std::endl;    
        }
        else{
            std::cout << "Error while building covariance matrix: can't find file: \n" ;
            std::cout << "\t" << name << std::endl;
            return 1;
        }
        //At this point, got the file!  Add it to fracentries and set readInArray back to zero:
        for (int i = 0; i < nbins; i ++ ){
            for (int j = 0; j < nbins; j++){
                fracentries[i][j] += readInArray[i][j];
            } //loop over j
        } //loop over i
    } //loop over cov_max_name
    return 0;
}
std::vector<float> appendVectors(std::vector<float>  vec1, std::vector<float>  vec2){        
    std::vector<float> result;
    unsigned int size1 = vec1.size();
    unsigned int size2 = vec2.size();
    result.resize(size1+size2);
    for (unsigned int i = 0; i < result.size(); i++){
        if (i < size1) result[i] = vec1[i];
        else result[i] = vec2[i-size1];
    }
    return result;
}
std::vector<float> appendVectors(std::vector<float>  vec1, std::vector<float>  vec2, std::vector<float>  vec3){
    std::vector<float> result;
    unsigned int size1 = vec1.size();
    unsigned int size2 = vec2.size();
    unsigned int size3 = vec3.size();
    result.resize(size1+size2+size3);
    for (unsigned int i = 0; i < result.size(); i++){
        if (i < size1) result[i] = vec1[i];
        else if (i < size1+size2) result[i] = vec2[i-size1];
        else result[i] = vec3[i-size1-size2];
    }
    return result;
}
TH1F* makeHistogram(std::vector<float> & input, double lowbin, double highbin){
    static int i = 0;
    char name[200];
    sprintf(name, "result_%i", i);
    TH1F * result = new TH1F(name,name,input.size(), lowbin, highbin);
    for (unsigned int j = 0; j < input.size(); j++){
        result ->SetBinContent(j+1, input[j]);
    }
    i++;
    return result;
}
std::vector<float> collapseVector(std::vector<float> input, int nbinsE, int nL){
    //this function takes a vector and collapses the fullosc block onto the nue background
    //make a vector to store the output:
    std::vector<float> output(nbinsE*nL*2,0);
    //Do this a simple but not super efficient way.  That's ok, though.
    //Take the input vector, and split it into nL pieces of 3*nbinsE each:
    std::vector<std::vector<float> > splitBaselines(nL, std::vector<float>(3*nbinsE,0));
    for (unsigned int i = 0; i < splitBaselines.size(); i++){
        std::copy( input.begin()+ (i*3*nbinsE), input.begin() + i*3*nbinsE + 3*nbinsE, splitBaselines[i].begin());
    }
    //Now everything is split, start putting things together in the output:
    for (unsigned int i = 0; i < splitBaselines.size(); i++){
        std::transform( splitBaselines[i].begin(), splitBaselines[i].begin() + nbinsE,
                        splitBaselines[i].begin()+ nbinsE, output.begin() + i*2*nbinsE,
                        std::plus<float>());
        std::copy(splitBaselines[i].begin()+2*nbinsE, splitBaselines[i].end(), output.begin()+ i*2*nbinsE + nbinsE);
    }
    return output;
}

# ifndef __CINT__
int main()
{
  multiple_detector_fit();
  return 0;
}
# endif

void organized_fitting(){
    multiple_detector_fit();
    return;
}

