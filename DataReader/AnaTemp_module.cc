
#ifndef DataReader_H
#define DataReader_H

// LArSoft includes
#include "Geometry/Geometry.h"

// ART includes.
#include "art/Framework/Core/EDAnalyzer.h"

// LArLight includes
#include <AnaProcess/Base/Base-TypeDef.hh>
#include <AnaProcess/DataFormat/DataFormat-TypeDef.hh>

namespace larlight {
 
  class DataReader : public art::EDAnalyzer{
  public:
 
    DataReader(const fhicl::ParameterSet&);
    virtual ~DataReader();

    void beginJob();

    void analyze (const art::Event&); 

  private:

    void ReadTrack(const art::Event& evt, const std::string mod_name, event_track* data_ptr);
    void ReadMCTruth(const art::Event& evt, const std::string mod_name, event_mc* data_ptr);

    std::vector<TTree*>      _trees;
    std::vector<std::string> _mod_names;
    std::vector<data_base*>  _data_ptr;
  };

} 

#endif//  DataReader_H

// DataReader.cc

// Framework includes
#include "art/Framework/Core/ModuleMacros.h"

namespace larlight {
  DEFINE_ART_MODULE(DataReader);
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

  //-----------------------------------------------------------------------
  // Constructor
  DataReader::DataReader(fhicl::ParameterSet const& pset) : _trees(DATA::DATA_TYPE_MAX,0), 
							    _mod_names(DATA::DATA_TYPE_MAX,""),
							    _data_ptr(DATA::DATA_TYPE_MAX,0)
  {
    _mod_names[DATA::Bezier]      = pset.get<std::string>("fModName_Bezier");
    _mod_names[DATA::Kalman3DSPS] = pset.get<std::string>("fModName_Kalman3DSPS");

    art::ServiceHandle<art::TFileService> fileService;    
    for(size_t i=0; i<(int)(DATA::DATA_TYPE_MAX); i++){

      DATA::DATA_TYPE type = (DATA::DATA_TYPE)i;

      if(_mod_names[i].size()){
	
	_trees[i] = fileService->make<TTree>(Form("%s_tree",DATA::DATA_TREE_NAME[i].c_str()),"");
	
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
	  mf::LogError("DataReader")<<Form("Data type %d not supported!",type);
	  break;
	}
	_tree[i]->Branch(Form("%s",DATA::DATA_TREE_NAME[i].c_str()),
			 _data_ptr[i]->GetName(),
			 &(_data_ptr[i]));
	
      }
    }

  }

  //-----------------------------------------------------------------------
  // Destructor
  DataReader::~DataReader(){};
   
  //-----------------------------------------------------------------------
  void DataReader::beginJob(){};
   

  //-----------------------------------------------------------------------
  void DataReader::analyze(const art::Event& evt) 
  {
    for(size_t i=0; i<(size_t)(DATA::DATA_TYPE_MAX); i++){

      if(!(_trees[i])) continue;

      DATA::DATA_TYPE type = (DATA::DATA_TYPE)i;
      switch(type){
      case DATA::Track:
      case DATA::Kalman3DSPS:
      case DATA::Bezier:
	ReadTrack(evt, _mod_names[i], (event_track*)(_data_ptr[i]));
	break;
      case DATA::MCTruth:
	ReadMC(evt, _mod_names[i], (event_mc*)(_data_ptr[i]));
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
      _tree[i]->Fill();
    }

  }

  //-----------------------------------------------------------------------
  void DataReader::ReadTrack(const art::Event& evt, const std::string mod_name, event_track* data_ptr){
    
    //art::Handle< std::vector< simb::MCTruth > > mcHandle;
    //event.getByLabel(fMCProducerLabel, mcHandle);

    art::Handle< std::vector< recob::Track > > recoTrackHandle;
    event.getByLabel(fTrackProducerLabel, recoTrackHandle);

  }

} // namespace opdet


