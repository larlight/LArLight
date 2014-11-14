/**
 * \file TreeTop.hh
 *
 * \ingroup MCInfo
 * 
 * @author David Caratelli
 */

/** \addtogroup MCInfo

    @{*/
#ifndef TREETOP_HH
#define TREETOP_HH

#include <iostream>
#include "TreeNode.hh"

/**
   \class TreeTop
   User defined class TreeTop ... these comments are used to generate
   doxygen documentation!
 */
class TreeTop : public TreeNode {
  
public:

  TreeTop() : TreeNode() {};

  TreeTop(int index) : TreeNode() { _index = index; };

  
private:
  
};

#endif
/** @} */ // end of doxygen group 

