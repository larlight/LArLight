{
  gSystem->Load("libUBOnlineDBI");

  ubpsql::DBConn::GetME(ubpsql::kUBDAQ_WRITER)->Configure(ubpsql::DBI_CONFIG[ubpsql::kCUSTOM_USER]);
  ubpsql::DBConn::GetME(ubpsql::kUBDAQ_READER)->Configure(ubpsql::DBI_CONFIG[ubpsql::kCUSTOM_USER]);
  
  ubpsql::RCWriter r;
  
  unsigned int new_run = r.GetLastRun() + 1;
  if(!(r.InsertNewRun(new_run))) return;

  // Check
  unsigned int new_run_check = r.GetLastRun();

  if(new_run == new_run_check) std::cout << "Inserted a new run: " << new_run << std::endl;
  else std::cout << "Failed to insert a new run: " << new_run " ... latest run as of now: " << new_run_check << std::endl;


}
