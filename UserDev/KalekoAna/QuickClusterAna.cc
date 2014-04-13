#ifndef QUICKCLUSTERANA_CC
#define QUICKCLUSTERANA_CC

#include "QuickClusterAna.hh"

namespace kaleko {
  
  bool QuickClusterAna::initialize() {
    
    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D"). 
    //
    
    _ana_tree=0;
    PrepareTTree();

    return true;
  }
  
  bool QuickClusterAna::analyze(larlight::storage_manager* storage) {
    
    //
    // Do your event-by-event analysis here. This function is called for 
    // each event in the loop. You have "storage" pointer which contains 
    // event-wise data. To see what is available, check the "Manual.pdf":
    //
    // http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
    // 
    // Or you can refer to Base/DataFormatConstants.hh for available data type
    // enum values. Here is one example of getting PMT waveform collection.
    //
    // larlight::event_fifo* my_pmtfifo_v = (larlight::event_fifo*)(storage->get_data(larlight::DATA::PMFIFO));
    //
    // if( event_fifo )
    //
    //   std::cout << "Event ID: " << my_pmtfifo_v->event_id() << std::endl;
    //
  


    //***********************************************************//
    //implement your own functions w/ loops, etc seperately here!// 
    //they should calculate variables (in this scope) that you   //
    //want to save to the ttree (filled once per analyze loop)   // 
    //***********************************************************//
    FillClusterParamsVector(storage);
    
    return true;
  }
  
  bool QuickClusterAna::finalize() {
    
    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
    //
    // Say you made a histogram pointer h1 to store. You can do this:
    //
    // if(_fout) { _fout->cd(); h1->Write(); }
    //
    // else 
    //   print(larlight::MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    //
    
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
  
  void QuickClusterAna::PrepareTTree() {
    
    if(!_ana_tree) {
      _ana_tree = new TTree("ana_tree","");
      //      _ana_tree->Branch("nhits","std::vector<int>",&_nhits);
      //      _ana_tree->Branch("cluscharge","std::vector<double>",&_cluscharge);
      //      _ana_tree->Branch("mother_energy","std::vector<double>",&_mother_energy);
      _ana_tree->Branch("nhits",&_nhits,"_nhits/I");
      _ana_tree->Branch("cluscharge",&_cluscharge,"_cluscharge/D");
      _ana_tree->Branch("mother_energy",&_mother_energy,"_mother_energy/D");
      _ana_tree->Branch("sum_charge",&_sum_charge,"_sum_charge/D");
      _ana_tree->Branch("mean_charge",&_mean_charge,"_mean_charge/D");
      _ana_tree->Branch("mean_x",&_mean_x,"_mean_x/D");
      _ana_tree->Branch("mean_y",&_mean_y,"_mean_y/D");
      _ana_tree->Branch("rms_x",&_rms_x,"_rms_x/D");
      _ana_tree->Branch("rms_y",&_rms_y,"_rms_y/D");
      _ana_tree->Branch("charge_wgt_x",&_charge_wgt_x,"_charge_wgt_x/D");
      _ana_tree->Branch("charge_wgt_y",&_charge_wgt_y,"_charge_wgt_y/D");
      _ana_tree->Branch("cluster_angle_2d",&_cluster_angle_2d,"_cluster_angle_2d/D");
      _ana_tree->Branch("angle_2d",&_angle_2d,"_angle_2d/D");
      _ana_tree->Branch("opening_angle",&_opening_angle,"_opening_angle/D");
      _ana_tree->Branch("opening_angle_charge_wgt",&_opening_angle_charge_wgt,"_opening_angle_charge_wgt/D");
      _ana_tree->Branch("closing_angle",&_closing_angle,"_closing_angle/D");
      _ana_tree->Branch("closing_angle_charge_wgt",&_closing_angle_charge_wgt,"_closing_angle_charge_wgt/D");
      _ana_tree->Branch("eigenvalue_principal",&_eigenvalue_principal,"_eigenvalue_principal/D");
      _ana_tree->Branch("eigenvalue_secondary",&_eigenvalue_secondary,"_eigenvalue_secondary/D");
      _ana_tree->Branch("verticalness",&_verticalness,"_verticalness/D");
      _ana_tree->Branch("length",&_length,"_length/D");
      _ana_tree->Branch("width",&_width,"_width/D");
      _ana_tree->Branch("hit_density_1D",&_hit_density_1D,"_hit_density_1D/D");
      _ana_tree->Branch("hit_density_2D",&_hit_density_2D,"_hit_density_2D/D");
      _ana_tree->Branch("multi_hit_wires",&_multi_hit_wires,"_multi_hit_wires/D");
      _ana_tree->Branch("N_Wires",&_N_Wires,"_N_Wires/D");
      _ana_tree->Branch("modified_hit_density",&_modified_hit_density,"_modified_hit_density/D");
      _ana_tree->Branch("N_Hits",&_N_Hits,"_N_Hits/D");
      _ana_tree->Branch("N_Hits_HC",&_N_Hits_HC,"_N_Hits_HC/D");
      _ana_tree->Branch("direction",&_direction,"_direction/I");
      _ana_tree->Branch("showerness",&_showerness,"_showerness/D");
      _ana_tree->Branch("trackness",&_trackness,"_trackness/D");  
      _ana_tree->Branch("offaxis_hits",&_offaxis_hits,"_offaxis_hits/D"); 
      
      //    larutil::PxPoint start_point;      ///< start point 
      //    larutil::PxPoint end_point;        ///< end point
  
    }
  }
  
  void QuickClusterAna::ClearTTreeVars(){
    
    //  _nhits.clear();
    //    _cluscharge.clear();
    //    _mother_energy.clear();
    
  }
  
  
  void QuickClusterAna::FillClusterParamsVector(larlight::storage_manager* storage){
    
    //grab the reconstructed clusters
    //larlight::event_cluster* ev_cluster = (larlight::event_cluster*)storage->get_data(larlight::DATA::FuzzyCluster);
    larlight::event_cluster* ev_cluster = (larlight::event_cluster*)(storage->get_data(larlight::DATA::ShowerAngleCluster));
    if(!ev_cluster) {
      print(larlight::MSG::ERROR,__FUNCTION__,Form("Did not find specified data product, FuzzyCluster!"));
      return;
    }
    
    //grab the reconstructed hits
    const larlight::DATA::DATA_TYPE hit_type = ev_cluster->get_hit_type();
    larlight::event_hit* ev_hits = (larlight::event_hit*)storage->get_data(hit_type);
    if(!ev_hits) {
      print(larlight::MSG::ERROR,__FUNCTION__,Form("Did not find specified data product, Hits!"));
      return;
    }
      
    //grab the MC showers
    larlight::event_mcshower* ev_mcshower = (larlight::event_mcshower*)storage->get_data(larlight::DATA::MCShower);
    if(!ev_mcshower) {
      print(larlight::MSG::ERROR,__FUNCTION__,Form("Did not find specified data product, MCShower!"));
      return;
    }


    // Now we make ClusterParamsAlgNew instance per cluster ... so initialize _clusterparams vector to its expected length;

    _clusterparams.clear();
    _clusterparams.reserve(ev_cluster->size());
    
    //loop over the reconstructed clusters
    for(auto const i_cluster: *ev_cluster){
      
      const std::vector<unsigned short> ass_index(i_cluster.association(hit_type));
      if(ass_index.size()<15) continue;

      const UChar_t plane = larutil::Geometry::GetME()->ChannelToPlane(ev_hits->at((*ass_index.begin())).Channel());
      const Double_t w2cm = larutil::GeometryUtilities::GetME()->WireToCm();
      const Double_t t2cm = larutil::GeometryUtilities::GetME()->TimeToCm();

      // Section A: Compute cluster parameters (ClusterParamsAlgNew) and store (_clusterparams)
      std::vector<larutil::PxHit> tmp_hits;
      tmp_hits.reserve(ass_index.size());

      for(auto const index : ass_index) {	
	larutil::PxHit h;
	h.w = ev_hits->at(index).Wire() * w2cm;
	h.t = ev_hits->at(index).PeakTime() * t2cm;
	h.plane = plane;
	h.charge = ev_hits->at(index).Charge();
	tmp_hits.push_back(h);
      }
      
      _clusterparams.push_back(::cluster::ClusterParamsAlgNew());
	
      try {
	(*_clusterparams.rbegin()).SetHits(tmp_hits);
	//	(*_clusterparams.rbegin()).FillPolygon();
	(*_clusterparams.rbegin()).FillParams();
      }catch( cluster::RecoUtilException) {
	
	print(larlight::MSG::ERROR,__FUNCTION__,Form("Cluster %d too bad to run ClusterParamsAlgNew!",i_cluster.ID()));
      }
      
    } //end loop over ev_cluster
    
    
    //
    // Section A done
    //
    

    CalculateTTreeVars(ev_cluster,ev_mcshower,ev_hits,_clusterparams);

    //_clusterparams.clear();
    return;
    
  }

  
  void QuickClusterAna::CalculateTTreeVars(larlight::event_cluster* ev_cluster, 
					   larlight::event_mcshower* ev_mcshower, 
					   larlight::event_hit* ev_hits, 
					   const std::vector<cluster::ClusterParamsAlgNew> &_clusterparams){
    
    //clear the TTree variables before doing any calculations (or, set to non-physical values
    //to indicate that somehting has gone wrong if you find those values in your output tree)
    ClearTTreeVars();
    

    
    if((int)ev_cluster->size() != (int)_clusterparams.size()){
      print(larlight::MSG::ERROR,__FUNCTION__,Form("Something is wrong. Number of reco clusters in event isn't matching number of elements in _clusterparams vector."));
      return;
    }


    //loop over MCShowers in the event (there should be only one for single electron events)
    double mother_energy;
   
    for(auto i_mcshower: *ev_mcshower){
    
      //sometimes there are more than one shower, cut out those
      if(i_mcshower.MotherMomentum()[3] > 0.01)
	mother_energy = i_mcshower.MotherMomentum()[3];
    
    }
    //    _mother_energy.push_back(mother_energy);
    _mother_energy=mother_energy;

    
 
   

    //loop over both reconstructed clusters and matching clusterparams vector at the same time
    const larlight::DATA::DATA_TYPE hit_type = ev_cluster->get_hit_type();

    for(int i = 0; i < (int)ev_cluster->size(); i++){

      double clus_q = 0;
      
      //list of the hits's indicies associated with this cluster
      std::vector<unsigned short> hit_index(ev_cluster->at(i).association(hit_type));
      
      //loop over the hits associated with this cluster, to sum up the charge
      for(auto const &index : hit_index){
	//sum the charge from all the hits in this cluster
	clus_q += ev_hits->at(index).Charge();
      }      
      
      //      _cluscharge.push_back(clus_q);
      //      _nhits.push_back((int)hit_index.size());
      _cluscharge=clus_q;
      _nhits=(int)hit_index.size();

      //fill params from clusterparamsalgnew into ttree variables
      const cluster::cluster_params tmp = _clusterparams.at(i).GetParams();
      _sum_charge= tmp.sum_charge;
      _mean_charge= tmp.mean_charge;      
      _mean_x= tmp.mean_x;                     
      _mean_y= tmp.mean_y;                     
      _rms_x= tmp.rms_x;                      
      _rms_y= tmp.rms_y;                      
      _charge_wgt_x= tmp.charge_wgt_x;               
      _charge_wgt_y= tmp.charge_wgt_y;               
      _cluster_angle_2d= tmp.cluster_angle_2d;           
      _angle_2d= tmp.angle_2d;                   
      _opening_angle= tmp.opening_angle;              
      _opening_angle_charge_wgt= tmp.opening_angle_charge_wgt;   
      _closing_angle= tmp.closing_angle;              
      _closing_angle_charge_wgt= tmp.closing_angle_charge_wgt;   
      _eigenvalue_principal= tmp.eigenvalue_principal;       
      _eigenvalue_secondary= tmp.eigenvalue_secondary;       
      _verticalness= tmp.verticalness;               
      _length= tmp.length;			
      _width= tmp.width;			
      _hit_density_1D= tmp.hit_density_1D;		
      _hit_density_2D= tmp.hit_density_2D;		
      _multi_hit_wires= tmp.multi_hit_wires;		
      _N_Wires= tmp.N_Wires;			
      _modified_hit_density= tmp.modified_hit_density;	
      _N_Hits= tmp.N_Hits;			
      _N_Hits_HC= tmp.N_Hits_HC;			
      _direction= tmp.direction;			
      _showerness= tmp.showerness;                 
      _trackness= tmp.trackness;                  
      _offaxis_hits= tmp.offaxis_hits;

      //fill TTree (once per cluster)
      if(_ana_tree) _ana_tree->Fill(); 
      
    } //end loop over reco cluster vector and _clusterparams vector

            
  }
  

}
#endif
