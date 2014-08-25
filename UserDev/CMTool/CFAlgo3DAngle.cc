#ifndef CFALGO3DANGLE_CXX
#define CFALGO3DANGLE_CXX

#include "CFAlgo3DAngle.hh"

namespace cmtool {

  //-------------------------------------------------------
  CFAlgo3DAngle::CFAlgo3DAngle() : CFloatAlgoBase()
  //-------------------------------------------------------
  {

	SetDebug(false) ;
	SetThetaCut(30) ;
	SetPhiCut(30)  ;
	SetRatio(0.05) ;
	cProfile = new TH1D("cProfile","Charge Profile",300,0,300);	

  }

  //-----------------------------
  void CFAlgo3DAngle::Reset()
  //-----------------------------
  {

  }

  //----------------------------------------------------------------------------------------------
  float CFAlgo3DAngle::Float(const std::vector<const cluster::ClusterParamsAlg*> &clusters)
  //----------------------------------------------------------------------------------------------
  {
    // Code-block by Kazu starts
    // This ensures all entries in "clusters" pointer vector are valid pointers.
    // You may take out this block if you want to allow matching using clusters from only 2 planes.
    // But make sure you handle the case of null pointer
    for(auto const& ptr : clusters) if(!ptr) return -1;
    // Code-block by Kazu ends


	int plane_0 = clusters.at(0)->GetParams().start_point.plane ;
	int plane_1 = clusters.at(1)->GetParams().start_point.plane ;
	int plane_2 = clusters.at(2)->GetParams().start_point.plane ;

	double angle_2d_0 = clusters.at(0)->GetParams().angle_2d;
	double angle_2d_1 = clusters.at(1)->GetParams().angle_2d;
	double angle_2d_2 = clusters.at(2)->GetParams().angle_2d;

	//Calculate angles theta and phi for cluster pairs across 2 planes
	double phi_01    = 0;   	
	double theta_01  = 0;
	double phi_12    = 0;   
	double theta_12  = 0;
	double phi_02 	= 0;
	double theta_02 = 0;

	double max_phi(0), middle_phi(0), min_phi(0);
	double max_theta(0), middle_theta(0), min_theta(0); 

	auto hits_0 = clusters.at(0)->GetParams().N_Hits ;
	auto hits_1 = clusters.at(1)->GetParams().N_Hits ;
	auto hits_2 = clusters.at(2)->GetParams().N_Hits ;
	
	//For ordering hits and rejecting pairs which have a large difference in hit number
	double max_hits(0), middle_hits(0), min_hits(0) ;


////////CHARGE PROFILE CALCULATION
/*	double hitDistance_radius = 0;
	double hitDistance_w = 0;
	double hitDistance_t = 0;
	int N_Bins =100 ;
	std::vector<double> chargeProfile(N_Bins);

	//Calculate distance of start point to each hit in cluster
	//M_Bins depends on "radius/length" hit parameter.
	//Fill corresponding M_Bin with correct charge weight
	for(int i=0; i < 3; i++){
		std::vector<larutil::PxHit> hitList(clusters.at(i)->GetParams().N_Hits) ;	

		for(auto const & h : clusters.at(i)->GetHitVector()){
			hitDistance_w = std::abs(clusters.at(i)->GetParams().start_point.w - h.w);
			hitDistance_t = std::abs(clusters.at(i)->GetParams().start_point.t - h.t);
			hitDistance_radius = sqrt(hitDistance_w * hitDistance_w + hitDistance_t * hitDistance_t ) ;

			if(hitDistance_radius <= clusters.at(i)->GetParams().length){
				int M_Bins = (N_Bins-1)* hitDistance_radius / clusters.at(i)->GetParams().length; 
				chargeProfile[M_Bins] 
					+= 1000*h.charge/clusters.at(i)->GetParams().sum_charge ; 
				cProfile->Fill(chargeProfile[M_Bins]);
				}
		}
			std::cout<<"Plane is: "<<clusters.at(i)->Plane()<<std::endl;
			std::cout<<"Number of Hits:" <<clusters.at(i)->GetNHits()<<std::endl;
			std::cout<<"Start Point: "<<clusters.at(i)->GetParams().start_point.w<<std::endl;
			std::cout<<"End Point: "<<clusters.at(i)->GetParams().end_point.w<<std::endl;
			std::cout<<"CHARGE PROFILE: "<<std::endl;

				for(int m = 0; m < chargeProfile.size(); m++) std::cout<<chargeProfile[m]<<", ";

			std::cout<<"\nMean, RMS, sigRMS are: "<<cProfile->GetMean(1)<<", "<<cProfile->GetRMS(1)<<", "<<cProfile->GetRMSError(1)<<std::endl;
	} */
////////////CHARGE PROFILE		

	//Calculate phi and theta from first 2 planes; check if third plane is consistent
	larutil::GeometryUtilities::GetME()->Get3DaxisN(plane_0,plane_1,angle_2d_0,angle_2d_1,phi_01,theta_01);	
	larutil::GeometryUtilities::GetME()->Get3DaxisN(plane_1,plane_2,angle_2d_1,angle_2d_2,phi_12,theta_12);	
	larutil::GeometryUtilities::GetME()->Get3DaxisN(plane_2,plane_0,angle_2d_2,angle_2d_0,phi_02,theta_02);	

	//Adjust the range of phis/thetas that are bigger than 360 or less than 0.
	FixPhiTheta(phi_01,theta_01);
	FixPhiTheta(phi_12,theta_12);
	FixPhiTheta(phi_02,theta_02);
	
	//Order phi and theta for ratio calculation later
	SetMaxMiddleMin(phi_01,phi_12,phi_02,max_phi,middle_phi,min_phi);
	SetMaxMiddleMin(theta_01,theta_12,theta_02,max_theta,middle_theta,min_theta);

	//Order hits from most to least 	
	SetMaxMiddleMin(hits_0,hits_1,hits_2,max_hits,middle_hits,min_hits);
	
	double ratio_max_min = 1;
	double ratio_max_middle =1;
	double ratio 		= 1;		

	//Test ratio for ChooseThetaOrPhi
	double ratio_angle =1;

	//This takes into account the fact that 0 and 360 having the same relative value (for both theta and phi)
	if(min_phi + 360 < max_phi +_phi_cut && min_phi +360 > max_phi - _phi_cut)
	 {	
		min_phi +=360 ;
		SetMaxMiddleMin(max_phi, middle_phi, min_phi,max_phi,middle_phi,min_phi);
	  }

	 if(min_theta + 360 < max_theta +_theta_cut && min_theta +360 > max_theta -  _theta_cut)
	  {	
		min_theta +=360 ;
		SetMaxMiddleMin(max_theta,middle_theta,min_theta,max_theta,middle_theta,min_theta);
	   }

	ChooseThetaOrPhi(max_theta,middle_theta,min_theta,max_phi,middle_phi,min_phi,ratio_angle);

	//Test to make sure that max hits is not too much bigger than min
	if(max_hits > middle_hits + 600 )
		ratio *=0.01 ;
	else{
		ratio_max_min = min_hits / max_hits ;
		ratio_max_middle = middle_hits/max_hits ;	
		ratio = ratio_angle * ratio_max_min * ratio_max_middle ;
		}	

	if(_debug && ratio > _ratio_cut ){
		std::cout<<"\nNhits planes 0, 1, 2: " <<clusters.at(0)->GetParams().N_Hits<<", "<<clusters.at(1)->GetParams().N_Hits
				 <<", "<<clusters.at(2)->GetParams().N_Hits ;
		std::cout<<"\nTotal ratio is: " <<ratio<<" ratio_angle: "<<ratio_angle ; 
		std::cout<<"\nStart End Points:  "<<clusters.at(0)->GetParams().start_point.w<<", "<<clusters.at(0)->GetParams().end_point.w<<"\n\t\t"
		<<clusters.at(1)->GetParams().start_point.w<<", "<<clusters.at(1)->GetParams().end_point.w<<"\n\t\t "
		<<clusters.at(2)->GetParams().start_point.w<<", "<<clusters.at(2)->GetParams().end_point.w;
	
		std::cout<<"\nTheta,Phi MaxMM : "<<max_theta<<", "<<middle_theta<<", "<<min_theta<<"\n\t\t"
				 <<max_phi<<", "<<middle_phi<<", "<<min_phi;
		std::cout<<"\nFor planes 0 and 1: "<<std::endl ;
		std::cout<<"\tPhi : "<<phi_01<<std::endl;
		std::cout<<"\tTheta : "<<theta_01<<std::endl ;
		std::cout<<"For planes 1 and 2: "<<std::endl ;
		std::cout<<"\tPhi : "<<phi_12<<std::endl;
		std::cout<<"\tTheta : "<<theta_12<<std::endl ;
		std::cout<<"For plane 0 and 2: "<<std::endl ;
		std::cout<<"\tPhi : "<<phi_02<<std::endl ;
		std::cout<<"\tTheta : "<<theta_02<<std::endl;
		std::cout<<"MATCH FOUND************************"<<std::endl<<std::endl;
	}
	
	
	return(ratio > _ratio_cut ? ratio : -1) ;
  }

 //--------------------------------
  void CFAlgo3DAngle::FixPhiTheta(double &phi, double &theta)
 //--------------------------------	
   {
		while(phi <= 0)
			phi +=360 ;
		while(phi > 360)
			phi -= 360 ;

		while(theta <= 0)
			theta += 360 ;
	    while(theta > 360)
			theta-=360;
	}	

  //------------------------------
  void CFAlgo3DAngle::SetMaxMin(const double angle_1, const double angle_2, double &max_angle, double &min_angle)
  //------------------------------
	{

		if(angle_1 > angle_2){
			max_angle = angle_1 ;
			min_angle = angle_2 ;
		 }
		else{
			max_angle = angle_2;
			min_angle	= angle_1 ;
		  }
   }

  //------------------------------
  void CFAlgo3DAngle::SetMaxMiddleMin(const double first, const double second, const double third, double &max, double &middle, double &min) 
  //------------------------------
  {

	if(first > second && first > third){
		max = first;
		}
	else if (first > second && first < third){
		max = third ;
		middle = first ;
		min = second ;
		}
	else if (first > third && first < second){
		max = second ;
		middle = first ;
		min = third ; 
		}
	else if(first <second && first <third)
		min = first ;
	

	if (max == first && second > third){
		middle = second ; 
		min = third    ;
		}
	else if (max ==first && third > second){
		middle = third ;	
		min = second 	;
		}

	if(min ==first && second > third){
		middle = third ;
		max	= second;
		}
	else if(min ==first && third > second){
		middle = second ;
		max = third ;
	    }


	//Very rarely, the angles(or hits) may be equal
	if( first == second && first > third ){
		max = first;
		middle = second ;
		min = third ;
		}
	else if( first == second && first < third){
		max = third;
		middle = first ;
		min = second ;
		}
	
	else if( first ==third && first > second){
		max = first;
		middle = third; 	
		min = second; 
		}
	
	else if( first == third && first < second){
		max = second ;
		middle = first;
		min = third ;
		}

	else if( second ==third && second < first){
		max = first;
		middle = third; 	
		min = second; 
		}
	
	else if( second == third && second > first){
		max = second;
		middle = third;
		min = first ;
		}
	

}

 //--------------------------------
  void CFAlgo3DAngle::ChooseThetaOrPhi(const double theta_max, const double theta_middle, const double theta_min, const double phi_max, const double phi_middle, const double phi_min, double &ratio_angle)
 //--------------------------------	
   {
	double theta_diff_1 = 0;
	double theta_diff_2 = 0;
	double phi_diff_1 = 0;
	double phi_diff_2 = 0;

	//Note: We only need theta_diff_2, phi_diff_2 in the event that theta_diff_1, phi_diff_1 are 0	
	theta_diff_1 = theta_max - theta_middle ;
	theta_diff_2 = theta_middle - theta_min ;
	phi_diff_1  = phi_max - phi_middle ;
	phi_diff_2  = phi_middle - phi_min ;


	if(theta_diff_1 != 0 ){
    	if(phi_diff_1 != 0 && phi_diff_1 < theta_diff_1) 
			ratio_angle = phi_middle/phi_max ; 
		else if(phi_diff_1 ==0 && phi_diff_2 < theta_diff_1) 
			ratio_angle =  phi_min/phi_middle;
		else 
			ratio_angle = theta_middle/theta_max;
		}
	else if(theta_diff_1 ==0){
    	if(phi_diff_1 != 0 && phi_diff_1 < theta_diff_2) 
			ratio_angle = phi_middle/phi_max ; 
		else if(phi_diff_1 ==0 && phi_diff_2 < theta_diff_2) 
			ratio_angle =  phi_min/phi_middle;
		else 
			ratio_angle = theta_min/theta_middle;
		}
	

	}


  //------------------------------
  void CFAlgo3DAngle::Report()
  //------------------------------
  {

  }
    
}
#endif
