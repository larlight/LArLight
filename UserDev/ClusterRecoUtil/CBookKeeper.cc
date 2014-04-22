#ifndef CBOOKKEEPER_CC
#define CBOOKKEEPER_CC

#include "CBookKeeper.hh"

namespace cluster {

  CBookKeeper::CBookKeeper(unsigned short nclusters)
  { 
    Reset(nclusters);
  }

  void CBookKeeper::Reset(unsigned short nclusters)
  {
    _out_index.clear();
    _out_index.reserve(nclusters);
    for(size_t i=0; i<nclusters; ++i) _out_index.push_back(i);
  }

  void CBookKeeper::Merge(unsigned short index1, unsigned short index2)
  {
    if( index1 >= _out_index.size() || index2 >= _out_index.size() )
      throw RecoUtilException(Form("Invalid cluster index: %d or %d",index1,index2));

    auto out_index1 = _out_index.at(index1);
    auto out_index2 = _out_index.at(index2);

    if(out_index1 == out_index2) return;

    auto merged_index = ( out_index1 > out_index2 ? out_index2 : out_index1 );

    for(auto &v : _out_index) {

      if( v == out_index1 || v == out_index2 )
	v = merged_index;
      else if( v > merged_index )
	v -= 1;
    }
    
  }

  bool CBookKeeper::IsMerged(unsigned short index1, unsigned short index2)
  { 
    if( index1 >= _out_index.size() || index2 >= _out_index.size() )
      throw RecoUtilException(Form("Invalid cluster index: %d or %d",index1,index2));

    return _out_index.at(index1) == _out_index.at(index2); 
  }

  
  std::vector<unsigned short> CBookKeeper::GetMergedSet(unsigned short index1)
  {

    if( index1 >= _out_index.size() )
      throw RecoUtilException(Form("Invalid cluster index: %d ",index1));

    auto out_index = _out_index.at(index1);
    std::vector<unsigned short> result;
    
    for(size_t i=0; i<_out_index.size(); ++i)
      if( _out_index.at(i) == out_index ) result.push_back(i);

    return result;
  }

  std::vector<std::vector<unsigned short> > CBookKeeper::GetMergedSetAll()
  {
    size_t cluster_index_max=0;
    for(auto const &v : _out_index) if(v > cluster_index_max) cluster_index_max = v;

    std::vector<std::vector<unsigned short> > result(cluster_index_max + 1,std::vector<unsigned short>());

    for(size_t i=0; i<_out_index.size(); ++i)
      
      result.at(_out_index.at(i)).push_back(i);

    return result;
  }

}

#endif
