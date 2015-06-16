//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class larlight::decode_algo_exception+;
#pragma link C++ class larlight::algo_base+;
#pragma link C++ class larlight::algo_slow_readout_decoder+;
#pragma link C++ class larlight::algo_xmit_decoder+;
#pragma link C++ class larlight::algo_tpc_xmit+;
#pragma link C++ class larlight::algo_pmt_xmit+;
#pragma link C++ class larlight::algo_tpc_huffman+;
#pragma link C++ class larlight::algo_sn_tpc_huffman+;
#pragma link C++ class larlight::algo_trig_decoder+;
#pragma link C++ class larlight::algo_fem_decoder_base+;
#endif
