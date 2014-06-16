/**
 * \file Node.hh
 *
 * \ingroup FhiclLite
 * 
 * \brief Class def header for a class Node
 *
 * @author kazuhiro
 */

/** \addtogroup FhiclLite

    @{*/
#ifndef NODE_HH
#define NODE_HH

#include <iostream>
#include <map>
#include "Leaf.hh"

namespace fhicl {
  
  /**
     \class Node
     User defined class Node ... these comments are used to generate
     doxygen documentation!
  */
  class Node{
    
  public:
    
    /// Default constructor
    Node(std::string name="");
    
    /// Default destructor
    virtual ~Node(){};

    const std::string& name() const { return _name; }

    void Register(const ::fhicl::Node &node);

    void Register(const ::fhicl::Leaf &leaf);

  protected:

    std::string _name;
    std::map<std::string, std::pair<fhicl::DataType_t,size_t> > _register;
    std::vector<fhicl::Leaf> _leaf;
    std::vector<fhicl::Node> _node;
  };
}

#endif
/** @} */ // end of doxygen group 

