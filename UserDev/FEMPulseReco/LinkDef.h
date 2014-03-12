//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class larlight::preco_algo_base+;
#pragma link C++ class larlight::algo_fixed_window+;
#pragma link C++ class larlight::algo_threshold+;
#pragma link C++ class larlight::ped_estimator+;
#pragma link C++ class larlight::pulse_reco+;
#pragma link C++ struct larlight::pulse_param+;
#endif

