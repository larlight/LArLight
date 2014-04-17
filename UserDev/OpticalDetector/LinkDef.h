//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace optdata;
#pragma link C++ class optdata::ChannelData+;
#pragma link C++ class optdata::ChannelDataGroup+;
#pragma link C++ class optdata::FIFOChannel+;
#pragma link C++ class optdata::PMTTrigger+;

#pragma link C++ namespace opdet;
#pragma link C++ class opdet::OpticalWFGen+;
//ADD_NEW_CLASS ... do not change this line
#endif

