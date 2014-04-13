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

    if(!sumCharge) sumCharge = new TH1D("sumCharge","Sum charge",10,-3000,100000);
    else sumCharge->Reset();
    if(!meanCharge) meanCharge = new TH1D("meanCharge","Mean charge",10,-500,3500);
    else meanCharge->Reset();
    
    if(!meanX) meanX = new TH1D("meanX","Mean in x",12,0,110);
    else meanX->Reset();
    if(!meanY) meanY = new TH1D("meanY","Mean in y",10,0,60);
    else meanY->Reset();
    
    if(!rmsX) rmsX = new TH1D("rmsX","RMS in x",12,0,50);
    else rmsX->Reset();
    if(!rmsY) rmsY = new TH1D("rmsY","RMS in y",10,0,15);
    else rmsY->Reset();
		
    if(!chargeWgtX) chargeWgtX = new TH1D("chargeWgtX","Charge weighted x",12,10,100);
    else chargeWgtX->Reset();
    if(!chargeWgtY) chargeWgtY = new TH1D("chargeWgtY","Charge weigthed y",10,0,60);
    else chargeWgtY->Reset();
    
    if(!clusterAngle2d) clusterAngle2d = new TH1D("clusterAngle2d","Cluster angle 2d",12,0,100);
    else clusterAngle2d->Reset();
    if(!angle2d) angle2d = new TH1D("angle2d","Angle 2d",15,-200,200);
    else angle2d->Reset();
    
    if(!openAngle) openAngle = new TH1D("openAngle","Opening angle",10,0,2.25);
    else openAngle->Reset();
    if(!openAngleChargeWgt) openAngleChargeWgt = new TH1D("openAngleChargeWgt","Opening angle, charge weighted",10,0,2.0);
    else openAngleChargeWgt->Reset();
    
    if(!closeAngleChargeWgt) closeAngleChargeWgt = new TH1D("closeAngleChargeWgt","Closing angle, charge weighted",12,0,2.25);
    else closeAngleChargeWgt->Reset();
    if(!closeAngle) closeAngle = new TH1D("closeAngle","Close angle",10,0,2.25);
    else closeAngle->Reset();

    if(!hitDensity1d) hitDensity1d = new TH1D("hitDensity1d","Hit density 1d",12,0,10);
    else hitDensity1d->Reset();
    if(!hitDensity2d) hitDensity2d = new TH1D("hitDensity2d","Hit density 2d",10,0,60);
    else hitDensity2d->Reset();
    if(!modifiedHitDens) modifiedHitDens = new TH1D("modifiedHitDens","Modified Hit Density",10,0,20);
    else modifiedHitDens->Reset();


  //  if(!h7) h7 = new TH1D("h7","Opening angle, charge weighted",10,0,2.0);
 //   else h7->Reset();
  //  if(!h8) h8 = new TH1D("h8","Opening angle, charge weighted",10,0,2.0);
   // else h8->Reset();
    
    
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
    event_cluster * my_cluster_v = (event_cluster *)(storage->get_data(DATA::DBCluster));
  
    event_hit * my_hit_v = (event_hit*)(storage->get_data(my_cluster_v->get_hit_type()));
   
	
   //cluster::ClusterParamsAlgNew  fCPAlg; // = new cluster::ClusterParamsAlgNew();

 
    for(auto const clustit : *my_cluster_v) {
      std::cout << " Clust ID " << clustit.ID() << " Run: " << mu_cluster_v->run() << " SubRunID " << my_cluster_v->subrun() << " EventID " <<  my_cluster_v->event_id() << " " << my_cluster_v << std::endl;    

     //auto const hit_index_v = clustit.association(my_cluster_v->get_hit_type());
        auto const hit_index_v = clustit.association(DATA::FFTHit);
        std::vector<const larlight::hit *> hit_vector;
        hit_vector.clear();
        
        for(auto const hit_index : hit_index_v) {
            hit_vector.push_back( const_cast<const larlight::hit *>(&(my_hit_v->at(hit_index))) );
            my_hit_v->at(hit_index);
        }
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
	sumCharge->Draw();

	meanCharge->Fill(fResult.mean_charge); 
	meanCharge->Draw();

	rmsX->Fill(fResult.rms_x);
	rmsX->Draw();

	rmsY->Fill(fResult.rms_y);
	rmsY->Draw();

	meanX->Fill(fResult.mean_x);
 	meanX->Draw();

	meanY->Fill(fResult.mean_y);
	meanY->Draw();

	chargeWgtX->Fill(fResult.charge_wgt_x);
	chargeWgtX->Draw();

	chargeWgtY->Fill(fResult.charge_wgt_y);
	chargeWgtY->Draw();
	
	clusterAngle2d->Fill(fResult.cluster_angle_2d);
	clusterAngle2d->Draw();

	angle2d->Fill(fResult.angle_2d);
	angle2d->Draw();

	openAngle->Fill(fResult.opening_angle);
	openAngle->Draw();
	
	openAngleChargeWgt->Fill(fResult.opening_angle_charge_wgt);
//	openAngleChargeWgt->SetLineColor(3);
	openAngleChargeWgt->Draw() ; //"P same");

	closeAngle->Fill(fResult.closing_angle);
	closeAngle->Draw();
	
	closeAngleChargeWgt->Fill(fResult.closing_angle_charge_wgt);
//	closeAngleChargeWgt->SetLineColor(3);
	closeAngleChargeWgt->Draw() ; //"P same");

	hitDensity1d->Fill(fResult.hit_density_1D) ;
	hitDensity1d->Draw();

	hitDensity2d->Fill(fResult.hit_density_2D) ;
	hitDensity2d->Draw();

	modifiedHitDens->Fill(fResult.modified_hit_density) ;
	modifiedHitDens->Draw();

  }
  
  
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
	modifiedHitDens->Write(); }
     else 
       print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    

    return true;
  }
}
#endif
