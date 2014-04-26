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
    std::vector<unsigned short>::clear();
    std::vector<unsigned short>::reserve(nclusters);
    //for(size_t i=0; i<nclusters; ++i) std::vector<unsigned short>::push_back(i);
    for(size_t i=0; i<nclusters; ++i) this->push_back(i);
  }

  void CBookKeeper::Merge(unsigned short index1, unsigned short index2)
  {
    if( index1 >= this->size() || index2 >= this->size() )
      throw RecoUtilException(Form("Invalid cluster index: %d or %d",index1,index2));

    auto out_index1 = this->at(index1);
    auto out_index2 = this->at(index2);

    if(out_index1 == out_index2) return;

    // Make out_index1 to be the smaller one
    if(out_index1 > out_index2) std::swap(out_index1,out_index2);
	
    for(auto &v : (*this)) {

      if( v == out_index1 || v == out_index2 )
	v = out_index1;
      else if( v > out_index2 )
	v -= 1;
    }
    
  }

  bool CBookKeeper::IsMerged(unsigned short index1, unsigned short index2) const
  { 
    if( index1 >= this->size() || index2 >= this->size() )
      throw RecoUtilException(Form("Invalid cluster index: %d or %d",index1,index2));

    return this->at(index1) == this->at(index2); 
  }

  
  std::vector<unsigned short> CBookKeeper::GetMergedSet(unsigned short index1) const
  {

    if( index1 >= this->size() )
      throw RecoUtilException(Form("Invalid cluster index: %d ",index1));

    auto out_index = this->at(index1);
    std::vector<unsigned short> result;
    
    for(size_t i=0; i<this->size(); ++i)
      if( this->at(i) == out_index ) result.push_back(i);

    return result;
  }

  void CBookKeeper::PassResult(std::vector<std::vector<unsigned short> > &result) const
  {
    size_t cluster_index_max=0;
    for(auto const &v : (*this)) if(v > cluster_index_max) cluster_index_max = v;

    result.clear();
    result.resize(cluster_index_max + 1, std::vector<unsigned short>());

    for(size_t i=0; i<this->size(); ++i)
      result.at(this->at(i)).push_back(i);
  }

  void CBookKeeper::Combine(const CBookKeeper &another)
  {
    // Check length compatibility between this instance's result and "another"
    std::vector<std::vector<unsigned short> > my_result;
    this->PassResult(my_result);
    if(my_result.size() != another.size()) {
      throw RecoUtilException(Form("Input has an incompatible size (%zu != %zu)",
				   my_result.size(),
				   another.size())
			      );
      return;
    }

    // Check if "another" result is different from input
    std::vector<std::vector<unsigned short> > another_result;
    another.PassResult(another_result);
    if(another_result.size() == my_result.size()) return;

    // Combine
    for(auto const& ares : another_result) {

      if(ares.size()==1) continue;

      // Get one of cluster to be used for merging
      unsigned short target = my_result.at(ares.at(0)).at(0);

      for(auto const &res_index : ares) {

	for(auto const &orig_index : my_result.at(res_index)) {

	  if(target == orig_index) continue;
	
	  else this->Merge(target,orig_index);

	}

      }

    }

  }

}

#endif
