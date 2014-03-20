//
// Example C++ routine to run your analysis module, ClusterRecoUtil.
//

#include "ClusterRecoUtil-TypeDef.hh"
#include "ClusterRecoAlg-TypeDef.hh"
#include "Base-TypeDef.hh"
#include "DataFormat-TypeDef.hh"
#include <iostream>

int main(int argc, char** argv){

  larlight::storage_manager mgr;

  if(argc<2){
    std::cerr<<"Need input file name!"<<std::endl;
    return 1;
  }
  mgr.add_in_filename(argv[1]);
  
  if(argc>2)
    mgr.set_in_rootdir(argv[2]);

  mgr.set_io_mode(larlight::storage_manager::READ);

  mgr.open();

  cluster::ClusterParamsExecutor algo;

  while(mgr.next_event()) {

    auto clusters = (larlight::event_cluster*)(mgr.get_data(larlight::DATA::ShowerAngleCluster));
    auto hits = (const larlight::event_hit*)(mgr.get_data(clusters->get_hit_type()));

    for(auto const &c : *clusters) {

      algo.LoadCluster(c,hits);

    }

  }
  return 0;
}
