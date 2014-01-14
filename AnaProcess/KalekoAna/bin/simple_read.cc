#include <TSystem.h>
#include <TVector3.h>
#include <storage_manager.hh>
#include <Base-TypeDef.hh>

int main(int argc, char** argv){

  if(argc<2) {
    
    std::cerr << Form("Usage: %s $INPUT_FILE\n",argv[0]) << std::endl;

    return 1;

  }

  //
  // A simple routine to read a data file and perform an event loop.
  // This is a test routine for larlight::storage_manager class which interacts
  // decoder output root file. 
  //

  larlight::storage_manager my_storage;

  // If you wish, change the message level to DEBUG.
  // Commented out by default.
  my_storage.set_verbosity(larlight::MSG::DEBUG);
  

  // Step 0: Set I/O mode: we are reading in, so "READ"
  my_storage.set_io_mode(larlight::storage_manager::READ);

  // Step 0.5: Specify data type to readout
  my_storage.set_data_to_read(larlight::DATA::Track);

  // Step 1: Add input file from user input
  my_storage.add_in_filename(argv[1]);

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

  // Let's loop over!
  while(my_storage.next_event()){

    larlight::event_track* my_event_track = (larlight::event_track*)(my_storage.get_data(larlight::DATA::Track));

    if(!my_event_track) continue;

    else{

      std::cout << Form("Found %zu tracks for event %d...",
			my_event_track->size(),
			my_event_track->event_id())
		<< std::endl;
      for(size_t i=0; i<my_event_track->size(); i++){

	std::cout << Form("    Track %zu holding %zu space-points...",i,my_event_track->at(i).n_point()) 
		  << std::endl;

      }
    }
  }

  my_storage.close();
  return 1;
}


