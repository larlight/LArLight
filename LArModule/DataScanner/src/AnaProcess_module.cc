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

// Framework includes
#include "art/Framework/Core/ModuleMacros.h"
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

// LArSoft includes
#include "Geometry/Geometry.h"
#include "RecoBase/Wire.h"
#include "RecoBase/Hit.h"
#include "RecoBase/Track.h"
#include "RecoBase/Cluster.h"
#include "RecoBase/SpacePoint.h"
#include "RecoBase/Shower.h"
#include "AnalysisBase/Calorimetry.h"
#include "SimulationBase/MCTruth.h"
#include "OpticalDetectorData/FIFOChannel.h"
#include "OpticalDetectorData/OpticalTypes.h"
#include "RecoAlg/ClusterParamsAlg.h"
#include "Utilities/LArProperties.h"
#include "Utilities/GeometryUtilities.h"
#include "Utilities/DetectorProperties.h"

// ART includes.
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/FindManyP.h"
#include "art/Persistency/Common/Ptr.h"
#include "art/Persistency/Common/PtrVector.h"

// LArLight includes
#include <AnaProcess/Base/Base-TypeDef.hh>
#include <AnaProcess/DataFormat/DataFormat-TypeDef.hh>

// ROOT includes
#include "TTree.h"
#include "TPrincipal.h"

namespace datascanner {

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

    /// Function to read & store calibrated wire data
    void ReadWire(const art::Event& evt, const std::string mod_name, larlight::event_wire* data_ptr);

    /// Function to read & store reconstructed hit data
    void ReadHit(const art::Event& evt, const std::string mod_name, larlight::event_hit* data_ptr);

    /// Function to read & store reconstructed hit data
    void ReadCluster(const art::Event& evt, const std::string mod_name, larlight::event_cluster* data_ptr);

    /// Function to read & store spacepoints
    void ReadPMT(const art::Event& evt, const std::string mod_name, larlight::event_pmt* data_ptr);

    /// Function to read & store spacepoints
    void ReadSPS(const art::Event& evt, const std::string mod_name, larlight::event_sps* data_ptr);

    /// Function to read & store Tracking information
    void ReadTrack(const art::Event& evt, const std::string mod_name, larlight::event_track* data_ptr);

    /// Function to read & store MCTruth information
    void ReadMCTruth(const art::Event& evt, const std::string mod_name, larlight::event_mc* data_ptr);

    /// Function to read & store MCTruth information
    void ReadMCPartArray(const art::Event& evt, const std::string mod_name, larlight::event_mc* data_ptr);

    /// Function to read & store Shower variables
    void ReadShower(const art::Event& evt, const std::string mod_name, larlight::event_shower* data_ptr);

    /// Function to read & store Calorimetry variables
    void ReadCalorimetry(const art::Event& evt, const std::string mod_name, larlight::event_calorimetry* data_ptr);

    /// Function to store user specific variables
    void StoreUserInfo(const art::Event& evt, larlight::event_user* data_ptr);

    /// Utility function to parse module name string
    void ParseModuleName(std::vector<std::string> &mod_names, std::string name);

    std::vector<TTree*>                    _trees;     ///< output data holder TTree
    std::vector<std::vector<std::string> > _mod_names; ///< input data production module names input from FCL file
    std::vector<larlight::data_base*>                _data_ptr;  ///< output data holder class object pointers

    // ClusterParamsAlg module
    cluster::ClusterParamsAlg fCParamsAlg;

    // Utility conversion factors
    double CONV_WIRE2CM; ///< wire -> length (cm) conversion factor
    double CONV_TIME2CM; ///< time -> length (cm) conversion factor

    // Service modules
    art::ServiceHandle<util::DetectorProperties> _detp;
    art::ServiceHandle<util::LArProperties> _larp;
    art::ServiceHandle<geo::Geometry> _geo;
  };

} 

#endif//  DataScanner_H

// DataScanner.cc

namespace datascanner {
  DEFINE_ART_MODULE(DataScanner);
}

namespace datascanner {

  //#######################################################################################################
  DataScanner::DataScanner(fhicl::ParameterSet const& pset) : EDAnalyzer(pset),
							      _trees(larlight::DATA::DATA_TYPE_MAX,0), 
							      _data_ptr(larlight::DATA::DATA_TYPE_MAX,0),
							      fCParamsAlg(pset.get< fhicl::ParameterSet >("ClusterParamsAlg"),pset.get< std::string >("module_type"))
  //#######################################################################################################
  {

    // Initialize module name container
    for(size_t i=0; i<(size_t)(larlight::DATA::DATA_TYPE_MAX); i++)
      
      _mod_names.push_back(std::vector<std::string>());

    // Obtain module names for input data
    // If a user set an empty string for these params, they are ignored for processing.
    ParseModuleName ( _mod_names[larlight::DATA::Bezier],               pset.get<std::string>("fModName_Bezier")               );
    ParseModuleName ( _mod_names[larlight::DATA::Kalman3DSPS],          pset.get<std::string>("fModName_Kalman3DSPS")          );
    ParseModuleName ( _mod_names[larlight::DATA::Kalman3DHit],          pset.get<std::string>("fModName_Kalman3DHit")          );
    ParseModuleName ( _mod_names[larlight::DATA::MCTruth],              pset.get<std::string>("fModName_MCTruth")              );
    ParseModuleName ( _mod_names[larlight::DATA::SpacePoint],           pset.get<std::string>("fModName_SpacePoint")           );
    ParseModuleName ( _mod_names[larlight::DATA::FIFOChannel],          pset.get<std::string>("fModName_FIFOChannel")          );
    ParseModuleName ( _mod_names[larlight::DATA::Wire],                 pset.get<std::string>("fModName_CalData")              );
    ParseModuleName ( _mod_names[larlight::DATA::CrawlerHit],           pset.get<std::string>("fModName_CrawlerHit")           );
    ParseModuleName ( _mod_names[larlight::DATA::GausHit],              pset.get<std::string>("fModName_GausHit")              );
    ParseModuleName ( _mod_names[larlight::DATA::APAHit],               pset.get<std::string>("fModName_APAHit")               );
    ParseModuleName ( _mod_names[larlight::DATA::FFTHit],               pset.get<std::string>("fModName_FFTHit")               );
    ParseModuleName ( _mod_names[larlight::DATA::CrawlerCluster],       pset.get<std::string>("fModName_CrawlerCluster")       );
    ParseModuleName ( _mod_names[larlight::DATA::DBCluster],            pset.get<std::string>("fModName_DBCluster")            );
    ParseModuleName ( _mod_names[larlight::DATA::FuzzyCluster],         pset.get<std::string>("fModName_FuzzyCluster")         );
    ParseModuleName ( _mod_names[larlight::DATA::HoughCluster],         pset.get<std::string>("fModName_HoughCluster")         );
    ParseModuleName ( _mod_names[larlight::DATA::ShowerAngleCluster],   pset.get<std::string>("fModName_ShowerAngleCluster")   );
    ParseModuleName ( _mod_names[larlight::DATA::Shower],               pset.get<std::string>("fModName_Shower")               );
    ParseModuleName ( _mod_names[larlight::DATA::Calorimetry],          pset.get<std::string>("fModName_Calorimetry")          );

    // Next we make storage data class objects for those data types specified in fcl files.
    art::ServiceHandle<art::TFileService>  fileService;

    for(size_t i=0; i<(int)(larlight::DATA::DATA_TYPE_MAX); i++){

      larlight::DATA::DATA_TYPE type = (larlight::DATA::DATA_TYPE)i;

      // Check if a user provided an input module name for this data type.
      if(_mod_names[i].size() || type==larlight::DATA::UserInfo){
	// Create TTree
	_trees[i] = fileService->make<TTree>(Form("%s_tree",larlight::DATA::DATA_TREE_NAME[i].c_str()),"");

	// Next, create data class objects
	switch(type){
	case larlight::DATA::MCTruth:
	  _data_ptr[i]=(larlight::data_base*)(new larlight::event_mc);
	  break;
	case larlight::DATA::Track:
	case larlight::DATA::Kalman3DSPS:
	case larlight::DATA::Kalman3DHit:
	case larlight::DATA::Bezier:
	  _data_ptr[i]=(larlight::data_base*)(new larlight::event_track);
	  break;
	case larlight::DATA::SpacePoint:
	  _data_ptr[i]=(larlight::data_base*)(new larlight::event_sps);
	  break;
	case larlight::DATA::FIFOChannel:
	  _data_ptr[i]=(larlight::data_base*)(new larlight::event_pmt);
	  break;
	case larlight::DATA::Hit:
	case larlight::DATA::CrawlerHit:
	case larlight::DATA::GausHit:
	case larlight::DATA::APAHit:
	case larlight::DATA::FFTHit:
	  _data_ptr[i]=(larlight::data_base*)(new larlight::event_hit);
	  break;
	case larlight::DATA::Wire:
	  _data_ptr[i]=(larlight::data_base*)(new larlight::event_wire);
	  break;
	case larlight::DATA::Cluster:
	case larlight::DATA::CrawlerCluster:
	case larlight::DATA::DBCluster:
	case larlight::DATA::FuzzyCluster:
	case larlight::DATA::HoughCluster:
	case larlight::DATA::ShowerAngleCluster:
	  _data_ptr[i]=(larlight::data_base*)(new larlight::event_cluster);
	  break;
	case larlight::DATA::UserInfo:
	  _data_ptr[i]=(larlight::data_base*)(new larlight::event_user);
	  break;
	case larlight::DATA::Shower:
	  _data_ptr[i]=(larlight::data_base*)(new larlight::event_shower);
	  break;
	case larlight::DATA::Calorimetry:
	  _data_ptr[i]=(larlight::data_base*)(new larlight::event_calorimetry);
	  break;
	case larlight::DATA::Event:
	case larlight::DATA::Seed:
	case larlight::DATA::DATA_TYPE_MAX:
	  mf::LogError("DataScanner")<<Form("Data type %d not supported!",type);
	  break;
	}

	if(_data_ptr[i]) {
	  // Set TTree branch to the created data class object's address
	  _trees[i]->Branch(Form("%s_branch",larlight::DATA::DATA_TREE_NAME[i].c_str()),
			    _data_ptr[i]->GetName(),
			    &(_data_ptr[i]));
	}
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
  {
    // Some detector constants
    CONV_WIRE2CM = _geo->WirePitch(0,1,0);    //wire pitch in cm
    CONV_TIME2CM = (_detp->SamplingRate()/1000.) * _larp->DriftVelocity(_larp->Efield(),_larp->Temperature());
  }

  //#######################################################################################################
  void DataScanner::analyze(const art::Event& evt) 
  //#######################################################################################################
  {
    // Loop over data type. We check whether or not to fill each data type
    for(size_t i=0; i<(size_t)(larlight::DATA::DATA_TYPE_MAX); i++){
      
      // If data pointer is not set, we don't have to fill this data type
      if(!(_trees[i])) continue;
      
      // Reset data
      _data_ptr[i]->clear_data();

      // Fill common variables such as run, subrun, and event id
      _data_ptr[i]->set_run      ( evt.id().run()    );
      _data_ptr[i]->set_subrun   ( evt.id().subRun() );
      _data_ptr[i]->set_event_id ( evt.id().event()  );

      // Reaching this point means we want to fill this data type.
      // Handle different kind of data (class wise)
      larlight::DATA::DATA_TYPE type = (larlight::DATA::DATA_TYPE)i;
      switch(type){

      case larlight::DATA::Track:
      case larlight::DATA::Kalman3DSPS:
      case larlight::DATA::Kalman3DHit:
      case larlight::DATA::Bezier:
	// Data types to be stored in event_track class
	for(size_t j=0; j<_mod_names[i].size(); ++j)
	  ReadTrack(evt, _mod_names[i][j], (larlight::event_track*)(_data_ptr[i]));
	break;

      case larlight::DATA::MCTruth:
	// Data type to be stored in event_mc class
	for(size_t j=0; j<_mod_names[i].size(); ++j){
	  ReadMCTruth     (evt, _mod_names[i][j], (larlight::event_mc*)(_data_ptr[i]));
	  ReadMCPartArray (evt, _mod_names[i][j], (larlight::event_mc*)(_data_ptr[i]));
	}
	break;

      case larlight::DATA::SpacePoint:
 	// Data type to be stored in event_sps class
	for(size_t j=0; j<_mod_names[i].size(); ++j)
	  ReadSPS(evt,_mod_names[i][j], (larlight::event_sps*)(_data_ptr[i]));
	break;

      case larlight::DATA::FIFOChannel:
	// Data type to be stored in event_pmt class
	for(size_t j=0; j<_mod_names[i].size(); ++j)
	  ReadPMT(evt,_mod_names[i][j], (larlight::event_pmt*)(_data_ptr[i]));
	break;

      case larlight::DATA::Wire:
	// Data type to be stored in event_wire class
	for(size_t j=0; j<_mod_names[i].size(); ++j)
	  ReadWire(evt,_mod_names[i][j],(larlight::event_wire*)(_data_ptr[i]));
	break;

      case larlight::DATA::Hit:
      case larlight::DATA::CrawlerHit:
      case larlight::DATA::GausHit:
      case larlight::DATA::APAHit:
      case larlight::DATA::FFTHit:
	// Data type to be stored in event_wire class
	for(size_t j=0; j<_mod_names[i].size(); ++j)
	  ReadHit(evt,_mod_names[i][j],(larlight::event_hit*)(_data_ptr[i]));
	break;

      case larlight::DATA::Cluster:
      case larlight::DATA::DBCluster:
      case larlight::DATA::FuzzyCluster:
      case larlight::DATA::HoughCluster:
      case larlight::DATA::CrawlerCluster:
      case larlight::DATA::ShowerAngleCluster:
	// Data type to be stored in event_cluster class
	for(size_t j=0; j<_mod_names[i].size(); ++j)
	  ReadCluster(evt,_mod_names[i][j],(larlight::event_cluster*)(_data_ptr[i]));
	break;

      case larlight::DATA::UserInfo:
	// Data type to be stored in user_info class
	StoreUserInfo(evt,(larlight::event_user*)(_data_ptr[i]));
	break;

      case larlight::DATA::Shower:
	for(size_t j=0; j<_mod_names[i].size(); ++j)
	  ReadShower(evt,_mod_names[i][j],(larlight::event_shower*)(_data_ptr[i]));
	break;

      case larlight::DATA::Calorimetry:
	for(size_t j=0; j<_mod_names[i].size(); ++j)
	  ReadCalorimetry(evt,_mod_names[i][j],(larlight::event_calorimetry*)(_data_ptr[i]));
	break;
	
      case larlight::DATA::Event:
      case larlight::DATA::Seed:
      case larlight::DATA::DATA_TYPE_MAX:
	break;
      }
      // Fill this TTree
      _trees[i]->Fill();
    }

  }

  //#######################################################################################################
  void DataScanner::ReadWire(const art::Event& evt, const std::string mod_name, larlight::event_wire* data_ptr){
  //#######################################################################################################

    std::vector<const recob::Wire*> wireArray;
    try{

      evt.getView(mod_name,wireArray);

    }catch (art::Exception const& e) {

      if (e.categoryCode() != art::errors::ProductNotFound ) throw;

    }

    for(size_t i=0; i<wireArray.size(); i++){

      const recob::Wire* wire_ptr(wireArray.at(i));
      
      larlight::wire wire_light(wire_ptr->Signal(),
				wire_ptr->Channel(),
				(larlight::GEO::View_t)(wire_ptr->View()),
				(larlight::GEO::SigType_t)(wire_ptr->SignalType()));
      
      data_ptr->add_wire(wire_light);
      
    }

  }

  //#######################################################################################################
  void DataScanner::ReadHit(const art::Event& evt, const std::string mod_name, larlight::event_hit* data_ptr){
  //#######################################################################################################

    std::vector<const recob::Hit*> hitArray;
    try{

      evt.getView(mod_name,hitArray);

    }catch (art::Exception const& e) {

      if (e.categoryCode() != art::errors::ProductNotFound ) throw;

    }

    for(size_t i=0; i<hitArray.size(); i++){
      
      const recob::Hit* hit_ptr(hitArray.at(i));

      larlight::hit hit_light;

      hit_light.set_waveform(hit_ptr->fHitSignal);
      hit_light.set_times(hit_ptr->StartTime(),
			  hit_ptr->PeakTime(),
			  hit_ptr->EndTime());
      hit_light.set_times_err(hit_ptr->SigmaStartTime(),
			      hit_ptr->SigmaPeakTime(),
			      hit_ptr->SigmaEndTime());
      hit_light.set_charge(hit_ptr->Charge(),hit_ptr->Charge(true));
      hit_light.set_charge_err(hit_ptr->SigmaCharge(),hit_ptr->SigmaCharge(true));
      hit_light.set_multiplicity(hit_ptr->Multiplicity());
      hit_light.set_channel(hit_ptr->Channel());
      hit_light.set_wire(hit_ptr->WireID().Wire);
      hit_light.set_fit_goodness(hit_ptr->GoodnessOfFit());
      hit_light.set_view((larlight::GEO::View_t)(hit_ptr->View()));
      hit_light.set_sigtype((larlight::GEO::SigType_t)(hit_ptr->SignalType()));

      data_ptr->add_hit(hit_light);
    }
  }

  //#######################################################################################################
  void DataScanner::ReadCluster(const art::Event& evt, const std::string mod_name, larlight::event_cluster* data_ptr){
  //#######################################################################################################

    std::vector<const recob::Cluster*> clusterArray;
    try{

      evt.getView(mod_name,clusterArray);

    }catch (art::Exception const& e) {

      if (e.categoryCode() != art::errors::ProductNotFound ) throw;

      return;
    }

    art::Handle< std::vector<recob::Cluster> > cluster_handle;
    evt.getByLabel(mod_name, cluster_handle);
    art::FindManyP<recob::Hit> hit_m(cluster_handle, evt, mod_name);

    for(size_t i=0; i<clusterArray.size(); ++i) {

      const recob::Cluster* cluster_ptr(clusterArray.at(i));
      
      larlight::cluster cluster_light;
      cluster_light.set_charge(cluster_ptr->Charge());
      cluster_light.set_dtdw(cluster_ptr->dTdW());
      cluster_light.set_dqdw(cluster_ptr->dQdW());
      cluster_light.set_dtdw_err(cluster_ptr->SigmadTdW());
      cluster_light.set_dqdw_err(cluster_ptr->SigmadQdW());
      cluster_light.set_id(cluster_ptr->ID());
      cluster_light.set_view((larlight::GEO::View_t)(cluster_ptr->View()));
      cluster_light.set_start_vtx(cluster_ptr->StartPos());
      cluster_light.set_end_vtx(cluster_ptr->EndPos());
      cluster_light.set_start_vtx_err(cluster_ptr->SigmaStartPos());
      cluster_light.set_end_vtx_err(cluster_ptr->SigmaEndPos());

      const std::vector<art::Ptr<recob::Hit> > hit_v = hit_m.at(i);

      for(auto const hit_ptr : hit_v){

	larlight::hit hit_light;

	hit_light.set_waveform(hit_ptr->fHitSignal);
        hit_light.set_times(hit_ptr->StartTime(),
                            hit_ptr->PeakTime(),
                            hit_ptr->EndTime());
        hit_light.set_times_err(hit_ptr->SigmaStartTime(),
                                hit_ptr->SigmaPeakTime(),
                                hit_ptr->SigmaEndTime());
        hit_light.set_charge(hit_ptr->Charge(),hit_ptr->Charge(true));
        hit_light.set_charge_err(hit_ptr->SigmaCharge(),hit_ptr->SigmaCharge(true));
        hit_light.set_multiplicity(hit_ptr->Multiplicity());
        hit_light.set_channel(hit_ptr->Channel());
        hit_light.set_wire(hit_ptr->WireID().Wire);
        hit_light.set_fit_goodness(hit_ptr->GoodnessOfFit());
        hit_light.set_view((larlight::GEO::View_t)(hit_ptr->View()));
        hit_light.set_sigtype((larlight::GEO::SigType_t)(hit_ptr->SignalType()));
	
	cluster_light.add_hit(hit_light);
	
      }

      data_ptr->add_cluster(cluster_light);
    }
  }

  //#######################################################################################################
  void DataScanner::ReadPMT(const art::Event& evt, const std::string mod_name, larlight::event_pmt* data_ptr){
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
      larlight::PMT::DISCRIMINATOR disc_id = larlight::PMT::DISC_MAX;
      switch(cat){
      case optdata::kFEMCosmicHighGain:
      case optdata::kFEMCosmicLowGain:
      case optdata::kCosmicPMTTrigger:
	disc_id = larlight::PMT::COSMIC_DISC;
	break;
      case optdata::kFEMBeamHighGain:
      case optdata::kFEMBeamLowGain:
      case optdata::kBeamPMTTrigger:
	disc_id = larlight::PMT::BEAM_DISC;
	break;
      case optdata::kUndefined:
      case optdata::kHighGain:
      case optdata::kLowGain:
      default:
	disc_id = larlight::PMT::DISC_MAX;
      }

      larlight::pmtfifo fifo_light(fifo_ptr->ChannelNumber(),
				   fifo_ptr->Frame(),
				   fifo_ptr->TimeSlice(),
				   disc_id,
				   *fifo_ptr);
      
      data_ptr->add_fifo(fifo_light);
    }
    
  }


  //#######################################################################################################
  void DataScanner::ReadSPS(const art::Event& evt, const std::string mod_name, larlight::event_sps* data_ptr){
  //#######################################################################################################

    std::vector<const recob::SpacePoint* > spsArray;
    try {

      evt.getView(mod_name,spsArray);

    }catch (art::Exception const& e) {

      if (e.categoryCode() != art::errors::ProductNotFound ) throw;

    }

    for(size_t i=0; i<spsArray.size(); ++i){

      const recob::SpacePoint* sps_ptr(spsArray.at(i));
      
      larlight::spacepoint sps_light(sps_ptr->ID(),
				     sps_ptr->XYZ()[0],    sps_ptr->XYZ()[1],    sps_ptr->XYZ()[2],
				     sps_ptr->ErrXYZ()[0], sps_ptr->ErrXYZ()[1], sps_ptr->ErrXYZ()[2],
				     sps_ptr->Chisq());
      data_ptr->add_sps(sps_light);

    }

  }

  //#######################################################################################################
  void DataScanner::ReadMCPartArray(const art::Event& evt, const std::string mod_name, larlight::event_mc* data_ptr){
  //#######################################################################################################
    
    std::vector<const simb::MCParticle*> mciArray;

    try {

      evt.getView(mod_name,mciArray);

    }catch (art::Exception const& e) {

      if (e.categoryCode() != art::errors::ProductNotFound ) throw;

    }
    
    for(size_t i=0; i < mciArray.size(); i++){

      const simb::MCParticle* part(mciArray.at(i));

      larlight::part_mc part_light(part->PdgCode(), part->TrackId(), part->Mother(), part->Process());
      
      for(size_t k=0; k<part->NumberTrajectoryPoints(); k++)
	
	part_light.add_track(part->Vx(k), part->Vy(k), part->Vz(k), part->T(k),
			     part->Px(k), part->Py(k), part->Pz(k));
      
      for(size_t k=0; k<(size_t)(part->NumberDaughters()); k++)
	
	part_light.add_daughter(part->Daughter(k));
      
      data_ptr->add_part(part_light);
      
    }
  }

  //#######################################################################################################
  void DataScanner::ReadMCTruth(const art::Event& evt, const std::string mod_name, larlight::event_mc* data_ptr){
  //#######################################################################################################

    std::vector<const simb::MCTruth*> mciArray;

    try {

      evt.getView(mod_name,mciArray);

    }catch (art::Exception const& e) {

      if (e.categoryCode() != art::errors::ProductNotFound ) throw;

      return;
    }

    for(size_t i=0; i < mciArray.size(); ++i){

      if(i>0) {
	mf::LogError("DataScanner")<<" Detected multiple MCTruth! The structure does not support this...";
	break;
      }

      const simb::MCTruth* mci_ptr(mciArray.at(i));

      data_ptr->set_gen_code( (larlight::MC::Origin_t) mci_ptr->Origin() );

      for(size_t j=0; j < (size_t)(mci_ptr->NParticles()); ++j){
	
	const simb::MCParticle part(mci_ptr->GetParticle(j));
	larlight::part_mc part_light(part.PdgCode(), part.TrackId(), part.Mother(), part.Process());

	for(size_t k=0; k<part.NumberTrajectoryPoints(); k++)

	  part_light.add_track(part.Vx(k), part.Vy(k), part.Vz(k), part.T(k),
			       part.Px(k), part.Py(k), part.Pz(k));
	
	for(size_t k=0; k<(size_t)(part.NumberDaughters()); k++)
	  
	  part_light.add_daughter(part.Daughter(k));
	
	data_ptr->add_part(part_light);
      }
    }
  }

  //#######################################################################################################
  void DataScanner::ReadShower(const art::Event& evt, const std::string mod_name, larlight::event_shower* data_ptr){
  //#######################################################################################################
    
    std::vector<const recob::Shower*> showerArray;

    try {

      evt.getView(mod_name,showerArray);

    }catch (art::Exception const& e) {

      if (e.categoryCode() != art::errors::ProductNotFound ) throw;

      return;
    }

    art::Handle< std::vector<recob::Shower> > shower_handle;
    evt.getByLabel(mod_name, shower_handle);
    art::FindManyP<recob::Cluster> cluster_m(shower_handle, evt, mod_name);

    for(size_t i=0; i < showerArray.size(); ++i) {

      const recob::Shower* shower_ptr(showerArray.at(i));

      larlight::shower light_shower;

      light_shower.set_id(shower_ptr->ID());
      light_shower.set_total_charge(shower_ptr->TotalCharge());
      light_shower.set_direction(shower_ptr->Direction());
      light_shower.set_direction_err(shower_ptr->DirectionErr());
      //light_shower.set_max_width(shower_ptr->MaxWidthX(),shower_ptr->MaxWidthY());
      //light_shower.set_distance_max_width(shower_ptr->DistanceMaxWidth());

      const std::vector<art::Ptr<recob::Cluster> > cluster_v = cluster_m.at(i);

      for(auto const cluster_ptr : cluster_v){
	
	larlight::cluster cluster_light;
	cluster_light.set_charge(cluster_ptr->Charge());
	cluster_light.set_dtdw(cluster_ptr->dTdW());
	cluster_light.set_dqdw(cluster_ptr->dQdW());
	cluster_light.set_dtdw_err(cluster_ptr->SigmadTdW());
	cluster_light.set_dqdw_err(cluster_ptr->SigmadQdW());
	cluster_light.set_id(cluster_ptr->ID());
	cluster_light.set_view((larlight::GEO::View_t)(cluster_ptr->View()));
	cluster_light.set_start_vtx(cluster_ptr->StartPos());
	cluster_light.set_end_vtx(cluster_ptr->EndPos());
	cluster_light.set_start_vtx_err(cluster_ptr->SigmaStartPos());
	cluster_light.set_end_vtx_err(cluster_ptr->SigmaEndPos());	
	
	light_shower.add_cluster(cluster_light);
      }

      data_ptr->add_shower(light_shower);
    }

  }

  //##########################################################################################################################
  void DataScanner::ReadCalorimetry(const art::Event& evt, const std::string mod_name, larlight::event_calorimetry* data_ptr){
  //##########################################################################################################################

    std::vector<const anab::Calorimetry*> caloArray;

    try {

      evt.getView(mod_name,caloArray);

    }catch (art::Exception const& e) {

      if (e.categoryCode() != art::errors::ProductNotFound ) throw;

    }

    for(size_t i=0; i < caloArray.size(); ++i) {

      const anab::Calorimetry* calo_ptr(caloArray.at(i));
      
      larlight::calorimetry light_calo;

      light_calo.set_dedx(calo_ptr->dEdx());
      light_calo.set_dqdx(calo_ptr->dQdx());
      light_calo.set_residual_range(calo_ptr->ResidualRange());
      light_calo.set_deadwire_range(calo_ptr->DeadWireResRC());
      light_calo.set_kinetic_energy(calo_ptr->KineticEnergy());
      light_calo.set_range(calo_ptr->Range());
      light_calo.set_track_pitch(calo_ptr->TrkPitchVec());

      data_ptr->add_calorimetry(light_calo);

    }

  }

  //#######################################################################################################
  void DataScanner::ReadTrack(const art::Event& evt, const std::string mod_name, larlight::event_track* data_ptr){
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

      // Prepare storage track object
      larlight::track track_light;
      
      //
      // Start copying data
      //
      
      // ID
      track_light.set_run      ( evt.id().run()    );
      track_light.set_subrun   ( evt.id().subRun() );
      track_light.set_event_id ( evt.id().event()  );
      track_light.set_track_id ( track_ptr->ID()   );

      // Direction & points
      for(size_t i=0; i<track_ptr->NumberTrajectoryPoints(); i++) {

	track_light.add_vertex     (track_ptr->LocationAtPoint(i));

	track_light.add_direction  (track_ptr->DirectionAtPoint(i));

      }

      // Covariance
      for(size_t i=0; i<track_ptr->NumberCovariance(); i++)

	track_light.add_covariance (track_ptr->CovarianceAtPoint(i));

      // Momentum
      for(size_t i=0; i<track_ptr->NumberFitMomentum(); i++)

	track_light.add_momentum   (track_ptr->MomentumAtPoint(i));
     
      // Store this track
      data_ptr->add_track(track_light);

    }

    // Done
  }

  //#########################################################################
  void DataScanner::StoreUserInfo(const art::Event& evt, larlight::event_user* data_ptr)
  //###########################################################################
  {
    // This function does any necessary operation to store extra variables in user_info.

    larlight::user_info my_ui;

    // Block to fill isShower() cut variables
    if(_data_ptr[larlight::DATA::ShowerAngleCluster]) {

      for(auto const mod_name : _mod_names[larlight::DATA::ShowerAngleCluster]) {

	std::vector<const recob::Cluster*> clusterArray;
	try{
	  evt.getView(mod_name,clusterArray);
	}catch (art::Exception const& e) {
	  if (e.categoryCode() != art::errors::ProductNotFound ) throw;
	  continue;
	}
	//
	// Obtain input clusters
	//
	art::Handle< std::vector<recob::Cluster> > clusterListHandle;
	evt.getByLabel(mod_name,clusterListHandle);

	art::FindManyP<recob::Hit> fmh(clusterListHandle, evt, mod_name);
      
	for(size_t i=0; i < clusterListHandle->size(); i++){

	  art::Ptr<recob::Cluster> iCluster (clusterListHandle,i);
	  std::vector< art::Ptr<recob::Hit> > iHitList = fmh.at(i);

	  // Prepare output bector
	  double lineslope  = iCluster->dQdW();
	  double start_wire = iCluster->StartPos()[0];
	  double start_time = iCluster->StartPos()[1];
	  double end_wire   = iCluster->EndPos()[0];
	  double end_time   = iCluster->EndPos()[1];

	  // Do an equivalent calculation as ClusterParamsAlg::isShower()
	  
	  double length = TMath::Sqrt( pow((start_wire - end_wire),2) * CONV_WIRE2CM * 
				       pow((start_time - end_time),2) * CONV_TIME2CM );
	  double xangle = fCParamsAlg.Get2DAngleForHit(start_wire, start_time, iHitList);

	  if(xangle >  90) xangle -= 180;
	  if(xangle < -90) xangle += 180;

	  double HighBin,LowBin,invHighBin,invLowBin,altWeight=0.;
	  double PrincipalEigenvalue=1.,ModPV=-900;
	  double multihit=0; 
	  double ModHitDensity=0;
	  
	  // OffAxisHits calculation
	  fCParamsAlg.FindDirectionWeights(lineslope, 
					   start_wire, start_time,
					   end_wire, end_time,
					   iHitList,
					   HighBin, LowBin, invHighBin, invLowBin, &altWeight);
	  altWeight /= length;
	  
	  // Principal value calculation
	  TPrincipal pc(2,"D");
	  fCParamsAlg.GetPrincipal(iHitList,&pc);
	  PrincipalEigenvalue = (*pc.GetEigenValues())[0];
	  ModPV = TMath::Log(1-PrincipalEigenvalue);

	  // MultiHit calculation
	  multihit = fCParamsAlg.MultiHitWires(iHitList) / length;

	  // Modified hit density calculation
	  double HitDensity = iHitList.size()/length;
	  ModHitDensity = HitDensity - (1.82 * cosh(3.14/180*abs(xangle)-1.24) - 1.56);

	  // Store
	  my_ui.append("vModHitDensity",ModHitDensity);
	  my_ui.append("vMultiHit",multihit);
	  my_ui.append("vPrincipalHD",ModPV);
	  my_ui.append("vOffAxisHit",altWeight);
	} // end of cluster loop
      } // end of input module loop
    }

    // store user_info
    data_ptr->add_user_info(my_ui);
  }

} // namespace 

/** @} */ // end of doxygen group
