//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace larlight;
#pragma link C++ namespace larlight::MSG;
#pragma link C++ namespace larlight::FEM;
#pragma link C++ namespace larlight::DATA;
#pragma link C++ namespace larlight::GEO;
#pragma link C++ namespace larlight::MC;
#pragma link C++ namespace larlight::FORMAT;

#pragma link C++ enum larlight::GEO::SigType_t;
#pragma link C++ enum larlight::GEO::View_t;

#pragma link C++ enum larlight::DATA::DATA_TYPE;

#pragma link C++ class larlight::Message+;
#pragma link C++ class larlight::larlight_base+;

#endif
