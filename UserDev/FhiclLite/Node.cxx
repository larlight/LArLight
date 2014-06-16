#ifndef NODE_CXX
#define NODE_CXX

#include "Node.hh"

namespace fhicl {

  Node::Node(std::string name) : _name(name),
				 _register(),
				 _leaf(),
				 _node()
  {
    if(_name.empty())

      throw FhiclLiteException("Must provide non-empty name!");
  }

  void Node::Register(const ::fhicl::Node &node)
  {
    if(_register.find(node.name()) != _register.end())

      throw FhiclLiteException(Form("Name %s already registered!", node.name().c_str()));
    
    const std::pair<fhicl::DataType_t,size_t> value(kNode,_node.size());

    const std::pair<std::string,std::pair<fhicl::DataType_t,size_t> > key_value(node.name(),value);

    _register.insert(key_value);

    _node.push_back(node);

  }
  
  void Node::Register(const ::fhicl::Leaf &leaf)
  {

    if(_register.find(leaf.name()) != _register.end())

      throw FhiclLiteException(Form("Name %s already registered!", leaf.name().c_str()));

    const std::pair<fhicl::DataType_t,size_t> value(leaf.type(),_leaf.size());

    const std::pair<std::string,std::pair<fhicl::DataType_t,size_t> > key_value(leaf.name(),value);

    _register.insert(key_value);

    _leaf.push_back(leaf);

  }



}

#endif
