#ifndef STREE_BASE_CC
#define STREE_BASE_CC

#include "stree_base.hh"

stree_base::stree_base()
{
  _type = DATA::DATA_TYPE_MAX;
  _tree = 0;
  clear_event();
}

void stree_base::clear_event()
{
  _entry=-1;
}

void stree_base::initialize()
{
  if(_tree) delete _tree;
  _tree = 0;
  clear_event();
}

void stree_base::read_tree(TChain* ch)
{
  ch->SetBranchAddress("_entry",&_entry);
}

TTree* stree_base::write_tree()
{
  if(_tree) return _tree;

  // Create a new TTree if not existing
  _tree =  new TTree(Form("%s_tree",DATA::DATA_TREE_NAME[_type].c_str()),
		     Form("%s Tree",DATA::DATA_TREE_NAME[_type].c_str()));
  // Create branch
  _tree->Branch("_entry",&_entry,"_entry/I");
  
  return _tree;
}



#endif
