#include "DataFormat-TypeDef.hh"

int main()
{

  larlight::storage_manager man;
  man.set_io_mode(man.WRITE);
  man.set_out_filename("aho.root");
  man.set_verbosity(larlight::MSG::DEBUG);
  man.open();

  for(int i=0; i<10; i++) {

    for(int j=0; j<larlight::DATA::DATA_TYPE_MAX; ++j) {

      if( j == larlight::DATA::Seed ||
	  j == larlight::DATA::Event ||
	  j == larlight::DATA::MCTrajectory ||
	  //	  j == larlight::DATA::MCNeutrino ||
	  j == larlight::DATA::DATA_TYPE_MAX )

	continue;
      man.get_data((larlight::DATA::DATA_TYPE)j);
    }

    man.next_event();

  }
  
  man.close();

  return 1;
}
