//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Mar 28 20:34:34 2014 by ROOT version 5.34/12
// from TChain argoanatree/
//////////////////////////////////////////////////////////

#ifndef argoana_h
#define argoana_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "ana_base.hh"
// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

namespace larlight {

class argoana {
public :
   TTree          *fChain;   //!pointer to the argoanalyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           run;
   Int_t           subrun;
   Int_t           event;
   Double_t        evttime;
   Double_t        beamtime;
   Double_t        pot;
   Int_t           isdata;
   Double_t        taulife;
   Double_t        vtxx_reco;
   Double_t        vtxy_reco;
   Double_t        vtxz_reco;
   Int_t           nclusu_reco;
   Int_t           nclusv_reco;
   Int_t           nclusw_reco;
   Int_t           ntracks_reco;
   Int_t           nvertextracks_reco;
   Int_t           nvertexclustersu_reco;
   Int_t           nvertexclustersv_reco;
   Int_t           nvertexclustersw_reco;
   Int_t           ntrackendonboundary_reco;
   Double_t        nearestx[3];
   Double_t        nearesty[3];
   Double_t        nearestz[3];
   Double_t        trackstart_dcosx_reco;
   Double_t        trackstart_dcosy_reco;
   Double_t        trackstart_dcosz_reco;
   Double_t        trackexit_dcosx_reco;
   Double_t        trackexit_dcosy_reco;
   Double_t        trackexit_dcosz_reco;
   Double_t        trackstart_x_reco;
   Double_t        trackstart_y_reco;
   Double_t        trackstart_z_reco;
   Double_t        trackexit_x_reco;
   Double_t        trackexit_y_reco;
   Double_t        trackexit_z_reco;
   Double_t        enu_reco;
   Int_t           nclupertrack_reco;
   Double_t        trkvtxx[42];   //[ntracks_reco]
   Double_t        trkvtxy[42];   //[ntracks_reco]
   Double_t        trkvtxz[42];   //[ntracks_reco]
   Double_t        trkendx[42];   //[ntracks_reco]
   Double_t        trkendy[42];   //[ntracks_reco]
   Double_t        trkendz[42];   //[ntracks_reco]
   Double_t        trkstartdcosx[42];   //[ntracks_reco]
   Double_t        trkstartdcosy[42];   //[ntracks_reco]
   Double_t        trkstartdcosz[42];   //[ntracks_reco]
   Double_t        trkenddcosx[42];   //[ntracks_reco]
   Double_t        trkenddcosy[42];   //[ntracks_reco]
   Double_t        trkenddcosz[42];   //[ntracks_reco]
   Double_t        trkke[42][2];   //[ntracks_reco]
   Double_t        trkrange[42][2];   //[ntracks_reco]
   Double_t        trkpitchc[42][2];   //[ntracks_reco]
   Int_t           nmaxtrkhits[2];
   Int_t           ntrkhits[42][2];   //[ntracks_reco]
   Double_t        trkdedx[42][2][1000];   //[ntracks_reco]
   Double_t        trkdqdx[42][2][1000];   //[ntracks_reco]
   Double_t        trkresrg[42][2][1000];   //[ntracks_reco]
   Int_t           trkhitindexc[42][2][1000];   //[ntracks_reco]
   Int_t           trkpid[42][2];   //[ntracks_reco]
   Int_t           trkpidndf[42][2];   //[ntracks_reco]
   Double_t        trkpidchi2[42][2];   //[ntracks_reco]
   Double_t        trkpida[42][2];   //[ntracks_reco]
   Double_t        trkmissinge[42][2];   //[ntracks_reco]
   Double_t        trkmissingeavg[42][2];   //[ntracks_reco]
   Int_t           trktruepdgu[42];   //[ntracks_reco]
   Double_t        trktrueeffu[42];   //[ntracks_reco]
   Double_t        trktruepuru[42];   //[ntracks_reco]
   Int_t           trktruepdgv[42];   //[ntracks_reco]
   Double_t        trktrueeffv[42];   //[ntracks_reco]
   Double_t        trktruepurv[42];   //[ntracks_reco]
   Int_t           nmatched_reco;
   Double_t        trk_mom_minos;
   Double_t        trk_charge_minos;
   Double_t        trk_dcosx_minos;
   Double_t        trk_dcosy_minos;
   Double_t        trk_dcosz_minos;
   Double_t        trk_vtxx_minos;
   Double_t        trk_vtxy_minos;
   Double_t        trk_vtxz_minos;
   Int_t           trk_index_minos;
   Float_t         mc_index_minos;
   Double_t        mc_pdg_minos;
   Double_t        mc_px_minos;
   Double_t        mc_py_minos;
   Double_t        mc_pz_minos;
   Double_t        mc_ene_minos;
   Double_t        mc_mass_minos;
   Double_t        mc_vtxx_minos;
   Double_t        mc_vtxy_minos;
   Double_t        mc_vtxz_minos;
   Int_t           trkcontained_minos;
   Int_t           test_charge_minos;
   Double_t        rdiff_minos;
   Double_t        xdiff_minos;
   Double_t        ydiff_minos;
   Double_t        thetadiff_minos;
   Int_t           parpdg;
   Double_t        parmom;
   Int_t           nuPDG_truth;
   Int_t           ccnc_truth;
   Int_t           mode_truth;
   Double_t        enu_truth;
   Double_t        Q2_truth;
   Int_t           hitnuc_truth;
   Double_t        W_truth;
   Double_t        nuvtxx_truth;
   Double_t        nuvtxy_truth;
   Double_t        nuvtxz_truth;
   Double_t        nu_dcosx_truth;
   Double_t        nu_dcosy_truth;
   Double_t        nu_dcosz_truth;
   Double_t        lep_mom_truth;
   Double_t        lep_dcosx_truth;
   Double_t        lep_dcosy_truth;
   Double_t        lep_dcosz_truth;
   Int_t           mcevts_truth;
   Double_t        beamwgt;
   Double_t        tpx_flux;
   Double_t        tpy_flux;
   Double_t        tpz_flux;
   Int_t           tptype_flux;
   Int_t           no_dead_wires_muon;
   Int_t           no_hits;
   Int_t           hit_plane[6295];   //[no_hits]
   Int_t           hit_wire[6295];   //[no_hits]
   Int_t           hit_channel[6295];   //[no_hits]
   Double_t        hit_peakT[6295];   //[no_hits]
   Double_t        hit_charge[6295];   //[no_hits]
   Double_t        hit_ph[6295];   //[no_hits]
   Int_t           hit_trkid[6295];   //[no_hits]
   Double_t        hit_etruth[6295];   //[no_hits]
   Double_t        twodvtx_w_reco[2];
   Double_t        twodvtx_t_reco[2];
   Double_t        twodvtx_w_truth[2];
   Double_t        twodvtx_t_truth[2];
   Int_t           nkingaclustersu_reco;
   Int_t           nkingaclustersv_reco;
   Int_t           nvertexkingaclustersu_reco;
   Int_t           nvertexkingaclustersv_reco;
   Int_t           nlinemergerclustersu_reco;
   Int_t           nlinemergerclustersv_reco;
   Int_t           nvertexlinemergerclustersu_reco;
   Int_t           nvertexlinemergerclustersv_reco;
   Int_t           ndbscanclustersu_reco;
   Int_t           ndbscanclustersv_reco;
   Int_t           nvertexdbscanclustersu_reco;
   Int_t           nvertexdbscanclustersv_reco;
   Int_t           no_kingaclusters;
   Int_t           kingaclusters_planeNo[1];   //[no_kingaclusters]
   Double_t        Start_pt_w_kingaCl[1];   //[no_kingaclusters]
   Double_t        Start_pt_t_kingaCl[1];   //[no_kingaclusters]
   Int_t           no_linemergerclusters;
   Int_t           linemergerclusters_planeNo[1];   //[no_linemergerclusters]
   Double_t        Start_pt_w_linemergerCl[1];   //[no_linemergerclusters]
   Double_t        Start_pt_t_linemergerCl[1];   //[no_linemergerclusters]
   Int_t           no_fuzzyclusters;
   Int_t           fNhitsClust;
   Double_t        two_trackstart_dcosx_reco[2];
   Double_t        two_trackstart_dcosy_reco[2];
   Double_t        two_trackstart_dcosz_reco[2];
   Double_t        two_trackexit_dcosx_reco[2];
   Double_t        two_trackexit_dcosy_reco[2];
   Double_t        two_trackexit_dcosz_reco[2];
   Int_t           no_primaries;
   Int_t           geant_list_size;
   Int_t           pdg[2498];   //[geant_list_size]
   Double_t        Eng[2498];   //[geant_list_size]
   Double_t        Px[2498];   //[geant_list_size]
   Double_t        Py[2498];   //[geant_list_size]
   Double_t        Pz[2498];   //[geant_list_size]
   Double_t        StartPointx[2498];   //[geant_list_size]
   Double_t        StartPointy[2498];   //[geant_list_size]
   Double_t        StartPointz[2498];   //[geant_list_size]
   Double_t        EndPointx[2498];   //[geant_list_size]
   Double_t        EndPointy[2498];   //[geant_list_size]
   Double_t        EndPointz[2498];   //[geant_list_size]
   Int_t           NumberDaughters[2498];   //[geant_list_size]
   Int_t           Mother[2498];   //[geant_list_size]
   Int_t           TrackId[2498];   //[geant_list_size]
   Int_t           process_primary[2498];   //[geant_list_size]
   Double_t        Kin_Eng_reco[42];   //[ntracks_reco]
   Double_t        muon_Kin_Eng_reco;
   Int_t           genie_no_primaries;
   Int_t           genie_primaries_pdg[106];   //[genie_no_primaries]
   Double_t        genie_Eng[106];   //[genie_no_primaries]
   Double_t        genie_Px[106];   //[genie_no_primaries]
   Double_t        genie_Py[106];   //[genie_no_primaries]
   Double_t        genie_Pz[106];   //[genie_no_primaries]
   Double_t        genie_P[106];   //[genie_no_primaries]
   Int_t           genie_status_code[106];   //[genie_no_primaries]
   Double_t        genie_mass[106];   //[genie_no_primaries]
   Int_t           genie_trackID[106];   //[genie_no_primaries]
   Int_t           genie_ND[106];   //[genie_no_primaries]
   Int_t           genie_mother[106];   //[genie_no_primaries]
   Double_t        pmttime;
   Int_t           pmt1[4];
   Int_t           pmt2[4];
   Int_t           pmt3[4];
   Int_t           pmt4[4];

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_subrun;   //!
   TBranch        *b_event;   //!
   TBranch        *b_evttime;   //!
   TBranch        *b_beamtime;   //!
   TBranch        *b_pot;   //!
   TBranch        *b_isdata;   //!
   TBranch        *b_taulife;   //!
   TBranch        *b_vtxx_reco;   //!
   TBranch        *b_vtxy_reco;   //!
   TBranch        *b_vtxz_reco;   //!
   TBranch        *b_nclusu_reco;   //!
   TBranch        *b_nclusv_reco;   //!
   TBranch        *b_nclusw_reco;   //!
   TBranch        *b_ntracks_reco;   //!
   TBranch        *b_nvertextracks_reco;   //!
   TBranch        *b_nvertexclustersu_reco;   //!
   TBranch        *b_nvertexclustersv_reco;   //!
   TBranch        *b_nvertexclustersw_reco;   //!
   TBranch        *b_ntrackendonboundary_reco;   //!
   TBranch        *b_nearestx;   //!
   TBranch        *b_nearesty;   //!
   TBranch        *b_nearestz;   //!
   TBranch        *b_trackstart_dcosx_reco;   //!
   TBranch        *b_trackstart_dcosy_reco;   //!
   TBranch        *b_trackstart_dcosz_reco;   //!
   TBranch        *b_trackexit_dcosx_reco;   //!
   TBranch        *b_trackexit_dcosy_reco;   //!
   TBranch        *b_trackexit_dcosz_reco;   //!
   TBranch        *b_trackstart_x_reco;   //!
   TBranch        *b_trackstart_y_reco;   //!
   TBranch        *b_trackstart_z_reco;   //!
   TBranch        *b_trackexit_x_reco;   //!
   TBranch        *b_trackexit_y_reco;   //!
   TBranch        *b_trackexit_z_reco;   //!
   TBranch        *b_enu_reco;   //!
   TBranch        *b_nclupertrack_reco;   //!
   TBranch        *b_trkvtxx;   //!
   TBranch        *b_trkvtxy;   //!
   TBranch        *b_trkvtxz;   //!
   TBranch        *b_trkendx;   //!
   TBranch        *b_trkendy;   //!
   TBranch        *b_trkendz;   //!
   TBranch        *b_trkstartdcosx;   //!
   TBranch        *b_trkstartdcosy;   //!
   TBranch        *b_trkstartdcosz;   //!
   TBranch        *b_trkenddcosx;   //!
   TBranch        *b_trkenddcosy;   //!
   TBranch        *b_trkenddcosz;   //!
   TBranch        *b_trkke;   //!
   TBranch        *b_trkrange;   //!
   TBranch        *b_trkpitchc;   //!
   TBranch        *b_nmaxtrkhits;   //!
   TBranch        *b_ntrkhits;   //!
   TBranch        *b_trkdedx;   //!
   TBranch        *b_trkdqdx;   //!
   TBranch        *b_trkresrg;   //!
   TBranch        *b_trkhitindexc;   //!
   TBranch        *b_trkpid;   //!
   TBranch        *b_trkpidndf;   //!
   TBranch        *b_trkpidchi2;   //!
   TBranch        *b_trkpida;   //!
   TBranch        *b_trkmissinge;   //!
   TBranch        *b_trkmissingeavg;   //!
   TBranch        *b_trktruepdgu;   //!
   TBranch        *b_trktrueeffu;   //!
   TBranch        *b_trktruepuru;   //!
   TBranch        *b_trktruepdgv;   //!
   TBranch        *b_trktrueeffv;   //!
   TBranch        *b_trktruepurv;   //!
   TBranch        *b_nmatched_reco;   //!
   TBranch        *b_trk_mom_minos;   //!
   TBranch        *b_trk_charge_minos;   //!
   TBranch        *b_trk_dcosx_minos;   //!
   TBranch        *b_trk_dcosy_minos;   //!
   TBranch        *b_trk_dcosz_minos;   //!
   TBranch        *b_trk_vtxx_minos;   //!
   TBranch        *b_trk_vtxy_minos;   //!
   TBranch        *b_trk_vtxz_minos;   //!
   TBranch        *b_trk_index_minos;   //!
   TBranch        *b_mc_index_minos;   //!
   TBranch        *b_mc_pdg_minos;   //!
   TBranch        *b_mc_px_minos;   //!
   TBranch        *b_mc_py_minos;   //!
   TBranch        *b_mc_pz_minos;   //!
   TBranch        *b_mc_ene_minos;   //!
   TBranch        *b_mc_mass_minos;   //!
   TBranch        *b_mc_vtxx_minos;   //!
   TBranch        *b_mc_vtxy_minos;   //!
   TBranch        *b_mc_vtxz_minos;   //!
   TBranch        *b_trkcontained_minos;   //!
   TBranch        *b_test_charge_minos;   //!
   TBranch        *b_rdiff_minos;   //!
   TBranch        *b_xdiff_minos;   //!
   TBranch        *b_ydiff_minos;   //!
   TBranch        *b_thetadiff_minos;   //!
   TBranch        *b_parpdg;   //!
   TBranch        *b_parmom;   //!
   TBranch        *b_nuPDG_truth;   //!
   TBranch        *b_ccnc_truth;   //!
   TBranch        *b_mode_truth;   //!
   TBranch        *b_enu_truth;   //!
   TBranch        *b_Q2_truth;   //!
   TBranch        *b_hitnuc_truth;   //!
   TBranch        *b_W_truth;   //!
   TBranch        *b_nuvtxx_truth;   //!
   TBranch        *b_nuvtxy_truth;   //!
   TBranch        *b_nuvtxz_truth;   //!
   TBranch        *b_nu_dcosx_truth;   //!
   TBranch        *b_nu_dcosy_truth;   //!
   TBranch        *b_nu_dcosz_truth;   //!
   TBranch        *b_lep_mom_truth;   //!
   TBranch        *b_lep_dcosx_truth;   //!
   TBranch        *b_lep_dcosy_truth;   //!
   TBranch        *b_lep_dcosz_truth;   //!
   TBranch        *b_mcevts_truth;   //!
   TBranch        *b_beamwgt;   //!
   TBranch        *b_tpx_flux;   //!
   TBranch        *b_tpy_flux;   //!
   TBranch        *b_tpz_flux;   //!
   TBranch        *b_tptype_flux;   //!
   TBranch        *b_no_dead_wires_muon;   //!
   TBranch        *b_no_hits;   //!
   TBranch        *b_hit_plane;   //!
   TBranch        *b_hit_wire;   //!
   TBranch        *b_hit_channel;   //!
   TBranch        *b_hit_peakT;   //!
   TBranch        *b_hit_charge;   //!
   TBranch        *b_hit_ph;   //!
   TBranch        *b_hit_trkid;   //!
   TBranch        *b_hit_etruth;   //!
   TBranch        *b_twodvtx_w_reco;   //!
   TBranch        *b_twodvtx_t_reco;   //!
   TBranch        *b_twodvtx_w_truth;   //!
   TBranch        *b_twodvtx_t_truth;   //!
   TBranch        *b_nkingaclustersu_reco;   //!
   TBranch        *b_nkingaclustersv_reco;   //!
   TBranch        *b_nvertexkingaclustersu_reco;   //!
   TBranch        *b_nvertexkingaclustersv_reco;   //!
   TBranch        *b_nlinemergerclustersu_reco;   //!
   TBranch        *b_nlinemergerclustersv_reco;   //!
   TBranch        *b_nvertexlinemergerclustersu_reco;   //!
   TBranch        *b_nvertexlinemergerclustersv_reco;   //!
   TBranch        *b_ndbscanclustersu_reco;   //!
   TBranch        *b_ndbscanclustersv_reco;   //!
   TBranch        *b_nvertexdbscanclustersu_reco;   //!
   TBranch        *b_nvertexdbscanclustersv_reco;   //!
   TBranch        *b_no_kingaclusters;   //!
   TBranch        *b_kingaclusters_planeNo;   //!
   TBranch        *b_Start_pt_w_kingaCl;   //!
   TBranch        *b_Start_pt_t_kingaCl;   //!
   TBranch        *b_no_linemergerclusters;   //!
   TBranch        *b_linemergerclusters_planeNo;   //!
   TBranch        *b_Start_pt_w_linemergerCl;   //!
   TBranch        *b_Start_pt_t_linemergerCl;   //!
   TBranch        *b_no_fuzzyclusters;   //!
   TBranch        *b_fNhitsClust;   //!
   TBranch        *b_two_trackstart_dcosx_reco;   //!
   TBranch        *b_two_trackstart_dcosy_reco;   //!
   TBranch        *b_two_trackstart_dcosz_reco;   //!
   TBranch        *b_two_trackexit_dcosx_reco;   //!
   TBranch        *b_two_trackexit_dcosy_reco;   //!
   TBranch        *b_two_trackexit_dcosz_reco;   //!
   TBranch        *b_no_primaries;   //!
   TBranch        *b_geant_list_size;   //!
   TBranch        *b_pdg;   //!
   TBranch        *b_Eng;   //!
   TBranch        *b_Px;   //!
   TBranch        *b_Py;   //!
   TBranch        *b_Pz;   //!
   TBranch        *b_StartPointx;   //!
   TBranch        *b_StartPointy;   //!
   TBranch        *b_StartPointz;   //!
   TBranch        *b_EndPointx;   //!
   TBranch        *b_EndPointy;   //!
   TBranch        *b_EndPointz;   //!
   TBranch        *b_NumberDaughters;   //!
   TBranch        *b_Mother;   //!
   TBranch        *b_TrackId;   //!
   TBranch        *b_process_primary;   //!
   TBranch        *b_Kin_Eng_reco;   //!
   TBranch        *b_muon_Kin_Eng_reco;   //!
   TBranch        *b_genie_no_primaries;   //!
   TBranch        *b_genie_primaries_pdg;   //!
   TBranch        *b_genie_Eng;   //!
   TBranch        *b_genie_Px;   //!
   TBranch        *b_genie_Py;   //!
   TBranch        *b_genie_Pz;   //!
   TBranch        *b_genie_P;   //!
   TBranch        *b_genie_status_code;   //!
   TBranch        *b_genie_mass;   //!
   TBranch        *b_genie_trackID;   //!
   TBranch        *b_genie_ND;   //!
   TBranch        *b_genie_mother;   //!
   TBranch        *b_pmttime;   //!
   TBranch        *b_pmt1;   //!
   TBranch        *b_pmt2;   //!
   TBranch        *b_pmt3;   //!
   TBranch        *b_pmt4;   //!

   argoana(TTree *tree=0);
   virtual ~argoana();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   bool    is_matched_Loop(int run,int subrun,int event,int &mcpdg,int &recopdg);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
  };
}
#endif
