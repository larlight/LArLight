#ifndef MCPART_CC
#define MCPART_CC

#include "mcpart.hh"

//####################################################
mcpart::mcpart(DATA::DATA_TYPE type) : data_base(type)
//####################################################
{
  _name = DATA::DATA_TREE_NAME[type];
  initialize();
}

//####################################################
void clear_event(bool all=false)
//####################################################
{


}

//####################################################
void set_address(TTree* t)
//####################################################
{


}

#endif
