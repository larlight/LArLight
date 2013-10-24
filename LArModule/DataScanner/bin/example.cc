// LArLight include
#include "Base-TypeDef.hh"
#include "DataFormat-TypeDef.hh"

int main(int argc, char** argv)
{

  storage_manager mgr;
  mgr.set_verbosity(MSG::DEBUG);
  mgr.add_in_filename(argv[1]);
  mgr.set_data_to_read(DATA::Kalman3DSPS);
  mgr.set_io_mode(mgr.READ);
  mgr.set_in_rootdir("scanner");
  mgr.open();
  while(mgr.next_event()){
    
    event_track* track_array = (event_track*)(mgr.get_data(DATA::Kalman3DSPS));

    std::cout<<Form("Event %d .... %zu tracks found!",track_array->event_id(),track_array->GetTrackCollection().size())<<std::endl;

  }
  mgr.close();
}
