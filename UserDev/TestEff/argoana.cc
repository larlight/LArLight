#include "argoana.hh"
#include <iostream>


bool larlight::argoana::is_matched_Loop(int inrun,int insubrun,int inevent,int &mcpdg,int &recopdg)
{
  
   Long64_t nentries = fChain->GetEntries();

   double RDIFF_CUT=12.; 
   double THETADIFF_CUT=0.17;
   
 // Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    //nb = fChain->GetEntry(jentry);   nbytes += nb;
    //if (Cut(ientry) < 0) continue;
    if (jentry%1000000==0) std::cout<<jentry<<"/"<<nentries<<std::endl;

    b_isdata->GetEntry(ientry);
    b_run->GetEntry(ientry);
    b_subrun->GetEntry(ientry);
    b_event->GetEntry(ientry);
    
    if(run!=inrun || subrun!=insubrun || inevent!=event)
	continue;
       
    
    b_vtxx_reco->GetEntry(ientry);
    b_vtxy_reco->GetEntry(ientry);
    b_vtxz_reco->GetEntry(ientry);
    b_ntracks_reco->GetEntry(ientry);
    b_trk_index_minos->GetEntry(ientry);
    b_trkvtxx->GetEntry(ientry);
    b_trkvtxy->GetEntry(ientry);
    b_trkvtxz->GetEntry(ientry);
    b_nmatched_reco->GetEntry(ientry);
    b_trk_mom_minos->GetEntry(ientry);
    b_trk_charge_minos->GetEntry(ientry);
    b_rdiff_minos->GetEntry(ientry);
    b_thetadiff_minos->GetEntry(ientry);
    b_rdiff_minos->GetEntry(ientry);
    b_xdiff_minos->GetEntry(ientry);
    b_ydiff_minos->GetEntry(ientry);
    b_thetadiff_minos->GetEntry(ientry);
    b_trackstart_dcosx_reco->GetEntry(ientry);
    b_trackstart_dcosy_reco->GetEntry(ientry);
    b_trackstart_dcosz_reco->GetEntry(ientry);
    b_trackexit_dcosx_reco->GetEntry(ientry);
    b_trackexit_dcosy_reco->GetEntry(ientry);
    b_trackexit_dcosz_reco->GetEntry(ientry);
    b_trackexit_x_reco->GetEntry(ientry);
    b_trackexit_y_reco->GetEntry(ientry);
    b_trackexit_z_reco->GetEntry(ientry);
    b_trk_dcosx_minos->GetEntry(ientry);
    b_trk_dcosy_minos->GetEntry(ientry);
    b_trk_dcosz_minos->GetEntry(ientry);
    b_trk_vtxx_minos->GetEntry(ientry);
    b_trk_vtxy_minos->GetEntry(ientry);
    b_trk_vtxz_minos->GetEntry(ientry);
    b_trackstart_x_reco->GetEntry(ientry);
    b_trackstart_y_reco->GetEntry(ientry);
    b_trackstart_z_reco->GetEntry(ientry);
    b_nearestz->GetEntry(ientry);
    b_nearesty->GetEntry(ientry);
    if (isdata==0){
      b_ccnc_truth->GetEntry(ientry);
      b_nuPDG_truth->GetEntry(ientry);
      b_mode_truth->GetEntry(ientry);
      b_pot->GetEntry(ientry);
      b_enu_truth->GetEntry(ientry);
      b_nuvtxx_truth->GetEntry(ientry);
      b_nuvtxy_truth->GetEntry(ientry);
      b_nuvtxz_truth->GetEntry(ientry);
      b_lep_dcosx_truth->GetEntry(ientry);
      b_lep_dcosy_truth->GetEntry(ientry);
      b_lep_dcosz_truth->GetEntry(ientry);
      b_lep_mom_truth->GetEntry(ientry);
      b_hitnuc_truth->GetEntry(ientry);
    }

    if (isdata){
      vtxx_reco -= 1.867576;
      trackstart_x_reco -= 1.867576;
    }

//     int iset = -1;
//     if (isdata == 1) iset = 0;
//     else iset = 1;
// 	 
//     if (iset == 1){
//       totpot[iset] += pot/300;
//     }


    //if (run==657&&event==10293) std::cout<<"found"<<std::endl;
    //if (run==657) std::cout<<"found"<<std::endl;
//     int itype = 0;
//     if (isdata==1){
//       itype = 1;
//       for (size_t i = 0; i<runs.size(); ++i){
// 	if (run==runs[i]&&event==events[i]) itype = 0;
//       }
//     }
//     else if (ccnc_truth==1) //nc
//       itype = 1;
//     else if (trk_charge_minos>-100&&((trk_charge_minos<0&&nuPDG_truth==-14)||(trk_charge_minos>0&&nuPDG_truth==14))) //WS
//       itype = 2;
//     else if (mode_truth==0)//ccqe
//       itype = 3;
//     else if (mode_truth==1)//ccres
//       itype = 4;
//     else 
//       itype = 5;

//     double wei = 1;
//     if (nuPDG_truth==12){
//       wei = nue_wei->GetBinContent(nue_wei->FindBin(enu_truth));
//     }
//     if (nuPDG_truth==14){
//       wei = numu_wei->GetBinContent(numu_wei->FindBin(enu_truth));
//     }
//     if (nuPDG_truth==-12){
//       wei = anue_wei->GetBinContent(anue_wei->FindBin(enu_truth));
//     }
//     if (nuPDG_truth==-14){
//       wei = anumu_wei->GetBinContent(anumu_wei->FindBin(enu_truth));
//     }

//     if (nuvtxx_truth>0.+FIDX_CUT 
// 	&&  nuvtxx_truth<47.-FIDX_CUT  
// 	&&  nuvtxy_truth>-20.+FIDY_CUT 
// 	&&  nuvtxy_truth<20.-FIDY_CUT
// 	&&  nuvtxz_truth>0.+FIDZup_CUT 
// 	&&  nuvtxz_truth<90.-FIDZdown_CUT){
//       if (ccnc_truth==0){
// 	double isowei = 1;
// 	if (hitnuc_truth == 2212) isowei = 20./18;
// 	else if (hitnuc_truth == 2112) isowei = 20./22;
// 	if (nuPDG_truth==14){
// 	  true_thz_nu->Fill(acos(lep_dcosz_truth*TMath::Cos(beam_angle)+lep_dcosy_truth*TMath::Sin(beam_angle))*180/3.1416,wei);
// 	  true_pmu_nu->Fill(lep_mom_truth,wei);
// 	  true_thz_wei_nu->Fill(acos(lep_dcosz_truth*TMath::Cos(beam_angle)+lep_dcosy_truth*TMath::Sin(beam_angle))*180/3.1416,wei*isowei);
// 	  true_pmu_wei_nu->Fill(lep_mom_truth,wei*isowei);
// 	}
// 	if (nuPDG_truth==-14){
// 	  true_thz_anu->Fill(acos(lep_dcosz_truth*TMath::Cos(beam_angle)+lep_dcosy_truth*TMath::Sin(beam_angle))*180/3.1416,wei);
// 	  true_pmu_anu->Fill(lep_mom_truth,wei);
// 	  true_thz_wei_anu->Fill(acos(lep_dcosz_truth*TMath::Cos(beam_angle)+lep_dcosy_truth*TMath::Sin(beam_angle))*180/3.1416,isowei*wei);
// 	  true_pmu_wei_anu->Fill(lep_mom_truth,isowei*wei);
// 	}
//       }
 //   }


  //  int icut = -1;
//     if (vtxx_reco>0.+FIDX_CUT
// 	&& vtxx_reco<47.-FIDX_CUT
// 	&& vtxy_reco>-20.+FIDY_CUT
// 	&& vtxy_reco<20.-FIDY_CUT
// 	&& vtxz_reco>0.+FIDZup_CUT
// 	&& vtxz_reco<90.-FIDZdown_CUT
// 	&& trackstart_x_reco>0.+FIDX_CUT
// 	&& trackstart_x_reco<47.-FIDX_CUT
// 	&& trackstart_y_reco>-20.+FIDY_CUT
// 	&& trackstart_y_reco<20.-FIDY_CUT
// 	&& trackstart_z_reco>0.+FIDZup_CUT
// 	&& trackstart_z_reco<90.-FIDZdown_CUT){
      
    //  if (isdata) ndatafv++;      
      
      if(nmatched_reco>=1&&trk_mom_minos>0){
	//	if (trkvtxx[trk_index_minos]>0.+FIDX_CUT
	//	    &&trkvtxx[trk_index_minos]<47.-FIDX_CUT
	//	    &&trkvtxy[trk_index_minos]>-20.+FIDY_CUT
	//	    &&trkvtxy[trk_index_minos]<20.-FIDY_CUT
	//	    &&trkvtxz[trk_index_minos]>0.+FIDZup_CUT
	//	    &&trkvtxz[trk_index_minos]<90.-FIDZdown_CUT
	//	    &&nearestz[2]>3){
	
	//if (nearestz[2]>3){
	
// 	  double lardirectionEnd[3]={trackexit_dcosx_reco, trackexit_dcosy_reco, trackexit_dcosz_reco};
// 	  double larEnd[3]= { trackexit_x_reco, trackexit_y_reco, trackexit_z_reco};
// 	  double minosdirectionStart[3]={ trk_dcosx_minos, trk_dcosy_minos, trk_dcosz_minos };
// 	  
// 	  double minosStart[3]={trk_vtxx_minos, trk_vtxy_minos, trk_vtxz_minos };
// 	  
// 	  project(lardirectionEnd, larEnd, minosdirectionStart,minosStart,xdiff,ydiff,rdiff,totaldiff,thetadiff,isdata);
// 	  
// 	  double minosvtx[3] = {(trk_vtxx_minos*100.)-117.4, (trk_vtxy_minos*100.)+19.3, (trk_vtxz_minos*100.)+147.1};
// 	  
// 	  double distance=sqrt(pow(vtxx_reco-minosvtx[0],2)+pow(vtxy_reco-minosvtx[1],2)+pow(vtxz_reco-minosvtx[2],2));
// 	  
// 	  MOM_OFFSET=(0.002198*distance)-.04404; 
// 	  
// 	  trk_mom_minos += MOM_OFFSET;
	  
	  
	  if(rdiff_minos<RDIFF_CUT&&thetadiff_minos<THETADIFF_CUT){
	  //if(rdiff<RDIFF_CUT&&thetadiff<THETADIFF_CUT){
// 	    if (trk_charge_minos<0){
// 	      icut = 0;
// 	    }
// 	    else if (trk_charge_minos>0){
// 	      icut = 1;
// 	    }
	    
	    if(trk_index_minos < ntracks_reco ) // reasonable situation 
	      {
		recopdg=trkpid[trk_index_minos][0];
		if (isdata==0 ){
		    mcpdg=mc_pdg_minos;
		 }
	      }
	    
	    return true;
	  }
	//}
      } // end nmatched_reco >=1
  //  }
  
  
  }
  
  mcpdg=0;
  recopdg=0;
  return false;
}




larlight::argoana::argoana(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {

#ifdef SINGLE_TREE
      // The following code should be used if you want this class to access
      // a single tree instead of a chain
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("Memory Directory");
      if (!f || !f->IsOpen()) {
         f = new TFile("Memory Directory");
      }
      f->GetObject("anatree",tree);

#else // SINGLE_TREE

      // The following code should be used if you want this class to access a chain
      // of trees.
      TChain * chain = new TChain("anatree","");
      //chain->Add("/data/argoneutnu/tjelectrons_hist.root/analysistree/anatree");
        chain->Add("/data/argoneutnu/tjneutrinos_hist.root/analysistree/anatree");
      tree = chain;
#endif // SINGLE_TREE

   }
   Init(tree);
}

larlight::argoana::~argoana()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t larlight::argoana::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t larlight::argoana::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void larlight::argoana::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("subrun", &subrun, &b_subrun);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("evttime", &evttime, &b_evttime);
   fChain->SetBranchAddress("beamtime", &beamtime, &b_beamtime);
   fChain->SetBranchAddress("pot", &pot, &b_pot);
   fChain->SetBranchAddress("isdata", &isdata, &b_isdata);
   fChain->SetBranchAddress("taulife", &taulife, &b_taulife);
   fChain->SetBranchAddress("vtxx_reco", &vtxx_reco, &b_vtxx_reco);
   fChain->SetBranchAddress("vtxy_reco", &vtxy_reco, &b_vtxy_reco);
   fChain->SetBranchAddress("vtxz_reco", &vtxz_reco, &b_vtxz_reco);
   fChain->SetBranchAddress("nclusu_reco", &nclusu_reco, &b_nclusu_reco);
   fChain->SetBranchAddress("nclusv_reco", &nclusv_reco, &b_nclusv_reco);
   fChain->SetBranchAddress("nclusw_reco", &nclusw_reco, &b_nclusw_reco);
   fChain->SetBranchAddress("ntracks_reco", &ntracks_reco, &b_ntracks_reco);
   fChain->SetBranchAddress("nvertextracks_reco", &nvertextracks_reco, &b_nvertextracks_reco);
   fChain->SetBranchAddress("nvertexclustersu_reco", &nvertexclustersu_reco, &b_nvertexclustersu_reco);
   fChain->SetBranchAddress("nvertexclustersv_reco", &nvertexclustersv_reco, &b_nvertexclustersv_reco);
   fChain->SetBranchAddress("nvertexclustersw_reco", &nvertexclustersw_reco, &b_nvertexclustersw_reco);
   fChain->SetBranchAddress("ntrackendonboundary_reco", &ntrackendonboundary_reco, &b_ntrackendonboundary_reco);
   fChain->SetBranchAddress("nearestx", nearestx, &b_nearestx);
   fChain->SetBranchAddress("nearesty", nearesty, &b_nearesty);
   fChain->SetBranchAddress("nearestz", nearestz, &b_nearestz);
   fChain->SetBranchAddress("trackstart_dcosx_reco", &trackstart_dcosx_reco, &b_trackstart_dcosx_reco);
   fChain->SetBranchAddress("trackstart_dcosy_reco", &trackstart_dcosy_reco, &b_trackstart_dcosy_reco);
   fChain->SetBranchAddress("trackstart_dcosz_reco", &trackstart_dcosz_reco, &b_trackstart_dcosz_reco);
   fChain->SetBranchAddress("trackexit_dcosx_reco", &trackexit_dcosx_reco, &b_trackexit_dcosx_reco);
   fChain->SetBranchAddress("trackexit_dcosy_reco", &trackexit_dcosy_reco, &b_trackexit_dcosy_reco);
   fChain->SetBranchAddress("trackexit_dcosz_reco", &trackexit_dcosz_reco, &b_trackexit_dcosz_reco);
   fChain->SetBranchAddress("trackstart_x_reco", &trackstart_x_reco, &b_trackstart_x_reco);
   fChain->SetBranchAddress("trackstart_y_reco", &trackstart_y_reco, &b_trackstart_y_reco);
   fChain->SetBranchAddress("trackstart_z_reco", &trackstart_z_reco, &b_trackstart_z_reco);
   fChain->SetBranchAddress("trackexit_x_reco", &trackexit_x_reco, &b_trackexit_x_reco);
   fChain->SetBranchAddress("trackexit_y_reco", &trackexit_y_reco, &b_trackexit_y_reco);
   fChain->SetBranchAddress("trackexit_z_reco", &trackexit_z_reco, &b_trackexit_z_reco);
   fChain->SetBranchAddress("enu_reco", &enu_reco, &b_enu_reco);
   fChain->SetBranchAddress("nclupertrack_reco", &nclupertrack_reco, &b_nclupertrack_reco);
   fChain->SetBranchAddress("trkvtxx", trkvtxx, &b_trkvtxx);
   fChain->SetBranchAddress("trkvtxy", trkvtxy, &b_trkvtxy);
   fChain->SetBranchAddress("trkvtxz", trkvtxz, &b_trkvtxz);
   fChain->SetBranchAddress("trkendx", trkendx, &b_trkendx);
   fChain->SetBranchAddress("trkendy", trkendy, &b_trkendy);
   fChain->SetBranchAddress("trkendz", trkendz, &b_trkendz);
   fChain->SetBranchAddress("trkstartdcosx", trkstartdcosx, &b_trkstartdcosx);
   fChain->SetBranchAddress("trkstartdcosy", trkstartdcosy, &b_trkstartdcosy);
   fChain->SetBranchAddress("trkstartdcosz", trkstartdcosz, &b_trkstartdcosz);
   fChain->SetBranchAddress("trkenddcosx", trkenddcosx, &b_trkenddcosx);
   fChain->SetBranchAddress("trkenddcosy", trkenddcosy, &b_trkenddcosy);
   fChain->SetBranchAddress("trkenddcosz", trkenddcosz, &b_trkenddcosz);
   fChain->SetBranchAddress("trkke", trkke, &b_trkke);
   fChain->SetBranchAddress("trkrange", trkrange, &b_trkrange);
   fChain->SetBranchAddress("trkpitchc", trkpitchc, &b_trkpitchc);
   fChain->SetBranchAddress("nmaxtrkhits", nmaxtrkhits, &b_nmaxtrkhits);
   fChain->SetBranchAddress("ntrkhits", ntrkhits, &b_ntrkhits);
   fChain->SetBranchAddress("trkdedx", trkdedx, &b_trkdedx);
   fChain->SetBranchAddress("trkdqdx", trkdqdx, &b_trkdqdx);
   fChain->SetBranchAddress("trkresrg", trkresrg, &b_trkresrg);
   fChain->SetBranchAddress("trkhitindexc", trkhitindexc, &b_trkhitindexc);
   fChain->SetBranchAddress("trkpid", trkpid, &b_trkpid);
   fChain->SetBranchAddress("trkpidndf", trkpidndf, &b_trkpidndf);
   fChain->SetBranchAddress("trkpidchi2", trkpidchi2, &b_trkpidchi2);
   fChain->SetBranchAddress("trkpida", trkpida, &b_trkpida);
   fChain->SetBranchAddress("trkmissinge", trkmissinge, &b_trkmissinge);
   fChain->SetBranchAddress("trkmissingeavg", trkmissingeavg, &b_trkmissingeavg);
   fChain->SetBranchAddress("trktruepdgu", trktruepdgu, &b_trktruepdgu);
   fChain->SetBranchAddress("trktrueeffu", trktrueeffu, &b_trktrueeffu);
   fChain->SetBranchAddress("trktruepuru", trktruepuru, &b_trktruepuru);
   fChain->SetBranchAddress("trktruepdgv", trktruepdgv, &b_trktruepdgv);
   fChain->SetBranchAddress("trktrueeffv", trktrueeffv, &b_trktrueeffv);
   fChain->SetBranchAddress("trktruepurv", trktruepurv, &b_trktruepurv);
   fChain->SetBranchAddress("nmatched_reco", &nmatched_reco, &b_nmatched_reco);
   fChain->SetBranchAddress("trk_mom_minos", &trk_mom_minos, &b_trk_mom_minos);
   fChain->SetBranchAddress("trk_charge_minos", &trk_charge_minos, &b_trk_charge_minos);
   fChain->SetBranchAddress("trk_dcosx_minos", &trk_dcosx_minos, &b_trk_dcosx_minos);
   fChain->SetBranchAddress("trk_dcosy_minos", &trk_dcosy_minos, &b_trk_dcosy_minos);
   fChain->SetBranchAddress("trk_dcosz_minos", &trk_dcosz_minos, &b_trk_dcosz_minos);
   fChain->SetBranchAddress("trk_vtxx_minos", &trk_vtxx_minos, &b_trk_vtxx_minos);
   fChain->SetBranchAddress("trk_vtxy_minos", &trk_vtxy_minos, &b_trk_vtxy_minos);
   fChain->SetBranchAddress("trk_vtxz_minos", &trk_vtxz_minos, &b_trk_vtxz_minos);
   fChain->SetBranchAddress("trk_index_minos", &trk_index_minos, &b_trk_index_minos);
   fChain->SetBranchAddress("mc_index_minos", &mc_index_minos, &b_mc_index_minos);
   fChain->SetBranchAddress("mc_pdg_minos", &mc_pdg_minos, &b_mc_pdg_minos);
   fChain->SetBranchAddress("mc_px_minos", &mc_px_minos, &b_mc_px_minos);
   fChain->SetBranchAddress("mc_py_minos", &mc_py_minos, &b_mc_py_minos);
   fChain->SetBranchAddress("mc_pz_minos", &mc_pz_minos, &b_mc_pz_minos);
   fChain->SetBranchAddress("mc_ene_minos", &mc_ene_minos, &b_mc_ene_minos);
   fChain->SetBranchAddress("mc_mass_minos", &mc_mass_minos, &b_mc_mass_minos);
   fChain->SetBranchAddress("mc_vtxx_minos", &mc_vtxx_minos, &b_mc_vtxx_minos);
   fChain->SetBranchAddress("mc_vtxy_minos", &mc_vtxy_minos, &b_mc_vtxy_minos);
   fChain->SetBranchAddress("mc_vtxz_minos", &mc_vtxz_minos, &b_mc_vtxz_minos);
   fChain->SetBranchAddress("trkcontained_minos", &trkcontained_minos, &b_trkcontained_minos);
   fChain->SetBranchAddress("test_charge_minos", &test_charge_minos, &b_test_charge_minos);
   fChain->SetBranchAddress("rdiff_minos", &rdiff_minos, &b_rdiff_minos);
   fChain->SetBranchAddress("xdiff_minos", &xdiff_minos, &b_xdiff_minos);
   fChain->SetBranchAddress("ydiff_minos", &ydiff_minos, &b_ydiff_minos);
   fChain->SetBranchAddress("thetadiff_minos", &thetadiff_minos, &b_thetadiff_minos);
   fChain->SetBranchAddress("parpdg", &parpdg, &b_parpdg);
   fChain->SetBranchAddress("parmom", &parmom, &b_parmom);
   fChain->SetBranchAddress("nuPDG_truth", &nuPDG_truth, &b_nuPDG_truth);
   fChain->SetBranchAddress("ccnc_truth", &ccnc_truth, &b_ccnc_truth);
   fChain->SetBranchAddress("mode_truth", &mode_truth, &b_mode_truth);
   fChain->SetBranchAddress("enu_truth", &enu_truth, &b_enu_truth);
   fChain->SetBranchAddress("Q2_truth", &Q2_truth, &b_Q2_truth);
   fChain->SetBranchAddress("hitnuc_truth", &hitnuc_truth, &b_hitnuc_truth);
   fChain->SetBranchAddress("W_truth", &W_truth, &b_W_truth);
   fChain->SetBranchAddress("nuvtxx_truth", &nuvtxx_truth, &b_nuvtxx_truth);
   fChain->SetBranchAddress("nuvtxy_truth", &nuvtxy_truth, &b_nuvtxy_truth);
   fChain->SetBranchAddress("nuvtxz_truth", &nuvtxz_truth, &b_nuvtxz_truth);
   fChain->SetBranchAddress("nu_dcosx_truth", &nu_dcosx_truth, &b_nu_dcosx_truth);
   fChain->SetBranchAddress("nu_dcosy_truth", &nu_dcosy_truth, &b_nu_dcosy_truth);
   fChain->SetBranchAddress("nu_dcosz_truth", &nu_dcosz_truth, &b_nu_dcosz_truth);
   fChain->SetBranchAddress("lep_mom_truth", &lep_mom_truth, &b_lep_mom_truth);
   fChain->SetBranchAddress("lep_dcosx_truth", &lep_dcosx_truth, &b_lep_dcosx_truth);
   fChain->SetBranchAddress("lep_dcosy_truth", &lep_dcosy_truth, &b_lep_dcosy_truth);
   fChain->SetBranchAddress("lep_dcosz_truth", &lep_dcosz_truth, &b_lep_dcosz_truth);
   fChain->SetBranchAddress("mcevts_truth", &mcevts_truth, &b_mcevts_truth);
   fChain->SetBranchAddress("beamwgt", &beamwgt, &b_beamwgt);
   fChain->SetBranchAddress("tpx_flux", &tpx_flux, &b_tpx_flux);
   fChain->SetBranchAddress("tpy_flux", &tpy_flux, &b_tpy_flux);
   fChain->SetBranchAddress("tpz_flux", &tpz_flux, &b_tpz_flux);
   fChain->SetBranchAddress("tptype_flux", &tptype_flux, &b_tptype_flux);
   fChain->SetBranchAddress("no_dead_wires_muon", &no_dead_wires_muon, &b_no_dead_wires_muon);
   fChain->SetBranchAddress("no_hits", &no_hits, &b_no_hits);
   fChain->SetBranchAddress("hit_plane", hit_plane, &b_hit_plane);
   fChain->SetBranchAddress("hit_wire", hit_wire, &b_hit_wire);
   fChain->SetBranchAddress("hit_channel", hit_channel, &b_hit_channel);
   fChain->SetBranchAddress("hit_peakT", hit_peakT, &b_hit_peakT);
   fChain->SetBranchAddress("hit_charge", hit_charge, &b_hit_charge);
   fChain->SetBranchAddress("hit_ph", hit_ph, &b_hit_ph);
   fChain->SetBranchAddress("hit_trkid", hit_trkid, &b_hit_trkid);
   fChain->SetBranchAddress("hit_etruth", hit_etruth, &b_hit_etruth);
   fChain->SetBranchAddress("twodvtx_w_reco", twodvtx_w_reco, &b_twodvtx_w_reco);
   fChain->SetBranchAddress("twodvtx_t_reco", twodvtx_t_reco, &b_twodvtx_t_reco);
   fChain->SetBranchAddress("twodvtx_w_truth", twodvtx_w_truth, &b_twodvtx_w_truth);
   fChain->SetBranchAddress("twodvtx_t_truth", twodvtx_t_truth, &b_twodvtx_t_truth);
   fChain->SetBranchAddress("nkingaclustersu_reco", &nkingaclustersu_reco, &b_nkingaclustersu_reco);
   fChain->SetBranchAddress("nkingaclustersv_reco", &nkingaclustersv_reco, &b_nkingaclustersv_reco);
   fChain->SetBranchAddress("nvertexkingaclustersu_reco", &nvertexkingaclustersu_reco, &b_nvertexkingaclustersu_reco);
   fChain->SetBranchAddress("nvertexkingaclustersv_reco", &nvertexkingaclustersv_reco, &b_nvertexkingaclustersv_reco);
   fChain->SetBranchAddress("nlinemergerclustersu_reco", &nlinemergerclustersu_reco, &b_nlinemergerclustersu_reco);
   fChain->SetBranchAddress("nlinemergerclustersv_reco", &nlinemergerclustersv_reco, &b_nlinemergerclustersv_reco);
   fChain->SetBranchAddress("nvertexlinemergerclustersu_reco", &nvertexlinemergerclustersu_reco, &b_nvertexlinemergerclustersu_reco);
   fChain->SetBranchAddress("nvertexlinemergerclustersv_reco", &nvertexlinemergerclustersv_reco, &b_nvertexlinemergerclustersv_reco);
   fChain->SetBranchAddress("ndbscanclustersu_reco", &ndbscanclustersu_reco, &b_ndbscanclustersu_reco);
   fChain->SetBranchAddress("ndbscanclustersv_reco", &ndbscanclustersv_reco, &b_ndbscanclustersv_reco);
   fChain->SetBranchAddress("nvertexdbscanclustersu_reco", &nvertexdbscanclustersu_reco, &b_nvertexdbscanclustersu_reco);
   fChain->SetBranchAddress("nvertexdbscanclustersv_reco", &nvertexdbscanclustersv_reco, &b_nvertexdbscanclustersv_reco);
   fChain->SetBranchAddress("no_kingaclusters", &no_kingaclusters, &b_no_kingaclusters);
   fChain->SetBranchAddress("kingaclusters_planeNo", &kingaclusters_planeNo, &b_kingaclusters_planeNo);
   fChain->SetBranchAddress("Start_pt_w_kingaCl", &Start_pt_w_kingaCl, &b_Start_pt_w_kingaCl);
   fChain->SetBranchAddress("Start_pt_t_kingaCl", &Start_pt_t_kingaCl, &b_Start_pt_t_kingaCl);
   fChain->SetBranchAddress("no_linemergerclusters", &no_linemergerclusters, &b_no_linemergerclusters);
   fChain->SetBranchAddress("linemergerclusters_planeNo", &linemergerclusters_planeNo, &b_linemergerclusters_planeNo);
   fChain->SetBranchAddress("Start_pt_w_linemergerCl", &Start_pt_w_linemergerCl, &b_Start_pt_w_linemergerCl);
   fChain->SetBranchAddress("Start_pt_t_linemergerCl", &Start_pt_t_linemergerCl, &b_Start_pt_t_linemergerCl);
   fChain->SetBranchAddress("no_fuzzyclusters", &no_fuzzyclusters, &b_no_fuzzyclusters);
   fChain->SetBranchAddress("fNhitsClust", &fNhitsClust, &b_fNhitsClust);
   fChain->SetBranchAddress("two_trackstart_dcosx_reco", two_trackstart_dcosx_reco, &b_two_trackstart_dcosx_reco);
   fChain->SetBranchAddress("two_trackstart_dcosy_reco", two_trackstart_dcosy_reco, &b_two_trackstart_dcosy_reco);
   fChain->SetBranchAddress("two_trackstart_dcosz_reco", two_trackstart_dcosz_reco, &b_two_trackstart_dcosz_reco);
   fChain->SetBranchAddress("two_trackexit_dcosx_reco", two_trackexit_dcosx_reco, &b_two_trackexit_dcosx_reco);
   fChain->SetBranchAddress("two_trackexit_dcosy_reco", two_trackexit_dcosy_reco, &b_two_trackexit_dcosy_reco);
   fChain->SetBranchAddress("two_trackexit_dcosz_reco", two_trackexit_dcosz_reco, &b_two_trackexit_dcosz_reco);
   fChain->SetBranchAddress("no_primaries", &no_primaries, &b_no_primaries);
   fChain->SetBranchAddress("geant_list_size", &geant_list_size, &b_geant_list_size);
   fChain->SetBranchAddress("pdg", pdg, &b_pdg);
   fChain->SetBranchAddress("Eng", Eng, &b_Eng);
   fChain->SetBranchAddress("Px", Px, &b_Px);
   fChain->SetBranchAddress("Py", Py, &b_Py);
   fChain->SetBranchAddress("Pz", Pz, &b_Pz);
   fChain->SetBranchAddress("StartPointx", StartPointx, &b_StartPointx);
   fChain->SetBranchAddress("StartPointy", StartPointy, &b_StartPointy);
   fChain->SetBranchAddress("StartPointz", StartPointz, &b_StartPointz);
   fChain->SetBranchAddress("EndPointx", EndPointx, &b_EndPointx);
   fChain->SetBranchAddress("EndPointy", EndPointy, &b_EndPointy);
   fChain->SetBranchAddress("EndPointz", EndPointz, &b_EndPointz);
   fChain->SetBranchAddress("NumberDaughters", NumberDaughters, &b_NumberDaughters);
   fChain->SetBranchAddress("Mother", Mother, &b_Mother);
   fChain->SetBranchAddress("TrackId", TrackId, &b_TrackId);
   fChain->SetBranchAddress("process_primary", process_primary, &b_process_primary);
   fChain->SetBranchAddress("Kin_Eng_reco", Kin_Eng_reco, &b_Kin_Eng_reco);
   fChain->SetBranchAddress("muon_Kin_Eng_reco", &muon_Kin_Eng_reco, &b_muon_Kin_Eng_reco);
   fChain->SetBranchAddress("genie_no_primaries", &genie_no_primaries, &b_genie_no_primaries);
   fChain->SetBranchAddress("genie_primaries_pdg", genie_primaries_pdg, &b_genie_primaries_pdg);
   fChain->SetBranchAddress("genie_Eng", genie_Eng, &b_genie_Eng);
   fChain->SetBranchAddress("genie_Px", genie_Px, &b_genie_Px);
   fChain->SetBranchAddress("genie_Py", genie_Py, &b_genie_Py);
   fChain->SetBranchAddress("genie_Pz", genie_Pz, &b_genie_Pz);
   fChain->SetBranchAddress("genie_P", genie_P, &b_genie_P);
   fChain->SetBranchAddress("genie_status_code", genie_status_code, &b_genie_status_code);
   fChain->SetBranchAddress("genie_mass", genie_mass, &b_genie_mass);
   fChain->SetBranchAddress("genie_trackID", genie_trackID, &b_genie_trackID);
   fChain->SetBranchAddress("genie_ND", genie_ND, &b_genie_ND);
   fChain->SetBranchAddress("genie_mother", genie_mother, &b_genie_mother);
   fChain->SetBranchAddress("pmttime", &pmttime, &b_pmttime);
   fChain->SetBranchAddress("pmt1", pmt1, &b_pmt1);
   fChain->SetBranchAddress("pmt2", pmt2, &b_pmt2);
   fChain->SetBranchAddress("pmt3", pmt3, &b_pmt3);
   fChain->SetBranchAddress("pmt4", pmt4, &b_pmt4);
   Notify();
}

Bool_t larlight::argoana::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void larlight::argoana::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t larlight::argoana::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}

















