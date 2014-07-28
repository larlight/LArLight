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

  void clear() {std::vector<ISstep>::clear(); }

  ClassDef(ISevent,1);

};

#endif
/** @} */ // end of doxygen group 

