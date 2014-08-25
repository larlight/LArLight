#ifndef HIT_CC
#define HIT_CC

#include "hit.hh"

namespace larlight {

  //#################################################
  hit::hit(DATA::DATA_TYPE type) : data_base(type), 
				   fHitSignal() 
  //#################################################
  {
    if( _type != DATA::Hit &&
	_type != DATA::MCShowerHit &&
	_type != DATA::CrawlerHit &&
	_type != DATA::GausHit &&
	_type != DATA::APAHit &&
	_type != DATA::FFTHit &&
	_type != DATA::RFFHit ) {

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));
      
      _type = DATA::Hit;
    }
    clear_data();
  }

  //##########################################################################
  hit::hit(const hit &original) : data_base(original),
				  fHitSignal(original.fHitSignal),
				  fStartTime(original.fStartTime),
				  fSigmaStartTime(original.fSigmaStartTime),
				  fPeakTime(original.fPeakTime),
				  fSigmaPeakTime(original.fSigmaPeakTime),
				  fEndTime(original.fEndTime),
				  fSigmaEndTime(original.fSigmaEndTime),
				  fCharge(original.fCharge),
				  fSigmaCharge(original.fSigmaCharge),
				  fMaxCharge(original.fMaxCharge),
				  fSigmaMaxCharge(original.fSigmaMaxCharge),
				  fMultiplicity(original.fMultiplicity),
				  fGoodnessOfFit(original.fGoodnessOfFit),
				  fView(original.fView),
				  fSignalType(original.fSignalType),
                                  fChannel(original.fChannel),
                                  fWire(original.fWire)
  //##########################################################################
  {};
  
  //##########################################################################
  void hit::clear_data()
  //##########################################################################
  {
    data_base::clear_data();
    fHitSignal.clear();
    fStartTime = fPeakTime = fEndTime = -1;
    fSigmaStartTime = fSigmaPeakTime = fSigmaEndTime = -1;
    fCharge = fMaxCharge = -1;
    fSigmaCharge = fSigmaMaxCharge = -1;
    fMultiplicity = -1;
    fGoodnessOfFit = -1;
    fView = GEO::kUnknown;
    fSignalType = GEO::kMysteryType;
    fChannel = 0xffffffff;
    fWire    = 0xffffffff;
  }

  //#################################################################################
  event_hit::event_hit(DATA::DATA_TYPE type) : std::vector<larlight::hit>(), 
					       event_base(type) 
  //#################################################################################
  {
    if( _type != DATA::Hit &&
	_type != DATA::MCShowerHit &&
	_type != DATA::CrawlerHit &&
	_type != DATA::GausHit &&
	_type != DATA::APAHit &&
	_type != DATA::FFTHit &&
	_type != DATA::RFFHit ) {

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));
      
      _type = DATA::Hit;
    }

    clear_data();

  }


  //############################################################################
  void event_hit::get_axis_range(std::vector<Double_t> &chmax,
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
    for(event_hit::const_iterator iter(this->begin()); iter!=this->end(); ++iter) {

      GEO::View_t  view    =  (*iter).View();
      Double_t     wire    =  (Double_t)((*iter).Wire());
      Double_t     ch      =  (Double_t)((*iter).Channel());
      Double_t     tstart  =  (*iter).StartTime();
      Double_t     tend    =  (*iter).EndTime();

      if( wiremax[view] < 0 || wiremax[view] < wire )  wiremax[view] = wire;
      if( chmax[view]   < 0 || chmax[view]   < ch   )  chmax[view]   = ch;
      if( timemax[view] < 0 || timemax[view] < tend )  timemax[view] = tend;

      if( wiremin[view] < 0 || wiremin[view] > wire   )  wiremin[view] = wire;
      if( chmin[view]   < 0 || chmin[view]   > ch     )  chmin[view]   = ch;
      if( timemin[view] < 0 || timemin[view] > tstart )  timemin[view] = tstart;
      
    }

    return;
  }

  //############################################################################
  void event_hit::get_axis_range(std::vector<Double_t> &chmax,
				 std::vector<Double_t> &chmin,
				 std::vector<Double_t> &wiremax, 
				 std::vector<Double_t> &wiremin, 
				 std::vector<Double_t> &timemax, 
				 std::vector<Double_t> &timemin,
				 const std::vector<unsigned short>& hit_index) const
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
    for(std::vector<unsigned short>::const_iterator iter(hit_index.begin());
	iter!=hit_index.end();
	++iter) {
      
      unsigned short index = (*iter);

      if( index < this->size() ) {

	GEO::View_t  view    =  (this->at(index)).View();
	Double_t     wire    =  (Double_t)((this->at(index)).Wire());
	Double_t     ch      =  (Double_t)((this->at(index)).Channel());
	Double_t     tstart  =  (this->at(index)).StartTime();
	Double_t     tend    =  (this->at(index)).EndTime();
	
	if( wiremax[view] < 0 || wiremax[view] < wire )  wiremax[view] = wire;
	if( chmax[view]   < 0 || chmax[view]   < ch   )  chmax[view]   = ch;
	if( timemax[view] < 0 || timemax[view] < tend )  timemax[view] = tend;
	
	if( wiremin[view] < 0 || wiremin[view] > wire   )  wiremin[view] = wire;
	if( chmin[view]   < 0 || chmin[view]   > ch     )  chmin[view]   = ch;
	if( timemin[view] < 0 || timemin[view] > tstart )  timemin[view] = tstart;
      }else{
	
	Message::send(MSG::ERROR,__FUNCTION__,
		      Form("Hit index %d does not exist!",index));
	continue;
      }
    }

    return;
  }

}
#endif
  
