/**
 * \file DataScanner_module.cc
 *
 * \ingroup DataScanner
 * 
 * \brief Class definition file of DataScanner
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataScanner

@{*/
#ifndef DataScanner_H
#define DataScanner_H

// LArSoft includes
#include "Geometry/Geometry.h"
#include "RecoBase/Wire.h"
#include "RecoBase/Hit.h"
#include "RecoBase/Track.h"
#include "RecoBase/Cluster.h"
#include "RecoBase/SpacePoint.h"
#include "SimulationBase/MCTruth.h"
#include "OpticalDetectorData/FIFOChannel.h"
#include "OpticalDetectorData/OpticalTypes.h"

// ART includes.
#include "art/Framework/Core/EDAnalyzer.h"

// LArLight includes
#include <SimpleTree/Base/Base-TypeDef.hh>
#include <SimpleTree/DataFormat/DataFormat-TypeDef.hh>

namespace larlight {

  /**
     \class DataScanner
     DataScanner module to copy LArSoft data contents into LArLight data formatted file
  */ 
  class DataScanner : public art::EDAnalyzer{

  public:

    /// Constructor
    DataScanner(const fhicl::ParameterSet&);

    /// Destructor
    virtual ~DataScanner(){};

    /// Function to be called before an event loop
    void beginJob();

    /// Function to be called per event
    void analyze (const art::Event&); 

  private:
    /// Function to check if the given MC trajectory point is in the readout FV or not
    bool IsFV(Double_t x, Double_t y, Double_t z, Double_t t) const;

    /// Function to calculate the distance from the closest wall
    Double_t GetDistanceToWall(Double_t x, Double_t y, Double_t z) const;

    /// Function to read & store calibrated wire data
    //void ReadWire(const art::Event& evt, const std::string mod_name, event_wire* data_ptr);

    /// Function to read & store reconstructed hit data
    void ReadHit(const art::Event& evt, const std::string mod_name, hit* data_ptr);

    /// Function to read & store reconstructed hit data
    void ReadCluster(const art::Event& evt, const std::string mod_name, cluster* data_ptr);

    /// Function to read & store spacepoints
    //void ReadPMT(const art::Event& evt, const std::string mod_name, event_pmt* data_ptr);

    /// Function to read & store spacepoints
    void ReadSPS(const art::Event& evt, const std::string mod_name, sps* data_ptr);

    /// Function to read & store Tracking information
    void ReadTrack(const art::Event& evt, const std::string mod_name, track* data_ptr);

    /// Function to read & store MCTruth information
    void ReadMCTruth(const art::Event& evt, const std::string mod_name, mctruth* data_ptr);

    /// Function to read & store MCNeutrino information
    void ReadMCNeutrino(const art::Event& evt, 
			const std::string nu_mod_name, const std::string flux_mod_name,
			mcnu* data_ptr);

    /// Function to read & store MCParticle array information
    void ReadMCPartArray(const art::Event& evt, const std::string mod_name, mcstep* step_ptr, mcpart* part_ptr);

    /// Utility function to parse module name string
    void ParseModuleName(std::vector<std::string> &mod_names, std::string name);

    TTree* _tree; ///< output data holder TTree
    std::vector<std::vector<std::string> > _mod_names; ///< input data production module names input from FCL file
    std::vector<data_base*>                _data_ptr;  ///< output data holder class object pointers
    std::string                            _flux_mod;  ///< Module name for flux file
    Double_t _x_max; ///< Maximum X boundary of the detector
    Double_t _y_max; ///< Maximum X boundary of the detector
    Double_t _z_max; ///< Maximum X boundary of the detector
    Double_t _x_min; ///< Maximum X boundary of the detector
    Double_t _y_min; ///< Maximum X boundary of the detector
    Double_t _z_min; ///< Maximum X boundary of the detector
    Double_t _readout_startT; ///< Time at which readout window starts in G4 clock
    Double_t _readout_endT;   ///< Time at which readout window ends in G4 clock 
    Double_t _readout_freq;    ///< TPC sampling frequency in MHz
    Double_t _readout_size; ///< Readout window size in readout time thick
  };

} 

#endif//  DataScanner_H

// DataScanner.cc

// Framework includes
#include "art/Framework/Core/ModuleMacros.h"

namespace larlight {
  DEFINE_ART_MODULE(DataScanner);
}


// Framework includes
#include "art/Framework/Principal/Event.h"
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Persistency/Common/Ptr.h"
#include "art/Persistency/Common/PtrVector.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "art/Framework/Services/Optional/TFileService.h"
#include "art/Framework/Services/Optional/TFileDirectory.h"
#include "messagefacility/MessageLogger/MessageLogger.h"


namespace larlight {

  //#######################################################################################################
  DataScanner::DataScanner(fhicl::ParameterSet const& pset) : _data_ptr(DATA::DATA_TYPE_MAX,0),
							      _flux_mod("")
  //#######################################################################################################
  {

    // Initialize module name container
    for(size_t i=0; i<(size_t)(DATA::DATA_TYPE_MAX); i++)
      
      _mod_names.push_back(std::vector<std::string>());

    // Obtain module names for input data
    // If a user set an empty string for these params, they are ignored for processing.
    ParseModuleName ( _mod_names[DATA::Bezier],           pset.get<std::string>( "fModName_Bezier"           ));
    ParseModuleName ( _mod_names[DATA::Kalman3DSPS],      pset.get<std::string>( "fModName_Kalman3DSPS"      ));
    ParseModuleName ( _mod_names[DATA::Kalman3DHit],      pset.get<std::string>( "fModName_Kalman3DHit"      ));
    ParseModuleName ( _mod_names[DATA::GENIE_MCNeutrino], pset.get<std::string>( "fModName_GENIE_MCNeutrino" ));
    ParseModuleName ( _mod_names[DATA::GENIE_MCTruth],    pset.get<std::string>( "fModName_GENIE_MCTruth"    ));
    ParseModuleName ( _mod_names[DATA::CRY_MCTruth],      pset.get<std::string>( "fModName_CRY_MCTruth"      ));
    ParseModuleName ( _mod_names[DATA::SpacePoint],       pset.get<std::string>( "fModName_SpacePoint"       ));
    ParseModuleName ( _mod_names[DATA::FIFOChannel],      pset.get<std::string>( "fModName_FIFOChannel"      ));
    ParseModuleName ( _mod_names[DATA::Wire],             pset.get<std::string>( "fModName_CalData"          ));
    ParseModuleName ( _mod_names[DATA::CrawlerHit],       pset.get<std::string>( "fModName_CrawlerHit"       ));
    ParseModuleName ( _mod_names[DATA::GausHit],          pset.get<std::string>( "fModName_GausHit"          ));
    ParseModuleName ( _mod_names[DATA::APAHit],           pset.get<std::string>( "fModName_APAHit"           ));
    ParseModuleName ( _mod_names[DATA::FFTHit],           pset.get<std::string>( "fModName_FFTHit"           ));
    ParseModuleName ( _mod_names[DATA::RFFHit],           pset.get<std::string>( "fModName_RFFHit"           ));
    ParseModuleName ( _mod_names[DATA::CrawlerCluster],   pset.get<std::string>( "fModName_CrawlerCluster"   ));
    ParseModuleName ( _mod_names[DATA::DBCluster],        pset.get<std::string>( "fModName_DBCluster"        ));
    ParseModuleName ( _mod_names[DATA::FuzzyCluster],     pset.get<std::string>( "fModName_FuzzyCluster"     ));
    ParseModuleName ( _mod_names[DATA::HoughCluster],     pset.get<std::string>( "fModName_HoughCluster"     ));
    ParseModuleName ( _mod_names[DATA::MCParticle],       pset.get<std::string>( "fModName_MCParticle"       ));
    _flux_mod       = pset.get<std::string>("fModName_MCFlux");
    _readout_startT = pset.get<Double_t>("fReadoutStartT");
    _readout_endT   = pset.get<Double_t>("fReadoutEndT");
    _readout_freq   = pset.get<Double_t>("fReadoutFreq");
    _readout_size   = pset.get<Double_t>("fReadoutSize");

    // Set volume boundary parameters
    art::ServiceHandle<geo::Geometry> geo;
    _y_max = geo->DetHalfHeight();
    _y_min = (-1.) * _y_max;
    _z_min = 0;
    _z_max = geo->DetLength();
    _x_min = 0;
    _x_max = 2.*(geo->DetHalfWidth());

    // Next we make storage data class objects for those data types specified in fcl files.
    art::ServiceHandle<art::TFileService>  fileService;

    for(size_t i=0; i<(int)(DATA::DATA_TYPE_MAX); i++){

      DATA::DATA_TYPE type = (DATA::DATA_TYPE)i;
      
      // Check if a user provided an input module name for this data type.
      if(_mod_names[i].size()){
	// Create TTree
	if(!_tree) 
	  _tree = fileService->make<TTree>(DATA::TREE_NAME.c_str(),"Analysis Tree");

	// Next, create data class objects
	switch(type){
	case DATA::MCParticle:
	  _data_ptr[i]=(data_base*)(new mcpart());
	  break;
	case DATA::MCTrajectory:
	  _data_ptr[i]=(data_base*)(new mcstep());
	  break;
	case DATA::MCTruth:
	case DATA::GENIE_MCTruth:
	case DATA::CRY_MCTruth:
	  _data_ptr[i]=(data_base*)(new mctruth(type));
	  break;
	case DATA::MCNeutrino:
	case DATA::GENIE_MCNeutrino:
	  _data_ptr[i]=(data_base*)(new mcnu(type));
	  break;
	case DATA::Track:
	case DATA::Kalman3DSPS:
	case DATA::Kalman3DHit:
	case DATA::Bezier:
	  _data_ptr[i]=(data_base*)(new track(type));
	  break;
	case DATA::SpacePoint:
	  _data_ptr[i]=(data_base*)(new sps());
	  break;
	case DATA::Hit:
	case DATA::CrawlerHit:
	case DATA::GausHit:
	case DATA::APAHit:
	case DATA::FFTHit:
	case DATA::RFFHit:
	  _data_ptr[i]=(data_base*)(new hit(type));
	  break;
	case DATA::Cluster:
	case DATA::CrawlerCluster:
	case DATA::DBCluster:
	case DATA::FuzzyCluster:
	case DATA::HoughCluster:
	  _data_ptr[i]=(data_base*)(new cluster(type));
	  break;
	case DATA::Wire:
	case DATA::FIFOChannel:
	case DATA::Event:
	case DATA::UserInfo:
	case DATA::Seed:
	case DATA::Shower:
	case DATA::Calorimetry:
	case DATA::DATA_TYPE_MAX:
	  mf::LogError("DataScanner")<<Form("Data type %d not supported!",type);
	  break;
	}

	// Set TTree branch to the created data class object's address
	if(_data_ptr[i])
	  _data_ptr[i]->set_address(_tree,true);
      }
    }
    
  }

  //#######################################################################################################
  void DataScanner::ParseModuleName(std::vector<std::string> &mod_names, std::string name) 
  //#######################################################################################################
  {
    while(1){

      size_t pos = name.find(":");

      if(pos>=name.size()) break;

      mod_names.push_back(std::string(name.substr(0,pos)));

      name = name.substr(pos+1);
      
    }

    if(name.size()) mod_names.push_back(name);

  }


  //#######################################################################################################
  void DataScanner::beginJob()
  //#######################################################################################################
  {}

  //#######################################################################################################
  void DataScanner::analyze(const art::Event& evt) 
  //#######################################################################################################
  {
    // Loop over data type. We check whether or not to fill each data type
    for(size_t i=0; i<(size_t)(DATA::DATA_TYPE_MAX); i++){
      
      // If data pointer is not set, we don't have to fill this data type
      if(!(_data_ptr[i])) continue;
      
      // Reset data
      _data_ptr[i]->clear_event();
      
      // Reaching this point means we want to fill this data type.
      // Handle different kind of data (class wise)
      DATA::DATA_TYPE type = (DATA::DATA_TYPE)i;
      switch(type){

      case DATA::Track:
      case DATA::Kalman3DSPS:
      case DATA::Kalman3DHit:
      case DATA::Bezier:
	// Data types to be stored in event_track class
	for(size_t j=0; j<_mod_names[i].size(); ++j)
	  ReadTrack(evt, _mod_names[i][j], (track*)(_data_ptr[i]));
	break;
      case DATA::MCNeutrino:
      case DATA::GENIE_MCNeutrino:
	for(size_t j=0; j<_mod_names[i].size(); ++j){
	  ReadMCNeutrino(evt, _mod_names[i][j], _flux_mod, (mcnu*)(_data_ptr[i]));
	}
	break;
      case DATA::MCTruth:
      case DATA::GENIE_MCTruth:
      case DATA::CRY_MCTruth:
	// Data type to be stored in event_mc class
	for(size_t j=0; j<_mod_names[i].size(); ++j){
	  ReadMCTruth     (evt, _mod_names[i][j], (mctruth*)(_data_ptr[i]));
	}
	break;
      case DATA::MCParticle:
	for(size_t j=0; j<_mod_names[i].size(); ++j){
	  ReadMCPartArray(evt, _mod_names[i][j], 
			  (mcstep*)(_data_ptr[DATA::MCTrajectory]), 
			  (mcpart*)(_data_ptr[DATA::MCParticle] ));
	}
	break;
      case DATA::SpacePoint:
 	// Data type to be stored in event_sps class
	for(size_t j=0; j<_mod_names[i].size(); ++j)
	  ReadSPS(evt,_mod_names[i][j], (sps*)(_data_ptr[i]));
	break;

	//case DATA::FIFOChannel:
	//// Data type to be stored in event_pmt class
	//for(size_t j=0; j<_mod_names[i].size(); ++j)
	//ReadPMT(evt,_mod_names[i][j], (event_pmt*)(_data_ptr[i]));
	//break;
      case DATA::Hit:
      case DATA::CrawlerHit:
      case DATA::GausHit:
      case DATA::APAHit:
      case DATA::FFTHit:
      case DATA::RFFHit:
	// Data type to be stored in event_wire class
	for(size_t j=0; j<_mod_names[i].size(); ++j)
	  ReadHit(evt,_mod_names[i][j],(hit*)(_data_ptr[i]));
	break;
      case DATA::Cluster:
      case DATA::DBCluster:
      case DATA::FuzzyCluster:
      case DATA::HoughCluster:
      case DATA::CrawlerCluster:
	// Data type to be stored in event_cluster class
	for(size_t j=0; j<_mod_names[i].size(); ++j)
	  ReadCluster(evt,_mod_names[i][j],(cluster*)(_data_ptr[i]));
	break;
      case DATA::Wire:
      case DATA::FIFOChannel:
      case DATA::MCTrajectory:
      case DATA::Event:
      case DATA::Seed:
      case DATA::UserInfo:
      case DATA::Shower:
      case DATA::Calorimetry:
      case DATA::DATA_TYPE_MAX:
	break;
      }
    }
    _tree->Fill();
  }

  bool DataScanner::IsFV(Double_t x, Double_t y, Double_t z, Double_t t)  const{

    // Hard volume cut
    if( x > _x_max || x < _x_min || z > _z_max || z < _z_min || y > _y_max || y < _y_min ) return false;

    // Now compare x-coordinate to make sure this point is inside the readout window.
    Double_t drift_v = (_x_max - _x_min) / (_readout_size / _readout_freq); 
    Double_t drift_t = (x - _x_min) / drift_v;

    // Charge arrives @ wire-plane @ drift_t + t
    return ( _readout_startT < (drift_t + t) && (drift_t + t) < _readout_endT);
  }

  Double_t DataScanner::GetDistanceToWall(Double_t x, Double_t y, Double_t z) const{

    Double_t dx = std::max((x - _x_min), (_x_max - x));
    Double_t dy = std::max((y - _y_min), (_y_max - y));
    Double_t dz = std::max((z - _z_min), (_z_max - z));
    
    return std::max(dx,std::max(dy,dz));
  }

  /*
  //#######################################################################################################
  void DataScanner::ReadWire(const art::Event& evt, const std::string mod_name, event_wire* data_ptr){
  //#######################################################################################################

    std::vector<const recob::Wire*> wireArray;
    try{

      evt.getView(mod_name,wireArray);

    }catch (art::Exception const& e) {

      if (e.categoryCode() != art::errors::ProductNotFound ) throw;

    }

    for(size_t i=0; i<wireArray.size(); i++){

      const recob::Wire* wire_ptr(wireArray.at(i));

      wire wire_light(wire_ptr->Signal(),
		      wire_ptr->Channel(),
		      (GEO::View_t)(wire_ptr->View()),
		      (GEO::SigType_t)(wire_ptr->SignalType()));
      
      data_ptr->add_wire(wire_light);

    }

  }
  */
  //#######################################################################################################
  void DataScanner::ReadHit(const art::Event& evt, const std::string mod_name, hit* data_ptr){
  //#######################################################################################################

    std::vector<const recob::Hit*> hitArray;
    try{

      evt.getView(mod_name,hitArray);

    }catch (art::Exception const& e) {

      if (e.categoryCode() != art::errors::ProductNotFound ) throw;

    }

    for(size_t i=0; i<hitArray.size(); i++){
      
      const recob::Hit* hit_ptr(hitArray.at(i));

      data_ptr->add_hit((UChar_t)(hit_ptr->WireID().Plane), (UShort_t)(hit_ptr->WireID().Wire), hit_ptr->Channel(),
			hit_ptr->StartTime(), hit_ptr->PeakTime(), hit_ptr->EndTime(),
			hit_ptr->SigmaStartTime(), hit_ptr->SigmaPeakTime(), hit_ptr->SigmaEndTime(),
			hit_ptr->Charge(true), hit_ptr->SigmaCharge(true),0);

    }
  }

  //#######################################################################################################
  void DataScanner::ReadCluster(const art::Event& evt, const std::string mod_name, cluster* data_ptr){
  //#######################################################################################################

    std::vector<const recob::Cluster*> clusterArray;
    try{

      evt.getView(mod_name,clusterArray);

    }catch (art::Exception const& e) {

      if (e.categoryCode() != art::errors::ProductNotFound ) throw;

    }

    for(size_t i=0; i<clusterArray.size(); ++i) {

      const recob::Cluster* cluster_ptr(clusterArray.at(i));

      data_ptr->add_cluster(cluster_ptr->ID(), cluster_ptr->View(),
			    cluster_ptr->Charge(),
			    cluster_ptr->dTdW(), cluster_ptr->SigmadTdW(),
			    cluster_ptr->dQdW(), cluster_ptr->SigmadQdW(),
			    cluster_ptr->StartPos()[0], cluster_ptr->StartPos()[1], cluster_ptr->StartPos()[2],
			    cluster_ptr->SigmaStartPos()[0], cluster_ptr->SigmaStartPos()[1], cluster_ptr->SigmaStartPos()[2],
			    cluster_ptr->EndPos()[0], cluster_ptr->EndPos()[1], cluster_ptr->EndPos()[2],
			    cluster_ptr->SigmaEndPos()[0], cluster_ptr->SigmaEndPos()[1], cluster_ptr->SigmaEndPos()[2]);
    }
  }

  /*
  //#######################################################################################################
  void DataScanner::ReadPMT(const art::Event& evt, const std::string mod_name, event_pmt* data_ptr){
  //#######################################################################################################
    
    std::vector<const optdata::FIFOChannel*> pmtArray;
    try{

      evt.getView(mod_name,pmtArray);

    }catch (art::Exception const& e) {

      if (e.categoryCode() != art::errors::ProductNotFound ) throw;

    }

    for(size_t i=0; i<pmtArray.size(); ++i) {

      const optdata::FIFOChannel* fifo_ptr(pmtArray.at(i));

      optdata::Optical_Category_t cat(fifo_ptr->Category());
      PMT::DISCRIMINATOR disc_id = PMT::DISC_MAX;
      switch(cat){
      case optdata::kFEMCosmicHighGain:
      case optdata::kFEMCosmicLowGain:
      case optdata::kCosmicPMTTrigger:
	disc_id = PMT::COSMIC_DISC;
	break;
      case optdata::kFEMBeamHighGain:
      case optdata::kFEMBeamLowGain:
      case optdata::kBeamPMTTrigger:
	disc_id = PMT::BEAM_DISC;
	break;
      case optdata::kUndefined:
      case optdata::kHighGain:
      case optdata::kLowGain:
      default:
	disc_id = PMT::DISC_MAX;
      }

      pmtfifo fifo_light(fifo_ptr->ChannelNumber(),
			 fifo_ptr->Frame(),
			 fifo_ptr->TimeSlice(),
			 disc_id,
			 *fifo_ptr);
      
      data_ptr->add_fifo(fifo_light);
    }
    
  }

  */
  //#######################################################################################################
  void DataScanner::ReadSPS(const art::Event& evt, const std::string mod_name, sps* data_ptr){
  //#######################################################################################################

    std::vector<const recob::SpacePoint* > spsArray;
    try {

      evt.getView(mod_name,spsArray);

    }catch (art::Exception const& e) {

      if (e.categoryCode() != art::errors::ProductNotFound ) throw;

    }

    for(size_t i=0; i<spsArray.size(); ++i){

      const recob::SpacePoint* sps_ptr(spsArray.at(i));

      data_ptr->add_spacepoint(sps_ptr->ID(),
			       sps_ptr->XYZ()[0], sps_ptr->XYZ()[1], sps_ptr->XYZ()[2],
			       sps_ptr->ErrXYZ()[0], sps_ptr->ErrXYZ()[1], sps_ptr->ErrXYZ()[2],
			       sps_ptr->Chisq());

    }

  }

  //#######################################################################################################
  void ReadMCNeutrino(const art::Event& evt, 
		      const std::string nu_mod_name, const std::string flux_mod_name,
		      mcnu* data_ptr)
  //#######################################################################################################
  {
    std::vector<const simb::MCNeutrino*> nuArray;
    try {

      evt.getView(nu_mod_name,nuArray);

    }catch (art::Exception const& e) {

      if (e.categoryCode() != art::errors::ProductNotFound ) throw;

    }

    std::vector<const simb::MCFlux*> fluxArray;
    try {

      evt.getView(flux_mod_name,fluxArray);

    }catch (art::Exception const& e) {

      if (e.categoryCode() != art::errors::ProductNotFound ) throw;

    }

    

    for(size_t i=0; i < nuArray.size(); ++i){
      
      const simb::MCNeutrino* nu_ptr(nuArray.at(i));

      if(fluxArray.size()){

	const simb::MCFlux flux_ptr(fluxArray.at(0));

	data_ptr->add_neutrino(nu_ptr->Nu().PdgCode(), nu_ptr->CCNC(), nu_ptr->Nu().StatusCode(),
			       nu_ptr->QSqr(), nu_ptr->W(), 
			       nu_ptr->HitNuc(), 
			       nu_ptr->Nu().Vx(), nu_ptr->Nu().Vy(), nu_ptr->Nu().Vz(),
			       nu_ptr->Nu().E(), nu_ptr->Nu().Px(), nu_ptr->Nu().Py(), nu_ptr->Nu().Pz(),
			       nc_ptr->Lepton().E(), nu_ptr->Lepton().Px(), nu_ptr->Lepton().Py(), nu_ptr->Lepton().Pz(),
			       flux_ptr->ftptype, flux_ptr->ftpx, flux_ptr->ftpy, flux_ptr->ftpz);
      }else{
	data_ptr->add_neutrino(nu_ptr->Nu().PdgCode(), nu_ptr->CCNC(), nu_ptr->Nu().StatusCode(),
			       nu_ptr->QSqr(), nu_ptr->W(), 
			       nu_ptr->HitNuc(), 
			       nu_ptr->Nu().Vx(), nu_ptr->Nu().Vy(), nu_ptr->Nu().Vz(),
			       nu_ptr->Nu().E(), nu_ptr->Nu().Px(), nu_ptr->Nu().Py(), nu_ptr->Nu().Pz(),
			       nc_ptr->Lepton().E(), nu_ptr->Lepton().Px(), nu_ptr->Lepton().Py(), nu_ptr->Lepton().Pz(),
			       99999, 0, 0, 0);

      }
    
  }

  void DataScanner::ReadMCPartArray(const art::Event& evt, const std::string mod_name, 
				    mcstep* step_ptr, mcpart* part_ptr){
  //#######################################################################################################
    
    std::vector<const simb::MCParticle*> mciArray;

    try {

      evt.getView(mod_name,mciArray);

    }catch (art::Exception const& e) {

      if (e.categoryCode() != art::errors::ProductNotFound ) throw;

    }
    
    for(size_t i=0; i < mciArray.size(); i++){

      const simb::MCParticle* part(mciArray.at(i));

      Double_t last_E=0;
      Double_t last_x=0;
      Double_t last_y=0;
      Double_t last_z=0;
      Double_t dx=0;
      Double_t de=0;
      Double_t dx_tot=0;

      Double_t fv_startx=0;
      Double_t fv_starty=0;
      Double_t fv_startz=0;
      Double_t fv_startt=0;
      Double_t fv_momx=0;
      Double_t fv_momy=0;
      Double_t fv_momz=0;
      Double_t fv_mom;
      Double_t dx_fv_tot=0;
      Bool_t   fv_first=true;
      
      for(size_t k=0; k<part->NumberTrajectoryPoints(); k++) {

	if(k>0) {
	  dx = sqrt( pow((last_x - part->Vx(k)),2) +
		      pow((last_y - part->Vy(k)),2) +
		      pow((last_z - part->Vz(k)),2) );
	  de = last_E - part->E(k);

	};
	dx_tot+=dx;

	if(IsFV(part->Vx(k), part->Vy(k), part->Vz(k), part->T(k))) {
	  
	  if(fv_first) {
	    fv_startx = part->Vx(k);
	    fv_starty = part->Vy(k);
	    fv_startz = part->Vz(k);
	    fv_startt = part->T(k);
	    fv_momx   = part->Px(k);
	    fv_momy   = part->Py(k);
	    fv_momz   = part->Pz(k);
	    fv_mom    = sqrt( pow(fv_momx,2) + pow(fv_momy,2) + pow(fv_momz,2) );
	    fv_first=false;
	  }
	  dx_fv_tot+=dx;

	  if(part_ptr)
	    part_ptr->set_fv_part_info(fv_mom, fv_momx, fv_momy, fv_momz,
				       fv_startx, fv_starty, fv_startz, fv_startt,
				       part->Vx(k), part->Vy(k), part->Vz(k), part->T(k),
				       dx_fv_tot);
	}

	if(step_ptr)
	  step_ptr->add_trajectory(part->TrackId(), part->PdgCode(),
				   part->Px(k), part->Py(k), part->Pz(k),
				   part->Vx(k), part->Vy(k), part->Vz(k),
				   dx, de);
      
	last_E = part->E(k);
	last_x = part->Vx(k);
	last_y = part->Vy(k);
	last_z = part->Vz(k);
      }
      if(part_ptr){
	part_ptr->set_part_info(part->P(),    part->Px(),   part->Py(),   part->Pz(),
				part->Vx(),   part->Vy(),   part->Vz(),   part->T(),
				part->EndX(), part->EndY(), part->EndZ(), part->EndT(),
				dx_tot);
	part_ptr->add_particle(part->TrackId(), part->PdgCode(), part->StatusCode());
      }
    }
  }

  //#######################################################################################################
  void DataScanner::ReadMCTruth(const art::Event& evt, const std::string mod_name, mctruth* data_ptr){
  //#######################################################################################################

    std::vector<const simb::MCTruth*> mciArray;

    try {

      evt.getView(mod_name,mciArray);

    }catch (art::Exception const& e) {

      if (e.categoryCode() != art::errors::ProductNotFound ) throw;

    }

    for(size_t i=0; i < mciArray.size(); ++i){

      if(i>0) {
	mf::LogError("DataScanner")<<" Detected multiple MCTruth! The structure does not support this...";
	break;
      }

      const simb::MCTruth* mci_ptr(mciArray.at(i));

      for(size_t j=0; j < (size_t)(mci_ptr->NParticles()); ++j){
	
	const simb::MCParticle part(mci_ptr->GetParticle(j));

	data_ptr->add_primary(part.PdgCode(), part.TrackId(), part.StatusCode(), (UChar_t)(mci_ptr->Origin()),
			      part.NumberDaughters(), part.Mother(),
			      part.Vx(), part.Vy(), part.Vz(),
			      part.Mass(), part.E(),
			      part.P(), part.Px(), part.Py(), part.Pz());
      }
    }
  }


  //#######################################################################################################
  void DataScanner::ReadTrack(const art::Event& evt, const std::string mod_name, track* data_ptr){
  //#######################################################################################################

    std::vector<const recob::Track*> trackArray;

    try {

      evt.getView(mod_name,trackArray);

    }catch (art::Exception const& e) {

      if (e.categoryCode() != art::errors::ProductNotFound ) throw;

    }

    for(size_t i=0; i < trackArray.size(); ++i){

      // Obtain recob::Track object pointer
      const recob::Track* track_ptr(trackArray.at(i));

      // Direction & points
      Double_t length=0;
      for(size_t i=0; i<track_ptr->NumberTrajectoryPoints(); i++) {

	const TVector3 pos = track_ptr->LocationAtPoint(i);
	const TVector3 dir = track_ptr->DirectionAtPoint(i);
	data_ptr->add_trajectory(track_ptr->ID(),
				 pos[0],pos[1],pos[2],
				 dir[0],dir[1],dir[2],
				 track_ptr->MomentumAtPoint(i));
	
	if(i) length += (pos - (track_ptr->LocationAtPoint(i-1))).Mag();
      }
      const TVector3 end = track_ptr->End();
      const TVector3 pos = track_ptr->Vertex();
      data_ptr->add_track( track_ptr->ID(),
			   pos[0],pos[1],pos[2],GetDistanceToWall(pos[0],pos[1],pos[2]),
			   end[0],end[1],end[2],GetDistanceToWall(end[0],end[1],end[2]),
			   track_ptr->Theta(), track_ptr->Phi(), track_ptr->VertexMomentum(), length);
      
    }
    
    // Done
  }

} // namespace 

/** @} */ // end of doxygen group
