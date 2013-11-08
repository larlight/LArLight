/**
 * \file QCTableBase.hh
 *
 * \ingroup ProcessDB
 * 
 * \brief Class def header for a class QCTableBase
 *
 * @author kterao
 */

/** \addtogroup ProcessDB

    @{*/

#ifndef QCTABLEBASE_HH
#define QCTABLEBASE_HH

#include "TableBase.hh"

/**
   \class QCTableBase
   This is a base, abstract class of RunQCTable and TimeQCTable.
   This class defines common functions/variables among those daughters. In particular:
       (*) Functions to set cuts on field values
       (*) Load() function to query the database with cut values set by a user
   Daughters, RunQCTable and TimeQCTable, have two different composit keys. The first
   uses run & sub-run number while the latter uses start & end timestamp of a validity 
   range. Those daughters must implement CreateTable() and Fetch() methods as those
   methods depend on field definitions.
*/
class QCTableBase : public TableBase {

public:

  /// Default constructor
  QCTableBase(std::string name) : TableBase(name) { Initialize(); };

  /// Default destructor
  virtual ~QCTableBase(){};

  /// Method to initialize the instance
  virtual void Initialize();

  /// Method to clear loaded variable holders
  virtual void ClearValues();

  /// Method to initialize range cut values
  virtual void InitializeCuts();

  /// Method to clear query result
  virtual void ClearResult();

  /// Method to load ALL variables from the database applying cuts if set by users
  virtual bool Load(QC::MonKey_t type, size_t limit=0);

  /// Method to load the next result row (call after QCTable::Load) ... TO BE IMPLEMENTED BY DAUGHTERS
  virtual bool Fetch()=0;

  //
  // Setter for cut values ... note the base class only implements mean, sigma and reference 
  //                           as these are common to inherit table classes.
  //

  /// Setter for mean value range
  void SetRangeMean  (Double_t min, Double_t max) { _cut_mean.first=min;  _cut_mean.second=max;  };

  /// Setter for sigma value range
  void SetRangeSigma (Double_t min, Double_t max) { _cut_sigma.first=min; _cut_sigma.second=max; };
  
  /// Setter for reference 
  void SetReference  (TString ref) { _cut_ref = ref; };

  //
  // Getter for loaded variables
  //
  /// Mean value getter
  Double_t   Mean()      const { return _mean;   };

  /// Sigma value getter
  Double_t   Sigma()     const { return _sigma;  };

  /// Run number getter
  Int_t      Run()       const { return _run;    };

  /// Sub run number getter
  Int_t      SubRun()    const { return _subrun; };

  /// Channel number getter
  Short_t    Channel()   const { return _ch;     };

  /// Validity range start time getter
  TTimeStamp TimeStart() const { return _tstart;  };

  /// Validity range end time getter
  TTimeStamp TimeEnd()   const { return _tend;   };

  /// Reference getter
  TString    Reference() const { return _ref;    };

protected:

  size_t _res_key;    ///< A key to access query result

  TTimeStamp _tstart; ///< validity range start time variable holder
  TTimeStamp _tend;   ///< validity range end time variable holder
  Double_t _mean;     ///< QC mean value holder
  Double_t _sigma;    ///< QC sigma value holder
  Int_t    _run;      ///< run number holder
  Int_t    _subrun;   ///< sub run number holder
  Short_t  _ch;       ///< channel number holder
  TString  _ref;      ///< reference holder

  //
  // Cut range value holders
  //

  /// Query loads entries with start & ent time within the set range in this variable IFF SET BY USER.
  std::pair<TTimeStamp,TTimeStamp> _cut_time;

  /// Query loads entries with mean value within the set range in this variable IFF SET BY USER.
  std::pair<Double_t,Double_t> _cut_mean;

  /// Query loads entries with sigma value within the set range in this variable IFF SET BY USER.
  std::pair<Double_t,Double_t> _cut_sigma;

  /// Query loads entries with run number within the set range in this variable IFF SET BY USER  
  std::pair<Int_t,Int_t> _cut_run;

  /// Query loads entries with subrun number within the set range in this variable IFF SET BY USER  
  std::pair<Int_t,Int_t> _cut_subrun;

  /// Query loads entries with channel number within the set range in this variable IFF SET BY USER  
  std::pair<Short_t,Short_t> _cut_ch;

  /// Query loads entries with reference equal to variable IFF SET BY USER  
  TString _cut_ref;
  
};


#endif
