#ifndef ALGO_FIXED_WINDOW_CC
#define ALGO_FIXED_WINDOW_CC

#include "algo_fixed_window.hh"

//***************************************************************
algo_fixed_window::algo_fixed_window() : preco_algo_base() {
//***************************************************************

  _name = "algo_fixed_window";
  
  reset();

  _index_start = 0;

  _index_end = 0;

}

void algo_fixed_window::reset(){

  if(!(_pulse_v.size()))

    _pulse_v.push_back(_pulse);

  _pulse_v[0].reset_param();

}

//***************************************************************
bool algo_fixed_window::reco(const std::vector<UShort_t> *wf) {
//***************************************************************

  this->reset();

  _pulse_v[0].t_start = (double)(_index_start);

  if(!_index_end)

    _pulse_v[0].t_end = (double)(wf->size() - 1);

  else

    _pulse_v[0].t_end = (double)_index_end;

  _pulse_v[0].t_max = preco_algo_base::max(wf, _pulse_v[0].peak, _index_start, _index_end);

  _pulse_v[0].peak -= _ped_mean;

  preco_algo_base::integral(wf, _pulse_v[0].area, _index_start, _index_end);

  _pulse_v[0].area = _pulse_v[0].area - ( _pulse_v[0].t_end - _pulse_v[0].t_start + 1) * _ped_mean;

  return true;

}

#endif
