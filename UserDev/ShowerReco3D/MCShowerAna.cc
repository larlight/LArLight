#ifndef MCSHOWERANA_CC
#define MCSHOWERANA_CC

#include "MCShowerAna.hh"

namespace larlight {

  MCShowerAna::MCShowerAna() : ana_base()
  {
    _name = "MCShowerAna";

    hMCX = hMCY = hMCZ = 0;
    hdcosX = hdcosY = hdcosZ = 0;

    hMCEdepMCQ = 0;

    hAmpQEff = 0;
    
    hEner = hEnerFrac = hDetEnerFrac = 0;
    
    hMCEner = recohEner = 0;

    hMCMotherEner = 0;

    hdEdx = 0;

  }

  bool MCShowerAna::initialize() {

    hMCEdepEff = new TH1D("hMCEdepEff","MC Deposited / Particle Energy; Fraction; MCShower",
			  201,-0.0025,1.0025);

    hMCQEff = new TH1D("hMCQEff","MC Charge Detected / Deposited Charge; Fraction; MCShower",
		       201,-0.0025,1.0025);

    hMCEdepMCQ = new TH2D("hMCEdepMCQ",
			  "MC Energy vs. # of Electrons; # electrons; Energy [MeV]",
			  100,0,3.e5,100,0,2000);
    
    hAmpQEff = new TH1D("hAmpQEff","# Electrons from Hit Amp / MC; Ratio; Collection Hits",
			100,0,2);

    hMCX = new TH1D("hMCX","",100,-120,120);
    hMCY = new TH1D("hMCY","",100,-120,120);
    hMCZ = new TH1D("hMCZ","",100,-120,120);
    
    hdcosX = new TH1D("hdcosX","",100,-5,5);
    hdcosY = new TH1D("hdcosY","",100,-5,5);
    hdcosZ = new TH1D("hdcosZ","",100,-5,5);
    
    
    hEnerFrac = new TH1D("hEnerFrac","MC-Reco Fractional Difference (Reco vs. Detected MC); Diff-Frac; Reco Shower",100,-5,5);
    hDetEnerFrac = new TH1D("hDetEnerFrac","MC-MC Fractional Difference (Mother vs. Detected); Diff-Frac; MC Shower",100,0,1);
    hMCEner = new TH1D("hMCEner","Deposited MC Shower Energy; MeV; MCShower",100,-10,2000);
    hMCMotherEner = new TH1D("hMCMotherEner","Mother Energy; MeV; MCShower",100,-10,2000);
    recohEner = new TH1D("recohEner","",100,-10,2000);
    recorrhEner = new TH1D("recorrhEner","",100,-1000,1000);
    
    //// New Histograms
    // MIP Energy vs. True
    hEner = new TH1D("hEner","MC True-Reco  Energy; MeV; Reco Shower",100,-1000,1000);
    //  Energy vs. Deposited
    hEnerDep = new TH1D("hEnerDep","MC Dep-Reco  Energy; MeV; Reco Shower",100,-1000,1000);
    // corrected  vs. Deposited
    hEnerDepCorr = new TH1D("hEnerDepCorr","MC Dep-Reco  Energy,Corr; MeV; Reco Shower",100,-1000,1000);
    //  Energy - Deposited vs. Deposited
    hEnerDep2D = new TH2D("hEnerDepvsEn","MC Dep-Reco  Energy vs En; MeV; Reco Shower",100,0,2000,100,-1000,1000);
    
    
    // MIP Energy vs. True
    hMIPEner = new TH1D("hMIPEner","MC True-Reco MIP Energy; MeV; Reco Shower",100,-1000,1000);
    // MIP Energy vs. Deposited
    hMIPEnerDep = new TH1D("hMIPEnerDep","MC Dep-Reco MIP Energy; MeV; Reco Shower",100,-1000,1000);
    // corrected MIP vs. Deposited
    hMIPEnerDepCorr = new TH1D("hMIPEnerDepCorr","MC Dep-Reco MIP Energy,Corr; MeV; Reco Shower",100,-1000,1000);
    // MIP Energy - Deposited vs. Deposited
    hMIPEnerDep2D = new TH2D("hMIPEnerDepvsEn","MC Dep-Reco MIP Energy vs En; MeV; Reco Shower",100,0,2000,100,-1000,1000);
    
    
    hdEdx = new TH1D("hdEdx","",60,0,12);

    return true;
  }
  
  bool MCShowerAna::analyze(storage_manager* storage) {

    auto geo  = ::larutil::Geometry::GetME();
    auto detp = ::larutil::DetectorProperties::GetME();

    // Get data products from storage
    auto mcshower_v = (event_mcshower* )( storage->get_data(DATA::MCShower) );
    auto shower_v   = (event_shower*   )( storage->get_data(DATA::Shower)   );
    auto cluster_v  = (event_cluster*  )( storage->get_data(DATA::Cluster)  );
    auto hit_v      = (event_hit*      )( storage->get_data(DATA::GausHit)  );
    
    // Check data exists
    if( !mcshower_v || !shower_v || !cluster_v || !hit_v) {
      std::cerr<<"Missing some data! not doing anything..."<<std::endl;
      return false;
    }

    // Here, we assume there is only 1 MCShower exists in an event
    // because otherwise we have to think about which MCShower possibly corresponds to 
    // a given reconstructed shower.
    if( mcshower_v->size() != 1) {
      std::cerr<<Form("Found %zu MCShower! (>1) Ignore this event... ",mcshower_v->size())<<std::endl;
      return false;
    }

    //
    // Use LArLight's version of BackTracker
    //
    if(!fBTAlg.Prepare(storage)) return false;

    auto mcshower_index = fBTAlg.ShowerIDs().at(0);

    auto const& mcs = mcshower_v->at(mcshower_index);
    auto const& mc_mom_orig = mcs.MotherMomentum();
    auto const& mc_mom = mcs.DaughterMomentum();
    auto const& mc_vtx = mcs.MotherPosition();
    std::vector<double> mc_dcos(3,0);
    for(size_t i=0; i<3; ++i)
      mc_dcos.at(i) = mc_mom.at(i)/mc_mom.at(3);

    hDetEnerFrac->Fill( (mc_mom_orig.at(3)*1.e3 - mc_mom.at(3)) / (mc_mom.at(3) + mc_mom_orig.at(3)*1.e3) * 2 );

    // Loop over reco-ed shower and loop over corresponding clusters & hits 
    // to fill MC charge information
    for(auto const& reco_shower : *shower_v) {
      
      auto& cluster_index = reco_shower.association(DATA::Cluster);

      std::vector<double> recoq(3,0);        // Reconstructed charge on a given plane
      std::vector<double> mcq_total(3,0);    // Total MCShower's charge on a given plane
      std::vector<double> mcq_detected(3,0); // Detected MCShower's charge 
      
      // Loop over associated clusters to fill mc/reco charge info 
      for(auto const& c_index : cluster_index) {

	auto& hit_index = cluster_v->at(c_index).association(DATA::GausHit);
	UChar_t plane = geo->ChannelToPlane(hit_v->at(hit_index.at(0)).Channel());

	std::vector<const larlight::hit*> hits;
	hits.reserve(hit_index.size());
	for(auto const& h_index : hit_index) {
	  hits.push_back(&(hit_v->at(h_index)));
	  int multiplier=1;
	  if(plane<2) multiplier=2;
	  recoq.at(plane) += hit_v->at(h_index).Charge(true)*multiplier;
	}
	mcq_total.at(plane)    = mcs.Charge(GEO::View_t(plane));
	mcq_detected.at(plane) = fBTAlg.MCShowerQ(hits).at(0) * detp->ElectronsToADC();
      }

      auto const& reco_vtx  = reco_shower.ShowerStart();
      auto const& reco_dcos = reco_shower.Direction();
      auto const& reco_en   = reco_shower.Energy();
      auto const& reco_MIPen   = reco_shower.MIPEnergy();
      
      /*
      for(size_t i=0; i<3; ++i) {
	std::cout << "Plane " << i << std::endl
		  << "Reco-ed Q: " << recoq.at(i) << std::endl
		  << "MC Q: " << mcq_total.at(i) << std::endl
		  << "MC Q in this cluster: " << mcq_detected.at(i)  << std::endl
		  << " correction factor : " << (mcq_detected.at(i)/mcq_total.at(i)) << std::endl
		  << "corrected reco r: " << reco_en.at(i)/(mcq_detected.at(i)/mcq_total.at(i))
		  << "corrected reco MIP r: " << reco_MIPen.at(i)/(mcq_detected.at(i)/mcq_total.at(i)) 
		  << std::endl
		  << std::endl;
      }
      */

      // 
      // Fill histograms
      //

      // MC-based efficiency
      hMCEdepEff->Fill( mc_mom.at(3) / ((mc_mom_orig.at(3)*1.e3)) );
      hMCQEff->Fill( mcq_detected.at(2) / mcq_total.at(2) );

      // dEdX
      hdEdx->Fill(reco_shower.dEdx().at(2));

      // Shower start XYZ
      hMCX->Fill(mc_vtx.at(0) - reco_vtx[0]);
      hMCY->Fill(mc_vtx.at(1) - reco_vtx[1]);
      hMCZ->Fill(mc_vtx.at(2) - reco_vtx[2]);

      // dcos
      hdcosX->Fill(mc_dcos.at(0) - reco_dcos[0]);
      hdcosY->Fill(mc_dcos.at(1) - reco_dcos[1]);
      hdcosZ->Fill(mc_dcos.at(2) - reco_dcos[2]);

      // Energy
      hEner->Fill(mc_mom.at(3) - reco_en.at(2));


      hMIPEner->Fill(mc_mom_orig.at(3)*1e3 - reco_MIPen.at(2));
      hMIPEnerDep->Fill(mc_mom.at(3) - reco_MIPen.at(2)); 
      hMIPEnerDepCorr->Fill(mc_mom.at(3) - reco_MIPen.at(2)/(mcq_detected.at(2)/mcq_total.at(2)));
      hMIPEnerDep2D->Fill(mc_mom.at(3),mc_mom.at(3) - reco_MIPen.at(2)); 
      
      hEner->Fill(mc_mom_orig.at(3)*1e3 - reco_en.at(2));
      hEnerDep->Fill(mc_mom.at(3) - reco_en.at(2)); 
      hEnerDepCorr->Fill(mc_mom.at(3) - reco_en.at(2)/(mcq_detected.at(2)/mcq_total.at(2)));
      hEnerDep2D->Fill(mc_mom.at(3),mc_mom.at(3) - reco_en.at(2)); 
      
      
      hMCEner->Fill(mc_mom.at(3));
      recohEner->Fill(reco_en.at(2));
      hMCEner->Fill(mc_mom.at(3));
      hMCMotherEner->Fill(mc_mom_orig.at(3)*1.e3);

      hMCEdepMCQ->Fill(mcq_detected.at(2),mc_mom.at(3));

      hAmpQEff->Fill(recoq.at(2) / mcq_detected.at(2));

      recorrhEner->Fill(mc_mom_orig.at(3)*1.e3 - reco_en.at(2)/(mcq_detected.at(2)/mcq_total.at(2)));
      
      hEnerFrac->Fill( (mc_mom.at(3) - reco_en.at(2)) / (mc_mom.at(3) + reco_en.at(2)) * 2. );
      /*
      std::cout
	<< "MCShower Mother Energy    : " << mc_mom_orig.at(3)*1.e3  << std::endl
	<< "MCShower Deposited Energy : " << mc_mom.at(3)            << std::endl
	<< "Reco-ed Energy            : " << reco_en.at(2)           << std::endl
	<< std::endl;
      */      
    }
    
    return true;
  }

  bool MCShowerAna::finalize() {

    if(_fout) {
      
      hMCEdepEff->Write();
      hMCQEff->Write();

      hMCEdepMCQ->Write();

      hAmpQEff->Write();
      
      hMCX->Write();
      hMCY->Write();
      hMCZ->Write();
    
      hdcosX->Write();
      hdcosY->Write();
      hdcosZ->Write();
    
      //hEner->Write();
      hEnerFrac->Write();
      hDetEnerFrac->Write();
      hMCEner->Write();
      hMCMotherEner->Write();
      recohEner->Write();
    
      recorrhEner->Write();
      
      hdEdx->Write();
      
      hMIPEner->Write();
      hMIPEnerDep->Write(); 
      hMIPEnerDepCorr->Write(); 
      hMIPEnerDep2D->Write(); 
      
      hEner->Write();
      hEnerDep->Write(); 
      hEnerDepCorr->Write(); 
      hEnerDep2D->Write(); 
      
    }    
    return true;
  }

}
#endif
