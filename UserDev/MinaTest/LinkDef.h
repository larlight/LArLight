//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

//#pragma link C++ class MinaTest+;
#pragma link C++ class ToyData+;
#pragma link C++ class std::vector<ToyData>+;
#pragma link C++ class ToyEvent+;
#pragma link C++ class ToyAna+;
#pragma link C++ class MinaTestEvent+;
#pragma link C++ class MinaTestPhoton+;
#pragma link C++ class MinaTestParticle+;
#pragma link C++ class DarkPhotonAnaC+;
#pragma link C++ class LineDist+;
#pragma link C++ class ISevent+;
#pragma link C++ class ISstep+;
#pragma link C++ class ReflectivityStep+;
#pragma link C++ class ReflectivityEvent+;
//ADD_NEW_CLASS ... do not change this line
#endif

