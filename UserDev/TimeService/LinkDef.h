//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace util;
#pragma link C++ class util::TimeServiceException+;
#pragma link C++ class util::ElecClock+;
#pragma link C++ class std::vector<util::ElecClock>+;
#pragma link C++ class std::set<util::ElecClock>+;
#pragma link C++ class std::pair<std::set<util::ElecClock>::iterator,bool>+;
#pragma link C++ class util::TimeService+;

//ADD_NEW_CLASS ... do not change this line
#endif
