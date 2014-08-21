//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class  optreco::preco_algo_base+;
#pragma link C++ class  optreco::preco_exception+;
#pragma link C++ class  optreco::preco_manager+;
#pragma link C++ class  optreco::algo_fixed_window+;
#pragma link C++ class  optreco::algo_threshold+;
#pragma link C++ class  optreco::ped_estimator+;
#pragma link C++ struct optreco::pulse_param+;
#pragma link C++ class larlight::pulse_reco+;
#endif

