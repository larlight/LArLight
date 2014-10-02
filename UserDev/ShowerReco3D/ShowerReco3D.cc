#ifndef SHOWERRECO3D_CXX
#define SHOWERRECO3D_CXX

#include "ShowerReco3D.hh"

namespace larlight {

  ShowerReco3D::ShowerReco3D() : ana_base(), fMatchMgr(nullptr), fShowerAlgo(nullptr)
  {
    _name="ShowerReco3D";
    fClusterType = DATA::Cluster;

    auto geom = ::larutil::Geometry::GetME();
    if(!fMatchMgr) fMatchMgr = new ::cmtool::CMatchManager(geom->Nplanes());
  }
  
  bool ShowerReco3D::initialize() {
    _mgr = 0;
    if(!fShowerAlgo) {
      
      throw ::cluster::CRUException("Shower reco algorithm not attached... aborting.");
      return false;

    }
    // Make sure cluster type is a valid one
    if( fClusterType != DATA::Cluster &&
	fClusterType != DATA::MCShowerCluster &&
	fClusterType != DATA::DBCluster &&
        fClusterType != DATA::FuzzyCluster &&
	fClusterType != DATA::CrawlerCluster &&
	fClusterType != DATA::ShowerAngleCluster &&
	fClusterType != DATA::RyanCluster)

      throw ::cluster::CRUException(Form("Not supported cluster type: %s",
					 DATA::DATA_TREE_NAME[fClusterType].c_str())
				    );
    
    return true;
  }
  
  bool ShowerReco3D::analyze(storage_manager* storage) {

    _mgr = storage;
    // Re-initialize tools
    fShowerAlgo->Reset();
    fMatchMgr->Reset();

    // Retrieve clusters and fed into the algorithm
    std::vector<std::vector<larutil::PxHit> > local_clusters;

    fCRUHelper.GeneratePxHit(storage,fClusterType,local_clusters);
    
    fMatchMgr->SetClusters(local_clusters);

    local_clusters.clear();

//=============================================================================================================================================	
	// getting all the clusters
	const std::vector<::cluster::ClusterParamsAlg>& allclusters =  fMatchMgr->GetInputClusters();
	std::cout<< "Size of all the vector in the cluster... for the pi0 case this should divide by 2 :   Size ="<<allclusters.size();
	        // THIS IS GOING TO BE UGLY FOR NOW>>> WILL TAKE CARE LATER
        // first get the protovertex again 
        // first Just take the projected lines of the two clusters per each plane 
        // This can come from using the clusterparams algs start and end poing to get the protovertex in each plane
        std::vector<std::pair<double,double>> ProtoVertex(3);
        std::vector<std::vector<std::pair<std::pair<double,double>,std::pair<double,double>>>>  Hemlines(3);
        //^^^^^^^HAHAHAHAHA this need to go _RG
        // sort the clusters into the two planes
        // only do this on active planes 
        std::vector<bool> activeplane(3,false);
    for(auto const & cl : allclusters)
        {
        // this is still ugly... clean up later ... RG  
        std::pair<double,double> tempwta(cl.GetParams().start_point.w,cl.GetParams().start_point.t);
        std::pair<double,double> tempwtb(cl.GetParams().end_point.w,cl.GetParams().end_point.t);
        std::pair<std::pair<double,double>,std::pair<double,double>> tempabline(tempwta,tempwtb);
        Hemlines[cl.Plane()].push_back(tempabline);
        //SOOO UGLY!!!  
        activeplane[cl.Plane()] = true;
        }
        // now we have a mess 
        // keep calm
        std::vector<std::vector<std::pair<double,double>>>  slopeceptsbyplane(3);
        for( unsigned int k =0; k<3; k++){
        if(activeplane[k]){
                for(unsigned int z= 0; z<2 ; z++){
        double slope = 0;
        double cept = 0;
        double deltaw = Hemlines[k][z].first.first -Hemlines[k][z].second.first;
        double deltat = Hemlines[k][z].first.second -Hemlines[k][z].second.second;
        // will need to avoid inf slopes
        slope = deltat/deltaw;
        cept = Hemlines[k][z].first.second - (slope * Hemlines[k][z].first.first) ;
        std::pair<double,double> TempSC(slope,cept);
        slopeceptsbyplane[k].push_back(TempSC);
                }// looping over the lines in each plane
        }// if active plane
        }//     

        // now we should have a vector of planes.... each with a vector of pairs of slope and cepts ..... NEED TO BE NICER
        std::pair<double,double> BlankPair(-999,-999);
        std::vector<std::pair<double,double>> ProtoVertex2D(3,BlankPair);

        for(unsigned int z=0; z< 3; z++){
                // the pair format is pair<slope,cept>
                // need to find out when to skip/.... when we skip fill with -999
                if(activeplane[z]){
                double PVwire = -999;
                double PVtime = -999;
                PVwire = (slopeceptsbyplane[z][0].second-slopeceptsbyplane[z][1].second)/(slopeceptsbyplane[z][1].first-slopeceptsbyplane[z][0].first);
                PVtime = slopeceptsbyplane[z][0].first * PVwire + slopeceptsbyplane[z][0].second;
                std::pair<double,double> TempPV(PVwire,PVtime);
                ProtoVertex2D[z] = TempPV;
                }// if active plane
                }// for loop over the protovertex2d

std::cout<<"\t THESE ARE THE PROTOVERTEX : \n\t\t" << ProtoVertex2D[0].first<<" , "<<  ProtoVertex2D[0].second
                                        <<"\n\t\t" << ProtoVertex2D[1].first<<" , "<<  ProtoVertex2D[1].second
                                        <<"\n\t\t" << ProtoVertex2D[2].first<<" , "<<  ProtoVertex2D[2].second
                                        <<std::endl;
 




// now lets see how the start points did. for each cluster
	// if the plane is active then try checking the start points
	// needs to be fixed for the plane

	// This will be the new vector of clusters
	// it is not const yet because we want to change start point on some things
//	 std::vector<::cluster::ClusterParamsAlg> Pi0Clusters;
	
	for(auto const & cl : allclusters)
	{
		if(activeplane[cl.Plane()]){ 
		double SPDistFromPV = -999;
		double EPDistFromPV = -999;
		SPDistFromPV =sqrt( pow((ProtoVertex2D[cl.Plane()].first-cl.GetParams().start_point.w),2)+pow((ProtoVertex2D[cl.Plane()].second-cl.GetParams().start_point.t),2));
		EPDistFromPV =sqrt( pow((ProtoVertex2D[cl.Plane()].first-cl.GetParams().end_point.w),2)+pow((ProtoVertex2D[cl.Plane()].second-cl.GetParams().end_point.t),2) );
		std::cout<< " PLANE : "<<cl.Plane()<<std::endl;
		std::cout<<" THIS IS THE PROTO VERTEX "<< ProtoVertex2D[cl.Plane()].first<<" , "<<ProtoVertex2D[cl.Plane()].second<<std::endl;
			std::cout<<"Start Point "<<cl.GetParams().start_point.w << "  ,  " <<cl.GetParams().start_point.t<<" The Distance(cm) is : "<<SPDistFromPV<<std::endl;	
			std::cout<<"End Point "<<cl.GetParams().end_point.w << "  ,  " <<cl.GetParams().end_point.t<<" The Distance(cm) is : "<<EPDistFromPV<<std::endl;	

/*
		// make a temp cluster 
		::cluster::ClusterParamsAlg  tempcluster =cl.GetParams();
		// so basically if start point dist value is greater than end point... we need to switch and set values
		if(SPDistFromPV>EPDistFromPV){
		larutil::PxPoint currentep = cl.GetParams().end_point;
		larutil::PxPoint currentsp = cl.GetParams().start_point;
		tempcluster.set_start_point(currentep);
		tempcluster.set_end_point(currentsp);
		::cluster::ClusterParamsAlg tmpcluster = tempcluster;
		Pi0Clusters.push_back(tmpcluster);	
			}// if the sp is larger than then ep
		// then we need to push back. if it is changed of if not then just push back as is. 
		else Pi0Clusters.push_back(cl);	
	*/

		}// if active plane 	
	}// loop over all clusters again to correct things
 
//std::cout<< " SIZE OF THE PI0 CLUSTERS AFTER THE REWORKING " <<Pi0Clusters.size()<<std::endl;	

// if the start point is farther away from the PV need to switch and make a setter function. in cluster params alg 



//=============================================================================================================================================	

	
    // Run matching & retrieve matched cluster indices
    try{
      fMatchMgr->Process();
    }catch( ::cmtool::CMTException &e){
      e.what();
      return false;
    }

    auto const& matched_pairs = fMatchMgr->GetBookKeeper().GetResult();

    //
    // Run shower reco algorithm and store output
    //

    // Prepare output storage
    auto shower_v = (event_shower*)(storage->get_data(DATA::Shower));
    shower_v->clear();
    shower_v->reserve(matched_pairs.size());
    shower_v->set_event_id(storage->get_data(fClusterType)->event_id());
    shower_v->set_run(storage->get_data(fClusterType)->run());
    shower_v->set_subrun(storage->get_data(fClusterType)->subrun());
    
    // Loop over matched pairs
    for(auto const& pair : matched_pairs) {
      
      // Create a vector of clusters to be passed onto the algorithm
      std::vector< ::cluster::ClusterParamsAlg> clusters;
      clusters.reserve(pair.size());

      
            
      // Create an association vector
      std::vector<unsigned int> ass_index;
      ass_index.reserve(pair.size());

      for(auto const& cluster_index : pair) {

	ass_index.push_back(cluster_index);
	
	clusters.push_back( fMatchMgr->GetInputClusters().at(cluster_index) );
	
      }

      // Run algorithm
      //larlight::shower result = fShowerAlgo->Reconstruct(clusters);
	// passing along the direction info
      larlight::shower result = fShowerAlgo->ReconstructPi0(clusters,ProtoVertex2D);

      // Add association
      result.add_association(fClusterType,ass_index);

      // Set ID
      result.set_id(shower_v->size());

      // Store
      shower_v->push_back(result);

    } // done looping over matched cluster pairs

    return true;
  }
  
  bool ShowerReco3D::finalize() {

    return true;
  }
  
}
#endif
