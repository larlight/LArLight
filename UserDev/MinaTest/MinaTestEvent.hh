/**
 * \file MinaTest.hh
 *
 * \ingroup MinaTest
 * 
 * \brief Class def header for a class MinaTest
 *
 * @author mina
 */

/** \addtogroup MinaTest

    @{*/
#ifndef MINATESTEVENT_HH
#define MINATESTEVENT_HH

#include <iostream>
#include <TObject.h>
#include <vector>
#include <climits>
#include <limits>
#include "MinaTestParticle.hh"
#include "MinaTestPhoton.hh"

/**
   \class MinaTest
   User defined class MinaTest ... these comments are used to generate
   doxygen documentation!
 */
class MinaTestEvent : public std::vector<MinaTestParticle> {

public:
  
  //Constructor
  MinaTestEvent() : std::vector<MinaTestParticle>() { clear(); };

  //Destructor
  virtual ~MinaTestEvent(){};
  
  //Setter for event 
  void SetEvent(int EventID, int run) {Evt = EventID; Run = run;}
  int GetEvent() const {return Evt;}
  int GetRun() const {return Run;}

  void clear() { Evt = 0; Run = 0; std::vector<MinaTestParticle>::clear(); }

protected:
  
  int Evt;
  int Run;
  
  ClassDef(MinaTestEvent,1);

};


#endif
/** @} */ // end of doxygen group 

