//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace ubpsql;
#pragma link C++ namespace ubpsql::MSG;

#pragma link C++ class ubpsql::DBBase+;
#pragma link C++ class ubpsql::DBException+;
#pragma link C++ class ubpsql::InvalidUser+;
#pragma link C++ class ubpsql::ConnectionError+;

#pragma link C++ class ubpsql::DBConn+;
#pragma link C++ class ubpsql::DBAppBase+;

#pragma link C++ class ubpsql::RCReader+;
#pragma link C++ class ubpsql::RCWriter+;
//ADD_NEW_CLASS ... do not change this line
#endif




