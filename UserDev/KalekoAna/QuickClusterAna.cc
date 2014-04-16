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

    myNeuralNetPath = std::getenv("MAKE_TOP_DIR") + std::string("/UserDev/FANN/trained_nets/cascade_net.net");
    
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
      //      _ana_tree->Branch("mc_mother_energy","std::vector<double>",&_mc_mother_energy);
      
      _ana_tree->Branch("nhits",&_nhits,"_nhits/I");
      _ana_tree->Branch("cluscharge",&_cluscharge,"_cluscharge/D");
      _ana_tree->Branch("mc_mother_energy",&_mc_mother_energy,"_mc_mother_energy/D");
      _ana_tree->Branch("mc_angle_2d",&_mc_angle_2d,"_mc_angle_2d/D");
      _ana_tree->Branch("view",&_view,"_view/I");
      _ana_tree->Branch("dk_good_clus",&_dk_good_clus,"_dk_good_clus/O");
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
    //    _mc_mother_energy.clear();
    
  }
  
  
  void QuickClusterAna::FillClusterParamsVector(larlight::storage_manager* storage){
    
    //grab the reconstructed clusters
    //note, ShowerAngleCluster has a track-like cut on clusters, which frequently gets rid 
    // of the first "best" cluster starting from the origin of the shower vertex.
    larlight::event_cluster* ev_cluster = (larlight::event_cluster*)storage->get_data(larlight::DATA::FuzzyCluster);
    //larlight::event_cluster* ev_cluster = (larlight::event_cluster*)(storage->get_data(larlight::DATA::ShowerAngleCluster));
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
    //    _clusterparams.reserve(ev_cluster->size());

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
	(*_clusterparams.rbegin()).setNeuralNetPath(myNeuralNetPath);
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

    return;
    
  }

  
  void QuickClusterAna::CalculateTTreeVars(larlight::event_cluster* ev_cluster, 
					   larlight::event_mcshower* ev_mcshower, 
					   larlight::event_hit* ev_hits, 
					   const std::vector<cluster::ClusterParamsAlgNew> &_clusterparams){
    
    //clear the TTree variables before doing any calculations (or, set to non-physical values
    //to indicate that somehting has gone wrong if you find those values in your output tree)
    ClearTTreeVars();

    //the size of _clusterparams will be less than the number of reco clusters, because we don't calculate the
    //reco params for clusters with <15 events.
    /*
    if((int)ev_cluster->size() != (int)_clusterparams.size()){
      print(larlight::MSG::ERROR,__FUNCTION__,Form("Something is wrong. Number of reco clusters in event (%d) isn't matching number of elements in _clusterparams vector (%d). I won't fill the ttree for this event.",(int)ev_cluster->size(),(int)_clusterparams.size()));
      return;
    }
    */

    //loop over MCShowers in the event (there should be only one for single electron events)
    larlight::mcshower main_shower;
    for(auto i_mcshower: *ev_mcshower){
    
      //sometimes there are more than one shower, cut out those
      if(i_mcshower.MotherMomentum().at(3) > 0.01){
	main_shower = i_mcshower;
	_mc_mother_energy = i_mcshower.MotherMomentum().at(3);
      }
    }
    //    _mc_mother_energy.push_back(mc_mother_energy);
    TVector3 dir_vector((main_shower.MotherMomentum()).at(0),
			(main_shower.MotherMomentum()).at(1),
			(main_shower.MotherMomentum()).at(2));
    double mag = dir_vector.Mag();
    TVector3 dir_vector_norm(dir_vector[0]/mag,
			     dir_vector[1]/mag,
			     dir_vector[2]/mag);

    const larlight::DATA::DATA_TYPE hit_type = ev_cluster->get_hit_type();
    larlight::GEO::View_t view = larlight::GEO::kUnknown;

    //loop over both reconstructed clusters and matching clusterparams vector at the same time
    //since there are more ev_clusters than elements in _clusterparams, 
    //clus_index is the current reco cluster you're on (even if it has <15 events)
    //and params_index is the current params you're on (there are fewer of these, all have >15 events)
    int params_index = 0;
    for(int clus_index = 0; clus_index < (int)ev_cluster->size(); clus_index++ ){

      //get some mc stuff that depends on which view the cluster is in
      view = ev_cluster->at(clus_index).View();
      _view = (int)view;

      //2dangle depends on which plane you're in -- use geometryservices
      _mc_angle_2d = larutil::GeometryUtilities::GetME()->Get2DangleFrom3D(view,dir_vector_norm);
      //convert to degrees
      _mc_angle_2d *= (180/3.14159);

      std::cout<<"_mc_angle_2d is "<<_mc_angle_2d<<" in view "<<_view<<std::endl;

      double clus_q = 0;
      
      //list of the hits's indicies associated with this cluster
      std::vector<unsigned short> hit_index(ev_cluster->at(clus_index).association(hit_type));
      
      //skip the cluster if there are <15 hits. don't increment params_index
      if( (int)hit_index.size() < 15) continue;
      
      //loop over the hits associated with this cluster, to sum up the charge
      for(auto const &index : hit_index){
	//sum the charge from all the hits in this cluster
	clus_q += ev_hits->at(index).Charge();
      }      
      
      //      _cluscharge.push_back(clus_q);
      //      _nhits.push_back((int)hit_index.size());
      _cluscharge=clus_q;
      _nhits=(int)hit_index.size();


      //      std::cout<<Form("clus_index is %d, param_index is %d\n",clus_index,params_index);
      //fill params from clusterparamsalgnew into ttree variables
      const cluster::cluster_params tmp = _clusterparams.at(params_index).GetParams();
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

      //find out if dk thinks this cluster is a good one
      _dk_good_clus = DKShittyClusterTest(main_shower,ev_cluster->at(clus_index));

      //fill TTree (once per cluster)
      if(_ana_tree)
	_ana_tree->Fill(); 

      params_index++;
    } //end loop over reco cluster vector and _clusterparams vector

            
  }
  

  bool QuickClusterAna::DKShittyClusterTest(const larlight::mcshower &main_shower,
					    const larlight::cluster &i_cluster){
    bool is_shitty = true;
   
    //insert code here
    // Kazu: "Maybe you can cut on clusters that have same direction as MCShower 
    // (2D projected direction) + and minimum hit/charge to see how some parameters
    // look like for "good" clusters.
    // so we can use tree.draw("","_dk_good_clus==1","")

    is_shitty = is_shitty && _cluscharge > 10000;

    is_shitty = is_shitty && _nhits > 30;

    is_shitty = is_shitty && (std::abs(_mc_angle_2d - _angle_2d) < 10);


    return is_shitty;
  }//end DKShittyClusterTest
}
#endif
