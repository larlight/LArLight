//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class larlight::ExecuteCompression+;
#pragma link C++ class larlight::ViewCompression+;
#pragma link C++ class larlight::HitMaker+;
#pragma link C++ class larlight::HitViewer+;
#pragma link C++ class larlight::WFViewer+;
//ADD_NEW_CLASS ... do not change this line

#pragma link C++ namespace compress+;

#pragma link C++ class compress::CompressionAlgoBase+;
#pragma link C++ class compress::CompressionAlgoThresh+;

#endif


