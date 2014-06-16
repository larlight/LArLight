/**
 * \file MatchBookKeeper.hh
 *
 * \ingroup ClusterStudy
 * 
 * \brief Class def header for a class MatchBookKeeper
 *
 * @author kazuhiro
 */

/** \addtogroup ClusterStudy

    @{*/
#ifndef MATCHBOOKKEEPER_HH
#define MATCHBOOKKEEPER_HH

#include <iostream>
#include <vector>
#include <map>
#include "RecoUtilException.hh"

namespace cmatch {
  /**
     \class MatchBookKeeper
     User defined class MatchBookKeeper ... these comments are used to generate
     doxygen documentation!
  */
  class MatchBookKeeper {
    
  public:
    
    /// Default constructor
    MatchBookKeeper();
    
    /// Default destructor
    virtual ~MatchBookKeeper(){};
  
    /// Reset method
    void Reset();

    /// Method to register matched clusters
    void Match(const std::vector<unsigned int>& matched_indexes,
	       const float& score);

    /// Method to get result
    std::vector<std::vector<unsigned int> > GetResult() const;

    /// Method to pass result
    void PassResult(std::vector<std::vector<unsigned int> >& result) const;

  protected:

    std::multimap<float,std::vector<unsigned int> > _register;

  };
}
#endif
/** @} */ // end of doxygen group 

