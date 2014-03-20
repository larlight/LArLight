#ifndef CLUSTERPARAMSALGNEW_HH
#define CLUSTERPARAMSALGNEW_HH

#include "ClusterParamsAlgNew.hh"

//-----Math-------
#include <math.h>       
#define PI 3.14159265


namespace cluster{

  ClusterParamsAlgNew::ClusterParamsAlgNew()
  {
    Initialize();
  }

  ClusterParamsAlgNew::~ClusterParamsAlgNew(){
    if (principal != 0) delete principal;
  }

  ClusterParamsAlgNew::ClusterParamsAlgNew(const std::vector<larutil::PxHit> &inhitlist){
    Initialize();
    SetHits(inhitlist);
  }

  void ClusterParamsAlgNew::SetHits(const std::vector<larutil::PxHit> &inhitlist){

    // Make default values
    // Is done by the struct
    hitVector=inhitlist;
    if(!(hitVector.size())) {
      throw RecoUtilException("Provided empty hit list!");
      return;
    }
    fplane=hitVector.at(0).plane;

  }

  void ClusterParamsAlgNew::Initialize()
  {
    // Clear hit vector
    hitVector.clear();

    // Set pointer attributes
    if(!principal) principal = new TPrincipal(2);    
    if(!gser) gser = (larutil::GeometryUtilities*)(larutil::GeometryUtilities::GetME());
    if(!detp) detp = (larutil::DetectorProperties*)(larutil::DetectorProperties::GetME());
    if(!geo)  geo  = (larutil::Geometry*)(larutil::Geometry::GetME());
    if(!larp) larp = (larutil::LArProperties*)(larutil::LArProperties::GetME());


    //--- Initilize attributes values ---//
    fWire2Cm.resize(geo->Nplanes(),0);
    for(size_t i=0; i<fWire2Cm.size(); ++i)
      fWire2Cm.at(i) = geo->WirePitch(0,1,(UChar_t)i);

    fTime2Cm = (detp->SamplingRate()/1.e3);
    fTime2Cm *= (larp->DriftVelocity(larp->Efield(),larp->Temperature()));

    finished_GetAverages       = false;
    finished_GetRoughAxis      = false;
    finished_GetProfileInfo    = false;
    finished_RefineStartPoints = false;
    finished_GetFinalSlope     = false;

    rough_2d_slope=-999.999;    // slope 
    rough_2d_intercept=-999.999;    // slope 
       
    rough_begin_point.w=-999.999;
    rough_end_point.w=-999.999;
     
    rough_begin_point.t=-999.999;
    rough_end_point.t=-999.999;

    profile_integral_forward=-999.999;
    profile_integral_backward=-999.999;
    profile_maximum_bin=-999;
    
    fChargeCutoffThreshold[0]=500;
    fChargeCutoffThreshold[1]=500;
    fChargeCutoffThreshold[2]=1000;


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

  void ClusterParamsAlgNew::GetParams(cluster::cluster_params&  inputstruct){
    inputstruct = _this_params;
    return;
  }


  void ClusterParamsAlgNew::GetAverages(bool override){
    if(!override) { //Override being set, we skip all this logic.
      //OK, no override. Stop if we're already finshed.
      if (finished_GetAverages) return;
    }

    principal -> Clear();

    _this_params.N_Hits = hitVector.size();

    std::map<double, int> wireMap;

    double mean_charge = 0.0;



    for (auto & hit : hitVector){
      double data[2];
      data[0] = hit.w;
      data[1] = hit.t;
      principal -> AddRow(data);
      _this_params.charge_wgt_x += hit.w*hit.charge;
      _this_params.charge_wgt_y += hit.t*hit.charge;
      mean_charge += hit.charge;

      wireMap[hit.w] ++;

    }
    _this_params.N_Wires = wireMap.size();
    _this_params.multi_hit_wires = _this_params.N_Hits - _this_params.N_Wires;

    _this_params.charge_wgt_x /= mean_charge;
    _this_params.charge_wgt_y /= mean_charge;

    _this_params.mean_x = (* principal -> GetMeanValues())[0];
    _this_params.mean_y = (* principal -> GetMeanValues())[1];
    _this_params.mean_charge /= _this_params.N_Hits;

    principal -> MakePrincipals();

    _this_params.eigenvalue_principal = (* principal -> GetEigenValues() )[0];
    _this_params.eigenvalue_secondary = (* principal -> GetEigenValues() )[1];

    finished_GetAverages = true;
  }

  // Also does the high hitlist
  void ClusterParamsAlgNew::GetRoughAxis(bool override){
    if(!override) { //Override being set, we skip all this logic.
      //OK, no override. Stop if we're already finshed.
      if (finished_GetRoughAxis) return;
      //Try to run the previous function if not yet done.
      if (!finished_GetAverages) GetAverages(true);
    } else {
      //Try to run the previous function if not yet done.
      if (!finished_GetAverages) GetAverages(true);
    }

    //using the charge weighted coordinates find the axis from slope
    double ncw=0;
    double sumtime=0;//from sum averages
    double sumwire=0;//from sum averages
    double sumwiretime=0;//sum over (wire*time)
    double sumwirewire=0;//sum over (wire*wire)
    //next loop over all hits again
    for (auto & hit : hitVector){
      //if charge is above avg_charge
      if(hit.charge > _this_params.mean_charge){
        ncw+=1;
        sumwire+=hit.w;
        sumtime+=hit.t;
        sumwiretime+=hit.w*hit.t;
        sumwirewire+=pow(hit.w,2);  
      }//for high charge
    }//For hh loop

    //Looking for the slope and intercept of the line above avg_charge hits
    double slopecw=0;
    //double ceptcw=0;
    slopecw= (ncw*sumwiretime- sumwire*sumtime)/(ncw*sumwirewire-sumwire*sumwire);//slope for cw
    //ceptcw= _this_params.charge_wgt_y  -slopecw*(_this_params.charge_wgt_x);//intercept for cw
    //Getthe 2D_angle
    _this_params.cluster_angle_2d = atan(slopecw)*180/PI;


    finished_GetRoughAxis = true;
    return;
  }


  void ClusterParamsAlgNew::GetProfileInfo(bool override)  {
    if(!override) { //Override being set, we skip all this logic.
      //OK, no override. Stop if we're already finshed.
      if (finished_GetProfileInfo) return;
      //Try to run the previous function if not yet done.
      if (!finished_GetRoughAxis) GetRoughAxis(true);
    } else {
      if (!finished_GetRoughAxis) GetRoughAxis(true);
    }

    //these variables need to be initialized to other values? 
    if(rough_2d_slope==-999.999 || rough_2d_intercept==-999.999 ) 
      GetRoughAxis(true);      

    coarse_nbins=2;
    profile_nbins=100;
    
    charge_profile.clear();
    coarse_charge_profile.clear();
    charge_profile.resize(profile_nbins);
    coarse_charge_profile.resize(coarse_nbins);
    
    //get slope of lines orthogonal to those found crossing the shower.
    double inv_2d_slope=0;
    if(rough_2d_slope){
      inv_2d_slope=-1./rough_2d_slope; //*fWireTimetoCmCm*fWireTimetoCmCm;
    }
    else
      inv_2d_slope=-999999.;

    inter_high=-999999;
    inter_low=999999;
    inter_high_side=-999999;
    inter_low_side=999999;
    //loop over all hits. Create coarse and fine profiles of the charge weight to help refine the start/end points and have a first guess of direction
    for(auto & hit : hitVector)
    {
      
      //calculate intercepts along   
      double intercept=hit.t-inv_2d_slope*(double)hit.w;
        
      double side_intercept=hit.t-rough_2d_slope*(double)hit.w;
        
      if(intercept > inter_high ){
        inter_high=intercept;
      }
        
      if(intercept < inter_low ){
        inter_low=intercept;
      }  
    
      if(side_intercept > inter_high_side ){
        inter_high_side=side_intercept;
      }
        
      if(side_intercept < inter_low_side ){
        inter_low_side=side_intercept;
      }
    

    }   // end of first HitIter loop, at this point we should have the extreme intercepts 
  
    /////////////////////////////////////////////
    // Second loop. Fill profiles. 
    /////////////////////////////////////////////
    
    // get projected points at the beginning and end of the axis.
    
    larutil::PxPoint HighOnlinePoint, LowOnlinePoint,BeginOnlinePoint;
    
    gser->GetPointOnLineWSlopes(rough_2d_slope,rough_2d_intercept,inter_high,HighOnlinePoint);
    gser->GetPointOnLineWSlopes(rough_2d_slope,rough_2d_intercept,inter_low,LowOnlinePoint);
  

    //define BeginOnlinePoint as the one with lower wire number (for now)
    
    BeginOnlinePoint = (HighOnlinePoint.w > LowOnlinePoint.w) ? LowOnlinePoint : HighOnlinePoint;
    
    projectedlength=gser->Get2DDistance(HighOnlinePoint,LowOnlinePoint);
     
    double current_maximum=0; 
    for(auto & hit : hitVector)
    {
     
      larutil::PxPoint OnlinePoint;
      // get coordinates of point on axis.
      gser->GetPointOnLine(rough_2d_slope,LowOnlinePoint,hit,OnlinePoint);
    
      double linedist=gser->Get2DDistance(OnlinePoint,BeginOnlinePoint);
      double ortdist=gser->Get2DDistance(OnlinePoint,hit);
    
      ////////////////////////////////////////////////////////////////////// 
      //calculate the weight along the axis, this formula is based on rough guessology. 
      // there is no physics motivation behind the particular numbers, A.S.
      /////////////////////////////////////////////////////////////////////// 
      double weight= (ortdist<1.) ? 10*hit.charge : 5*hit.charge/ortdist;
    
      int fine_bin=(int)linedist/projectedlength*profile_nbins;
      int coarse_bin=(int)linedist/projectedlength*coarse_nbins; 
      
      if(fine_bin<profile_nbins)  //only fill if bin number is in range
      {
        charge_profile[fine_bin]+=weight;
        if(charge_profile[fine_bin]>current_maximum && fine_bin!=0 && fine_bin!=profile_nbins-1) //find maximum bin on the fly.
        {
          current_maximum=charge_profile[fine_bin];
          profile_maximum_bin=fine_bin; 
        }
      }
      
      if(coarse_bin<coarse_nbins) //only fill if bin number is in range
      coarse_charge_profile[coarse_bin]+=weight;
      
           
    }  // end second loop on hits. Now should have filled profile vectors.
      
    finished_GetProfileInfo = true;
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
      if (finished_RefineStartPoints) return;
      //Try to run the previous function if not yet done.
      if (!finished_GetProfileInfo) GetProfileInfo(true);
    } else {
      if (!finished_GetProfileInfo) GetProfileInfo(true);
    }
	
    
    profile_integral_forward=0;
    profile_integral_backward=0;
    
    //calculate the forward and backward integrals counting int the maximum bin.
    
    for(int ibin=0;ibin<profile_nbins;ibin++)
    {
    if(ibin<=profile_maximum_bin)  
      profile_integral_forward+=charge_profile[ibin];
    
    if(ibin>=profile_maximum_bin)  
      profile_integral_backward+=charge_profile[ibin];
      
    }
    
    // now, we have the forward and backward integral so start stepping forward and backward to find the trunk of the 
    // shower. This is done making sure that the running integral until less than 1% is left and the bin is above 
    // a set theshold many empty bins.
    
    //forward loop
    double running_integral=profile_integral_forward;
    int startbin,endbin;
    for(startbin=profile_maximum_bin;startbin>1;startbin--)
    {
      running_integral-=charge_profile[startbin];
      if( charge_profile[startbin]<fChargeCutoffThreshold[fplane] && running_integral/profile_integral_forward<0.01 )
        break;
    }
    
    //backward loop
    running_integral=profile_integral_backward;
    for(endbin=profile_maximum_bin;endbin<profile_nbins-1;endbin++)
    {
      running_integral-=charge_profile[endbin];
      if( charge_profile[endbin]<fChargeCutoffThreshold[fplane] && running_integral/profile_integral_backward<0.01 )
      break;
    }
    
    // now have profile start and endpoints. Now translate to wire/time. Will use wire/time that are on the rough axis.
    //projectedlength is the length from inter_low to interhigh along the rough_2d_axis
    // on bin distance is: 
     larutil::PxPoint OnlinePoint;
     
     double ort_intercept_begin=(inter_high-inter_low)/profile_nbins*startbin;
     
     
     
    gser->GetPointOnLineWSlopes(rough_2d_slope,
            rough_2d_intercept,
            ort_intercept_begin,
            rough_begin_point);
     
    double ort_intercept_end=(inter_high-inter_low)/profile_nbins*endbin;
     
    gser->GetPointOnLineWSlopes(rough_2d_slope,
            rough_2d_intercept,
            ort_intercept_end,
            rough_end_point);
     
        //Ryan's Shower Strip finder work here. 
                //First we need to define the strip width that we want
                        double d=0.6;//this is the width of the strip.... this needs to be tuned to something.
        //===============================================================================================================       
                        // Will need to feed in the set of hits that we want. 
		//	const std::vector<larutil::PxHit*> whole;
			std::vector <larutil::PxHit> subhit;
		//	larutil::PxHit startHit;
		//	Double_t linearlimit;
		//	Double_t ortlimit;
		//	Double_t lineslopetest;
		//	larutil::PxHit averageHit;
//			avgwire= averageHit.w;
//			avgtime= averageHit.t;
		//	gser->SelectLocalHitlist(whole,subhit,startHit,linearlimit,ortlimit,lineslopetest,averageHit);
                        std::vector<std::pair<double,double>> vertil;//vertex in tilda-space pair(x-til,y-til)
                        vertil.clear();// This isn't needed?
                        std::vector<double> vs;//vector of the sum of the distance of a vector to every vertex in tilda-space
                        vs.clear();// this isn't needed?
                        std::vector< larutil::PxHit>  ghits;// $$This needs to be corrected//this is the good hits that are between strip
                        double vd=0;//the distance for vertex... just needs to be something 0
                        int n=0;
                        double fardistcurrent=0;
                        larutil::PxPoint startpoint;
			double gwiretime; 
			double gwire; 
			double gtime;
			double gwirewire;
			larutil::PxHit farhit;// will come from somewhere	

	//=============building the local vector===================
	//this is clumsy... but just want to get something to work for now. 
	//THIS is REAL Horrible and CLUMSY... We can make things into helper functions soon. 
		std::vector<larutil::PxHit> returnhits;
		std::vector<double> radiusofhit;
		std::vector<int> closehits;
		unsigned int minhits=50;	
	        double maxreset=0;
		double avgwire=0;
		double avgtime=0;
	if(minhits>hitVector.size()){
    for(auto & hit : hitVector){
	std::pair<double,double> rv(rough_end_point.w,rough_end_point.t);
		closehits.clear();
		radiusofhit.clear();
	        returnhits.clear();
       // for( unsigned int a=0; a<hit.size(); a++){
                double d= sqrt( pow((rv.first-hit.w),2) + pow((rv.second-hit.t),2)  );
        maxreset+=d;
        radiusofhit.push_back(d);}
        for(unsigned int b=0; b<minhits; b++){
                int minss= std::min_element(radiusofhit.begin(),radiusofhit.end())-radiusofhit.begin();
                closehits.push_back(minss);
                radiusofhit[minss] = maxreset;}
                //now make the vector o just the close hits using the closehit index
for( unsigned int k=0; k < closehits.size(); k++){
//first find the average wire and time for each set of hits... and make that the new origin before the transpose.
	avgwire+=hitVector[closehits[k]].w;
	avgtime+=hitVector[closehits[k]].t;
     returnhits.push_back(hitVector[closehits[k]]);}
	}//if hitVector is big enough	
	
	avgwire=avgwire/closehits.size();
	avgtime=avgtime/closehits.size();
	subhit=returnhits;












	
		








        //===============================================================================================================       

                //Now we need to do the transformation into "tilda-space"
                         for(unsigned int a=0; a<subhit.size();a++){
                                for(unsigned int b=a+1;b<subhit.size();b++){
                                                if(subhit[a].w != subhit[b].w){
                        double xtil = ((subhit[a].t-avgtime)-(subhit[b].t-avgtime))/((subhit[a].w-avgwire)-(subhit[b].w-avgwire));
                        double ytil = (subhit[a].w - avgwire)*xtil -(subhit[a].t-avgtime);
                        //push back the tilda vertex point on the pair
                                std::pair<double,double> tv(xtil,ytil);
                                vertil.push_back(tv);
                                                                                }//if Wires are not the same
                                                                            }//for over b
                                                               }//for over a
                // look at the distance from a tilda-vertex to all other tilda-verticies
                        for(unsigned int z=0;z<vertil.size();z++){
                                for(unsigned int c=0; c<vertil.size();c++){
                          vd+= sqrt(pow((vertil[z].first-vertil[c].first),2)+pow((vertil[z].second-vertil[c].second),2));}//for c loop
                                                         vs.push_back(vd);
                                                         vd=0.0;}//for z loop
                //need to find the min of the distance of vertex in tilda-space
                //this will get me the area where things are most linear
                int minvs= std::min_element(vs.begin(),vs.end())-vs.begin();
                // now use the min position to find the vertex in tilda-space
                //now need to look a which hits are between the tilda lines from the points
                //in the tilda space everything in wire time is based on the new origin which is at the average wire/time
                               double tilwire=vertil[minvs].first;//slope
                               double tiltimet=-vertil[minvs].second+d*sqrt(1+pow(tilwire,2));//negative cept is accounted for top strip
                               double tiltimeb=-vertil[minvs].second-d*sqrt(1+pow(tilwire,2));//negative cept is accounted for bottom strip
                        // look over the subhit list and ask for which are inside of the strip
                        for(unsigned int a=0; a<subhit.size(); a++){
        double dtstrip= (-tilwire * (subhit[a].w-avgwire) +(subhit[a].t-avgtime)-tiltimet)/sqrt(tilwire*tilwire+1);
        double dbstrip= (-tilwire * (subhit[a].w-avgwire) +(subhit[a].t-avgtime)-tiltimeb)/sqrt(tilwire*tilwire+1);

        if((dtstrip<0.0 && dbstrip>0.0)||(dbstrip<0.0 && dtstrip>0.0)){
                ghits.push_back(subhit[a]);
                                }//if between strip
                                                                }//for a loop

        //=======================Do stuff with the good hits============================

        //of these small set of hits just fit a simple line. 
        //then we will need to see which of these hits is farthest away 

        for(unsigned int g; g<ghits.size();g++){
                // should call the helper funtion to do the fit
                //but for now since there is no helper function I will just write it here......again
                n+=1;
                gwiretime+= ghits[g].w *ghits[g].t;
                gwire+= ghits[g].w;
                gtime+= ghits[g].t;
                gwirewire+= ghits[g].w* ghits[g].w;
                // now work on calculating the distance in wire time space from the far point
                        //farhit needs to be a hit that is given to me
                double fardist= sqrt(pow(ghits[g].w-farhit.w,2)+pow(ghits[g].t-farhit.t,2));
                //come back to this... there is a better way to do this probably in the loop
                //there should also be a check that the hit that is farthest away has subsequent hits after it on a few wires
                        if(fardist>fardistcurrent){
                        fardistcurrent=fardist;
                        //start point hit 
                        startpoint =ghits[g];}//if fardist... this is the point to use for the start point
                                                }//for ghits loop

       // double gslope=(n* gwiretime- gwire*gtime)/(n*gwirewire -gwire*gwire);
       // double gcept= gtime/n -gslope*(gwire/n);



    
       
    
    // ok. now have rough_begin_point and rough_end_point. No decision about direction has been made yet.
    // need to define physical direction with openind angles and pass that to Ryan's line finder.
     
    // Ariana Elena Mina put it here!  You have
    // rough_end_point
    // rough_end_point
    // and use them to get the axis

    finished_RefineStartPoints = true;
    return;
  }
  
  void ClusterParamsAlgNew::GetFinalSlope(bool override) {
    if(!override) { //Override being set, we skip all this logic.
      //OK, no override. Stop if we're already finshed.
      if (finished_GetFinalSlope) return;
      //Try to run the previous function if not yet done.
      if (!finished_RefineStartPoints) RefineStartPoints(true);
    } else {
      //Try to run the previous function if not yet done.
      if (!finished_RefineStartPoints) RefineStartPoints(true);
    }

    /**
     * Calculates the following variables:
     * hit_density_1D
     * hit_density_2D
     * angle_2d
     * direction
     */


    finished_RefineStartPoints = true;
    return;
  }
  
  
  void ClusterParamsAlgNew::RefineDirection(larutil::PxPoint &start,
              larutil::PxPoint &end) {
    
    UChar_t plane = (*hitVector.begin()).plane;

    Double_t wire_2_cm = fWire2Cm.at(plane);
    Double_t time_2_cm = fTime2Cm;
    
    Double_t SEP_X = (end.w - start.w) / wire_2_cm;
    Double_t SEP_Y = (end.t - start.t) / time_2_cm;

    Double_t rms_forward   = 0;
    Double_t rms_backward  = 0;
    Double_t mean_forward  = 0;
    Double_t mean_backward = 0;
    Double_t weight_total  = 0;
    for(auto const hit : hitVector) {

      weight_total = hit.charge; 

      // Compute forward mean
      Double_t SHIT_X = (hit.w - start.w) / wire_2_cm;
      Double_t SHIT_Y = (hit.t - start.t) / time_2_cm;

      Double_t cosangle = (SEP_X*SHIT_X + SEP_Y*SHIT_Y);
      cosangle /= ( pow(pow(SEP_X,2)+pow(SEP_Y,2),0.5) * pow(pow(SHIT_X,2)+pow(SHIT_Y,2),0.5));

      mean_forward += cosangle * hit.charge;
      rms_forward  += pow(cosangle * hit.charge,2);
      
      // Compute backward mean
      SHIT_X = (hit.w - end.w) / wire_2_cm;
      SHIT_Y = (hit.t - end.t) / time_2_cm;
      
      cosangle  = (SEP_X*SHIT_X + SEP_Y*SHIT_Y);
      cosangle /= ( pow(pow(SEP_X,2)+pow(SEP_Y,2),0.5) * pow(pow(SHIT_X,2)+pow(SHIT_Y,2),0.5));
      
      mean_backward += cosangle * hit.charge;
      rms_backward  += pow(cosangle * hit.charge,2);
      
    }

    rms_forward   = pow(rms_forward/pow(weight_total,2),0.5);
    mean_forward /= weight_total;

    rms_backward   = pow(rms_backward/pow(weight_total,2),0.5);
    mean_backward /= weight_total;
    
    if(rms_forward / mean_forward < rms_backward / mean_backward)
      std::cout<<"Right Direction"<<std::endl;
    else
      std::cout<<"Wrong Direction"<<std::endl;
    
  }
  
  
  
  
}

#endif
