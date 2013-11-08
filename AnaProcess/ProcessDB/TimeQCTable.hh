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

  /// Method to initialize the instance
  virtual void Initialize();

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


#endif
