#ifndef JOBQUEUETABLE
#define JOBQUEUETABLE

#include "TableBase.hh"

class JobQueueTable : public TableBase {

public:

  JobQueueTable(){};

  virtual ~JobQueueTable(){};

  /// A function to return table definition string
  virtual const std::string GetTableDef(DB::DB_t type) const;
  

};

#endif
