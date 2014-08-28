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
        auto const& hits = (event_hit*)(storage->get_data(hit_type));
	// make the output cluster
	auto Output_cluster = (event_cluster*)(storage->get_data(DATA::RyanCluster));

 	int nplanes = larutil::Geometry::GetME()->Nplanes();
	std::vector<std::pair<double,double>> AvgPairSI(nplanes);	
	// Need to get the average of the incomming clusters
	// Everything is in CM space already
	double S0 = 0;
	double S1 = 0;
	double S2 = 0;
	double I0 = 0;
	double I1 = 0;
	double I2 = 0;
	int count0 = 0;
	int count1 = 0;
	int count2 = 0;
	for(auto const& c : *Incoming_cluster) {
	    auto const& hit_index_array = c.association(hit_type);
		// only run when we have a cluster of hits greater than
		if(hit_index_array.size()>15){
		std::pair<double,double>qws= HighQSlope(hit_index_array, hits);
            	//std::cout<<qws.first<<"  ,   "<<qws.second<<std::endl;
			if(hits->at(hit_index_array[0]).View()==0){
			count0+=1;
			S0+=qws.first;
			I0+=qws.second;
			//std::cout<<" We have a 0 plane "<<std::endl;
			}// if view ==0
			if(hits->at(hit_index_array[0]).View()==1){
			count1+=1;
			S1+=qws.first;
			I1+=qws.second;
			//std::cout<<" We have a 1 plane "<<std::endl;
			}// if view ==1	
			if(hits->at(hit_index_array[0]).View()==2){
			count2+=1;
			S2+=qws.first;
			I2+=qws.second;
			//std::cout<<" We have a 2 plane "<<std::endl;
			}// if view ==2
		     }//  if hits are over 15
    	    }// for loop over the incoming clusters

	if(count0>0){
		std::pair<double,double>  t0(S0/count0,I0/count0);
		 AvgPairSI[0] = t0;}
	else{
	std::pair<double,double> n0(-999,-999);
	 AvgPairSI[0] = n0;}

	if(count1>0){
		std::pair<double,double>  t1(S1/count1,I1/count1);
		 AvgPairSI[1] = t1;}
	else{
	std::pair<double,double> n1(-999,-999);
	 AvgPairSI[1] = n1;}

	if(count2>0){
		std::pair<double,double>  t2(S2/count2,I2/count2);
		 AvgPairSI[2] = t2;}
	else{
	std::pair<double,double> n2(-999,-999);
	 AvgPairSI[2] = n2;}
std::cout<<" Size of avg pairs"<<AvgPairSI.size()<<std::endl;

 
	// Now that we know the average... let's try to force it into two regions
std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>>  blah = ForceRegions( hits, AvgPairSI);


std::cout<< "\033[94m lets see what things do  "<< blah[0].first.size()<<"  , \033[94m"<<blah[0].second.size()<<std::endl;
std::cout<< "\033[95m lets see what things do  "<< blah[1].first.size()<<"  , \033[95m"<<blah[1].second.size()<<std::endl;
std::cout<< "\033[96m lets see what things do  "<< blah[2].first.size()<<"  , \033[96m"<<blah[2].second.size()<<std::endl;
	

// Lets fill these hits out into clusters. o


      for( unsigned int a=0 ; a<blah.size();a++){
      ::larlight::cluster this_cluster;
      ::larlight::cluster that_cluster;
	// get the hits to assosiate
	
		for( auto const& hit : blah[a].first) {
      this_cluster.add_association(DATA::GausHit,std::vector<unsigned int>(1,hit));
			}
      Output_cluster->push_back(this_cluster);
		for( auto const& hit : blah[a].second) {
      that_cluster.add_association(DATA::GausHit,std::vector<unsigned int>(1,hit));
			}
      Output_cluster->push_back(that_cluster);
	}// loop over balh




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
std::pair<double,double>   NCfilter::HighQSlope(std::vector<unsigned int> hitindex ,larlight::event_hit *const hits){
	double T2CM = larutil::GeometryUtilities::GetME()->TimeToCm();	
	double W2CM = larutil::GeometryUtilities::GetME()->WireToCm();	
        double totcharge  = 0;
        double avgcharge = 0;
        double awiretime=  0;
        double awire =  0;
        double atime =  0;
        double awirewire = 0;
	int na=0;
for(auto const& hit_index : hitindex) {
                totcharge+=hits->at(hit_index).Charge();}
        avgcharge=totcharge/hitindex.size();

for(auto const& hit_index : hitindex) {
                if(hits->at(hit_index).Charge()>avgcharge){
                na+=1;
                awiretime += hits->at(hit_index).Wire() * hits->at(hit_index).PeakTime()*W2CM*T2CM;
                awire += hits->at(hit_index).Wire()*W2CM;
                atime += hits->at(hit_index).PeakTime()*T2CM;
                awirewire += hits->at(hit_index).Wire()*W2CM*hits->at(hit_index).Wire()*W2CM;
                                        }//if charge
                                   }//end of regiona for loop
	double aroughslope = (na * awiretime - awire * atime)/(na*awirewire-awire*awire);
	double aroughcept= atime/na - aroughslope *(awire/na);
	std::pair<double,double> rs(aroughslope,aroughcept);
	return rs;

	}//endl of QWtSlope func
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>>  NCfilter::ForceRegions(larlight::event_hit *const hits, std::vector<std::pair<double,double>> APP){

	double T2CM = larutil::GeometryUtilities::GetME()->TimeToCm();	
	double W2CM = larutil::GeometryUtilities::GetME()->WireToCm();	
	std::vector<unsigned int> startervector;
	std::pair<std::vector<unsigned int>,std::vector<unsigned int>> planepair(startervector,startervector);
	std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>> vectplanepair(3,planepair);
	// loop over all the hits. 	
	//for(auto const& hit : *hits) {
	for(unsigned int a=0;  a < hits->size();a++) {
//                std::cout<< "\033[92m"<<a<<std::endl;
	//which plane?
	//int cplane= hit.View();
	int cplane= hits->at(a).View();
  //              std::cout<< "\033[91m"<<APP[cplane].first<<std::endl;
	if(APP[cplane].first==-999||APP[cplane].second==-999){ std::cout<<" \033[98m GOT A -999"; continue;}
        double regionval= (-APP[cplane].first * hits->at(a).Wire()*W2CM+ hits->at(a).PeakTime()*T2CM - APP[cplane].second) / sqrt(APP[cplane].first*APP[cplane].first +1);
	// sort into proper places using viewPP)
        if(regionval>0.0000) vectplanepair[cplane].first.push_back(a);
        if(regionval<0.0000) vectplanepair[cplane].second.push_back(a);
	}// for loop over hit index




/*
	 if(hit->View()==plane){
        double regionval= (-slope * hit->WireID().Wire+ hit->PeakTime() - cept) / sqrt(slope*slope +1);
        if(regionval>0.0000){
                tra.push_back(hit);}
        if(regionval<0.0000){
                trb.push_back(hit);}
                }
        }
        std::pair<std::vector<art::Ptr<recob::Hit> >,std::vector<art::Ptr<recob::Hit> > > tr(tra,trb);
*/
	return vectplanepair;


	}// FOrce Regions







}
#endif
