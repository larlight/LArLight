#ifndef PRECO_ALGO_BASE_CC
#define PRECO_ALGO_BASE_CC

#include "preco_algo_base.hh"

//***************************************************************
bool check_index(const std::vector<UShort_t> *wf, const size_t &begin, size_t &end)
//***************************************************************
{
  if(begin >= wf->size() || end >= wf->size() || begin > end){

    Message::send(MSG::ERROR,__FUNCTION__,
		  Form("Invalid arguments: waveform length = %zu, begin = %zu, end = %zu!",
		       wf->size(), begin, end));
    return false;

  }

  if(!end) end = wf->size() - 1;

  return true;
}

//***************************************************************
preco_algo_base::preco_algo_base() : decoder_base(), _pulse()
//***************************************************************
{

  _name = "preco_algo_base";
  
  _ped_mean = _ped_rms = -1;

  reset();

}

void preco_algo_base::reset(){

  _pulse.reset_param();

  _pulse_v.clear();

  _pulse_v.reserve(3);
  
}

//***************************************************************
const pulse_param* preco_algo_base::get_pulse(size_t index) const
//***************************************************************
{

  if(index >= _pulse_v.size()) return 0;

  else return &(_pulse_v.at(index));

}

//***************************************************************
bool preco_algo_base::integral(const std::vector<UShort_t> *wf,
			       double &result,
			       size_t begin,
			       size_t end) const
//***************************************************************
{
  
  if(!check_index(wf,begin,end)) return false;
  
  std::vector<UShort_t>::const_iterator begin_iter(wf->begin());
  
  std::vector<UShort_t>::const_iterator end_iter(wf->begin());

  begin_iter = begin_iter + begin;

  end_iter = end_iter + end + 1;
  
  result = (double)(std::accumulate(begin_iter, end_iter, 0));

  return true;
}

//***************************************************************
bool preco_algo_base::derivative(const std::vector<UShort_t> *wf,
				 std::vector<int32_t> &diff,
				 size_t begin,
				 size_t end) const 
//***************************************************************
{

  if(check_index(wf,begin,end)){
    
    diff.clear();
    diff.reserve(end - begin);

    for(size_t index = begin ; index <= end ; ++index)
      
      diff.push_back(wf->at(index+1) - wf->at(index));

    return true;
  }

  return false;

}

//***************************************************************
size_t preco_algo_base::max(const std::vector<UShort_t> *wf,
			    double &result,
			    size_t begin,
			    size_t end) const
//***************************************************************
{

  size_t target_index = wf->size() + 1;

  result = 0;

  if(check_index(wf,begin,end)) {

    for(size_t index = begin; index <= end; ++index)
      
      if( result < wf->at(index)) { target_index = index; result = (double)(wf->at(index)); }
    
  }

  return target_index;

}

//***************************************************************
size_t preco_algo_base::min(const std::vector<UShort_t> *wf,
			    double &result,
			    size_t begin,
			    size_t end) const
//***************************************************************
{

  size_t target_index = wf->size() + 1;

  result = 4096;

  if(check_index(wf,begin,end)) {

    for(size_t index = begin; index <= end; ++index)
      
      if( result > wf->at(index)) { target_index = index; result = (double)(wf->at(index)); }
    
  }

  return target_index;

}

#endif
