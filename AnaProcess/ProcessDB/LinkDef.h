//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace DB;
#pragma link C++ namespace QC;
#pragma link C++ class ProcessDBI+;
#pragma link C++ class ProcessDBIManager+;
#pragma link C++ class TableBase+;
#pragma link C++ class RunQCTable+;
#pragma link C++ class TimeQCTable+;
//#pragma link C++ class QCTableBase+;
#endif
