#ifndef DATA_BASE_CC
#define DATA_BASE_CC

#include "data_base.hh"

data_base::data_base(DATA::DATA_TYPE type) : fmwk_base()
{
  _name  = DATA::DATA_TREE_NAME[type];
  _type  = type;
  initialize();
}

void data_base::initialize()
{
  clear_event();
}

void data_base::clear_event()
{
  _entry=-1;
}

void data_base::set_address(TTree* t)
{
  if(t->GetBranch("_entry")) t->SetBranchAddress("_entry",&_entry);
  else t->Branch("entry",&_entry,"entry/I");
}

#endif
