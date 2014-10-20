/**
 * \file Tree.hh
 *
 * \ingroup MCInfo
 * 
 * \brief Class def header for a class Tree
 *
 * @author David Caratelli
 */

/** \addtogroup MCInfo

    @{*/
#ifndef TREE_HH
#define TREE_HH

#include <iostream>
#include <vector>

/**
   \class Tree
   User defined class Tree ... these comments are used to generate
   doxygen documentation!
 */
class TreeNode{

public:

  /// Default Constructor
  TreeNode(){ _children.clear(); };

  /// constructor to use
  TreeNode(int index){ _index = index; _children.clear(); };

  /// Default destructor
  virtual ~TreeNode(){};

  /// Add a child to this noe
  void AddChild(TreeNode tn) { _children.push_back(tn); };

  /// Get children of this node
  std::vector<TreeNode> getChildren() { return _children; }

  /// Count number of nodes
  void countNodes(int& count);

  /// Get node index
  int getNodeIndex() { return _index; }

  /// Get closest brother (parent's child that is next in the list)
  //TreeNode* getClosestBrother();

  /// Get closest uncle (paren't sibling that is next in the list)
  //TreeNode* getClosestUncle();

  /// Is this node a leaf (i.e. the end of the line...no sub-nodes?)
  bool isLeaf() { if (_children.size() == 0) { return true; } return false; }

protected:

  //when using this class for MCparticles index should be TrackId
  int _index;

  bool _hasParent;

  std::vector<TreeNode> _children;

};

#endif
/** @} */ // end of doxygen group 

