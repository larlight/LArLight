//
// Example C++ routine to run analysis module, ana_base.
// The usage is same for inherited analysis class instance.
//

#include <TSystem.h>
#include <Analysis-TypeDef.hh>

int main(){
  // Create ana_processor instance
  ana_processor my_proc;

  // Set input root file: this is decoder output root file.
  // This time, we use a sample file prepared.
  my_proc.add_input_file(Form("%s/dat/sample.root",gSystem->Getenv("ANA_PROC_DIR")));

  // Specify IO mode
  my_proc.set_io_mode(storage_manager::READ);

  // Set output root file: this is a separate root file in which your
  // analysis module can store anything such as histograms, your own TTree, etc.
  my_proc.set_ana_output_file("myout.root");

  // Create analysis class instance. For this example, ana_base.
  // To show how one can run multiple analysis modules at once,
  // we make multiple ana_base instance.

  ana_base* my_ana_1=new ana_base();
  ana_base* my_ana_2=new ana_base();
  ana_base* my_ana_3=new ana_base();

  // Add analysis modules to the processor

  my_proc.add_process(my_ana_1);
  my_proc.add_process(my_ana_2);
  my_proc.add_process(my_ana_3);

  // Let's run it.

  my_proc.run();

  // done!
}
