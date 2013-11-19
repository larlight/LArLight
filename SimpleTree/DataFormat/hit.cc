#ifndef HIT_CC
#define HIT_CC

#include "hit.hh"

//#####################################################################
hit::hit(DATA::DATA_TYPE type) : data_base(type)
//#####################################################################
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

//#####################################################################
void hit::clear_event(bool all)
//#####################################################################
{
  // Clear data_base variables
  data_base::clear_event(all);

  for(UShort_t index = 0;
      (index < _num_hits) || (all && index<DATA::kMaxHits);
      ++index) {
    _plane[index]        = 0;
    _wire[index]         = 0;
    _channel[index]      = 0;
    _startT[index]       = 0;
    _peakT[index]        = 0;
    _endT[index]         = 0;
    _startT_sigma[index] = 0;
    _peakT_sigma[index]  = 0;
    _endT_sigma[index]   = 0;
    _charge[index]       = 0;
    _charge_sigma[index] = 0;
    _trackID[index]      = 0;
  }
  _num_hits = 0;
}

//#####################################################################
void hit::add_hit(UChar_t  plane,  UShort_t wire,   UShort_t channel,
		  Double_t startT, Double_t peakT,  Double_t endT,
		  Double_t startT_sigma, Double_t peakT_sigma,  Double_t endT_sigma,
		  Double_t charge, Double_t charge_sigma,
		  UShort_t trackID)
//#####################################################################
{
  _plane[_num_hits]        = plane;
  _wire[_num_hits]         = wire;
  _channel[_num_hits]      = channel;
  _startT[_num_hits]       = startT;
  _peakT[_num_hits]        = peakT;
  _endT[_num_hits]         = endT;
  _startT_sigma[_num_hits] = startT_sigma;
  _peakT_sigma[_num_hits]  = peakT_sigma;
  _endT_sigma[_num_hits]   = endT_sigma;
  _charge[_num_hits]       = charge;
  _charge_sigma[_num_hits] = charge_sigma;
  _trackID[_num_hits]      = trackID;
  _num_hits++;
}

//#####################################################################
void hit::set_address(TTree* t)
//#####################################################################
{

  //Set address of the data_base variables
  data_base::set_address(t);

  if(t->GetBranch(Form("num_%s",_name.c_str()))) t->SetBranchAddress(Form("num_%s",_name.c_str()),&_num_hits);
  else t->Branch(Form("num_%s",_name.c_str()),&_num_hits,Form("num_%s/s",_name.c_str()));

  if(t->GetBranch(Form("%s_plane",_name.c_str()))) t->SetBranchAddress(Form("%s_plane",_name.c_str()),_plane);
  else t->Branch(Form("%s_plane",_name.c_str()),_plane,Form("%s_plane[num_hits]/b",_name.c_str()));

  if(t->GetBranch(Form("%s_wire",_name.c_str()))) t->SetBranchAddress(Form("%s_wire",_name.c_str()),_wire);
  else t->Branch(Form("%s_wire",_name.c_str()),_wire,Form("%s_wire[num_hits]/s",_name.c_str()));

  if(t->GetBranch(Form("%s_channel",_name.c_str()))) t->SetBranchAddress(Form("%s_channel",_name.c_str()),_channel);
  else t->Branch(Form("%s_channel",_name.c_str()),_channel,Form("%s_channel[num_hits]/s",_name.c_str()));

  if(t->GetBranch(Form("%s_startT",_name.c_str()))) t->SetBranchAddress(Form("%s_startT",_name.c_str()),_startT);
  else t->Branch(Form("%s_startT",_name.c_str()),_startT,Form("%s_startT[num_hits]/D",_name.c_str()));

  if(t->GetBranch(Form("%s_peakT",_name.c_str()))) t->SetBranchAddress(Form("%s_peakT",_name.c_str()),_peakT);
  else t->Branch(Form("%s_peakT",_name.c_str()),_peakT,Form("%s_peakT[num_hits]/D",_name.c_str()));

  if(t->GetBranch(Form("%s_endT",_name.c_str()))) t->SetBranchAddress(Form("%s_endT",_name.c_str()),_endT);
  else t->Branch(Form("%s_endT",_name.c_str()),_endT,Form("%s_endT[num_hits]/D",_name.c_str()));

  if(t->GetBranch(Form("%s_startT_sigma",_name.c_str()))) t->SetBranchAddress(Form("%s_startT_sigma",_name.c_str()),_startT_sigma);
  else t->Branch(Form("%s_startT_sigma",_name.c_str()),_startT_sigma,Form("%s_startT_sigma[num_hits]/D",_name.c_str()));

  if(t->GetBranch(Form("%s_peakT_sigma",_name.c_str()))) t->SetBranchAddress(Form("%s_peakT_sigma",_name.c_str()),_peakT_sigma);
  else t->Branch(Form("%s_peakT_sigma",_name.c_str()),_peakT_sigma,Form("%s_peakT_sigma[num_hits]/D",_name.c_str()));

  if(t->GetBranch(Form("%s_endT_sigma",_name.c_str()))) t->SetBranchAddress(Form("%s_endT_sigma",_name.c_str()),_endT_sigma);
  else t->Branch(Form("%s_endT_sigma",_name.c_str()),_endT_sigma,Form("%s_endT_sigma[num_hits]/D",_name.c_str()));

  if(t->GetBranch(Form("%s_charge",_name.c_str()))) t->SetBranchAddress(Form("%s_charge",_name.c_str()),_charge);
  else t->Branch(Form("%s_charge",_name.c_str()),_charge,Form("%s_charge[num_hits]/D",_name.c_str()));

  if(t->GetBranch(Form("%s_charge_sigma",_name.c_str()))) t->SetBranchAddress(Form("%s_charge_sigma",_name.c_str()),_charge_sigma);
  else t->Branch(Form("%s_charge_sigma",_name.c_str()),_charge_sigma,Form("%s_charge_sigma[num_hits]/D",_name.c_str()));

  if(t->GetBranch(Form("%s_trackID",_name.c_str()))) t->SetBranchAddress(Form("%s_trackID",_name.c_str()),_trackID);
  else t->Branch(Form("%s_trackID",_name.c_str()),_trackID,Form("%s_trackID[num_hits]/s",_name.c_str()));

}

#endif
