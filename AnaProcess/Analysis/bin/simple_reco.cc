//
// Example C++ routine to run analysis module, ana_base.
// The usage is same for inherited analysis class instance.
//

#include <TSystem.h>
#include <Analysis-TypeDef.hh>
#include <TString.h>
int main(int argc, char** argv){
  // Create ana_processor instance
  ana_processor my_proc;

  // Set input root file: this is decoder output root file.
  // This time, we use a sample file prepared.
  my_proc.add_input_file(argv[1]);

  // Set output data root file
  my_proc.set_output_file("out.root");

  // Specify IO mode
  my_proc.set_io_mode(storage_manager::BOTH);

  // Set output root file: this is a separate root file in which your
  // analysis module can store anything such as histograms, your own TTree, etc.
  my_proc.set_ana_output_file("myout.root");

  // Create analysis class instance. For this example, ana_base.
  // To show how one can run multiple analysis modules at once,
  // we make multiple ana_base instance.

  //ana_base* my_ana_1=new ana_base();
  //ana_base* my_ana_2=new ana_base();
  //ana_base* my_ana_3=new ana_base();

  // Add analysis modules to the processor

  //my_proc.add_process(my_ana_1);
  //my_proc.add_process(my_ana_2);
  //my_proc.add_process(my_ana_3);

  my_proc.add_process(new pmtbaseline());

  // Let's run it.

  my_proc.run();

  // done!
}
