/**
 * \file CFAlgoArray.hh
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CFAlgoArray
 *
 * @author kazuhiro_NAME
 */

/** \addtogroup CMTool

    @{*/
#ifndef CFALGOARRAY_HH
#define CFALGOARRAY_HH

#include "CFloatAlgoBase.hh"

namespace cmtool {
  /**
     \class CFAlgoArray
     User implementation for CFloatAlgoBase class
     This is an array of priority algorithms. It has multiple
     modes to compute combined Float() return value.
  */
  class CFAlgoArray : public CFloatAlgoBase {
    
  public:

    enum EvaluationMode_t {
      /**
	 DEFAULT: simply add Float() return values except negative return.
	 If there is a negative return value, it returns that and stop processing
	 the rest of algorithms
      */
      kPositiveAddition,
      /**
	 Simply add Float() return values from all algorithms.
	 Note some algorithms are allowed to return negative values.
      */
      kSimpleAddition,
      /**
	 Multiply Float() return values from all algorithms except negative
	 return. If there is a negative return, it returns that and stop
	 processing the rest of algorithms
      */
      kMultiplication,
      /**
	 If all algorithms return positive values, return the last algorithm's
	 Float() return value. If any algorithm returns negative, it returns that
	 and stop processing the rest of algorithms
      */      
      kLastAlgo,
    };
    
    /// Default constructor
    CFAlgoArray();
    
    /// Default destructor
    virtual ~CFAlgoArray(){};

    /// Setter to add a new algorithm
    void AddAlgo(CFloatAlgoBase* algo) { _algo_array.push_back(algo); }

    /// Setter for an evaluation
    void SetMode(EvaluationMode_t mode) { _mode = mode; }

    /**
       Core function: given a set of CPANs, return a float which indicates 
       the compatibility the cluster combination.
    */
    virtual float Float(const std::vector<const cluster::ClusterParamsAlg*> &cluster);

    /**
       Optional function: called after each iterative approach if a manager class is
       run with verbosity level <= kPerIteration. Maybe useful for debugging.
    */
    virtual void Report() { for(auto const& algo : _algo_array) algo->Report(); }
    
    /// Function to reset the algorithm instance, called together with manager's Reset()
    virtual void Reset() { for(auto const& algo : _algo_array) algo->Reset(); }

    /**
       Optional function: called at the beginning of 1st iteration. This is called per event.
     */
    virtual void EventBegin(const std::vector<cluster::ClusterParamsAlg> &clusters)
    { for(auto const& algo : _algo_array) algo->EventBegin(clusters); }

    /**
       Optional function: called at the end of event ... after the last merging iteration is over.
     */
    virtual void EventEnd()
    { for(auto const& algo : _algo_array) algo->EventEnd(); }
 
    /**
       Optional function: called at the beggining of each iterative loop.
       This provides all clusters' information in case the algorithm need them. Note this
       is called per iteration which may be more than once per event.
     */
    virtual void IterationBegin(const std::vector<cluster::ClusterParamsAlg> &clusters)
    { for(auto const& algo : _algo_array) algo->IterationBegin(clusters); }

    /**
       Optional function: called at the end of each iterative loop.
     */
    virtual void IterationEnd()
    { for(auto const& algo : _algo_array) algo->IterationEnd(); }

  protected:

    /// vector of algorithms
    std::vector<CFloatAlgoBase*> _algo_array;

    /// evaluation mode
    EvaluationMode_t _mode;

  };
}
#endif
/** @} */ // end of doxygen group 

