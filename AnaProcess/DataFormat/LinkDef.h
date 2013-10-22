//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class data_base+;
#pragma link C++ class std::vector<TVector3>+;

#pragma link C++ class pmtfifo+;
#pragma link C++ class std::vector<pmtfifo>+;
#pragma link C++ class event_pmt;

#pragma link C++ class part_mc+;
#pragma link C++ class std::vector<part_mc>+;
#pragma link C++ class event_mc+;

#pragma link C++ class wire+;
#pragma link C++ class std::vector<wire>+;
#pragma link C++ class event_wire+;

#pragma link C++ class hit+;
#pragma link C++ class std::vector<hit>+;
#pragma link C++ class event_hit+;

#pragma link C++ class cluster+;
#pragma link C++ class std::vector<cluster>+;
#pragma link C++ class event_cluster+;

#pragma link C++ class track+;
#pragma link C++ class std::vector<track>+;
#pragma link C++ class event_track+;

#pragma link C++ class spacepoint+;
#pragma link C++ class std::vector<spacepoint>+;
#pragma link C++ class event_sps+;

#pragma link C++ class user_info+;
#pragma link C++ class std::vector<user_info>+;
#pragma link C++ class event_user+;

#pragma link C++ class storage_manager+;

#endif
