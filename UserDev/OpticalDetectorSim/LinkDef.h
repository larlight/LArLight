//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace opdet;
#pragma link C++ class opdet::OpticalDetectorException+;
#pragma link C++ class opdet::UBOpticalADC+;
#pragma link C++ class std::vector<float>+;
#pragma link C++ class opdet::WFAlgoDigitizedSPE+;
#pragma link C++ class opdet::WFAlgoAnalyticalSPE+;
#pragma link C++ class opdet::WFAlgoArray+;
#pragma link C++ class opdet::ChConfig+;
#pragma link C++ class opdet::Tester+;
#pragma link C++ class opdet::RandomServer+;
//ADD_NEW_CLASS ... do not change this line
#endif








