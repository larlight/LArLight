//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace larutil;

#pragma link C++ class std::vector<std::vector<Float_t> >+;

#pragma link C++ class larutil::LArUtilException+;
//#pragma link C++ class larutil::LArUtilBase+;
#pragma link C++ class larutil::Geometry+;
#pragma link C++ class larutil::LArProperties+;
#pragma link C++ class larutil::DetectorProperties+;
#pragma link C++ class larutil::GeometryUtilities+;

#pragma link C++ class larutil::pxpoint+;
#pragma link C++ class larutil::pxline+;

//ADD_NEW_CLASS ... do not change this line
#endif




