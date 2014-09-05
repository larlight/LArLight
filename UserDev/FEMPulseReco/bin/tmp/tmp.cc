#ifndef ALGO_CLASS_NAME_CC
#define ALGO_CLASS_NAME_CC

#include "Algo_Class_Name.hh"

namespace optreco {

  //**********************************************************
  Algo_Class_Name::Algo_Class_Name() : preco_algo_base() {
  //**********************************************************

    _name = "Algo_Class_Name";

    reset();

  }

  void Algo_Class_Name::reset(){

    preco_algo_base::reset();

  }

  //*******************************************************************
  bool Algo_Class_Name::reco(const std::vector<unsigned short> &wf) {
  //*******************************************************************

    this->reset();

    return true;

  }
}
#endif
