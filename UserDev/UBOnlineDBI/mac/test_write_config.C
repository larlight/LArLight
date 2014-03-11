/*
  This script doesn't work for now.
  You know why? It's because fucking stupid ROOT CINT cannot handle std::map yet.
  ROOT team was too happy figuring out how to make std::vector work.
  Use python script instead. Why? because it works and better in every possible way.

 */

{
  
#include <map>
  
  gSystem->Load("libUBOnlineDBI");

  ubpsql::DBConn::GetME(ubpsql::kUBDAQ_WRITER)->Configure(ubpsql::DBI_CONFIG[ubpsql::kCUSTOM_USER]);
  ubpsql::DBConn::GetME(ubpsql::kUBDAQ_READER)->Configure(ubpsql::DBI_CONFIG[ubpsql::kCUSTOM_USER]);
  
  ubpsql::RCWriter r;
  //  ubsql::DBConn::GetME(ubpsql::kUBDAQ_WRITER)->SetVerbosity(ubsql::MSG::kDEBUG);
  //  r.SetVerbosity(ubpsql::MSG::kDEBUG);

  std::map<std::string,std::string> hstore_column;

  hstore_column["par0"]="var0";
  hstore_column["par1"]="var1";

  r.InsertSubConfigValuesPerCrate("ktest1",0,hstore_column);
  

}
