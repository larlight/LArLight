//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class preco_algo_base+;
#pragma link C++ class algo_fixed_window+;
#pragma link C++ class algo_threshold+;
#pragma link C++ class ped_estimator+;
#pragma link C++ class pulse_reco+;
#pragma link C++ struct pulse_param+;
#endif

