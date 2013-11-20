#ifndef PMTFIFO_CC
#define PMTFIFO_CC

#include "pmtfifo.hh"

pmtfifo::pmtfifo() : data_base(DATA::FIFOChannel)
{
  _name=DATA::TREE_NAME;
  initialize();
}

void pmtfifo::clear_event(bool all)
{

  // Clear data_base data
  data_base::clear_event(all);
  
  // Clear track data
  for(UShort_t index = 0;
      (index < _num_pmtfifo) || (all && index<DATA::kMaxPMTFIFO);
      ++index) {

    _channel[index] = 0;
    _discID[index]  = 0; 
    _frame[index]   = 0;  
    _sample[index]  = 0; 
    _index[index]   = 0;  
    _size[index]    = 0;   

  }

  for(UInt_t index = 0;
      (index < _num_pmtfifo) || (all && index<DATA::kMaxPMTWords);
      ++index) 

	_adc[index] = 0;

  _num_pmtfifo=0;
  _num_adc=0;
  
}

void pmtfifo::add_pmtfifo(UChar_t channel, UChar_t disc,
			  UInt_t  frame,   UInt_t  sample,
			  const std::vector<UShort_t> wf)
{
  _channel[_num_pmtfifo] = channel;
  _discID[_num_pmtfifo]    = disc;
  _frame[_num_pmtfifo]   = frame;
  _sample[_num_pmtfifo]  = sample;
  for(auto v : wf){
    _adc[_num_adc] = v;
    _num_adc++;
  }
  _num_pmtfifo++;
}

Bool_t pmtfifo::set_address(TTree* t,Bool_t create)
{
  /// set base class address
  Bool_t exist = set_address(t,create);
  
  if(t->GetBranch(Form("num_%sfifo",_name.c_str()))) t->SetBranchAddress(Form("num_%sfifo",_name.c_str()),&_num_pmtfifo);
  else {
    exist = false;
    if(create) t->Branch(Form("num_%sfifo",_name.c_str()),&_num_pmtfifo,Form("num_%sfifo/s",_name.c_str()));
  }

  if(t->GetBranch(Form("%s_channel",_name.c_str()))) t->SetBranchAddress(Form("%s_channel",_name.c_str()),_channel);
  else if(create) t->Branch(Form("%s_channel",_name.c_str()),_channel,Form("%s_channel[num_%sfifo]/b",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_discID",_name.c_str()))) t->SetBranchAddress(Form("%s_discID",_name.c_str()),_discID);
  else if(create) t->Branch(Form("%s_discID",_name.c_str()),_discID,Form("%s_discID[num_%sfifo]/b",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_frame",_name.c_str()))) t->SetBranchAddress(Form("%s_frame",_name.c_str()),_frame);
  else if(create) t->Branch(Form("%s_frame",_name.c_str()),_frame,Form("%s_frame[num_%sfifo]/b",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_sample",_name.c_str()))) t->SetBranchAddress(Form("%s_sample",_name.c_str()),_sample);
  else if(create) t->Branch(Form("%s_sample",_name.c_str()),_sample,Form("%s_sample[num_%sfifo]/b",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("num_%s_adc",_name.c_str()))) t->SetBranchAddress(Form("num_%s_adc",_name.c_str()),&_num_adc);
  else if(create) t->Branch(Form("num_%s_adc",_name.c_str()),&_num_adc,Form("num_%s_adc/i",_name.c_str()));

  if(t->GetBranch(Form("%s_adc",_name.c_str()))) t->SetBranchAddress(Form("%s_adc",_name.c_str()),_adc);
  else if(create) t->Branch(Form("%s_adc",_name.c_str()),_adc,Form("%s_adc[num_%sadc]/s",_name.c_str(),_name.c_str()));

  return exist;
}

#endif
