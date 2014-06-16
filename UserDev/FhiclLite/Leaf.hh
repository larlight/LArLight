/**
 * \file Leaf.hh
 *
 * \ingroup FhiclLite
 * 
 * \brief Class def header for a class Leaf
 *
 * @author kazuhiro
 */

/** \addtogroup FhiclLite

    @{*/
#ifndef LEAF_HH
#define LEAF_HH

// stl
#include <string>
#include <iostream>
#include <vector>
// ROOT
#include <TString.h>
// FhiclLite
#include "FhiclLiteConstants.hh"
#include "FhiclLiteException.hh"

namespace fhicl {
  /**
     \class Leaf
     User defined class Leaf ... these comments are used to generate
     doxygen documentation!
  */
  class Leaf : public std::vector<std::string> {
    
  public:
    
    /// Default constructor
    Leaf(std::string name="",
	 DataType_t type=kSingle);
    
    /// Default destructor
    virtual ~Leaf(){};

    const DataType_t type() const { return _type; }

    const std::string& name() const { return _name; }

  protected:

    std::string _name;

    DataType_t _type;

  };
}

#endif
/** @} */ // end of doxygen group 

