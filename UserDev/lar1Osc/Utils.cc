#include "Utils.hh"
#include "TFile.h"
#include "TH1F.h"
#include "TH1D.h"

#include <iostream>
#include <iomanip>
#include <string>

namespace lar1{
  Utils::Utils(){
    mc_generation = 5;
    randService.SetSeed(time(0));
    reconfigure();
  }

  void Utils::setMC_Generation(int g){
    mc_generation = g; 
    reconfigure();
    return;
  }

  void Utils::reconfigure(){
    std::cout <<"Reconfiguring Utils with mc_generation =" << mc_generation<<std::endl;

    PotNormNubar  = 10e20;
    PotNormNu     = 6.6e20;

    nue_fidCut_x = 25.0;
    nue_fidCut_y = 25.0;
    nue_fidCut_zUp = 5.0;
    nue_fidCut_zDown = 100.0;

    numu_fidCut_x = 15.0;
    numu_fidCut_y = 15.0;
    numu_fidCut_zUp = 15.0;
    numu_fidCut_zDown = 80.0;

    // fidCut_x = 17;
    // fidCut_y = 17;
    // fidCut_zUp = 17;
    // fidCut_zDown = 17;

    // For reference, the monte carlo for ND spans 7m in x and is centered at 130cm
    // These coordinates try to represent the actual beam center.
    // Bathtub option:
    // iDet = 0
    nd_xmin = -200.0;
    nd_xmax =  200.0;
    nd_ymin = -242.0;
    nd_ymax =  158.0;
    nd_zmin =  0.0;
    nd_zmax =  500.0;

    // iDet = ?
    nd_long_xmin = -200.0;
    nd_long_xmax =  200.0;
    nd_long_ymin = -242.0;
    nd_long_ymax =  158.0;
    nd_long_zmin =  0.0;
    nd_long_zmax =  2*365.0;

    // Other option, shorter

    // iDet = 1
    ub_xmin =  0.0;
    ub_xmax =  2*128.0;
    ub_ymin = -116.5;
    ub_ymax =  116.5;
    ub_zmin =  0.0;
    ub_zmax =  1036.0;
    // make FD 8.4m on a side to get to the MB equivalent FV (in size, not tonage)
    // fd_xmin =  -420.0;
    // fd_xmax =  420.0;
    // fd_ymin =  -420.0;
    // fd_ymax =  420.0;
    // fd_zmin =  0.0;
    // fd_zmax =  840.0;

    // Max x direction (active): -600 to 600  
    // Max x direction (active): -600 to 600
    // Max x direction (active): 0 to 1500

    // iDet = 3
    fd_xmin =  -450.0;
    fd_xmax =  450.0;
    fd_ymin =  -450.0;
    fd_ymax =  450.0;
    fd_zmin =  0.0;
    fd_zmax =  900.0;

    // icarus stuff;
    // icarus active volume is reported to be ~476t
    // I think this is about the closest I can come:
    // Length 15m, make a square in x and y at lengths 4.76 meters.
    // iDet = 5
    ic_xmin =  -350.0;
    ic_xmax =  350.0;
    ic_ymin =  -158.0;
    ic_ymax =  158.0;
    ic_zmin =  0.0;
    ic_zmax =  1795.0;

    // MiniBooNE shaped info:
    // iDet = 4
    kMB_center = TVector3(0.0, 0.0, 600.0);
    kMB_radius = 600;
    kMB_fidcut = 100;

    return;
  }

  Utils::~Utils(){
  }



  Double_t Utils::GetPOTNormNuMI( Int_t iflux, Int_t iLoc) const{

    Double_t POT_Sim = 1;
    Double_t POTnorm = 1;

    if (iflux == kNu || iflux == kNu_Fosc)
      POTnorm = 1.0e20;
    if (iflux == kNubar || iflux == kNubar_Fosc)
      POTnorm = 1.0e20;

   switch (iLoc)
    {
      case k100m:
        POT_Sim = -1;
        break;
      case k470m:
        if (mc_generation == 2) POT_Sim = 1.228e20;
          break;
      default:
        std::cout << "No detector location selected, or invalid selection.  Error.\n";
        return -999;
    }


 
    POTnorm /= POT_Sim;

    if (POTnorm > 1e-5) return POTnorm;
    else return 1.0;

   return 1;
  }

  Double_t Utils::GetPOTNorm( Int_t iflux, Int_t iLoc) const{

    // Simple function, really, but put into utils to abstract it out of reprocessing.
    // 
    // ideally, this should involve a database lookup.  But I'm going to hardcode it.
    // Putting in values of 1 right now but that'll update with the final POT Numbers:
    // 

    std::cout << "Iflux is : " << iflux << std::endl;

    Double_t POT_Sim = 1;
    Double_t POTnorm = 1;

    if (iflux == kNu || iflux == kNu_Fosc)
      POTnorm = 6.6e20;
    if (iflux == kNubar || iflux == kNubar_Fosc)
      POTnorm = 10e20;

    switch (iLoc)
    {
      case k100m:
        switch (iflux)
        {
          case kNu:
            if (mc_generation == 5) POT_Sim = 7.86E+19;
            if (mc_generation == 4) POT_Sim = 7.858E+19;
            if (mc_generation == 3) POT_Sim = 7.879E+19;
            if (mc_generation == 2) POT_Sim = 1.74e20;
            break;
          case kNu_Fosc:
            if (mc_generation == 5) POT_Sim = 7.637E+19;
            if (mc_generation == 4) POT_Sim = 7.637E+19;
            if (mc_generation == 3) POT_Sim = 7.637E+19;
            if (mc_generation == 2) POT_Sim = 1.79e20;
            break;
          case kNubar:
            if (mc_generation == 2) POT_Sim = 8.87e20;
          case kNubar_Fosc:
            if (mc_generation == 2) POT_Sim = 7.42e20;
          default:
            POT_Sim = -1;
            break;

        }
        break;
      case k150m:
        if (mc_generation != 3){
          std::cerr << "ERROR: the mc version selected is invalid for this detector\n.";
          exit(-1);
        }
        switch (iflux)
        {
          case kNu:
            POT_Sim = 1.536E+20;
            break;
          case kNu_Fosc:
            POT_Sim = 1.475E+20;
            break;
          case kNubar:
          case kNubar_Fosc:
          default:
            POT_Sim = -1;
            break;
        }
        break;
      case k175m:
        if (mc_generation != 3){
          std::cerr << "ERROR: the mc version selected is invalid for this detector\n.";
          exit(-1);
        }
        switch (iflux)
        {
          case kNu:
          case kNu_Fosc:
          case kNubar:
          case kNubar_Fosc:
          default:
            POT_Sim = -1;
            break;
        }
        break;
      case k200m:
        if (mc_generation != 3){
          std::cerr << "ERROR: the mc version selected is invalid for this detector\n.";
          exit(-1);
        }
        switch (iflux)
        {
          case kNu:
            POT_Sim = 2.591E+20;
            break;
          case kNu_Fosc:
            POT_Sim = 2.494E+20;
            break;
          case kNubar:
          case kNubar_Fosc:
          default:
            POT_Sim = -1;
            break;
        }
        break;
      case k470m:
        switch (iflux)
        {
          case kNu:
            if (mc_generation == 5) POT_Sim = 5.25E+21;
            if (mc_generation == 4) POT_Sim = 6.087E+21;
            if (mc_generation == 3) POT_Sim = 6.053E+21;
            if (mc_generation == 2) POT_Sim = 5.57E+21;
            break;
          case kNu_Fosc:
            if (mc_generation == 5) POT_Sim = 1.57E+21;
            if (mc_generation == 4) POT_Sim = 2.592E+21;
            if (mc_generation == 3) POT_Sim = 2.592E+21;
            if (mc_generation == 2) POT_Sim = 5.37E+21;
            break;
          case kNubar:
            if (mc_generation == 2) POT_Sim = 3.70E+22;
          case kNubar_Fosc:
            if (mc_generation == 2) POT_Sim = 3.58E+22;
          default:
            POT_Sim = -1;
            break;
        }
        break;
      case k600m_onaxis:
        if (mc_generation < 3){
          std::cerr << "ERROR: the mc version selected is invalid for this detector\n.";
          exit(-1);
        }
        switch (iflux)
        {
          case kNu:
            if (mc_generation == 5) POT_Sim = 5.42E+20;
            if (mc_generation == 4) POT_Sim = 5.4163E+20;
            if (mc_generation == 3) POT_Sim = 5.434E+20;
            break;
          case kNu_Fosc:
            if (mc_generation == 5) POT_Sim = 6.507E+20;
            if (mc_generation == 4) POT_Sim = 6.507E+20;
            if (mc_generation == 3) POT_Sim = 6.507E+20;
            // POT_Sim = 2.766e20;
            break;
          case kNubar:
          case kNubar_Fosc:
          default:
            POT_Sim = -1;
            break;
        }
        break;
      case k600m_offaxis:
        if (mc_generation != 3){
          std::cerr << "ERROR: the mc version selected is invalid for this detector\n.";
          exit(-1);
        }
        switch (iflux)
        {
          case kNu:
            POT_Sim = 2.386E+20;
            break;
          case kNu_Fosc:
            POT_Sim = 7.357E+20;
            break;
          case kNubar:
          case kNubar_Fosc:
          default:
            POT_Sim = -1;
            break;
        }
        break;
      default:
        std::cout << "No detector location selected, or invalid selection.  Error.\n";
        return -999;
    }


 
    POTnorm /= POT_Sim;

    if (POTnorm > 1e-5) return POTnorm;
    else return 1.0;

  }




  //=======================================================================================
  // Reweight the flux using histograms in FluxRW tools
  //=======================================================================================
  Double_t Utils::GetFluxWeight( Double_t energy, Int_t iflux, Int_t inno, Int_t ndecay ){

    Double_t wgt = 0;
    Int_t ntype = 0;
    Int_t ptype = 0;
    // string ntypestr[4] = { "nue", "nuebar", "numu", "numubar" };
    std::string ptypestr = "";

    // normal samples
    if( iflux == kNu || iflux == kNubar ){  
      if      (inno == 12)  ntype = 1;
      else if (inno == -12) ntype = 2;
      else if (inno == 14)  ntype = 3;
      else if (inno == -14) ntype = 4;
    }
    // full osc samples
    else if( iflux == kNu_Fosc || iflux == kNubar_Fosc ){  
      if      (inno == 12)  ntype = 3;
      else if (inno == -12) ntype = 4;
      else if (inno == 14)  ntype = 3;
      else if (inno == -14) ntype = 4;
    }

    // Determine ptype using that list described in NtupleReprocessing.C
    if (ndecay > 0 && ndecay < 5){ ptype = 3; ptypestr = "K0L"; }
    else if (ndecay > 4 && ndecay < 11){ ptype = 4; ptypestr = "K+"; }
    else if (ndecay == 11 || ndecay == 12){ ptype = 1; ptypestr = "muon"; }
    else if (ndecay == 13 || ndecay == 14){ ptype = 2; ptypestr = "pion"; }
    
    if( ntype == 0 || ptype == 0) {
      std::cout << "Wrong type neutrino or parent is present. Returning weight = 0." << std::endl;
      std::cout << "ntype = " << ntype << ",  ptype = " << ptype << std::endl;
      return 0;
    }

    // neutrino mode
    if( iflux == kNu || iflux == kNu_Fosc ) 
      wgt = fluxrw_nu.GetWeight( energy, ntype, ptype );
    // antineutrino mode
    else if( iflux == kNubar || iflux == kNubar_Fosc )
      wgt = fluxrw_nubar.GetWeight( energy, ntype, ptype );

    // if ( verbose ) std::cout << std::setprecision(3) << "neutrino = " << ntypestr[ntype-1] << " (" << energy << " GeV), parent = " 
   	    // << ptypestr << ", flux weight = " << wgt << std::endl;
    
    return wgt;

  }


  //=========================================================================================
  // Polar angle Theta - angle between momentum vector and z-axis
  //=========================================================================================
  Double_t Utils::GetTheta( const TVector3 & mom, const TVector3 & ref ) const{

    return GetTheta( mom.X(), mom.Y(), mom.Z(), ref );
    
  }

  Double_t Utils::GetTheta( Double_t px, Double_t py, Double_t pz, const TVector3 & ref ) const{

    // a.b = |a| |b| cos(theta)
    Double_t cosTheta = (ref.X()*px + ref.Y()*py + ref.Z()*pz) / (sqrt(px*px + py*py + pz*pz) * ref.Mag());
    Double_t theta = TMath::ACos(cosTheta);
    return theta;

  }

  //=================================================================================
  // Polar angle Phi - angle in the x-y plane, x-axis = 0
  //=================================================================================
  Double_t Utils::GetPhi( TVector3 mom, TVector3 ref ) const{

    mom.RotateUz(ref.Unit());
    return GetPhi( mom.X(), mom.Y() );
    
  }

  Double_t Utils::GetPhi( Double_t px, Double_t py ) const{

    return atan2( py, px );

  }

  //=================================================================================
  // CCQE neutrino energy
  //=================================================================================
  Double_t Utils::NuEnergyCCQE( Double_t lep_energy, Double_t lep_p, Double_t lep_theta,
                                Double_t mass, Int_t mode, bool verbose ) const
  {
    
    Double_t M_n = 939.565;    // MeV/c2
    Double_t M_p = 938.272;    // MeV/c2
    Double_t bindingE = 30.0;  // MeV

    if ( verbose ) std::cout << "NuEnergyCCQE w/ binding E = " << bindingE << " and mass = " << mass << std::endl;
    if ( verbose ) std::cout << "flux mode = " << mode << std::endl;
    if ( verbose ) std::cout << "lep_energy = " << lep_energy << "; lep_p = " << lep_p << "; lep_theta = " << lep_theta << std::endl;
    
    Double_t nu_energy = -1000;
    
    if( mode == 0 || mode == 1 ) {
      Double_t nu_energy_num = pow(M_n,2) - pow(M_p - bindingE,2)
        - pow(mass,2) + 2.0*(M_p - bindingE)*lep_energy;
      Double_t nu_energy_den = 2.0*(M_p - bindingE - lep_energy + lep_p*cos(lep_theta));
      if ( verbose ) std::cout << " Nu E numerator   = " << nu_energy_num << std::endl;
      if ( verbose )std::cout << " Nu E denominator = " << nu_energy_den << std::endl;
      if( nu_energy_den ) nu_energy = nu_energy_num / nu_energy_den;
   
    } else if ( mode == 2 || mode == 3 ) {
      Double_t nu_energy_num = pow(M_p,2) - pow(M_n - bindingE,2)
        - pow(mass,2) + 2.0*(M_n - bindingE)*lep_energy;
      Double_t nu_energy_den = 2.0*(M_n - bindingE - lep_energy + lep_p*cos(lep_theta));
      if ( verbose ) std::cout << " Nu E numerator   = " << nu_energy_num << std::endl;
      if ( verbose ) std::cout << " Nu E denominator = " << nu_energy_den << std::endl;
      if( nu_energy_den ) nu_energy = nu_energy_num / nu_energy_den;
    }
    if ( verbose ) std::cout << " Nu Energy = " << nu_energy << " MeV" << std::endl;

    if( nu_energy >= 0 && nu_energy < 10000 ) 
      return nu_energy;
    else
      return -1000;
  }



  Double_t Utils::NuEnergyCalo(std::vector<Int_t> *pdg, 
                               std::vector<TLorentzVector> *momentum,
                               double lepEnergy,
                               bool smearing,
                               Bool_t include_neutrons, Bool_t include_pizeros,
                               Double_t prot_thresh, Double_t pion_thresh, 
                               Double_t kaon_thresh,
                               bool verbose ) 
  {

    if ( verbose ) std::cout << "Determining calorimetric energy" << std::endl;

    Double_t M_n = .939565;    // GeV/c2

    // Take the lepton energy
    Double_t total_energy = lepEnergy;

    // Add in any charged vertex activity
    // total_energy += VertexEnergy(pdg, momentum, smearing, 
    //                              prot_thresh, pion_thresh,kaon_thresh,
    //                              verbose);

    // Now, if we want to include neutrons or pions, get them:
    for( unsigned int i = 0; i < pdg->size(); ++i ){
     
      //std::cout << " part: " << pdg->at(i) << ", energy: " << momentum->at(i).E() << std::endl;

      // First check if particles are visible and over threshold
      // Then, assign a smeared energy.

      if( abs(pdg->at(i)) > 5000 ) continue;                           // ignore nuclear fragments
      if( abs(pdg->at(i)) == 12 || abs(pdg->at(i)) == 14 ) continue;   // ignore neutrinos
      if( abs(pdg->at(i)) == 11 || abs(pdg->at(i)) == 13 ) continue;   // ignore leptons

      if( !include_neutrons && abs(pdg->at(i)) == 2112 ) continue;                // neutrons
      if( !include_pizeros  && abs(pdg->at(i)) == 111  ) continue;                // pizeros
      if( abs(pdg->at(i)) == 2212 ) continue;  // skip protons because they're accounted for
      if( abs(pdg->at(i)) == 211 ) continue;  // skip pions because they're accounted for
      if( abs(pdg->at(i)) == 321 ) continue;  // skip kaons because they're accounted for

      total_energy += momentum->at(i).E();
      if (abs(pdg->at(i)) == 2112) total_energy -= M_n;

    }

    if ( verbose ) std::cout << " Total Energy = " << total_energy << " GeV" << std::endl;
    
    return total_energy;
    
  }

  Double_t Utils::GetLeptonEnergy(Double_t energy,
                                  bool smearing,
                                  int PDG, bool contained,
                                  double containedLength) 
  {
    double res;

    if (abs(PDG) == 13 ){ // muon
      if (contained){
        res = 0.02;
        // std::cout << "  Resolution is: " << res << std::endl;
        return randService.Gaus(energy, res*energy);
      }
      else{
        res = -0.102*log(0.000612*containedLength);
        // std::cout << "  Resolution is: " << res << std::endl;
        return randService.Gaus(energy, 5*res*energy);
      }
    }
    else if (abs(PDG) == 11){ // electron
      // electrons have 15%sqrt(e/1Gev) resolution.  Maybe.
      res = 0.15 / sqrt(energy);
      // std::cout << "  Resolution is: " << res << std::endl;
      return randService.Gaus(energy, res*energy);
    }
    else
      return 0.0;
  }

  //=================================================================================
  // Get the visible energy near the interaction vertex
  //=================================================================================
  Double_t Utils::VertexEnergy( std::vector<Int_t> *pdg, 
                                std::vector<TLorentzVector> *momentum,
                                bool smearing,
                                Double_t prot_thresh, 
                                Double_t pion_thresh,
                                Double_t kaon_thresh,
                                bool verbose  ) {

    if ( verbose ) std::cout << "Determining hadronic kinetic energy near vertex" << std::endl;

    Double_t M_p  = .938272;    // GeV/c2
    Double_t M_pi = .139570;    // GeV/c2
    Double_t M_k = .493667;    // GeV/c2
    // Double_t M_n = .939565;    // GeV/c2

    Double_t total_energy = 0;
    Double_t HadronicResolution = 0.05;
    if (!smearing) HadronicResolution = 0.00;
    for( unsigned int i = 0; i < pdg->size(); ++i ){
     
      if ( verbose ) std::cout << " part: " << pdg->at(i) << ", energy: " << momentum->at(i).E() << std::endl;
      
      //if( abs(pdg->at(i)) > 5000 ) continue;                           // ignore nuclear fragments
      //if( abs(pdg->at(i)) == 12 || abs(pdg->at(i)) == 14 ) continue;   // ignore neutrinos
      //if( abs(pdg->at(i)) == 2112 ) continue;                          // neutrons
      //if( abs(pdg->at(i)) == 111  ) continue;                          // pizeros
     
      if( abs(pdg->at(i)) == 2212 && momentum->at(i).E()-M_p  > prot_thresh ){  // proton threshold
        total_energy 
          += randService.Gaus(momentum->at(i).E() - M_p, 
                              HadronicResolution*(momentum->at(i).E() - M_p));
      }
      if( abs(pdg->at(i)) == 211  && momentum->at(i).E()-M_pi > pion_thresh ){  // pion threshold
        total_energy 
          += randService.Gaus(momentum->at(i).E() - M_pi, 
                              HadronicResolution*(momentum->at(i).E() ));      
      }
      if( abs(pdg->at(i)) == 321  && momentum->at(i).E()-M_k > kaon_thresh ){  // pion threshold
        total_energy 
          += randService.Gaus(momentum->at(i).E(), 
                              HadronicResolution*(momentum->at(i).E() ));
      }

    }
    
    if ( verbose ) std::cout << " Total Hadronic Kinetic Energy = " << total_energy << " GeV" << std::endl;
    
    return total_energy;
    
  }


  Double_t Utils::TotalPhotonEnergy(Int_t idet, 
                                    std::vector<TLorentzVector> *p1pos,
                                    std::vector<TLorentzVector> *p1mom,
                                    std::vector<TLorentzVector> *p2pos,
                                    std::vector<TLorentzVector> *p2mom,
                                    std::vector<TLorentzVector> *miscpos,
                                    std::vector<TLorentzVector> *miscmom) const{
    
    if(   p1pos->size()   != p1mom->size() 
       || p2pos->size()   != p2mom->size() 
       || miscpos->size() != miscmom->size() ){
      std::cout << "photon vectors don't match!!" << std::endl;
      exit(1);
    }
    Double_t energy = 0.0;

    for( unsigned int i = 0; i < p1pos->size(); i++ ){
      TVector3 photon1Pos((*p1pos).at(i).X(), (*p1pos).at(i).Y(), (*p1pos).at(i).Z() );
      if( IsFiducial( idet, photon1Pos ) )
        energy += p1mom->at(i).E();

    }

    for( unsigned int i = 0; i < p2pos->size(); i++ ){

      TVector3 photon2Pos((*p2pos).at(i).X(), (*p2pos).at(i).Y(), (*p2pos).at(i).Z() );
      if( IsFiducial( idet, photon2Pos ) )
        energy += p2mom->at(i).E();

    }

    for( unsigned int i = 0; i < miscpos->size(); i++ ){

      TVector3 photon2Pos((*miscpos).at(i).X(), (*miscpos).at(i).Y(), (*miscpos).at(i).Z() );
      if( IsFiducial( idet, photon2Pos ) )
        energy += miscmom->at(i).E();

    }

    return energy;


  }

  //==========================================================================
  // Check if point is in some fiducial volume definition
  //==========================================================================
  void Utils::GetDetBoundary(Int_t idet, 
                             Double_t &xmin, Double_t &xmax, 
  			                     Double_t &ymin, Double_t &ymax,
                             Double_t &zmin, Double_t &zmax ) const{
    
    // LAr1-ND
    if( idet == kND){
      xmin = nd_xmin;
      xmax = nd_xmax;
      ymin = nd_ymin;
      ymax = nd_ymax;
      zmin = nd_zmin;
      zmax = nd_zmax;
    }
    // Uboone
    else if( idet == kUB ){
      xmin = ub_xmin;
      xmax = ub_xmax;
      ymin = ub_ymin;
      ymax = ub_ymax;
      zmin = ub_zmin;
      zmax = ub_zmax;
    }
    // LAr1-FD
    // else if( idet == kFD ){
    //   xmin = fd_xmin;
    //   xmax = fd_xmax;
    //   ymin = fd_ymin;
    //   ymax = fd_ymax;
    //   zmin = fd_zmin;
    //   zmax = fd_zmax;
    // }
    else if (idet == kMB){
      xmin = kMB_radius;
      xmax = 0;
      ymin = 0;
      ymax = 0;
      zmin = 0;
      zmax = 0;
    }
    else if (idet == kIC){
      xmin = ic_xmin;
      xmax = ic_xmax;
      ymin = ic_ymin;
      ymax = ic_ymax;
      zmin = ic_zmin;
      zmax = ic_zmax;
    }
    else if( idet == kND_long ){
      xmin = nd_long_xmin;
      xmax = nd_long_xmax;
      ymin = nd_long_ymin;
      ymax = nd_long_ymax;
      zmin = nd_long_zmin;
      zmax = nd_long_zmax;
    }
    else{
      std::cout << "Don't recognize idet" << std::endl;
      exit(1);
    }
    return;

  }

  //=========================================================================================
  // Check if point is in some fiducial volume definition
  //==========================================================================================
  Bool_t Utils::IsFiducial( Int_t idet, const TVector3 & vtx, std::string signal) const{
    

    Double_t xmin(0.0), xmax(0.0), ymin(0.0), ymax(0.0), zmin(0.0), zmax(0.0);

    GetDetBoundary( idet, xmin, xmax, ymin, ymax, zmin, zmax );

    double fidCut_x = 0;

    // fidCut_x = fidCut;
    // fidCut_y = fidCut;
    // fidCut_z = fidCut;
    if (signal == "numu"){
      xmin += numu_fidCut_x;
      xmax -= numu_fidCut_x;
      ymin += numu_fidCut_y;
      ymax -= numu_fidCut_y;
      zmin += numu_fidCut_zUp; 
      zmax -= numu_fidCut_zDown;
      fidCut_x = numu_fidCut_x;
    }
    else if (signal == "nue"){
      xmin += nue_fidCut_x;
      xmax -= nue_fidCut_x;
      ymin += nue_fidCut_y;
      ymax -= nue_fidCut_y;
      zmin += nue_fidCut_zUp; 
      zmax -= nue_fidCut_zDown;
      fidCut_x = nue_fidCut_x;
    }
    else{
      xmin += 17;
      xmax -= 17;
      ymin += 17;
      ymax -= 17;
      zmin += 17; 
      zmax -= 17;
    }


    if( idet == kIC ){  // Cut out volume around each side of the center of x:
       
      // gets the center.  If the cuts above on x aren't symmetric, this is wrong.
      Double_t xcenter = (ic_xmax + ic_xmin)/2; 
      if ( vtx.X() < (xcenter + 50 + fidCut_x) && vtx.X() > (xcenter - 50 -fidCut_x) ) return false; //cut around the center APA

    }

    if( vtx.X() > xmin && vtx.X() < xmax && vtx.Y() > ymin && vtx.Y() < ymax && vtx.Z() > zmin && vtx.Z() < zmax )
      return true;
    else
      return false;

  }

  Bool_t Utils::IsFiducialMB(Int_t idet, TVector3 vtx, double fidCut) const
  {
    if (idet != kMB) return false;

    // Get distance from center for this point:
    TVector3 dist = vtx - kMB_center;
    if (dist.Mag() < kMB_radius - kMB_fidcut) return true;
    else return false;
  }

  //=========================================================================================
  // Check if point is in some active volume definition
  //==========================================================================================
  Bool_t Utils::IsActive( Int_t idet, TVector3 vtx, double cut ) const{

    if (idet == kMB) return IsFiducialMB(idet,vtx,0.0);
    
    Double_t xmin(0.0), xmax(0.0), ymin(0.0), ymax(0.0), zmin(0.0), zmax(0.0);

    GetDetBoundary( idet, xmin, xmax, ymin, ymax, zmin, zmax );
    
    if( idet == kIC ){  // Cut out volume around each side of the center of x:   
      // gets the center.  If the cuts above on x aren't symmetric, this is wrong.
      Double_t xcenter = (ic_xmax + ic_xmin)/2; 
      //cut around the center APA
      if ( vtx.X() < (xcenter + 50 + cut) && 
           vtx.X() > (xcenter - 50 - cut) ) return false; 
    }
    if( vtx.X() > xmin+cut && vtx.X() < xmax-cut && 
        vtx.Y() > ymin+cut && vtx.Y() < ymax-cut && 
        vtx.Z() > zmin+cut && vtx.Z() < zmax-cut )
      return true;
    else
      return false;

  }

  //=========================================================================================
  // Calculate fiducial mass
  //=========================================================================================
  Double_t Utils::GetFidMass( Int_t idet, std::string signal ) const{
    
    Double_t xmin(0.0), xmax(0.0), ymin(0.0), ymax(0.0), zmin(0.0), zmax(0.0);

    GetDetBoundary( idet, xmin, xmax, ymin, ymax, zmin, zmax );

    if (idet == kMB) return 1.4*3.14159*(4.0/3.0)*pow((xmin-kMB_fidcut)/100.0,3);

    double fidCut_x = 0;
    if (signal == "numu"){
      xmin += numu_fidCut_x;
      xmax -= numu_fidCut_x;
      ymin += numu_fidCut_y;
      ymax -= numu_fidCut_y;
      zmin += numu_fidCut_zUp; 
      zmax -= numu_fidCut_zDown;
      fidCut_x = numu_fidCut_x;
    }
    else if (signal == "nue"){
      xmin += nue_fidCut_x;
      xmax -= nue_fidCut_x;
      ymin += nue_fidCut_y;
      ymax -= nue_fidCut_y;
      zmin += nue_fidCut_zUp; 
      zmax -= nue_fidCut_zDown;
      fidCut_x = nue_fidCut_x;
    }
    else{
      xmin += 17;
      xmax -= 17;
      ymin += 17;
      ymax -= 17;
      zmin += 17; 
      zmax -= 17;
    }
    Double_t rho = 1.4;  // g/cm^3 = tons/m^3

    Double_t total_vol = (xmax-xmin)*(ymax-ymin)*(zmax-zmin)/pow(100.0,3) * rho;
    if (idet == kIC) 
      total_vol = (xmax-xmin-100-2*fidCut_x)*(ymax-ymin)*(zmax-zmin)/pow(100.0,3) * rho;
    // if( idet == kND ){  // Cut out volume around each side of the center of x:
    //   Double_t xcenter = (nd_xmax + nd_xmin)/2;  // gets the center.  If the cuts above on x aren't symmetric, this is wrong.
    //   total_vol -= ((xcenter + fidCut_x) - (xcenter - fidCut_x))*(ymax-ymin)*(zmax-zmin)/pow(100.0,3) * rho;
    // }

    return total_vol;

  }


  //=========================================================================================
  // Calculate fiducial mass
  //=========================================================================================
  Double_t Utils::GetActiveMass( Int_t idet ) const{
    
    Double_t xmin(0.0), xmax(0.0), ymin(0.0), ymax(0.0), zmin(0.0), zmax(0.0);

    GetDetBoundary( idet, xmin, xmax, ymin, ymax, zmin, zmax );
    if (idet == kMB) return 1.4*3.14159*(4.0/3.0)*pow(xmin/100.0,3);

    Double_t rho = 1.4;  // g/cm^3 = tons/m^3

    double total_mass = (xmax-xmin)*(ymax-ymin)*(zmax-zmin)/pow(100.0,3) * rho;
    if (idet == kIC) 
      total_mass = (xmax-xmin-100)*(ymax-ymin)*(zmax-zmin)/pow(100.0,3) * rho;
    return total_mass;

  }

  // 
  //Find out if line intersects the plane
  // 
  Bool_t Utils::IntersectsPlane(const TVector3 & startPos, const TVector3 & startDir,
                                const TVector3 & planeCorner1,
                                const TVector3 & planeCorner2,
                                const TVector3 & planeCorner3,
                                const bool verbose) const{
                                // ) const{

    if (startDir.Mag() == 0) return false;

    // a and b are defined to be vectors parallel to the plane:
    TVector3 a = planeCorner2 - planeCorner1;
    TVector3 b = planeCorner3 - planeCorner1;

    // Normal vector to the plane
    TVector3 n = a.Cross(b);

    //Check that the Normal vector isn't 0 - that the vectors aren't parallel
    if (n.Mag() == 0){
      if (verbose) std::cout << "Error, input vectors to define the plane are parallel." << std::endl;
      return false;
    }


    // Any point on the plane can be parametrized with the following equation:
    // p = planeCorner1 + u*a + v*b
    // Any point on the line can be parametrized with:
    // l = startPos + t*startDir
    // If this line intersects this plane, then for some values of (u,v,t) p = l
    // startPos + t*startDir = planeCorner1 + u*a + v*b
    // or
    // startPos - planeCorner1 = (-startDir, a, b) . (t, u, v) (Dot product)
    // (startDir, a, b) is a matrix with each column equal to the said vector.  It's 3x3 matrix

    // Invert the matrix, so we can solve for t, u, v directly.  Only singular if the line is parallel
    // to the plane.  In which case we can return false.
    // 
    // Check that the line is not parallel to the plane with dot product to normal vector:
    if (abs(n.Dot(startDir)/(n.Mag()*startDir.Mag())) < 1e-10){
      if (verbose) std::cout << "Direction is parallel to the plane, exiting." << std::endl;
      return false;
    }
    
    // Create the matrix:
    TMatrixD mat(3,3);
    // format is mat(row_n, col_n)
    mat(0,0) = -startDir.X();
    mat(0,1) = a.X();
    mat(0,2) = b.X();
    mat(1,0) = -startDir.Y();
    mat(1,1) = a.Y();
    mat(1,2) = b.Y();
    mat(2,0) = -startDir.Z();
    mat(2,1) = a.Z();
    mat(2,2) = b.Z();

    TMatrix inv = mat.Invert();


    // Create the vector to multiply.
    TMatrix t(3,1);
    t(0,0) = startPos.X() - planeCorner1.X();
    t(1,0) = startPos.Y() - planeCorner1.Y();
    t(2,0) = startPos.Z() - planeCorner1.Z();

    // Do the matrix multiplication:
    TMatrix result = inv*t;

    // t is  largely irrelevant in this function, but u and v are important.
    // if u and v are both within [0,1] then the line intersects the plane in the desired range

    if (verbose) std::cout << "\tt, u, v are "<< result(0,0) << ", " << result(1,0) << ", " << result(2,0) << std::endl;

    if (result(0,0) > 0){ // Intersects in forward direction
      if (result(1,0) >= 0 && result (1,0) <= 1)
      {
        if (result(2,0) >= 0 && result (2,0) <= 1)
        {
          return true;
        }
      }
      // then success! they intersect
    }
    return false;

  }

  double  Utils::GetContainedLength(const TVector3 & startPoint, 
                                    const TVector3 & startDir, 
                                    int idet,
                                    double precision) const{

    if (!IsActive(idet, startPoint)){
      std::cout << "Failed the IsActive cut!\n";
      return 0;
    }
    TVector3 unitDir = startDir.Unit();

    double alpha = 10;
    double alpha_prev = 0;
    bool finished = false;

    while (!finished){

      // std::cout << "Current pos is (" 
      //           << (startPoint + unitDir*alpha).X() << ", "
      //           << (startPoint + unitDir*alpha).Y() << ", "
      //           << (startPoint + unitDir*alpha).Z() << ")\n";

      // done when the steps are less than precision:
      if (fabs(alpha - alpha_prev) < precision){
        finished = true;
        break;
      }

      if (alpha > alpha_prev){
        // if the point at alpha is active, take another step of the same length
        if (IsActive(idet, startPoint + unitDir*alpha)){
          double step = alpha - alpha_prev;
          alpha_prev = alpha;
          alpha += step;
          continue;
        }
        // if it's not active, step backwards half a step
        else{
          double step = 0.5*(alpha - alpha_prev);
          alpha_prev = alpha;
          alpha -= step;
          continue;
        }
      }
      else{
        // this means that we've gone too far and are stepping backwards.
        // if the point at alpha is not active, take another step of the same length
        if (!IsActive(idet, startPoint + unitDir*alpha)){
          double step = alpha_prev - alpha;
          alpha_prev = alpha;
          alpha -= step;
          continue;
        }
        else{
          // else we're moving backwards but have crossed the boundary again.
          double step = 0.5*(alpha_prev - alpha);
          alpha_prev = alpha;
          alpha += step;
          continue;
        }
      }

    }

    return alpha;

  }


  double  Utils::GetLengthToStart(const TVector3 & startPoint, 
                                  const TVector3 & startDir, 
                                  int idet)  const{

    if (!IsActive(idet, startPoint)){
      // std::cout << "Failed the IsActive cut!\n";
      return 0;
    }
    return GetContainedLength(startPoint,-startDir,idet);

  }
  
  
  double  Utils::GetYZLengthToStart(const TVector3 & startPoint,
                                    const TVector3 & startDir,
                                    int idet) const{

    if (!IsActive(idet, startPoint)){
      // std::cout << "Failed the IsActive cut!\n";
      return 0;
    }
    
    TVector3 newdir=startDir;
    newdir[0]=0;
    return GetContainedLength(startPoint,-newdir,idet);

  }
  
  
  // bool PhotonsAreParallel(TVector3 & photon1_start, TVector3 & photon1_mom
  //                         TVector3 & photon2_start, TVector3 & photon2_mom)
  // {
  //   // Want to know if the conversion point
  //   // of either photons is on axis of the other
    
  // }


  //###########################################################################################

}


