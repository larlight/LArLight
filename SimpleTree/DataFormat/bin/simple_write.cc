#include <TSystem.h>
#include <TVector3.h>
#include <storage_manager.hh>
#include <constants.hh>

int main(){

  //
  // A simple routine to read a data file and perform an event loop.
  // This is a test routine for storage_manager class which interacts
  // decoder output root file. 
  //

  storage_manager my_storage;

  // If you wish, change the message level to DEBUG.
  // Commented out by default.
  my_storage.set_verbosity(MSG::DEBUG);
  

  // Step 0: Set I/O mode: we are reading in, so "READ"
  my_storage.set_io_mode(storage_manager::WRITE);

  // Step 1: Set output file 
  my_storage.set_out_filename("trial.root");

  // Step 2: Open a file.
  my_storage.open();

  // Step 3: Check if a file is opened.
  if(!my_storage.is_open()) {
    std::cerr << "File open failed!" << std::endl;
    return 0;
  }
  
  // Step 4: Check if it's ready to perform I/O
  if(!my_storage.is_ready_io()) {
    std::cerr << "I/O preparation failed!" << std::endl;
  }

  // Let's fill event_track for 100 events.
  track* my_track = (track*)(my_storage.get_data(DATA::Track));

  for( int i=0; i<100; i++){

    // Store event
    my_storage.next_event();

  }

  my_storage.close();
  return 1;
}


