/**
 * \file event.hh
 *
 * \ingroup SimpleTreeData
 * 
 * @author Kazu - Nevis 2013
 */

/** \addtogroup SimpleTreeData

    @{*/

#ifndef EVENT_HH
#define EVENT_HH

#include "data_base.hh"
/**
   \class event
   LArSoft Event class equivalent data container
*/
class event : public data_base {

public:

  /// Default constructor
  event();

  /// Default destructor
  virtual ~event(){};

  /**
     Implementation data_base::clear_event. 
     When the argument boolean "all" is set to true, clear ALL C-array data elements to the default value.
     When "all" is set to false (default), only filled entries are cleared.
     This way we save time to loop over array elements.
  */
  virtual void clear_event(bool all=false);

  /// Implementation of track data address setter
  virtual Bool_t set_address(TTree* t,Bool_t create=false);

  /// Method to add an event
  void set_event(UInt_t eventID, UInt_t runID, UInt_t subrunID,
		 Bool_t isdata,
		 Double_t triggerT, Double_t beamT,
		 Double_t pot, Double_t etau);

  //--- Getter methods ---//
  UInt_t   eventID()  const { return _eventID;  };
  UInt_t   runID()    const { return _runID;    };
  UInt_t   subrunID() const { return _subrunID; };
  Double_t triggerT() const { return _triggerT; };
  Double_t beamT()    const { return _beamT;    };
  Double_t pot()      const { return _pot;      };
  Double_t etau()     const { return _etau;     };
  Bool_t   isdata()   const { return _isdata;   };

protected:
  
  UInt_t   _eventID;  ///< Event counter 
  UInt_t   _runID;    ///< Run number
  UInt_t   _subrunID; ///< Sub-Run number
  Double_t _triggerT; ///< Trigger time in second
  Double_t _beamT;    ///< Beam time in second
  Double_t _pot;      ///< Protons on target
  Double_t _etau;     ///< Electron drift lifetime
  Bool_t   _isdata;   ///< Flag: MC=0, DATA=1

};

#endif

/** @} */ // end of doxygen group 
