#ifndef HIT_CC
#define HIT_CC

#include "hit.hh"

hit::hit(DATA::DATA_TYPE type) : data_base(type)
{
  if(type!=DATA::Hit && 
     type!=DATA::FFTHit &&
     type!=DATA::APAHit)
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


void hit::clear_event()
{

}

void hit::set_address(TTree* t)
{

  if(t->GetBranch("no_hits")) t->SetBranchAddress("no_hits",&_no_hits);
  else t->Branch("no_hits",&_no_hits,"no_hits/I");

  if(t->GetBranch("hit_plane")) t->SetBranchAddress("hit_plane",_hit_plane);
  else t->Branch("hit_plane",_hit_plane,"hit_plane[no_hits]/I");

  if(t->GetBranch("hit_wire")) t->SetBranchAddress("hit_wire",_hit_wire);
  else t->Branch("hit_wire",_hit_wire,"hit_wire[no_hits]/I");

  if(t->GetBranch("hit_channel")) t->SetBranchAddress("hit_channel",_hit_channel);
  else t->Branch("hit_channel",_hit_channel,"hit_channel[no_hits]/I");

  if(t->GetBranch("hit_peakT")) t->SetBranchAddress("hit_peakT",_hit_peakT);
  else t->Branch("hit_peakT",_hit_peakT,"hit_peakT[no_hits]/D");

  if(t->GetBranch("hit_charge")) t->SetBranchAddress("hit_charge",_hit_charge);
  else t->Branch("hit_charge",_hit_charge,"hit_charge[no_hits]/D");

  if(t->GetBranch("hit_ph")) t->SetBranchAddress("hit_ph",_hit_ph);
  else t->Branch("hit_ph",_hit_ph,"hit_ph[no_hits]/D");

  if(t->GetBranch("hit_trkid")) t->SetBranchAddress("hit_trkid",_hit_trkid);
  else t->Branch("hit_trkid",_hit_trkid,"hit_trkid[no_hits]/I");

}

#endif
