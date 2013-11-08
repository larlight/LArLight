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

/**
   \class RunQCTable
   Quality control database table for run-wise quality monitoring.
*/
class RunQCTable : public QCTableBase {

public:

  /// Default constructor
  RunQCTable() : QCTableBase("RunQCTable") { Initialize(); };

  /// Default destructor
  virtual ~RunQCTable(){};

  /// Method to initialize the instance
  virtual void Initialize();

  /// Method to create a table if not exists yet ... TO BE IMPLEMENTED BY DAUGHTERS
  virtual bool CreateTable() const = 0;

  /// Method to load the next result row (call after QCTable::Load) ... TO BE IMPLEMENTED BY DAUGHTERS
  virtual bool Fetch()=0;

  //
  // Setter for cut values ... implement run and sub-run cut
  //

  /// Setter for run range
  void SetRangeRun  (Int_t min, Int_t max) { _cut_run.first=min; _cut_run.second=max; };

  /// Setter for subrun range
  void SetRangeSubRun  (Int_t min, Int_t max) { _cut_subrun.first=min; _cut_subrun.second=max; };

};


#endif
