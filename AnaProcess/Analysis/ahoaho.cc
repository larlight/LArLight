#ifndef AHOAHO_CC
#define AHOAHO_CC

#include "ahoaho.hh"

bool ahoaho::initialize() {

  //
  // This function is called in the beggining of event loop
  // Do all variable initialization you wish to do here.
  // If you need, you have an output root file pointer "_fout".

  return true;
}

bool ahoaho::analyze(storage_manager* storage) {

  //
  // Do your event-by-event analysis here. This function is called for 
  // each event in the loop. You have "storage" pointer which contains 
  // event-wise data. For a reference of pmt_wf_collection class instance, 
  // see the class index in the documentation.

  return true;
}

bool ahoaho::finalize() {

  // This function is called at the end of event loop.
  // Do all variable finalization you wish to do here.
  // If you need, you can store your ROOT class instance in the output
  // file. You have an access to the output file through "_fout" pointer.

  return true;
}

#endif
