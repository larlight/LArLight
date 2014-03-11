//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class std::set<UInt_t>+;
#pragma link C++ class larlight::reco_wf+;
#pragma link C++ class larlight::pulse_viewer+;
#pragma link C++ class larlight::pulse_analyzer+;
#pragma link C++ class larlight::led_pulse_selector+;
#pragma link C++ class larlight::led_pulse_analyzer+;
#pragma link C++ class larlight::pulse_selector_base+;
#pragma link C++ class larlight::pulse_selector_simple+;
#pragma link C++ class larlight::waveform_sampler+;
//ADD_NEW_CLASS ... do not change this line
#endif




