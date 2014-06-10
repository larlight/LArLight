#ifndef CMALGOPOLYSHORTESTDIST_CC
#define CMALGOPOLYSHORTESTDIST_CC

#include "CMAlgoPolyShortestDist.hh"

namespace cluster {

  //-------------------------------------------------------
  CMAlgoPolyShortestDist::CMAlgoPolyShortestDist() : CBoolAlgoBase()
  //-------------------------------------------------------
  {
    SetMinDistSquared(1.);
    SetMinNumHits(0);
    SetDebug(false);
  }

  //-----------------------------
  void CMAlgoPolyShortestDist::Reset()
  //-----------------------------
  {

  }

  //------------------------------------------------------------------------------------------
  void CMAlgoPolyShortestDist::EventBegin(const std::vector<cluster::ClusterParamsAlgNew> &clusters)
  //------------------------------------------------------------------------------------------
  {
    tmp_min_dist = 99999;
  }

  //-------------------------------
  void CMAlgoPolyShortestDist::EventEnd()
  //-------------------------------
  {

  }

  //-----------------------------------------------------------------------------------------------
  void CMAlgoPolyShortestDist::IterationBegin(const std::vector<cluster::ClusterParamsAlgNew> &clusters)
  //-----------------------------------------------------------------------------------------------
  {

  }

  //------------------------------------
  void CMAlgoPolyShortestDist::IterationEnd()
  //------------------------------------
  {

  }
  
  //----------------------------------------------------------------
  bool CMAlgoPolyShortestDist::Bool(const ClusterParamsAlgNew &cluster1,
			       const ClusterParamsAlgNew &cluster2)
  //----------------------------------------------------------------
  {
    if( (cluster1.GetParams().N_Hits < _min_hits) ||
	(cluster2.GetParams().N_Hits < _min_hits) )
      return false;

    //if either has < 3 sides do not merge!
    if ( (cluster1.GetParams().PolyObject.Size() < 2) or
	 (cluster2.GetParams().PolyObject.Size() < 2) ){
      return false;
    }

    //loop over the points on the first polygon and calculate
    //distance to each point on the second polygon
    //if any two points are close enough to each other,
    //merge the two clusters

    unsigned int npoints1 = cluster1.GetParams().PolyObject.Size();
    unsigned int npoints2 = cluster2.GetParams().PolyObject.Size();
    //loop over points on first polygon
    for(unsigned int i = 0; i < npoints1; ++i){
      float pt1w = cluster1.GetParams().PolyObject.Point(i).first;
      float pt1t = cluster1.GetParams().PolyObject.Point(i).second;
      //loop over points on second polygon
      for(unsigned int j = 0; j < npoints2; ++j){
	float pt2w = cluster2.GetParams().PolyObject.Point(j).first;
	float pt2t = cluster2.GetParams().PolyObject.Point(j).second;
	double distsqrd = pow(pt2w-pt1w,2)+pow(pt2t-pt1t,2);
	
	if(distsqrd < tmp_min_dist) tmp_min_dist = distsqrd;

	if(_debug){
	  std::cout<<"two polygon points dist2 is "<<distsqrd<<std::endl;
	  std::cout<<"minimum dist was "<<tmp_min_dist<<std::endl;
	}
	if(distsqrd<_dist_sqrd_cut)
	  return true;
      }

    }

    return false;
  }

  //------------------------------
  void CMAlgoPolyShortestDist::Report()
  //------------------------------
  {

  }
    
}
#endif
