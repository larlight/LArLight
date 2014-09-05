#ifndef TMPCLUSTERANA_CC
#define TMPCLUSTERANA_CC

#include "tmpClusterAna.hh"

namespace kaleko {

  bool tmpClusterAna::initialize() {
    
    _ana_tree=0;
    PrepareTTree();
    
    return true;
  }
  
  bool tmpClusterAna::analyze(larlight::storage_manager* storage) {
    
    ClearTTreeVars();
    
    CalculateShit(storage);
    
    //fill TTree (once per event)
    if(_ana_tree)
      _ana_tree->Fill(); 
    
    return true;
  }
  
  bool tmpClusterAna::finalize() {
    
    if(_fout) {
      
      _fout->cd();
      if(_ana_tree)
	_ana_tree->Write();
      
    }
    else 
      print(larlight::MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    
    delete _ana_tree;
    
    return true;
  }
  
  
  void tmpClusterAna::PrepareTTree() {
    
    if(!_ana_tree) {
      _ana_tree = new TTree("ana_tree","");
      
      _ana_tree->Branch("n_clusters",&_n_clusters,"nclusters/I");
      _ana_tree->Branch("n_hits_tiny_clusters",&_n_hits_tiny_clusters,"nhits_tiny_clusters/I");
      _ana_tree->Branch("n_hits_big_clusters",&_n_hits_big_clusters,"nhits_big_clusters/I");
      _ana_tree->Branch("qtot_tiny_clusters",&_qtot_tiny_clusters,"qtot_tiny_clusters/D");
      _ana_tree->Branch("qtot_big_clusters",&_qtot_big_clusters,"qtot_big_clusters/D");
      
    }
  }
  
  void tmpClusterAna::ClearTTreeVars(){
    
    _n_clusters = 0;
    _n_hits_tiny_clusters = 0;
    _n_hits_big_clusters = 0;
    _qtot_tiny_clusters = 0;
    _qtot_big_clusters = 0;
    
  }
  
  
  void tmpClusterAna::CalculateShit(larlight::storage_manager* storage){
    
    //grab the reconstructed clusters
    larlight::event_cluster* ev_cluster = (larlight::event_cluster*)storage->get_data(larlight::DATA::Cluster);
    if(!ev_cluster) {
      print(larlight::MSG::ERROR,__FUNCTION__,Form("Did not find specified data product, Cluster!"));
      return;
    }
    
    //grab the reconstructed hits
    const larlight::DATA::DATA_TYPE hit_type = ev_cluster->get_hit_type();
    larlight::event_hit* ev_hits = (larlight::event_hit*)storage->get_data(hit_type);
    if(!ev_hits) {
      print(larlight::MSG::ERROR,__FUNCTION__,Form("Did not find specified data product, Hits!"));
      return;
    }
    
    //loop over the reconstructed clusters
    for(auto const i_cluster: *ev_cluster){

      auto const& ass_index = i_cluster.association(hit_type);
            
      const UChar_t plane = larutil::Geometry::GetME()->ChannelToPlane(ev_hits->at((*ass_index.begin())).Channel());
       
      //for now, only look at plane 0
      if(plane != 0) continue;
      
      _n_clusters++;
      
      //if this cluster has <10 hits, it is a "tiny" cluster
      if(ass_index.size()<10){
	for(auto const index : ass_index) {
	  _n_hits_tiny_clusters++;
	  _qtot_tiny_clusters += ev_hits->at(index).Charge();
	}
      }
      //else this cluster is a "big" cluster
      else{
	for(auto const index : ass_index) {
	  _n_hits_big_clusters++;
	  _qtot_big_clusters += ev_hits->at(index).Charge();
	}
      }
      
    } //end loop over ev_clusters
    
    return;
    
  }
  
}
#endif
