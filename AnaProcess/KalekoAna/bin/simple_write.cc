#include <TSystem.h>
#include <TVector3.h>
#include <storage_manager.hh>
#include <Base-TypeDef.hh>

int main(){

  //
  // A simple routine to read a data file and perform an event loop.
  // This is a test routine for storage_manager class which interacts
  // decoder output root file. 
  //

  larlight::storage_manager my_storage;

  // If you wish, change the message level to DEBUG.
  // Commented out by default.
  my_storage.set_verbosity(larlight::MSG::DEBUG);
  

  // Step 0: Set I/O mode: we are reading in, so "READ"
  my_storage.set_io_mode(larlight::storage_manager::WRITE);

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
  larlight::event_track* my_event_track = (larlight::event_track*)(my_storage.get_data(larlight::DATA::Track));
  int run_id = 1;
  int subrun_id = 1;
  for( int i=0; i<100; i++){

    int event_id = i;
    my_event_track->set_run(run_id);
    my_event_track->set_subrun(subrun_id);
    my_event_track->set_event_id(event_id);

    // Let's make 2 tracks!
    for( int j=0; j<2; j++){
      larlight::track t;
      t.set_track_id(j); 

      // Let's make a track with 20 fake space points
      for(int k=0; k<20; k++){
	t.add_vertex    ( TVector3( (double)k,(double)k,(double)k ) );
	t.add_momentum  ( 1.);
	t.add_direction ( TVector3( (double)k,(double)k,(double)k ) );
      }
    
      // Append to the event track array
      my_event_track->push_back(t);
    }

    // Store event
    my_storage.next_event();

  }

  my_storage.close();
  return 1;
}


