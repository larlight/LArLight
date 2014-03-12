//
// Code for the Arachne Event Display
// Author: Nathaniel Tagg ntagg@otterbein.edu
// 
// Licence: this code is free for non-commertial use. Note other licences may apply to 3rd-party code.
// Any use of this code must include attribution to Nathaniel Tagg at Otterbein University, but otherwise 
// you're free to modify and use it as you like.
//

#include "TreeReader.h"
#include <TTree.h>
#include <TBranch.h>
#include <TLeaf.h>
#include <TTreeFormula.h>
#include <TError.h>
#include <TClass.h>
#include <sstream>
#include <iostream>

using std::string;
using std::vector;
using std::pair;
using std::map;

TreeReader::TreeReader(TTree* tree)
  : fTree(tree)
{
  fDefaultValue = -1;
}

TreeReader::~TreeReader()
{
}

Double_t TreeReader::getVal(TLeaf* leaf, int index, int second_index)
{
  if(!leaf) return fDefaultValue;
  
  int i = index;
  if(second_index>=0) {
    // Treat as a 2d array. Should be safe even if it's not.
    Int_t col_size = leaf->GetLenStatic();
    if(second_index >= col_size) { 
      Info("TreeReader::getVal","Requested 2nd dimension element out of bounds. Leafname:%s index:%d index2:%d StaticLen:%d Ndata:%d",leaf->GetName(),index,second_index,leaf->GetLenStatic(),leaf->GetNdata());      
    } else {
      i = second_index + col_size*index;
    }
  }
  if(i >= leaf->GetLen()) {
    Info("TreeReader::getVal","Requested element out of bounds. Leafname:%s index:%d index2:%d StaticLen:%d Len:%d Ndata:%d",leaf->GetName(),index,second_index,leaf->GetLenStatic(),leaf->GetLen(),leaf->GetNdata());
    return fDefaultValue;
  }
  return leaf->GetValue(i);
}

Double_t TreeReader::getVal(const string& leafname, int index, int second_index)
{
  TLeaf* leaf = fTree->GetLeaf(leafname.c_str());
  if(!leaf) {
    Info("TreeReader::getVal","tried to get value for nonexistant leaf named %s",leafname.c_str());
    return fDefaultValue;
  }
  return TreeReader::getVal(leaf,index,second_index);
}


string TreeReader::getStr(TLeaf* leaf, int index, int second_index)
{
  if(!leaf) return "Err";
  std::ostringstream out;
  string type = leaf->IsA()->GetName();
  if((type == string("TLeafD")) || (type == string("TLeafF"))) {
    out << getVal(leaf,index,second_index);
  } else {
    out << getInt(leaf,index,second_index);
  }
  return out.str();
}

  


Double_t TreeReader::getF(const std::string& formula, int index)
{
  TTreeFormula ttf("tff",formula.c_str(),fTree);
  if(ttf.GetNdata()<index) return fDefaultValue;
  return ttf.EvalInstance(index);
}

