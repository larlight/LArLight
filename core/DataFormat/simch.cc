#ifndef SIMCH_CC
#define SIMCH_CC

#include "simch.hh"

namespace larlight {

  //--------------------------------------------------
  void ide::clear_data()
  //--------------------------------------------------
  {
    TObject::Clear();
    trackID      = DATA::INVALID_INT;
    numElectrons = DATA::INVALID_DOUBLE;
    energy       = DATA::INVALID_DOUBLE;

    x = DATA::INVALID_DOUBLE;
    y = DATA::INVALID_DOUBLE;
    z = DATA::INVALID_DOUBLE;
  }

  //--------------------------------------------------
  simch::simch(DATA::DATA_TYPE type) : data_base(type)
  //--------------------------------------------------
  {
    if(_type != DATA::SimChannel) {
      
      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));
      
      _type=DATA::SimChannel;
    }
    
    clear_data();
    
  }

  //--------------------------------------------------
  void simch::clear_data() 
  //--------------------------------------------------
  {
    data_base::clear_data();
    fChannel = DATA::INVALID_INT;
    fTDCIDEs.clear();
  }

  //--------------------------------------------------------------------------
  std::vector<larlight::ide> simch::TrackIDsAndEnergies(UShort_t startTDC,
							UShort_t endTDC) const
  //--------------------------------------------------------------------------
  {
    // make a map of track ID values to larlight::ide objects
    std::map<UInt_t, larlight::ide> idToIDE;

    std::vector<larlight::ide> ides;
    if(startTDC > endTDC) return ides;

    std::map<UShort_t, std::vector<larlight::ide> >::const_iterator mitr;
    std::map<UShort_t, std::vector<larlight::ide> >::const_iterator start = fTDCIDEs.lower_bound(startTDC);
    std::map<UShort_t, std::vector<larlight::ide> >::const_iterator end   = fTDCIDEs.upper_bound(endTDC);

    for(mitr = start; mitr != end; mitr++){

      // grab the vector of IDEs for this tdc
      const std::vector<larlight::ide> &idelist = (*mitr).second;
      std::vector<larlight::ide>::const_iterator itr = idelist.begin();
      // now loop over them and add their content to the map
      while( itr != idelist.end() ){

	if( idToIDE.find((*itr).trackID) != idToIDE.end() ){
          Double_t nel1   = idToIDE[(*itr).trackID].numElectrons;
          Double_t nel2   = (*itr).numElectrons;
          Double_t en1    = idToIDE[(*itr).trackID].energy;
          Double_t en2    = (*itr).energy;
          Double_t energy = en1+en2;
          Double_t weight = nel1 + nel2;
          // make a weighted average for the location information
          idToIDE[(*itr).trackID].x            = ((*itr).x*nel2 + idToIDE[(*itr).trackID].x*nel1)/weight;
          idToIDE[(*itr).trackID].y            = ((*itr).y*nel2 + idToIDE[(*itr).trackID].y*nel1)/weight;
          idToIDE[(*itr).trackID].z            = ((*itr).z*nel2 + idToIDE[(*itr).trackID].z*nel1)/weight;
          idToIDE[(*itr).trackID].numElectrons = weight;
          idToIDE[(*itr).trackID].energy = energy;
        } // end if the track id for this one is found
        else{
          larlight::ide temp(*itr);
          idToIDE[(*itr).trackID] = temp;
        }

	itr++;
      } // end loop over vector
    } // end loop over tdc values


    ides.reserve(idToIDE.size());
    // now fill the vector with the ides from the map
    for(auto itr = idToIDE.begin(); itr != idToIDE.end(); itr++)

      ides.push_back((*itr).second);


    return ides;
  }
    
  //-----------------------------------------------------------------------------
  Double_t simch::Charge(UShort_t tdc) const
  //-----------------------------------------------------------------------------
  {
    auto const tdc_itr = fTDCIDEs.find(tdc);
    if(tdc_itr == fTDCIDEs.end()) return 0;

    Double_t charge = 0;
    for(auto const this_ide : (*tdc_itr).second)

      charge += this_ide.numElectrons;
    
    return charge;
  }

  //-----------------------------------------------------------------------------
  Double_t simch::Energy(UShort_t tdc) const
  //-----------------------------------------------------------------------------
  {
    auto const tdc_itr = fTDCIDEs.find(tdc);
    if(tdc_itr == fTDCIDEs.end()) return 0;

    Double_t energy = 0;
    for(auto const this_ide : (*tdc_itr).second)

      energy += this_ide.energy;
    
    return energy;
  }

  //-----------------------------------------------------------------------------
  void simch::add_ide(UShort_t tdc, ide in)
  //-----------------------------------------------------------------------------
  {
    auto tdc_itr = fTDCIDEs.find(tdc);

    if(tdc_itr == fTDCIDEs.end()) {
      // case 1: new tdc
      fTDCIDEs.insert(std::pair<UShort_t,std::vector<larlight::ide> >(tdc,std::vector<larlight::ide>(1,in)));
      return;
    }

    // case 2: combining w/ existing ide from the same track id
    for(auto i_ide : (*tdc_itr).second) {
      
      if(i_ide.trackID == in.trackID) {
	
	Double_t weight = i_ide.numElectrons + in.numElectrons;

	i_ide.x = (i_ide.x * i_ide.numElectrons + in.x * in.numElectrons)/weight;
	i_ide.y = (i_ide.y * i_ide.numElectrons + in.y * in.numElectrons)/weight;
	i_ide.z = (i_ide.z * i_ide.numElectrons + in.z * in.numElectrons)/weight;
	i_ide.numElectrons = weight;
	i_ide.energy += in.energy;

	return;
      }

    }
    
    // case 3: inserting this ide as the 1st one from this track id
    (*tdc_itr).second.push_back(in);
    return;

  }


  //------------------------------------------------------------------------------
  event_simch::event_simch(DATA::DATA_TYPE type) : std::vector<larlight::simch>(),
						   event_base(type)
  //------------------------------------------------------------------------------
  {
    if(_type != DATA::SimChannel) {
      
      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));
      
      _type=DATA::SimChannel;
    }
    
    clear_data();
    
  }

}
#endif
