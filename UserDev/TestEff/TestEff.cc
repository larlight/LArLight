#ifndef TESTEFF_CC
#define TESTEFF_CC

#include "TestEff.hh"
// #include "../ClusterRecoUtil/ClusterParamsAlgNew.hh"
#include "ClusterParams.hh"
#include "LArUtilManager.hh"

namespace larlight {

  bool TestEff::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D"). 
    //   
//    TH1D *hist1 = new TH1D("hist1","title",Nbins,lowerBin,upperBin) ;	

    if(!fGSer) fGSer = (larutil::GeometryUtilities*)(larutil::GeometryUtilities::GetME());


    if(!sumCharge) sumCharge = new TH1D("sumCharge","Sum charge",10,-3000,100000);
    else sumCharge->Reset();
    if(!meanCharge) meanCharge = new TH1D("meanCharge","Mean charge",100,-500,3500);
    else meanCharge->Reset();
    
    if(!meanX) meanX = new TH1D("meanX","Mean in x",120,0,110);
    else meanX->Reset();
    if(!meanY) meanY = new TH1D("meanY","Mean in y",100,0,60);
    else meanY->Reset();
    
    if(!rmsX) rmsX = new TH1D("rmsX","RMS in x",120,0,50);
    else rmsX->Reset();
    if(!rmsY) rmsY = new TH1D("rmsY","RMS in y",100,0,15);
    else rmsY->Reset();
		
    if(!chargeWgtX) chargeWgtX = new TH1D("chargeWgtX","Charge weighted x",120,10,100);
    else chargeWgtX->Reset();
    if(!chargeWgtY) chargeWgtY = new TH1D("chargeWgtY","Charge weigthed y",100,0,60);
    else chargeWgtY->Reset();
    
    if(!clusterAngle2d) clusterAngle2d = new TH1D("clusterAngle2d","Cluster angle 2d",120,0,100);
    else clusterAngle2d->Reset();
    if(!angle2d) angle2d = new TH1D("angle2d","Angle 2d",150,-200,200);
    else angle2d->Reset();
    
    if(!openAngle) openAngle = new TH1D("openAngle","Opening angle",100,0,2.25);
    else openAngle->Reset();
    if(!openAngleChargeWgt) openAngleChargeWgt = new TH1D("openAngleChargeWgt","Opening angle, charge weighted",100,0,2.0);
    else openAngleChargeWgt->Reset();
    
    if(!closeAngleChargeWgt) closeAngleChargeWgt = new TH1D("closeAngleChargeWgt","Closing angle, charge weighted",120,0,2.25);
    else closeAngleChargeWgt->Reset();
    if(!closeAngle) closeAngle = new TH1D("closeAngle","Close angle",100,0,2.25);
    else closeAngle->Reset();

    if(!hitDensity1d) hitDensity1d = new TH1D("hitDensity1d","Hit density 1d",120,0,10);
    else hitDensity1d->Reset();
    if(!hitDensity2d) hitDensity2d = new TH1D("hitDensity2d","Hit density 2d",100,0,60);
    else hitDensity2d->Reset();
    if(!modifiedHitDens) modifiedHitDens = new TH1D("modifiedHitDens","Modified Hit Density",100,0,20);
    else modifiedHitDens->Reset();

//////////////////////////////////////////////////////////////
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
    
 ///////////////////////////////////////////////////////////////
    // shower vs track parameters
    
    if(!hitDensity1d_sh) hitDensity1d_sh = new TH1D("hitDensity1d_sh","Hit Density 1D, Showers ",100,0,30);
    else hitDensity1d_sh->Reset();
    hitDensity1d_sh->SetLineColor(kRed);
    if(!hitDensity2d_sh) hitDensity2d_sh = new TH1D("hitDensity2d_sh","Hit Density 2D, Showers",100,0,8);
    else hitDensity2d_sh->Reset();
    hitDensity2d_sh->SetLineColor(kRed);
    if(!modifiedHitDens_sh) modifiedHitDens_sh = new TH1D("modifiedHitDens_sh","Modified Hit Density 1D, Showers ",100,0,60);
    else modifiedHitDens_sh->Reset();
     modifiedHitDens_sh->SetLineColor(kRed);
    if(!multihitw_sh) multihitw_sh = new TH1D("multihitw_sh","MultiHitWire, Showers",120,0,120);
    else multihitw_sh->Reset();
     multihitw_sh->SetLineColor(kRed);
    if(!eigenvalue_principal_sh) eigenvalue_principal_sh = new TH1D("eigenvalue_principal_sh","EigenValue Principal, Showers ",100,-15,0);
    else eigenvalue_principal_sh->Reset();
     eigenvalue_principal_sh->SetLineColor(kRed);
    if(!length_sh) length_sh = new TH1D("length_sh","length, showers",100,0,100);
    else length_sh->Reset();
     length_sh->SetLineColor(kRed);
    if(!width_sh) width_sh = new TH1D("width_sh","width, showers",20,0,40);
    else width_sh->Reset();
     width_sh->SetLineColor(kRed);
    
    if(!openAngle_sh) openAngle_sh = new TH1D("openAngle_sh","Opening Angle, Showers",180,0,1.1*TMath::Pi());
    else openAngle_sh->Reset();
     openAngle_sh->SetLineColor(kRed);
    if(!openAngleChargeWgt_sh) openAngleChargeWgt_sh = new TH1D("openAngleChargeWgt_sh","Opening Angle, Charge Wieghted, Showers",180,0,1.1*TMath::Pi());
    else openAngleChargeWgt_sh->Reset();
     openAngleChargeWgt_sh->SetLineColor(kRed);
    
    if(!closeAngle_sh) closeAngle_sh = new TH1D("closeAngle_sh","Closing Angle, Showers",180,0,1.1*TMath::Pi());
    else closeAngle_sh->Reset();
     closeAngle_sh->SetLineColor(kRed);
    if(!closeAngleChargeWgt_sh) closeAngleChargeWgt_sh = new TH1D("closeAngleChargeWgt_sh","Closing Angle, Charge Wieghted, Showers",180,0,1.1*TMath::Pi());
    else closeAngleChargeWgt_sh->Reset();
     closeAngleChargeWgt_sh->SetLineColor(kRed);
    
    ////////////////////////////////////////////////////////////////////////////////
    if(!hitDensity1d_tr) hitDensity1d_tr = new TH1D("hitDensity1d_tr","Hit Density 1D, Tracks ",100,0,30);
    else hitDensity1d_tr->Reset();
    if(!hitDensity2d_tr) hitDensity2d_tr = new TH1D("hitDensity2d_tr","Hit Density 2D, Tracks",100,0,8);
    else hitDensity2d_tr->Reset();
    if(!modifiedHitDens_tr) modifiedHitDens_tr = new TH1D("modifiedHitDens_tr","Modified Hit Density 1D, Tracks ",100,0,60);
    else modifiedHitDens_tr->Reset();
    if(!multihitw_tr) multihitw_tr = new TH1D("multihitw_tr","MultiHitWire, Tracks",120,0,120);
    else multihitw_tr->Reset();
    
    if(!eigenvalue_principal_tr) eigenvalue_principal_tr = new TH1D("eigenvalue_principal_tr","EigenValue Principal, Tracks ",100,-15,0);
    else eigenvalue_principal_tr->Reset();
    if(!length_tr) length_tr = new TH1D("length_tr","length, Tracks",100,0,100);
    else length_tr->Reset();
    if(!width_tr) width_tr = new TH1D("width_tr","width, Tracks",20,0,40);
    else width_tr->Reset();
    
    if(!openAngle_tr) openAngle_tr = new TH1D("openAngle_tr","Opening Angle, Tracks",180,0,1.1*TMath::Pi());
    else openAngle_tr->Reset();
    if(!openAngleChargeWgt_tr) openAngleChargeWgt_tr = new TH1D("openAngleChargeWgt_tr","Opening Angle, Charge Wieghted, Tracks",180,0,1.1*TMath::Pi());
    else openAngleChargeWgt_tr->Reset();
    
    if(!closeAngle_tr) closeAngle_tr = new TH1D("closeAngle_tr","Closing Angle, Tracks",180,0,1.1*TMath::Pi());
    else closeAngle_tr->Reset();
    if(!closeAngleChargeWgt_tr) closeAngleChargeWgt_tr = new TH1D("closeAngleChargeWgt_tr","Closing Angle, Charge Wieghted, Tracks",180,0,1.1*TMath::Pi());
    else closeAngleChargeWgt_tr->Reset();
    
    
    larutil::LArUtilManager::Reconfigure(larlight::GEO::kArgoNeuT);
    
    return true;
  }
  
  bool TestEff::analyze(storage_manager* storage) {
  
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
    
    /// /////////////////////////getting the MCTruth info out, if exists
    
    event_mctruth  *mctruth_v = (event_mctruth *)(storage->get_data(DATA::MCTruth)); 
    
    
    
    TLorentzVector mct_vtx;
    if (mctruth_v!=NULL && mctruth_v->size()) 
      {
        if ( mctruth_v->size()>1 )  // this needs to be changed
            std::cout <<  "Found more than 1 MCTruth. Only use the 1st one... \n " << std::endl;
        if (mctruth_v->at(0).GetParticles().at(0).PdgCode() == 11)  {    // electron    
            mct_vtx = mctruth_v->at(0).GetParticles().at(0).Trajectory().at(0).Position();
            std::cout << "\n electron " << std::endl; 
	    is_mc_shower=true;
	    }
        else if ( mctruth_v->at(0).GetParticles().at(0).PdgCode() == 22 )   { 
            int trajsize= mctruth_v->at(0).GetParticles().at(0).Trajectory().size();
            mct_vtx = mctruth_v->at(0).GetParticles().at(0).Trajectory().at(trajsize-1).Position();
            std::cout <<  "\n gamma " << std::endl;
	    is_mc_shower=true;
	    }
        else if ( mctruth_v->at(0).GetParticles().at(0).PdgCode() == 13 )  //      ## muon    
	    {
            mct_vtx = mctruth_v->at(0).GetParticles().at(0).Trajectory().at(0).Position();
            std::cout <<  "\n muon " << std::endl;
	    is_mc_track=true;
	    }
	else{
	   is_mc_track=true;
	    mct_vtx = mctruth_v->at(0).GetParticles().at(0).Trajectory().at(0).Position();  
	    std::cout <<  "\n proton ? " << std::endl;
	   }
	std::cout << " Truth vertex (" << mct_vtx[0] << "," << mct_vtx[1] << "," << mct_vtx[2] << "," << std::endl;
      } 
    
    std::cout << " is shower: " << is_mc_shower << " is track: " << is_mc_track << std::endl;
    /// /////////////////////////////////////////////// End Getting MC Truth info out.	    
    
    event_cluster * my_cluster_v = (event_cluster *)(storage->get_data(DATA::FuzzyCluster));



  
   // event_hit * my_hit_v = (event_hit*)(storage->get_data(my_cluster_v->get_hit_type()));
     event_hit * my_hit_v = (event_hit*)(storage->get_data(DATA::FFTHit));
    std::cout << " ful hitlist size: " << my_hit_v->size() << std::endl;
	
   //cluster::ClusterParamsAlgNew  fCPAlg; // = new cluster::ClusterParamsAlgNew();
///  //////////////// start looping on clusters or planes to get all hits.
 
      ///using all hits:	 // comment in or out as needed
     for (int ipl=0;ipl<larutil::Geometry::GetME()->Nplanes();ipl++) {
    
        std::vector<const larlight::hit *> hit_vector;
	//         hit_vector.clear();
	
        hit_vector.clear();
        hit_vector.reserve(my_hit_v->size());

	 std::cout << " plane: " << ipl << " Run: " << my_hit_v->run() << " SubRunID " << my_hit_v->subrun() << " EventID " <<  my_hit_v->event_id() << " " << my_hit_v << std::endl;    
	
        for(auto &h : *my_hit_v) {

        if( larutil::Geometry::GetME()->ChannelToPlane(h.Channel()) == ipl )
	    hit_vector.push_back((const larlight::hit*)(&h));

	}
	
         std::cout << " +++ in TestEff " << hit_vector.size() << " at plane: " << ipl << std::endl; 
      /// end using all hits  

	 /// using cluster only. // comment in or out as needed
//     for(auto const clustit : *my_cluster_v) {
//       std::cout << " Clust ID " << clustit.ID() << " Run: " << my_cluster_v->run() << " SubRunID " << my_cluster_v->subrun() << " EventID " <<  my_cluster_v->event_id() << " " << my_cluster_v << std::endl;    
// 
//      //auto const hit_index_v = clustit.association(my_cluster_v->get_hit_type());
//     
//         auto const hit_index_v = clustit.association(DATA::FFTHit);
//         std::vector<const larlight::hit *> hit_vector;
//         hit_vector.clear();
//         
//         for(auto const hit_index : hit_index_v) {
//             hit_vector.push_back( const_cast<const larlight::hit *>(&(my_hit_v->at(hit_index))) );
//             //my_hit_v->at(hit_index);
//         }
        
       /// end cluster comment out   
        
        std::cout << " +++ in TestEff " << hit_vector.size() << std::endl;  
	
   
	    

	if(hit_vector.size() < 20)   // do not bother with too small hitlists
	    {
	     continue;
	    }
	    
	    
	    
	    
        ::cluster::ClusterParamsAlgNew  fCPAlg;
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
        fCPAlg.Report();
	
	::cluster::cluster_params fResult=fCPAlg.GetParams();

	sumCharge->Fill(fResult.sum_charge) ;
	meanCharge->Fill(fResult.mean_charge); 
	rmsX->Fill(fResult.rms_x);
	rmsY->Fill(fResult.rms_y);
	meanX->Fill(fResult.mean_x);
	meanY->Fill(fResult.mean_y);
	chargeWgtX->Fill(fResult.charge_wgt_x);
	chargeWgtY->Fill(fResult.charge_wgt_y);
	clusterAngle2d->Fill(fResult.cluster_angle_2d);
	angle2d->Fill(fResult.angle_2d);
	openAngle->Fill(fResult.opening_angle);
	openAngleChargeWgt->Fill(fResult.opening_angle_charge_wgt);
	closeAngle->Fill(fResult.closing_angle);
	closeAngleChargeWgt->Fill(fResult.closing_angle_charge_wgt);
	hitDensity1d->Fill(fResult.hit_density_1D) ;
	hitDensity2d->Fill(fResult.hit_density_2D) ;
	modifiedHitDens->Fill(fResult.modified_hit_density) ;
	
	if(is_mc_shower)
	{
	  openAngle_sh->Fill(fResult.opening_angle);
	  openAngleChargeWgt_sh->Fill(fResult.opening_angle_charge_wgt);
	  closeAngle_sh->Fill(fResult.closing_angle);
	  closeAngleChargeWgt_sh->Fill(fResult.closing_angle_charge_wgt);
	  hitDensity1d_sh->Fill(fResult.hit_density_1D) ;
	  hitDensity2d_sh->Fill(fResult.hit_density_2D) ;
	  modifiedHitDens_sh->Fill(fResult.modified_hit_density) ;
	  multihitw_sh->Fill(fResult.multi_hit_wires) ;  
	  eigenvalue_principal_sh ->Fill(TMath::Log(1-fResult.eigenvalue_principal)) ;
	  length_sh->Fill(fResult.length) ;
	  width_sh ->Fill(fResult.width) ;
	  
	}
	else if(is_mc_track)
	{
	  openAngle_tr->Fill(fResult.opening_angle);
	  openAngleChargeWgt_tr->Fill(fResult.opening_angle_charge_wgt);
	  closeAngle_tr->Fill(fResult.closing_angle);
	  closeAngleChargeWgt_tr->Fill(fResult.closing_angle_charge_wgt);
	  hitDensity1d_tr->Fill(fResult.hit_density_1D) ;
	  hitDensity2d_tr->Fill(fResult.hit_density_2D) ;
	  modifiedHitDens_tr->Fill(fResult.modified_hit_density) ;
	  multihitw_tr->Fill(fResult.multi_hit_wires) ;  
	  eigenvalue_principal_tr ->Fill(TMath::Log(1-fResult.eigenvalue_principal)) ;
	  length_tr->Fill(fResult.length) ;
	  width_tr ->Fill(fResult.width) ;
	  
	}

   	larutil::PxPoint mcplanevtx;
	if (mctruth_v!=NULL && mctruth_v->size()) {
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
	    TVector3 mct_momhelp = TVector3(mct_mom[0],mct_mom[1],mct_mom[2] );
	    double mct_angle2d=fGSer->Get2DangleFrom3D(ipl,mct_momhelp);
	    int direction=(fabs(mct_angle2d) < TMath::Pi()/2)  ?  1 : -1;
	    
	    reco2Dangle->Fill(fResult.angle_2d-mct_angle2d*180/TMath::Pi());
	    if ( fabs(fResult.start_point.w-mcplanevtx.w) < fabs(fResult.end_point.w-mcplanevtx.w) )  // got the direction right
	      reco2DangleGood->Fill(fResult.angle_2d-mct_angle2d*180/TMath::Pi());
	    
	    recoDir->Fill(fResult.direction - direction);
	    //reco2Dangle reco2DangleGood;
	    //recoDir;
	    
	  
	}
	
	

     } // end loop on clusters


  
  
    return true;
  }

  bool TestEff::finalize() {

    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
    //
    // Say you made a histogram pointer meanX to store. You can do this:
    //

    
    sumCharge->Draw();
    meanCharge->Draw();
    rmsX->Draw();
    rmsY->Draw();
    meanX->Draw();
    meanY->Draw();
    chargeWgtX->Draw();
    chargeWgtY->Draw();
    clusterAngle2d->Draw();
    angle2d->Draw();
    openAngle->Draw();	
//	openAngleChargeWgt->SetLineColor(3);
    openAngleChargeWgt->Draw() ; //"P same");
    closeAngle->Draw();
//	closeAngleChargeWgt->SetLineColor(3);
    closeAngleChargeWgt->Draw() ; //"P same");
    hitDensity1d->Draw();
    hitDensity2d->Draw();
    modifiedHitDens->Draw();
    
    // MCTruth plots:
    
    recostartwire->Draw();
    recostarttime->Draw();
    recostartwirenodir->Draw();
    recostarttimenodir->Draw();
    reco2Dangle->Draw();
    reco2DangleGood->Draw();
    recoDir->Draw();
    
    // separation plots
    TCanvas * openAngle_c= new TCanvas("openAngle","openAngle");
    openAngle_sh->DrawCopy();
    openAngle_tr->DrawCopy("same");
   // openAngle_c->Print();
    
    TCanvas * openAngleChargeWgt_c= new TCanvas("openAngleChargeWgt","openAngleChargeWgt");
    openAngleChargeWgt_sh->DrawCopy();
    openAngleChargeWgt_tr->DrawCopy("same");
    
    TCanvas * closeAngle_c= new TCanvas("closeAngle_c","closeAngle_c");
    closeAngle_sh->DrawCopy();
    closeAngle_tr->DrawCopy("same");
    
    TCanvas * closeAngleChargeWgt_c= new TCanvas("closeAngleChargeWgt_c","closeAngleChargeWgt_c");
    closeAngleChargeWgt_sh->DrawCopy();
     closeAngleChargeWgt_tr->DrawCopy("same");
    
    TCanvas * hitDensity1d_c= new TCanvas("hitDensity1d_c","hitDensity1d_c");
    hitDensity1d_sh->DrawCopy();
    hitDensity1d_tr->DrawCopy("same");
    
    
    TCanvas * hitDensity2d_c= new TCanvas("hitDensity2d_c","hitDensity2d_c");
    hitDensity2d_sh->DrawCopy();
    hitDensity2d_tr->DrawCopy("same");
    
    TCanvas * modifiedHitDens_c= new TCanvas("modifiedHitDens_c","modifiedHitDens_c");
    modifiedHitDens_sh->DrawCopy();
    modifiedHitDens_tr->DrawCopy("same");
       
    TCanvas * multihitw_c= new TCanvas("multihitw_c","multihitw_c");
    multihitw_sh->DrawCopy();
    multihitw_tr->DrawCopy("same");  
    
    TCanvas * eigenvalue_principal_c= new TCanvas("eigenvalue_principal_c","eigenvalue_principal_c");
    eigenvalue_principal_sh ->DrawCopy();
    eigenvalue_principal_tr->DrawCopy("same");
    
    TCanvas * length_c= new TCanvas("length_c","length_c");
    length_sh->DrawCopy();
    length_tr->DrawCopy("same");
    
    TCanvas * width_c= new TCanvas("width_c","width_c");
    width_sh ->DrawCopy();
     width_tr ->DrawCopy("same");	  
	
   
    
    
   
   
   
 
   
   
  
  
    
    
    
     if(_fout) { 
	_fout->cd(); 
	sumCharge->Write();
	meanCharge->Write();
	meanX->Write();
	meanY->Write();
	rmsX->Write();
	rmsY->Write();
	chargeWgtX->Write();
	chargeWgtY->Write();
	clusterAngle2d->Write();
	angle2d->Write();
	
	openAngle->Write(); 
	openAngleChargeWgt->Write(); 
	closeAngle->Write();
	closeAngleChargeWgt->Write();    
/*	
	hitDensity2d->Write();
	hitDensity1d->Write();*/

	modifiedHitDens->Write(); 
	recostartwire->Write();
	recostarttime->Write();
	recostartwirenodir->Write();
	recostarttimenodir->Write();
	reco2Dangle->Write();
	reco2DangleGood->Write();
	recoDir->Write();
	
	openAngle_sh->Write();
      openAngleChargeWgt_sh->Write();
      closeAngle_sh->Write();
      closeAngleChargeWgt_sh->Write();
      hitDensity1d_sh->Write();
      hitDensity2d_sh->Write();
      modifiedHitDens_sh->Write();
      multihitw_sh->Write();
      eigenvalue_principal_sh ->Write();
      length_sh->Write();
      width_sh ->Write();
	  
	
      openAngle_tr->Write();
      openAngleChargeWgt_tr->Write();
      closeAngle_tr->Write();
      closeAngleChargeWgt_tr->Write();
      hitDensity1d_tr->Write();
      hitDensity2d_tr->Write();
      modifiedHitDens_tr->Write();
      multihitw_tr->Write();  
      eigenvalue_principal_tr->Write();
      length_tr->Write();
      width_tr ->Write();
	
	
	
      }

     else 
       print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    

    return true;
  }
}
#endif
