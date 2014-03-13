/////////////////////////////////////////////////////////////////////
///
/// HoughBaseAlg class
///
/// Ben Carls, bcarls@fnal.gov
///
/// The Hough transform employed by fuzzy clustering is a heavily modified variant of the original 
/// Hough line code. It identifies lines in fuzzy clusters (e.g. muon tracks) and splits them off
/// into new clusters. 
///
/// The algorithm is based on the Progressive Probabilistic Hough Transform (PPHT).
/// See the following paper for details:
///
/// J. Matas et al., Robust Detection of Lines Using the Progressive Probabilistic Hough Transform,
/// Computer Vision and Image Understanding, Volume 78, Issue 1, April 2000, Pages 119–137
///
////////////////////////////////////////////////////////////////////////

/*
extern "C" {
#include <sys/types.h>
#include <sys/stat.h>
}
*/

#include <sstream>
#include <fstream>
#include <math.h>
#include <algorithm>
#include <vector>
#include <stdint.h>
//#include <boost/bind.hpp>

#include <TF1.h>
#include <TH1D.h>
#include <TStopwatch.h>
#include <TRandom.h>
//#include "CLHEP/Random/RandFlat.h"

/*
#include "art/Framework/Principal/Event.h" 
#include "fhiclcpp/ParameterSet.h" 
#include "art/Framework/Principal/Handle.h" 
#include "art/Persistency/Common/Ptr.h" 
#include "art/Persistency/Common/PtrVector.h" 
#include "art/Framework/Services/Registry/ServiceHandle.h" 
#include "art/Framework/Services/Optional/TFileService.h" 
#include "art/Framework/Services/Optional/TFileDirectory.h" 
#include "messagefacility/MessageLogger/MessageLogger.h" 
*/
 
//#include "Filters/ChannelFilter.h"
//#include "RecoAlg/HoughBaseAlg.hh"
#include "HoughBaseAlg.hh"
//#include "RecoBase/Hit.include"
//#include "RecoBase/Cluster.h"
//#include "Geometry/Geometry.h"
//#include "Geometry/CryostatGeo.h"
//#include "Geometry/TPCGeo.h"
//#include "Geometry/PlaneGeo.h"
//#include "Geometry/WireGeo.h"
//#include "Utilities/LArProperties.h"
//#include "Utilities/DetectorProperties.h"
//#include "Utilities/AssociationUtil.h"


#define PI TMath::Pi()
#define a0  0 /*-4.172325e-7f*/   /*(-(float)0x7)/((float)0x1000000); */
#define a1 1.000025f        /*((float)0x1922253)/((float)0x1000000)*2/Pi; */
#define a2 -2.652905e-4f    /*(-(float)0x2ae6)/((float)0x1000000)*4/(Pi*Pi); */
#define a3 -0.165624f       /*(-(float)0xa45511)/((float)0x1000000)*8/(Pi*Pi*Pi); */
#define a4 -1.964532e-3f    /*(-(float)0x30fd3)/((float)0x1000000)*16/(Pi*Pi*Pi*Pi); */
#define a5 1.02575e-2f      /*((float)0x191cac)/((float)0x1000000)*32/(Pi*Pi*Pi*Pi*Pi); */
#define a6 -9.580378e-4f    /*(-(float)0x3af27)/((float)0x1000000)*64/(Pi*Pi*Pi*Pi*Pi*Pi); */

#define _sin(x) ((((((a6*(x) + a5)*(x) + a4)*(x) + a3)*(x) + a2)*(x) + a1)*(x) + a0)
#define _cos(x) _sin(TMath::Pi()*0.5 - (x))

//------------------------------------------------------------------------------
larreco::HoughBaseAlg::HoughBaseAlg()
{
  this->reconfigure();
}

//------------------------------------------------------------------------------
larreco::HoughBaseAlg::~HoughBaseAlg()
{
}

//------------------------------------------------------------------------------
//void larreco::HoughBaseAlg::reconfigure(fhicl::ParameterSet const& pset)
void larreco::HoughBaseAlg::reconfigure()
{ 

  fMaxLines = 20;
  fMinHits  = 5;
  fSaveAccumulator = 0;
  fNumAngleCells = 2e4;
  fRhoResolutionFactor = 5;
  fMaxDistance = 1.0;
  fMaxSlope = 80.;
  fRhoZeroOutRange = 0.;
  fThetaZeroOutRange = 0;
  fPerCluster = 1;
  fMissedHits = 1;
  fMissedHitsDistance = 2.;
  fMissedHitsToLineSize = 0.25;
  return;
}

//------------------------------------------------------------------------------
larreco::HoughTransform::HoughTransform()
{  
  //m_accum=NULL;
}


//------------------------------------------------------------------------------
size_t larreco::HoughBaseAlg::Transform(std::vector<larlight::hit> const& hits,
					std::vector<UInt_t>  *fpointId_to_clusterId,
					UInt_t                clusterId, // The id of the cluster we are examining
					Int_t                        *nClusters,
                                        std::vector<larreco::protoTrack>    *linesFound
					)
{

  Int_t nClustersTemp = *nClusters;
  
  HoughTransform c;
  
  const larutil::Geometry* geom = larutil::Geometry::GetME();
  const larutil::LArProperties* larprop = larutil::LArProperties::GetME();
  const larutil::DetectorProperties* detprop = larutil::DetectorProperties::GetME();

  //filter::ChannelFilter chanFilt;

  UInt_t     channel = hits.at(0).Channel();
  UInt_t wire    = 0;
  UInt_t wireMax = 0;


  //mf::LogInfo("HoughBaseAlg") << "nClusters is: " << *nClusters;


  //size_t cinctr = 0;
  //geo::View_t    view = geom->Cryostat(cs).TPC(t).Plane(p).View();
  larlight::GEO::SigType_t sigt = geom->SignalType(channel);
  std::vector<Int_t> skip;  
  
  //factor to make x and y scale the same units
  Float_t wirePitch = geom->WirePitch(geom->View(channel));
  Float_t xyScale  = 0.001*larprop->DriftVelocity(larprop->Efield(),larprop->Temperature())*detprop->SamplingRate()/wirePitch;
  //xyScale        *= detprop->SamplingRate()/wirePitch;

  Float_t wire_dist = wirePitch;
  Float_t tickToDist = larprop->DriftVelocity(larprop->Efield(),larprop->Temperature())*1.e-3 * detprop->SamplingRate();
  //tickToDist *= 1.e-3 * detprop->SamplingRate(); // 1e-3 is conversion of 1/us to 1/ns


  //mf::LogInfo("HoughBaseAlg") << "xyScale: " << xyScale;
  //mf::LogInfo("HoughBaseAlg") << "tickToDist: " << tickToDist;
  
  Int_t x, y;
  //UInt_t channel, plane, wire, tpc, cstat;
  //there must be a better way to find which plane a cluster comes from
  Int_t dx = geom->Nwires(geom->ChannelToPlane(hits.at(0).Channel())); // number of wires
  Int_t dy = geom->SignalType(hits.at(0).SignalType()); //number of time samples. 

  skip.clear();
  skip.resize(hits.size());
  std::vector<Int_t> listofxmax;
  std::vector<Int_t> listofymax;  
  std::vector<Int_t> hitsTemp;        //indecies ofcandidate hits
  std::vector<Int_t> sequenceHolder; //channels of hits in list
  std::vector<Int_t> currentHits;    //working vector of hits 
  std::vector<Int_t> lastHits;       //best list of hits
  std::vector<UShort_t> clusterHits;
  Float_t indcolscaling = 0.;       //a parameter to account for the different 
        			   ////characteristic hit width of induction and collection plane
  /// \todo: the collection plane's characteristic hit width's are, 
  /// \todo: on average, about 5 time samples wider than the induction plane's. 
  /// \todo: this is hard-coded for now.
  if(sigt == larlight::GEO::kInduction)
    indcolscaling = 0.;
  else
    indcolscaling = 1.;
  //indcolscaling = 0;
  Float_t centerofmassx = 0;
  Float_t centerofmassy = 0;
  Float_t denom = 0; 
  Float_t intercept=0.;
  Float_t slope = 0.;
  //this array keeps track of the hits that have already been associated with a line. 
  Int_t xMax = 0;
  Int_t yMax = 0;
  Int_t yClearStart;
  Int_t yClearEnd;
  Int_t xClearStart;
  Int_t xClearEnd;
  Int_t maxCell = 0;
  Float_t rho;
  Float_t theta; 
  Int_t accDx(0), accDy(0);
  Float_t pCornerMin[2];
  Float_t pCornerMax[2];
  //Float_t pCorner0[2];
  //Float_t pCorner1[2];
  //bool newChannel = false;
  //UInt_t lastChannel;

  UInt_t fMaxWire = 0;
  Int_t iMaxWire = 0;
  UInt_t fMinWire = 99999999;
  Int_t iMinWire = -1;




  /// Outline of PPHT, J. Matas et. al. 
  /// ---------------------------------------
  /// 
  ///LOOP over hits, picking a random one
  ///  Enter the point into the accumulator
  ///  IF it is already in the accumulator or part of a line, skip it
  ///  Store it in a vector of points that have been chosen
  ///
  ///  Find max value in accumulator; IF above threshold, create a line
  ///    Subtract points in line from accumulator
  ///    
  ///
  ///END LOOP over hits, picking a random one
  ///

  ///Init specifies the size of the two-dimensional accumulator 
  ///(based on the arguments, number of wires and number of time samples). 
  c.Init(dx,dy,fRhoResolutionFactor,fNumAngleCells);
  /// Adds all of the hits to the accumulator
  //mf::LogInfo("HoughBaseAlg") << "Beginning PPHT";

  c.GetAccumSize(accDy, accDx);

  // Define the prototrack object
  protoTrack protoTrackToLoad;


  // The number of lines we've found
  UInt_t nLinesFound = 0;
  std::vector<UInt_t> accumPoints(hits.size(),0);
  Int_t nAccum = 0;

  /// count is how many points are left to randomly insert
  Int_t count = 0;
  for(auto fpointId_to_clusterIdItr = fpointId_to_clusterId->begin(); fpointId_to_clusterIdItr != fpointId_to_clusterId->end();fpointId_to_clusterIdItr++)
    if(*fpointId_to_clusterIdItr == clusterId)
      count++;

  UInt_t randInd;

  /// Get the random number generator
  //art::ServiceHandle<art::RandomNumberGenerator> rng;
  //CLHEP::HepRandomEngine & engine = rng -> getEngine();
  //CLHEP::RandFlat flat(engine);
  TStopwatch w;
  //Float_t timeTotal = 0;

  for( ; count > 0; ){

    /// The random hit we are unsigned
    ///examining int randInd = rand() % hits.size();
    //randInd = (UInt_t)(flat.fire()*hits.size());
    randInd = (UInt_t)(gRandom->Uniform()*hits.size());

    //std::cout << count << " " << randInd << std::endl;
    
      /// If the point isn't in the current fuzzy cluster, skip it
    if(fpointId_to_clusterId->at(randInd) != clusterId)
      continue;

    --count;

    /// Skip if it's already in a line
    if(skip[randInd]==1){
      continue;
    }

    /// If we have already accumulated the point, skip it
    if(accumPoints[randInd])
      continue;
    accumPoints[randInd]=1;
   
    /// zeroes out the neighborhood of all previous lines  
    for(auto listofxmaxItr = listofxmax.begin(); listofxmaxItr != listofxmax.end(); ++listofxmaxItr) {
      yClearStart = listofymax[listofxmaxItr-listofxmax.begin()] - fRhoZeroOutRange;
      if (yClearStart < 0) yClearStart = 0;
      
      yClearEnd = listofymax[listofxmaxItr-listofxmax.begin()] + fRhoZeroOutRange;
      if (yClearEnd >= accDy) yClearEnd = accDy - 1;
      
      xClearStart = *listofxmaxItr - fThetaZeroOutRange;
      if (xClearStart < 0) xClearStart = 0;
      
      xClearEnd = *listofxmaxItr + fThetaZeroOutRange;
      if (xClearEnd >= accDx) xClearEnd = accDx - 1;
      
      for (y = yClearStart; y <= yClearEnd; ++y){
        for (x = xClearStart; x <= xClearEnd; ++x){
          c.SetCell(y,x,0);
        }
      }
    }/// end loop over size of listxmax
  
    /// Find the weightiest cell in the accumulator.
    UInt_t channel = hits.at(randInd).Channel();
    wireMax = hits.at(randInd).Wire();

    /// Add the randomly selected point to the accumulator
    //w.Start();
    std::vector<Int_t> max = c.AddPointReturnMax(wireMax, (Int_t)(hits.at(randInd).PeakTime()));
    maxCell = max[0];
    xMax    = max[1];
    yMax    = max[2];
    //w.Stop();
    //std::cout << "Real Time: " << w.RealTime() << std::endl;
    //timeTotal += w.CpuTime();
    ++nAccum; 

    //mf::LogVerbatim("HoughBaseAlg") << "cout: " << count << " maxCell: " << maxCell << std::endl;
    //mf::LogVerbatim("HoughBaseAlg") << "xMax: " << xMax << " yMax: " << yMax << std::endl;

    /// The threshold calculation, see http://www.via.cornell.edu/ece547/projects/Hough/webpage/statistics.html
    /// accDx is the number of rho bins,m_rowLength
    //TF1 *threshGaus = new TF1("threshGaus","(1/([0]*std::sqrt(2*TMath::Pi())))*exp(-0.5*pow(((x-[1])/[0]),2))");
    //Double_t sigma = std::sqrt(((Double_t)nAccum/(Double_t)accDx)*(1-1/(Double_t)accDx));
    //Double_t mean = (Double_t)nAccum/(Double_t)accDx;
    //threshGaus->SetParameter(0,sigma);
    //threshGaus->SetParameter(1,mean);
    //mf::LogVerbatim("HoughBaseAlg") << "threshGaus mean: " << mean << " sigma: " << sigma << " accDx: " << accDx << std::endl;
    //mf::LogVerbatim("HoughBaseAlg") << "nAccum: " << nAccum << std::endl;
    //mf::LogVerbatim("HoughBaseAlg") << "threshGaus integral range: " << mean-2*sigma << " to " << maxCell << std::endl;
    //mf::LogVerbatim("HoughBaseAlg") << "threshGaus integral: " << threshGaus->Integral(mean-2*sigma,maxCell) << std::endl;
    //mf::LogVerbatim("HoughBaseAlg") << "threshGaus integral: " << threshGaus->Integral(0,maxCell) << std::endl;


    /// The threshold calculation using a Poisson distribution instead
    //Double_t poisProbSum = 0;
    //for(int j = 0; j <= maxCell; j++){
    //Double_t poisProb = TMath::Poisson(j,mean);
    //poisProbSum+=poisProb;
    //mf::LogVerbatim("HoughBaseAlg") << "Poisson: " << poisProb << std::endl;
    //}
    //mf::LogVerbatim("HoughBaseAlg") << "Poisson prob sum: " << poisProbSum << std::endl;
    //mf::LogVerbatim("HoughBaseAlg") << "Probability it is higher: " << 1-poisProbSum << std::endl;

    // Continue if the probability of finding a point, (1-poisProbSum) is the probability of finding a 
    // value of maxCell higher than what it currently is
    //if( (1-poisProbSum) > 1e-13)
    //continue;


    // The threshold calculation using a Binomial distribution instead
    Double_t binomProbSum = TMath::BinomialI(1/(Double_t)accDx,nAccum,maxCell);
    //std::cout << "nAccum: " << nAccum << std::endl;
    //std::cout << "maxCell: " << maxCell << std::endl;
    //std::cout << "BinomialI: " << binomProbSum << std::endl;
    //std::cout << std::endl;
    //mf::LogVerbatim("HoughBaseAlg") << "Probability it is higher: " << 1-binomProbSum << std::endl;
    //Continue if the probability of finding a point, (1-poisProbSum) is the probability of finding a
    //value of maxCell higher than what it currently is
    if( binomProbSum > 1e-21)
      continue;



    /// Continue if the biggest maximum for the randomly selected point is smaller than fMinHits
    //if (maxCell < fMinHits) 
      //continue;


    /// Find the center of mass of the 3x3 cell system (with maxCell at the center). 
    denom = centerofmassx = centerofmassy = 0;
  
    if(xMax > 0 && yMax > 0 && xMax + 1 < accDx && yMax + 1 < accDy){  
      for(Int_t i = -1; i < 2; ++i){
        for(Int_t j = -1; j < 2; ++j){
          denom += c.GetCell(yMax+i,xMax+j);
          centerofmassx += j*c.GetCell(yMax+i,xMax+j);
          centerofmassy += i*c.GetCell(yMax+i,xMax+j);
        }
      }
      centerofmassx /= denom;
      centerofmassy /= denom;      
    }
    else  centerofmassx = centerofmassy = 0;
  
    ///fill the list of cells that have already been found
    listofxmax.push_back(xMax);
    listofymax.push_back(yMax);

    // Find the lines equation
    c.GetEquation(yMax+centerofmassy, xMax+centerofmassx, rho, theta);
    slope = -1./tan(theta);    
    intercept = (rho/sin(theta));
    Float_t distance;

    if(!std::isinf(slope) && !std::isnan(slope)){
      sequenceHolder.clear();
      fMaxWire = 0;
      iMaxWire = 0;
      fMinWire = 99999999;
      iMinWire = -1;
      hitsTemp.clear();
      for(auto hitsItr = hits.begin(); hitsItr != hits.end(); ++hitsItr){
        wire = (*hitsItr).Wire();
        if(fpointId_to_clusterId->at(hitsItr - hits.begin()) != clusterId)
          continue;
        channel = (*hitsItr).Channel();
        distance = (std::abs((*hitsItr).PeakTime()-slope*(Float_t)((*hitsItr).Wire())-intercept)/(std::sqrt(xyScale*xyScale*slope*slope+1)));
        if(distance < fMaxDistance+(((*hitsItr).EndTime()-(*hitsItr).StartTime())/2.)+indcolscaling && skip[hitsItr-hits.begin()]!=1){
          hitsTemp.push_back(hitsItr-hits.begin());
          sequenceHolder.push_back(channel);
        }
      }/// end iterator over hits
   
      if(hitsTemp.size() < 5) continue;
      currentHits.clear();  
      lastHits.clear();
      Int_t j; 
      currentHits.push_back(0);
      for(auto sequenceHolderItr = sequenceHolder.begin(); sequenceHolderItr+1 != sequenceHolder.end(); ++sequenceHolderItr) {
        j = 1;
        //while((chanFilt.BadChannel(*sequenceHolderItr+j)) == true) j++;
        if(sequenceHolder[sequenceHolderItr-sequenceHolder.begin()+1]-sequenceHolder[sequenceHolderItr-sequenceHolder.begin()] <= j + fMissedHits) currentHits.push_back(sequenceHolderItr-sequenceHolder.begin()+1);
        else if(currentHits.size() > lastHits.size()) {
          lastHits = currentHits;
          currentHits.clear();
        }
        else currentHits.clear();
      } 
      if(currentHits.size() > lastHits.size()) lastHits = currentHits;

      clusterHits.clear();    

      //if(lastHits.size() < 5) continue;
      if(lastHits.size() < (UInt_t)fMinHits) continue;







      //// Check if lastHits has hits with big gaps in it
      //// lastHits[i] is ordered in increasing channel and then increasing peak time,
      //// as a consequence, if the line has a negative slope and there are multiple hits in the line for a channel,
      //// we have to go back to the first hit (in terms of lastHits[i]) of that channel to find the distance
      //// between hits
      ////std::cout << "New line" << std::endl;
      ////std::cout << "slope: " << slope << std::endl;
      //int missedHits=0;
      //int lastHitsChannel = 0;
      //fMaxWire = 0;
      //iMaxWire = 0;
      //fMinWire = 99999999;
      //iMinWire = -1;
      //newChannel = false;
      //lastChannel = hits[hitsTemp[lastHits[0]]]->Wire()->RawDigit()->Channel();
      //for(auto lastHitsItr = lastHits.begin(); lastHitsItr != lastHits.end()-1; ++lastHitsItr) {

        //newChannel = false;
        //if(slope < 0){
          //if(hits[hitsTemp[*lastHitsItr+1]]->Wire()->RawDigit()->Channel() != lastChannel){
            //newChannel = true;
          //}           
        //}
        //if(slope > 0 || !newChannel){

          ////std::cout << hits[hitsTemp[lastHits[i]]]->Wire()->RawDigit()->Channel() << " " << ((hits[hitsTemp[lastHits[i]]]->StartTime()+hits[hitsTemp[lastHits[i]]]->EndTime())/2.) << std::endl;
          //pCorner0[0] = (hits[hitsTemp[*lastHitsItr]]->Wire()->RawDigit()->Channel())*wire_dist;
          //pCorner0[1] = ((hits[hitsTemp[*lastHitsItr]]->StartTime()+hits[hitsTemp[*lastHitsItr]]->EndTime())/2.)*tickToDist;
          //pCorner1[0] = (hits[hitsTemp[*lastHitsItr+1]]->Wire()->RawDigit()->Channel())*wire_dist;
          //pCorner1[1] = ((hits[hitsTemp[*lastHitsItr+1]]->StartTime()+hits[hitsTemp[*lastHitsItr+1]]->EndTime())/2.)*tickToDist;
          ////std::cout << std::sqrt( pow(pCorner0[0]-pCorner1[0],2) + pow(pCorner0[1]-pCorner1[1],2)) << std::endl;
          ////if(std::sqrt(pow(pCorner0[0]-pCorner1[0],2) + pow(pCorner0[1]-pCorner1[1],2)) > fMissedHitsDistance)
          //if((pCorner0[0]-pCorner1[0])*(pCorner0[0]-pCorner1[0]) + (pCorner0[1]-pCorner1[1])*(pCorner0[1]-pCorner1[1]) > fMissedHitsDistance*fMissedHitsDistance)
            //missedHits++;
        ////} else if (slope < 0 && newChannel && nHitsInChannel > 1){
        //} else if (slope < 0 && newChannel){
          ////std::cout << lastHitsChannel << " " << lastHits[i+1] << " " << lastChannel << std::endl;
          ////std::cout << hits[hitsTemp[lastHits[lastHitsChannel]]]->Wire()->RawDigit()->Channel() << " " << ((hits[hitsTemp[lastHits[lastHitsChannel]]]->StartTime()+hits[hitsTemp[lastHits[lastHitsChannel]]]->EndTime())/2.) << std::endl;
          //pCorner0[0] = (hits[hitsTemp[lastHits[lastHitsChannel]]]->Wire()->RawDigit()->Channel())*wire_dist;
          //pCorner0[1] = ((hits[hitsTemp[lastHits[lastHitsChannel]]]->StartTime()+hits[hitsTemp[lastHits[lastHitsChannel]]]->EndTime())/2.)*tickToDist;
          //pCorner1[0] = (hits[hitsTemp[*lastHitsItr+1]]->Wire()->RawDigit()->Channel())*wire_dist;
          //pCorner1[1] = ((hits[hitsTemp[*lastHitsItr+1]]->StartTime()+hits[hitsTemp[*lastHitsItr+1]]->EndTime())/2.)*tickToDist;
          ////std::cout << std::sqrt( pow(pCorner0[0]-pCorner1[0],2) + pow(pCorner0[1]-pCorner1[1],2)) << std::endl;
          ////if(std::sqrt( pow(pCorner0[0]-pCorner1[0],2) + pow(pCorner0[1]-pCorner1[1],2)) > fMissedHitsDistance             )
          //if((pCorner0[0]-pCorner1[0])*(pCorner0[0]-pCorner1[0]) + (pCorner0[1]-pCorner1[1])*(pCorner0[1]-pCorner1[1]) > fMissedHitsDistance*fMissedHitsDistance)
            //missedHits++;
          //lastChannel=hits[hitsTemp[*lastHitsItr+1]]->Wire()->RawDigit()->Channel();
          //lastHitsChannel=lastHitsItr-lastHits.begin()+1;
        //}
      //}
      ////std::cout << "missedHits " << missedHits << std::endl;
      ////std::cout << "lastHits.size() " << lastHits.size() << std::endl;
      ////std::cout << "missedHits/lastHits.size() " << (Double_t)missedHits/((Double_t)lastHits.size()-1) << std::endl;
      //if((Float_t)missedHits/((Float_t)lastHits.size()-1) > fMissedHitsToLineSize)
        //continue;







      if(std::abs(slope)>fMaxSlope ){
        continue;
      }

      //std::cout << std::endl;
      //std::cout  << "Found line!" << std::endl
                                       //<< "Slope: " << slope << std::endl
                                       //<< "Intercept: " << intercept << std::endl
                                       //<< "Number of hits: " << lastHits.size() << std::endl
                                       //<< "Wire: " << fMinWire << " Peak time: " 
                                       //<< hits[iMinWire]->PeakTime() << std::endl
                                       //<< "Wire: " << fMaxWire << " Peak time: " 
                                       //<< hits[iMaxWire]->PeakTime() << std::endl;


      // Add new line to list of lines
      Float_t totalQ = 0;
      std::vector<larlight::hit> lineHits;
      nClustersTemp++;
      ///std::cout << "nClusters: " << *nClusters << std::endl;
      for(auto lastHitsItr = lastHits.begin(); lastHitsItr != lastHits.end(); ++lastHitsItr) {
        fpointId_to_clusterId->at(hitsTemp[(*lastHitsItr)]) = nClustersTemp-1;
        //clusterHits.push_back(hits[hitsTemp[(*lastHitsItr)]]);
        //totalQ += clusterHits.back()->Charge();
        totalQ += hits.at(hitsTemp[(*lastHitsItr)]).Charge();
        wire = hits.at(hitsTemp[(*lastHitsItr)]).Wire();

        if(!accumPoints[hitsTemp[(*lastHitsItr)]])
          count--;
        
        skip[hitsTemp[(*lastHitsItr)]]=1;

        lineHits.push_back(hits.at(hitsTemp[(*lastHitsItr)]));


        /// Subtract points from the accumulator that have already been used
        if(accumPoints[hitsTemp[(*lastHitsItr)]]) 
          c.SubtractPoint(wire, (Int_t)(hits.at(hitsTemp[(*lastHitsItr)]).PeakTime()));
        
        if(wire < fMinWire){
          fMinWire = wire;
          iMinWire = hitsTemp[(*lastHitsItr)];
        }
        if(wire > fMaxWire){
          fMaxWire = wire;
          iMaxWire = hitsTemp[(*lastHitsItr)];
        }
      }
      pCornerMin[0] = (hits.at(iMinWire).Channel())*wire_dist;
      pCornerMin[1] = ((hits.at(iMinWire).StartTime()+hits.at(iMinWire).EndTime())/2.)*tickToDist;
      pCornerMax[0] = (hits.at(iMaxWire).Channel())*wire_dist;
      pCornerMax[1] = ((hits.at(iMaxWire).StartTime()+hits.at(iMaxWire).EndTime())/2.)*tickToDist;

      ///std::cout << std::endl;
      ///std::cout << "pCornerMin[0]: " << pCornerMin[0] << " pCornerMin[1]: " << pCornerMin[1] << std::endl;
      ///std::cout << "pCornerMax[0]: " << pCornerMax[0] << " pCornerMax[1]: " << pCornerMax[1] << std::endl;
      protoTrackToLoad.Init(nClustersTemp-1,
            slope,
            intercept,
            totalQ,
            pCornerMin[0],
            pCornerMin[1],
            pCornerMax[0],
            pCornerMax[1],
            iMinWire,
            iMaxWire,
            fMinWire,
            fMaxWire,
            lineHits);
      linesFound->push_back(protoTrackToLoad);
      
    }/// end if !std::isnan


    nLinesFound++;

    if(nLinesFound>(UInt_t)fMaxLines)
      break;

  }/// end loop over hits

  //std::cout << "Average cpu time: " << timeTotal/nAccum << std::endl;
  //std::cout << "Total cpu time: " << timeTotal << std::endl;

  lastHits.clear();

  listofxmax.clear();
  listofymax.clear();

  // saves a bitmap image of the accumulator (useful for debugging), 
  // with scaling based on the maximum cell value
  if(fSaveAccumulator){   
    UChar_t *outPix = new UChar_t [accDx*accDy];
    //finds the maximum cell in the accumulator for image scaling
    Int_t cell, pix = 0, maxCell = 0;
    for (Int_t y = 0; y < accDy; ++y){ 
      for (Int_t x = 0; x < accDx; ++x){
        cell = c.GetCell(y,x);
        if (cell > maxCell) maxCell = cell;
      }
    }
    for (Int_t y = 0; y < accDy; ++y){
      for (Int_t x = 0; x < accDx; ++x){ 
        //scales the pixel weights based on the maximum cell value     
        if(maxCell > 0)
          pix = (Int_t)((1500*c.GetCell(y,x))/maxCell);
        outPix[y*accDx + x] = pix;
      }
    }
                
    HLSSaveBMPFile("houghaccum.bmp", outPix, accDx, accDy);
    delete [] outPix;
  }// end if saving accumulator

  *nClusters=nClustersTemp;

  return 1; 
}


//------------------------------------------------------------------------------
larreco::HoughTransform::~HoughTransform()
{
}

//------------------------------------------------------------------------------
// returns a vector<Int_t> where the first is the overall maximum,
// the second is the max x value, and the third is the max y value.
inline std::vector<Int_t> larreco::HoughTransform::AddPointReturnMax(Int_t x, 
								   Int_t y)
{
  std::vector<Int_t> max(3,0);
  if (x>m_dx || y>m_dy || x<0.0 || y<0.0)
    return max;
  return DoAddPointReturnMax(x, y);
}



//------------------------------------------------------------------------------
inline bool larreco::HoughTransform::SubtractPoint(Int_t x, Int_t y)
{
  if (x>m_dx || y>m_dy || x<0.0 || y<0.0)
    return false;
  return DoSubtractPoint(x, y);
}


//------------------------------------------------------------------------------
void larreco::HoughTransform::Init(Int_t dx, 
				   Int_t dy, 
				   Int_t rhores,
				   Int_t numACells)
{
  m_numAngleCells=numACells;
  m_rhoResolutionFactor = rhores;
  m_accum.clear();
  //m_accum.resize(m_numAngleCells);
  m_numAccumulated = 0;   
  //   m_cosTable.clear();
  //   m_sinTable.clear();
  //m_cosTable.resize(m_numAngleCells);
  //m_sinTable.resize(m_numAngleCells);
  //if (dx == m_dx && dy == m_dy)
  //return;
  m_dx = dx;
  m_dy = dy;
  m_rowLength = (Int_t)(m_rhoResolutionFactor*2 * std::sqrt(dx*dx + dy*dy));
  m_accum.resize(m_numAngleCells);
  //for(int i = 0; i < m_numAngleCells; i++)
    //m_accum[i].resize((Int_t)(m_rowLength));

  //Int_t angleIndex;
  //Float_t a, angleStep = TMath::Pi()/m_numAngleCells;
  //for (a=0.0, angleIndex = 0; angleIndex < m_numAngleCells; ++angleIndex){
    //m_cosTable[angleIndex] = (cos(a));
    //m_sinTable[angleIndex] = (sin(a));
    //a += angleStep;
  //}
}


//------------------------------------------------------------------------------
Int_t larreco::HoughTransform::GetMax(Int_t &xmax, 
				    Int_t &ymax)
{
  Int_t maxVal = -1;
  for(UInt_t i = 0; i < m_accum.size(); i++){
    for(auto rhoIter=m_accum[i].begin(); rhoIter!=m_accum[i].end(); ++rhoIter){
      if((*rhoIter).second > maxVal) {
      //if((*rhoIter) > maxVal) {
        maxVal = (*rhoIter).second;
	//maxVal = (*rhoIter);
	xmax = i;
        ymax = (*rhoIter).first;
	//ymax = rhoIter-m_accum[i].begin();
      }
    }
  }

  return maxVal;
}

//------------------------------------------------------------------------------
// returns a vector<Int_t> where the first is the overall maximum,
// the second is the max x value, and the third is the max y value.
inline std::vector<Int_t> larreco::HoughTransform::DoAddPointReturnMax(Int_t x, 
								     Int_t y)
{
  std::vector<Int_t> max(3,-1);

  Int_t distCenter = (Int_t)(m_rowLength/2.);
  
  // prime the lastDist variable so our linear fill works below
  Int_t lastDist = (Int_t)(distCenter+(m_rhoResolutionFactor*x));

  //Int_t max_val = minHits-1;
  Int_t max_val = 0;
  Int_t dist;
  Int_t val;
  Int_t stepDir;
  Int_t cell;

  //std::pair<std::map<Int_t,Int_t>::iterator,bool> m_accumA;
  std::map<Int_t,Int_t>::iterator m_accumA;
  std::map<Int_t,Int_t>::iterator m_accumALast;

  // loop through all angles a from 0 to 180 degrees
  Float_t angleStep = PI/m_numAngleCells;
  Float_t angleStepInverse = m_numAngleCells/PI;
  Float_t angleStepInt;
  //for (Float_t a = angleStep; a < TMath::Pi(); a+=angleStep){
  for (Float_t a = angleStep; a < PI; a+=angleStep){
   
    angleStepInt = (a*angleStepInverse);
    // Calculate the basic line equation dist = cos(a)*x + sin(a)*y.
    // Shift to center of row to cover negative values
    dist = (Int_t)(distCenter+(m_rhoResolutionFactor*(cos(a)*x + sin(a)*y)));
    // sanity check to make sure we stay within our row
    //if (dist >= 0 && dist<m_rowLength){
      if(lastDist==dist){
        m_accumA = m_accum[angleStepInt].insert(m_accum[angleStepInt].end(),std::pair<Int_t,Int_t>(lastDist,0));
        //val = m_accumA.first->second++;
        val = m_accumA->second++;
        
        if( max_val < val){
	  max_val = val;
          max[0] = val;
          max[1] = lastDist;
          max[2] = (angleStepInt);
        }
        m_accumALast = m_accumA;
        //mf::LogVerbatim("HoughBaseAlg") << "First, a: " << a << " lastDist: " << lastDist << std::endl;
      }
      else{
        // fill in all values in row a, not just a single cell
        m_accumALast = m_accum[angleStepInt].begin();
        stepDir = dist>lastDist ? 1 : -1;
        for (cell=lastDist; cell!=dist; cell+=stepDir){  
          m_accumA = m_accum[angleStepInt].insert(m_accumALast,std::pair<Int_t,Int_t>(cell,0));
          //val = m_accumA.first->second++;
          val = m_accumA->second++;
          //// Note, m_accum is a vector of associative containers, "a" calls the vector element, "cell" is the container key, and the ++ iterates the value correspoding to the key
          if(max_val < val){
	    max_val = val;
            max[0] = val;
            max[1] = cell;
            max[2] = (angleStepInt);
          }
          m_accumALast = m_accumA;
        }      
      }
    //}
    lastDist = dist;
  }
  m_numAccumulated++;


  //mf::LogVerbatim("HoughBaseAlg") << "Add point says xmax: " << *xmax << " ymax: " << *ymax << std::endl;

  return max;
}



//------------------------------------------------------------------------------
inline bool larreco::HoughTransform::DoSubtractPoint(Int_t x, Int_t y)
{
  Int_t distCenter = (Int_t)(m_rowLength/2.);
 
  // prime the lastDist variable so our linear fill works below
  Int_t lastDist = (Int_t)(distCenter+(m_rhoResolutionFactor*x));
  Int_t dist;
  Int_t stepDir;
  // loop through all angles a from 0 to 180 degrees
  Float_t angleStep = PI/m_numAngleCells;
  for (Float_t a = angleStep; a < PI; a+=angleStep){
    // Calculate the basic line equation dist = cos(a)*x + sin(a)*y.
    // Shift to center of row to cover negative values
    dist = (Int_t)(distCenter+(m_rhoResolutionFactor*((cos(a)*x + sin(a)*y))));
    // sanity check to make sure we stay within our row
      if(lastDist==dist)
	m_accum[(Int_t)(a/angleStep)][lastDist]--;
      else{
	// fill in all values in row a, not just a single cell
	stepDir = dist>lastDist ? 1 : -1;
	for (Int_t cell=lastDist; cell!=dist; cell+=stepDir){   
	  m_accum[(Int_t)(a/angleStep)][cell]--;//maybe add weight of hit here?
          // Note, m_accum is a vector of associative containers, "a" calls the vector element, "cell" is the container key, and the -- iterates the value correspoding to the key
	}      
      }
    lastDist = dist;
  }
  m_numAccumulated--;

  return true;
}

//------------------------------------------------------------------------------
//this method saves a BMP image of the Hough Accumulator, which can be viewed with gimp
void larreco::HoughBaseAlg::HLSSaveBMPFile(const Char_t *fileName, UChar_t *pix, Int_t dx, Int_t dy)
{
  ofstream bmpFile(fileName, std::ios::binary);
  bmpFile.write("B", 1);
  bmpFile.write("M", 1);
  Int_t bitsOffset = 54 +256*4; 
  Int_t size = bitsOffset + dx*dy; //header plus 256 entry LUT plus pixels
  bmpFile.write((const Char_t *)&size, 4);
  Int_t reserved = 0;
  bmpFile.write((const Char_t *)&reserved, 4);
  bmpFile.write((const Char_t *)&bitsOffset, 4);
  Int_t bmiSize = 40;
  bmpFile.write((const Char_t *)&bmiSize, 4);
  bmpFile.write((const Char_t *)&dx, 4);
  bmpFile.write((const Char_t *)&dy, 4);
  short planes = 1;
  bmpFile.write((const Char_t *)&planes, 2);
  short bitCount = 8;
  bmpFile.write((const Char_t *)&bitCount, 2);
  Int_t i, temp = 0;
  for (i=0; i<6; i++)
    bmpFile.write((const Char_t *)&temp, 4);  // zero out optional color info
  // write a linear LUT
  Char_t lutEntry[4]; // blue,green,red
  lutEntry[3] = 0;  // reserved part
  for (i=0; i<256; i++)
    {
      lutEntry[0] = lutEntry[1] = lutEntry[2] = i;
      bmpFile.write(lutEntry, sizeof lutEntry);
    }
  // write the actual pixels
  bmpFile.write((const Char_t *)pix, dx*dy);
}
 

//------------------------------------------------------------------------------
size_t larreco::HoughBaseAlg::FastTransform(std::vector<larlight::cluster>        const& clusIn,
					    std::vector<larlight::hit>            const& hitIn,
					    std::vector<larlight::cluster>             & ccol,  
					    std::vector<std::vector<UShort_t> >        & clusHitsOut_index)
{
  std::vector<Int_t> skip;  

  //art::FindManyP<recob::Hit> fmh(clusIn, evt, label);

  const larutil::Geometry* geom = larutil::Geometry::GetME();
  //  const larutil::DetectorProperties* detprop = larutil::DetectorProperties::GetME();
  //  const larutil::LArProperties* larprop = larutil::LArProperties::GetME();

  //filter::ChannelFilter chanFilt;
  HoughTransform c;

  // Get the random number generator
  //art::ServiceHandle<art::RandomNumberGenerator> rng;
  //CLHEP::HepRandomEngine & engine = rng -> getEngine();
  //CLHEP::RandFlat flat(engine);

  std::vector<UShort_t> hit;

  for(auto view : geom->Views() ){

    print(larlight::MSG::DEBUG,__FUNCTION__,Form("Analyzing view %d",view));

    std::vector<larlight::cluster>::const_iterator clusterIter = clusIn.begin();
    Int_t clusterID = 0;//the unique ID of the cluster
    
    size_t cinctr = 0;
    while(clusterIter != clusIn.end()) {

      print(larlight::MSG::DEBUG,__FUNCTION__,Form("Analyzing cinctr=%zu ",cinctr));

      hit.clear();
      if(fPerCluster){
	if((*clusterIter).View() == view) hit = clusIn.at(cinctr).association((*hitIn.begin()).data_type());
      }
      else{   
	while(clusterIter != clusIn.end()){
	  if( (*clusterIter).View() == view ){
	    
	    hit = clusIn.at(cinctr).association((*hitIn.begin()).data_type());
	  }// end if cluster is in correct view
	  clusterIter++;
	  ++cinctr;
	}//end loop over clusters
      }//end if not fPerCluster
      if(hit.size() == 0){ 
	if(fPerCluster){
	  clusterIter++;
	  ++cinctr;
	}
	continue;
      }
      
      /*
      //factor to make x and y scale the same units
      Double_t xyScale  = .001*larprop->DriftVelocity(larprop->Efield(),larprop->Temperature());
      xyScale        *= detprop->SamplingRate()/geom->WirePitch(0,1,p,t,cs);
      
      Int_t x, y;
      Int_t dx = geom->Cryostat(cs).TPC(t).Plane(p).Nwires();//number of wires 
      Int_t dy = hit[0]->Wire()->NSignal();//number of time samples. 
      skip.clear();
      skip.resize(hit.size());
      std::vector<Int_t> listofxmax;
      std::vector<Int_t> listofymax;  
      std::vector<Int_t> hitTemp;        //indecies ofcandidate hits
      std::vector<Int_t> sequenceHolder; //channels of hits in list
      std::vector<Int_t> currentHits;    //working vector of hits 
      std::vector<Int_t> lastHits;       //best list of hits
      art::PtrVector<recob::Hit> clusterHits;
      Double_t indcolscaling = 0.;       //a parameter to account for the different 
      //characteristic hit width of induction and collection plane
      Double_t centerofmassx = 0;
      Double_t centerofmassy = 0;
      Double_t denom = 0; 
      Double_t intercept=0.;
      Double_t slope = 0.;
      //this array keeps track of the hits that have already been associated with a line. 
      Int_t xMax = 0;
      Int_t yMax = 0;
      Double_t rho;
      Double_t theta; 
      Int_t accDx(0), accDy(0);
      
      
      //Init specifies the size of the two-dimensional accumulator 
      //(based on the arguments, number of wires and number of time samples). 
      //adds all of the hits (that have not yet been associated with a line) to the accumulator
      c.Init(dx,dy,fRhoResolutionFactor,fNumAngleCells);
      
      
      // count is how many points are left to randomly insert
      UInt_t count = hit.size();
      std::vector<UInt_t> accumPoints;
      accumPoints.resize(hit.size());
      Int_t nAccum = 0;
      UInt_t nLinesFound = 0;
      
      for( ; count > 0; count--){
      
      
      // The random hit we are examining
      UInt_t randInd = (UInt_t)(flat.fire()*hit.size());
      
      // Skip if it's already in a line
      if(skip[randInd]==1)
      continue;
      
      // If we have already accumulated the point, skip it
      if(accumPoints[randInd])
      continue;
      accumPoints[randInd]=1;
      
      // zeroes out the neighborhood of all previous lines  
      for(UInt_t i = 0; i < listofxmax.size(); ++i){
      Int_t yClearStart = listofymax[i] - fRhoZeroOutRange;
      if (yClearStart < 0) yClearStart = 0;
      
      Int_t yClearEnd = listofymax[i] + fRhoZeroOutRange;
      if (yClearEnd >= accDy) yClearEnd = accDy - 1;
      
      Int_t xClearStart = listofxmax[i] - fThetaZeroOutRange;
      if (xClearStart < 0) xClearStart = 0;
      
      Int_t xClearEnd = listofxmax[i] + fThetaZeroOutRange;
      if (xClearEnd >= accDx) xClearEnd = accDx - 1;
      
      for (y = yClearStart; y <= yClearEnd; ++y){
      for (x = xClearStart; x <= xClearEnd; ++x){
      c.SetCell(y,x,0);
      }
      }
      }// end loop over size of listxmax
      
      //find the weightiest cell in the accumulator.
      Int_t maxCell = 0;
      UInt_t wireMax = hit[randInd]->WireID().Wire;
      xMax = 0;
      yMax = 0;
      
      // Add the randomly selected point to the accumulator
      maxCell = c.AddPointReturnMax(wireMax, (Int_t)(hit[randInd]->PeakTime()), &yMax, &xMax, fMinHits);
      nAccum++; 
      
      // Continue if the biggest maximum for the randomly selected point is smaller than fMinHits
      if (maxCell < fMinHits) 
      continue;
      
      // Find the center of mass of the 3x3 cell system (with maxCell at the center). 
      denom = centerofmassx = centerofmassy = 0;
      
      if(xMax > 0 && yMax > 0 && xMax + 1 < accDx && yMax + 1 < accDy){  
      for(Int_t i = -1; i < 2; ++i){
      for(Int_t j = -1; j < 2; ++j){
      denom += c.GetCell(yMax+i,xMax+j);
      centerofmassx += j*c.GetCell(yMax+i,xMax+j);
      centerofmassy += i*c.GetCell(yMax+i,xMax+j);
      }
      }
      centerofmassx /= denom;
      centerofmassy /= denom;      
      }
      else  centerofmassx = centerofmassy = 0;
      
      //fill the list of cells that have already been found
      listofxmax.push_back(xMax);
      listofymax.push_back(yMax);
      
      // Find the lines equation
      c.GetEquation(yMax+centerofmassy, xMax+centerofmassx, rho, theta);
      //c.GetEquation(yMax, xMax, rho, theta);
      slope = -1./tan(theta);    
      intercept = (rho/sin(theta));
      //mf::LogVerbatim("HoughBaseAlg") << std::endl;
      //mf::LogVerbatim("HoughBaseAlg") << "slope: " << slope << " intercept: " << intercept << std::endl; 
      //mf::LogInfo("HoughBaseAlg") << "slope: " << slope << " intercept: " << intercept;
      Double_t distance;
      /// \todo: the collection plane's characteristic hit width's are, 
      /// \todo: on average, about 5 time samples wider than the induction plane's. 
      /// \todo: this is hard-coded for now.
      if(sigt == geo::kInduction)
      indcolscaling = 5.;
      else
      indcolscaling = 0.;
      // What is this?   
      indcolscaling = 0;
      
      if(!std::isinf(slope) && !std::isnan(slope)){
      sequenceHolder.clear();
      hitTemp.clear();
      for(size_t i = 0; i < hit.size(); ++i){
      distance = (TMath::Abs(hit[i]->PeakTime()-slope*(Double_t)(hit[i]->WireID().Wire)-intercept)/(std::sqrt(pow(xyScale*slope,2)+1)));
      
      if(distance < fMaxDistance+((hit[i]->EndTime()-hit[i]->StartTime())/2.)+indcolscaling  && skip[i]!=1){
      hitTemp.push_back(i);
      sequenceHolder.push_back(hit[i]->Channel());
      }
      
      }// end loop over hits
      
      if(hitTemp.size() < 2) continue;
      currentHits.clear();  
      lastHits.clear();
      Int_t j; 
      currentHits.push_back(0);
      for(size_t i = 0; i + 1 < sequenceHolder.size(); ++i){  
      j = 1;
      while((chanFilt.BadChannel(sequenceHolder[i]+j)) == true) j++;
      if(sequenceHolder[i+1]-sequenceHolder[i] <= j + fMissedHits) currentHits.push_back(i+1);
      else if(currentHits.size() > lastHits.size()) {
      lastHits = currentHits;
      currentHits.clear();
      }
      else currentHits.clear();
      }
      
      
      if(currentHits.size() > lastHits.size()) lastHits = currentHits;
      
      
      
      
      // Check if lastHits has hits with big gaps in it
      UInt_t     channel = hit[0]->Wire()->RawDigit()->Channel();
      Double_t wirePitch = geom->WirePitch(geom->View(channel));
      Double_t wire_dist = wirePitch;
      Double_t tickToDist = larprop->DriftVelocity(larprop->Efield(),larprop->Temperature());
      tickToDist *= 1.e-3 * detprop->SamplingRate(); // 1e-3 is conversion of 1/us to 1/ns
      //std::cout << "New line" << std::endl;
      Int_t missedHits=0;
      for(size_t i = 0; i < lastHits.size()-1; ++i) {
      //std::cout << hit[hitTemp[lastHits[i]]]->Wire()->RawDigit()->Channel() << std::endl;
      Double_t pCorner0[2];
      pCorner0[0] = (hit[hitTemp[lastHits[i]]]->Wire()->RawDigit()->Channel())*wire_dist;
      pCorner0[1] = ((hit[hitTemp[lastHits[i]]]->StartTime()+hit[hitTemp[lastHits[i]]]->EndTime())/2.)*tickToDist;
      Double_t pCorner1[2];
      pCorner1[0] = (hit[hitTemp[lastHits[i+1]]]->Wire()->RawDigit()->Channel())*wire_dist;
      pCorner1[1] = ((hit[hitTemp[lastHits[i+1]]]->StartTime()+hit[hitTemp[lastHits[i+1]]]->EndTime())/2.)*tickToDist;
      //std::cout << std::sqrt( pow(pCorner0[0]-pCorner1[0],2) + pow(pCorner0[1]-pCorner1[1],2)) << std::endl;
      if(std::sqrt( pow(pCorner0[0]-pCorner1[0],2) + pow(pCorner0[1]-pCorner1[1],2)) > fMissedHitsDistance             )
      missedHits++;
      }
      //std::cout << "missedHits " << missedHits << std::endl;
      //std::cout << "lastHits.size() " << lastHits.size() << std::endl;
      //std::cout << "missedHits/lastHits.size() " << (Double_t)missedHits/(Double_t)lastHits.size() << std::endl;
      if((Double_t)missedHits/(Double_t)lastHits.size() > fMissedHitsToLineSize)
      continue;
      
      
      
      
      
      clusterHits.clear();    
      Double_t totalQ = 0.;
      if(lastHits.size() < 5) continue;
      
      
      for(size_t i = 0; i < lastHits.size(); ++i) {
      clusterHits.push_back(hit[hitTemp[lastHits[i]]]);
      totalQ += clusterHits.back()->Charge();
      skip[hitTemp[lastHits[i]]]=1;
      } 
      //protection against very steep uncorrelated hits
      if(std::abs(slope)>fMaxSlope 
      && std::abs((*clusterHits.begin())->Wire()->RawDigit()->Channel()-
      clusterHits[clusterHits.size()-1]->Wire()->RawDigit()->Channel())>=0
      )
      continue;
      
      
      
      UInt_t sw = (*clusterHits.begin())->WireID().Wire;
      UInt_t ew = (*(clusterHits.end()-1))->WireID().Wire;
      
      recob::Cluster cluster(sw, 0.,
      (*clusterHits.begin())->PeakTime(), 0.,
      ew, 0., 
      (clusterHits[clusterHits.size()-1])->PeakTime(), 0.,
      slope, 0., 
      -999., 0., 
      totalQ,
      geom->View((*clusterHits.begin())->Channel()),
      clusterID);	      
      
      ++clusterID;
      ccol.push_back(cluster);
      clusHitsOut.push_back(clusterHits);
      
      //Turn off hit sharing. T. Yang 9/14/12
      //	      //allow Double_t assignment of first and last hits
      //	      for(size_t i = 0; i < lastHits.size(); ++i){ 
      //		if(skip[hitTemp[lastHits[i]]] ==1){
      //		  channel = hit[hitTemp[lastHits[i]]]->Wire()->RawDigit()->Channel();  
      //		  if( channel == sc || channel == ec) skip[i] = 0;
      //		}
      //	      }
      
      }// end if !std::isnan
      
      nLinesFound++;
      
      if(nLinesFound>(UInt_t)fMaxLines)
      break;
      
      
      }// end loop over hits*/
      
      std::vector<Double_t> slopevec;std::vector<Double_t> totalQvec;
      std::vector<std::vector<UShort_t> >   planeClusHitsOut;

      std::vector<larlight::hit> these_hits;
      these_hits.reserve(hit.size());
      for(auto const h : hit)
	these_hits.push_back(hitIn.at(h));

      this->FastTransform(these_hits,planeClusHitsOut,slopevec,totalQvec );
      
      //LOG_DEBUG("HoughBaseAlg") << "Made it through FastTransform" << planeClusHitsOut.size();

      for(size_t xx = 0; xx < planeClusHitsOut.size(); ++xx){
	
	std::vector<UShort_t> ass_index;
	ass_index.reserve(planeClusHitsOut.at(xx).size());
	for(auto const index : planeClusHitsOut.at(xx))
	  ass_index.push_back(hit.at(index));

	larlight::cluster cl(larlight::DATA::HoughCluster);
	std::vector<Double_t> start(2,0);
	std::vector<Double_t> end(2,0);
	UShort_t start_index = (*(ass_index.begin()));
	UShort_t end_index = (*(ass_index.rbegin()));
	start[0] = hitIn.at(start_index).Wire();
	start[1] = hitIn.at(start_index).PeakTime();

	end[0] = hitIn.at(end_index).Wire();
	end[1] = hitIn.at(end_index).PeakTime();
	
	cl.set_view(geom->View(hitIn.at(start_index).Channel()));
	cl.set_id(clusterID);
	cl.set_charge(totalQvec.at(xx));
	cl.set_start_vtx(start);
	cl.set_end_vtx(end);
	cl.set_dtdw(slopevec.at(xx));
	cl.add_association(hitIn.at(start_index).data_type(), ass_index);
	ccol.push_back(cl);
	++clusterID;
	clusHitsOut_index.push_back(ass_index);
      }
      
      
      hit.clear();
      //  lastHits.clear();
      if(clusterIter != clusIn.end()){
	clusterIter++;
	++cinctr;
      }
      // listofxmax.clear();
      // listofymax.clear();
    }//end loop over clusters
    
  }// end loop over views

  return ccol.size(); 

}

//------------------------------------------------------------------------------
size_t larreco::HoughBaseAlg::FastTransform(std::vector<larlight::hit>          const& clusIn,
					    std::vector<std::vector<UShort_t > >     & clusHitsOut )
  {
   std::vector<Double_t> slopevec; std::vector<Double_t> totalQvec;
   return  FastTransform( clusIn, clusHitsOut, slopevec, totalQvec );
        
  }



//------------------------------------------------------------------------------
size_t larreco::HoughBaseAlg::FastTransform(std::vector <larlight::hit>        const& clusIn,
					    std::vector<std::vector<UShort_t> >     & clusHitsOut, 
					    std::vector<Double_t> &slopevec, std::vector<Double_t> &totalQvec )
{
  std::vector<Int_t> skip;  

  //art::FindManyP<recob::Hit> fmh(clusIn, evt, label);

  //art::ServiceHandle<geo::Geometry> geom;
  //art::ServiceHandle<util::LArProperties> larprop;
  //art::ServiceHandle<util::DetectorProperties> detprop;
  const larutil::Geometry* geom = larutil::Geometry::GetME();
  const larutil::DetectorProperties* detprop = larutil::DetectorProperties::GetME();
  const larutil::LArProperties* larprop = larutil::LArProperties::GetME();

  //filter::ChannelFilter chanFilt;
  HoughTransform c;

  // Get the random number generator
  //art::ServiceHandle<art::RandomNumberGenerator> rng;
  //CLHEP::HepRandomEngine & engine = rng -> getEngine();
  //CLHEP::RandFlat flat(engine);

//   for(size_t cs = 0; cs < geom->Ncryostats(); ++cs){
//     for(size_t t = 0; t < geom->Cryostat(cs).NTPC(); ++t){
//       for(UInt_t p = 0; p < geom->Cryostat(cs).TPC(t).Nplanes(); ++p) {
// 	art::PtrVector<recob::Cluster>::const_iterator clusterIter = clusIn.begin();
// 	Int_t clusterID = 0;//the unique ID of the cluster


	

  size_t cinctr = 0;
  //while(clusterIter != clusIn.end()) {
  //hit.clear();
  //hit.reserve(clusIn.size());
  //for(size_t i=0; i<clusIn.size(); ++i)
  //hit.push_back(i);   // this is new
  
  if(clusIn.size() == 0){ 
    if(fPerCluster){
      ++cinctr;
    }
    return -1;
  }
  
  UInt_t p = geom->ChannelToPlane(clusIn.at(0).Channel());
  
  //	geo::View_t    view = geom->Cryostat(cs).TPC(t).Plane(p).View();
  larlight::GEO::SigType_t sigt = geom->SignalType(p);
  
  // if(fPerCluster){
  //   if((*clusterIter)->View() == view) hit = fmh.at(cinctr);
  // }
  // else{   
  //  while(clusterIter != clusIn.end()){
  //    if( (*clusterIter)->View() == view ){
  
	//	hit = fmh.at(cinctr);
	 //     }// end if cluster is in correct view
	 //     clusterIter++;
	  //    ++cinctr;
	//    }//end loop over clusters
	//  }//end if not fPerCluster

  if(clusIn.size() == 0){ 
    if(fPerCluster){
      //      clusterIter++;
      ++cinctr;
    }
    return -1; //continue;
  }
  
  //factor to make x and y scale the same units
  Double_t xyScale  = .001*larprop->DriftVelocity(larprop->Efield(),larprop->Temperature());
  xyScale        *= detprop->SamplingRate()/geom->WirePitch(0,1,p);
  Double_t wirePitch = geom->WirePitch(clusIn.at(0).View());
  
  Int_t x, y;
  Int_t dx = geom->Nwires(p); //number of wires
  //Int_t dy = hit.at(0)->Wire()->NSignal();//number of time samples. 
  Int_t dy = detprop->ReadOutWindowSize();
  skip.clear();
  skip.resize(clusIn.size());
  std::vector<Int_t> listofxmax;
  std::vector<Int_t> listofymax;  
  std::vector<Int_t> hitTemp;        //indecies ofcandidate hits
  std::vector<Int_t> sequenceHolder; //channels of hits in list
  std::vector<Int_t> currentHits;    //working vector of hits 
  std::vector<Int_t> lastHits;       //best list of hits
  //art::PtrVector<recob::Hit> clusterHits;
  std::vector<UShort_t> clusterHits;
  Float_t indcolscaling = 0.;       //a parameter to account for the different 
  //characteristic hit width of induction and collection plane
  Float_t centerofmassx = 0;
  Float_t centerofmassy = 0;
  Float_t denom = 0; 
  Float_t intercept=0.;
  Float_t slope = 0.;
  //this array keeps track of the hits that have already been associated with a line. 
  Int_t xMax = 0;
  Int_t yMax = 0;
  Float_t rho;
  Float_t theta; 
  Int_t accDx(0), accDy(0);
  
  
  //Init specifies the size of the two-dimensional accumulator 
  //(based on the arguments, number of wires and number of time samples). 
  //adds all of the hits (that have not yet been associated with a line) to the accumulator
  c.Init(dx,dy,fRhoResolutionFactor,fNumAngleCells);
  
  // count is how many points are left to randomly insert
  UInt_t count = clusIn.size();
  std::vector<UInt_t> accumPoints;
  accumPoints.resize(clusIn.size());
  Int_t nAccum = 0;
  UInt_t nLinesFound = 0;
  
  for( ; count > 0; count--){
    
    
    // The random hit we are examining
    UInt_t randInd = (UInt_t)(gRandom->Uniform()*clusIn.size());
    
    //LOG_DEBUG("HoughBaseAlg") << "randInd=" << randInd << " and size is " << clusIn.size();

  // Skip if it's already in a line
    if(skip.at(randInd)==1)
      continue;
    
    // If we have already accumulated the point, skip it
    if(accumPoints.at(randInd))
      continue;
    accumPoints.at(randInd)=1;
    
    // zeroes out the neighborhood of all previous lines  
    for(UInt_t i = 0; i < listofxmax.size(); ++i){
      Int_t yClearStart = listofymax.at(i) - fRhoZeroOutRange;
      if (yClearStart < 0) yClearStart = 0;
      
      Int_t yClearEnd = listofymax.at(i) + fRhoZeroOutRange;
      if (yClearEnd >= accDy) yClearEnd = accDy - 1;
      
      Int_t xClearStart = listofxmax.at(i) - fThetaZeroOutRange;
      if (xClearStart < 0) xClearStart = 0;
      
      Int_t xClearEnd = listofxmax.at(i) + fThetaZeroOutRange;
      if (xClearEnd >= accDx) xClearEnd = accDx - 1;
      
      for (y = yClearStart; y <= yClearEnd; ++y){
	for (x = xClearStart; x <= xClearEnd; ++x){
	  c.SetCell(y,x,0);
	}
      }
    }// end loop over size of listxmax
  

    //find the weightiest cell in the accumulator.
    Int_t maxCell = 0;
    UInt_t wireMax = clusIn.at(randInd).Wire();
    
    // Add the randomly selected point to the accumulator
    std::vector<Int_t> max = c.AddPointReturnMax(wireMax, 
					       (Int_t)(clusIn.at(randInd).PeakTime()));
    maxCell = max.at(0);
    xMax    = max.at(1);
    yMax    = max.at(2);
    nAccum++; 
    
    // Continue if the biggest maximum for the randomly selected point is smaller than fMinHits
    if (maxCell < fMinHits) 
      continue;
    
    // Find the center of mass of the 3x3 cell system (with maxCell at the center). 
    denom = centerofmassx = centerofmassy = 0;
    
    if(xMax > 0 && yMax > 0 && xMax + 1 < accDx && yMax + 1 < accDy){  
      for(Int_t i = -1; i < 2; ++i){
	for(Int_t j = -1; j < 2; ++j){
	  denom += c.GetCell(yMax+i,xMax+j);
	  centerofmassx += j*c.GetCell(yMax+i,xMax+j);
	  centerofmassy += i*c.GetCell(yMax+i,xMax+j);
	}
      }
      centerofmassx /= denom;
      centerofmassy /= denom;      
    }
    else  centerofmassx = centerofmassy = 0;
    
    //fill the list of cells that have already been found
    listofxmax.push_back(xMax);
    listofymax.push_back(yMax);
    
    // Find the lines equation
    c.GetEquation(yMax+centerofmassy, xMax+centerofmassx, rho, theta);
    //c.GetEquation(yMax, xMax, rho, theta);
    slope = -1./tan(theta);    
    intercept = (rho/sin(theta));
    //mf::LogVerbatim("HoughBaseAlg") << std::endl;
    //mf::LogVerbatim("HoughBaseAlg") << "slope: " << slope << " intercept: " << intercept << std::endl; 
    //mf::LogInfo("HoughBaseAlg") << "slope: " << slope << " intercept: " << intercept;
    Double_t distance;
    /// \todo: the collection plane's characteristic hit width's are, 
    /// \todo: on average, about 5 time samples wider than the induction plane's. 
    /// \todo: this is hard-coded for now.
    if(sigt == larlight::GEO::kInduction)
      indcolscaling = 5.;
    else
      indcolscaling = 0.;
    // What is this?   
    //indcolscaling = 0;
    
    
        
    if(!std::isinf(slope) && !std::isnan(slope)){
      sequenceHolder.clear();
      hitTemp.clear();
      for(size_t i = 0; i < clusIn.size(); ++i){
	distance = (TMath::Abs(clusIn.at(i).PeakTime()-slope*(Double_t)(clusIn.at(i).Wire())-intercept)/(std::sqrt(pow(xyScale*slope,2)+1)));
	
	if(distance < fMaxDistance+((clusIn.at(i).EndTime()-clusIn.at(i).StartTime())/2.)+indcolscaling  && skip.at(i)!=1){
	  hitTemp.push_back(i);
	  sequenceHolder.push_back(clusIn.at(i).Channel());
	}
	
      }// end loop over hits
      
      if(hitTemp.size() < 2) continue;
      currentHits.clear();  
      lastHits.clear();
      Int_t j; 
      currentHits.push_back(0);
      for(size_t i = 0; i + 1 < sequenceHolder.size(); ++i){  
	j = 1;
	//while((chanFilt.BadChannel(sequenceHolder.at(i)+j)) == true) j++;
	if(sequenceHolder.at(i+1)-sequenceHolder.at(i) <= j + fMissedHits) currentHits.push_back(i+1);
	else if(currentHits.size() > lastHits.size()) {
	  lastHits = currentHits;
	  currentHits.clear();
	}
	else currentHits.clear();
      }
      
      
      if(currentHits.size() > lastHits.size()) lastHits = currentHits;
      
      
      
      // Check if lastHits has hits with big gaps in it
      // lastHits[i] is ordered in increasing channel and then increasing peak time,
      // as a consequence, if the line has a negative slope and there are multiple hits in the line for a channel,
      // we have to go back to the first hit (in terms of lastHits[i]) of that channel to find the distance
      // between hits
      //std::cout << "New line" << std::endl;
      Double_t wire_dist = wirePitch;
      Double_t tickToDist = larprop->DriftVelocity(larprop->Efield(),larprop->Temperature());
      tickToDist *= 1.e-3 * detprop->SamplingRate(); // 1e-3 is conversion of 1/us to 1/ns
      Int_t missedHits=0;
      Int_t lastHitsChannel = 0;//lastHits.at(0);
      Int_t nHitsPerChannel = 1;

      /*
      LOG_DEBUG("HoughBaseAlg") << "filling the pCorner arrays around here..."
				<< "\n but first, lastHits size is " << lastHits.size() 
				<< " and lastHitsChannel=" << lastHitsChannel;
      */

      Double_t pCorner0[2];
      Double_t pCorner1[2];
      UInt_t lastChannel = clusIn.at(hitTemp.at(lastHits.at(0))).Channel();

      for(size_t i = 0; i < lastHits.size()-1; ++i) {
	bool newChannel = false;
	if(slope < 0){
	  if(clusIn.at(hitTemp.at(lastHits.at(i+1))).Channel() != lastChannel){
	    newChannel = true;
	  }
	  if(clusIn.at(hitTemp.at(lastHits.at(i+1))).Channel() == lastChannel)
	    nHitsPerChannel++;
	}
	

	if(slope > 0 || (!newChannel && nHitsPerChannel <= 1)){

	  //std::cout << hits[hitsTemp[lastHits[i]]]->Wire()->RawDigit()->Channel() << " " << ((hits[hitsTemp[lastHits[i]]]->StartTime()+hits[hitsTemp[lastHits[i]]]->EndTime())/2.) << std::endl;
	  pCorner0[0] = (clusIn.at(hitTemp.at(lastHits.at(i))).Channel())*wire_dist;
	  pCorner0[1] = ((clusIn.at(hitTemp.at(lastHits.at(i))).StartTime()+clusIn.at(hitTemp.at(lastHits.at(i))).EndTime())/2.)*tickToDist;
	  pCorner1[0] = (clusIn.at(hitTemp.at(lastHits.at(i+1))).Channel())*wire_dist;
	  pCorner1[1] = ((clusIn.at(hitTemp.at(lastHits.at(i+1))).StartTime()+clusIn.at(hitTemp.at(lastHits.at(i+1))).EndTime())/2.)*tickToDist;
	  //std::cout << std::sqrt( pow(pCorner0[0]-pCorner1[0],2) + pow(pCorner0[1]-pCorner1[1],2)) << std::endl;
	  if(std::sqrt( pow(pCorner0[0]-pCorner1[0],2) + pow(pCorner0[1]-pCorner1[1],2)) > fMissedHitsDistance             )
	    missedHits++;
	}


	else if (slope < 0 && newChannel && nHitsPerChannel > 1){

	  //std::cout << hits[hitsTemp[lastHits[lastHitsChannel]]]->Wire()->RawDigit()->Channel() << " " << ((hits[hitsTemp[lastHits[lastHitsChannel]]]->StartTime()+hits[hitsTemp[lastHits[lastHitsChannel]]]->EndTime())/2.) << std::endl;
	  pCorner0[0] = (clusIn.at(hitTemp.at(lastHits.at(lastHitsChannel))).Channel())*wire_dist;
	  pCorner0[1] = ((clusIn.at(hitTemp.at(lastHits.at(lastHitsChannel))).StartTime()+clusIn.at(hitTemp.at(lastHits.at(lastHitsChannel))).EndTime())/2.)*tickToDist;
	  pCorner1[0] = (clusIn.at(hitTemp.at(lastHits.at(i+1))).Channel())*wire_dist;
	  pCorner1[1] = ((clusIn.at(hitTemp.at(lastHits.at(i+1))).StartTime()+clusIn.at(hitTemp.at(lastHits.at(i+1))).EndTime())/2.)*tickToDist;
	  //std::cout << std::sqrt( pow(pCorner0[0]-pCorner1[0],2) + pow(pCorner0[1]-pCorner1[1],2)) << std::endl;
	  if(std::sqrt( pow(pCorner0[0]-pCorner1[0],2) + pow(pCorner0[1]-pCorner1[1],2)) > fMissedHitsDistance             )
	    missedHits++;
	  lastChannel=clusIn.at(hitTemp.at(lastHits.at(i))).Channel();
	  lastHitsChannel=i+1;
	  nHitsPerChannel=0;
	}
      }

      //std::cout << "missedHits " << missedHits << std::endl;
      //std::cout << "lastHits.size() " << lastHits.size() << std::endl;
      //std::cout << "missedHits/lastHits.size() " << (Double_t)missedHits/((Double_t)lastHits.size()-1) << std::endl;
      if((Double_t)missedHits/((Double_t)lastHits.size()-1) > fMissedHitsToLineSize)
	continue;
      
      
      
      
      clusterHits.clear();    
      Double_t totalQ = 0.;
      if(lastHits.size() < 5) continue;
      
      for(size_t i = 0; i < lastHits.size(); ++i) {
	clusterHits.push_back(hitTemp.at(lastHits.at(i)));

	totalQ += clusIn.at(hitTemp.at(lastHits.at(i))).Charge();

	skip.at(hitTemp.at(lastHits.at(i)))=1;
      } 
      //protection against very steep uncorrelated hits
      if(abs(slope)>fMaxSlope 
	 && abs( clusIn.at((*clusterHits.begin())).Channel()-
		 clusIn.at((*clusterHits.rbegin())).Channel())>=0
	 )
	continue;
      
      
      
      // UInt_t sw = (*clusterHits.begin())->WireID().Wire;
      // UInt_t ew = (*(clusterHits.end()-1))->WireID().Wire;
      
      /*  recob::Cluster cluster(sw, 0.,
	  (*clusterHits.begin())->PeakTime(), 0.,
	  ew, 0., 
	  (clusterHits[clusterHits.size()-1])->PeakTime(), 0.,
	  slope, 0., 
	  -999., 0., 
	  totalQ,
	  geom->View((*clusterHits.begin())->Channel()),
	  clusterID);	*/      
      
      //   ++clusterID;
      //  ccol.push_back(cluster);
      clusHitsOut.push_back(clusterHits);
      slopevec.push_back(slope);
      totalQvec.push_back(totalQ);
      //Turn off hit sharing. T. Yang 9/14/12
      //	      //allow Double_t assignment of first and last hits
      //	      for(size_t i = 0; i < lastHits.size(); ++i){ 
      //		if(skip[hitTemp[lastHits[i]]] ==1){
      //		  channel = hit[hitTemp[lastHits[i]]]->Wire()->RawDigit()->Channel();  
      //		  if( channel == sc || channel == ec) skip[i] = 0;
      //		}
      //	      }
      
    }// end if !std::isnan
    
    nLinesFound++;
    
    if(nLinesFound>(UInt_t)fMaxLines)
      break;
    
    
  }// end loop over hits
  
  // saves a bitmap image of the accumulator (useful for debugging), 
  // with scaling based on the maximum cell value
  if(fSaveAccumulator){   
    UChar_t *outPix = new UChar_t [accDx*accDy];
    //finds the maximum cell in the accumulator for image scaling
    Int_t cell, pix = 0, maxCell = 0;
    for (y = 0; y < accDy; ++y){ 
      for (x = 0; x < accDx; ++x){
	cell = c.GetCell(y,x);
	if (cell > maxCell) maxCell = cell;
      }
    }
    for (y = 0; y < accDy; ++y){
      for (x = 0; x < accDx; ++x){ 
	//scales the pixel weights based on the maximum cell value     
	if(maxCell > 0)
	  pix = (Int_t)((1500*c.GetCell(y,x))/maxCell);
	outPix[y*accDx + x] = pix;
      }
    }
    
    HLSSaveBMPFile("houghaccum.bmp", outPix, accDx, accDy);
    delete [] outPix;
  }// end if saving accumulator
  
  lastHits.clear();
  // 	  if(clusterIter != clusIn.end()){
  // 	    clusterIter++;
  // 	    ++cinctr;
  // 	  }
  listofxmax.clear();
  listofymax.clear();
  //}//end loop over clusters
  
  //       }//end loop over planes
  //     }// end loop over tpcs
  //   }// end loop over cryostats
  
  return clusHitsOut.size();
  
}





//------------------------------------------------------------------------------
size_t larreco::HoughBaseAlg::Transform(std::vector<larlight::hit> const& hits,
					Double_t                          & slope,
					Double_t                          & intercept)
{
  HoughTransform c;

  const larutil::Geometry* geom = larutil::Geometry::GetME();
  const larutil::DetectorProperties* detp = larutil::DetectorProperties::GetME();
  Int_t dx = geom->Nwires(0);               //number of wires 
  //Int_t dy = hits[0]->Wire()->NSignal();//number of time samples. 
  Int_t dy = detp->ReadOutWindowSize(); 
  c.Init(dx,dy,fRhoResolutionFactor,fNumAngleCells);

  for(UInt_t i=0;i < hits.size(); ++i){
    c.AddPointReturnMax(hits[i].Wire(), (Int_t)(hits[i].PeakTime()));
  }// end loop over hits

  //gets the actual two-dimensional size of the accumulator
  Int_t accDx = 0;
  Int_t accDy = 0;
  c.GetAccumSize(accDy, accDx);

  //find the weightiest cell in the accumulator.
  Int_t xMax = 0;
  Int_t yMax = 0;
  c.GetMax(yMax,xMax);

  //find the center of mass of the 3x3 cell system (with maxCell at the center). 
  Float_t centerofmassx = 0.;
  Float_t centerofmassy = 0.;
  Float_t denom         = 0.;
    
  if(xMax > 0 && yMax > 0 && xMax+1 < accDx && yMax+1 < accDy){  
    for(Int_t i = -1; i < 2; ++i){
      for(Int_t j = -1; j < 2; ++j){
	denom         += c.GetCell(yMax+i,xMax+j);
	centerofmassx += j*c.GetCell(yMax+i,xMax+j);
	centerofmassy += i*c.GetCell(yMax+i,xMax+j);
      }
    }
    centerofmassx /= denom;
    centerofmassy /= denom;      
  }
  else  centerofmassx = centerofmassy = 0;

  Float_t rho   = 0.;
  Float_t theta = 0.;
  c.GetEquation(yMax+centerofmassy, xMax+centerofmassx, rho, theta);
  slope     = -1./tan(theta);    
  intercept = rho/sin(theta);
  
  ///\todo could eventually refine this method to throw out hits that are 
  ///\todo far from the hough line and refine the fit

  return hits.size();
}







