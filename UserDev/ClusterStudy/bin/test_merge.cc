#include "Analysis-TypeDef.hh"
#include "ClusterStudy-TypeDef.hh"
#include "TSystem.h"

int main(int argc, char** argv) {

  larlight::ana_processor mgr;

  mgr.add_input_file("../../../combined_02.root");
    
  mgr.set_input_rootdir("scanner");

  mgr.set_output_file("");

  mgr.set_io_mode(larlight::storage_manager::READ);
  
  mgr.set_ana_output_file("");
  
  larlight::ClusterMerger* proc = new larlight::ClusterMerger;;

  proc->GetManager().AddMergeAlgo(new cluster::CMAlgoFake());

  proc->GetManager().AddSeparateAlgo(new cluster::CMAlgoFake());

  proc->GetManager().DebugMode(cluster::CMergeManager::kPerIteration);
  //proc.GetManager().DebugMode(cluster.CMergeManager.kPerAlgoSet);
  //proc.GetManager().DebugMode(cluster.CMergeManager.kPerMerging);

  mgr.add_process(proc);

  while (1) {
    
    mgr.process_event();

  };

  return 1;
}



