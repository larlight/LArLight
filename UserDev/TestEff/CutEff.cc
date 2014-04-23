#ifndef CUTEFF_CC
#define CUTEFF_CC

#include "CutEff.hh"
// #include "../ClusterRecoUtil/ClusterParamsAlgNew.hh"
#include "ClusterParams.hh"
#include "LArUtilManager.hh"

namespace larlight {

  bool CutEff::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D"). 
    //   
//    TH1D *hist1 = new TH1D("hist1","title",Nbins,lowerBin,upperBin) ;	
    larutil::LArUtilManager::Reconfigure(larlight::GEO::kArgoNeuT);   
    if(!fGSer) fGSer = (larutil::GeometryUtilities*)(larutil::GeometryUtilities::GetME());

    //////////////////////////Setting up the TTree
    fMainTree=new TTree("testeff","testeff");
    
    fMainTree->Branch("run",&fRun,"run/I");
    fMainTree->Branch("subrun",&fSubRun,"subrun/I");
    fMainTree->Branch("event",&fEvent,"event/I");
    fMainTree->Branch("nplanes",&fNPlanes,"nplanes/I");
    
    
    fMainTree->Branch("nclusters",&fNClusters,"nclusters/I");
    fMainTree->Branch("nparticles",&fNParticles,"nparticles/I");
    
    
    //These are defined per MCparticle  
    fMainTree->Branch("MCpdgOrig","std::vector< int>", &fMCPDGstart);  
    fMainTree->Branch("MCPDGisPrimary","std::vector< int>", &fMCPDGisPrimary);
    fMainTree->Branch("MCenergyOrig","std::vector<double>", &fMCenergystart);
    fMainTree->Branch("MCphiOrig","std::vector< double >", &fMCPhistart);
    fMainTree->Branch("MCthetaOrig","std::vector<double >", &fMCThetastart);
    fMainTree->Branch("MCXOrig","std::vector<double >", &fMCXOrig);
    fMainTree->Branch("MCYOrig","std::vector<double >", &fMCYOrig);
    fMainTree->Branch("MCZOrig","std::vector<double >", &fMCZOrig);
    
    
    //These are defined per cluster
    fMainTree->Branch("mcpdg","std::vector< int>", &mcpdg);
    fMainTree->Branch("mcenergy","std::vector<double>", &mcenergy);
    fMainTree->Branch("mcphi","std::vector< double >", &mcphi);
    fMainTree->Branch("mctheta","std::vector<double >", &mctheta);
    fMainTree->Branch("mcomega","std::vector<double >", &mcomega);
    fMainTree->Branch("mcx","std::vector< double >", &mcx);
    fMainTree->Branch("mcy","std::vector<double >", &mcy);
    fMainTree->Branch("mcz","std::vector<double >", &mcz);
    
    fMainTree->Branch("mcdirection","std::vector< int>", &mcdirection);
    //  fMainTree->Branch("mcenergyfraction","std::vector< double>", &mcenergyfraction);
    fMainTree->Branch("mcwirevertex","std::vector<unsigned int>", &mcwirevertex);
    fMainTree->Branch("mctimevertex","std::vector<double>", &mctimevertex);
    //    fMainTree->Branch("mcdistfromorigin","std::vector< double>", &mcdistfromorigin);
    
    
    ///////////// reconstructed quantities
    fMainTree->Branch("plane","std::vector< int>", &plane);
    fMainTree->Branch("wire_vertex","std::vector<unsigned int>", &fWireVertex);
    fMainTree->Branch("time_vertex","std::vector<double>", &fTimeVertex);
    
    fMainTree->Branch("wire_last","std::vector<unsigned int>", &fWireLast);
    fMainTree->Branch("time_last","std::vector<double>", &fTimeLast);
    
    fMainTree->Branch("fDirection","std::vector<int>", &fDirection);
    
    fMainTree->Branch("fOffAxis","std::vector<double>", &fOffAxis);
    fMainTree->Branch("fOffAxisNorm","std::vector<double>", &fOffAxisNorm);
    fMainTree->Branch("fNhits","std::vector<int>", &fNhits);
    
    fMainTree->Branch("fHitDensity","std::vector<double>", &fHitDensity);
    fMainTree->Branch("fLength","std::vector<double>", &fLength);
    fMainTree->Branch("fWidth","std::vector<double>", &fWidth);  
    
    // fMainTree->Branch("fOffAxisNormHD","std::vector<double>", &fOffAxisNormHD);
    // fMainTree->Branch("fOnAxisNormHD","std::vector<double>", &fOnAxisNormHD);
    
    
    fMainTree->Branch("fPrincipal","std::vector<double>", &fPrincipal);
    fMainTree->Branch("slope2D","std::vector<double>", &slope2D);
    fMainTree->Branch("fMultiHitWires","std::vector<int>", &fMultiHitWires);
       
    fMainTree->Branch("sumCharge","std::vector<double>",&fsumCharge);
    fMainTree->Branch("meanCharge","std::vector<double>",&fmeanCharge );
    fMainTree->Branch("rmsX","std::vector<double>",&frmsX );
    fMainTree->Branch("rmsY","std::vector<double>",&frmsY );
    fMainTree->Branch("meanX","std::vector<double>",&fmeanX );
    fMainTree->Branch("meanY","std::vector<double>",&fmeanY );
    fMainTree->Branch("chargeWgtX","std::vector<double>",&fchargeWgtX );
    fMainTree->Branch("chargeWgtY","std::vector<double>",&fchargeWgtY );
    fMainTree->Branch("clusterAngle2d","std::vector<double>",&fclusterAngle2d );
    fMainTree->Branch("angle2d","std::vector<double>",&fangle2d );
    fMainTree->Branch("openAngle","std::vector<double>",&fopenAngle );
    fMainTree->Branch("openAngleChargeWgt","std::vector<double>",&fopenAngleChargeWgt );
    fMainTree->Branch("closeAngle","std::vector<double>",&fcloseAngle );
    fMainTree->Branch("closeAngleChargeWgt","std::vector<double>",&fcloseAngleChargeWgt );
    fMainTree->Branch("hitDensity1d","std::vector<double>",&fhitDensity1d );
    fMainTree->Branch("hitDensity2d","std::vector<double>",&fhitDensity2d );
    fMainTree->Branch("modifiedHitDens","std::vector<double>",&fmodifiedHitDens );
    fMainTree->Branch("ModifiedMeanCharge","std::vector<double>",&fModifiedMeanCharge );
    fMainTree->Branch("ChargeRMS","std::vector<double>",&fChargeRMS );
    
    
    fMainTree->Branch("fShowerness","std::vector<double>",&fShowerness );
    fMainTree->Branch("fTrackness","std::vector<double>",&fTrackness );
    
    fMainTree->Branch("max_charge","std::vector<double>",&max_charge );
    fMainTree->Branch("max_charge_3","std::vector<double>",&max_charge_3 );	
    fMainTree->Branch("PassCut","std::vector<bool>",&fPassCut );	
    
    
    fMainTree->SetAlias("electron","mcpgd==11");
    fMainTree->SetAlias("proton","mcpgd==2212");
    fMainTree->SetAlias("muon","mcpgd==13");
    fMainTree->SetAlias("collection","plane==1");
    fMainTree->SetAlias("induction","plane==0");	   
    fMainTree->SetAlias("good","fNhits>20");	   
    
    ////////////////////////////////////////////////

    
     
     
       
    if(!allEvts) allEvts = new TH1D("allEvts","allEvts",100,0,30);
    else allEvts->Reset();
    if(!allClusters) allClusters = new TH1D("allClusters","allClusters",100,0,30);
    else allClusters->Reset();
    
    if(!anyShowerClusters) anyShowerClusters = new TH1D("anyShowerClusters","anyShowerClusters",100,0,30);
    else anyShowerClusters->Reset();
    if(!anyShowerEvents) anyShowerEvents = new TH1D("anyShowerEvents","anyShowerEvents",100,0,30);
    else anyShowerEvents->Reset();
    
    if(!bothplaneShowerEvents) bothplaneShowerEvents = new TH1D("bothplaneShowerEvents","bothplaneShowerEvents",100,0,30);
    else bothplaneShowerEvents->Reset();
    if(!bothplaneShowerEventsmatched) bothplaneShowerEventsmatched = new TH1D("bothplaneShowerEventsmatched","bothplaneShowerEventsmatched",100,0,30);
    else bothplaneShowerEventsmatched->Reset();
    
    
    if(!anyShowerClusters_FANN) anyShowerClusters_FANN = new TH1D("anyShowerClusters_FANN","anyShowerClusters_FANN",100,0,30);
    else anyShowerClusters_FANN->Reset();
    if(!anyShowerEvents_FANN) anyShowerEvents_FANN = new TH1D("anyShowerEvents_FANN","anyShowerEvents_FANN",100,0,30);
    else anyShowerEvents_FANN->Reset();
    
    if(!bothplaneShowerEvents_FANN) bothplaneShowerEvents_FANN = new TH1D("bothplaneShowerEvents_FANN","bothplaneShowerEvents_FANN",100,0,30);
    else bothplaneShowerEvents_FANN->Reset();
    if(!bothplaneShowerEventsmatched_FANN) bothplaneShowerEventsmatched_FANN = new TH1D("bothplaneShowerEventsmatched_FANN","bothplaneShowerEventsmatched_FANN",100,0,30);
    else bothplaneShowerEventsmatched_FANN->Reset();
    
    
// 		
//     if(!chargeWgtX) chargeWgtX = new TH1D("chargeWgtX","Charge weighted x",120,10,100);
//     else chargeWgtX->Reset();
//     if(!chargeWgtY) chargeWgtY = new TH1D("chargeWgtY","Charge weigthed y",100,0,60);
//     else chargeWgtY->Reset();
//     
//     if(!clusterAngle2d) clusterAngle2d = new TH1D("clusterAngle2d","Cluster angle 2d",120,0,100);
//     else clusterAngle2d->Reset();
//     if(!angle2d) angle2d = new TH1D("angle2d","Angle 2d",150,-200,200);
//     else angle2d->Reset();
//     
//     if(!openAngle) openAngle = new TH1D("openAngle","Opening angle",100,0,2.25);
//     else openAngle->Reset();
//     if(!openAngleChargeWgt) openAngleChargeWgt = new TH1D("openAngleChargeWgt","Opening angle, charge weighted",100,0,2.0);
//     else openAngleChargeWgt->Reset();
//     
//     if(!closeAngleChargeWgt) closeAngleChargeWgt = new TH1D("closeAngleChargeWgt","Closing angle, charge weighted",120,0,2.25);
//     else closeAngleChargeWgt->Reset();
//     if(!closeAngle) closeAngle = new TH1D("closeAngle","Close angle",100,0,2.25);
//     else closeAngle->Reset();
// 
//     if(!hitDensity1d) hitDensity1d = new TH1D("hitDensity1d","Hit density 1d",120,0,10);
//     else hitDensity1d->Reset();
//     if(!hitDensity2d) hitDensity2d = new TH1D("hitDensity2d","Hit density 2d",100,0,60);
//     else hitDensity2d->Reset();
//     if(!modifiedHitDens) modifiedHitDens = new TH1D("modifiedHitDens","Modified Hit Density",100,0,20);
//     else modifiedHitDens->Reset();
// 
//     if(!trackness) trackness = new TH1D("trackness","trackness",100,0,1);
//     else trackness->Reset();
//     if(!showerness) showerness = new TH1D("showerness","Modified Hit Density",100,0,20);
//     else showerness->Reset();
// 
// 
// 
// //////////////////////////////////////////////////////////////
 //   recohists looking at reconstruction vs MCTruth
    if(!recostartwire) recostartwire = new TH1D("recostartwire","Start wire, reco - truth ",150,-50,50);
    else recostartwire->Reset();
    if(!recostarttime) recostarttime = new TH1D("recostarttime","Start time, reco - truth",150,-50,50);
    else recostarttime->Reset();
    if(!recostartwirenodir) recostartwirenodir = new TH1D("recostartwirenodir","Start wire, reco - truth, ignore bad direction ",150,-50,50);
    else recostartwirenodir->Reset();
    if(!recostarttimenodir) recostarttimenodir = new TH1D("recostarttimenodir","Start time, reco - truth, ignore bad direction",150,-50,50);
    else recostarttimenodir->Reset();
    
    if(!reco2Dangle) reco2Dangle = new TH1D("reco2Dangle","2D angle, reco - truth ",360,-180,180);
    else reco2Dangle->Reset();
    if(!reco2DangleGood) reco2DangleGood = new TH1D("reco2DangleGood","2D angle, reco - truth, direction right",360,-180,180);
    else reco2DangleGood->Reset();
    if(!recoDir) recoDir = new TH1D("recoDir","2D direction, reco - truth",7,-3,3);
    else recoDir->Reset();
    
    
    
    
//  ///////////////////////////////////////////////////////////////
//     // shower vs track parameters
//     
//     if(!hitDensity1d_sh) hitDensity1d_sh = new TH1D("hitDensity1d_sh","Hit Density 1D, Showers ",100,0,30);
//     else hitDensity1d_sh->Reset();
//     hitDensity1d_sh->SetLineColor(kRed);
//     if(!hitDensity2d_sh) hitDensity2d_sh = new TH1D("hitDensity2d_sh","Hit Density 2D, Showers",100,0,8);
//     else hitDensity2d_sh->Reset();
//     hitDensity2d_sh->SetLineColor(kRed);
//     if(!modifiedHitDens_sh) modifiedHitDens_sh = new TH1D("modifiedHitDens_sh","Modified Hit Density 1D, Showers ",100,0,25);
//     else modifiedHitDens_sh->Reset();
//      modifiedHitDens_sh->SetLineColor(kRed);
//     if(!multihitw_sh) multihitw_sh = new TH1D("multihitw_sh","MultiHitWire, Showers",120,0,120);
//     else multihitw_sh->Reset();
//      multihitw_sh->SetLineColor(kRed);
//     if(!eigenvalue_principal_sh) eigenvalue_principal_sh = new TH1D("eigenvalue_principal_sh","EigenValue Principal, Showers ",100,-15,0);
//     else eigenvalue_principal_sh->Reset();
//      eigenvalue_principal_sh->SetLineColor(kRed);
//     if(!length_sh) length_sh = new TH1D("length_sh","length, showers",100,0,100);
//     else length_sh->Reset();
//      length_sh->SetLineColor(kRed);
//     if(!width_sh) width_sh = new TH1D("width_sh","width, showers",20,0,40);
//     else width_sh->Reset();
//      width_sh->SetLineColor(kRed);
//     
//     if(!openAngle_sh) openAngle_sh = new TH1D("openAngle_sh","Opening Angle, Showers",180,0,1.1*TMath::Pi());
//     else openAngle_sh->Reset();
//      openAngle_sh->SetLineColor(kRed);
//     if(!openAngleChargeWgt_sh) openAngleChargeWgt_sh = new TH1D("openAngleChargeWgt_sh","Opening Angle, Charge Wieghted, Showers",180,0,1.1*TMath::Pi());
//     else openAngleChargeWgt_sh->Reset();
//      openAngleChargeWgt_sh->SetLineColor(kRed);
//     
//     if(!closeAngle_sh) closeAngle_sh = new TH1D("closeAngle_sh","Closing Angle, Showers",180,0,1.1*TMath::Pi());
//     else closeAngle_sh->Reset();
//      closeAngle_sh->SetLineColor(kRed);
//     if(!closeAngleChargeWgt_sh) closeAngleChargeWgt_sh = new TH1D("closeAngleChargeWgt_sh","Closing Angle, Charge Wieghted, Showers",180,0,1.1*TMath::Pi());
//     else closeAngleChargeWgt_sh->Reset();
//      closeAngleChargeWgt_sh->SetLineColor(kRed);
//     
//     ////////////////////////////////////////////////////////////////////////////////
//     if(!hitDensity1d_tr) hitDensity1d_tr = new TH1D("hitDensity1d_tr","Hit Density 1D, Tracks ",100,0,30);
//     else hitDensity1d_tr->Reset();
//     if(!hitDensity2d_tr) hitDensity2d_tr = new TH1D("hitDensity2d_tr","Hit Density 2D, Tracks",100,0,8);
//     else hitDensity2d_tr->Reset();
//     if(!modifiedHitDens_tr) modifiedHitDens_tr = new TH1D("modifiedHitDens_tr","Modified Hit Density 1D, Tracks ",100,0,25);
//     else modifiedHitDens_tr->Reset();
//     if(!multihitw_tr) multihitw_tr = new TH1D("multihitw_tr","MultiHitWire, Tracks",120,0,120);
//     else multihitw_tr->Reset();
//     
//     if(!eigenvalue_principal_tr) eigenvalue_principal_tr = new TH1D("eigenvalue_principal_tr","EigenValue Principal, Tracks ",100,-15,0);
//     else eigenvalue_principal_tr->Reset();
//     if(!length_tr) length_tr = new TH1D("length_tr","length, Tracks",100,0,100);
//     else length_tr->Reset();
//     if(!width_tr) width_tr = new TH1D("width_tr","width, Tracks",20,0,40);
//     else width_tr->Reset();
//     
//     if(!openAngle_tr) openAngle_tr = new TH1D("openAngle_tr","Opening Angle, Tracks",180,0,1.1*TMath::Pi());
//     else openAngle_tr->Reset();
//     if(!openAngleChargeWgt_tr) openAngleChargeWgt_tr = new TH1D("openAngleChargeWgt_tr","Opening Angle, Charge Wieghted, Tracks",180,0,1.1*TMath::Pi());
//     else openAngleChargeWgt_tr->Reset();
//     
//     if(!closeAngle_tr) closeAngle_tr = new TH1D("closeAngle_tr","Closing Angle, Tracks",180,0,1.1*TMath::Pi());
//     else closeAngle_tr->Reset();
//     if(!closeAngleChargeWgt_tr) closeAngleChargeWgt_tr = new TH1D("closeAngleChargeWgt_tr","Closing Angle, Charge Wieghted, Tracks",180,0,1.1*TMath::Pi());
//     else closeAngleChargeWgt_tr->Reset();
//     
//     /////////////////////////////////////
//     ///muons vs protons vs electrons testing of max hit charge
//     
//     // total charge distribution
//     // maxcharge, max3charge to TTree
//     // charge histograms for each plane here.
//     
//     if(!Charge_e[0]) Charge_e[0] = new TH1D("Charge_e[0]","Charge_e[0]",200,-10,10000);
//     else Charge_e[0]->Reset();
//     if(!Charge_p[0]) Charge_p[0] = new TH1D("Charge_p[0]","Charge_p[0]",200,-10,10000);
//     else Charge_p[0]->Reset();
//     if(!Charge_mu[0]) Charge_mu[0] = new TH1D("Charge_mu[0]","Charge_mu[0]",200,-10,10000);
//     else Charge_mu[0]->Reset();
//     
//     if(!Charge_e[1]) Charge_e[1] = new TH1D("Charge_e[1]","Charge_e[1]",200,-10,20000);
//     else Charge_e[1]->Reset();
//     if(!Charge_p[1]) Charge_p[1] = new TH1D("Charge_p[1]","Charge_p[1]",200,-10,20000);
//     else Charge_p[1]->Reset();
//     if(!Charge_mu[1]) Charge_mu[1] = new TH1D("Charge_mu[1]","Charge_mu[1]",200,-10,20000);
//     else Charge_mu[1]->Reset();
    

    return true;
  }
  
  bool CutEff::analyze(storage_manager* storage) {
  
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
    // event_fifo* my_pmtfifo_v = (event_fifo*)(storage->get_data(DATA::PMFIFO));
    //


    // if( event_fifo )
    //
    //   std::cout << "Event ID: " << my_pmtfifo_v->event_id() << std::endl;
    //


    bool is_mc_shower=false;
    bool is_mc_track=false;
    bool is_electron=false;
    bool is_gamma=false;    

    /// /////////////////////////getting the MCTruth info out, if exists
    
    event_mctruth  *mctruth_v = (event_mctruth *)(storage->get_data(DATA::MCTruth)); 
    
    event_cluster * my_cluster_v = (event_cluster *)(storage->get_data(DATA::DBCluster));

  
   // event_hit * my_hit_v = (event_hit*)(storage->get_data(my_cluster_v->get_hit_type()));
   // event_hit * my_hit_v = (event_hit*)(storage->get_data(DATA::FFTHit));
    event_hit * my_hit_v = (event_hit*)(storage->get_data(DATA::FFTHit));
    std::cout << " full hitlist size: " << my_hit_v->size() << std::endl;
    std::cout << " number of clusters: " << my_cluster_v->size() << std::endl;	
    std::cout << "++++  +++++ cut eff! +++++ ++++" << std::endl;
    if(mctruth_v!=NULL && mctruth_v->size() )
      fNParticles= mctruth_v->at(0).GetParticles().size(); 
    else 
      fNParticles = 0;
    
    fNPlanes=larutil::Geometry::GetME()->Nplanes();
   // fNClusters=fNPlanes;    // then should be" my_cluster_v->size() or fNPlanes
    fNClusters=my_cluster_v->size();    // using clusters
    init_tree_vectors();
    
    std::cout << "after init vectors" << std::endl;
    std::cout << " fNParticles::  " << fNParticles << std::endl;
    
    TLorentzVector mct_vtx;
     TLorentzVector mct_vtx_sh;
	  /* 	else{
  	    	    if ( mctruth_v->at(NPart).GetParticles().at(NPart).PdgCode() == 22 )   { 
       	        	trajsize= mctruth_v->at(NPart).GetParticles().at(NPart).Trajectory().size();
       	        	std::cout <<  "\n gamma " << std::endl;
		      }
       	        std::cout <<  "\n pi-0 " << std::endl;
                    }*/

    if (mctruth_v!=NULL && mctruth_v->size()) 
      {
       	if ( mctruth_v->size() > 1)  // this needs to be changed
            std::cout <<  "Found more than 1 MCTruth. Only use the 1st one... \n " << std::endl;
	//int NPart=0;
        for(int NPart=0;NPart<mctruth_v->at(0).GetParticles().size();NPart++){
            if (mctruth_v->at(0).GetParticles().at(NPart).PdgCode() == 11)  {    // electron    
		//if(NPart==0)
		{ mct_vtx = mctruth_v->at(0).GetParticles().at(NPart).Trajectory().at(0).Position();
		  mct_vtx_sh = mctruth_v->at(0).GetParticles().at(NPart).Trajectory().at(0).Position();
		}

           	 is_mc_shower=true;
           	 std::cout << "\n electron " << std::endl;
	    	 is_electron=true; 
	     }
            else if ( mctruth_v->at(0).GetParticles().at(NPart).PdgCode() == 22 )   { 
                int trajsize= mctruth_v->at(0).GetParticles().at(NPart).Trajectory().size();
               // if(NPart==0)
		{ mct_vtx = mctruth_v->at(0).GetParticles().at(NPart).Trajectory().at(trajsize-1).Position();
		  mct_vtx_sh = mctruth_v->at(0).GetParticles().at(NPart).Trajectory().at(0).Position();
		}

                std::cout <<  "\n gamma " << std::endl;
	        is_mc_shower=true;
		is_gamma=true;
	     }
            else if ( mctruth_v->at(0).GetParticles().at(NPart).PdgCode() == 13 )  //      ## muon    
	     {
	        if(NPart==0)
		  mct_vtx = mctruth_v->at(0).GetParticles().at(NPart).Trajectory().at(0).Position();
                std::cout <<  "\n muon " << std::endl;
	        is_mc_track=true;
	     }
   	    else{
	        is_mc_track=true;
		 if(NPart==0)
		  mct_vtx = mctruth_v->at(0).GetParticles().at(NPart).Trajectory().at(0).Position();  
	        std::cout <<  "\n proton ? " << std::endl;
	     }
	std::cout << " Truth vertex (" << mct_vtx[0] << "," << mct_vtx[1] << "," << mct_vtx[2] << "," << std::endl;
	fMCPDGstart[NPart]=mctruth_v->at(0).GetParticles().at(NPart).PdgCode();
	fMCenergystart[NPart]=mctruth_v->at(0).GetParticles().at(NPart).Trajectory().at(0).E();
	if(mctruth_v->at(0).GetParticles().at(NPart).Trajectory().at(0).E()){
 	      double lep_dcosx_truth = mctruth_v->at(0).GetParticles().at(NPart).Trajectory().at(0).Px()/mctruth_v->at(0).GetParticles().at(NPart).Trajectory().at(0).E();
 	      double lep_dcosy_truth = mctruth_v->at(0).GetParticles().at(NPart).Trajectory().at(0).Py()/mctruth_v->at(0).GetParticles().at(NPart).Trajectory().at(0).E();
 	      double lep_dcosz_truth = mctruth_v->at(0).GetParticles().at(NPart).Trajectory().at(0).Pz()/mctruth_v->at(0).GetParticles().at(NPart).Trajectory().at(0).E();
//     
	      std::cout << "-----  cx,cy,cz " << lep_dcosx_truth << " " << lep_dcosy_truth << " " << lep_dcosz_truth << std::endl;
	      double Px= mctruth_v->at(0).GetParticles().at(NPart).Trajectory().at(0).Px();
	      double Py= mctruth_v->at(0).GetParticles().at(NPart).Trajectory().at(0).Py();
	      double Pz= mctruth_v->at(0).GetParticles().at(NPart).Trajectory().at(0).Pz();
	      std::cout << " Px,Py,Pz, P2 " << Px << " "<< Py << " " <<Pz << " ; " << TMath::Sqrt(Px*Px+Py*Py+Pz*Pz)  << " P, as is: "<< mctruth_v->at(0).GetParticles().at(NPart).Trajectory().at(0).E()  << std::endl;
//            
//     
 	      fMCPhistart[NPart]=( (lep_dcosx_truth == 0.0 && lep_dcosz_truth == 0.0) ? 0.0 : TMath::ATan2(lep_dcosx_truth,lep_dcosz_truth));
 	      fMCThetastart[NPart]=( (lep_dcosx_truth == 0.0 && lep_dcosy_truth == 0.0 && lep_dcosz_truth == 0.0) ? 0.0 : TMath::Pi()*0.5-TMath::ATan2(std::sqrt(lep_dcosx_truth*lep_dcosx_truth + lep_dcosz_truth*lep_dcosz_truth),lep_dcosy_truth) );
//     
//    
	      fMCPhistart[NPart]=(180*fMCPhistart[NPart]/TMath::Pi());
	      fMCThetastart[NPart]=( 180*fMCThetastart[NPart]/TMath::Pi());
	      
	      std::cout << " phi, theta: " << fMCPhistart[NPart] << " " <<  fMCThetastart[NPart] << std::endl;
	      
	}
	fMCPDGisPrimary[NPart]=(mctruth_v->at(0).GetParticles().at(NPart).Process()=="primary");
	
	fMCXOrig[NPart]=mct_vtx[0];
	fMCYOrig[NPart]=mct_vtx[1];
	fMCZOrig[NPart]=mct_vtx[2];
      } 
//       if(fMCPDGstart[0]==11)
//          if(fMCPDGstart[0]==2212 || fMCPDGstart[0]==13 )
         allEvts->Fill(fMCenergystart[0]);  // need to correct for full events.
       std::cout << " is shower: " << is_mc_shower << " is track: " << is_mc_track << std::endl;
    /// /////////////////////////////////////////////// End Getting MC Truth info out.	    
    }
    else
       allEvts->Fill(0);  // need to correct for full events.
  
    
 
 
 
 
   //cluster::ClusterParamsAlgNew  fCPAlg; // = new cluster::ClusterParamsAlgNew();
///  //////////////// start looping on clusters or planes to get all hits.
 
     fRun=my_hit_v->run();
     fSubRun=my_hit_v->subrun();
     fEvent=my_hit_v->event_id();
    
//       ///using all hits:	 // comment in or out as needed
//      for (int ipl=0;ipl<fNPlanes;ipl++) {
//     
//         std::vector<const larlight::hit *> hit_vector;
// 	
//         hit_vector.clear();
//         hit_vector.reserve(my_hit_v->size());
// 
// 	 std::cout << " plane: " << ipl << " Run: " << my_hit_v->run() << " SubRunID " << my_hit_v->subrun() << " EventID " <<  my_hit_v->event_id() << " " << my_hit_v << std::endl;   
// 	 
// 	
// 	 
// 	int iplane = ipl;
// 	
// 	//double locmax=0;
// 	std::vector< double > locmaxvec;
// 	locmaxvec.resize(3);
// 	locmaxvec[0]=0;
// 	locmaxvec[1]=1;
// 	locmaxvec[2]=2;
//         for(auto &h : *my_hit_v) {
//     /*      if( larutil::Geometry::GetME()->ChannelToPlane(h.Channel()) == ipl )
//  	  hit_vector.push_back((const larlight::hit*)(&h));
// 	}*/ 
// 	  
//         if( larutil::Geometry::GetME()->ChannelToPlane(h.Channel()) == ipl )
// 	  {hit_vector.push_back((const larlight::hit*)(&h));
// 	   if(h.Charge()>locmaxvec[0])  
// 	      {int xx=0;
// 	       while(h.Charge()>locmaxvec[xx] && xx<locmaxvec.size()) //stop at xx element which is the highest.
// 		  xx++;
//                
// 	    //   std::cout << " found max at location: " << xx-1 << " "<< h.Charge()<< std::endl;
// 	       
// 	       
//                for(int ix=0;ix<xx-1;ix++)
//                 locmaxvec[ix]=locmaxvec[ix+1];		
//  
// 	       locmaxvec[xx-1]=h.Charge();
// 	       
// 	      }
// 	   // std::cout << " filling histograms for ipl = " << ipl << std::endl;  
// // 	    if(fMCPDGstart[0]==11)
// // 	      Charge_e[ipl]->Fill(h.Charge());
// // 	    if(fMCPDGstart[0]==13)
// // 	      Charge_mu[ipl]->Fill(h.Charge());
// // 	    if(fMCPDGstart[0]==2212)
// // 	      Charge_p[ipl]->Fill(h.Charge());
// 	  }
// 	}
// 	
// 	max_charge_3[ipl]=0;
// 	for(int ix=0;ix<locmaxvec.size();ix++)
// 	  {std::cout << "max hits " << locmaxvec[ix] << std::endl;
// 	  max_charge_3[ipl]+=locmaxvec[ix];
// 	  max_charge[ipl]=locmaxvec[ix];
// 	  }
// 	
// 	
//    
//  
// 	
// 	
//          std::cout << " +++ in CutEff " << hit_vector.size() << " at plane: " << ipl << std::endl; 
//       /// end using all hits  

     
     
    
   

     
    int nclustershowers=0;  // number of cluster like showers.  
    int nclusterFANNshowers=0;  // number of cluster like showers.   
    
    int nclustershowersperplane[2]={0,0};  // number of cluster like showers.  
    int nclusterFANNshowersperplane[2]={0,0};  // number of cluster like showers.   
    
    //new cuts
    const double modhitcut=1.05;
//     const double sumchargepercmindcut=1700.;
//     const double sumchargepercmindcut=3500.;
     const double sumchargepercmcut[2]={1700.,3500.};
//     const double modchargeindcut=1.3;  // TBD
//     const double modchargecolcut=2.5;
    const double modchargecut[2]={1.3,2.5};
    
    //const double chargeRMScutcoll=700.;
    //const double chargeRMScutind=300.;
    const double chargeRMScut[2]={300.,700.};
    const double multihitwirecut=1.;
    const double principalcompcut=-7.;
     
    const double showernesscut=0.9;
    //first pass:
    // modhitcut + chargeRMScutcol
    
    
	 /// using cluster only. // comment in or out as needed // need to set ipl as counter. and iplane as plane
     int ipl=-1; // just so that I can increment it at the start and not end of loop. ;-)
    for(auto const clustit : *my_cluster_v) {
      std::cout << "loop nr " << ipl+2 << std::endl;
      std::cout << " Clust ID " << clustit.ID() << " Run: " << my_cluster_v->run() << " SubRunID " << my_cluster_v->subrun() << " EventID " <<  my_cluster_v->event_id() << " " << my_cluster_v << std::endl;    
      //one entry per cluster
    
     //auto const hit_index_v = clustit.association(my_cluster_v->get_hit_type());
    
//         auto const hit_index_v = clustit.association(DATA::CrawlerHit);
        auto const hit_index_v = clustit.association(DATA::FFTHit);
        std::vector<const larlight::hit *> hit_vector;
	std::cout << " cluster hits: " << hit_index_v.size() << std::endl;
        hit_vector.clear();
	std::vector< double > locmaxvec;
	locmaxvec.resize(3);
	locmaxvec[0]=0;
	locmaxvec[1]=1;
	locmaxvec[2]=2;
	ipl++; // should be zero on first loop.
        int iplane = larutil::Geometry::GetME()->ChannelToPlane(my_hit_v->at(hit_index_v[0]).Channel()) ;
        std::cout << " ipl " << ipl << " iplane "<< iplane << std::endl;
	
	
        for(auto const hit_index : hit_index_v) {
            hit_vector.push_back( const_cast<const larlight::hit *>(&(my_hit_v->at(hit_index))) );
             auto h=my_hit_v->at(hit_index);
//        }
	 //  std::cout << " in hit index " << std::endl;  
	   if(h.Charge()>locmaxvec[0])  
	      {int xx=0;
	       while(h.Charge()>locmaxvec[xx] && xx<locmaxvec.size()) //stop at xx element which is the highest.
		  xx++;
               
	    //   std::cout << " found max at location: " << xx-1 << " "<< h.Charge()<< std::endl;
	       
	       
               for(int ix=0;ix<xx-1;ix++)
                locmaxvec[ix]=locmaxvec[ix+1];		
 
	       locmaxvec[xx-1]=h.Charge();
	       
	      }
          //  std::cout << " in hit index after locmacvec " << " ipl " << ipl << std::endl;  	      
  	   // std::cout << " filling histograms for ipl = " << ipl << std::endl;  
// 	    if(fMCPDGstart[0]==11)
// 	      Charge_e[iplane]->Fill(h.Charge());
// 	    if(fMCPDGstart[0]==13)
// 	      Charge_mu[iplane]->Fill(h.Charge());
// 	    if(fMCPDGstart[0]==2212)
// 	      Charge_p[iplane]->Fill(h.Charge());
	  }
	
	max_charge_3[ipl]=0;
	for(int ix=0;ix<locmaxvec.size();ix++)
	  {std::cout << "max hits " << locmaxvec[ix] << " " << ix << " ipl " << ipl << " " << max_charge_3.size() << std::endl;
	  max_charge_3.at(ipl)+=locmaxvec[ix];
	  max_charge.at(ipl)=locmaxvec[ix];
	  }
        
              
    
        /// end cluster comment out   
        //   if (ipl>fNPlanes-1)
        //  continue;
        //      std::cout << " +++ in CutEff " << hit_vector.size() << std::endl;  


        if(hit_vector.size() < 20)   // do not bother with too small hitlists
        {
            continue;
        }


        ::cluster::ClusterParamsAlgNew  fCPAlg;
        fCPAlg.setNeuralNetPath("../FANN/trained_nets/cascade_net.net");
        fCPAlg.Initialize();
        if(fCPAlg.SetHits(hit_vector) ==-1 )	
            continue;
        fCPAlg.GetAverages(true);
        fCPAlg.GetRoughAxis(true);
        fCPAlg.GetProfileInfo(true);
        fCPAlg.RefineStartPointAndDirection(true);
        //fCPAlg.RefineDirection(true);
        //fCPAlg.RefineStartPoints(true);
        //fCPAlg.FillPolygon()
        fCPAlg.GetFinalSlope(true);
        fCPAlg.TrackShowerSeparation(true);
        fCPAlg.Report();
	
	::cluster::cluster_params fResult=fCPAlg.GetParams();


	/// fill Reco Tree Info
	plane[ipl]=iplane;
	fWireVertex[ipl]=fResult.start_point.w;
	fTimeVertex[ipl]=fResult.start_point.t;
	
	fWireLast[ipl]=fResult.end_point.w;
	fTimeLast[ipl]=fResult.end_point.t;
	
	fDirection[ipl]=fResult.direction;
	
	//fOffAxis[ipl]=fResult.direction;
	//fOffAxisNorm[ipl]=fResult.direction;
	fNhits[ipl]=fResult.N_Hits;
	
	fHitDensity[ipl]=fResult.hit_density_1D;
	fLength[ipl]=fResult.length;
	fWidth[ipl]=fResult.width;  
	
	//  fOffAxisNormHD      fOffAxisNormHD[ipl]=;
	//  fOnAxisNormHD      fOnAxisNormHD[ipl]=;
	
	
	fPrincipal[ipl]=TMath::Log(1-fResult.eigenvalue_principal);
	slope2D[ipl]=fResult.angle_2d;
	fMultiHitWires[ipl]=fResult.multi_hit_wires;
	
	
	fsumCharge[ipl]=fResult.sum_charge;
	fmeanCharge[ipl]=fResult.mean_charge;
	frmsX[ipl]=fResult.rms_x;
	frmsY[ipl]=fResult.rms_y;
	fmeanX[ipl]=fResult.mean_x;
	fmeanY[ipl]=fResult.mean_y;
	fchargeWgtX[ipl]=fResult.charge_wgt_x;
	fchargeWgtY[ipl]=fResult.charge_wgt_y;
	fclusterAngle2d[ipl]=fResult.cluster_angle_2d;
	fangle2d[ipl]=fResult.angle_2d;
	fopenAngle[ipl]=fResult.opening_angle;
	fopenAngleChargeWgt[ipl]=fResult.opening_angle_charge_wgt;
	fcloseAngle[ipl]=fResult.closing_angle;
	fcloseAngleChargeWgt[ipl]=fResult.closing_angle_charge_wgt;
	fhitDensity1d[ipl]=fResult.hit_density_1D;
	fhitDensity2d[ipl]=fResult.hit_density_2D;
	fmodifiedHitDens[ipl]=fResult.modified_hit_density;
	
	fShowerness[ipl]=fResult.showerness;
        fTrackness[ipl]=fResult.trackness;
	
	
	 fModifiedMeanCharge[ipl]=fResult.modmeancharge;
         fChargeRMS[ipl]=fResult.RMS_charge;
	
	///////////////////////////////////////
	
	
	////////////// Fill MC Truth info
	
	larutil::PxPoint mcplanevtx;
	if (mctruth_v!=NULL && mctruth_v->size()) {
	  // Correct for new particles
	  std::cout << "mct vtx " << mct_vtx[0] << " " << mct_vtx[1] << " " <<mct_vtx[2] << std::endl;
	  std::cout << "mct vtx " << mct_vtx_sh[0] << " " << mct_vtx_sh[1] << " " <<mct_vtx_sh[2] << std::endl;
	  if(mct_vtx[2]<0.2) mct_vtx[2]=0.6;
	    if(mct_vtx[2]>90) mct_vtx[2]=90.;
	  if(mct_vtx[1]<-18) mct_vtx[1]=-18.;
	  if(mct_vtx[1]>18) mct_vtx[1]=18.;
	  if(mct_vtx[0]<0) mct_vtx[0]=0.5;
	  if(mct_vtx[0]>50) mct_vtx[0]=45.;
	   std::cout << "mct vtx " << mct_vtx[0] << " " << mct_vtx[1] << " " <<mct_vtx[2] << std::endl;
	  mcplanevtx=fGSer->Get2DPointProjectionCM(&mct_vtx,0);
	  std::cout << " wire vertex " << mcplanevtx.w << "," << mcplanevtx.t << std::endl;
	  recostartwire->Fill(fResult.start_point.w-mcplanevtx.w);
	  recostarttime->Fill(fResult.start_point.t-mcplanevtx.t);
	  
	  double mindiffwire= ( fabs(fResult.start_point.w-mcplanevtx.w) < fabs(fResult.end_point.w-mcplanevtx.w)  )  ? 
	  fResult.start_point.w-mcplanevtx.w : fResult.end_point.w-mcplanevtx.w;
	  
	  double mindifftime= ( fabs(fResult.start_point.t-mcplanevtx.t) < fabs(fResult.end_point.t-mcplanevtx.t)  )  ? 
	  fResult.start_point.t-mcplanevtx.t : fResult.end_point.t-mcplanevtx.t;						
	  recostartwirenodir->Fill(mindiffwire);
	  recostarttimenodir->Fill(mindifftime);
	  
	  TLorentzVector mct_mom = mctruth_v->at(0).GetParticles().at(0).Trajectory().at(0).Momentum();
	  TVector3 mct_momhelp=TVector3(mct_mom[0],mct_mom[1],mct_mom[2]);
	  double mct_angle2d=fGSer->Get2DangleFrom3D(plane[ipl],mct_momhelp.Unit());
	  int direction=(fabs(mct_angle2d) < TMath::Pi()/2)  ?  1 : -1;
	  
	  reco2Dangle->Fill(fResult.angle_2d-mct_angle2d*180/TMath::Pi());
	  if ( fabs(fResult.start_point.w-mcplanevtx.w) < fabs(fResult.end_point.w-mcplanevtx.w) )  // got the direction right
	      reco2DangleGood->Fill(fResult.angle_2d-mct_angle2d*180/TMath::Pi());
	  
	  recoDir->Fill(fResult.direction - direction);
	  
	  
	  /// these need to be modified, once we have the looping through clusters efficiency.
	  /// Currently using NPart = 0. Set this to the most appropriate later.
	  
	  int NPart=0;
	  
	  
	  mcpdg[ipl]=mctruth_v->at(0).GetParticles().at(NPart).PdgCode();  
	  //needs to be fixed
	  mcenergy[ipl]=mctruth_v->at(0).GetParticles().at(NPart).Trajectory().at(0).E();  
// 	  if(fMCPDGstart[0]==11)
// 	  if(fMCPDGstart[0]==2212 || fMCPDGstart[0]==13 )
	    allClusters->Fill(mcenergy[ipl]);
	  // mcphi[ipl]=;  
	  // mctheta[ipl]=;  
	  mcomega[ipl]=mct_angle2d*180/TMath::Pi();  
	  if(mcpdg[ipl]==22)
	  {
	    int trajsize= mctruth_v->at(0).GetParticles().at(NPart).Trajectory().size();
	    mcx[ipl]=mctruth_v->at(0).GetParticles().at(NPart).Trajectory().at(trajsize-1).Position()[0]; 
	    mcy[ipl]=mctruth_v->at(0).GetParticles().at(NPart).Trajectory().at(trajsize-1).Position()[1];  
	    mcz[ipl]=mctruth_v->at(0).GetParticles().at(NPart).Trajectory().at(trajsize-1).Position()[2];
	  }
	  else  
	  {
	    mcx[ipl]=mctruth_v->at(0).GetParticles().at(NPart).Trajectory().at(0).Position()[0];  
	    mcy[ipl]=mctruth_v->at(0).GetParticles().at(NPart).Trajectory().at(0).Position()[1];  
	    mcz[ipl]=mctruth_v->at(0).GetParticles().at(NPart).Trajectory().at(0).Position()[2];
	  }
	  
	  mcdirection[ipl]=direction;
	  //    mcenergyfraction[ipl]=;
	  mcwirevertex[ipl]=mcplanevtx.w;
	  mctimevertex[ipl]=mcplanevtx.t;
	  
	  //reco2Dangle reco2DangleGood;
	  //recoDir;
	}
	else
	   allClusters->Fill(1);
	////////////// End Fill MC Truth info
	/// /// ///  ///  cut testing
	
	
	  //any cluster that passes cuts
//     if(fMCPDGstart[0]==11)
//  if(fMCPDGstart[0]==2212 || fMCPDGstart[0]==13 )
    // {
      if(fmodifiedHitDens[ipl]>modhitcut || fChargeRMS[ipl]>chargeRMScut[iplane])	  
      {std::cout << " stand cuts good" << iplane << std::endl;
      anyShowerClusters->Fill(mcenergy[ipl]);
	nclustershowers++;
        nclustershowersperplane[iplane]++;
      }
      if(fShowerness[ipl] > showernesscut )
      {
	std::cout << " FANN cuts good" << iplane << std::endl;
	anyShowerClusters_FANN->Fill(mcenergy[ipl]);
	nclusterFANNshowers++;
	nclusterFANNshowersperplane[iplane]++;
      }
      
      std::cout << " nclustershowers: " << nclustershowers << " FANN clusters " << nclusterFANNshowers << std::endl;
      std::cout << " nclustershowers, per plane: " << nclustershowersperplane[0] << " " << nclustershowersperplane[1] << " FANN clusters " << nclusterFANNshowersperplane[0] <<" " << nclusterFANNshowersperplane[1]<< std::endl;
      
    // }  

       
       
       /// /// /// end cut testing
     } // end loop on clusters

    fMainTree->Fill();
  
//     if(fMCPDGstart[0]==11)
//     if(fMCPDGstart[0]==2212 || fMCPDGstart[0]==13 )
  //  {
    fMCenergystart.resize(1,1);
    // fill Event histograms
    if(nclustershowers>=2)
      anyShowerEvents->Fill(fMCenergystart[0]);
    
    if(nclusterFANNshowers>=2)
      anyShowerEvents_FANN->Fill(fMCenergystart[0]);  
    
    
    if(nclustershowersperplane[0]>0 && nclustershowersperplane[1]>0)
      bothplaneShowerEvents->Fill(fMCenergystart[0]);
    
    
    if(nclusterFANNshowersperplane[0]>0 && nclusterFANNshowersperplane[1]>0)
      bothplaneShowerEvents_FANN->Fill(fMCenergystart[0]);
    
    //any event that has two shower clusters one in eache plane that match/reconstruct inside fid vol.  
  //  TH1D* bothplaneShowerEventsmatched;
  //  } 
    
   

    //any event that has two shower clusters one in eache plane that match/reconstruct inside fid vol.  
   // TH1D* bothplaneShowerEventsmatched_FANN;
      
      
      
  
    return true;
  }

  bool CutEff::finalize() {

    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
    //
    // Say you made a histogram pointer meanX to store. You can do this:
    //

    

 
    // MCTruth plots:
    
    recostartwire->Draw();
    recostarttime->Draw();
    recostartwirenodir->Draw();
    recostarttimenodir->Draw();
    reco2Dangle->Draw();
    reco2DangleGood->Draw();
    recoDir->Draw();
    
    

    
    
    
//     // Efficiency plots
//      TCanvas * allevts_c= new TCanvas("All Events","All Events");
//      allEvts->DrawCopy();
//      
//      
//      TCanvas * allclusts_c= new TCanvas("All Clusters","All Clusters");
//      allClusters->DrawCopy();
     
     TCanvas * surviveclusts_c= new TCanvas("surviving Clusters","surviving Clusters");
     allClusters->DrawCopy();
     anyShowerClusters->SetLineColor(kRed);
     anyShowerClusters->DrawCopy("same");
     
     
     TCanvas * surviveevts_c= new TCanvas("Events with two showers","Events with two showers");
     allEvts->DrawCopy();
     anyShowerEvents->SetLineColor(kRed);
     anyShowerEvents->DrawCopy("same");
     
     TCanvas * surviveplaneevts_c= new TCanvas("Events with two showers in each plane","Events with two showers in each plane");
     allEvts->DrawCopy();
     bothplaneShowerEvents->SetLineColor(kRed);
     bothplaneShowerEvents->DrawCopy("same");
     
     
//      TCanvas * surviveplaneevtsmatch_c= new TCanvas("Events with two showers in each plane, matched","Events with two showers in each plane, matched");
//      allEvts->DrawCopy();
//      bothplaneShowerEventsmatched->SetLineColor(kRed);
//      bothplaneShowerEventsmatched->DrawCopy("same");
     
// 
     TCanvas * surviveclusts_cFANN= new TCanvas("surviving Clusters FANN","surviving Clusters FANN");
     allClusters->DrawCopy();
     anyShowerClusters_FANN->SetLineColor(kRed);
     anyShowerClusters_FANN->DrawCopy("same");
     
     
     TCanvas * surviveevts_c_FANN= new TCanvas("Events with two showers FANN","Events with two showers FANN");
     allEvts->DrawCopy();
     anyShowerEvents_FANN->SetLineColor(kRed);
     anyShowerEvents_FANN->DrawCopy("same");
     
     TCanvas * surviveplaneevts_c_FANN= new TCanvas("Events with two showers in each plane  FANN","Events with two showers in each plane  FANN");
     allEvts->DrawCopy();
     bothplaneShowerEvents_FANN->SetLineColor(kRed);
     bothplaneShowerEvents_FANN->DrawCopy("same");
     
     
//      TCanvas * surviveplaneevtsmatch_c_FANN= new TCanvas("Events with two showers in each plane, matched  FANN","Events with two showers in each plane, matched  FANN");
//      allEvts->DrawCopy();
//      bothplaneShowerEventsmatched_FANN->SetLineColor(kRed);
//      bothplaneShowerEventsmatched_FANN->DrawCopy("same");
     
     
    std::cout << "+++++All Clusters: " << allClusters->GetEntries() << std::endl;
   
    std::cout << " All Clusters passing cuts: " << anyShowerClusters->GetEntries() << std::endl;
     
    std::cout << " All Clusters passing FANN cuts: " << anyShowerClusters_FANN->GetEntries() << std::endl;
     
     std::cout << "++++ All Events: " << allEvts->GetEntries() << std::endl;
         // one entry per event

    
    std::cout << " All events with two showers cuts: " << anyShowerEvents->GetEntries() << std::endl;
    std::cout << " All events with two showers, two planes cuts: " << bothplaneShowerEvents->GetEntries() << std::endl; 
    
    std::cout << " All events with two FANN showers cuts: " << anyShowerEvents_FANN->GetEntries() << std::endl;
    std::cout << " All events with two FANN showers, two planes cuts: " << bothplaneShowerEvents_FANN->GetEntries() << std::endl;
    
    
//     //any cluster that passes cuts
//     TH1D* anyShowerClusters;
//     //any event that has two shower clusters (no 2 plane req) 
//     TH1D* anyShowerEvents;
//     //any event that has two shower clusters one in eache plane  
//     TH1D* bothplaneShowerEvents;
//     //any event that has two shower clusters one in eache plane that match/reconstruct inside fid vol.  
//     TH1D* bothplaneShowerEventsmatched;
//         
//     TH1D* anyShowerClusters_FANN;
//     //any event that has two shower clusters (no 2 plane req) 
//     TH1D* anyShowerEvents_FANN;
//     //any event that has two shower clusters one in eache plane  
//     TH1D* bothplaneShowerEvents_FANN;
     

     
     
     
    
     if(_fout) { 
	_fout->cd(); 
    // one entry per event
    allEvts->Write();
    //one entry per cluster
    allClusters->Write();
    //any cluster that passes cuts
    anyShowerClusters->Write();
    //any event that has two shower clusters (no 2 plane req) 
    anyShowerEvents->Write();
    //any event that has two shower clusters one in eache plane  
    bothplaneShowerEvents->Write();
    //any event that has two shower clusters one in eache plane that match/reconstruct inside fid vol.  
    bothplaneShowerEventsmatched->Write();
    
    
    //any cluster that passes cuts
    anyShowerClusters_FANN->Write();
    //any event that has two shower clusters (no 2 plane req) 
    anyShowerEvents_FANN->Write();
    //any event that has two shower clusters one in eache plane  
    bothplaneShowerEvents_FANN->Write();
    //any event that has two shower clusters one in eache plane that match/reconstruct inside fid vol.  
    bothplaneShowerEventsmatched_FANN->Write();
    
    

    
// 
// 	modifiedHitDens->Write(); 
	recostartwire->Write();
	recostarttime->Write();
	recostartwirenodir->Write();
	recostarttimenodir->Write();
	reco2Dangle->Write();
	reco2DangleGood->Write();
	recoDir->Write();
	
// 	openAngle_sh->Write();
//       openAngleChargeWgt_sh->Write();
//       closeAngle_sh->Write();
//       closeAngleChargeWgt_sh->Write();
//       hitDensity1d_sh->Write();
//       hitDensity2d_sh->Write();
//       modifiedHitDens_sh->Write();
//       multihitw_sh->Write();
//       eigenvalue_principal_sh ->Write();
//       length_sh->Write();
//       width_sh ->Write();
// 	  
// 	
//       openAngle_tr->Write();
//       openAngleChargeWgt_tr->Write();
//       closeAngle_tr->Write();
//       closeAngleChargeWgt_tr->Write();
//       hitDensity1d_tr->Write();
//       hitDensity2d_tr->Write();
//       modifiedHitDens_tr->Write();
//       multihitw_tr->Write();  
//       eigenvalue_principal_tr->Write();
//       length_tr->Write();
//       width_tr ->Write();
//      fMainTree->Write();
// 	
	
      }

     else 
       print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    

    return true;
  }
  
  
  void CutEff::init_tree_vectors(){

    if(!fNParticles && !fNClusters)
    {
     std::cout << " no particles and no clusters - nothing to here " << std::endl;
     return;
    }
    //These are defined per MCparticle  
       fMCPDGstart.clear();  
       fMCPDGisPrimary.clear();  
       fMCenergystart.clear();  
       fMCPhistart.clear();  
       fMCThetastart.clear();  
       fMCXOrig.clear();  
       fMCYOrig.clear();  
       fMCZOrig.clear();  
    
    
    //These are defined per cluster
       mcpdg.clear();  
       mcenergy.clear();  
       mcphi.clear();  
       mctheta.clear();  
       mcomega.clear();  
       mcx.clear();  
       mcy.clear();  
       mcz.clear();
    
       mcdirection.clear();
    //    mcenergyfraction.clear();
       mcwirevertex.clear();
       mctimevertex.clear();
    //    mcdistfromorigin.clear();
    
    
    ///////////// reconstructed quantities
       plane.clear();
       fWireVertex.clear();
       fTimeVertex.clear();
    
       fWireLast.clear();
       fTimeLast.clear();
    
       fDirection.clear();
    
       fOffAxis.clear();
       fOffAxisNorm.clear();
       fNhits.clear();
    
       fHitDensity.clear();
       fLength.clear();
       fWidth.clear();  
    
    //  fOffAxisNormHD      fOffAxisNormHD.clear();
    //  fOnAxisNormHD      fOnAxisNormHD.clear();
    
    
       fPrincipal.clear();
       slope2D.clear();
       fMultiHitWires.clear();
    
    
    
       fsumCharge.clear();
       fmeanCharge.clear();
       frmsX.clear();
       frmsY.clear();
       fmeanX.clear();
       fmeanY.clear();
       fchargeWgtX.clear();
       fchargeWgtY.clear();
       fclusterAngle2d.clear();
       fangle2d.clear();
       fopenAngle.clear();
       fopenAngleChargeWgt.clear();
       fcloseAngle.clear();
       fcloseAngleChargeWgt.clear();
       fhitDensity1d.clear();
       fhitDensity2d.clear();
       fmodifiedHitDens.clear();
       fShowerness.clear();
       fTrackness.clear();
       fModifiedMeanCharge.clear();;
       fChargeRMS.clear();;
    
    
    
    
    ////////////////////////// now resize.
    //These are defined per MCparticle  
       fMCPDGstart.resize(fNParticles);  
       fMCPDGisPrimary.resize(fNParticles);  
       fMCenergystart.resize(fNParticles);  
       fMCPhistart.resize(fNParticles);  
       fMCThetastart.resize(fNParticles);  
       fMCXOrig.resize(fNParticles);  
       fMCYOrig.resize(fNParticles);  
       fMCZOrig.resize(fNParticles);  
    
    
    //These are defined per cluster
       mcpdg.resize(fNClusters);  
       mcenergy.resize(fNClusters);  
       mcphi.resize(fNClusters);  
       mctheta.resize(fNClusters);  
       mcomega.resize(fNClusters);
       mcx.resize(fNClusters);  
       mcy.resize(fNClusters);  
       mcz.resize(fNClusters);
    
       mcdirection.resize(fNClusters);
    //    mcenergyfraction.resize(fNClusters);
       mcwirevertex.resize(fNClusters);
       mctimevertex.resize(fNClusters);
    //    mcdistfromorigin.resize(fNClusters);
    
    
    ///////////// reconstructed quantities
       plane.resize(fNClusters);
       fWireVertex.resize(fNClusters);
       fTimeVertex.resize(fNClusters);
    
       fWireLast.resize(fNClusters);
       fTimeLast.resize(fNClusters);
    
       fDirection.resize(fNClusters);
    
       fOffAxis.resize(fNClusters);
       fOffAxisNorm.resize(fNClusters);
       fNhits.resize(fNClusters);
    
       fHitDensity.resize(fNClusters);
       fLength.resize(fNClusters);
       fWidth.resize(fNClusters);  
    
    //  fOffAxisNormHD      fOffAxisNormHD.resize(fNClusters);
    //  fOnAxisNormHD      fOnAxisNormHD.resize(fNClusters);
    
    
       fPrincipal.resize(fNClusters);
       slope2D.resize(fNClusters);
       fMultiHitWires.resize(fNClusters);
    
    
    
       fsumCharge.resize(fNClusters);
       fmeanCharge.resize(fNClusters);
       frmsX.resize(fNClusters);
       frmsY.resize(fNClusters);
       fmeanX.resize(fNClusters);
       fmeanY.resize(fNClusters);
       fchargeWgtX.resize(fNClusters);
       fchargeWgtY.resize(fNClusters);
       fclusterAngle2d.resize(fNClusters);
       fangle2d.resize(fNClusters);
       fopenAngle.resize(fNClusters);
       fopenAngleChargeWgt.resize(fNClusters);
       fcloseAngle.resize(fNClusters);
       fcloseAngleChargeWgt.resize(fNClusters);
       fhitDensity1d.resize(fNClusters);
       fhitDensity2d.resize(fNClusters);
       fmodifiedHitDens.resize(fNClusters);
     
       fShowerness.resize(fNClusters);
       fTrackness.resize(fNClusters);
    
       
       fModifiedMeanCharge.resize(fNClusters);;
       fChargeRMS.resize(fNClusters);;
       
      max_charge_3.clear();
	  max_charge.clear();
      max_charge_3.resize(fNClusters);
	  max_charge.resize(fNClusters);
    
    }
  
}
#endif
