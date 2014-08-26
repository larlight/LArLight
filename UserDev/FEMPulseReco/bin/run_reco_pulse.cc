//
// Example C++ routine to run analysis module, ana_base.
// The usage is same for inherited analysis class instance.
//

#include <TSystem.h>
#include <PulseReco-TypeDef.hh>
#include <Analysis-TypeDef.hh>
#include <TString.h>
int main(int argc, char** argv){
  // Create ana_processor instance
  larlight::ana_processor my_proc;

  my_proc.add_input_file(argv[1]);

  my_proc.set_output_file("out.root");

  my_proc.set_io_mode(larlight::storage_manager::BOTH);

  my_proc.set_ana_output_file("ana.root");

  larlight::pulse_reco preco;

  optreco::algo_fixed_window algo;

  //preco->set_reco_algo(new algo_threshold);
  preco.get_manager().add_reco_algo(&algo);

  preco.get_manager().set_ped_algo(preco.get_manager().kHEAD);
  
  preco.get_manager().set_ped_nsample_cosmic(3);

  my_proc.add_process(&preco);

  // Let's run it.

  my_proc.run();

  // done!
}
