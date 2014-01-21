#ifndef CLUSTER_CC
#define CLUSTER_CC

#include "cluster.hh"

namespace larlight{

  //############################################################################
  cluster::cluster(DATA::DATA_TYPE type) : data_base(type)
  //############################################################################
  {
    if( _type != DATA::Cluster &&
	_type != DATA::FuzzyCluster &&
	_type != DATA::DBCluster &&
	_type != DATA::CrawlerCluster &&
	_type != DATA::HoughCluster &&
	_type != DATA::ShowerAngleCluster )

      {
	Message::send(MSG::ERROR,__FUNCTION__,
		      Form("Provided data type (%d) not a valid cluster data type! Setting it to DATA::Hit...",_type));
	
	_type = DATA::Cluster;
      }

    clear_data();
  }

  //############################################################################
  void cluster::get_axis_range(std::vector<Double_t> &chmax,
			       std::vector<Double_t> &chmin,
			       std::vector<Double_t> &wiremax, 
			       std::vector<Double_t> &wiremin, 
			       std::vector<Double_t> &timemax, 
			       std::vector<Double_t> &timemin) const
  //############################################################################
  {

    // Make sure input vector is of size wire plane with initial value -1 (if not yet set)
    chmax.resize((GEO::kW+1),-1);
    wiremax.resize((GEO::kW+1),-1);
    timemax.resize((GEO::kW+1),-1);
    chmin.resize((GEO::kW+1),-1);
    wiremin.resize((GEO::kW+1),-1);
    timemin.resize((GEO::kW+1),-1);

    //vtx is a vector of hits for one cluster?
    for(std::vector<larlight::hit>::const_iterator iter(fHitVector.begin());
	iter!=fHitVector.end();
	++iter) {

      GEO::View_t  view    =  (*iter).View();
      Double_t       wire    =  (Double_t)((*iter).Wire());
      Double_t       ch      =  (Double_t)((*iter).Channel());
      Double_t       tstart  =  (*iter).StartTime();
      Double_t       tend    =  (*iter).EndTime();

      if( wiremax[view] < 0 || wiremax[view] < wire )  wiremax[view] = wire;
      if( chmax[view]   < 0 || chmax[view]   < ch   )  chmax[view]   = ch;
      if( timemax[view] < 0 || timemax[view] < tend )  timemax[view] = tend;

      if( wiremin[view] < 0 || wiremin[view] > wire   )  wiremin[view] = wire;
      if( chmin[view]   < 0 || chmin[view]   > ch     )  chmin[view]   = ch;
      if( timemin[view] < 0 || timemin[view] > tstart )  timemin[view] = tstart;
      
    }

    return;
  }

  //####################################################################################
  event_cluster::event_cluster(DATA::DATA_TYPE type) : std::vector<larlight::cluster>(), 
						       data_base(type) 
  //####################################################################################
  {
    if( _type != DATA::Cluster &&
	_type != DATA::FuzzyCluster &&
	_type != DATA::DBCluster &&
	_type != DATA::CrawlerCluster &&
	_type != DATA::HoughCluster &&
	_type != DATA::ShowerAngleCluster )

      {
	Message::send(MSG::ERROR,__FUNCTION__,
		      Form("Provided data type (%d) not a valid cluster data type! Setting it to DATA::Hit...",_type));
	
	_type = DATA::Cluster;
      }

    clear_data();

  }

  //############################################################################
  void event_cluster::get_axis_range(std::vector<Double_t> &chmax,
				     std::vector<Double_t> &chmin,
				     std::vector<Double_t> &wiremax, 
				     std::vector<Double_t> &wiremin, 
				     std::vector<Double_t> &timemax, 
				     std::vector<Double_t> &timemin) const
  //############################################################################
  {
    //fCluster_v is a vector of all clusters in the event
    for(size_t i=0; i<this->size(); ++i) {

      (this->at(i)).get_axis_range(chmax,chmin,wiremax,wiremin,timemax,timemin);

    }

    return;
  }
}  
  
#endif
  
