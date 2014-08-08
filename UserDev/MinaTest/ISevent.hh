/**
 * \file ISevent.hh
 *
 * \ingroup MinaTest
 * 
 * \brief Class def header for a class ISevent
 *
 * @author ehimwich
 */

/** \addtogroup MinaTest

    @{*/
#ifndef ISEVENT_HH
#define ISEVENT_HH

#include <iostream>
#include <TObject.h>
#include <vector>
#include "ISstep.hh"

/**
   \class ISevent
   User defined class ISevent ... these comments are used to generate
   doxygen documentation!
 */
class ISevent : public std::vector<ISstep> {

public:

  /// Default constructor
  ISevent() : std::vector<ISstep>() { clear(); };

  /// Default destructor
  virtual ~ISevent(){};

  void SetEventID(int id) { EventID = id;}
  int  fEventID() const { return EventID; }

  void clear() { EventID = -1; std::vector<ISstep>::clear(); }

private:
  
  int EventID;
  
  ClassDef(ISevent,1);

};

#endif
/** @} */ // end of doxygen group 

