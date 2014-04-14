#ifndef CLUSTERPARAMSALGNEW_HH
#define CLUSTERPARAMSALGNEW_HH

#include "ClusterParamsAlgNew.hh"

//-----Math-------
#include <math.h>       
#define PI 3.14159265


namespace cluster{

  ClusterParamsAlgNew::ClusterParamsAlgNew() : fPrincipal(2,"D")
  {
    fGSer=nullptr;
    Initialize();
  }

  ClusterParamsAlgNew::ClusterParamsAlgNew(const std::vector<const larlight::hit*> &inhitlist)
    : fPrincipal(2,"D")
  {
    fGSer=nullptr;
    SetHits(inhitlist);
  }

  ClusterParamsAlgNew::ClusterParamsAlgNew(const std::vector<larutil::PxHit> &inhitlist)
    : fPrincipal(2,"D")
  {
    fGSer=nullptr;
    SetHits(inhitlist);
  }

  int ClusterParamsAlgNew::SetHits(const std::vector<larutil::PxHit> &inhitlist){

    Initialize();

    // Make default values
    // Is done by the struct
    if(!(inhitlist.size())) {
      throw RecoUtilException("Provided empty hit list!");
      return -1;
    }

    fHitVector.reserve(inhitlist.size());
    for(auto h : inhitlist)

      fHitVector.push_back(h);
    
    fPlane=fHitVector[0].plane;
    
      
    if (fHitVector.size()<10)
    {
     std::cout << " the hitlist is too small. Continuing to run may result in crash!!! " <<std::endl;
     return -1;
    }
    else
      return fHitVector.size();
    
  }

  int ClusterParamsAlgNew::SetHits(const std::vector<const larlight::hit*> &inhitlist){

    // Make default values
    // Is done by the struct
    if(!(inhitlist.size())) {
      throw RecoUtilException("Provided empty hit list!");
      return -1;
    }
    
    Initialize();

    UChar_t plane = larutil::Geometry::GetME()->ChannelToPlane((*inhitlist.begin())->Channel());

    fHitVector.reserve(inhitlist.size());
    for(auto h : inhitlist) {
      fHitVector.push_back(larutil::PxHit());

      (*fHitVector.rbegin()).t = h->PeakTime() * fGSer->TimeToCm();
      (*fHitVector.rbegin()).w = h->Wire() * fGSer->WireToCm();
      (*fHitVector.rbegin()).charge = h->Charge();
      (*fHitVector.rbegin()).plane = plane;
    }
    fPlane=fHitVector[0].plane;
    
        
    if (fHitVector.size()<10)
    {
     std::cout << " the hitlist is too small. Continuing to run may result in crash!!! " << std::endl;
     return -1;
    }
    else
      return fHitVector.size();
    
  }

  void  ClusterParamsAlgNew::GetFANNVector(std::vector<float> & data){
    unsigned int length = 10;
    if (data.size() != length) 
      data.resize(length);
    data[0]  = fParams.opening_angle / PI;
    data[1]  = fParams.opening_angle_charge_wgt / PI;
    data[2]  = fParams.closing_angle / PI;
    data[3]  = fParams.closing_angle_charge_wgt / PI;
    data[4]  = fParams.eigenvalue_principal;
    data[5]  = fParams.eigenvalue_secondary;
    data[6]  = fParams.width / fParams.length;
    data[7]  = fParams.hit_density_1D / fParams.modified_hit_density;
    data[8]  = fParams.multi_hit_wires/fParams.N_Wires;
    data[9]  = fParams.N_Hits_HC / fParams.N_Hits;
    // data[10] = 0.0;
    // data[11] = 0.0;
    // data[12] = 0.0;
    return;
  }

  // std::vector<float> & ClusterParamsAlgNew::GetFANNVector(){
  //   std::vector<float> result;
  //   GetFANNVector(result);
  //   return result;
  // }

  void  ClusterParamsAlgNew::PrintFANNVector(){
    std::vector<float> data;
    GetFANNVector(data);
    std::cout << "Printing FANN input vector:\n"
              << "   Opening Angle (normalized)  ... : " << data[0] << "\n"
              << "   Opening Angle charge weight  .. : " << data[1] << "\n"
              << "   Closing Angle (normalized)  ... : " << data[2] << "\n"
              << "   Closing Angle charge weight  .. : " << data[3] << "\n"
              << "   Principal Eigenvalue  ......... : " << data[4] << "\n"
              << "   Secondary Eigenvalue  ......... : " << data[5] << "\n"
              << "   Width / Length  ............... : " << data[6] << "\n"
              << "   Hit Density / M.H.D.  ......... : " << data[7] << "\n"
              << "   Percent MultiHit Wires  ....... : " << data[8] << "\n"
              << "   Percent High Charge Hits  ..... : " << data[9] << "\n";
              // << "   empty  ........................ : " << data[10] << "\n"
              // << "   empty  ........................ : " << data[11] << "\n"
              // << "   empty  ........................ : " << data[12] << "\n";
    return;
  }


  void ClusterParamsAlgNew::SetArgoneutGeometry(){
    larutil::LArUtilManager::Reconfigure(larlight::GEO::kArgoNeuT);
  }




  void ClusterParamsAlgNew::Initialize()
  {

    // Set pointer attributes
    fPrincipal.Clear();
    if(!fGSer) fGSer = (larutil::GeometryUtilities*)(larutil::GeometryUtilities::GetME());

    //--- Initilize attributes values ---//
    fFinishedGetAverages       = false;
    fFinishedGetRoughAxis      = false;
    fFinishedGetProfileInfo    = false;
    fFinishedRefineStartPoints = false;
    fFinishedRefineDirection   = false;
    fFinishedGetFinalSlope     = false;
    fFinishedRefineStartPointAndDirection = false;
    fFinishedTrackShowerSep    = false;

    fRough2DSlope=-999.999;    // slope 
    fRough2DIntercept=-999.999;    // slope 
       
    fRoughBeginPoint.w=-999.999;
    fRoughEndPoint.w=-999.999;
     
    fRoughBeginPoint.t=-999.999;
    fRoughEndPoint.t=-999.999;

    fProfileIntegralForward=999.999;
    fProfileIntegralBackward=999.999;
    fProfileMaximumBin=-999;
    
    fChargeCutoffThreshold.clear();
    fChargeCutoffThreshold.resize(3,0);
    fChargeCutoffThreshold.at(0)=500;
    fChargeCutoffThreshold.at(1)=500;
    fChargeCutoffThreshold.at(2)=1000;

    fHitVector.clear();

    fParams.Clear();
    
    // Initialize the neural network:
    fannModule.LoadFromFile(fNeuralNetPath);

  }

  void ClusterParamsAlgNew::Report(){
    std::cout << "ClusterParamsAlgNew Report: "  << "\n"
              << "\tFinishedGetAverages "        << fFinishedGetAverages << "\n"
              << "\tFinishedGetRoughAxis "       << fFinishedGetRoughAxis << "\n"
              << "\tFinishedGetProfileInfo "     << fFinishedGetProfileInfo << "\n"
              << "\tFinishedRefineStartPoints "  << fFinishedRefineStartPoints << "\n"
              << "\tFinishedRefineDirection "    << fFinishedRefineDirection << "\n"
              << "\tFinishedGetFinalSlope "      << fFinishedGetFinalSlope << "\n"
              << "--------------------------------------" << "\n";
    fParams.Report();
  }

  void ClusterParamsAlgNew::FillParams(bool override_DoGetAverages      ,  
                                       bool override_DoGetRoughAxis     ,  
                                       bool override_DoGetProfileInfo   ,  
                                       // bool override_DoRefineDirection  ,
                                       // bool override_DoRefineStartPoints,
                                       bool override_DoStartPointsAndDirection,
                                       bool override_DoGetFinalSlope    ,
                                       bool override_DoTrackShowerSep   ){
    GetAverages      (override_DoGetAverages      );
    GetRoughAxis     (override_DoGetRoughAxis     );
    GetProfileInfo   (override_DoGetProfileInfo   );
    RefineStartPointAndDirection(override_DoStartPointsAndDirection);
    // RefineDirection  (override_DoRefineDirection  );
    // RefineStartPoints(override_DoRefineStartPoints);
    GetFinalSlope    (override_DoGetFinalSlope    );
    TrackShowerSeparation(override_DoTrackShowerSep);
  }

  void ClusterParamsAlgNew::GetAverages(bool override){
    if(!override) { //Override being set, we skip all this logic.
      //OK, no override. Stop if we're already finshed.
      if (fFinishedGetAverages) return;
    }

    fPrincipal.Clear();

    fParams.N_Hits = fHitVector.size();

    std::map<double, int> wireMap;

    fParams.sum_charge = 0.;
    int uniquewires = 0;
    int multi_hit_wires = 0;
    for(auto& hit : fHitVector){
      // std::cout << "This hit has charge " <<  hit.charge << "\n";

      double data[2];
      data[0] = hit.w;
      data[1] = hit.t;
      fPrincipal.AddRow(data);
      fParams.charge_wgt_x += hit.w * hit.charge;
      fParams.charge_wgt_y += hit.t * hit.charge;
      fParams.sum_charge += hit.charge;


      if (wireMap[hit.w] == 0) {
        uniquewires ++;
      }
      if (wireMap[hit.w] == 1) {
        multi_hit_wires ++;
      }
      wireMap[hit.w] ++;


    }


    fParams.N_Wires = uniquewires;
    fParams.multi_hit_wires = multi_hit_wires;

    fParams.charge_wgt_x /= fParams.sum_charge;
    fParams.charge_wgt_y /= fParams.sum_charge;

    if(fPrincipal.GetMeanValues()->GetNrows()<2) {
      throw cluster::RecoUtilException();
      return;
    }

    fParams.mean_x = (* fPrincipal.GetMeanValues())[0];
    fParams.mean_y = (* fPrincipal.GetMeanValues())[1];
    fParams.mean_charge = fParams.sum_charge / fParams.N_Hits;

    fPrincipal.MakePrincipals();

    fParams.eigenvalue_principal = (* fPrincipal.GetEigenValues() )[0];
    fParams.eigenvalue_secondary = (* fPrincipal.GetEigenValues() )[1];

    fFinishedGetAverages = true;
    // Report();
  }

  // Also does the high hitlist
  void ClusterParamsAlgNew::GetRoughAxis(bool override){
    if(!override) { //Override being set, we skip all this logic.
      //OK, no override. Stop if we're already finshed.
      if (fFinishedGetRoughAxis) return;
      //Try to run the previous function if not yet done.
      if (!fFinishedGetAverages) GetAverages(true);
    } else {
      //Try to run the previous function if not yet done.
      if (!fFinishedGetAverages) GetAverages(true);
    }

    double rmsx = 0.0;
    double rmsy = 0.0;

    //using the charge weighted coordinates find the axis from slope
    double ncw=0;
    double sumtime=0;//from sum averages
    double sumwire=0;//from sum averages
    double sumwiretime=0;//sum over (wire*time)
    double sumwirewire=0;//sum over (wire*wire)
    //next loop over all hits again

    for (auto& hit : fHitVector){
      // First, abuse this loop to calculate rms in x and y
      rmsx += pow(fParams.mean_x - hit.w, 2)/fParams.N_Hits;
      rmsy += pow(fParams.mean_y - hit.t, 2)/fParams.N_Hits;

      //if charge is above avg_charge
      // std::cout << "This hit has charge " <<  hit . charge << "\n";
       
      if(hit.charge > fParams.mean_charge){
        ncw+=1;
        sumwire+=hit.w;
        sumtime+=hit.t;
        sumwiretime+=hit.w * hit.t;
        sumwirewire+=pow(hit.w,2);  
      }//for high charge
    }//For hh loop

    fParams.rms_x = sqrt(rmsx);
    fParams.rms_y = sqrt(rmsy);
 
    fParams.N_Hits_HC = ncw;
    //Looking for the slope and intercept of the line above avg_charge hits

   // std::cout << " ncw, and parentheses " << ncw << " " << (ncw*sumwiretime- sumwire*sumtime) << " " <<(ncw*sumwirewire-sumwire*sumwire) << std::endl;
    if((ncw*sumwirewire-sumwire*sumwire) > 0.00001)
      fRough2DSlope= (ncw*sumwiretime- sumwire*sumtime)/(ncw*sumwirewire-sumwire*sumwire);//slope for cw
    else
      fRough2DSlope=999;
    fRough2DIntercept= fParams.charge_wgt_y  -fRough2DSlope*(fParams.charge_wgt_x);//intercept for cw
    //Getthe 2D_angle
    fParams.cluster_angle_2d = atan(fRough2DSlope)*180/PI;

 
    fFinishedGetRoughAxis = true;    
    // Report();
    return;
  }


  void ClusterParamsAlgNew::GetProfileInfo(bool override)  {
    if(!override) { //Override being set, we skip all this logic.
      //OK, no override. Stop if we're already finshed.
      if (fFinishedGetProfileInfo) return;
      //Try to run the previous function if not yet done.
      if (!fFinishedGetRoughAxis) GetRoughAxis(true);
    } else {
      if (!fFinishedGetRoughAxis) GetRoughAxis(true);
    }
    bool drawOrtHistos = false;

    //these variables need to be initialized to other values? 
    if(fRough2DSlope==-999.999 || fRough2DIntercept==-999.999 ) 
      GetRoughAxis(true);      
    
    fCoarseNbins=2;
    fProfileNbins=100;
    
    fChargeProfile.clear();
    fCoarseChargeProfile.clear();
    fChargeProfile.resize(fProfileNbins,0);
    fCoarseChargeProfile.resize(fCoarseNbins,0);
    
    //get slope of lines orthogonal to those found crossing the shower.
    double inv_2d_slope=0;
    if(fabs(fRough2DSlope)>0.001)
      inv_2d_slope=-1./fRough2DSlope;
    else
      inv_2d_slope=-999999.;
    // std::cout << " N_Hits is " << fParams.N_Hits << std::endl;
    // std::cout << " inv_2d_slope is " << inv_2d_slope << std::endl;
    // std::cout << " fRough2DSlope is " << fRough2DSlope << std::endl;
    // std::cout << " conversions " << fWire2Cm.at(fPlane) 
    //           << " " << fTime2Cm << std::endl;
    
    double InterHigh=-999999;
    double InterLow=999999;
    double WireHigh=-999999;
    double WireLow=999999;
    fInterHigh_side=-999999;
    fInterLow_side=999999;
    double min_ortdist(999), max_ortdist(-999); // needed to calculate width
    //loop over all hits. Create coarse and fine profiles
    // of the charge weight to help refine the start/end 
    // points and have a first guess of direction

    //std::cout << " inv_2d_slope " << inv_2d_slope << std::endl;
    
    for(auto& hit : fHitVector)
    {
      
      //calculate intercepts along   
      double intercept=hit.t - inv_2d_slope * (double)(hit.w);
      double side_intercept=hit.t - fRough2DSlope * (double)(hit.w);
      
      larutil::PxPoint OnlinePoint;
      // get coordinates of point on axis.
      fGSer->GetPointOnLine(fRough2DSlope,fRough2DIntercept,&hit,OnlinePoint);

   //   std::cout << "normal point " << hit.w << ","<<hit.t << " online point " <<  OnlinePoint.w << "," <<  OnlinePoint.t << std::endl; 
      
      double ortdist=fGSer->Get2DDistance(&OnlinePoint,&hit);
      if (ortdist < min_ortdist) min_ortdist = ortdist;
      if (ortdist > max_ortdist) max_ortdist = ortdist;
      
      if(inv_2d_slope!=-999999)   //almost all cases
      {	
	if(intercept > InterHigh ){
	  InterHigh=intercept;
	  }
        
	if(intercept < InterLow ){
	  InterLow=intercept;
	  }  
      }
      else    //slope is practically horizontal. Care only about wires.
	{
	  if(hit.w > WireHigh ){
	    WireHigh=hit.w;
	    }
        
	  if(hit.w < WireLow ){
	    WireLow=hit.w;
	    }
	}
    
      if(side_intercept > fInterHigh_side ){
        fInterHigh_side=side_intercept;
      }
        
      if(side_intercept < fInterLow_side ){
        fInterLow_side=side_intercept;
      }
    

    }   
    // end of first HitIter loop, at this point we should
    // have the extreme intercepts 
  
    /////////////////////////////////////////////
    // Second loop. Fill profiles. 
    /////////////////////////////////////////////
    
    // get projected points at the beginning and end of the axis.
    
    larutil::PxPoint HighOnlinePoint, LowOnlinePoint,BeginOnlinePoint, EndOnlinePoint;
    
  if(inv_2d_slope!=-999999)   //almost all cases
     {	
      fGSer->GetPointOnLineWSlopes(fRough2DSlope,fRough2DIntercept,
                                 InterHigh,HighOnlinePoint);
      fGSer->GetPointOnLineWSlopes(fRough2DSlope,fRough2DIntercept,
                                 InterLow,LowOnlinePoint);
     }
  else   //need better treatment of horizontal events.
    {
      larutil::PxPoint ptemphigh(fPlane,WireHigh,(fInterHigh_side+fInterLow_side)/2);
      larutil::PxPoint ptemplow(fPlane,WireLow,(fInterHigh_side+fInterLow_side)/2);
      fGSer->GetPointOnLine(fRough2DSlope,fRough2DIntercept,&ptemphigh,HighOnlinePoint);
      fGSer->GetPointOnLine(fRough2DSlope,fRough2DIntercept,&ptemplow,LowOnlinePoint);
    }
    
//     std::cout << " extreme intercepts: low: " << InterLow 
//               << " " << InterHigh << std::endl;
//     std::cout << " extreme intercepts: side: " << fInterLow_side 
//               << " " << fInterHigh_side << std::endl;
//     std::cout << "axis + intercept "  << fRough2DSlope << " " 
//               << fRough2DIntercept << std::endl;
//     
    std::cout << " Low online point: " << LowOnlinePoint.w << ", " << LowOnlinePoint.t 
              << " High: " << HighOnlinePoint.w << ", " << HighOnlinePoint.t << std::endl; 

    //std::cout << " max_min ortdist" << max_ortdist << " " << min_ortdist << std::endl;	      
    //std::cout << " hit list size " << fHitVector.size() << std::endl;
    //define BeginOnlinePoint as the one with lower wire number (for now), adjust intercepts accordingly	      
    if(HighOnlinePoint.w >= LowOnlinePoint.w)
    {
      BeginOnlinePoint=LowOnlinePoint;
      fBeginIntercept=InterLow;
      EndOnlinePoint=HighOnlinePoint;
      fEndIntercept=InterHigh;
    }
    else
    {
      BeginOnlinePoint=HighOnlinePoint;
      fBeginIntercept=InterHigh;
      EndOnlinePoint=LowOnlinePoint;
      fEndIntercept=InterLow;        
    }
   
    fProjectedLength=fGSer->Get2DDistance(&BeginOnlinePoint,&EndOnlinePoint);
     
//     std::cout << " projected length " << fProjectedLength 
//                << " Begin Point " << BeginOnlinePoint.w << " " 
//                << BeginOnlinePoint.t  << " End Point " << EndOnlinePoint.w << ","<< EndOnlinePoint.t << std::endl;
    

    // Some fitting variables to make a histogram:
   
    std::vector<double> ort_profile;
    const int NBINS=100;
    ort_profile.resize(NBINS);
    
    std::vector<double> ort_dist_vect;
    ort_dist_vect.reserve(fParams.N_Hits);

    double current_maximum=0; 
    for(auto& hit : fHitVector)
    {
     
      larutil::PxPoint OnlinePoint;
      // get coordinates of point on axis.
     // std::cout << BeginOnlinePoint << std::endl;
      //std::cout << &OnlinePoint << std::endl;
      fGSer->GetPointOnLine(fRough2DSlope,&BeginOnlinePoint,&hit,OnlinePoint);
      double ortdist=fGSer->Get2DDistance(&OnlinePoint,&hit);
      
      double linedist=fGSer->Get2DDistance(&OnlinePoint,&BeginOnlinePoint);
    //  double ortdist=fGSer->Get2DDistance(&OnlinePoint,&hit);
      ort_dist_vect.push_back(ortdist);
      int ortbin;
      if (ortdist == 0)
        ortbin = 0;
      else 
        ortbin =(int)(ortdist-min_ortdist)/(max_ortdist-min_ortdist)*(NBINS-1);
           
      ort_profile.at(ortbin)+=hit.charge;
      //if (ortdist < min_ortdist) min_ortdist = ortdist;
      //if (ortdist > max_ortdist) max_ortdist = ortdist;

      ////////////////////////////////////////////////////////////////////// 
      //calculate the weight along the axis, this formula is based on rough guessology. 
      // there is no physics motivation behind the particular numbers, A.S.
      // \todo: switch to something that is motivated and easier to 
      //        spell than guessology.  C.A.
      /////////////////////////////////////////////////////////////////////// 
      double weight= (ortdist<1.) ? 10 * (hit.charge) : 5 * (hit.charge) / ortdist;
    
      int fine_bin  =(int)(linedist/fProjectedLength*(fProfileNbins-1));
      int coarse_bin=(int)(linedist/fProjectedLength*(fCoarseNbins-1));
      /*
      std::cout << "linedist: " << linedist << std::endl;
      std::cout << "fProjectedLength: " << fProjectedLength << std::endl;
      std::cout << "fProfileNbins: " << fProfileNbins << std::endl;
      std::cout << "fine_bin: " << fine_bin << std::endl;
      std::cout << "coarse_bin: " << coarse_bin << std::endl;
      */

//      std::cout << "length" << linedist <<   " fine_bin, coarse " << fine_bin << " " << coarse_bin << std::endl;
      
      if(fine_bin<fProfileNbins)  //only fill if bin number is in range
      {
        fChargeProfile.at(fine_bin)+=weight;
        //find maximum bin on the fly:
        if(fChargeProfile.at(fine_bin)>current_maximum 
          && fine_bin!=0 && fine_bin!=fProfileNbins-1) 
        {
          current_maximum=fChargeProfile.at(fine_bin);
          fProfileMaximumBin=fine_bin; 
        }
      }
      
      if(coarse_bin<fCoarseNbins) //only fill if bin number is in range
        fCoarseChargeProfile.at(coarse_bin)+=weight;
      
    }  // end second loop on hits. Now should have filled profile vectors.

    double integral=0; 
    int ix=0;
    // int fbin=0;
    for(ix=0;ix<NBINS;ix++)
    {
      integral+=ort_profile.at(ix);
      if(integral>=0.95*fParams.sum_charge)
      {
        break;
      }
    }

    fParams.width=2*(double)ix/(double)(NBINS-1)*(max_ortdist-min_ortdist);  // multiply by two because ortdist is folding in both sides. 
    
    
    if (drawOrtHistos){
      TH1F * ortDistHist = 
	new TH1F("ortDistHist", 
                         "Orthogonal Distance to axis;Distance (cm)",
                         100, min_ortdist, max_ortdist);
      TH1F * ortDistHistCharge = 
                new TH1F("ortDistHistCharge", 
                         "Orthogonal Distance to axis (Charge Weighted);Distance (cm)", 
                         100, min_ortdist, max_ortdist);
      TH1F * ortDistHistAndrzej= 
                new TH1F("ortDistHistAndrzej",
                         "Orthogonal Distance Andrzej weighting",
                         100, min_ortdist, max_ortdist);

      for (int index = 0; index < fParams.N_Hits; index++){
        ortDistHist -> Fill(ort_dist_vect.at(index));
        ortDistHistCharge -> Fill(ort_dist_vect.at(index), fHitVector.at(index).charge);
        double weight= (ort_dist_vect.at(index)<1.) ? 
	  10 * (fHitVector.at(index).charge) : 
	  (5 * (fHitVector.at(index).charge)/ort_dist_vect.at(index));
        ortDistHistAndrzej -> Fill(ort_dist_vect.at(index), weight);
      }
      ortDistHist -> Scale(1.0/ortDistHist->Integral());
      ortDistHistCharge -> Scale(1.0/ortDistHistCharge->Integral());
      ortDistHistAndrzej -> Scale(1.0/ortDistHistAndrzej->Integral());

      TCanvas * ortCanv = new TCanvas("ortCanv","ortCanv", 600, 600);
      ortCanv -> cd();
      ortDistHistAndrzej -> SetLineColor(3);
      ortDistHistAndrzej -> Draw("");
      ortDistHistCharge -> Draw("same");
      ortDistHist -> SetLineColor(2);
      ortDistHist -> Draw("same");

      TLegend * leg = new TLegend(.4,.6,.8,.9);
      leg -> SetHeader("Charge distribution");
      leg -> AddEntry(ortDistHist, "Unweighted Hits");
      leg -> AddEntry(ortDistHistCharge, "Charge Weighted Hits");
      leg -> AddEntry(ortDistHistAndrzej, "Charge Weighted by Guessology");
      leg -> Draw();

      ortCanv -> Update();
    }

    fProfileIntegralForward=0;
    fProfileIntegralBackward=0;
    
    //calculate the forward and backward integrals counting int the maximum bin.
    
    
    for(int ibin=0;ibin<fProfileNbins;ibin++)
    {
      if(ibin<=fProfileMaximumBin)  
        fProfileIntegralForward+=fChargeProfile.at(ibin);
      
      if(ibin>=fProfileMaximumBin)  
        fProfileIntegralBackward+=fChargeProfile.at(ibin);
    }
    
    // now, we have the forward and backward integral so start
    // stepping forward and backward to find the trunk of the 
    // shower. This is done making sure that the running 
    // integral until less than 1% is left and the bin is above 
    // a set theshold many empty bins.
    
   
    
     //forward loop
    double running_integral=fProfileIntegralForward;
    int startbin,endbin;
    for(startbin=fProfileMaximumBin;startbin>1;startbin--)
    {
      running_integral-=fChargeProfile.at(startbin);
      if( fChargeProfile.at(startbin)<fChargeCutoffThreshold.at(fPlane) && running_integral/fProfileIntegralForward<0.01 )
        break;
    }
    
    //backward loop
    running_integral=fProfileIntegralBackward;
    for(endbin=fProfileMaximumBin;endbin<fProfileNbins-1;endbin++)
    {
      running_integral-=fChargeProfile.at(endbin);
      if( fChargeProfile.at(endbin)<fChargeCutoffThreshold.at(fPlane) && running_integral/fProfileIntegralBackward<0.01 )
        break;
    }
    
    // now have profile start and endpoints. Now translate to wire/time. 
    // Will use wire/time that are on the rough axis.
    // fProjectedLength is the length from fInterLow to interhigh a
    // long the rough_2d_axis on bin distance is: 
    // larutil::PxPoint OnlinePoint;
    
  if(inv_2d_slope!=-999999)   //almost all cases
     {	   
    double ort_intercept_begin=fBeginIntercept+(fEndIntercept-fBeginIntercept)/fProfileNbins*startbin;
    //std::cout << " ort_intercept_begin: " << ort_intercept_begin << std::endl;
    fGSer->GetPointOnLineWSlopes(fRough2DSlope,
                                 fRough2DIntercept,
                                 ort_intercept_begin,
                                 fRoughBeginPoint);
    
    double ort_intercept_end=fBeginIntercept+(fEndIntercept-fBeginIntercept)/fProfileNbins*endbin;
    fRoughBeginPoint.plane=fPlane;
    
    //std::cout << " ort_intercept_end: " << ort_intercept_end << std::endl;
    fGSer->GetPointOnLineWSlopes(fRough2DSlope,
                                 fRough2DIntercept,
                                 ort_intercept_end,
                                 fRoughEndPoint);
    fRoughEndPoint.plane=fPlane;
     }
   else
   {
    double wire_begin=WireLow+(WireHigh-WireLow)/fProfileNbins*startbin;
    //std::cout << " wire_begin: " << wire_begin << std::endl;
   
    larutil::PxPoint ptemphigh(fPlane,wire_begin,(fInterHigh_side+fInterLow_side)/2);
    fGSer->GetPointOnLine(fRough2DSlope,fRough2DIntercept,&ptemphigh,fRoughBeginPoint);
    fRoughBeginPoint.plane=fPlane;
    
    double wire_end=WireLow+(WireHigh-WireLow)/fProfileNbins*endbin;
    //std::cout << " wire_end: " << wire_end << std::endl;
      
    larutil::PxPoint ptemplow(fPlane,wire_end,(fInterHigh_side+fInterLow_side)/2);
    fGSer->GetPointOnLine(fRough2DSlope,fRough2DIntercept,&ptemplow,fRoughEndPoint);
    fRoughEndPoint.plane=fPlane;
     
    }
     
    std::cout << "  rough start points "  << fRoughBeginPoint.w << ", " << fRoughBeginPoint.t << " end: " <<  fRoughEndPoint.w << " " << fRoughEndPoint.t << std::endl;
    
     // ok. now have fRoughBeginPoint and fRoughEndPoint. No decision about direction has been made yet.
    fParams.start_point = fRoughBeginPoint;
    fParams.end_point   = fRoughEndPoint;
    // fRoughEndPoint
    // fRoughEndPoint
    // and use them to get the axis


    fFinishedGetProfileInfo = true;
    // Report();
    
    return;
  }
  
  
  /**
   * Calculates the following variables:
   * length
   * width
   * hit_density_1D
   * hit_density_2D
   * direction
   * @param override [description]
   */
  void ClusterParamsAlgNew::RefineStartPoints(bool override) {
    // if(!override) { //Override being set, we skip all this logic.
    //   //OK, no override. Stop if we're already finshed.
    //   if (fFinishedRefineStartPoints) return;
    //   //Try to run the previous function if not yet done.
    //   if (!fFinishedRefineDirection) RefineDirection(true);
    // } else {
    //   if (!fFinishedRefineDirection) RefineDirection(true);
    // }
    
    
     // need to define physical direction with openind angles and pass that to Ryan's line finder.
    
    
    // Direction decision has been moved entirely to Refine Direction()
    // opening and closing angles are already set
    // they are associated with the start and endpoints.
    // If refine direction opted to switch the shower direction, 
    // it also switched opening and closing angles.
    
    /////////////////////////////////
    // fParams.direction=  ...
    
    // Direction is decided by RefineDirection()
    larutil::PxPoint startHit,endHit;
    startHit=fRoughBeginPoint;
    endHit=fRoughEndPoint;

    
    ///////////////////////////////
    //Ryan's Shower Strip finder work here. 
    //First we need to define the strip width that we want
    double d=0.6;//this is the width of the strip.... this needs to be tuned to something.
    //===============================================================================================================       
    // Will need to feed in the set of hits that we want. 
    //	const std::vector<larutil::PxHit*> whole;
    std::vector <const larutil::PxHit*> subhit;
    double linearlimit=8;
    double ortlimit=12;
    double lineslopetest(fRough2DSlope);
    larutil::PxHit averageHit;
    //also are we sure this is actually doing what it is supposed to???
    //are we sure this works?
    //is anybody even listening?  Were they eaten by a grue?    
    fGSer->SelectLocalHitlist(fHitVector,subhit, startHit,
                              linearlimit,ortlimit,lineslopetest,
                              averageHit);

    
    if(!(subhit.size()) || subhit.size()<3) {
      std::cout<<"Subhit list is empty or too small. Using rough start/end points..."<<std::endl;
      // GetOpeningAngle();
      fParams.start_point = fRoughBeginPoint;
      fParams.end_point   = fRoughEndPoint;
      // fRoughEndPoint
      // fRoughEndPoint
      // and use them to get the axis
      
      fFinishedRefineStartPoints = true;
      return;
    }
    
    double avgwire= averageHit.w;
    double avgtime= averageHit.t;
    //vertex in tilda-space pair(x-til,y-til)
    std::vector<std::pair<double,double>> vertil;
    // vertil.clear();// This isn't needed?
    vertil.reserve(subhit.size() * subhit.size());
    //vector of the sum of the distance of a vector to every vertex in tilda-space
    std::vector<double> vs;
    // vs.clear();// this isn't needed?
    // $$This needs to be corrected//this is the good hits that are between strip
    std::vector<const larutil::PxHit*>  ghits;
    ghits.reserve(subhit.size());
    int n=0;
    double fardistcurrent=0;
    larutil::PxHit startpoint;
    double gwiretime=0; 
    double gwire=0; 
    double gtime=0;
    double gwirewire=0;
    //KAZU!!! I NEED this too//this will need to come from somewhere... 
    //This is some hit that is from the way far side of the entire shower cluster...
    larutil::PxPoint farhit= fRoughEndPoint;
    
    //=============building the local vector===================
    //this is clumsy... but just want to get something to work for now. 
    //THIS is REAL Horrible and CLUMSY... We can make things into helper functions soon. 
    std::vector<larutil::PxHit> returnhits;
    std::vector<double> radiusofhit;
    std::vector<int> closehits;
    //unsigned int minhits=0;	
    //double maxreset=0;
    //    double avgwire=0;
    //    double avgtime=0;
    //    if(minhits<fHitVector.size()){
    //      for(auto & hit : fHitVector){
    //	std::pair<double,double> rv(fRoughEndPoint.w,fRoughEndPoint.t);
    //	closehits.clear();
    //	radiusofhit.clear();
    //	returnhits.clear();
    // for( unsigned int a=0; a<hit.size(); a++){
    //	double d= sqrt( pow((rv.first-hit.w),2) + pow((rv.second-hit.t),2)  );
    //      maxreset+=d;
    //        radiusofhit.push_back(d);}
    //      for(unsigned int b=0; b<minhits; b++){
    //	int minss= std::min_element(radiusofhit.begin(),radiusofhit.end())-radiusofhit.begin();
    //	closehits.push_back(minss);
    //	radiusofhit[minss] = maxreset;}
    //now make the vector o just the close hits using the closehit index
    //      for( unsigned int k=0; k < closehits.size(); k++){
    //	//first find the average wire and time for each set of hits... and make that the new origin before the transpose.
    //	avgwire+=fHitVector[closehits[k]].w;
    //	avgtime+=fHitVector[closehits[k]].t;
    //	returnhits.push_back(fHitVector[closehits[k]]);}
    //   }//if fHitVector is big enough	
    
    //    avgwire=avgwire/closehits.size();
    //   avgtime=avgtime/closehits.size();
    //    subhit=returnhits;
    
    //==============================================================================
    //Now we need to do the transformation into "tilda-space"
    for(unsigned int a=0; a<subhit.size();a++){
      for(unsigned int b=a+1;b<subhit.size();b++){
        if(subhit.at(a)->w != subhit.at(b)->w){
          double xtil = ((subhit.at(a)->t - avgtime) - (subhit.at(b)->t - avgtime));
          xtil /= ((subhit.at(a)->w - avgwire)-(subhit.at(b)->w - avgwire));
          double ytil = (subhit.at(a)->w - avgwire)*xtil -(subhit.at(a)->t - avgtime);
          //push back the tilda vertex point on the pair
          std::pair<double,double> tv(xtil,ytil);
          vertil.push_back(tv);
        }//if Wires are not the same
      }//for over b
    }//for over a
    // look at the distance from a tilda-vertex to all other tilda-verticies
    for(unsigned int z=0;z<vertil.size();z++){
      double vd=0;//the distance for vertex... just needs to be something 0
      for(unsigned int c=0; c<vertil.size();c++)

        vd+= sqrt(pow((vertil.at(z).first - vertil.at(c).first),2)
                + pow((vertil.at(z).second-vertil.at(c).second),2));
      vs.push_back(vd);
      vd=0.0;
    }//for z loop
    
    if(vs.size()==0)   //al hits on same wire?!
    {
       std::cout<<"vertil list is empty. all subhits are on the same wire?"<<std::endl;
      // GetOpeningAngle();
      fParams.start_point = fRoughBeginPoint;
      fParams.end_point   = fRoughEndPoint;
      // fRoughEndPoint
      // fRoughEndPoint
      // and use them to get the axis
      
      fFinishedRefineStartPoints = true;
      return;
    }
    //need to find the min of the distance of vertex in tilda-space
    //this will get me the area where things are most linear
    int minvs= std::min_element(vs.begin(),vs.end())-vs.begin();
    // now use the min position to find the vertex in tilda-space
    //now need to look a which hits are between the tilda lines from the points
    //in the tilda space everything in wire time is based on the new origin which is at the average wire/time
    double tilwire=vertil.at(minvs).first;//slope
    double tiltimet=-vertil.at(minvs).second+d*sqrt(1+pow(tilwire,2));//negative cept is accounted for top strip
    double tiltimeb=-vertil.at(minvs).second-d*sqrt(1+pow(tilwire,2));//negative cept is accounted for bottom strip
    // look over the subhit list and ask for which are inside of the strip
    for(unsigned int a=0; a<subhit.size(); a++){
      double dtstrip= (-tilwire * (subhit.at(a)->w - avgwire) 
                     +(subhit.at(a)->t - avgtime)-tiltimet)/sqrt(tilwire*tilwire+1);
      double dbstrip= (-tilwire * (subhit.at(a)->w - avgwire) 
                     +(subhit.at(a)->t - avgtime)-tiltimeb)/sqrt(tilwire*tilwire+1);
      
      if((dtstrip<0.0 && dbstrip>0.0)||(dbstrip<0.0 && dtstrip>0.0)){
        ghits.push_back(subhit.at(a));
      }//if between strip
    }//for a loop
    //=======================Do stuff with the good hits============================
    
    //of these small set of hits just fit a simple line. 
    //then we will need to see which of these hits is farthest away 
    
    for(unsigned int g=0; g<ghits.size();g++){
      // should call the helper funtion to do the fit
      //but for now since there is no helper function I will just write it here......again
      n+=1;
      gwiretime+= ghits.at(g)->w * ghits.at(g)->t;
      gwire+= ghits.at(g)->w;
      gtime+= ghits.at(g)->t;
      gwirewire+= ghits.at(g)->w * ghits.at(g)->w;
      // now work on calculating the distance in wire time space from the far point
      //farhit needs to be a hit that is given to me
      double fardist= sqrt(pow(ghits.at(g)->w - farhit.w,2)+pow(ghits.at(g)->t - farhit.t,2));
      //come back to this... there is a better way to do this probably in the loop
      //there should also be a check that the hit that is farthest away has subsequent hits after it on a few wires
      if(fardist>fardistcurrent){
        fardistcurrent=fardist;
        //if fardist... this is the point to use for the start point
        startpoint.t = ghits.at(g)->t;
        startpoint.w = ghits.at(g)->w;
        startpoint.plane = ghits.at(g)->plane;
        startpoint.charge = ghits.at(g)->charge;
      }
    }//for ghits loop
    //This can be the new start point
    // std::cout<<"Here Kazu"<<std::endl;
    // std::cout<<"Ryan This is the new SP ("<<startpoint.w<<" , "<<startpoint.t<<")"<<std::endl;
    // double gslope=(n* gwiretime- gwire*gtime)/(n*gwirewire -gwire*gwire);
    // double gcept= gtime/n -gslope*(gwire/n);
    
    //should this be here? Id argue this might be done once outside.
    fParams.length=fGSer->Get2DDistance((larutil::PxPoint *)&startpoint,&endHit);
    if(fParams.length)
      fParams.hit_density_1D=fParams.N_Hits/fParams.length;
    else
      fParams.hit_density_1D=0;
    
    if(fParams.length && fParams.width)
      fParams.hit_density_2D=fParams.N_Hits/fParams.length/fParams.width;
    else
      fParams.hit_density_2D=0;
    
    
    fParams.start_point=startpoint;
    
    static int count(0);
    count ++;
    // std::cout << "Completed refine start point " << count << " times!\n";

    fFinishedRefineStartPoints = true;
   // Report();
    return;
  }
  
  
  ///////////////////////////////////////////////////////
  ////
  /////////////////////////////////////////////////////////////
  
  void ClusterParamsAlgNew::GetFinalSlope(bool override) {
    if(!override) { //Override being set, we skip all this logic.
      //OK, no override. Stop if we're already finshed.
      if (fFinishedGetFinalSlope) return;
      //Try to run the previous function if not yet done.
      if (!fFinishedRefineStartPoints) RefineStartPoints(true);
    } else {
      //Try to run the previous function if not yet done.
      if (!fFinishedRefineStartPoints) RefineStartPoints(true);
    }

    /**
     * Calculates the following variables:
     * angle_2d
     * modified_hit_density
     */


    //double xangle=Get2DAngleForHit( wstn,tstn, hitlist);
    const int NBINS=720;
    std::vector< int > fh_omega_single(NBINS,0);   //720,-180., 180.
    
    // fParams.start_point
    
    double current_maximum=0;
    double curr_max_bin=-1;
    
    for( auto hit : fHitVector){
      
      double omx=fGSer->Get2Dangle((larutil::PxPoint *)&hit,&fParams.start_point);  // in rad and assuming cm/cm space.
      int nbin=(omx+TMath::Pi())*(NBINS-1)/(2*TMath::Pi());
      if(nbin >= NBINS) nbin=NBINS-1;
      if(nbin < 0) nbin=0;
      fh_omega_single[nbin]+=hit.charge;
      
      if( fh_omega_single[nbin]>current_maximum )
      {
        current_maximum= fh_omega_single[nbin];
        curr_max_bin=nbin;
      }
	
      
    }
    
  
    fParams.angle_2d=(curr_max_bin/720*(2*TMath::Pi()))-TMath::Pi();
    fParams.angle_2d*=180/PI;
    std::cout << " Final 2D angle: " << fParams.angle_2d << " degrees " << std::endl;
    
    if(cos(fParams.angle_2d))
      fParams.modified_hit_density=fParams.hit_density_1D/cos(fParams.angle_2d);
    else 
      fParams.modified_hit_density=fParams.hit_density_1D;
       
    fFinishedGetFinalSlope = true;
    return;
  }
  
  /**
   * This function calculates the opening/closing angles
   * It also makes a decision on whether or not to flip the direction
   * the cluster.  Then the start point is refined later.
   * @param override [description]
   */
  void ClusterParamsAlgNew::RefineDirection(bool override) {
    // if(!override) { //Override being set, we skip all this logic.
    //   //OK, no override. Stop if we're already finshed.
    //   if (fFinishedRefineDirection) return;
    //   //Try to run the previous function if not yet done.
    //   if (!fFinishedGetProfileInfo) GetProfileInfo(true);
    // } else {
    //   //Try to run the previous function if not yet done.
    //   if (!fFinishedGetProfileInfo) GetProfileInfo(true);
    // }
    
    // double wire_2_cm = fGSer->WireToCm();
    // double time_2_cm = fGSer->TimeToCm();
    
    // Removing the conversion since these points are set above in cm-cm space
    //   -- Corey
      
    larutil::PxPoint this_startPoint, this_endPoint;

    if (fFinishedRefineStartPoints){
      this_startPoint = fParams.start_point;
      this_endPoint   = fParams.end_point;
    }
    else{
      this_startPoint = fRoughBeginPoint;
      this_endPoint   = fRoughEndPoint;
    }

    std::cout << "Angle: Start point: (" << this_startPoint.w 
              << ", " << this_startPoint.t << ")\n";
    std::cout << "Angle: End point  : (" << this_endPoint.w   
              << ", " << this_endPoint.t << ")\n";

    double endStartDiff_x = (this_endPoint.w - this_startPoint.w);
    double endStartDiff_y = (this_endPoint.t - this_startPoint.t);
    double rms_forward   = 0;
    double rms_backward  = 0;
    double mean_forward  = 0;
    double mean_backward = 0;
    double weight_total  = 0;
    double hit_counter_forward  = 0;
    double hit_counter_backward = 0;
    
    if (endStartDiff_y == 0 && endStartDiff_x == 0) {
      std::cerr << "Error:  end point and start point are the same!\n";
      return;
    }

    double percentage = 0.95;
    double percentage_HC = 0.95*fParams.N_Hits_HC/fParams.N_Hits;
    const int NBINS=200;
    const double wgt = 1.0/fParams.N_Hits;

    // Containers for the angle histograms
    std::vector<float> opening_angle_bin(NBINS,0.0 ) ;
    std::vector<float> closing_angle_bin(NBINS,0.0 ) ;
    std::vector<float> opening_angle_highcharge_bin(NBINS,0.0 ) ;
    std::vector<float> closing_angle_highcharge_bin(NBINS,0.0 ) ;
    std::vector<float> opening_angle_chargeWgt_bin(NBINS,0.0 ) ;
    std::vector<float> closing_angle_chargeWgt_bin(NBINS,0.0 ) ;
    //hard coding this for now, should use SetRefineDirectionQMin function
    fQMinRefDir  = 25;

    int index = -1;
    for(auto& hit : fHitVector) {
      index ++;
      //skip this hit if below minimum cutoff param
      if(hit.charge < fQMinRefDir) continue;

      weight_total = hit.charge; 

      // Compute forward mean
      double hitStartDiff_x = (hit.w - this_startPoint.w) ;
      double hitStartDiff_y = (hit.t - this_startPoint.t) ;
      
      if (hitStartDiff_x == 0 && hitStartDiff_y == 0) continue;

      double cosangle_start = (endStartDiff_x*hitStartDiff_x + 
                               endStartDiff_y*hitStartDiff_y);
      cosangle_start /= ( pow(pow(endStartDiff_x,2)+pow(endStartDiff_y,2),0.5)
                        * pow(pow(hitStartDiff_x,2)+pow(hitStartDiff_y,2),0.5));

      if(cosangle_start>0) {
        // Only take into account for hits that are in "front"
        //no weighted average, works better as flat average w/ min charge cut
        mean_forward += cosangle_start;
        rms_forward  += pow(cosangle_start,2);
        hit_counter_forward++;
      }

      // Compute backward mean
      double hitEndDiff_x = (hit.w - this_endPoint.w);
      double hitEndDiff_y = (hit.t - this_endPoint.t);
      if (hitEndDiff_x == 0 && hitEndDiff_y == 0) continue;

      double cosangle_end  = (endStartDiff_x*hitEndDiff_x +
                              endStartDiff_y*hitEndDiff_y) * (-1.);
      cosangle_end /= ( pow(pow(endStartDiff_x,2)+pow(endStartDiff_y,2),0.5) 
                      * pow(pow(hitEndDiff_x,2)+pow(hitEndDiff_y,2),0.5));
      
      if(cosangle_end>0) {
        //no weighted average, works better as flat average w/ min charge cut
        mean_backward += cosangle_end;
        rms_backward  += pow(cosangle_end,2);
        hit_counter_backward++;
      }

      int N_bins_OPEN = NBINS * acos(cosangle_start)/PI;
      int N_bins_CLOSE = NBINS * acos(cosangle_end)/PI;
      // std::cout << "endStartDiff_x :" << endStartDiff_x << std::endl;
      // std::cout << "endStartDiff_y :" << endStartDiff_y << std::endl;
      // std::cout << "cosangle_start :" << cosangle_start << std::endl;
      // std::cout << "cosangle_end   :" << cosangle_end << std::endl;
      // std::cout << "N_bins_OPEN    :" << N_bins_OPEN << std::endl; 
      // std::cout << "N_bins_CLOSE   :" << N_bins_CLOSE << std::endl; 

      opening_angle_chargeWgt_bin[N_bins_OPEN ] 
                    += hit.charge/fParams.sum_charge;
      closing_angle_chargeWgt_bin[N_bins_CLOSE] 
                    += hit.charge/fParams.sum_charge;
      opening_angle_bin[N_bins_OPEN] += wgt;
      closing_angle_bin[N_bins_CLOSE] += wgt;

      //Also fill bins for high charge hits
      if(hit.charge > fParams.mean_charge){
        opening_angle_highcharge_bin[N_bins_OPEN] += wgt;
        closing_angle_highcharge_bin[N_bins_CLOSE] += wgt;
      }

    }

    //initialize iterators for the angles
    int iBin(0), jBin(0), kBin(0), lBin(0), mBin(0), nBin(0);  
    double percentage_OPEN(0.0),
           percentage_CLOSE(0.0),
           percentage_OPEN_HC(0.0),
           percentage_CLOSE_HC(0.0),  //The last 2 are for High Charge (HC)
           percentage_OPEN_CHARGEWGT(0.0),
           percentage_CLOSE_CHARGEWGT(0.0); 

    for(iBin = 0; percentage_OPEN<= percentage && iBin < NBINS; iBin++)
    {
      percentage_OPEN += opening_angle_bin[iBin];
    }

    for(jBin = 0; percentage_CLOSE<= percentage && jBin < NBINS; jBin++)
    {
      percentage_CLOSE += closing_angle_bin[jBin];
    }

    for(kBin = 0; percentage_OPEN_HC<= percentage_HC && kBin < NBINS; kBin++)
    {
      percentage_OPEN_HC += opening_angle_highcharge_bin[kBin];
    }

    for(lBin = 0; percentage_CLOSE_HC<= percentage_HC && lBin < NBINS; lBin++)
    {
      percentage_CLOSE_HC += closing_angle_highcharge_bin[lBin];
    }

    for(mBin = 0; percentage_OPEN_CHARGEWGT<= percentage && mBin < NBINS; mBin++)
    {
      percentage_OPEN_CHARGEWGT += opening_angle_chargeWgt_bin[mBin];
    }

    for(nBin = 0; percentage_CLOSE_CHARGEWGT<= percentage && nBin < NBINS; nBin++)
    {
      percentage_CLOSE_CHARGEWGT += closing_angle_chargeWgt_bin[nBin];
    }

    double opening_angle = iBin * PI /NBINS ;
    double closing_angle = jBin * PI /NBINS ;
    double opening_angle_highcharge = kBin * PI /NBINS ;
    double closing_angle_highcharge = lBin * PI /NBINS ;
    double opening_angle_charge_wgt = mBin * PI /NBINS ;
    double closing_angle_charge_wgt = nBin * PI /NBINS ;

    // std::cout<<"opening angle: "<<opening_angle<<std::endl;
    // std::cout<<"closing angle: "<<closing_angle<<std::endl;
    // std::cout<<"opening high charge angle: "<<opening_angle_highcharge<<std::endl;
    // std::cout<<"closing high charge angle: "<<closing_angle_highcharge<<std::endl;
    // std::cout<<"opening high charge wgt  : "<<opening_angle_charge_wgt<<std::endl;
    // std::cout<<"closing high charge wgt  : "<<closing_angle_charge_wgt<<std::endl;
    // std::cout<<"fCoarseChargeProfile     : "<<fCoarseChargeProfile[0] 
    //          << ", " << fCoarseChargeProfile[1] << std::endl;

    double value_1 = closing_angle/opening_angle -1;
    // if (fCoarseChargeProfile.size() != 2) std::cout <<" !!!!!!!!!!!!!!!!!! \n\n\n\n";
    double value_2 = (fCoarseChargeProfile[0]/fCoarseChargeProfile[1]);
    if (value_2 < 100.0 && value_2 > 0.01)
      value_2 = log(value_2);
    else
      value_2 = 0.0;
    double value_3 = closing_angle_charge_wgt/opening_angle_charge_wgt -1;

    // if (value_1 > 1.0) value_1 = -1.0/value_1;
    // if (value_2 < 1.0) value_2 = -1.0/value_2;
    // if (value_3 > 1.0) value_3 = -1.0/value_3;

    // std::cout << "value_1 : " << value_1 << std::endl;
    // std::cout << "value_2 : " << value_2 << std::endl;
    // std::cout << "value_3 : " << value_3 << std::endl;

    // Using a sigmoid function to determine flipping.
    // I am going to weigh all of the values above (1, 2, 3) equally.
    // But, since value 2 in particular, I'm going to cut things off at 5.

    if (value_1 > 3  ) value_1 = 3.0;
    if (value_1 < -3 ) value_1 = -3.0;
    if (value_2 > 3  ) value_2 = 3.0;
    if (value_2 < -3 ) value_2 = -3.0;
    if (value_3 > 3  ) value_3 = 3.0;
    if (value_3 < -3 ) value_3 = -3.0;

    double Exp = exp(value_1 + value_2 + value_3);
    double sigmoid = (Exp - 1)/(Exp + 1);

    // std::cout << "sigmoid: " << sigmoid << std::endl;

    bool flip = false;
    if (sigmoid < 0) flip = true;
    if (flip){
      std::cout << "Flipping!" << std::endl;
      std::swap(opening_angle,closing_angle);
      std::swap(opening_angle_highcharge,closing_angle_highcharge);
      std::swap(opening_angle_charge_wgt,closing_angle_charge_wgt);
      std::swap(fParams.start_point,fParams.end_point);
      std::swap(fRoughBeginPoint,fRoughEndPoint);
    }
    else{
      std::cout << "Not Flipping!\n";
    }

    fParams.opening_angle = opening_angle;
    fParams.opening_angle_charge_wgt = opening_angle_charge_wgt;
    fParams.closing_angle = closing_angle;
    fParams.closing_angle_charge_wgt = closing_angle_charge_wgt;

    fFinishedRefineDirection = true;

    return;
  } //end RefineDirection
  

  void ClusterParamsAlgNew::FillPolygon()
  {
    if(fHitVector.size()) {
      std::cout<<"Filling polygon!"<<std::endl;
      fGSer->SelectPolygonHitList(fHitVector,fParams.container_polygon);
      //now making Polygon Object
      std::pair<float,float> tmpvertex;
      //make Polygon Object as in mac/PolyOverlap.cc
      std::vector<std::pair<float,float> > vertices;
      for (unsigned int i=0; i<fParams.container_polygon.size(); i++){
	tmpvertex = std::make_pair( fParams.container_polygon.at(i)->w,
				    fParams.container_polygon.at(i)->t );
	vertices.push_back( tmpvertex );
      }
      fParams.PolyObject = Polygon( vertices );

      std::cout<<fParams.container_polygon.size()<<std::endl;
    }
  }
  
  
  ///////////////////////////////////////////////////////////////////////////////////
  
  void ClusterParamsAlgNew::RefineStartPointAndDirection(bool override){
    // This function is meant to pick the direction.
    // It refines both the start and end point, and then asks 
    // if it should flip.
    
    std::cout << " here!!! "  << std::endl;
    
    if(!override) { //Override being set, we skip all this logic.
      //OK, no override. Stop if we're already finshed.
    if (fFinishedRefineStartPointAndDirection) return;
      //Try to run the previous function if not yet done.
    if (!fFinishedGetProfileInfo) GetProfileInfo(true);
    } else {
      //Try to run the previous function if not yet done.
      if (!fFinishedGetProfileInfo) GetProfileInfo(true);
    }
    std::cout << "REFINING .... " << std::endl;
    std::cout << "  Rough start and end point: " << std::endl; 
    std::cout << "    s: (" << fParams.start_point.w << ", " 
              << fParams.start_point.t << ")" << std::endl;
    std::cout << "    e: (" << fParams.end_point.w << ", " 
              << fParams.end_point.t << ")" << std::endl;
    RefineStartPoints();
    std::cout << "  Once Refined start and end point: " << std::endl;
    std::cout << "    s: (" << fParams.start_point.w << ", " 
              << fParams.start_point.t << ")" << std::endl;
    std::cout << "    e: (" << fParams.end_point.w << ", " 
              << fParams.end_point.t << ")" << std::endl;
    std::swap(fParams.start_point,fParams.end_point);
    std::swap(fRoughBeginPoint,fRoughEndPoint);
    RefineStartPoints();
    std::cout << "  Twice Refined start and end point: " << std::endl;
    std::cout << "    s: (" << fParams.start_point.w << ", " 
              << fParams.start_point.t << ")" << std::endl;
    std::cout << "    e: (" << fParams.end_point.w << ", " 
              << fParams.end_point.t << ")" << std::endl;
    std::swap(fParams.start_point,fParams.end_point);
    std::swap(fRoughBeginPoint,fRoughEndPoint);
    RefineDirection();
    std::cout << "  Final start and end point: " << std::endl;
    std::cout << "    s: (" << fParams.start_point.w << ", " 
              << fParams.start_point.t << ")" << std::endl;
    std::cout << "    e: (" << fParams.end_point.w << ", " 
              << fParams.end_point.t << ")" << std::endl;

    fParams.direction = (fParams.start_point.w < fParams.end_point.w)	 ? 1 : -1;     
	      
    return;   
  }

  void ClusterParamsAlgNew::TrackShowerSeparation(bool override){
    std::vector<float> FeatureVector, outputVector;
    GetFANNVector(FeatureVector);
    fannModule.run(FeatureVector,outputVector);
    fParams.trackness  = outputVector[1];
    fParams.showerness = outputVector[0];
  }


  
} //end namespace

#endif
