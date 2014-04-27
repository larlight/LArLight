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
    _prohibit_merge.clear();
    _prohibit_merge.reserve(nclusters);
    std::vector<unsigned short>::clear();
    std::vector<unsigned short>::reserve(nclusters);

    for(size_t i=0; i<nclusters; ++i) {
      this->push_back(i);
      _prohibit_merge.push_back(std::vector<bool>(nclusters-i,false));
    }
    _out_cluster_count = nclusters;
    
  }

  void CBookKeeper::ProhibitMerge(unsigned short index1, unsigned short index2)
  {
    if(index1 == index2)

      throw RecoUtilException(Form("<<%s>> Two input clusters identical (%d)",__FUNCTION__,index1));


    if( index1 >= this->size() || index2 >= this->size() )

      throw RecoUtilException(Form("Input cluster index (%d and/or %d) out of range",index1,index2));

    auto out_index1 = this->at(index1);
    auto out_index2 = this->at(index2);

    if(out_index1 == out_index2)
      throw RecoUtilException(Form("Cluster %d and %d already merged!",index1,index2));    

    if(out_index2 < out_index1) std::swap(out_index1,out_index2);

    _prohibit_merge.at(index1).at(index2) = true;

  }

  void CBookKeeper::Merge(unsigned short index1, unsigned short index2)
  {

    if(index1 == index2)

      throw RecoUtilException(Form("<<%s>> Two input clusters identical (%d)",__FUNCTION__,index1));


    if( index1 >= this->size() || index2 >= this->size() )

      throw RecoUtilException(Form("Input cluster index (%d and/or %d) out of range",index1,index2));

    auto out_index1 = this->at(index1);
    auto out_index2 = this->at(index2);

    if(out_index1 == out_index2) return;

    if(out_index2 < out_index1) std::swap(out_index1,out_index2);

    //if(_prohibit_merge.at(index1).at(index2)) return;

    //
    // Merge cluster indexes
    //
    for(auto &v : (*this)) {

      if( v == out_index1 || v == out_index2 )
	v = out_index1;
      else if( v > out_index2 )
	v -= 1;
    }

    /*
    //
    // Merge prohibit rule
    //
    // (1) handle index < out_index1
    for(size_t index=0; index < out_index1; ++index) {
      
      size_t tmp_out_index1 = out_index1 - index;
      size_t tmp_out_index2 = out_index2 - index;

      _prohibit_merge.at(index).at(tmp_out_index1) = ( _prohibit_merge.at(index).at(tmp_out_index1)
						       ||
						       _prohibit_merge.at(index).at(tmp_out_index2)
						       );
      
      for(size_t in_index=tmp_out_index2; 
	  in_index < _prohibit_merge.at(index).size();
	  ++in_index) {
	
	if(in_index >= (_out_cluster_count - 1 - index)) 

	  _prohibit_merge.at(index).at(in_index) = false;

	else
	  
	  _prohibit_merge.at(index).at(in_index) = _prohibit_merge.at(index).at(in_index+1);
      }
    }
    // (2) handle index == out_index1
    */    

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

    result.clear();
    result.resize(_out_cluster_count, std::vector<unsigned short>());

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
    if(another_result.size() >= my_result.size())
      throw RecoUtilException("The input has equal or more number of output clusters");

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
