/**
 * \file AnaPMT_module.cc
 * \ingroup Example
 *
 * \brief Class definition file of AnaPMT
 * @author kterao
 *
*/

/** \addtogroup Example
@{*/

#ifndef ANAPMT_H
#define ANAPMT_H

// LArSoft includes
#include "Geometry/Geometry.h"
#include "OpticalDetectorData/FIFOChannel.h"

// ART includes.
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "art/Framework/Services/Optional/TFileDirectory.h"
#include "art/Framework/Services/Optional/TFileService.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Persistency/Common/Ptr.h"
#include "art/Persistency/Common/PtrVector.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

namespace example {

  /**
     \class AnaPMT
     An example module to demonstrate how one can access data in LArSoft.
     This example accesses FIFOChannel data product which is PMT optical
     readout simulation output. Ultimately more complete example of various
     data member access method can be found in DataScanner module.
  */ 
 
  class AnaPMT : public art::EDAnalyzer{
  public:
 
    /// Default constructor
    AnaPMT(const fhicl::ParameterSet&);

    /// Default destructor
    virtual ~AnaPMT();

    /// Method to be called before the event loop
    void beginJob();

    /// Method to be called event-by-event
    void analyze (const art::Event&); 

  private:
    /// Input data product module name
    std::string _name_opt_module;

  };

} 

#endif//  ANAPMT_H

// AnaPMT.cc

namespace example {
  DEFINE_ART_MODULE(AnaPMT);
}


namespace example {

  //-----------------------------------------------------------------------
  // Constructor
  AnaPMT::AnaPMT(fhicl::ParameterSet const& pset) : art::EDAnalyzer(), _name_opt_module()
  {

    _name_opt_module     = pset.get< std::string          >("ModuleName_PMT");
    
  }

  //-----------------------------------------------------------------------
  // Destructor
  AnaPMT::~AnaPMT(){};
   
  //-----------------------------------------------------------------------
  void AnaPMT::beginJob(){};
   

  //-----------------------------------------------------------------------
  void AnaPMT::analyze(const art::Event& evt) 
  {

    //
    // Read in FIFOChannel array ... handle the case if there's no FIFOChannel
    //
    std::vector<const optdata::FIFOChannel*> pmtFIFOArray;
    try {
      evt.getView(_name_opt_module, pmtFIFOArray);
    }
    catch ( art::Exception const& err ) {
      if ( err.categoryCode() != art::errors::ProductNotFound ) throw;
    }
    
    for(size_t index=0; index < pmtFIFOArray.size(); ++index){
      
      const optdata::FIFOChannel* fifo(pmtFIFOArray.at(index));
      
      mf::LogWarning("AnaPMT")<<Form("Channel %d found with %zu ADC counts...",fifo->ChannelNumber(),fifo->size());

    }

    
  }
} // namespace example

/** @} */ // end of doxygen group
