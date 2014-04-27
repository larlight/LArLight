//############################################################################
//Author: Corey Adams	(corey.adams@yale.edu)
//February 25, 2013
//
// The macro plots the nue background event rates, as well as simulated signal,
// in the nue appearance 
//
//
//
//Inputs:
//(1) ntuple of neutrinos with enutrue, enurec, baseline, weight, nutype, etc...
//   for each baseline
//(2) ntuple of fully oscillated (numu-->nue) neutrinos with enutrue, erec, baseline
//   weight, nutype, etc... for each baseline (used for signal)
//
//	By default neutrino mode ntuples are normalized to 6.6e20 POT and antineutrino mode
//	is 10e20 POT.  You can change this by adjusting the scaling of event rates
// 	at baselines.
//
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


//for filling the histograms:
#if defined(__CINT__) && !defined(__MAKECINT__)
#include "NtupleReader.cxx+"
#else
#include "NtupleReader.cxx"
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
			-> 	Have a script that reads ntuples and fills histos, allows to change that
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
TH1F* makeHistogram(std::vector<float> & input, double lowbin, double highbin);

void add_plot_label( char* label, double x, double y, double size = 0.05, int color = 1, int font = 62, int align = 22 ){

  TLatex *latex = new TLatex( x, y, label );
  latex->SetNDC();
  latex->SetTextSize(size);
  latex->SetTextColor(color);
  latex->SetTextFont(font);
  latex->SetTextAlign(align);
  latex->Draw();

}



/*
//Some error codes I've whipped up:
1 	//Error reading the ntuples, check read_ntuple.C or read_ntuple_fosc.C
2	//No baselines!
3	//Couldn't open file with histograms
*/
int event_rate_calc()
{

//---------------Start of section you can alter to change the baselines:

	//Self explanatory, this is where the ntuples you want to use are located:

	char fileSource[200]="/Users/cja33/Desktop/LAr1ND/lar1Repos/lar1/MC2_proposal_ntuples/";
	// char fileSource[200]="/Users/cja33/Desktop/lar1Repos/lar1/CCQE/";
	// char fileSource[200]="/Users/cja33/Desktop/lar1Repos/lar1/Ecalo/";

	//Note: parts of this routine will write to that same folder, make sure its writable!

	bool verbose 	=false;	//verbose causes print outs of information that is good for checking
	bool debug 		=false;	//debug prints out info to make sure root is behaving...

  bool useHighDm = false;
  bool useGlobBF = false;

  std::string specialNameText = "IC_no_neut_det";
  std::string specialNameTextOsc = "IC_no_neut_osc";

	TString fileNameRoot = fileSource;
	fileNameRoot += "eventRates_";
	std::string mode = "nubar";	//beam mode to run in
	bool use100m = true;		//Include the detector at 100m?
	bool use470m = true;		//Include the detector at 470m?
	bool use700m = false;		//Include the detector at 700m?
  bool useT600 = true;

	bool forceRemake = false;
	
	//Note: there is no infrastructure to handle more than 3 baselines.
	//So, try at your own risk!
	
	Double_t ubooneScale = 1.0;	//Scale the event rates (uniformly across all events), uboone
	Double_t LAr1NDScale = (1.0);	//Scale the event rates (uniformly across all events), near det
	Double_t LAr1FDScale = (1.0);	//Scale the event rates (uniformly across all events), far det

  //Histogram definitions
  Double_t emin = 0.2;		//GeV
  Double_t emax = 3.0;		//GeV
  const Int_t nbinsE = 10;    //number of energy bins in each baseline, for each distribution (fosc, nue, or numu)
  
  std::string energyType = "ecalo1";
  // Options are etrue, eccqe, ecalo1, ecalo2;

  bool useNearDetStats = false; 			// Only matters if the covariance matrix vector is empty.
  // double nearDetSystematicError = 0.10; 	// Only matters if userNearDetStats = true
  // double flatSystematicError = 0.1;		// Only used if nearDetStats = false.
  std::vector<std::string>  cov_max_name;
  //cov_max_name.push_back(mode+"_covariance_matrix/output_fraccovmx_xsec.out");
    

    
//---------------End of section you should alter without care

  Int_t npoints = 1;


  if (use700m && useT600){
      std::cout << "Error: can't use both LAr1-FD and T600." << std::endl;
      return -1;
  }


	std::vector<int> baselines;
	std::vector<double> scales;
	std::vector<std::string> names;
	std::vector<double> volume;
	std::vector<NtupleReader> readerNue;
	std::vector<NtupleReader> readerNumu;
  
  //initialize all the baselines we'll use:
  if (use100m) baselines.push_back(100);
  if (use470m) baselines.push_back(470);
  if (use700m || useT600) baselines.push_back(700);
  //and they're corresponding scaling factors:
  if (use100m) scales.push_back(LAr1NDScale);
  if (use470m) scales.push_back(ubooneScale);
  if (use700m) scales.push_back(LAr1FDScale);
  if (useT600) scales.push_back(1.0);
  //some detector names just for fun:
  if (use100m) names.push_back("LAr1-ND");
  if (use470m) names.push_back("MicroBooNE");
  if (use700m) names.push_back("LAr1-FD");
  if (useT600) names.push_back("ICARUS");
  //fiducial volume FROM MC, also just for fun: (scale to get target volume)
  if (use100m) volume.push_back(60.6);
  if (use470m) volume.push_back(61.4);
  if (use700m) volume.push_back(909);
  if (useT600) volume.push_back(476);
  if (use100m){
    NtupleReader a("nue",fileSource, 100, mode, energyType, npoints, forceRemake);
    readerNue.push_back(a);
  }
  if (use470m){
    NtupleReader a("nue",fileSource, 470, mode, energyType, npoints, forceRemake);
    readerNue.push_back(a);
  }
  if (use700m) {
    NtupleReader a("nue",fileSource, 700, mode, energyType, npoints, forceRemake);
    a.setSpecialNameText(specialNameText);
    a.setSpecialNameTextOsc(specialNameTextOsc);
    readerNue.push_back(a);  }
  	if (useT600) {
      NtupleReader a("nue",fileSource, 700, mode, energyType, npoints, forceRemake);
      a.setSpecialNameText(specialNameText);
      a.setSpecialNameTextOsc(specialNameTextOsc);
      readerNue.push_back(a);
    }
  	//This value is the number of baselines:
  	Int_t nL = baselines.size();
	const Int_t nbins = nL*nbinsE*3;    //total number of energy bins for side-by-side distributions:
  	//just in case, if the number of baselines is 1 switch off nearDetStats:
	if (nL == 1) {
		useNearDetStats=false;
		//flatSystematicError = nearDetSystematicError;	
	}

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
  //     
  //     //Fitting variables
  //     Double_t dm2,sin22th;
	Double_t sin22thBF=0.003; //The LSND best fit values of the parameters (dm2 = 1.2)
  Double_t sin22thBF_glob=0.013; // (dm = 0.43)

  if (useGlobBF)
    sin22thBF = sin22thBF_glob;



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
	}
	

		
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

	NueFromNueCC_muonVec.resize(nL);
	NueFromNueCC_chargeKaonVec.resize(nL);
	NueFromNueCC_neutKaonVec.resize(nL);
	NueFromEScatterVec.resize(nL);
	NueFromNC_pi0Vec.resize(nL);
	NueFromNC_delta0Vec.resize(nL);
	NueFromNumuCCVec.resize(nL);
	DirtVec.resize(nL);
	OtherVec.resize(nL);



	//except for oscVev, its name[b_line][energy_bin]

	std::vector<std::vector< std::vector <float> > >	eventsnueoscVec;
	//scheme is: eventsnueoscVec[b_line][point][energybin]
	std::vector< std::vector <float> >	eventsSignalBestFitNuVec;
	std::vector< std::vector <float> >	eventsSignalBestFitNubarVec;
	
	//Make sure the vectors are the right size!
	eventsnueoscVec.resize(nL);
	eventsSignalBestFitNuVec.resize(nL);
	eventsSignalBestFitNubarVec.resize(nL);



	//Now actually read in the histograms, which are in files generated by read_ntuple.C and _fosc.C
	//Sure, you could include this loop above, but it is easier logically to read all ntuples 
	//and then import all the results.  And that's what we're going for here.
	//You shouldn't have to change anything in this loop if you don't edit read_ntuple.C (or fosc)
	for(int b_line = 0; b_line < nL; b_line++){
		//For each detector, we read in 3 files of histograms, nuebackground, numubkg, fosc
		// eventsnueoscVec[b_line].resize(npoints+1); // make room for the osc vectors
		
		eventsnueoscVec[b_line]  = readerNue[b_line].GetDataOsc();
		// Since this is the event rate calc function, gotta get all the goodies:
		NueFromNueCC_muonVec[b_line]       = readerNue[b_line].GetVectorFromTree
                                           ( (char*) "NueFromNueCC_muonVec");
		NueFromNueCC_chargeKaonVec[b_line] = readerNue[b_line].GetVectorFromTree
                                           ( (char*) "NueFromNueCC_chargeKaonVec");
		NueFromNueCC_neutKaonVec[b_line]   = readerNue[b_line].GetVectorFromTree
                                           ( (char*) "NueFromNueCC_neutKaonVec");
		NueFromEScatterVec[b_line]         = readerNue[b_line].GetVectorFromTree
                                           ( (char*) "NueFromEScatterVec");
		NueFromNC_pi0Vec[b_line]           = readerNue[b_line].GetVectorFromTree
                                           ( (char*) "NueFromNC_pi0Vec");
		NueFromNC_delta0Vec[b_line]        = readerNue[b_line].GetVectorFromTree
                                           ( (char*) "NueFromNC_delta0Vec");
		NueFromNumuCCVec[b_line]           = readerNue[b_line].GetVectorFromTree
                                           ( (char*) "NueFromNumuCCVec");
		DirtVec[b_line]                    = readerNue[b_line].GetVectorFromTree
                                           ( (char*) "DirtVec");
		OtherVec[b_line]                   = readerNue[b_line].GetVectorFromTree
                                           ( (char*) "OtherVec");
		
    if (useHighDm){
      eventsSignalBestFitNuVec[b_line]    = readerNue[b_line].GetVectorFromTree
                ( (char*) "edistrnueHighDmNuVec",(char*)"tvecfosc",true);
      eventsSignalBestFitNubarVec[b_line] = readerNue[b_line].GetVectorFromTree
                ( (char*) "edistrnueHighDmNubarVec",(char*)"tvecfosc",true);
    }
    else if (useGlobBF){
      eventsSignalBestFitNuVec[b_line]    = readerNue[b_line].GetVectorFromTree
                ( (char*) "edistrnueBestFit_globNuVec",(char*)"tvecfosc",true);
      eventsSignalBestFitNubarVec[b_line] = readerNue[b_line].GetVectorFromTree
                ( (char*) "edistrnueBestFit_globNubarVec",(char*)"tvecfosc",true);
    }
    else {
      eventsSignalBestFitNuVec[b_line]    = readerNue[b_line].GetVectorFromTree
                ( (char*) "edistrnueBestFitNuVec",(char*)"tvecfosc",true);
      eventsSignalBestFitNubarVec[b_line] = readerNue[b_line].GetVectorFromTree
                ( (char*) "edistrnueBestFitNubarVec",(char*)"tvecfosc",true);
    }

	}//end loop over baselines
	
  std::cout << "Got these values for nu signal: \n";
  std::cout << eventsSignalBestFitNubarVec[2][0] << ", "
            << eventsSignalBestFitNubarVec[2][1] << ", "
            << eventsSignalBestFitNubarVec[2][2] << "\n";

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

    //Scale the best fit signals:
    std::cout << "Using scale factor of " << sin22thBF << std::endl;
    std::transform(  eventsSignalBestFitNuVec[b_line].begin(), eventsSignalBestFitNuVec[b_line].end(),
            eventsSignalBestFitNuVec[b_line].begin(), 
            std::bind2nd(std::multiplies<float>(), scales[b_line]*sin22thBF));
    std::transform(  eventsSignalBestFitNubarVec[b_line].begin(), eventsSignalBestFitNubarVec[b_line].end(),
            eventsSignalBestFitNubarVec[b_line].begin(), 
            std::bind2nd(std::multiplies<float>(), scales[b_line]*sin22thBF));

    //Scale the various background correctly:
    std::transform(  NueFromNueCC_muonVec[b_line].begin(), NueFromNueCC_muonVec[b_line].end(),
            NueFromNueCC_muonVec[b_line].begin(), 
            std::bind2nd(std::multiplies<float>(), scales[b_line]));
    std::transform(  NueFromNueCC_chargeKaonVec[b_line].begin(), NueFromNueCC_chargeKaonVec[b_line].end(),
            NueFromNueCC_chargeKaonVec[b_line].begin(), 
            std::bind2nd(std::multiplies<float>(), scales[b_line]));
    std::transform(  NueFromNueCC_neutKaonVec[b_line].begin(), NueFromNueCC_neutKaonVec[b_line].end(),
            NueFromNueCC_neutKaonVec[b_line].begin(), 
            std::bind2nd(std::multiplies<float>(), scales[b_line]));
    std::transform(  NueFromEScatterVec[b_line].begin(), NueFromEScatterVec[b_line].end(),
            NueFromEScatterVec[b_line].begin(), 
            std::bind2nd(std::multiplies<float>(), scales[b_line]));
    std::transform(  NueFromNC_pi0Vec[b_line].begin(), NueFromNC_pi0Vec[b_line].end(),
            NueFromNC_pi0Vec[b_line].begin(), 
            std::bind2nd(std::multiplies<float>(), scales[b_line]));
    std::transform(  NueFromNC_delta0Vec[b_line].begin(), NueFromNC_delta0Vec[b_line].end(),
            NueFromNC_delta0Vec[b_line].begin(), 
            std::bind2nd(std::multiplies<float>(), scales[b_line]));
    std::transform(  NueFromNumuCCVec[b_line].begin(), NueFromNumuCCVec[b_line].end(),
            NueFromNumuCCVec[b_line].begin(), 
            std::bind2nd(std::multiplies<float>(), scales[b_line]));
    std::transform(  DirtVec[b_line].begin(), DirtVec[b_line].end(),
            DirtVec[b_line].begin(), 
            std::bind2nd(std::multiplies<float>(), scales[b_line]));
    std::transform(  OtherVec[b_line].begin(), OtherVec[b_line].end(),
            OtherVec[b_line].begin(), 
            std::bind2nd(std::multiplies<float>(), scales[b_line]));

    for (int point = 0; point <= npoints; point ++){
      std::transform(  eventsnueoscVec[b_line][point].begin(),
              eventsnueoscVec[b_line][point].end(),
              eventsnueoscVec[b_line][point].begin(), 
              std::bind2nd(std::multiplies<float>(), scales[b_line]));    
    } //loop over points
	} //loop over baselines, ends scaling	

	//Alright, we've got the histos for each baseline as well as the oscillated histos!

	TH1F * MBPhotExcess = new TH1F("MBPhotExcess","LowEExcess scaled to uboone", nbinsE, emin, emax);
	MBPhotExcess -> SetBinContent(1, 47);
	for (int i = 1; i < nbinsE; i++) MBPhotExcess -> SetBinContent(i+1, 1.0);
	// MBPhotExcess -> Draw();


	TLegend *leg;
	
	std::vector< TCanvas * > stackedCanvas;
	stackedCanvas.resize(nL);
	for (int j = 0; j <nL; j++){
		// root makes me want to be eaten by a grue,  sometimes.
		// why do i have to do this stupid naming shit to keep track of my histograms?
		if (j == 0)			stackedCanvas[j] = new TCanvas("sc0", "Stacked event rates", 700, 500);
		else if (j == 1)	stackedCanvas[j] = new TCanvas("sc1", "Stacked event rates", 700, 500);
		else if (j == 2)	stackedCanvas[j] = new TCanvas("sc2", "Stacked event rates", 700, 500);
		else return -1;
		
		TString nueTitle = "Nue Events at L = ";
		nueTitle += baselines[j];
		THStack * stack = new THStack("nue",nueTitle);
		//add in all the plots		
		
		
		TH1F * NueFromNueCC_muon = makeHistogram(NueFromNueCC_muonVec[j], emin, emax);
		TH1F * NueFromNueCC_chargeKaon = makeHistogram(NueFromNueCC_chargeKaonVec[j], emin, emax);
		TH1F * NueFromNueCC_neutKaon = makeHistogram(NueFromNueCC_neutKaonVec[j], emin, emax);
		TH1F * NueFromEScatter = makeHistogram(NueFromEScatterVec[j], emin, emax);
		TH1F * NueFromNC_pi0 = makeHistogram(NueFromNC_pi0Vec[j], emin, emax);
		TH1F * NueFromNC_delta0 = makeHistogram(NueFromNC_delta0Vec[j], emin, emax);
		TH1F * NueFromNumuCC = makeHistogram(NueFromNumuCCVec[j], emin, emax);
		TH1F * Dirt = makeHistogram(DirtVec[j], emin, emax);
		TH1F * Other = makeHistogram(OtherVec[j], emin, emax);


		TH1F * SignalNu = makeHistogram(eventsSignalBestFitNuVec[j], emin, emax);
		TH1F * SignalNubar = makeHistogram(eventsSignalBestFitNubarVec[j], emin, emax);

		SignalNu -> Add(SignalNubar);


		NueFromNueCC_muon		-> SetFillColor(30);
		NueFromNueCC_chargeKaon	-> SetFillColor(31);
		NueFromNueCC_neutKaon	-> SetFillColor(32);
		NueFromEScatter			-> SetFillColor(33);
		NueFromNC_pi0			-> SetFillColor(41);
		NueFromNC_delta0		-> SetFillStyle(41);
		NueFromNumuCC			-> SetFillColor(40);
		Dirt					-> SetFillColor(15);
		Other					-> SetFillColor(8);

		std::vector< float > totalEvents, signalEvents;
		totalEvents.resize(nbins);
		signalEvents.resize(nbins);


		// Set the bin errors to zero except for the very last bin:
		for (int i = 0; i < nbinsE; ++i)
		{
			totalEvents[i] = 0;
			NueFromNueCC_muon		-> SetBinError(i+1, 0.0);
			totalEvents[i]			+= NueFromNueCC_muon -> GetBinContent(i+1);
			NueFromNueCC_chargeKaon	-> SetBinError(i+1, 0.0);
			totalEvents[i]			+= NueFromNueCC_chargeKaon -> GetBinContent(i+1);
			NueFromNueCC_neutKaon	-> SetBinError(i+1, 0.0);
			totalEvents[i]			+= NueFromNueCC_neutKaon -> GetBinContent(i+1);
			NueFromEScatter			-> SetBinError(i+1, 0.0);
			totalEvents[i]			+= NueFromEScatter -> GetBinContent(i+1);
			NueFromNC_pi0			  -> SetBinError(i+1, 0.0);
			totalEvents[i]			+= NueFromNC_pi0 -> GetBinContent(i+1);
			NueFromNC_delta0		-> SetBinError(i+1, 0.0);
			totalEvents[i]			+= NueFromNC_delta0 -> GetBinContent(i+1);
			NueFromNumuCC			  -> SetBinError(i+1, 0.0);
			totalEvents[i]			+= NueFromNumuCC -> GetBinContent(i+1);
			Dirt					      -> SetBinError(i+1, 0.0);
			totalEvents[i]			+= Dirt -> GetBinContent(i+1);
			Other					      -> SetBinError(i+1, 0.0);
			totalEvents[i]			+= Other -> GetBinContent(i+1);
			signalEvents[i]			= SignalNu -> GetBinContent(i+1);
		}
 		
 		
		//Now set the errors on the last hist going in:
		for (int i = 0; i < nbinsE; ++i)
		{
			NueFromNumuCC -> SetBinError(i+1, sqrt(totalEvents[i]));
			SignalNu->SetBinContent(i+1, totalEvents[i]+SignalNu->GetBinContent(i+1));
			SignalNu->SetBinError(i+1, sqrt(signalEvents[i]));
		}
		
		
		stack -> Add(NueFromNueCC_muon);
		stack -> Add(NueFromNueCC_chargeKaon);
		stack -> Add(NueFromNueCC_neutKaon);
		//stack -> Add(NueFromEScatter);
		stack -> Add(NueFromNC_pi0);
		// stack -> Add(NueFromNC_delta0);
		stack -> Add(NueFromNumuCC);
		// stack -> Add(Dirt);
		// stack -> Add(Other);
		// stack ->Add(MBPhotExcess);


		// stack -> Add(SignalNu);

		//set up the legend
		leg = new TLegend(0.75,0.5,.95,0.9);
		leg->AddEntry(NueFromNueCC_muon, "#mu #rightarrow #nu_{e}");
		leg->AddEntry(NueFromNueCC_chargeKaon, "k^{+} #rightarrow #nu_{e}");
		leg->AddEntry(NueFromNueCC_neutKaon, "k^{0} #rightarrow #nu_{e}");
		//leg->AddEntry(NueFromEScatter, "#nu - e^{-}");
		leg->AddEntry(NueFromNC_pi0, "NC #pi^{0}");
		// leg->AddEntry(NueFromNC_delta0, "#Delta #rightarrow N#gamma");
		leg->AddEntry(NueFromNumuCC, "#nu_{#mu} CC");
		// leg->AddEntry(Dirt, "Dirt");
		// leg->AddEntry(Other, "Other");
		leg->AddEntry(SignalNu,"Signal");

		leg -> SetTextSize(0.04);



    // Get the value to be the maximum of the plot:
    double max = (stack -> GetMaximum());

    std::cout << "Max val: " << max << "\n";

    if (mode == "nubar"){
      if (j == 0) max *= 1.5;
      if (j == 1) max *= 2.5;
      if (j == 2) max *= 2.0;
    }
    if (mode == "nu"){
      if (j == 0) max *= 1.75;
      if (j == 1) max *= 2.0;
      if (j == 2) max *= 2.1;
    }
    std::cout << "Max val (scaled): " << max << "\n";



		TH1F *chr = stackedCanvas[j]->DrawFrame(emin-0.01,-0.01*(SignalNu->GetMaximum()),emax,1.0*max);
		
    //chr->GetYaxis()->SetLabelOffset(999);
		chr->GetYaxis()->SetTitle("Events");
		chr->GetYaxis()->SetTitleSize(0.06);
		chr->GetYaxis()->SetTitleOffset(.7);
		char name[200];
		if (baselines[j] == 100){
			sprintf(name, "LAr1-ND (%im)", baselines[j]);
		}
		else if (baselines[j] == 470){
			sprintf(name, "MicroBooNE (%im)", baselines[j]);
		}
		else if (baselines[j] == 700){
      if (use700m) sprintf(name, "LAr1-FD (%im)", baselines[j]);
      if (useT600) sprintf(name, "ICARUS (%im)", baselines[j]);
		}

		
		chr->GetXaxis()->CenterTitle();
		chr->GetXaxis()->SetLabelSize(0.05);
		TString forComparison = fileSource;
		if (forComparison == "/Users/cja33/Desktop/lar1/CCQE/"){
			chr->GetXaxis()->SetTitle("E_{#nu}^{QE} (GeV)");
		}
		else chr->GetXaxis()->SetTitle("Energy (GeV)");


		chr->GetXaxis()->SetTitle("Reconstructed Neutrino Energy (GeV)");
		chr->GetXaxis()->SetTitleOffset(1);
		chr->GetXaxis()->SetTitleSize(0.07);
		chr->GetXaxis()->SetLimits(emin-0.01,emax);
		chr->Draw();
		SignalNu -> SetLineStyle(2);

		add_plot_label( name , 0.5, 0.85, 0.05, 1 );
    if (useHighDm) 
      add_plot_label( (char*)"signal: (#Deltam^{2} = 50 eV^{2}, sin^{2}2#theta_{#mue} = 0.003)", 0.5, 0.775, 0.04, 1);
    else if (useGlobBF)
      add_plot_label( (char*)"signal: (#Deltam^{2} = 0.43 eV^{2}, sin^{2}2#theta_{#mue} = 0.013)", 0.5, 0.775, 0.04, 1);
    else  add_plot_label( (char*)"signal: (#Deltam^{2} = 1.2 eV^{2}, sin^{2}2#theta_{#mue} = 0.003)", 0.5, 0.775, 0.04, 1);

		stack -> Draw("E0 hist same");
		SignalNu -> Draw("E0 hist same");
		SignalNu -> Draw("E0 same");
		stack -> Draw("E0 hist  same");
		stack -> Draw("E0 same ");
		chr   -> Draw("same");

		// stack -> Draw(" same ");
		leg->Draw();

	}
    

    

    if (specialNameText != "") fileNameRoot = fileNameRoot + specialNameText + "_";
    if (specialNameTextOsc != "") fileNameRoot = fileNameRoot + specialNameTextOsc + "_";

    for (int i = 0; i < nL; i++)
    {
    	char fileName[200];
      if (useHighDm) 
        sprintf(fileName, "%s%i_%s_%s_highdm2.pdf", fileNameRoot.Data(), baselines[i], mode.c_str(),energyType.c_str());
      else if (useGlobBF) 
        sprintf(fileName, "%s%i_%s_%s_globBF.pdf", fileNameRoot.Data(), baselines[i], mode.c_str(),energyType.c_str());
      else sprintf(fileName, "%s%i_%s_%s.pdf", fileNameRoot.Data(), baselines[i], mode.c_str(),energyType.c_str());
    	stackedCanvas[i] -> Print(fileName, "pdf");
    	stackedCanvas[i] -> Print(fileName, "png");
    }
    


    std::cout << "All done!" << std::endl;
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

# ifndef __CINT__
int main()
{
  event_rate_calc();
  return 0;
}
# endif


