
#include "Analysis-TypeDef.hh"
#include "DataFormat-TypeDef.hh"
#include "Base-TypeDef.hh"
#include "ClusterRecoAlg-TypeDef.hh"
#include "ClusterStudy-TypeDef.hh"

int main(int argc, char** argv) {

  larlight::ana_processor ana_proc;
  
  ana_proc.set_io_mode(larlight::storage_manager::BOTH);

  ana_proc.add_input_file(argv[1]);

  larlight::storage_manager::get()->set_in_rootdir("scanner");

  ana_proc.set_output_file("aho.root");

  ana_proc.set_ana_output_file("");
  
  auto merger = new larlight::ClusterMerge;

  merger->set_mergealg(new larreco::ClusterMergeAlg);

  ana_proc.add_process(merger);

  ana_proc.run();

  return 1;
}

