/**
 * \file radiation_information.hh
 *
 * \ingroup ArianaPi0Test
 * 
 * \Fill some information that can change from detector to detector
 *
 * @author ah673
 */

/** \addtogroup ArianaPi0Test

    @{*/
#ifndef RADIATION_INFORMATION_HH
#define RADIATION_INFORMATION_HH

#include <iostream>
#include "Geometry.hh"

/**
   \class radiation_information
   User defined class radiation_information ... these comments are used to generate
   doxygen documentation!
 */


class radiation_information{

public:

  radiation_information(){} ; //_detWidth = 256.35; _detLength = 1036.8 ; _detHalfHeight = 116.5; };

  virtual ~radiation_information(){};

  //Get information about detector boundaries 

	
//	const larutil::Geometry * geo = larutil::Geometry::GetME() ; 

//  double DetWidth() { return larutil::Geometry::GetME()->DetHalfWidth() ; }
  //double DetLength() { return larutil::Geometry::GetME()->DetLength() ; }
 // double DetHalfHeight() { return larutil::Geometry::GetME()->DetHalfHeight() ; }
protected:
  
//	double _detWidth ; 
//	double _detLength ; 
//	double _detHalfHeight ;
 

};

#endif
/** @} */ // end of doxygen group 

