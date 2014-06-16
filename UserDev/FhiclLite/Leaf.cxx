#ifndef LEAF_CC
#define LEAF_CC

#include "Leaf.hh"

namespace fhicl {

  Leaf::Leaf(std::string name, DataType_t  type)
    : std::vector<std::string>(),
      _name(name),
      _type(type)
    {
      if(_name.empty())

	throw FhiclLiteException("Must provide non-empty name!");

    }


}

#endif
