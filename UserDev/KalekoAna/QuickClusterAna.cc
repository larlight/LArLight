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

    _hit_angles_forwards =0;
    _hit_angles_backwards=0;    
    PrepareHistos();
    
    myNeuralNetPath = std::getenv("MAKE_TOP_DIR") + std::string("/UserDev/FANN/trained_nets/cascade_net.net");

    w2cm = larutil::GeometryUtilities::GetME()->WireToCm();
    t2cm = larutil::GeometryUtilities::GetME()->TimeToCm();
    
    temp_tomerge_count = 0;
    temp_total_good = 0;

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
    tempcounter[0] = 0;
    tempcounter[1] = 0;
    tempcounter[2] = 0;

    FillClusterParamsVector(storage);

    //    std::cout<<Form("The number of clusters that passed this round of cuts is: (%d,%d,%d).",tempcounter[0],tempcounter[1],tempcounter[2])<<std::endl;
    for(int i=0; i<3; ++i){
      temp_total_good+=tempcounter[i];
      if(tempcounter[i] >= 2)
	temp_tomerge_count+=tempcounter[i];
    }

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
    

    std::cout<<Form("\n\nAfter all this, %d total clusters pass the reco cuts while %d clusters pass cuts && have another cluster in same view that passes cuts.\n\n",temp_total_good,temp_tomerge_count);

    return true;
  }
  
  void QuickClusterAna::PrepareTTree() {
    
    if(!_ana_tree) {
      _ana_tree = new TTree("ana_tree","");
      //      _ana_tree->Branch("nhits","std::vector<int>",&_nhits);
      //      _ana_tree->Branch("cluscharge","std::vector<double>",&_cluscharge);
      //      _ana_tree->Branch("mc_mother_energy","std::vector<double>",&_mc_mother_energy);
      
      _ana_tree->Branch("nhits",&_nhits,"nhits/I");
      _ana_tree->Branch("cluscharge",&_cluscharge,"cluscharge/D");
      _ana_tree->Branch("mc_mother_energy",&_mc_mother_energy,"mc_mother_energy/D");
      _ana_tree->Branch("mc_angle_2d",&_mc_angle_2d,"mc_angle_2d/D");
      _ana_tree->Branch("view",&_view,"view/I");
      _ana_tree->Branch("dk_good_clus",&_dk_good_clus,"dk_good_clus/O");
      _ana_tree->Branch("cluslength",&_cluslength,"cluslength/D");
      _ana_tree->Branch("poly_area",&_poly_area,"poly_area/D");
      _ana_tree->Branch("coneness",&_coneness,"coneness/D");
      

      _ana_tree->Branch("sum_charge",&_sum_charge,"sum_charge/D");
      _ana_tree->Branch("mean_charge",&_mean_charge,"mean_charge/D");
      //      _ana_tree->Branch("mean_x",&_mean_x,"mean_x/D");
      //      _ana_tree->Branch("mean_y",&_mean_y,"mean_y/D");
      //      _ana_tree->Branch("rms_x",&_rms_x,"rms_x/D");
      //      _ana_tree->Branch("rms_y",&_rms_y,"rms_y/D");
      //      _ana_tree->Branch("charge_wgt_x",&_charge_wgt_x,"charge_wgt_x/D");
      //      _ana_tree->Branch("charge_wgt_y",&_charge_wgt_y,"charge_wgt_y/D");
      _ana_tree->Branch("cluster_angle_2d",&_cluster_angle_2d,"cluster_angle_2d/D");
      _ana_tree->Branch("angle_2d",&_angle_2d,"angle_2d/D");
      _ana_tree->Branch("opening_angle",&_opening_angle,"opening_angle/D");
      _ana_tree->Branch("opening_angle_charge_wgt",&_opening_angle_charge_wgt,"opening_angle_charge_wgt/D");
      _ana_tree->Branch("closing_angle",&_closing_angle,"closing_angle/D");
      _ana_tree->Branch("closing_angle_charge_wgt",&_closing_angle_charge_wgt,"closing_angle_charge_wgt/D");
      _ana_tree->Branch("eigenvalue_principal",&_eigenvalue_principal,"eigenvalue_principal/D");
      _ana_tree->Branch("eigenvalue_secondary",&_eigenvalue_secondary,"eigenvalue_secondary/D");
      //      _ana_tree->Branch("verticalness",&_verticalness,"verticalness/D");
      _ana_tree->Branch("length",&_length,"length/D");
      _ana_tree->Branch("width",&_width,"width/D");
      _ana_tree->Branch("hit_density_1D",&_hit_density_1D,"hit_density_1D/D");
      _ana_tree->Branch("hit_density_2D",&_hit_density_2D,"hit_density_2D/D");
      _ana_tree->Branch("multi_hit_wires",&_multi_hit_wires,"multi_hit_wires/D");
      _ana_tree->Branch("N_Wires",&_N_Wires,"N_Wires/D");
      _ana_tree->Branch("modified_hit_density",&_modified_hit_density,"modified_hit_density/D");
      _ana_tree->Branch("N_Hits",&_N_Hits,"N_Hits/D");
      _ana_tree->Branch("N_Hits_HC",&_N_Hits_HC,"N_Hits_HC/D");
      //      _ana_tree->Branch("direction",&_direction,"direction/I");
      _ana_tree->Branch("showerness",&_showerness,"showerness/D");
      _ana_tree->Branch("trackness",&_trackness,"trackness/D");  
      _ana_tree->Branch("offaxis_hits",&_offaxis_hits,"offaxis_hits/D"); 
      
      //    larutil::PxPoint start_point;      ///< start point 
      //    larutil::PxPoint end_point;        ///< end point
  
    }
  }
  
  void QuickClusterAna::ClearTTreeVars(){
    
    //  _nhits.clear();
    //    _cluscharge.clear();
    //    _mc_mother_energy.clear();


    _hit_angles_forwards -> Reset();
    _hit_angles_backwards-> Reset();

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
      
      auto const& ass_index = i_cluster.association(hit_type);
      if(ass_index.size()<15) continue;

      const UChar_t plane = larutil::Geometry::GetME()->ChannelToPlane(ev_hits->at((*ass_index.begin())).Channel());

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
	(*_clusterparams.rbegin()).FillPolygon();
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


      double clus_q = 0;
      
      //list of the hits's indicies associated with this cluster
      auto const& hit_index = ev_cluster->at(clus_index).association(hit_type);
      
      //skip the cluster if there are <15 hits. don't increment params_index
      if( (int)hit_index.size() < 15) continue;
      
      //loop over the hits associated with this cluster, to sum up the charge
      //and to determine the "coneness" of the cluster
      double cosangle = 99999.;
      for(auto const &index : hit_index){
	//sum the charge from all the hits in this cluster
	clus_q += ev_hits->at(index).Charge();

	
	double SEP_x  = (ev_cluster->at(clus_index).EndPos().at(0) 
			 - ev_cluster->at(clus_index).StartPos().at(0)) * w2cm;
	double SEP_y  = (ev_cluster->at(clus_index).EndPos().at(1) 
			 - ev_cluster->at(clus_index).StartPos().at(1)) * t2cm;
	double SHIT_x = (ev_hits->at(index).Wire()     - ev_cluster->at(clus_index).StartPos().at(0)) * w2cm;
	double SHIT_y = (ev_hits->at(index).PeakTime() - ev_cluster->at(clus_index).StartPos().at(1)) * t2cm;    
	
	cosangle = ( SEP_x*SHIT_x + SEP_y*SHIT_y );
	cosangle = cosangle / (
			       pow( pow(SEP_x,2)+ pow(SEP_y,2) ,0.5) *
			       pow( pow(SHIT_x,2)+pow(SHIT_y,2),0.5)
			       );
	
	_hit_angles_forwards->Fill(cosangle,ev_hits->at(index).Charge());
	
	//now switch the vector to be from end point to start point and re-do
	SEP_x  = (ev_cluster->at(clus_index).StartPos().at(0)    - ev_cluster->at(clus_index).EndPos().at(0)) * w2cm;
	SEP_y  = (ev_cluster->at(clus_index).StartPos().at(1)    - ev_cluster->at(clus_index).EndPos().at(1)) * t2cm;
	SHIT_x = (ev_hits->at(index).Wire()     - ev_cluster->at(clus_index).EndPos().at(0)) * w2cm;
	SHIT_y = (ev_hits->at(index).PeakTime() - ev_cluster->at(clus_index).EndPos().at(1)) * t2cm;        
	
	cosangle = ( SEP_x*SHIT_x + SEP_y*SHIT_y );
	cosangle = cosangle / (
			       pow( pow(SEP_x,2)+ pow(SEP_y,2) ,0.5) *
			       pow( pow(SHIT_x,2)+pow(SHIT_y,2),0.5)
			       );
	
	_hit_angles_backwards->Fill(cosangle,ev_hits->at(index).Charge());
	
      }      
      
      //      _cluscharge.push_back(clus_q);
      //      _nhits.push_back((int)hit_index.size());
      _cluscharge=clus_q;
      _nhits=(int)hit_index.size();
      double forward_param = _hit_angles_forwards->GetRMS() / _hit_angles_forwards->GetMean();
      double backward_param= _hit_angles_backwards->GetRMS()/ _hit_angles_backwards->GetMean();
      _coneness = forward_param > backward_param ? forward_param : backward_param;

      //      std::cout<<Form("clus_index is %d, param_index is %d\n",clus_index,params_index);
      //fill params from clusterparamsalgnew into ttree variab<les
      //const cluster::cluster_params tmp = _clusterparams.at(params_index).GetParams();
      cluster::cluster_params tmp = _clusterparams.at(params_index).GetParams();
      _poly_area= tmp.PolyObject.Area();
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
      

      _cluslength=larutil::GeometryUtilities::GetME()->Get2DDistance(&tmp.start_point,
								     &tmp.end_point);
      //find out if dk thinks this cluster is a good one
      _dk_good_clus = DKShittyClusterTest(main_shower,ev_cluster->at(clus_index));
      //      if(_dk_good_clus)
	//	std::cout<<"mc says cluster "<<params_index<<" in view "<<_view+1<<" is GOOD"<<std::endl;
	//      else
	//	std::cout<<"cluster "<<params_index<<" in view "<<_view+1<<" is BAD"<<std::endl;

      if(FinalRecoCutTest(ev_cluster->at(clus_index)))
	tempcounter[_view]++;

      //fill TTree (once per cluster)
      if(_ana_tree)
	_ana_tree->Fill(); 

      params_index++;
    } //end loop over reco cluster vector and _clusterparams vector

            
  }
  
  void QuickClusterAna::PrepareHistos() {

    if(!_hit_angles_forwards)
      _hit_angles_forwards = new TH1F("_hit_angles_forwards","_hit_angles_forwards",100,-1.,1.);
    if(!_hit_angles_backwards)
      _hit_angles_backwards = new TH1F("_hit_angles_backwards","_hit_angles_backwards",100,-1.,1.);
    
  }
  
  bool QuickClusterAna::DKShittyClusterTest(const larlight::mcshower &main_shower,
					    const larlight::cluster &i_cluster){
    bool is_good = true;
   
    //insert code here
    // Kazu: "Maybe you can cut on clusters that have same direction as MCShower 
    // (2D projected direction) + and minimum hit/charge to see how some parameters
    // look like for "good" clusters.
    // so we can use tree.draw("","_dk_good_clus==1","")

    //to be a "good" cluster (at first glance):
    //must have >10k charge
    is_good = is_good && _cluscharge > 10000;
    //must have >30 hits
    is_good = is_good && _nhits > 30;
    //must have opening angle in [0,pi/2] (otherwise start point is probably bad)
    is_good = is_good && (_opening_angle > 0 && _opening_angle < 1.570795);
    //must have MC axis angle different than reco angle by less than half of the opening angle (axis pointing in somewhat of the right direction)
    is_good = is_good && (
			      std::abs(_mc_angle_2d - _angle_2d) < 0.5*_opening_angle*(180/3.14159) ||
			      std::abs(_mc_angle_2d - _angle_2d - 180) < 0.5*_opening_angle*(180/3.14159) ||
			      std::abs(_mc_angle_2d - _angle_2d + 180) < 0.5*_opening_angle*(180/3.14159)
			      );



    //    is_good = is_good && std::abs(_mc_angle_2d - _angle_2d) < 10;
    

    return is_good;
  }//end DKShittyClusterTest


  bool QuickClusterAna::FinalRecoCutTest(const larlight::cluster &i_cluster){

    bool is_good = true;
    

    //these values are determiend by drawing abs(mcangle-2dangle):recoparam
    //and seeing what cut on recoparam isolates the low-angle-difference bits
    is_good = is_good && _cluscharge > 50000;
    is_good = is_good && _nhits > 30;
    is_good = is_good && (_opening_angle > 0 && _opening_angle < 0.40);
    is_good = is_good && _cluslength > 20;

    return is_good;

  }
}
#endif
