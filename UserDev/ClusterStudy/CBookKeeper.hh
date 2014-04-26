/**
 * \file CBookKeeper.hh
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class CBookKeeper
 *
 * @author kazuhiro
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef CBOOKKEEPER_HH
#define CBOOKKEEPER_HH

#include <iostream>
#include <vector>
#include <TString.h>
#include "RecoUtilException.hh"

namespace cluster {

  /**
     \class CBookKeeper
     A utility class for CMergeManager which merge clusters using merging algorithms.
     One of major task for CMergeManager is to keep track of which clusters to be merged
     in the original input. CBookKeeper handles this part. It works with indexes. The user
     (primarily CMergeManager) provides number of clusters to CBookKeeper's constructor.
     Then it can ask CBookKeeper to merge two specific clusters by specifying index number
     of the cluster which has to be smaller than the previously specified number of clusters.
     CBookKeeper keeps track of which clusters are asked to merge together, and it can be
     asked to return a vector of merged cluster indexes.
  */
  class CBookKeeper : public std::vector<unsigned short>{
    
  public:
    
    /// Default constructor
    CBookKeeper(unsigned short nclusters=0);
    
    /// Default destructor
    virtual ~CBookKeeper(){};

    /// Reset method
    void Reset(unsigned short nclusters=0);

    /// Method to merge 2 clusters via index numbers
    void Merge(unsigned short index1, unsigned short index2);

    /**
       Method to retrieve a vector of cluster indexes which 
       is merged with the input cluster index. All indexes here
       are regarding the original cluster index.
    */
    std::vector<unsigned short> GetMergedSet(unsigned short index1) const;

    /**
       Method to ask if a given 2 clusters are already merged.
       This method is expected to be much faster than obtaining
       a merged cluster sets from GetMergedIndexes and check if
       two clusters are merged.
    */
    bool IsMerged(unsigned short index1, unsigned short index2) const;

    /**
       A method to get the full result. The return is a vector
       of merged cluster indexes (which is a vector of original cluster indexes).
    */
    void PassResult(std::vector<std::vector<unsigned short> > &result) const;


    std::vector<std::vector<unsigned short> > GetResult() const
    { 
      std::vector<std::vector<unsigned short> > result; 
      PassResult(result);
      return result;
    }

    /**
       Method to combine with another CBookKeeper instance.
     */
    
    void Combine(const CBookKeeper &another);

  protected:

    /**
       This vector is initialized to the length of the clusters to run book keeping.
       The index number therefore corresponds to the original clusters' index number.
       The value at a given index number corresponds to the merged output cluster
       index. For instance it is originally [0,1,2] if the CBookKeeper instance
       is created for length 3. After a user call Merge(0,1), it becomes [0,0,1],
       suggesting there are only 2 output clusters and the input cluster index 0
       and 2 now belong to the output cluster index 0.
     */
    //std::vector<unsigned short> _out_index;
    
  };
  
}

#endif
/** @} */ // end of doxygen group 

