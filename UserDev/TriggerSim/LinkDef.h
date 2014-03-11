//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace trigger;
#pragma link C++ namespace trigdata;

#pragma link C++ class std::vector<trigdata::Trigger>+;
#pragma link C++ class trigdata::Trigger+;
#pragma link C++ class trigger::SimpleClock+;
#pragma link C++ class trigger::UBTriggerAlgo+;
//ADD_NEW_CLASS ... do not change this line
#endif
