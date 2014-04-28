//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class larlight::event_builder+;
#pragma link C++ class larlight::evb_algo_tpc+;
#pragma link C++ class larlight::evb_algo_pmt+;
#pragma link C++ class larlight::evb_algo_trigger+;
#pragma link C++ class larlight::evb_exception+;
#endif
