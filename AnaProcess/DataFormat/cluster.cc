#ifndef CLUSTER_CC
#define CLUSTER_CC

#include "cluster.hh"

//############################################################################
void cluster::get_axis_range(double &xmax, double &xmin,
			     double &ymax, double &ymin,
			     double &zmax, double &zmin) const
//############################################################################
{
  /*
  //vtx is a vector of hits for one cluster?
  for(const auto &vtx : fHitVector) {

    if(vtx[0] > xmax) xmax = vtx[0];
    if(vtx[0] < xmin) xmin = vtx[0];
    if(vtx[1] > ymax) ymax = vtx[1];
    if(vtx[1] < ymin) ymin = vtx[1];
    if(vtx[2] > zmax) zmax = vtx[2];
    if(vtx[2] < zmin) zmin = vtx[2];
    
  }
  */
  return;
}

//############################################################################
void event_cluster::get_axis_range(double &xmax, double &xmin,
				   double &ymax, double &ymin,
				   double &zmax, double &zmin) const
//############################################################################
{
  /*
  for(auto clus : fCluster_v)

    clus.get_axis_range(xmax,xmin,ymax,ymin,zmax,zmin);
  */
  return;
}


#endif
