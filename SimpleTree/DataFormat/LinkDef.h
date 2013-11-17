//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace DATA;

#pragma link C++ class storage_manager+;
#pragma link C++ class data_base+;
#pragma link C++ class track+;
#pragma link C++ class sps+;
#pragma link C++ class hit+;
#pragma link C++ class mctruth+;
#pragma link C++ class cluster+;
#pragma link C++ class mcnu+;
#pragma link C++ class mcpart+;
#pragma link C++ class event+;
#endif
