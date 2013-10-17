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

#pragma link C++ class mctruth+;
#pragma link C++ class std::vector<mctruth>+;
#pragma link C++ class event_mc+;

#pragma link C++ class track+;
#pragma link C++ class std::vector<track>+;
#pragma link C++ class event_track+;

#pragma link C++ class user_info+;
#pragma link C++ class std::vector<user_info>+;
#pragma link C++ class user_collection+;

#pragma link C++ class storage_manager+;

#endif
