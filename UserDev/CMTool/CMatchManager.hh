/**
 * \file CMatchManager.hh
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CMatchManager
 *
 * @author kazuhiro
 */

/** \addtogroup CMTool

    @{*/
#ifndef CMATCHMANAGER_HH
#define CMATCHMANAGER_HH

#include <iostream>

#include "CMManagerBase.hh"
#include "CMatchBookKeeper.hh"
#include "CFloatAlgoBase.hh"

namespace cmtool {

  /**
     \class CMatchManager
     A class that instantiates merging algorithm(s) and run.
     The book-keeping of merged cluster sets are done by CMatchBookKeeper.
  */
  class CMatchManager : public CMManagerBase {

  public:
    
    /// Default constructor
    CMatchManager();
    
    /// Default destructor
    virtual ~CMatchManager(){};

    /// Method to reset itself
    virtual void Reset();

    /// A simple method to add an algorithm for merging
    void AddMatchAlgo(CFloatAlgoBase* algo) {_match_algo = algo;}

    /// A method to obtain book keeper
    const CMatchBookKeeper& GetBookKeeper() const { return _book_keeper; }

  protected:
    
    //
    // FMWK functions override
    //

    /// FMWK function called @ beginning of Process()
    virtual void EventBegin();

    /// FMWK function called @ beginning of iterative loop inside Process()
    virtual void IterationBegin();

    /// FMWK function called @ iterative loop inside Process()
    virtual bool IterationProcess();

    /// FMWK function called @ end of iterative loop inside Process()
    virtual void IterationEnd();
    
    /// FMWK function called @ end of Process()
    virtual void EventEnd();

  protected:

    /// Book keeper instance
    CMatchBookKeeper _book_keeper;

    /// Merging algorithm
    ::cmtool::CFloatAlgoBase* _match_algo;

  };
}

#endif
/** @} */ // end of doxygen group 

