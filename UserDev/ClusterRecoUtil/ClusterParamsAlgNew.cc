#ifndef CLUSTERPARAMSALGNEW_HH
#define CLUSTERPARAMSALGNEW_HH

#include "ClusterParamsAlgNew.hh"

//-----Math-------
#include <math.h>       
#define PI 3.14159265


namespace cluster{

  ClusterParamsAlgNew::ClusterParamsAlgNew(std::vector<larutil::PxHit>){

    // Make default values
    // Is done by the struct    
  
    // Make sure TPrincipal is initialized:
    principal = new TPrincipal(2);

    finished_GetAverages       = false;
    finished_GetRoughAxis      = false;
    finished_GetProfileInfo    = false;
    finished_RefineStartPoints = false;
    finished_GetFinalSlope     = false;

    gser = (larutil::GeometryUtilities*)(larutil::GeometryUtilities::GetME());
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

//What is this cruft?//      if(!override) { //Override being set, we skip all this logic.
//What is this cruft?//      //OK, no override. Stop if we're already finshed.
//What is this cruft?//      if (finished_GetAverages) return;
//What is this cruft?//    }
//What is this cruft?//
//What is this cruft?//    principal -> Clear();
//What is this cruft?//
//What is this cruft?//    _this_params.N_Hits = hitVector.size();
//What is this cruft?//
//What is this cruft?//    std::map<double, int> wireMap;
//What is this cruft?//
//What is this cruft?//    double mean_charge = 0.0;
//What is this cruft?//
//What is this cruft?//
//What is this cruft?//
//What is this cruft?//    for (auto & hit : hitVector){
//What is this cruft?//      double data[2];
//What is this cruft?//      data[0] = hit.w;
//What is this cruft?//      data[1] = hit.t;
//What is this cruft?//      principal -> AddRow(data);
//What is this cruft?//      _this_params.charge_wgt_x += hit.w*hit.charge;
//What is this cruft?//      _this_params.charge_wgt_y += hit.t*hit.charge;
//What is this cruft?//      mean_charge += hit.charge;
//What is this cruft?//
//What is this cruft?//      wireMap[hit.w] ++;
//What is this cruft?//
//What is this cruft?//    }
//What is this cruft?//    _this_params.N_Wires = wireMap.size();
//What is this cruft?//    _this_params.multi_hit_wires = _this_params.N_Hits - _this_params.N_Wires;
//What is this cruft?//
//What is this cruft?//    _this_params.charge_wgt_x /= mean_charge;
//What is this cruft?//    _this_params.charge_wgt_y /= mean_charge;
//What is this cruft?//
//What is this cruft?//    _this_params.mean_x = (* principal -> GetMeanValues())[0];
//What is this cruft?//    _this_params.mean_y = (* principal -> GetMeanValues())[1];
//What is this cruft?//    _this_params.mean_charge /= _this_params.N_Hits;
//What is this cruft?//
//What is this cruft?//    principal -> MakePrincipals();
//What is this cruft?//
//What is this cruft?//    _this_params.eigenvalue_principal = (* principal -> GetEigenValues() )[0];
//What is this cruft?//    _this_params.eigenvalue_secondary = (* principal -> GetEigenValues() )[1];
//What is this cruft?//
//What is this cruft?//}

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

    double inter_high=-999999;
    double inter_low=999999;
    double inter_high_side=-999999;
    double inter_low_side=999999;
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
     
     
     double projectedlength=gser->Get2DDistance(HighOnlinePoint,LowOnlinePoint);
     
      
      
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
	charge_profile[fine_bin]+=weight;
      
      if(coarse_bin<coarse_nbins) //only fill if bin number is in range
	coarse_charge_profile[coarse_bin]+=weight;
      
           
      }  // end second loop on hits. Now should have filled profile vectors.
      
    finished_GetProfileInfo = true;
    return;    
  }
  
  void ClusterParamsAlgNew::RefineStartPoints(bool override) {
    if(!override) { //Override being set, we skip all this logic.
      //OK, no override. Stop if we're already finshed.
      if (finished_RefineStartPoints) return;
      //Try to run the previous function if not yet done.
      if (!finished_GetProfileInfo) GetProfileInfo(true);
    } else {
      if (!finished_GetProfileInfo) GetProfileInfo(true);
    }
	//Ryan's Shower Strip finder work here. 

		//First we need to define the strip width that we want
			double d=0.6;//this is the width of the strip.... this needs to be tuned to something.
	//===============================================================================================================	
			// Will need to feed in the set of hits that we want. 
			std::vector<std::pair<double,double>> vertil;//vertex in tilda-space pair(x-til,y-til)
			vertil.clear();// This isn't needed?
			std::vector<double> vs;//vector of the sum of the distance of a vector to every vertex in tilda-space
			vs.clear();// this isn't needed?
			std::vector< hits>  ghits;// $$This needs to be corrected//this is the good hits that are between strip
			double vd=0;//the distance for vertex... just needs to be something 0
			int n=0;
			double fardistcurrent=0;
			HITHERE startpoint;
	//===============================================================================================================	
			
		//Now we need to do the transformation into "tilda-space"
			 for(unsigned int a=0; a<subhit.size();a++){
				for(unsigned int b=a+1;b<subhit.size();b++){
						if(subhit[a]->w != subhit[b]->w){
			double xtil = ((subhit[a]->t-avgtime)-(subhit[b]->t-avgtime))/((subhit[a]->w-avgwire)-(subhit[b]->w-avgwire));
			double ytil = (subhit[a]->w - avgwire)*xtil -(subhit[a]->t-avgtime);
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
        double dtstrip= (-tilwire * (subhit[a]->w-avgwire) +(subhit[a]->t-avgtime)-tiltimet)/sqrt(tilwire*tilwire+1);
        double dbstrip= (-tilwire * (subhit[a]->w-avgwire) +(subhit[a]->t-avgtime)-tiltimeb)/sqrt(tilwire*tilwire+1);

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
		gwiretime+= ghits[g]->w *ghits[g]->t;
		gwire+= ghits[g]->w;
		gtime+= ghits[g]->t;
		gwirewire+= ghits[g]->w* ghits[g]->w;
		// now work on calculating the distance in wire time space from the far point
			//farhit needs to be a hit that is given to me
		fardist= sqrt(pow(ghits[g]->w-farhit.w,2)+pow(ghits[g]->t-farhit.t,2));
		//come back to this... there is a better way to do this probably in the loop
		//there should also be a check that the hit that is farthest away has subsequent hits after it on a few wires
			if(fardist>fardistcurrent){
			fardistcurrent=fardist;
			//start point hit 
			startpoint =ghits[g];}//if fardist... this is the point to use for the start point
						}//for ghits loop

	double gslope=(n* gwiretime- gwire*gtime)/(n*gwirewire -gwire*gwire);
	double gcept= gtime/n -gslope*(gwire/n);


		

	

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
  
  
  
  
  
  
}

#endif
