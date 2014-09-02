#ifndef DIVREG_CC
#define DIVREG_CC

#include "DivReg.hh"
//#include "ClusterParamsAlg.hh"
//#include "LArUtilBase.hh"


namespace cluster {

  std::vector<std::pair<double,double>> DivReg::SplitLine(const std::vector<std::vector<larutil::PxHit>>& hits) const
  {
// first 
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

    for(auto const& hitvect : hits) {
	// This is like the loop over clusters
      // only run when we have a cluster of hits greater than
      //auto const& hit_index_array = c.association(hit_type);
      if(hitvect.size()>15){
	// calculate slope and cept in cm
	// double T2CM = larutil::GeometryUtilities::GetME()->TimeToCm();
        //double W2CM = larutil::GeometryUtilities::GetME()->WireToCm();
        double totcharge  = 0;
        double avgcharge = 0;
        double awiretime=  0;
        double awire =  0;
        double atime =  0;
        double awirewire = 0;
        int na=0;
	//int currentplane = -999;
	int currentplane= hitvect[0].plane;

	for(auto const& thehit : hitvect) {
	     totcharge+=thehit.charge;}
        avgcharge=totcharge/hitvect.size();

	for(auto const& thehit : hitvect) {
                if(thehit.charge>avgcharge){
                na+=1;
                awiretime += thehit.w * thehit.t;
                awire += thehit.w;
                atime += thehit.t;
                awirewire += thehit.w * thehit.w;
                                        }//if charge
                                   }//end of loop over the hits
        double aroughslope = (na * awiretime - awire * atime)/(na*awirewire-awire*awire);
        double aroughcept= atime/na - aroughslope *(awire/na);
        std::pair<double,double> rs(aroughslope,aroughcept);
 

//std::cout<< "\033[92m This is from the function "<< currentplane<<"\033[90m"<<std::endl;
        if(currentplane==0){
          count0+=1;
          S0+=rs.first;
          I0+=rs.second;
          //std::cout<<" We have a 0 plane "<<std::endl;
        }// if view ==0
        if(currentplane==1){
          count1+=1;
          S1+=rs.first;
          I1+=rs.second;
          //std::cout<<" We have a 1 plane "<<std::endl;
        }// if view ==1 
        if(currentplane==2){
          count2+=1;
          S2+=rs.first;
          I2+=rs.second;
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



    return AvgPairSI;
  }










  std::vector<std::pair<double,double>> DivReg::SplitLineC(const std::vector<larlight::cluster>& clusters, const std::vector<larlight::hit>& hits) const
  {
// first 
    int nplanes = larutil::Geometry::GetME()->Nplanes();
    std::vector<std::pair<double,double>> AvgPairSI(nplanes);
    // calculate slope and cept in cm
    double T2CM = larutil::GeometryUtilities::GetME()->TimeToCm();
    double W2CM = larutil::GeometryUtilities::GetME()->WireToCm();
    double S0 = 0;
    double S1 = 0;
    double S2 = 0;
    double I0 = 0;
    double I1 = 0;
    double I2 = 0;
    int count0 = 0;
    int count1 = 0;
    int count2 = 0;
	// finding the hit type
   	 auto const& hit_type = clusters[0].get_hit_type();
    	//for(auto const& c : *clusters) {
    	for(unsigned int a= 0 ; a < clusters.size();a++) {
      	auto const& hit_index_array = clusters[a].association(hit_type);
      	// only run when we have a cluster of hits greater than
      	 	if(hit_index_array.size()>15){
        	// This can be an alg
		double totcharge  = 0;
       		double avgcharge = 0;
        	double awiretime=  0;
        	double awire =  0;
        	double atime =  0;
        	double awirewire = 0;
		int currentplane= -999;
        	int na=0;	
	// getting the average charge of the cluster and setting the current plane of this cluster
	for( auto const& thehit : hit_index_array){
		currentplane = hits[thehit].View();
		totcharge+=hits[thehit].Charge();}
	avgcharge=totcharge/hit_index_array.size();
	// now calculating the average
	for(auto const& thehit : hit_index_array) {
                if(hits[thehit].Charge()>avgcharge){
                na+=1;
                awiretime += hits[thehit].Wire()* W2CM * hits[thehit].PeakTime()*T2CM;
                awire += hits[thehit].Wire()*W2CM;
                atime += hits[thehit].PeakTime()*T2CM;
                awirewire += hits[thehit].Wire()*W2CM * hits[thehit].Wire()*W2CM;
						}//if charge
					   }//end of loop over the hits
        double aroughslope = (na * awiretime - awire * atime)/(na*awirewire-awire*awire);
        double aroughcept= atime/na - aroughslope *(awire/na);
        std::pair<double,double> rs(aroughslope,aroughcept);


	//put the cluster info in the proper plane
        if(currentplane==0){
          count0+=1;
          S0+=rs.first;
          I0+=rs.second;
        			}// if view ==0
        if(currentplane==1){
          count1+=1;
          S1+=rs.first;
          I1+=rs.second;
       				 }// if view ==1 
        if(currentplane==2){
          count2+=1;
          S2+=rs.first;
          I2+=rs.second;
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

	// return the info
    return AvgPairSI;
  }





}

#endif


