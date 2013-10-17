
#ifndef DataScanner_H
#define DataScanner_H

// LArSoft includes
#include "Geometry/Geometry.h"
#include "RecoBase/Track.h"

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

    /// Function to read & store Tracking information
    void ReadTrack(const art::Event& evt, const std::string mod_name, event_track* data_ptr);

    /// Function to read & store MCTruth information
    void ReadMCTruth(const art::Event& evt, const std::string mod_name, event_mc* data_ptr);

    std::vector<TTree*>      _trees;     ///< output data holder TTree
    std::vector<std::string> _mod_names; ///< input data production module names
    std::vector<data_base*>  _data_ptr;  ///< output data holder class object pointers
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
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art/Framework/Services/Optional/TFileService.h"
#include "art/Framework/Services/Optional/TFileDirectory.h"
#include "messagefacility/MessageLogger/MessageLogger.h"


namespace larlight {

  //#######################################################################################################
  DataScanner::DataScanner(fhicl::ParameterSet const& pset) : _trees(DATA::DATA_TYPE_MAX,0), 
							      _mod_names(DATA::DATA_TYPE_MAX,""),
							      _data_ptr(DATA::DATA_TYPE_MAX,0)
  //#######################################################################################################
  {
    // Obtain module names for input data
    // If a user set an empty string for these params, they are ignored for processing.
    _mod_names[DATA::Bezier]      = pset.get<std::string>("fModName_Bezier");
    _mod_names[DATA::Kalman3DSPS] = pset.get<std::string>("fModName_Kalman3DSPS");
    _mod_names[DATA::MCTruth]     = pset.get<std::string>("fModName_MCTruth");

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
	case DATA::Event:
	case DATA::UserInfo:
	case DATA::FIFOChannel:
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
	ReadTrack(evt, _mod_names[i], (event_track*)(_data_ptr[i]));
	break;
	// Data type to be stored in event_mc class
      case DATA::MCTruth:
	ReadMCTruth(evt, _mod_names[i], (event_mc*)(_data_ptr[i]));
	break;
      case DATA::Event:
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
  void DataScanner::ReadMCTruth(const art::Event& evt, const std::string mod_name, event_mc* data_ptr){
  //#######################################################################################################
    // EMPTY ... place holder

  }


  //#######################################################################################################
  void DataScanner::ReadTrack(const art::Event& evt, const std::string mod_name, event_track* data_ptr){
  //#######################################################################################################

    // Obtain recob::Track object produced by the specified module name (mod_name)
    art::Handle< std::vector< recob::Track > > track_handle;
    evt.getByLabel(mod_name, track_handle);

    for(size_t i=0; i < track_handle->size(); ++i){

      // Obtain recob::Track object pointer
      art::Ptr<recob::Track> track_ptr(track_handle,i);

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


