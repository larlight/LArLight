//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace procdb;
//#pragma link C++ namespace procdb::DB;
//#pragma link C++ namespace procdb::ProcDB;
//#pragma link C++ namespace procdb::QCDB;
#pragma link C++ class procdb::ProcessDBI+;
#pragma link C++ class procdb::ProcessDBIManager+;
#pragma link C++ class procdb::TableBase+;
#pragma link C++ class procdb::JobQueueTable+;
#pragma link C++ class procdb::QCTableBase+;
#pragma link C++ class procdb::RunQCTable+;
#pragma link C++ class procdb::TimeQCTable+;
#pragma link C++ class procdb::ChQCTable+;
//#pragma link C++ class procdb::QCTableBase+;
#endif
