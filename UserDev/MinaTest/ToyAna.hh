/**
 * \file ToyAna.hh
 *
 * \ingroup MinaTest
 * 
 * \brief Class def header for a class ToyAna
 *
 * @author kazuhiro
 */

/** \addtogroup MinaTest

    @{*/
#ifndef TOYANA_HH
#define TOYANA_HH

#include <iostream>
#include <TTree.h>
#include "ToyData.hh"

/**
   \class ToyAna
   User defined class ToyAna ... these comments are used to generate
   doxygen documentation!
*/
class ToyAna{

public:
  
  /// Default constructor
  ToyAna(){};
  
  /// Default destructor
  virtual ~ToyAna(){};

  void RunEventLoop(TTree* tree);

};

#endif
/** @} */ // end of doxygen group 

