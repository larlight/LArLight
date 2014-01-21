/**
 * \file RunQCTable.hh
 *
 * \ingroup ProcessDB
 * 
 * \brief Class def header for a class RunQCTable
 *
 * @author kterao
 */

/** \addtogroup ProcessDB

    @{*/

#ifndef RUNQCTABLE_HH
#define RUNQCTABLE_HH

#include "QCTableBase.hh"

namespace procdb {
  /**
     \class RunQCTable
     Quality control database table for run-wise quality monitoring.
  */
  class RunQCTable : public QCTableBase {
    
  public:
    
    /// Default constructor
    RunQCTable();
    
    /// Default destructor
    virtual ~RunQCTable(){};
    
    /// Method to return table definition string
    virtual const std::string GetTableDef(DB::DB_t type) const;
    
    /// Method to initialize the instance
    virtual void Initialize();
    
    /// Method to fill an entry in the table
    bool Fill(QCDB::MonKey_t type, Int_t run, Int_t subrun, Double_t mean, Double_t sigma, std::string ref) const;
    
    /// Method to load the next result row (call after QCTable::Load)
    virtual bool Fetch();
    
    //
    // Setter for cut values ... implement run and sub-run cut
    //
    
    /// Setter for run range
    void SetRangeRun  (Int_t min, Int_t max) { _cut_run.first=min; _cut_run.second=max; };
    
    /// Setter for subrun range
    void SetRangeSubRun  (Int_t min, Int_t max) { _cut_subrun.first=min; _cut_subrun.second=max; };
    
  };
  
}
#endif
/** @} */ // end of doxygen group 
