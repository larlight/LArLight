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

namespace ProcDB{

  /// Field types for JobQueueTable
  enum QField_t {
    kQueueID=0, ///< Unique Index
    kQueueName, ///< Associated string identification
    kUserName,  ///< User name
    kCMD,       ///< Command to execute
    kStartTime, ///< Timestamp after which the 1st command execution will be tried
    kPeriod,    ///< Period between trials 
    kIteration, ///< Number of command executions
    kLogTime,   ///< Timestamp of last command execution
    kQFieldMax  
  };

  const std::string kQFieldName[kQFieldMax] = {
    "QueueID",   
    "QueueName",
    "UserName",
    "Command",
    "StartTime",
    "Period",
    "Iteration",
    "LogTime"
  };

};

namespace QCDB {

  /// Monitor object type, typically associated w/ an algorithm which makes one monitoring plot/value
  enum MonKey_t {
    kTEST=0,   ///< First test monitoring parameter
    kMonKeyMax ///< # of monitor types
  };

  /// Field types for data quality control tables
  enum QCField_t {
    kMonKey=0,   ///< MonKey_t   ... monitor key
    kChannel,    ///< Short_t    ... channel number
    kTimeBegin,  ///< TTimeStamp ... subject validity range start time
    kTimeEnd,    ///< TTimeStamp ... subject validity range end time
    kRun,        ///< Int_t      ... subject run number
    kSubRun,     ///< Short_t    ... subject sub-run number
    kMean,       ///< Double_t   ... subject quality control mean value
    kSigma,      ///< Double_t   ... subject quality control sigma value
    kReference,  ///< TString    ... subject reference value (can be anything to keep track of the QC analysis method used)
    kLogTime,    ///< TTimeStamp ... time at which the enry is made
    kQCFieldMax  ///< 
  };

  /// Field name for data quality control tables
  const std::string kQCFieldName[kQCFieldMax] = {
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

