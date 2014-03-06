{
  gSystem->Load("libUBOnlineDBI");

  ubpsql::DBConn::GetME(ubpsql::kUBDAQ_READER)->Configure(ubpsql::DBI_CONFIG[ubpsql::kCUSTOM_USER]);
  
  ubpsql::RCReader r;
  
  unsigned int run = r.GetLastRun();
  std::cout << "Last run: " << run << std::endl;


}
