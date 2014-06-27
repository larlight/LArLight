#ifndef CFALGOZOVERLAP_CXX
#define CFALGOZOVERLAP_CXX

#include "CFAlgoZOverlap.hh"

namespace cmtool {

  //-------------------------------------------------------
  CFAlgoZOverlap::CFAlgoZOverlap() : CFloatAlgoBase()
  //-------------------------------------------------------
  {
		_wire_ratio_cut = 0.1 ;  	//Preliminary cuts
		_N_Hits_cut		= 20   ;
  }

  //-----------------------------
  void CFAlgoZOverlap::Reset()
  //-----------------------------
  {

  }

  //----------------------------------------------------------------------------------------------
  float CFAlgoZOverlap::Float(const std::vector<const cluster::ClusterParamsAlgNew*> &clusters)
  //----------------------------------------------------------------------------------------------
  {

	double wire_distance 		= 0;
	double ratio 				= 1;
	double max_wire_distance	= -1;
	//Recond the start/end points that retunr the maximum wire distance
	double max_start_w			= -1; 
	double max_end_w			= -1;
	//Record the plane that contains the maximum wire spacing
	int max_plane				= -1; 

	double start_w				= 0;
	double end_w				= 0;
	double switch_w				= 0;

	for(auto const& c : clusters){
		if(c->GetParams().N_Hits > _N_Hits_cut){

/*			if(c->Plane() !=2){
				start_w  	= 0.2* c->GetParams().start_point.w ;
				end_w		= 0.2* c->GetParams().end_point.w  ;
				wire_distance 	= end_w - start_w 	;
				}
			else {		*/	
				start_w  	=  c->GetParams().start_point.w ;
				end_w		=  c->GetParams().end_point.w  ;
				wire_distance   = c->GetParams().end_point.w - c->GetParams().start_point.w ;
//				}

			if(wire_distance < 0)
				wire_distance *= -1 ;
	
			if(max_wire_distance < wire_distance){
				max_wire_distance = wire_distance;
				max_plane 	=	c->Plane();		
				
				if(start_w < end_w){
					max_start_w =	start_w ;
					max_end_w	=	end_w ;
				   }
				else{
					max_start_w = end_w;
					max_end_w	= start_w; 
				   }
			   }	
			}
	 }


 	for(auto const& c : clusters){
		if(c->GetParams().N_Hits > _N_Hits_cut){
			
			start_w  	=  c->GetParams().start_point.w ;
			end_w		=  c->GetParams().end_point.w  ;
			wire_distance   = c->GetParams().end_point.w - c->GetParams().start_point.w ;
			
			if(wire_distance < 0){
				wire_distance *= -1 ;
				switch_w = start_w ;
				start_w = end_w ;
				end_w 	= switch_w;
				}
	
			if(start_w <= max_end_w) // && end_w+25 >=max_start_w )
 				ratio *= wire_distance / max_wire_distance ; 
			else 
				ratio *=0.01 ;	
	
			if(_verbose){
			std::cout<<"\nThe wire distance for cluster in plane "<<c->Plane()<<" is: "<<wire_distance <<std::endl;
			std::cout<<"Max wire disatance is: "<<max_wire_distance<<std::endl ;	
			std::cout<<"Ratio is: "<<ratio<<std::endl;
			std::cout<<"Start and end points: "<<c->GetParams().start_point.w<<",  "<<c->GetParams().end_point.w<<std::endl;
			}		
		  }	
		else
			ratio *=0.01 ;

		}
	std::cout<<"******** ** ** **** END OF CLUSTER PERMUTAITON **** ** ** ***********" <<std::endl;	
	if(ratio>_wire_ratio_cut)
		std::cout<<" FOOOOUUUUNNNND ONE WOooooooooooooooooooooooooooooooooooooooooooooooooo: "<<ratio<<std::endl;

	return(ratio > _wire_ratio_cut ? ratio : -1) ;

  }

  //------------------------------
  void CFAlgoZOverlap::Report()
  //------------------------------
  {

  }
    
}
#endif
