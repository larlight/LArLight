#ifndef CFALGOTIMEPROF_CXX
#define CFALGOTIMEPROF_CXX

#include "CFAlgoTimeProf.hh"
#include "LArUtilBase.hh"
// ROOT includes
#include "TF1.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TPrincipal.h"
#include "TVectorD.h"
#include "TGraph.h"
#include "TMath.h"
#include "TH1D.h"
#include "TVirtualFitter.h"


namespace cmtool {

  //-------------------------------------------------------
  CFAlgoTimeProf::CFAlgoTimeProf() : CFloatAlgoBase(),
				     siga(nullptr),
				     siginta(nullptr),
				     sigb(nullptr),
				     sigintb(nullptr)
  //-------------------------------------------------------
  {

  }

  //-------------------------------
  CFAlgoTimeProf::~CFAlgoTimeProf()
  //-------------------------------
  {
    if(siga) delete siga;
    if(sigb) delete sigb;
    if(siginta) delete siginta;
    if(sigintb) delete sigintb;

    siga = sigb = siginta = sigintb = nullptr;
  }

  //-----------------------------
  void CFAlgoTimeProf::Reset()
  //-----------------------------
  {

  }

  //----------------------------------------------------------------------------------------------
  float CFAlgoTimeProf::Float(const std::vector<const cluster::ClusterParamsAlgNew*> &clusters)
  //----------------------------------------------------------------------------------------------
  {
  // We now have a vector a clusters.

  //### need a pointer to the cluster just return -1  
    for(auto const& ptr : clusters) if(!ptr) return -1;

        std::vector<larutil::PxHit> hits0;
        std::vector<larutil::PxHit> hits1;
        std::vector<larutil::PxHit> hits2;

	// loop over the clusters
        for(auto const& c : clusters)
	{
	std::cout<<"Size of the xluster vector"<<clusters.size()<<std::endl;
	//first lets print out some information about the clusters
	auto Plane = c->Plane();
	auto StartPoint = c->GetParams().start_point.t;
	auto EndPoint = c->GetParams().end_point.t;
	std::cout<<"\t RG Cluster info:\n \t plane: "<<Plane<<std::endl;
        std::cout<<"\tStart point: "<<StartPoint<<std::endl;
        std::cout<<"\tEnd Point: "<<EndPoint <<std::endl;

	// Get assosiations for this cluster
        if(c->Plane() ==0) hits0 = c->GetHitVector();
        if(c->Plane() ==1) hits1 = c->GetHitVector();
        if(c->Plane() ==2) hits2 = c->GetHitVector();

	}// for over the clusters

	std::cout<<"Looking for the hits vector size"<<hits0.size()<<","<<hits1.size()<<","<<hits2.size()<<std::endl;
//        std::cout<<"############# End of loop############# "<<std::endl;
	// make an integrale over the cluster
	bool pl0 = false;
	bool pl1 = false;
	bool pl2 = false;
	float tprof01 = -1;
	float tprof02 = -1;
	float tprof12 = -1;
	if(hits0.size()>0)
	{
	pl0 = true;
		if(hits1.size()>0)
		{
		pl1 = true;
		//we need to do a profile
		tprof01 =TProfCompare(hits0,hits1);
		}// hits1size>0
		if(hits2.size()>0)
		{
		pl2 = true;
		//we need to do a profile
		tprof02 =TProfCompare(hits0,hits2);
		}// hits2size>0
	}// hits0size >0

	if(hits1.size()>0)
	{
	pl1 = true;
		if(hits2.size()>0)
		{
		pl2 = true;
		//we need to do a profile
		tprof12 =TProfCompare(hits1,hits2);
		}//hits2.size>0
	}// hits1size>0

	// This is going to be slow is we are having to re-calutlate this ever time. For now it will have to do

//	std::cout<< " \t summary of timeprof Planes that are accepted :" <<pl0<<" | " <<pl1<<" | " <<pl2<<" |"<<std::endl;
	std::vector<float> tprofmatches;	
	std::cout<< " \t                  Value of timeprof(01,02,12) :" <<tprof01<<" | " <<tprof02<<" | " <<tprof12<<" |"<<std::endl;
	tprofmatches.push_back(tprof01);
	tprofmatches.push_back(tprof02);
	tprofmatches.push_back(tprof12);
	
	float matchscore=0;
	float avgcounter=0;
	std::cout<<"SIZE of trpfmoatch"<< tprofmatches.size()<<std::endl;
	for( unsigned int a=0;a<tprofmatches.size();a++)
	{
	if(tprofmatches[a]==-1) continue;	
	else {
		matchscore +=tprofmatches[a];
		avgcounter +=1;
		}// end of else
	}//for over the tprofmatchs
		if(avgcounter!=0){
		std::cout << " Match Score pree "<< matchscore<<std::endl;		
		std::cout<< " Counter "<< avgcounter;
		matchscore /= avgcounter;
		std::cout << " Match Score "<< matchscore<<std::endl;		
		}
		else{
		std::cout << " Match Score "<< -1<<std::endl;		
		 return -1;
		}
		return matchscore;
		
	
		
  
    //if(clusters.size()) return 1.;
    //else return -1.;
  }

// Making a function to do the profile test
 float CFAlgoTimeProf::TProfCompare(std::vector<larutil::PxHit> hita ,std::vector<larutil::PxHit> hitb)
 {
	    int nts = larutil::DetectorProperties::GetME()->NumberTimeSamples()*larutil::GeometryUtilities::GetME()->TimeToCm();
	// Where is this?
    //int nplanes = geom->Nplanes();
    int nplanes = 3;
    double ks = 0.0;
    std::vector< std::vector<TH1D*> > signals(nplanes);
    std::vector< std::vector<TH1D*> > pulses(nplanes);

    // One-time only initialization of class-member histograms
    if(!siga) siga = new TH1D("sig_a","sig_a",nts,0,nts);
    siga->Reset();
      
    if(!siginta) siginta = new TH1D("sigint_a","sigint_a",nts,0,nts);
    siginta->Reset();
      
    if(!sigb) sigb = new TH1D("sig_b","sig_b",nts,0,nts);
    sigb->Reset();
      
    if(!sigintb) sigintb = new TH1D("sigint_b","sigint_b",nts,0,nts);
    sigintb = new TH1D("sigint_b","sigint_b",nts,0,nts);
	 
// First loop over hits in A and make the hist
// in this case let's just use plane 0,1
        for( auto const& ha : hita){
          double time = ha.t;
          time -= larutil::DetectorProperties::GetME()->GetXTicksOffset(ha.plane)*larutil::GeometryUtilities::GetME()->TimeToCm();
          double charge = ha.charge;
          int bin = siga->FindBin(time);
          siga->SetBinContent(bin,siga->GetBinContent(bin)+charge);
          for (int j = bin; j<=siga->GetNbinsX(); ++j){
            siginta->SetBinContent(j,siginta->GetBinContent(j)+charge);
          }
        }
        if (siginta->Integral()) siginta->Scale(1./siginta->GetBinContent(siginta->GetNbinsX()));
	for( auto const& hb : hitb){
          double time = hb.t;
          time -= larutil::DetectorProperties::GetME()->GetXTicksOffset(hb.plane)*larutil::GeometryUtilities::GetME()->TimeToCm();
          double charge = hb.charge;
          int bin = sigb->FindBin(time);
          sigb->SetBinContent(bin,sigb->GetBinContent(bin)+charge);
          for (int j = bin; j<=sigb->GetNbinsX(); ++j){
            sigintb->SetBinContent(j,sigintb->GetBinContent(j)+charge);
          }
        }
        if (sigintb->Integral()) sigintb->Scale(1./sigintb->GetBinContent(sigintb->GetNbinsX()));
	ks = siginta->KolmogorovTest(sigintb);

	return ks;
 }

  //------------------------------
  void CFAlgoTimeProf::Report()
  //------------------------------
  {

  }
    
}
#endif
