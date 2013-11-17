#ifndef HIT_CC
#define HIT_CC

#include "hit.hh"

hit::hit(DATA::DATA_TYPE type) : data_base(type)
{
  if(type!=DATA::Hit && 
     type!=DATA::CrawlerHit &&
     type!=DATA::GausHit &&
     type!=DATA::FFTHit &&
     type!=DATA::APAHit &&
     type!=DATA::RFFHit )
    {
      print(MSG::ERROR,__FUNCTION__,
	    Form("Invalid data type for hit: %d ... setting default DATA::Hit type",type));
      _type = DATA::Hit;
    }
  else
    _type = type;

  _name = DATA::DATA_TREE_NAME[type];
  initialize();
}


void hit::clear_event(bool all)
{
  // Clear data_base variables
  data_base::clear_event(all);

  for(UShort_t index = 0;
      (index < _no_hits) || (all && index<DATA::kMaxHits);
      ++index) {
    _plane[index]   = 0;
    _wire[index]    = 0;
    _channel[index] = 0;
    _peakT[index]   = 0;
    _charge[index]  = 0;
    _ph[index]      = 0;
    _trackID[index] = 0;
  }
  _no_hits = 0;
}


void hit::add_hit(UChar_t  plane, UShort_t wire,   UShort_t channel,
		  Double_t peakT, Double_t charge, Double_t ph,
		  UShort_t trackID)
{
  _plane[_no_hits]   = plane;
  _wire[_no_hits]    = wire;
  _channel[_no_hits] = channel;
  _peakT[_no_hits]   = peakT;
  _charge[_no_hits]  = charge;
  _ph[_no_hits]      = ph;
  _trackID[_no_hits] = trackID;
  _no_hits++;
}

void hit::set_address(TTree* t)
{

  //Set address of the data_base variables
  data_base::set_address(t);

  if(t->GetBranch("no_hits")) t->SetBranchAddress("no_hits",&_no_hits);
  else t->Branch("no_hits",&_no_hits,"no_hits/s");

  if(t->GetBranch("hit_plane")) t->SetBranchAddress("hit_plane",_plane);
  else t->Branch("hit_plane",_plane,"hit_plane[no_hits]/b");

  if(t->GetBranch("hit_wire")) t->SetBranchAddress("hit_wire",_wire);
  else t->Branch("hit_wire",_wire,"hit_wire[no_hits]/s");

  if(t->GetBranch("hit_channel")) t->SetBranchAddress("hit_channel",_channel);
  else t->Branch("hit_channel",_channel,"hit_channel[no_hits]/s");

  if(t->GetBranch("hit_peakT")) t->SetBranchAddress("hit_peakT",_peakT);
  else t->Branch("hit_peakT",_peakT,"hit_peakT[no_hits]/D");

  if(t->GetBranch("hit_charge")) t->SetBranchAddress("hit_charge",_charge);
  else t->Branch("hit_charge",_charge,"hit_charge[no_hits]/D");

  if(t->GetBranch("hit_ph")) t->SetBranchAddress("hit_ph",_ph);
  else t->Branch("hit_ph",_ph,"hit_ph[no_hits]/D");

  if(t->GetBranch("hit_trackID")) t->SetBranchAddress("hit_trackID",_trackID);
  else t->Branch("hit_trackID",_trackID,"hit_trackID[no_hits]/s");

}

#endif
