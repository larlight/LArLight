/**
 * \file KalekoAna.hh
 *
 * \ingroup KalekoAna
 * 
 * \brief Class def header for a class KalekoAna
 *
 * @author davidkaleko
 */

/** \addtogroup KalekoAna

    @{*/
#ifndef KALEKOANA_HH
#define KALEKOANA_HH

#include <iostream>

/**
   \class KalekoAna
   User defined class KalekoAna ... these comments are used to generate
   doxygen documentation!
 */
class KalekoAna{

public:

  /// Default constructor
  KalekoAna(){};

  /// Default destructor
  virtual ~KalekoAna(){};

  /// Test function
  void testfn();

  int testfn2(std::string filename);

};

#endif
/** @} */ // end of doxygen group 

