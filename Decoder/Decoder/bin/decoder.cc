
#include "Decoder-TypeDef.hh"
#include "Algorithm-TypeDef.hh"

int main(int argc, char** argv){

  //
  // Example decoder routine
  //
  
  if(argc<2){
    std::cerr << "    Usage: decoder $INPUT_FILE" << std::endl << std::endl;
    return 1;
  }


  // Use a simple "out.root" for an output file name

  // Create the decoder instance
  decoder_manager decoder;

  // Choose your decoder algorithm
  algo_base* myalgo = new algo_xmit_decoder();
  //algo_base* myalgo = new algo_slow_readout_decoder();
  //algo_base* myalgo = new algo_trig_decoder();

  // If needed, set # words for backtrace.
  // In case of an error, decoder will print out this number of words being processed
  // prior to encountering the error word.
  myalgo->set_backtrace_mode(200);

  // Attach a decoder algorithm ... currently xmit, slow, or trigger
  decoder.set_decoder(myalgo);

  // Set data type to be written in the output
  decoder.set_data_to_write(DATA_STRUCT::WF_COLLECTION);
  //decoder.set_data_to_write(DATA_STRUCT::TRIG_INFO);

  // Set input file format ... ASCII or BINARY
  //decoder.set_format(FORMAT::ASCII);
  decoder.set_format(FORMAT::BINARY);

  // Set input file path
  decoder.add_input_filename(argv[1]);

  // Set output file path ... simple out.root here
  decoder.set_output_filename("out.root");

  // Set message level
  //    - DEBUG   ... for debugging purpose. Very verbose.
  //    - INFO    ... for validation purpose. Spits out summary info of channel/event
  //    - NORMAL  ... default mode. Only spits out run-relevant information
  //    - WARNING ... suppress NORMAL information
  //    - ERROR   ... suppress WARNING information
  // For the given info level, all lower level information will be suppressed.
  //decoder.set_verbosity(MSG::DEBUG);
  decoder.set_verbosity(MSG::NORMAL);

  // Set debug mode True if you wish to continue in the decoding event
  // loop with exception handling. This avoids saving an event with
  // missing event words or conflicting encoding algorithm, and continue
  // in the decoding event loop. When turned off, the program exits as
  // soon as it encounters any issue.
  //decoder.debug_mode(false);
  decoder.debug_mode(true);

  // Finish configuration. Now run it.
  decoder.run();
    
  // Reset properly before exits.
  decoder.reset();

  return 0;
}
    

