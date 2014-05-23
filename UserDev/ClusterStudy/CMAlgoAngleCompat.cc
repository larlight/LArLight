#ifndef CMALGOANGLECOMPAT_CC
#define CMALGOANGLECOMPAT_CC

#include "CMAlgoAngleCompat.hh"

namespace cluster {

  CMAlgoAngleCompat::CMAlgoAngleCompat() : CBoolAlgoBase() {

    //this just sets default values
    SetVerbose(true);

    SetAngleCut(30.); // in degrees

    SetMinHits(0);
    
    SetAllow180Ambig(false);

    SetUseOpeningAngle(false);

    angle_dist_histo = 0;

    angle_dist_histo = new TH1F("angle_dist_histo","Cluster Angle Differences",100,-360,360);

  } //end constructor

  bool CMAlgoAngleCompat::Bool(const ClusterParamsAlgNew &cluster1,
			       const ClusterParamsAlgNew &cluster2)
  {
    
    //if number of hits not large enough skip
    if ( (_minHits > 0) and ((cluster1.GetParams().N_Hits < _minHits) or (cluster2.GetParams().N_Hits < _minHits)) ) {
      return false;
    }

    //pretty sure we don't need conversion factors here. 
    //already in cm/cm units, degrees? need to check that
    double angle1 = cluster1.GetParams().angle_2d;// * _time_2_cm / _wire_2_cm;
    double angle2 = cluster2.GetParams().angle_2d;// * _time_2_cm / _wire_2_cm;
  
    //for some reason angles are frequently -999.99.
    //if either angle is this, clearly the cluster 2d angle is not well defined
    //and this algorithm does not apply
    if(angle1 < -998 || angle2 < -998)
      return false;

    if(angle_dist_histo){
      angle_dist_histo->Fill(angle1-angle2);
    }
    else
      std::cout<<"\n\n\nSOMETHING WENT HORRIBLY WRONG IN CMALGOANGLECOMPAT\n\n\n\n\n\n\n"<<std::endl;

    bool compatible = false;
    
    double my_cut_value = _max_allowed_2D_angle_diff;
    //if using opening angle, have angle cutoff be the smaller of the two opening angles
    if(_use_opening_angle) 
      my_cut_value = std::min(cluster1.GetParams().opening_angle, cluster2.GetParams().opening_angle);
    
    //if you don't care if clusters have been reconstructed backwards
    if(_allow_180_ambig)
      compatible = ( abs(angle1-angle2)     < _max_allowed_2D_angle_diff ||
		     abs(angle1-angle2-180) < _max_allowed_2D_angle_diff ||
		     abs(angle1-angle2+180) < _max_allowed_2D_angle_diff   );
    else
      compatible = ( abs(angle1-angle2)     < _max_allowed_2D_angle_diff );
    
  
    if(_verbose) {
	if(compatible) std::cout<<"These two clusters are compatible in angle."<<std::endl;
	else std::cout<<"These two clusters are NOT compatible in angle."<<std::endl;
    }
    
    return compatible;
    
  } // end Merge function 
  
  void CMAlgoAngleCompat::IterationBegin(const std::vector<cluster::ClusterParamsAlgNew> &clusters){
    
    if(angle_dist_histo) angle_dist_histo->Reset();


  }

}//end namespace cluster
#endif
