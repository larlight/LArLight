/**
 * \file ChQCTable.hh
 *
 * \ingroup ProcessDB
 * 
 * \brief Class def header for a class ChQCTable
 *
 * @author kterao
 */

/** \addtogroup ProcessDB

    @{*/

#ifndef CHQCTABLE_HH
#define CHQCTABLE_HH

#include "RunQCTable.hh"

/**
   \class ChQCTable
   Quality control database table for run-wise quality monitoring.
*/
class ChQCTable : public RunQCTable {

public:

  /// Default constructor
  ChQCTable();

  /// Default destructor
  virtual ~ChQCTable(){};

  /// Method to return table definition string
  virtual const std::string GetTableDef(DB::DB_t type) const;

  /// Method to initialize the instance
  virtual void Initialize();
  
  /// Method to fill an entry in the table
  bool Fill(QC::MonKey_t type, Int_t run, Int_t subrun, Short_t ch, Double_t mean, Double_t sigma, std::string ref) const;

  /// Method to load the next result row (call after QCTable::Load) 
  virtual bool Fetch();

  //
  // Setter for cut values ... implement channel number
  //

  /// Setter for run range
  void SetRangeChannel  (Short_t min, Short_t max) { _cut_ch.first=min; _cut_ch.second=max; };

};


#endif
