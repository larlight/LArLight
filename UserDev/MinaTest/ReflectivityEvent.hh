/**
 * \file ReflectivityEvent.hh
 *
 * \ingroup MinaTest
 * 
 * \brief Class def header for a class ReflectivityEvent
 *
 * @author ehimwich
 */

/** \addtogroup MinaTest

    @{*/
#ifndef REFLECTIVITYEVENT_HH
#define REFLECTIVITYEVENT_HH

#include <iostream>
#include <vector>
#include "ReflectivityStep.hh"

/**
   \class ReflectivityEvent
   User defined class ReflectivityEvent ... these comments are used to generate
   doxygen documentation!
 */
class ReflectivityEvent : public std::vector<ReflectivityStep>{

public:

  /// Default constructor
  ReflectivityEvent() : std::vector<ReflectivityStep>() { clear(); };

  /// Default destructor
  virtual ~ReflectivityEvent(){};

  void SetEventID(int id) {EventID = id;}
  int fEventID() const {return EventID;}
  
  void clear() {EventID = -1; std::vector<ReflectivityStep>::clear(); }
  
private:

  int EventID;

  ClassDef(ReflectivityEvent,1);

};

#endif
/** @} */ // end of doxygen group 

