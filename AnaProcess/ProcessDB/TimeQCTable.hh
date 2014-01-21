/**
 * \file TimeQCTable.hh
 *
 * \ingroup ProcessDB
 * 
 * \brief Class def header for a class TimeQCTable
 *
 * @author kterao
 */

/** \addtogroup ProcessDB

    @{*/

#ifndef TIMEQCTABLE_HH
#define TIMEQCTABLE_HH

#include "QCTableBase.hh"

namespace procdb {

  /**
     \class TimeQCTable
     Quality control database table for run-wise quality monitoring.
  */
  class TimeQCTable : public QCTableBase {
    
  public:
    
    /// Default constructor
    TimeQCTable();
    
    /// Default destructor
    virtual ~TimeQCTable(){};
    
    /// Method to return table definition string
    virtual const std::string GetTableDef(DB::DB_t type) const;
    
    /// Method to initialize the instance
    virtual void Initialize();
    
    /// Method to insert an entry to the database
    bool Fill(QCDB::MonKey_t type, TDatime tstart, TDatime tend, Double_t mean, Double_t sigma, std::string ref) const;
    
    /// Method to load the next result row (call after QCTable::Load)
    virtual bool Fetch();
    
    //
    // Setter for cut values ... implement time cut
    //
    
    /// Setter for time range
    void SetRangeTime  (TDatime min, TDatime max) { _cut_time.first=min; _cut_time.second=max; };
    
  };
  
}
#endif
/** @} */ // end of doxygen group 
