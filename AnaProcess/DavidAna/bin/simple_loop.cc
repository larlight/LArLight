//
// Example C++ routine to run analysis module, ana_base.
// The usage is same for inherited analysis class instance.
//

#include <TSystem.h>
#include <Analysis-TypeDef.hh>

int main(int argc, char** argv){

  // Create ana_processor instance
  larlight::ana_processor my_proc;

  // Set input root file: this is decoder output root file.
  // This time, we use a sample file prepared.
  my_proc.add_input_file(argv[1]);

  // Specify IO mode
  my_proc.set_io_mode(larlight::storage_manager::READ);

  my_proc.set_data_to_read(larlight::DATA::Kalman3DSPS);
  my_proc.set_data_to_read(larlight::DATA::SpacePoint);
  my_proc.set_data_to_read(larlight::DATA::MCTruth);
  
  larlight::storage_manager::get()->set_in_rootdir("scanner");
  // Set output root file: this is a separate root file in which your
  // analysis module can store anything such as histograms, your own TTree, etc.
  my_proc.set_ana_output_file("myout.root");

  // Create analysis class instance. For this example, ana_base.
  // To show how one can run multiple analysis modules at once,
  // we make multiple ana_base instance.

  my_proc.add_process(new larlight::TrackViewer);

  // Let's run it.

  my_proc.run();

  // done!
}
