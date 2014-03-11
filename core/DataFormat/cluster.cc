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
		      Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));
	
	_type = DATA::Cluster;
      }

    clear_data();
  }


  //####################################################################################
  event_cluster::event_cluster(DATA::DATA_TYPE type) : std::vector<larlight::cluster>(), 
						       event_base(type) 
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
		      Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));
	
	_type = DATA::Cluster;
      }

    clear_data();

  }

  DATA::DATA_TYPE cluster::get_hit_type() const
  {

    // Assume there's only 1 unique association
    
    if(size_association(DATA::Hit))        return DATA::Hit;
    if(size_association(DATA::FFTHit))     return DATA::FFTHit;
    if(size_association(DATA::CrawlerHit)) return DATA::CrawlerHit;
    if(size_association(DATA::APAHit))     return DATA::APAHit;
    else
      Message::send(MSG::ERROR,__FUNCTION__,"Did not find any associated hit!");
    return DATA::DATA_TYPE_MAX;

  }

  DATA::DATA_TYPE event_cluster::get_hit_type() const
  {
    
    if(this->size()) return this->at(0).get_hit_type();
    return DATA::DATA_TYPE_MAX;

  }

}  
  
#endif
  
