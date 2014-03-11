#ifndef LARPROPERTIES_CC
#define LARPROPERTIES_CC

#include "LArProperties.hh"

namespace larutil {

  LArProperties* LArProperties::_me = 0;

  LArProperties::LArProperties(std::string file_name, std::string tree_name) : LArUtilBase(file_name,tree_name)
  {
    if(_file_name.empty())
      _file_name = Form("%s/LArUtil/dat/%s",getenv("LARLIGHT_CORE_DIR"),kFILENAME_LARPROPERTIES.c_str());
    if(_tree_name.empty())
      _tree_name = kTREENAME_LARPROPERTIES;
    _name = "LArProperties";
    LoadData();
  }

  void LArProperties::SetBranchAddress()
  {

    if(!_data_tree) {
      throw LArUtilException(Form("DataTree has to be specified prior to %s function call",__FUNCTION__));
      return;
    }

    std::string error_msg("");
    if(!(_data_tree->GetBranch("fEfield")))           error_msg += "      fEfield\n";
    if(!(_data_tree->GetBranch("fTemperature")))      error_msg += "      fTemperature\n";
    if(!(_data_tree->GetBranch("fElectronlifetime"))) error_msg += "      fElectronlifetime\n";
    if(!(_data_tree->GetBranch("fRadiationLength")))  error_msg += "      fRadiationLength\n";
    if(!(_data_tree->GetBranch("fArgon39DecayRate"))) error_msg += "      fArgon39DecayRate\n";

    if(!(_data_tree->GetBranch("fZ")))     error_msg += "      fZ\n";
    if(!(_data_tree->GetBranch("fA")))     error_msg += "      fA\n";
    if(!(_data_tree->GetBranch("fI")))     error_msg += "      fI\n";
    if(!(_data_tree->GetBranch("fSa")))    error_msg += "      fSa\n";
    if(!(_data_tree->GetBranch("fSx0")))   error_msg += "      fSx0\n";
    if(!(_data_tree->GetBranch("fSx1")))   error_msg += "      fSx1\n";
    if(!(_data_tree->GetBranch("fScbar"))) error_msg += "      fScbar\n";

    if(!(_data_tree->GetBranch("fFastScintSpectrum"))) error_msg += "      fFastScintSpectrum\n";
    if(!(_data_tree->GetBranch("fFastScintEnergies"))) error_msg += "      fFastScintEnergies\n";
    if(!(_data_tree->GetBranch("fSlowScintSpectrum"))) error_msg += "      fSlowScintSpectrum\n";
    if(!(_data_tree->GetBranch("fSlowScintEnergies"))) error_msg += "      fSlowScintEnergies\n";
    if(!(_data_tree->GetBranch("fRIndexSpectrum")))    error_msg += "      fRIndexSpectrum\n";
    if(!(_data_tree->GetBranch("fRIndexEnergies")))    error_msg += "      fRIndexEnergies\n";
    if(!(_data_tree->GetBranch("fAbsLengthSpectrum"))) error_msg += "      fAbsLengthSpectrum\n";
    if(!(_data_tree->GetBranch("fAbsLengthEnergies"))) error_msg += "      fAbsLengthEnergies\n";
    if(!(_data_tree->GetBranch("fRayleighSpectrum")))  error_msg += "      fRayleighSpectrum\n";
    if(!(_data_tree->GetBranch("fRayleighEnergies")))  error_msg += "      fRayleighEnergies\n";

    if(!(_data_tree->GetBranch("fScintByParticleType"))) error_msg += "      fScintByParticleType\n";

    if(!(_data_tree->GetBranch("fProtonScintYield")))        error_msg += "      fProtonScintYield\n";
    if(!(_data_tree->GetBranch("fProtonScintYieldRatio")))   error_msg += "      fProtonScintYieldRatio\n";
    if(!(_data_tree->GetBranch("fMuonScintYield")))          error_msg += "      fMuonScintYield\n";
    if(!(_data_tree->GetBranch("fMuonScintYieldRatio")))     error_msg += "      fMuonScintYieldRatio\n";
    if(!(_data_tree->GetBranch("fPionScintYield")))          error_msg += "      fPionScintYield\n";
    if(!(_data_tree->GetBranch("fPionScintYieldRatio")))     error_msg += "      fPionScintYieldRatio\n";
    if(!(_data_tree->GetBranch("fKaonScintYield")))          error_msg += "      fKaonScintYield\n";
    if(!(_data_tree->GetBranch("fKaonScintYieldRatio")))     error_msg += "      fKaonScintYieldRatio\n";
    if(!(_data_tree->GetBranch("fElectronScintYield")))      error_msg += "      fElectronScintYield\n";
    if(!(_data_tree->GetBranch("fElectronScintYieldRatio"))) error_msg += "      fElectronScintYieldRatio\n";
    if(!(_data_tree->GetBranch("fAlphaScintYield")))         error_msg += "      fAlphaScintYield\n";
    if(!(_data_tree->GetBranch("fAlphaScintYieldRatio")))    error_msg += "      fAlphaScintYieldRatio\n";

    if(!(_data_tree->GetBranch("fScintYield")))           error_msg += "      fScintYield\n";
    if(!(_data_tree->GetBranch("fScintResolutionScale"))) error_msg += "      fScintResolutionScale\n";
    if(!(_data_tree->GetBranch("fScintFastTimeConst")))   error_msg += "      fScintFastTimeConst\n";
    if(!(_data_tree->GetBranch("fScintSlowTimeConst")))   error_msg += "      fScintSlowTimeConst\n";
    if(!(_data_tree->GetBranch("fScintYieldRatio")))      error_msg += "      fScintYieldRatio\n";
    if(!(_data_tree->GetBranch("fScintBirksConstant")))   error_msg += "      fScintBirksConstant\n";

    if(!(_data_tree->GetBranch("fEnableCerenkovLight")))  error_msg += "      fEnableCerenkovLight\n";

    if(!(_data_tree->GetBranch("fReflectiveSurfaceNames")))
      error_msg += "      fReflectiveSurfaceNames\n";
    if(!(_data_tree->GetBranch("fReflectiveSurfaceEnergies")))
      error_msg += "      fReflectiveSurfaceEnergies\n";
    if(!(_data_tree->GetBranch("fReflectiveSurfaceReflectances")))
      error_msg += "      fReflectiveSurfaceReflectances\n";
    if(!(_data_tree->GetBranch("fReflectiveSurfaceDiffuseFractions")))
      error_msg += "      fReflectiveSurfaceDiffuseFractions\n";

    if(!error_msg.empty()) {

      throw LArUtilException(Form("Missing following TBranches...\n%s",error_msg.c_str()));

      return;
    }

    _data_tree->SetBranchAddress("fEfield",&fEfield);
    _data_tree->SetBranchAddress("fTemperature",&fTemperature);
    _data_tree->SetBranchAddress("fElectronlifetime",&fElectronlifetime);
    _data_tree->SetBranchAddress("fRadiationLength",&fRadiationLength);
    _data_tree->SetBranchAddress("fArgon39DecayRate",&fArgon39DecayRate);

    _data_tree->SetBranchAddress("fZ",&fZ);
    _data_tree->SetBranchAddress("fA",&fA);
    _data_tree->SetBranchAddress("fI",&fI);
    _data_tree->SetBranchAddress("fSa",&fSa);
    _data_tree->SetBranchAddress("fSk",&fSk);
    _data_tree->SetBranchAddress("fSx0",&fSx0);
    _data_tree->SetBranchAddress("fSx1",&fSx1);
    _data_tree->SetBranchAddress("fScbar",&fScbar);
    
    _data_tree->SetBranchAddress("fFastScintSpectrum",&fFastScintSpectrum);
    _data_tree->SetBranchAddress("fFastScintEnergies",&fFastScintEnergies);
    _data_tree->SetBranchAddress("fSlowScintSpectrum",&fSlowScintSpectrum);
    _data_tree->SetBranchAddress("fSlowScintEnergies",&fSlowScintEnergies);
    _data_tree->SetBranchAddress("fRIndexSpectrum",&fRIndexSpectrum);
    _data_tree->SetBranchAddress("fRIndexEnergies",&fRIndexEnergies);
    _data_tree->SetBranchAddress("fAbsLengthSpectrum",&fAbsLengthSpectrum);
    _data_tree->SetBranchAddress("fAbsLengthEnergies",&fAbsLengthEnergies);
    _data_tree->SetBranchAddress("fRayleighSpectrum",&fRayleighSpectrum);
    _data_tree->SetBranchAddress("fRayleighEnergies",&fRayleighEnergies);

    _data_tree->SetBranchAddress("fScintByParticleType",&fScintByParticleType);

    _data_tree->SetBranchAddress("fProtonScintYield", &fProtonScintYield);
    _data_tree->SetBranchAddress("fProtonScintYieldRatio", &fProtonScintYieldRatio);
    _data_tree->SetBranchAddress("fMuonScintYield", &fMuonScintYield);
    _data_tree->SetBranchAddress("fMuonScintYieldRatio", &fMuonScintYieldRatio);
    _data_tree->SetBranchAddress("fPionScintYield", &fPionScintYield);
    _data_tree->SetBranchAddress("fPionScintYieldRatio", &fPionScintYieldRatio);
    _data_tree->SetBranchAddress("fKaonScintYield", &fKaonScintYield);
    _data_tree->SetBranchAddress("fKaonScintYieldRatio", &fKaonScintYieldRatio);
    _data_tree->SetBranchAddress("fElectronScintYield", &fElectronScintYield);
    _data_tree->SetBranchAddress("fElectronScintYieldRatio", &fElectronScintYieldRatio);
    _data_tree->SetBranchAddress("fAlphaScintYield", &fAlphaScintYield);
    _data_tree->SetBranchAddress("fAlphaScintYieldRatio", &fAlphaScintYieldRatio);
    
    _data_tree->SetBranchAddress("fScintYield", &fScintYield);
    _data_tree->SetBranchAddress("fScintResolutionScale", &fScintResolutionScale);
    _data_tree->SetBranchAddress("fScintFastTimeConst", &fScintFastTimeConst);
    _data_tree->SetBranchAddress("fScintSlowTimeConst", &fScintSlowTimeConst);
    _data_tree->SetBranchAddress("fScintYieldRatio", &fScintYieldRatio);
    _data_tree->SetBranchAddress("fScintBirksConstant", &fScintBirksConstant);
    
    _data_tree->SetBranchAddress("fEnableCerenkovLight", &fEnableCerenkovLight);

    _data_tree->SetBranchAddress("fReflectiveSurfaceNames", &fReflectiveSurfaceNames);
    _data_tree->SetBranchAddress("fReflectiveSurfaceEnergies", &fReflectiveSurfaceEnergies);
    _data_tree->SetBranchAddress("fReflectiveSurfaceReflectances", &fReflectiveSurfaceReflectances);
    _data_tree->SetBranchAddress("fReflectiveSurfaceDiffuseFractions", &fReflectiveSurfaceDiffuseFractions);


  }

  Double_t LArProperties::Density(Double_t temperature) const
  {
    // Default temperature use internal value.
    if(temperature == 0.)
      temperature = Temperature();
    
    Double_t density = -0.00615*temperature + 1.928;
    
    return density;
  }

  Double_t LArProperties::Efield(UInt_t planegap) const
  {
    if(planegap >= fEfield->size())
      throw LArUtilException("requesting Electric field in a plane gap that is not defined");
    
    return fEfield->at(planegap);
  }

  //------------------------------------------------------------------------------------//
  Double_t LArProperties::DriftVelocity(Double_t efield, Double_t temperature) const {
    
    // Drift Velocity as a function of Electric Field and LAr Temperature
    // from : W. Walkowiak, NIM A 449 (2000) 288-294
    //
    // Efield should have units of kV/cm
    // Temperature should have units of Kelvin
    
    // Default Efield, use internal value.
    if(efield == 0.)
      efield = Efield();
    //
    if(efield > 4.0) {

      std::ostringstream msg;
      msg <<"DriftVelocity Warning! : E-field value of "
	  << efield
	  << " kV/cm is outside of range covered by drift"
	  << " velocity parameterization. Returned value"
	  << " may not be correct";
      print(larlight::MSG::WARNING,__FUNCTION__,msg.str());
    }    
    
    // Default temperature use internal value.
    if(temperature == 0.)
      temperature = Temperature();
    
    if(temperature < 87.0 || temperature > 94.0) {
      std::ostringstream msg;
      msg << "DriftVelocity Warning! : Temperature value of "
	  << temperature
	  << " K is outside of range covered by drift velocity"
	  << " parameterization. Returned value may not be"
	  << " correct";
      print(larlight::MSG::WARNING,__FUNCTION__,msg.str());
    }    
    
    Double_t tshift = -87.203+temperature;
    Double_t xFit = 0.0938163-0.0052563*tshift-0.0001470*tshift*tshift;
    Double_t uFit = 5.18406+0.01448*tshift-0.003497*tshift*tshift-0.000516*tshift*tshift*tshift;
    Double_t vd;
    
    
    // Icarus Parameter Set, use as default
    Double_t  P1 = -0.04640; // K^-1
    Double_t  P2 = 0.01712;  // K^-1
    Double_t  P3 = 1.88125;   // (kV/cm)^-1
    Double_t  P4 =  0.99408;    // kV/cm
    Double_t  P5 =  0.01172;   // (kV/cm)^-P6
    Double_t  P6 =  4.20214;
    Double_t  T0 =  105.749;  // K
    // Walkowiak Parameter Set
    Double_t    P1W = -0.01481; // K^-1
    Double_t  P2W = -0.0075;  // K^-1
    Double_t   P3W =  0.141;   // (kV/cm)^-1
    Double_t   P4W =  12.4;    // kV/cm
    Double_t   P5W =  1.627;   // (kV/cm)^-P6
    Double_t   P6W =  0.317;
    Double_t   T0W =  90.371;  // K
    
    // From Craig Thorne . . . currently not documented
    // smooth transition from linear at small fields to 
    //     icarus fit at most fields to Walkowiak at very high fields
    if (efield < xFit) vd=efield*uFit;
    else if (efield<0.619) { 
      vd = ((P1*(temperature-T0)+1)
	    *(P3*efield*std::log(1+P4/efield) + P5*std::pow(efield,P6))
	    +P2*(temperature-T0));
    }
    else if (efield<0.699) {
      vd = 12.5*(efield-0.619)*((P1W*(temperature-T0W)+1)
				*(P3W*efield*std::log(1+P4W/efield) + P5W*std::pow(efield,P6W))
				+P2W*(temperature-T0W))+
	12.5*(0.699-efield)*((P1*(temperature-T0)+1)
			     *(P3*efield*std::log(1+P4/efield) + P5*std::pow(efield,P6))
			     +P2*(temperature-T0));
    }
    else {
      vd = ((P1W*(temperature-T0W)+1)
	    *(P3W*efield*std::log(1+P4W/efield) + P5W*std::pow(efield,P6W))
	    +P2W*(temperature-T0W));     
    }
    
    vd /= 10.;

    return vd; // in cm/us
  }

  //----------------------------------------------------------------------------------
  // The below function assumes that the user has applied the lifetime correction and
  // effective pitch between the wires (usually after 3D reconstruction). Using with
  // mean wire pitch will not give correct results.
  // parameters:
  //  dQdX in electrons/cm, charge (amplitude or integral obtained) divided by effective pitch for a given 3D track.
  // returns dEdX in MeV/cm
  Double_t LArProperties::BirksCorrection(Double_t dQdx) const
  {
    // Correction for charge quenching using parameterization from
    // S.Amoruso et al., NIM A 523 (2004) 275
    
    Double_t  A3t    = kRecombA;
    Double_t  K3t    = kRecombk;                     // in KV/cm*(g/cm^2)/MeV
    Double_t  rho    = this->Density();                    // LAr density in g/cm^3
    Double_t Wion    = 1000./kGeVToElectrons;        // 23.6 eV = 1e, Wion in MeV/e
    Double_t Efield  = this->Efield();                     // Electric Field in the drift region in KV/cm
    K3t           /= rho;                                // KV/MeV
    Double_t dEdx    = dQdx/(A3t/Wion-K3t/Efield*dQdx);    //MeV/cm
    
    return dEdx;
  }
  
  // Modified Box model correction 
  Double_t LArProperties::ModBoxCorrection(Double_t dQdx) const
  {
    // Modified Box model correction has better behavior than the Birks
    // correction at high values of dQ/dx.
    Double_t  rho    = this->Density();                    // LAr density in g/cm^3
    Double_t Wion    = 1000./kGeVToElectrons;        // 23.6 eV = 1e, Wion in MeV/e
    Double_t Efield  = this->Efield();                     // Electric Field in the drift region in KV/cm
    Double_t Beta    = kModBoxB / (rho * Efield);
    Double_t Alpha   = kModBoxA;
    Double_t dEdx = (exp(Beta * Wion * dQdx ) - Alpha) / Beta;
    
    return dEdx;
    
  }

  //----------------------------------------------------------------------------------
  // Restricted mean energy loss (dE/dx) in units of MeV/cm.
  //
  // For unrestricted mean energy loss, set tcut = 0, or tcut large.
  //
  // Arguments:
  //
  // mom  - Momentum of incident particle in GeV/c.
  // mass - Mass of incident particle in GeV/c^2.
  // tcut - Maximum kinetic energy of delta rays (MeV).
  //
  // Returned value is positive.
  //
  // Based on Bethe-Bloch formula as contained in particle data book.
  // Material parameters (stored in larproperties.fcl) are taken from
  // pdg web site http://pdg.lbl.gov/AtomicNuclearProperties/
  //
  Double_t LArProperties::Eloss(Double_t mom, Double_t mass, Double_t tcut) const
  {
    // Some constants.
    
    Double_t K = 0.307075;     // 4 pi N_A r_e^2 m_e c^2 (MeV cm^2/mol).
    Double_t me = 0.510998918; // Electron mass (MeV/c^2).
    
    // Calculate kinematic quantities.
    
    Double_t bg = mom / mass;           // beta*gamma.
    Double_t gamma = sqrt(1. + bg*bg);  // gamma.
    Double_t beta = bg / gamma;         // beta (velocity).
    Double_t mer = 0.001 * me / mass;   // electron mass / mass of incident particle.
    Double_t tmax = 2.*me* bg*bg / (1. + 2.*gamma*mer + mer*mer);  // Maximum delta ray energy (MeV).
    
    // Make sure tcut does not exceed tmax.
    
    if(tcut == 0. || tcut > tmax)
      tcut = tmax;
    
    // Calculate density effect correction (delta).
    
    Double_t x = std::log10(bg);
    Double_t delta = 0.;
    if(x >= fSx0) {
      delta = 2. * std::log(10.) * x - fScbar;
      if(x < fSx1)
	delta += fSa * std::pow(fSx1 - x, fSk);
    }
    
    // Calculate stopping number.
    
    Double_t B = 0.5 * std::log(2.*me*bg*bg*tcut / (1.e-12 * fI*fI))
      - 0.5 * beta*beta * (1. + tcut / tmax) - 0.5 * delta;
    
    // Don't let the stopping number become negative.
    
    if(B < 1.)
      B = 1.;
    
    // Calculate dE/dx.
    
    Double_t dedx = Density() * K*fZ*B / (fA * beta*beta);
    
    // Done.
    
    return dedx;
  }
  
  //----------------------------------------------------------------------------------
  // Energy loss fluctuation (sigma_E^2 / length in MeV^2/cm).
  //
  // Arguments:
  //
  // mom  - Momentum of incident particle in GeV/c.
  //
  // Based on Bichsel formula referred to but not given in pdg.
  //
  Double_t LArProperties::ElossVar(Double_t mom, Double_t mass) const
  {
    // Some constants.
    
    Double_t K = 0.307075;     // 4 pi N_A r_e^2 m_e c^2 (MeV cm^2/mol).
    Double_t me = 0.510998918; // Electron mass (MeV/c^2).
    
    // Calculate kinematic quantities.
    
    Double_t bg = mom / mass;          // beta*gamma.
    Double_t gamma2 = 1. + bg*bg;      // gamma^2.
    Double_t beta2 = bg*bg / gamma2;   // beta^2.
    
    // Calculate final result.
    
    Double_t result = gamma2 * (1. - 0.5 * beta2) * me * (fZ / fA) * K * Density();
    return result;
  }

  //---------------------------------------------------------------------------------
  std::map<Double_t,Double_t> LArProperties::FastScintSpectrum() const
  {
    if(fFastScintSpectrum->size()!=fFastScintEnergies->size()){
      std::ostringstream msg;
      msg << "The vectors specifying the fast scintillation spectrum are "
	  << " different sizes - " << fFastScintSpectrum->size()
	  << " " << fFastScintEnergies->size();
      throw LArUtilException(msg.str());
    }
    
    std::map<Double_t, Double_t> ToReturn;
    for(size_t i=0; i!=fFastScintSpectrum->size(); ++i)
      ToReturn[fFastScintEnergies->at(i)]=fFastScintSpectrum->at(i);
    
    return ToReturn;
  }

  //---------------------------------------------------------------------------------
  std::map<Double_t, Double_t> LArProperties::SlowScintSpectrum() const
  {
    if(fSlowScintSpectrum->size()!=fSlowScintEnergies->size()){
      std::ostringstream msg;
      msg << "The vectors specifying the slow scintillation spectrum are "
	  << " different sizes - " << fFastScintSpectrum->size()
	  << " " << fFastScintEnergies->size();
      throw LArUtilException(msg.str());
    }
    
    std::map<Double_t, Double_t> ToReturn;
    for(size_t i=0; i!=fSlowScintSpectrum->size(); ++i)
      ToReturn[fSlowScintEnergies->at(i)]=fSlowScintSpectrum->at(i);
    
    return ToReturn;
  }
  
  //---------------------------------------------------------------------------------
  std::map<Double_t, Double_t> LArProperties::RIndexSpectrum() const
  {
    if(fRIndexSpectrum->size()!=fRIndexEnergies->size()){
      std::ostringstream msg;
      msg << "The vectors specifying the RIndex spectrum are "
	  << " different sizes - " << fRIndexSpectrum->size()
	  << " " << fRIndexEnergies->size();
      throw LArUtilException(msg.str());
    }
    
    std::map<Double_t, Double_t> ToReturn;
    for(size_t i=0; i!=fRIndexSpectrum->size(); ++i)
      ToReturn[fRIndexEnergies->at(i)]=fRIndexSpectrum->at(i);
    
    return ToReturn;
  }


  //---------------------------------------------------------------------------------
  std::map<Double_t, Double_t> LArProperties::AbsLengthSpectrum() const
  {
    if(fAbsLengthSpectrum->size()!=fAbsLengthEnergies->size()){
      std::ostringstream msg;
      msg << "The vectors specifying the Abs Length spectrum are "
	  << " different sizes - " << fAbsLengthSpectrum->size()
	  << " " << fAbsLengthEnergies->size();
      throw LArUtilException(msg.str());
    }
    
    std::map<Double_t, Double_t> ToReturn;
    for(size_t i=0; i!=fAbsLengthSpectrum->size(); ++i)
      ToReturn[fAbsLengthEnergies->at(i)]=fAbsLengthSpectrum->at(i);
    
    return ToReturn;
  }

  //---------------------------------------------------------------------------------
  std::map<Double_t, Double_t> LArProperties::RayleighSpectrum() const
  {
    if(fRayleighSpectrum->size()!=fRayleighEnergies->size()){
      std::ostringstream msg;
      msg << "The vectors specifying the rayleigh spectrum are "
	  << " different sizes - " << fRayleighSpectrum->size()
	  << " " << fRayleighEnergies->size();
      throw LArUtilException(msg.str());
    }
    
    std::map<Double_t, Double_t> ToReturn;
    for(size_t i=0; i!=fRayleighSpectrum->size(); ++i)
      ToReturn[fRayleighEnergies->at(i)]=fRayleighSpectrum->at(i);
    
    return ToReturn;
  }

  //---------------------------------------------------------------------------------
  std::map<std::string, std::map<Double_t,Double_t> > LArProperties::SurfaceReflectances() const
  {
    std::map<std::string, std::map<Double_t, Double_t> > ToReturn;
    
    if(fReflectiveSurfaceNames->size()!=fReflectiveSurfaceReflectances->size()){
      std::ostringstream msg;
	msg << "The vectors specifying the surface reflectivities "
	    << "do not have consistent sizes";
	throw LArUtilException(msg.str());
    }
    for(size_t i=0; i!=fReflectiveSurfaceNames->size(); ++i){
      if(fReflectiveSurfaceEnergies->size()!=fReflectiveSurfaceReflectances->at(i).size()){
	std::ostringstream msg;
	msg << "The vectors specifying the surface reflectivities do not have consistent sizes";
	throw LArUtilException(msg.str());
      }
    }
    for(size_t iName=0; iName!=fReflectiveSurfaceNames->size(); ++iName)
      for(size_t iEnergy=0; iEnergy!=fReflectiveSurfaceEnergies->size(); ++iEnergy)
	ToReturn[fReflectiveSurfaceNames->at(iName)][fReflectiveSurfaceEnergies->at(iEnergy)]=fReflectiveSurfaceReflectances->at(iName)[iEnergy];
    
    return ToReturn;
    
  }
  
  //---------------------------------------------------------------------------------
  std::map<std::string, std::map<Double_t,Double_t> > LArProperties::SurfaceReflectanceDiffuseFractions() const
  {
    std::map<std::string, std::map<Double_t, Double_t> > ToReturn;
    
    if(fReflectiveSurfaceNames->size()!=fReflectiveSurfaceDiffuseFractions->size()){
      std::ostringstream msg;
      msg << "The vectors specifying the surface reflectivities do not have consistent sizes";
      LArUtilException(msg.str());
    }
    for(size_t i=0; i!=fReflectiveSurfaceNames->size(); ++i){
      if(fReflectiveSurfaceEnergies->size()!=fReflectiveSurfaceDiffuseFractions->at(i).size()){
	std::ostringstream msg;
	msg << "The vectors specifying the surface reflectivities do not have consistent sizes";
	throw LArUtilException(msg.str());
      }
    }
    for(size_t iName=0; iName!=fReflectiveSurfaceNames->size(); ++iName)
      for(size_t iEnergy=0; iEnergy!=fReflectiveSurfaceEnergies->size(); ++iEnergy)
	ToReturn[fReflectiveSurfaceNames->at(iName)][fReflectiveSurfaceEnergies->at(iEnergy)]=fReflectiveSurfaceDiffuseFractions->at(iName)[iEnergy];
    
    return ToReturn;
  }
  
}
#endif
