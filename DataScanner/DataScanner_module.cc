
#ifndef DataScanner_H
#define DataScanner_H

// LArSoft includes
#include "Geometry/Geometry.h"
#include "RecoBase/Track.h"
#include "RecoBase/SpacePoint.h"
#include "SimulationBase/MCTruth.h"

// ART includes.
#include "art/Framework/Core/EDAnalyzer.h"

// LArLight includes
#include <AnaProcess/Base/Base-TypeDef.hh>
#include <AnaProcess/DataFormat/DataFormat-TypeDef.hh>

namespace larlight {
 
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

    /// Function to read & store spacepoints
    void ReadSPS(const art::Event& evt, const std::string mod_name, event_sps* data_ptr);

    /// Function to read & store Tracking information
    void ReadTrack(const art::Event& evt, const std::string mod_name, event_track* data_ptr);

    /// Function to read & store MCTruth information
    void ReadMCTruth(const art::Event& evt, const std::string mod_name, event_mc* data_ptr);

    /// Function to read & store MCTruth information
    void ReadMCPartArray(const art::Event& evt, const std::string mod_name, event_mc* data_ptr);

    /// Utility function to parse module name string
    void ParseModuleName(std::vector<std::string> &mod_names, std::string name);
    
    std::vector<TTree*>                    _trees;     ///< output data holder TTree
    std::vector<std::vector<std::string> > _mod_names; ///< input data production module names input from FCL file
    std::vector<data_base*>                _data_ptr;  ///< output data holder class object pointers
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
  DataScanner::DataScanner(fhicl::ParameterSet const& pset) : _trees(DATA::DATA_TYPE_MAX,0), 
							      _data_ptr(DATA::DATA_TYPE_MAX,0)
  //#######################################################################################################
  {
    // Initialize module name container
    for(size_t i=0; i<(size_t)(DATA::DATA_TYPE_MAX); i++)
      
      _mod_names.push_back(std::vector<std::string>());

    // Obtain module names for input data
    // If a user set an empty string for these params, they are ignored for processing.
    ParseModuleName(_mod_names[DATA::Bezier],      pset.get<std::string>("fModName_Bezier"));
    ParseModuleName(_mod_names[DATA::Kalman3DSPS], pset.get<std::string>("fModName_Kalman3DSPS"));
    ParseModuleName(_mod_names[DATA::MCTruth],     pset.get<std::string>("fModName_MCTruth"));
    ParseModuleName(_mod_names[DATA::SpacePoint],  pset.get<std::string>("fModName_SpacePoint"));

    // Next we make TTree & storage data class objects for those data types specified in fcl files.
    art::ServiceHandle<art::TFileService> fileService;    
    for(size_t i=0; i<(int)(DATA::DATA_TYPE_MAX); i++){

      DATA::DATA_TYPE type = (DATA::DATA_TYPE)i;
      
      // Check if a user provided an input module name for this data type.
      if(_mod_names[i].size()){

	// Create TTree
	_trees[i] = fileService->make<TTree>(Form("%s_tree",DATA::DATA_TREE_NAME[i].c_str()),"");

	// Next, create data class objects
	switch(type){
	case DATA::MCTruth:
	  _data_ptr[i]=(data_base*)(new event_mc);
	  break;
	case DATA::Track:
	case DATA::Kalman3DSPS:
	case DATA::Bezier:
	  _data_ptr[i]=(data_base*)(new event_track);
	  break;
	case DATA::SpacePoint:
	  _data_ptr[i]=(data_base*)(new event_sps);
	  break;
	case DATA::Event:
	case DATA::UserInfo:
	case DATA::FIFOChannel:
	case DATA::Seed:
	case DATA::Shower:
	case DATA::Calorimetry:
	case DATA::Wire:
	case DATA::Hit:
	case DATA::Cluster:
	case DATA::DATA_TYPE_MAX:
	  mf::LogError("DataScanner")<<Form("Data type %d not supported!",type);
	  break;
	}
	// Set TTree branch to the created data class object's address
	_trees[i]->Branch(Form("%s_branch",DATA::DATA_TREE_NAME[i].c_str()),
			  _data_ptr[i]->GetName(),
			  &(_data_ptr[i]));
	
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
      
      // If TTree pointer is not set, we don't have to fill this data type
      if(!(_trees[i])) continue;
      
      // Reset data
      _data_ptr[i]->clear_data();

      // Fill common variables such as run, subrun, and event id
      _data_ptr[i]->set_run      ( evt.id().run()    );
      _data_ptr[i]->set_subrun   ( evt.id().subRun() );
      _data_ptr[i]->set_event_id ( evt.id().event()  );

      // Reaching this point means we want to fill this data type.
      // Handle different kind of data (class wise)
      DATA::DATA_TYPE type = (DATA::DATA_TYPE)i;
      switch(type){
	// Data types to be stored in event_track class
      case DATA::Track:
      case DATA::Kalman3DSPS:
      case DATA::Bezier:
	for(size_t j=0; j<_mod_names[i].size(); ++j)
	  ReadTrack(evt, _mod_names[i][j], (event_track*)(_data_ptr[i]));
	break;
	// Data type to be stored in event_mc class
      case DATA::MCTruth:
	for(size_t j=0; j<_mod_names[i].size(); ++j){
	  ReadMCTruth     (evt, _mod_names[i][j], (event_mc*)(_data_ptr[i]));
	  ReadMCPartArray (evt, _mod_names[i][j], (event_mc*)(_data_ptr[i]));
	}
	break;
 	// Data type to be stored in event_sps class
      case DATA::SpacePoint:
	for(size_t j=0; j<_mod_names[i].size(); ++j)
	  ReadSPS(evt,_mod_names[i][j], (event_sps*)(_data_ptr[i]));
	break;
      case DATA::Event:
      case DATA::Seed:
      case DATA::UserInfo:
      case DATA::FIFOChannel:
      case DATA::Shower:
      case DATA::Calorimetry:
      case DATA::Wire:
      case DATA::Hit:
      case DATA::Cluster:
      case DATA::DATA_TYPE_MAX:
	break;
      }
      // Fill this TTree
      _trees[i]->Fill();
    }

  }

  //#######################################################################################################
  void DataScanner::ReadSPS(const art::Event& evt, const std::string mod_name, event_sps* data_ptr){
  //#######################################################################################################

    std::vector<const recob::SpacePoint* > spsArray;
    try {

      evt.getView(mod_name,spsArray);

    }catch (art::Exception const& e) {

      if (e.categoryCode() != art::errors::ProductNotFound ) throw;

    }

    for(size_t i=0; i<spsArray.size(); ++i){

      const recob::SpacePoint* sps_ptr(spsArray.at(i));
      
      spacepoint sps_light(sps_ptr->ID(),
			   sps_ptr->XYZ()[0],    sps_ptr->XYZ()[1],    sps_ptr->XYZ()[2],
			   sps_ptr->ErrXYZ()[0], sps_ptr->ErrXYZ()[1], sps_ptr->ErrXYZ()[2],
			   sps_ptr->Chisq());
      data_ptr->add_sps(sps_light);

    }

  }

  //#######################################################################################################
  void DataScanner::ReadMCPartArray(const art::Event& evt, const std::string mod_name, event_mc* data_ptr){
  //#######################################################################################################
    
    std::vector<const simb::MCParticle*> mciArray;

    try {

      evt.getView(mod_name,mciArray);

    }catch (art::Exception const& e) {

      if (e.categoryCode() != art::errors::ProductNotFound ) throw;

    }
    
    for(size_t i=0; i < mciArray.size(); i++){

      const simb::MCParticle* part(mciArray.at(i));

      part_mc part_light(part->PdgCode(), part->TrackId(), part->Mother(), part->Process());
      
      for(size_t k=0; k<part->NumberTrajectoryPoints(); k++)
	
	part_light.add_track(part->Vx(k), part->Vy(k), part->Vz(k), part->T(k),
			     part->Px(k), part->Py(k), part->Pz(k));
      
      for(size_t k=0; k<(size_t)(part->NumberDaughters()); k++)
	
	part_light.add_daughter(part->Daughter(k));
      
      data_ptr->add_part(part_light);
      
    }
  }

  //#######################################################################################################
  void DataScanner::ReadMCTruth(const art::Event& evt, const std::string mod_name, event_mc* data_ptr){
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

      data_ptr->set_gen_code( (MC::Origin_t) mci_ptr->Origin() );

      for(size_t j=0; j < (size_t)(mci_ptr->NParticles()); ++j){
	
	const simb::MCParticle part(mci_ptr->GetParticle(j));
	part_mc part_light(part.PdgCode(), part.TrackId(), part.Mother(), part.Process());

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
  void DataScanner::ReadTrack(const art::Event& evt, const std::string mod_name, event_track* data_ptr){
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
      track track_light;

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

} // namespace opdet


