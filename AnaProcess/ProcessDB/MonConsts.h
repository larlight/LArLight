#ifndef MONCONSTS_H
#define MONCONSTS_H

#include <string>

namespace DB {

  /// Default value for an unsuccessful status
  const unsigned short INVALID_KEY = 0xffff;

  /// Database server type
  enum DB_t {
    MySQL=0,
    PostgreSQL,
    Oracle
  };

  /* 
     Data type ... THIS METHOD LIMITS AVAILABLE TYPES ... but probably OK for physicists usage...
     Note: we do not support "unsigned" as it is not supported in PostgreSQL and I am designing
           this package to be able to use between two server types.
  */
  enum DataType_t {
    SMALLINT=0, ///< 2 byte signed integer
    INT,        ///< 4 byte signed integer
    LONG,       ///< 8 byte signed integer
    FLOAT,      ///< 4 byte floating point
    DOUBLE,     ///< 8 byte floating point
    TIMESTAMP,  ///< Time stamp (8byte)
    DATATYPEMAX ///< Counter for # of data types
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

