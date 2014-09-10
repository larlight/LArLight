#ifndef NCFILTER_CC
#define NCFILTER_CC

#include "NCfilter.hh"
#include "ClusterParamsAlg.hh"
#include "LArUtilBase.hh"


namespace larlight {

  bool NCfilter::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D"). 
    //


    return true;
  }
  
  bool NCfilter::analyze(storage_manager* storage) {
        
    // grab the incoming clusters. 
    // these should be clusters that are very primative. like DB or hough
    auto Incoming_cluster = (const event_cluster*)(storage->get_data(DATA::DBCluster));
    auto const& hit_type = Incoming_cluster->get_hit_type();
    auto hits = (const event_hit*)(storage->get_data(hit_type));
    
    if(!hits || !Incoming_cluster) {
      print(MSG::ERROR,__FUNCTION__,"No DBCluster or associated hits found!");
      throw std::exception();
      return false;
    }

    // First of all create an output
    // make the output cluster
    auto Output_cluster = (event_cluster*)(storage->get_data(DATA::RyanCluster));
    
    // set event variables
    if(Output_cluster->size())
      print(MSG::WARNING,__FUNCTION__,"DATA::RyanCluster is not empty. Clearing it...");
    Output_cluster->clear_data();
    Output_cluster->set_event_id(Incoming_cluster->event_id());
    Output_cluster->set_run(Incoming_cluster->run());
    Output_cluster->set_subrun(Incoming_cluster->subrun());

    if(!(Incoming_cluster->size())){
      print(MSG::WARNING,__FUNCTION__,Form("Event %d has no DBCluster...",Incoming_cluster->event_id()));
      return true;
    }
    else if(!(hits->size())){
      print(MSG::ERROR,__FUNCTION__,Form("Event %d has no hits (but there's DBCluster!!!)",Incoming_cluster->event_id()));
      return false;
    }

    // Need some bool to check about the quality of the protoclusters
    // need some flags to see which planes to pursue  clustering in
    unsigned int nplanes = larutil::Geometry::GetME()->Nplanes();
    std::vector<std::pair<double,double>> AvgPairSI(nplanes);	
    std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>> BestClusters(nplanes);
    std::vector<bool> FlagGoodPlanes(nplanes,false);
    


//============Starting with  first pass alg============
// make a vector of clusters
	std::vector<larlight::cluster> clustervect;
    for(auto const& c : *Incoming_cluster) {
	clustervect.push_back(c);
	}
	std::vector<larlight::hit> hitsvect;
    for(auto const& h : *hits) {
	hitsvect.push_back(h);
	}
	AvgPairSI = fDivReg.SplitLineC(clustervect, hitsvect);
//====================Ending with alg===================

	

//=======================
//=== make the regions===
//=======================
   std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>>  FirstRegions = fForceRegions.ForceTwoRegions(hitsvect, AvgPairSI);
    std::cout<< "\033[94m lets see what things do  "<< FirstRegions[0].first.size()<<"  , \033[00m"<<FirstRegions[0].second.size()<<std::endl;
    std::cout<< "\033[95m lets see what things do  "<< FirstRegions[1].first.size()<<"  , \033[00m"<<FirstRegions[1].second.size()<<std::endl;
    std::cout<< "\033[96m lets see what things do  "<< FirstRegions[2].first.size()<<"  , \033[00m"<<FirstRegions[2].second.size()<<std::endl;
//=======================

//=============================
//=== Try to clean up region===
//=============================
//$$$$ need to impliment a look back function
//=======================

//=======================
//=== Check Bools========
//=======================
// do the time prof test...
	std::vector<bool> QualFirst = fPlaneCheckMatch.PlaneCheck( FirstRegions, hits);
	// the output is a vector with relations Q01,Q02,Q12
		std::vector<bool> flip(3,false);
		if(QualFirst[0]) {FlagGoodPlanes[0] = true; FlagGoodPlanes[1] = true;}
		if(QualFirst[1]) {FlagGoodPlanes[0] = true; FlagGoodPlanes[2] = true;}
		if(QualFirst[2]) {FlagGoodPlanes[1] = true; FlagGoodPlanes[2] = true;}
// also fill in a check for the protovertex
//$$$$ need to impliment The Protovertex 

//---------------------------------------------------------------------------------
// Check to see what we need to do next. Should we try another alg or keep theses? 
//---------------------------------------------------------------------------------
		for(unsigned int a = 0 ;a < nplanes;a++) if(!FlagGoodPlanes[a]) flip[a]= true;
		std::cout<<"\t Status of Plane Bools after first pass : "<<FlagGoodPlanes[0]<<FlagGoodPlanes[1]<<FlagGoodPlanes[2]<<std::endl;
		std::cout<<"\t Status of Flip Bools after first pass : "<<flip[0]<<flip[1]<<flip[2]<<std::endl;
//---------- If the plane bools are true then fill into the final cluster----------
		for(unsigned int b = 0 ;b < nplanes;b++) if(FlagGoodPlanes[b]) BestClusters[b]= FirstRegions[b];

	



// check the rough vertex
	std::vector<std::pair<double,double>> RoughVertex =fQuality.ProtoVertexCW(FirstRegions,hits);
	for(unsigned int k = 0 ; k<nplanes;k++)std::cout<< "This is the Protovertex0 :\t " <<RoughVertex[k].first<<" , "<<RoughVertex[k].second<<std::endl;
	
	/*
	/////temp just not trying this... 
	std::pair<double,double> RoughVertex0 =fQuality.ProtoVertexCW(pxfa_v0,pxfb_v0);
	std::pair<double,double> RoughVertex1 =fQuality.ProtoVertexCW(pxfa_v1,pxfb_v1);
	std::pair<double,double> RoughVertex2 =fQuality.ProtoVertexCW(pxfa_v2,pxfb_v2);
	// if we get a rough vertex of -999 -999 then that is not good. 
	std::cout<< "This is the Protovertex0 :\t " <<RoughVertex0.first<<" , "<<RoughVertex0.second<<std::endl;
	std::cout<< "This is the Protovertex1 :\t " <<RoughVertex1.first<<" , "<<RoughVertex1.second<<std::endl;
	std::cout<< "This is the Protovertex2 :\t " <<RoughVertex2.first<<" , "<<RoughVertex2.second<<std::endl;
*/


//-----------------------------------------------------------------------------------------------------------------------------------


//====================================================
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//============Starting with  flip pass alg============
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//====================================================
// if not all the plane bools are true then do some more work
for( unsigned int i = 0 ; i<flip.size();i++)
{
	if(flip[i]){
		std::vector<std::pair<double, double> > FlipAvgPairSI = fDivReg.FlipLineC(clustervect, hitsvect);
   		std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>>  FlipRegions = fForceRegions.ForceTwoFlipRegions(hitsvect, FlipAvgPairSI);
	// This being in the if will prevent overwriting the stuff that is already good.
		BestClusters[i] = FlipRegions[i];
	}// end of if flip[i]
}// for loop over all the flip stuff
//=======================
//=== Check Bools========
//=======================
// do the time prof test...
	std::vector<bool> QualFlip = fPlaneCheckMatch.PlaneCheck(BestClusters, hits);
///---------------------------------------------------------------------------------
// Check to see what we need to do next after the Rotate. Should we try another alg or keep theses? 
//---------------------------------------------------------------------------------
		std::vector<bool> RotateAlg(3,false);
                if(QualFlip[0]) {FlagGoodPlanes[0] = true; FlagGoodPlanes[1] = true;}
                if(QualFlip[1]) {FlagGoodPlanes[0] = true; FlagGoodPlanes[2] = true;}
                if(QualFlip[2]) {FlagGoodPlanes[1] = true; FlagGoodPlanes[2] = true;}
        	for(unsigned int a = 0 ;a < nplanes;a++) if(!FlagGoodPlanes[a]) RotateAlg[a]= true;
		std::cout<<"Status: Go to Rotate alg? : "<<RotateAlg[0]<<RotateAlg[1]<<RotateAlg[2]<<std::endl;

	std::vector<std::pair<double,double>> RoughFlipVertex =fQuality.ProtoVertexCW(BestClusters,hits);
	for(unsigned int k = 0 ; k<nplanes;k++)std::cout<< "This is the ProtoFlipVertex :\t " <<RoughFlipVertex[k].first<<" , "<<RoughFlipVertex[k].second<<std::endl;









//-----------------------------------------------------------------------------------------------------------------------------------






//====================================================
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//============Starting with  Rotate alg===============
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//====================================================
// if not all the plane bools are true then do some more work
for( unsigned int i = 0 ; i<RotateAlg.size();i++)
{
	// This is not that efficient... I am doing the loop a few time... RG Come back and clean up
	if(RotateAlg[i]){
	std::vector<std::pair<double, double> > RotateAvgPairSI = fDivReg.RotateLineC(clustervect, hitsvect);
   	std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>>  RotateRegions = fForceRegions.ForceTwoRotateRegions(hitsvect, RotateAvgPairSI);
	// This being in the if will prevent overwriting the stuff that is already good.
//	std::cout<< "  /t QQQ THIS IS FOR DEBUG OF THE ROTATE : "<< RotateRegions[i].first.size()<<" , "<<RotateRegions[i].second.size()<<std::endl;
		BestClusters[i] = RotateRegions[i];
	}// end of if Rotate[i]
}// for loop over all the Rotate stuff
//====================Ending with alg===================

//=======================
//=== Check Bools========
//=== for Rotation ======
//=======================
// do the time prof test...
	std::vector<bool> QualRot = fPlaneCheckMatch.PlaneCheck(BestClusters, hits);
	// the output is a vector with relations Q01,Q02,Q12
///---------------------------------------------------------------------------------
// Check to see what we need to do next after the Rotate. Should we try another alg or keep theses? 
//---------------------------------------------------------------------------------
		std::vector<bool> NextAlg(3,false);
                if(QualRot[0]) {FlagGoodPlanes[0] = true; FlagGoodPlanes[1] = true;}
                if(QualRot[1]) {FlagGoodPlanes[0] = true; FlagGoodPlanes[2] = true;}
                if(QualRot[2]) {FlagGoodPlanes[1] = true; FlagGoodPlanes[2] = true;}
        	for(unsigned int a = 0 ;a < nplanes;a++) if(!FlagGoodPlanes[a]) NextAlg[a]= true;
		std::cout<<"Status: Go to next alg? : "<<NextAlg[0]<<NextAlg[1]<<NextAlg[2]<<std::endl;

	std::vector<std::pair<double,double>> RoughRotVertex =fQuality.ProtoVertexCW(BestClusters,hits);
	for(unsigned int k = 0 ; k<nplanes;k++)std::cout<< "This is the ProtoRotVertex :\t " <<RoughRotVertex[k].first<<" , "<<RoughRotVertex[k].second<<std::endl;












//===================================================================
//====== THIS IS THE END OF ALL THE ALGS ============================
//===================================================================
//---------------------------------------------------------------------------------
		std::cout<<"\t Status of Good Planes at End : "<<FlagGoodPlanes[0]<<FlagGoodPlanes[1]<<FlagGoodPlanes[2]<<std::endl;
	// if we just have one plane..... take the next best thing... This will have to be the last step
	//if
//---------------------------------------------------------------------------------

//===================================================================
//=======Lets fill these hits out into clusters.===================== 
//===================================================================
    for( unsigned int a=0 ; a<BestClusters.size();a++){
	// need a check to see if we did well or not
	if(FlagGoodPlanes[a]){
      ::larlight::cluster lite_cluster;
      std::vector<unsigned int> hit_ass;
      // Skip if there's no hit
      if(!(BestClusters[a].first.size()) || !(BestClusters[a].second.size())) continue;

      //
      // Save clusters
      //

      // Clear data products
      hit_ass.clear();
      hit_ass.reserve(BestClusters[a].first.size());
      lite_cluster.clear_data();

      // Make association
      for( auto const& hit : BestClusters[a].first) hit_ass.push_back(hit);

      // Add association
      lite_cluster.add_association(hit_type,hit_ass);

      // Add a cluster to the output
      Output_cluster->push_back(lite_cluster);

      // Clear data products
      hit_ass.clear();
      hit_ass.reserve(BestClusters[a].second.size());
      lite_cluster.clear_data();

      // Make association
      for( auto const& hit : BestClusters[a].second) hit_ass.push_back(hit);

      // Add association
      lite_cluster.add_association(hit_type,hit_ass);

      // Add a cluster to the output
      Output_cluster->push_back(lite_cluster);

	}// if FLagGoodPlanes[a]
    }// loop over Best Clusters

    return true;
  }

  bool NCfilter::finalize() {

    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
    //
    // Say you made a histogram pointer h1 to store. You can do this:
    //
    // if(_fout) { _fout->cd(); h1->Write(); }
    //
    // else 
    //   print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    //
  
    return true;
  }

//---------------------
// Adding some fuctions 
//---------------------

  //---------------------------------------------------------------------------------------------

/*
std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>>  NCfilter::CheckQuality(larlight::event_hit *const hits,std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>> protocluster, std::vector<bool> Flags){


        // First Check based on bool.... if there is already a bool? Might not need this in here... Should do it in the code to fill clusters
	fucnflag0= false;
	fucnflag1= false;
	fucnflag2= false;
	
	

        // Checking the time profile
        for( unsigned int a=0 ; a<protocluster.size();a++){


                for(unsigned int k =0 ; k<protocluster[a].first.size(); k++){


                                }// protocluster for loop over a first cluster  

                }// for loop over protocluster


	}// End of Check Quality Func

*/








}
#endif
