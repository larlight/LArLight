#ifndef MONCONSTS_H
#define MONCONSTS_H

#include <string>

namespace DB {

  /// Default value for an unsuccessful status
  const unsigned short INVALID_KEY = 0xffff;

  /// Database server type
  enum DB_t {
    kMySQL=0,
    kPostgreSQL
  };

};

namespace QC {

  /// Monitor object type, typically associated w/ an algorithm which makes one monitoring plot/value
  enum MonKey_t {
    kTEST=0,   ///< First test monitoring parameter
    kMonKeyMax ///< # of monitor types
  };

  /// Field types for data quality control tables
  enum QCField_t {
    kMonKey=0,  ///< MonKey_t   ... monitor key
    kChannel,   ///< Short_t    ... channel number
    kTimeBegin, ///< TTimeStamp ... subject validity range start time
    kTimeEnd,   ///< TTimeStamp ... subject validity range end time
    kRun,       ///< Int_t      ... subject run number
    kSubRun,    ///< Short_t    ... subject sub-run number
    kMean,      ///< Double_t   ... subject quality control mean value
    kSigma,     ///< Double_t   ... subject quality control sigma value
    kReference, ///< TString    ... subject reference value (can be anything to keep track of the QC analysis method used)
    kLogTime,   ///< TTimeStamp ... time at which the enry is made
    kFieldMax   ///< 
  };

  /// Field name for data quality control tables
  const std::string kQCFieldName[kFieldMax] = {
    "MonitorKey",
    "Channel",
    "TimeBegin",
    "TimeEnd",
    "Run",
    "SubRun",
    "Mean",
    "Sigma",
    "Reference",
    "LogTime"
  };

};

#endif

