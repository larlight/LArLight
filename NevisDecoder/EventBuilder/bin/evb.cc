#include "EventBuilder-TypeDef.hh"

int main(int argc, char** argv) {

  larlight::event_builder* evb = new larlight::event_builder;

  evb->set_output_filename("out.root");
  evb->add_input(larlight::DATA::Trigger, argv[1]);
  evb->add_input(larlight::DATA::PMTFIFO, argv[2]);

  evb->set_master_stream(larlight::DATA::Trigger);

  //evb->set_verbosity(larlight::MSG::DEBUG);

  evb->run();

  return 0;
}
