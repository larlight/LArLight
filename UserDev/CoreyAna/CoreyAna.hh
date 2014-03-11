/**
 * \file CoreyAna.hh
 *
 * \ingroup CoreyAna
 * 
 * \brief Class def header for a class CoreyAna
 *
 * @author cja33
 */

/** \addtogroup CoreyAna

    @{*/
#ifndef COREYANA_HH
#define COREYANA_HH

#include <iostream>

/**
   \class CoreyAna
   User defined class CoreyAna ... these comments are used to generate
   doxygen documentation!
 */
class CoreyAna{

public:

  /// Default constructor
  CoreyAna(){};

  /// Default destructor
  virtual ~CoreyAna(){};
  /// Hello world!
  void HelloWorld() { std::cout << "Hello World!" << std::endl; }
};

#endif
/** @} */ // end of doxygen group 

