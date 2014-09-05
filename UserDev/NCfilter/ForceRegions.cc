#ifndef FORCEREGIONS_CC
#define FORCEREGIONS_CC


#include "ForceRegions.hh"
//#include "ClusterParamsAlg.hh"
//#include "LArUtilBase.hh"


namespace cluster {


  std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>>  ForceTwoRegions(const std::vector<larlight::hit>& hits, std::vector<std::pair<double,double>> APP) {

    double T2CM = larutil::GeometryUtilities::GetME()->TimeToCm();
    double W2CM = larutil::GeometryUtilities::GetME()->WireToCm();
    std::vector<unsigned int> startervector;
    std::pair<std::vector<unsigned int>,std::vector<unsigned int>> planepair(startervector,startervector);
    std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>> vectplanepair(3,planepair);
    // loop over all the hits.  
    //for(auto const& hit : *hits) {
    for(unsigned int a=0;  a < hits.size();a++) {
      //which plane?
      int cplane= hits[a].View();
      if(APP[cplane].first==-999||APP[cplane].second==-999){ std::cout<<" \033[98m GOT A -999"; continue;}
      double regionval= (-APP[cplane].first * hits[a].Wire()*W2CM+ hits[a].PeakTime()*T2CM - APP[cplane].second) / sqrt(APP[cplane].first*APP[cplane].first +1);
      // sort into proper places using viewPP
	      if(regionval>0.0000) vectplanepair[cplane].first.push_back(a);
	      if(regionval<0.0000) vectplanepair[cplane].second.push_back(a);
    }// for loop over hit index

    return vectplanepair;



	}




}

#endif


