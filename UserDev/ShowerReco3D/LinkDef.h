//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace showerreco;
#pragma link C++ class larlight::ShowerReco3D+;
#pragma link C++ class larlight::MCShowerAna+;
#pragma link C++ class larlight::ShowerBackTracker+;
#pragma link C++ class larlight::ShowerQuality+;
//ADD_NEW_CLASS ... do not change this line
#pragma link C++ class showerreco::ShowerRecoAlgBase+;
//#pragma link C++ class showerreco::ShowerRecoAlg+;
//#pragma link C++ class showerreco::Pi0ShowerRecoAlg+;
//ADD_NEW_ALG_CLASS ... do not change this line
#endif









