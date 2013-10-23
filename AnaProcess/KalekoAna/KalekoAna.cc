#include "KalekoAna.hh"

#include <TSystem.h>
#include <TVector3.h>
#include <storage_manager.hh>
#include <constants.hh>

void KalekoAna::testfn(){

  std::cout << "This is my test function!" << std::endl;

}

int KalekoAna::testfn2(std::string filename){

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
  my_storage.set_io_mode(storage_manager::READ);

  // Step 0.5: Specify data type to readout 
  // this is actually redundant, all set to "true" by default, 
  // better to set ones you don't want to false
  my_storage.set_data_to_read(DATA::Track);

  // Step 1: Add input file from user input
  my_storage.add_in_filename(filename);

  // Step 1.5? make sure you're looking in the "scanner" directory
  my_storage.set_in_rootdir("scanner");

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
  /*
  // Let's loop over!
  while(my_storage.next_event()){

    event_track* my_event_track = (event_track*)(my_storage.get_data(DATA::Track));

    if(!my_event_track) continue;

    else{

      const std::vector<track> my_track(my_event_track->GetTrackCollection());
      std::cout << Form("Found %zu tracks for event %d...",
			my_track.size(),
			my_event_track->event_id())
		<< std::endl;
      for(size_t i=0; i<my_track.size(); i++){

	std::cout << Form("    Track %zu holding %zu space-points...",i,my_track.at(i).n_point()) 
		  << std::endl;

      }
    }
  }
  */
  my_storage.close();
  return 1;
}

