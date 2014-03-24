#ifndef CLUSTERPARAMSALGNEW_HH
#define CLUSTERPARAMSALGNEW_HH

#include "ClusterParamsAlgNew.hh"

//-----Math-------
#include <math.h>       
#define PI 3.14159265


namespace cluster{

  ClusterParamsAlgNew::ClusterParamsAlgNew()
  {
    fPrincipal=nullptr;
    fGSer=nullptr;
    Initialize();
  }

  ClusterParamsAlgNew::~ClusterParamsAlgNew(){
    if (fPrincipal != 0) delete fPrincipal;
  }

  ClusterParamsAlgNew::ClusterParamsAlgNew(const std::vector<larutil::PxHit*> &inhitlist){
    fPrincipal=nullptr;
    fGSer=nullptr;
    Initialize();
    SetHits(inhitlist);
  }

  void ClusterParamsAlgNew::SetHits(const std::vector<larutil::PxHit*> &inhitlist){

    // Make default values
    // Is done by the struct
    if(!(inhitlist.size())) {
      throw RecoUtilException("Provided empty hit list!");
      return;
    }
    fParams.fHitPtrVector.clear();
    fParams.fHitPtrVector.reserve(inhitlist.size());
    for(auto h : inhitlist) {

      fParams.fHitPtrVector.push_back((const larutil::PxHit*)h);
     
    } 
    fPlane=fParams.fHitPtrVector.at(0)->plane;
    
  }

  void ClusterParamsAlgNew::SetHits(const std::vector<larutil::PxHit> &inhitlist){

    // Make default values
    // Is done by the struct
    if(!(inhitlist.size())) {
      throw RecoUtilException("Provided empty hit list!");
      return;
    }
    fParams.fHitPtrVector.clear();
    fParams.fHitPtrVector.reserve(inhitlist.size());
    for(auto &h : inhitlist) {

      fParams.fHitPtrVector.push_back(&h);
     
    } 
    fPlane=fParams.fHitPtrVector.at(0)->plane;
    
  }
  

  //Calculate opening angle
  double ClusterParamsAlgNew::GetOpeningAngle(const larutil::PxPoint *rough_start_point,
					      const larutil::PxPoint *rough_end_point,
					      const std::vector<const larutil::PxHit*> & hits)
  {
    
    double start_hit;
    double start_end_w = rough_start_point->w - rough_end_point->w;
    double start_end_t = rough_start_point->t - rough_end_point->t;
    double start_end_start_hit ;
    double dot_product ;
    double angle_hit_axis ;
    double opening_angle ;
    //int    Integral = 0;
    std::vector<int> binning_vector(100,0 ) ;
    start_end_start_hit = sqrt(pow(start_end_w,2)+ pow(start_end_t,2));
    
    for(auto hit : hits){
      
      dot_product = (hit->w - rough_start_point->w)*(start_end_w)+ (hit->t - rough_start_point->t) * (start_end_t) ; 
      start_hit = (hit->w - rough_start_point->w)*(hit->w - rough_start_point->w) + (hit->t - rough_start_point->t)*(hit->t - rough_start_point->t);
      start_hit = sqrt(start_hit);
      angle_hit_axis = dot_product/start_end_start_hit/start_hit ;
      int N_bins = 100 * acos(angle_hit_axis)/PI;
      binning_vector[N_bins]++;
    }
    
    int iBin(0);
    double percentage(0.0);
    for(iBin = 1;percentage<= 0.95; iBin++)
    {
      percentage += binning_vector[iBin]/(fParams.N_Hits) ;
    }
    
    opening_angle = iBin * PI /100 ;
    
    return opening_angle;
  }
  

  void ClusterParamsAlgNew::Initialize()
  {

    // Set pointer attributes
    if(!fPrincipal) fPrincipal = new TPrincipal(2);    
    if(!fGSer) fGSer = (larutil::GeometryUtilities*)(larutil::GeometryUtilities::GetME());

    //--- Initilize attributes values ---//
    fWire2Cm.resize(larutil::Geometry::GetME()->Nplanes(),0);
    for(size_t i=0; i<fWire2Cm.size(); ++i)
      fWire2Cm.at(i) = larutil::Geometry::GetME()->WirePitch(0,1,(UChar_t)i);

    fTime2Cm  = larutil::DetectorProperties::GetME()->SamplingRate()/1.e3;
    fTime2Cm *= larutil::LArProperties::GetME()->DriftVelocity(larutil::LArProperties::GetME()->Efield(),
							       larutil::LArProperties::GetME()->Temperature());

    fFinishedGetAverages       = false;
    fFinishedGetRoughAxis      = false;
    fFinishedGetProfileInfo    = false;
    fFinishedRefineStartPoints = false;
    fFinishedGetFinalSlope     = false;

    fRough2DSlope=-999.999;    // slope 
    fRough2DIntercept=-999.999;    // slope 
       
    fRoughBeginPoint.w=-999.999;
    fRoughEndPoint.w=-999.999;
     
    fRoughBeginPoint.t=-999.999;
    fRoughEndPoint.t=-999.999;

    fProfileIntegralForward=999.999;
    fProfileIntegralBackward=999.999;
    fProfileMaximumBin=-999;
    
    fChargeCutoffThreshold[0]=500;
    fChargeCutoffThreshold[1]=500;
    fChargeCutoffThreshold[2]=1000;

    fParams.Clear();
    
  }

  void ClusterParamsAlgNew::Report(){
    std::cout << "ClusterParamsAlgNew Report: "  << "\n"
              << "\tFinishedGetAverages "        << fFinishedGetAverages << "\n"
              << "\tFinishedGetRoughAxis "       << fFinishedGetRoughAxis << "\n"
              << "\tFinishedGetProfileInfo "     << fFinishedGetProfileInfo << "\n"
              << "\tFinishedRefineStartPoints "  << fFinishedRefineStartPoints << "\n"
              << "\tFinishedGetFinalSlope "      << fFinishedGetFinalSlope << "\n"
              << "--------------------------------------" << "\n";
    fParams.Report();
  }

  void ClusterParamsAlgNew::FillParams(bool override_DoGetAverages      ,  
                                       bool override_DoGetRoughAxis     ,  
                                       bool override_DoGetProfileInfo   ,  
                                       bool override_DoRefineStartPoints,  
                                       bool override_DoGetFinalSlope     ){
    GetAverages      (override_DoGetAverages      );
    GetRoughAxis     (override_DoGetRoughAxis     );
    GetProfileInfo   (override_DoGetProfileInfo   );
    RefineStartPoints(override_DoRefineStartPoints);
    GetFinalSlope    (override_DoGetFinalSlope    );
  }

  void ClusterParamsAlgNew::GetAverages(bool override){
    if(!override) { //Override being set, we skip all this logic.
      //OK, no override. Stop if we're already finshed.
      if (fFinishedGetAverages) return;
    }

    fPrincipal -> Clear();

    fParams.N_Hits = fParams.fHitPtrVector.size();

    std::map<double, int> wireMap;

    fParams.sum_charge = 0.;
    for(auto const hit : fParams.fHitPtrVector){
      std::cout << "This hit has charge " <<  hit -> charge << "\n";
      double data[2];
      data[0] = hit->w;
      data[1] = hit->t;
      fPrincipal -> AddRow(data);
      fParams.charge_wgt_x += hit->w * hit->charge;
      fParams.charge_wgt_y += hit->t * hit->charge;
      fParams.sum_charge += hit->charge;

      wireMap[hit->w] ++;

    }
    fParams.N_Wires = wireMap.size();
    fParams.multi_hit_wires = fParams.N_Hits - fParams.N_Wires;

    fParams.charge_wgt_x /= fParams.sum_charge;
    fParams.charge_wgt_y /= fParams.sum_charge;

    std::cout 
      << " charge weights:  x: " << fParams.charge_wgt_x 
      << " y: "                  << fParams.charge_wgt_y 
      << " mean charge: "        << fParams.sum_charge 
      << std::endl;
    fParams.mean_x = (* fPrincipal->GetMeanValues())[0];
    fParams.mean_y = (* fPrincipal->GetMeanValues())[1];
    fParams.mean_charge = fParams.sum_charge / fParams.N_Hits;

    fPrincipal -> MakePrincipals();

    fParams.eigenvalue_principal = (* fPrincipal -> GetEigenValues() )[0];
    fParams.eigenvalue_secondary = (* fPrincipal -> GetEigenValues() )[1];

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
    for (auto const hit : fParams.fHitPtrVector){
      // First, abuse this loop to calculate rms in x and y
      rmsx += pow(fParams.mean_x - hit->w, 2);
      rmsx += pow(fParams.mean_x - hit->w, 2);
      //if charge is above avg_charge
      std::cout << "This hit has charge " <<  hit -> charge << "\n";
      if(hit->charge > fParams.mean_charge){
        ncw+=1;
        sumwire+=hit->w;
        sumtime+=hit->t;
        sumwiretime+=hit->w * hit->t;
        sumwirewire+=pow(hit->w,2);  
      }//for high charge
    }//For hh loop

    //Looking for the slope and intercept of the line above avg_charge hits
    fRough2DSlope= (ncw*sumwiretime- sumwire*sumtime)/(ncw*sumwirewire-sumwire*sumwire);//slope for cw
    fRough2DIntercept= fParams.charge_wgt_y  -fRough2DSlope*(fParams.charge_wgt_x);//intercept for cw
    //Getthe 2D_angle
    fParams.cluster_angle_2d = atan(fRough2DSlope)*180/PI;

    std::cout << "fRough2DSlope is " << fRough2DSlope << std::endl;


    fFinishedGetRoughAxis = true;    
    Report();
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
    if(fabs(fRough2DSlope)>0.001){
      inv_2d_slope=-1./fRough2DSlope*fTime2Cm*fTime2Cm/(fWire2Cm.at(fPlane)*fWire2Cm.at(fPlane));
    }
    else
      inv_2d_slope=-999999.;
    std::cout << " N_Hits is " << fParams.N_Hits << std::endl;
    std::cout << " inv_2d_slope is " << inv_2d_slope << std::endl;
    std::cout << " fRough2DSlope is " << fRough2DSlope << std::endl;
    std::cout << " conversions " << fWire2Cm.at(fPlane) << " " << fTime2Cm << std::endl;
    
    fInterHigh=-999999;
    fInterLow=999999;
    fInterHigh_side=-999999;
    fInterLow_side=999999;
    //loop over all hits. Create coarse and fine profiles of the charge weight to help refine the start/end points and have a first guess of direction
    for(auto const hit : fParams.fHitPtrVector)
    {
      
      //calculate intercepts along   
      double intercept=hit->t - inv_2d_slope * (double)(hit->w);
        
      double side_intercept=hit->t - fRough2DSlope * (double)(hit->w);
        
      if(intercept > fInterHigh ){
        fInterHigh=intercept;
      }
        
      if(intercept < fInterLow ){
        fInterLow=intercept;
      }  
    
      if(side_intercept > fInterHigh_side ){
        fInterHigh_side=side_intercept;
      }
        
      if(side_intercept < fInterLow_side ){
        fInterLow_side=side_intercept;
      }
    

    }   // end of first HitIter loop, at this point we should have the extreme intercepts 
  
    /////////////////////////////////////////////
    // Second loop. Fill profiles. 
    /////////////////////////////////////////////
    
    // get projected points at the beginning and end of the axis.
    
    larutil::PxPoint HighOnlinePoint, LowOnlinePoint,BeginOnlinePoint;
    
    fGSer->GetPointOnLineWSlopes(fRough2DSlope,fRough2DIntercept,fInterHigh,&HighOnlinePoint);
    fGSer->GetPointOnLineWSlopes(fRough2DSlope,fRough2DIntercept,fInterLow,&LowOnlinePoint);

    std::cout << " extreme intercepts: low: " << fInterLow 
              << " " << fInterHigh << std::endl;
    std::cout << " extreme intercepts: side: " << fInterLow_side 
              << " " << fInterHigh_side << std::endl;
    std::cout << "axis + intercept "  << fRough2DSlope << " " 
              << fRough2DIntercept << std::endl;
    
    std::cout << " Low online point: " << LowOnlinePoint.w << ", " << LowOnlinePoint.t 
              << " High: " << HighOnlinePoint.w << ", " << HighOnlinePoint.t << std::endl; 

    //define BeginOnlinePoint as the one with lower wire number (for now)
    
    BeginOnlinePoint = (HighOnlinePoint.w > LowOnlinePoint.w) ? LowOnlinePoint : HighOnlinePoint;
    
    fProjectedLength=fGSer->Get2DDistance(&HighOnlinePoint,&LowOnlinePoint);
     
    std::cout << " projected length " << fProjectedLength 
              << " Begin Point " << BeginOnlinePoint.w << " " 
              << BeginOnlinePoint.t << std::endl;
    
    double current_maximum=0; 
    for(auto const hit : fParams.fHitPtrVector)
    {
     
      larutil::PxPoint OnlinePoint;
      // get coordinates of point on axis.
      std::cout << &BeginOnlinePoint << std::endl;
      std::cout << &OnlinePoint << std::endl;
      fGSer->GetPointOnLine(fRough2DSlope,&BeginOnlinePoint,hit,&OnlinePoint);
     //std::cout << " Online Point " << OnlinePoint.w << " " << OnlinePoint.t << std::endl; 
      double linedist=fGSer->Get2DDistance(&OnlinePoint,&BeginOnlinePoint);
      double ortdist=fGSer->Get2DDistance(&OnlinePoint,hit);
    
      ////////////////////////////////////////////////////////////////////// 
      //calculate the weight along the axis, this formula is based on rough guessology. 
      // there is no physics motivation behind the particular numbers, A.S.
      /////////////////////////////////////////////////////////////////////// 
      double weight= (ortdist<1.) ? 10 * (hit->charge) : 5 * (hit->charge) / ortdist;
    
      int fine_bin=(int)(linedist/fProjectedLength*fProfileNbins);
      int coarse_bin=(int)(linedist/fProjectedLength*fCoarseNbins);
      std::cout << "linedist: " << linedist << std::endl;
      std::cout << "fProjectedLength: " << fProjectedLength << std::endl;
      std::cout << "fProfileNbins: " << fProfileNbins << std::endl;
      std::cout << "fine_bin: " << fine_bin << std::endl;
      std::cout << "coarse_bin: " << coarse_bin << std::endl;
      //std::cout << "length" << linedist <<   " fine_bin, coarse " << fine_bin << " " << coarse_bin << std::endl;
      
      if(fine_bin<fProfileNbins)  //only fill if bin number is in range
      {


        fChargeProfile.at(fine_bin)+=weight;
        if(fChargeProfile.at(fine_bin)>current_maximum && fine_bin!=0 && fine_bin!=fProfileNbins-1) //find maximum bin on the fly.
        {
          current_maximum=fChargeProfile.at(fine_bin);
          fProfileMaximumBin=fine_bin; 
        }
      }
      
      if(coarse_bin<fCoarseNbins) //only fill if bin number is in range
	fCoarseChargeProfile.at(coarse_bin)+=weight;
      
           
    }  // end second loop on hits. Now should have filled profile vectors.
      
    fFinishedGetProfileInfo = true;
    return;    
  }
  
  
  /**
   * Calculates the following variables:
   * length
   * width
   * @param override [description]
   */
  void ClusterParamsAlgNew::RefineStartPoints(bool override) {
    if(!override) { //Override being set, we skip all this logic.
      //OK, no override. Stop if we're already finshed.
      if (fFinishedRefineStartPoints) return;
      //Try to run the previous function if not yet done.
      if (!fFinishedGetProfileInfo) GetProfileInfo(true);
    } else {
      if (!fFinishedGetProfileInfo) GetProfileInfo(true);
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
    
    // now, we have the forward and backward integral so start stepping forward and backward to find the trunk of the 
    // shower. This is done making sure that the running integral until less than 1% is left and the bin is above 
    // a set theshold many empty bins.
    
    //forward loop
    double running_integral=fProfileIntegralForward;
    int startbin,endbin;
    for(startbin=fProfileMaximumBin;startbin>1;startbin--)
      {
	running_integral-=fChargeProfile.at(startbin);
	if( fChargeProfile.at(startbin)<fChargeCutoffThreshold[fPlane] && running_integral/fProfileIntegralForward<0.01 )
	  break;
      }
    
    //backward loop
    running_integral=fProfileIntegralBackward;
    for(endbin=fProfileMaximumBin;endbin<fProfileNbins-1;endbin++)
      {
	running_integral-=fChargeProfile.at(endbin);
	if( fChargeProfile.at(endbin)<fChargeCutoffThreshold[fPlane] && running_integral/fProfileIntegralBackward<0.01 )
	  break;
      }
    
    // now have profile start and endpoints. Now translate to wire/time. Will use wire/time that are on the rough axis.
    //fProjectedLength is the length from fInterLow to interhigh along the rough_2d_axis
    // on bin distance is: 
   // larutil::PxPoint OnlinePoint;
    
    double ort_intercept_begin=fInterLow+(fInterHigh-fInterLow)/fProfileNbins*startbin;
    
    std::cout << " ort_intercept_begin: " << ort_intercept_begin << std::endl;
    
    fGSer->GetPointOnLineWSlopes(fRough2DSlope,
				 fRough2DIntercept,
				 ort_intercept_begin,
				 &fRoughBeginPoint);
    
    double ort_intercept_end=fInterLow+(fInterHigh-fInterLow)/fProfileNbins*endbin;
    
    std::cout << " ort_intercept_end: " << ort_intercept_end << std::endl;
    
    fGSer->GetPointOnLineWSlopes(fRough2DSlope,
				 fRough2DIntercept,
				 ort_intercept_end,
				 &fRoughEndPoint);
    
    
    std::cout << fRoughBeginPoint.w << ", " << fRoughBeginPoint.t << " end: " <<  fRoughEndPoint.w << " " << fRoughEndPoint.t << std::endl;
    
    //Ryan's Shower Strip finder work here. 
    //First we need to define the strip width that we want
    double d=0.6;//this is the width of the strip.... this needs to be tuned to something.
    //===============================================================================================================       
    // Will need to feed in the set of hits that we want. 
    //	const std::vector<larutil::PxHit*> whole;
    std::vector <const larutil::PxHit*> subhit;
    larutil::PxHit startHit;
    startHit.w = fRoughBeginPoint.w;
    startHit.t = fRoughBeginPoint.t;
    startHit.plane = fRoughBeginPoint.plane;
    Double_t linearlimit=10;
    Double_t ortlimit=10;
    Double_t lineslopetest;
    larutil::PxHit averageHit;
    //also are we sure this is actually doing what it is supposed to???
    //are we sure this works? 
    fGSer->SelectLocalHitlist(fParams.fHitPtrVector,subhit,
			      &startHit,
			      linearlimit,ortlimit,lineslopetest,
			      &averageHit);

    double avgwire= averageHit.w;
    double avgtime= averageHit.t;
    std::vector<std::pair<double,double>> vertil;//vertex in tilda-space pair(x-til,y-til)
    vertil.clear();// This isn't needed?
    vertil.reserve(subhit.size() * subhit.size());
    std::vector<double> vs;//vector of the sum of the distance of a vector to every vertex in tilda-space
    vs.clear();// this isn't needed?
    std::vector<const larutil::PxHit*>  ghits;// $$This needs to be corrected//this is the good hits that are between strip
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
    //    if(minhits<fParams.fHitPtrVector.size()){
    //      for(auto & hit : fParams.fHitPtrVector){
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
    //	avgwire+=fParams.fHitPtrVector[closehits[k]].w;
    //	avgtime+=fParams.fHitPtrVector[closehits[k]].t;
    //	returnhits.push_back(fParams.fHitPtrVector[closehits[k]]);}
    //   }//if fParams.fHitPtrVector is big enough	
    
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

	vd+= sqrt(pow((vertil.at(z).first - vertil.at(c).first),2) + pow((vertil.at(z).second-vertil.at(c).second),2));

      vs.push_back(vd);
      vd=0.0;
    }//for z loop
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
      double dtstrip= (-tilwire * (subhit.at(a)->w - avgwire) +(subhit.at(a)->t - avgtime)-tiltimet)/sqrt(tilwire*tilwire+1);
      double dbstrip= (-tilwire * (subhit.at(a)->w - avgwire) +(subhit.at(a)->t - avgtime)-tiltimeb)/sqrt(tilwire*tilwire+1);
      
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
    std::cout<<"Here Kazu"<<std::endl;
    std::cout<<"Ryan This is the new SP ("<<startpoint.w<<" , "<<startpoint.t<<")"<<std::endl;
    // double gslope=(n* gwiretime- gwire*gtime)/(n*gwirewire -gwire*gwire);
    // double gcept= gtime/n -gslope*(gwire/n);
    
    // ok. now have fRoughBeginPoint and fRoughEndPoint. No decision about direction has been made yet.
    // need to define physical direction with openind angles and pass that to Ryan's line finder.
 
    
    fParams.opening_angle = GetOpeningAngle(&fRoughBeginPoint,
					    &fRoughEndPoint, 
					    fParams.fHitPtrVector);

    fParams.start_point = fRoughBeginPoint;
    fParams.end_point   = fRoughEndPoint;
    // fRoughEndPoint
    // fRoughEndPoint
    // and use them to get the axis

    fFinishedRefineStartPoints = true;
    return;
  }
  
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
     * hit_density_1D
     * hit_density_2D
     * angle_2d
     * direction
     */


    fFinishedRefineStartPoints = true;
    return;
  }
  
  
  void ClusterParamsAlgNew::RefineDirection(larutil::PxPoint &start,
					    larutil::PxPoint &end) {
    
    UChar_t plane = (*fParams.fHitPtrVector.begin())->plane;

    Double_t wire_2_cm = fWire2Cm.at(plane);
    Double_t time_2_cm = fTime2Cm;
    
    Double_t SEP_X = (end.w - start.w) / wire_2_cm;
    Double_t SEP_Y = (end.t - start.t) / time_2_cm;

    Double_t rms_forward   = 0;
    Double_t rms_backward  = 0;
    Double_t mean_forward  = 0;
    Double_t mean_backward = 0;
    Double_t weight_total  = 0;
    Double_t hit_counter   = 0;
    
    //hard coding this for now, should use SetRefineDirectionQMin function
    fQMinRefDir  = 25;

    for(auto const hit : fParams.fHitPtrVector) {

      //skip this hit if below minimum cutoff param
      if(hit->charge < fQMinRefDir) continue;

      hit_counter++;

      weight_total = hit->charge; 

      // Compute forward mean
      Double_t SHIT_X = (hit->w - start.w) / wire_2_cm;
      Double_t SHIT_Y = (hit->t - start.t) / time_2_cm;

      Double_t cosangle = (SEP_X*SHIT_X + SEP_Y*SHIT_Y);
      cosangle /= ( pow(pow(SEP_X,2)+pow(SEP_Y,2),0.5) * pow(pow(SHIT_X,2)+pow(SHIT_Y,2),0.5));

      //no weighted average, works better as flat average w/ min charge cut
      mean_forward += cosangle;
      rms_forward  += pow(cosangle,2);
      
      // Compute backward mean
      SHIT_X = (hit->w - end.w) / wire_2_cm;
      SHIT_Y = (hit->t - end.t) / time_2_cm;
      
      cosangle  = (SEP_X*SHIT_X + SEP_Y*SHIT_Y);
      cosangle /= ( pow(pow(SEP_X,2)+pow(SEP_Y,2),0.5) * pow(pow(SHIT_X,2)+pow(SHIT_Y,2),0.5));
      
      //no weighted average, works better as flat average w/ min charge cut
      mean_backward += cosangle;
      rms_backward  += pow(cosangle,2);
      
    }

    //no weighted average, works better as flat average w/ min charge cut

    rms_forward   = pow(rms_forward/hit_counter,0.5);
    mean_forward /= hit_counter;

    rms_backward   = pow(rms_backward/hit_counter,0.5);
    mean_backward /= hit_counter;

    if(mean_forward > mean_backward && rms_forward < rms_backward) return;
    std::swap(start,end);
  }
  
  
  
  
}

#endif
