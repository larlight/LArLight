//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class larlight::data_base+;
#pragma link C++ class std::vector<TVector3>+;

#pragma link C++ class larlight::pmtfifo+;
#pragma link C++ class std::vector<larlight::pmtfifo>+;
#pragma link C++ class larlight::event_pmtfifo+;

#pragma link C++ class larlight::tpcfifo+;
#pragma link C++ class std::vector<larlight::tpcfifo>+;
#pragma link C++ class larlight::event_tpcfifo+;

#pragma link C++ class larlight::trigger+;

#pragma link C++ class larlight::part_mc+;
#pragma link C++ class std::vector<larlight::part_mc>+;
#pragma link C++ class larlight::event_mc+;

#pragma link C++ class larlight::wire+;
#pragma link C++ class std::vector<larlight::wire>+;
#pragma link C++ class larlight::event_wire+;

#pragma link C++ class larlight::hit+;
#pragma link C++ class std::vector<larlight::hit>+;
#pragma link C++ class larlight::event_hit+;

#pragma link C++ class std::vector<TVector3>+;
#pragma link C++ class std::vector<TMatrixD>+;
#pragma link C++ class larlight::cluster+;
#pragma link C++ class std::vector<larlight::cluster>+;
#pragma link C++ class larlight::event_cluster+;

#pragma link C++ class larlight::track+;
#pragma link C++ class std::vector<larlight::track>+;
#pragma link C++ class larlight::event_track+;

#pragma link C++ class larlight::spacepoint+;
#pragma link C++ class std::vector<larlight::spacepoint>+;
#pragma link C++ class larlight::event_sps+;

#pragma link C++ class larlight::shower+;
#pragma link C++ class std::vector<larlight::shower>+;
#pragma link C++ class larlight::event_shower+;

#pragma link C++ class larlight::calorimetry+;
#pragma link C++ class std::vector<larlight::calorimetry>+;
#pragma link C++ class larlight::event_calorimetry+;

#pragma link C++ class larlight::user_info+;
#pragma link C++ class std::vector<larlight::user_info>+;
#pragma link C++ class larlight::event_user+;


#pragma link C++ class std::pair<std::string,bool>+;
#pragma link C++ class std::pair<std::string,std::string>+;
#pragma link C++ class std::pair<std::string,std::vector<bool> >+;
#pragma link C++ class std::pair<std::string,std::vector<int> >+;
#pragma link C++ class std::pair<std::string,std::vector<double> >+;
#pragma link C++ class std::pair<std::string,std::vector<std::string> >+;

#pragma link C++ class std::map<std::string,bool>+;
#pragma link C++ class std::map<std::string,std::string>+;
#pragma link C++ class std::map<std::string,std::vector<bool> >+;
#pragma link C++ class std::map<std::string,std::vector<int > >+;
#pragma link C++ class std::map<std::string,std::vector<double> >+;
#pragma link C++ class std::map<std::string,std::vector<std::string> >+;


#pragma link C++ class larlight::storage_manager+;

#endif
