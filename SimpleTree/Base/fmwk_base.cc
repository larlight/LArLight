#ifndef LARLIGHT_BASE_CC
#define LARLIGHT_BASE_CC

#include "fmwk_base.hh"

//#####################################################
void fmwk_base::set_verbosity(MSG::Level level)
//#####################################################
{

  _verbosity_level=level;

  for(size_t i=(size_t)(MSG::DEBUG); i<(size_t)(MSG::MSG_TYPE_MAX); ++i)
    _verbosity[i]=false;

  switch(level){
  case MSG::DEBUG:
    _verbosity[MSG::DEBUG]=true;
  case MSG::INFO:
    _verbosity[MSG::INFO]=true;
  case MSG::NORMAL:
    _verbosity[MSG::NORMAL]=true;
  case MSG::WARNING:
    _verbosity[MSG::WARNING]=true;
  case MSG::ERROR:
    _verbosity[MSG::ERROR]=true;
  case MSG::MSG_TYPE_MAX:
    break;
  }

}

#endif
