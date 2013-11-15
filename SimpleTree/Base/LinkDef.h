//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace MSG;
#pragma link C++ namespace DATA;

#pragma link C++ class Message+;
#pragma link C++ class fmwk_base+;
#endif
