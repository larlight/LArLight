#ifndef CMALGOSTARTTRACK_CC
#define CMALGOSTARTTRACK_CC

#include "CMAlgoStartTrack.hh"

namespace cluster {

  //-------------------------------------------------------
  CMAlgoStartTrack::CMAlgoStartTrack() : CBoolAlgoBase()
  //-------------------------------------------------------
  {
    
    SetMinHits(10);
    SetMinWidth(1.00000);
    SetMinOpeningAngle(0.15000);
    SetMinEP(0.99000);

  }

  //-----------------------------
  void CMAlgoStartTrack::Reset()
  //-----------------------------
  {

  }

  //------------------------------------------------------------------------------------------
  void CMAlgoStartTrack::EventBegin(const std::vector<cluster::ClusterParamsAlgNew> &clusters)
  //------------------------------------------------------------------------------------------
  {

  }

  //-------------------------------
  void CMAlgoStartTrack::EventEnd()
  //-------------------------------
  {

  }

  //-----------------------------------------------------------------------------------------------
  void CMAlgoStartTrack::IterationBegin(const std::vector<cluster::ClusterParamsAlgNew> &clusters)
  //-----------------------------------------------------------------------------------------------
  {

  }

  //------------------------------------
  void CMAlgoStartTrack::IterationEnd()
  //------------------------------------
  {

  }
  
  //----------------------------------------------------------------
  bool CMAlgoStartTrack::Bool(const ClusterParamsAlgNew &cluster1,
			       const ClusterParamsAlgNew &cluster2)
  //----------------------------------------------------------------
  {
    
    if(cluster1.GetParams().N_Hits < _min_hits ||
       cluster2.GetParams().N_Hits < _min_hits )
      return false;
    
    bool FirstTrackSecondBlob = false;
    bool SecondTrackFirstBlob = false;

    if(IsStartTrack(cluster1) && IsOverlappingBlob(cluster2))
      FirstTrackSecondBlob = true;
    if(IsStartTrack(cluster2) && IsOverlappingBlob(cluster1))
      SecondTrackFirstBlob = true;

    //if cluster1 looks like a track and cluster2 looks like a blob
    if (FirstTrackSecondBlob)
      {
	std::pair<float,float> trackEndPoint = 
	  std::pair<float,float>( cluster1.GetParams().end_point.w, cluster1.GetParams().end_point.t );

	//if the tracklike cluster's endpoint is inside polygon of blob
	if(cluster2.GetParams().PolyObject.PointInside(trackEndPoint))
	  return true;
	else
	  return false;
      }

    //if cluster2 looks like a track and cluster1 looks like a blob
    else if (SecondTrackFirstBlob)
      {
	std::pair<float,float> trackEndPoint = 
	  std::pair<float,float>( cluster2.GetParams().end_point.w, cluster2.GetParams().end_point.t );

	//if the tracklike cluster's endpoint is inside polygon of blob
	if(cluster1.GetParams().PolyObject.PointInside(trackEndPoint))
	  return true;
	else
	  return false;
      }

    else
      return false;
       
    //the reason for not just using (one track one blob) and 
    //polyoverlap for this is that 
    //this was merging BNB events where the track is clearly in a different
    //direction than the blob
    
    //this will fail when the tracklike cluster is reconstructed backwards
    //(looks like it's a track going into a blob, rather than exiting)
  
    
  }

  //------------------------------
  void CMAlgoStartTrack::Report()
  //------------------------------
  {

  }
    

  bool CMAlgoStartTrack::IsStartTrack(const ClusterParamsAlgNew &cluster)
  {
    if(cluster.GetParams().eigenvalue_principal > _min_EP) 
      return true;
    else
      return false;
     
  }

  bool CMAlgoStartTrack::IsOverlappingBlob(const ClusterParamsAlgNew &cluster)
  {
    if(cluster.GetParams().width > _min_width && 
       cluster.GetParams().opening_angle > _min_opening_angle &&
       cluster.GetParams().eigenvalue_principal < _min_EP)
      return true;
    else
      return false;
  }
}
#endif
