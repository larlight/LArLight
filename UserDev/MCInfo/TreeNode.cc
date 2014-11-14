#ifndef TREE_CC
#define TREE_CC

#include "TreeNode.hh"

void TreeNode::countNodes(int& count){

  count += 1;
  for (size_t i=0; i < _children.size(); i++)
    _children.at(i).countNodes(count);

  return;
}

#endif
