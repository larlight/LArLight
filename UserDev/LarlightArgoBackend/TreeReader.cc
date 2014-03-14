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

JsonElement TreeReader::getJson(const std::string& leafname, int index, int second_index)
{  
  TLeaf* leaf = fTree->GetLeaf(leafname.c_str());
  return getJson(leaf,index,second_index);
}
  
JsonElement TreeReader::getJson(TLeaf* leaf, int index, int second_index)
{
  if(!leaf) return JsonElement(); // null element.

  Double_t v = getVal(leaf,index,second_index);
  
  string type = leaf->GetTypeName();
  if((type == "Double_t") || (type == "Float_t") )
    return JsonElement(v);

  return JsonElement((Int_t)v);
}

Double_t TreeReader::getF(const std::string& formula, int index)
{
  TTreeFormula ttf("tff",formula.c_str(),fTree);
  if(ttf.GetNdata()<index) return fDefaultValue;
  return ttf.EvalInstance(index);
}

JsonElement TreeReader::jsonF(const std::string& formula, int index)
{
  TTreeFormula ttf("tff",formula.c_str(),fTree);
  if(ttf.GetNdata()<index) return JsonElement(); // null value!
  Double_t v = ttf.EvalInstance(index);
  if(ttf.IsInteger()) return JsonElement((int)v);
  return JsonElement(v);
}


// Make array of objects using formulae.


std::vector<JsonObject> TreeReader::makeVector(const vector<pair< string,string> >& key_leaf_pairs)
{ 
  std::vector<JsonObject> retval; 
  // Find the leaves.

  if(key_leaf_pairs.size()<1) return retval;
  vector<TLeaf*> leaves;
  Int_t count = 0;
  for(int i=0;i<key_leaf_pairs.size();i++ ) {
    const string& key = key_leaf_pairs[i].first;
    const string& leafname = key_leaf_pairs[i].second;
    TLeaf* lf = fTree->GetLeaf(leafname.c_str());
    leaves.push_back(lf);
    if(!lf) {
      Info("TreeReader::getVal","tried to get value for nonexistent leaf named %s",leafname.c_str());
    } else {
      Int_t n = lf->GetLen();
      if(n>count) count=n;
    }
  }
  // Create the results.
  for(Int_t jj=0; jj< count; jj++) {
    JsonObject t;    
    for(size_t i=0;i<key_leaf_pairs.size(); i++) {
      t.add(key_leaf_pairs[i].first, getJson(leaves[i],jj)); // TODO: get multiple indexing right.
    }
    retval.push_back(t);
  }
  return retval;
}


JsonArray TreeReader::makeArray(const vector<pair< string,string> >& key_leaf_pairs)
{ 
  JsonArray r;
  std::vector<JsonObject> arr = makeVector(key_leaf_pairs); 
  for(size_t i=0;i<arr.size();i++) {
    r.add(arr[i]);
  }
  return r;
}


JsonArray TreeReader::makeFArray(const vector<pair< string,string> >& key_formula_pairs)
{
  // Build the formula objects
  if(key_formula_pairs.size()<1) return JsonArray(); 
  vector<TTreeFormula*> formulae;
  for(int i=0;i<key_formula_pairs.size();i++ ) {
    const string& key = key_formula_pairs[i].first;
    const string& formula = key_formula_pairs[i].second;
    formulae.push_back(new TTreeFormula(key.c_str(),formula.c_str(),fTree));    
  }
  // Make sure tree is reloaded so this will work. Expensive!
  Int_t n =formulae[0]->GetNdata();
  fTree->GetEntry(fTree->GetReadEntry(),1);

  // Make sure that all the formulas yield the same number of entries.
  n = formulae[0]->GetNdata();
  for(int i=0;i<formulae.size();i++) {
    if(formulae[i]->GetNdata() != n) {
      Info("TreeReader::makeArray"," Problem building array: Formula %s does not match entries to %s",
        formulae[i]->GetName(),formulae[0]->GetName());
      return JsonArray();
      
    }
  }
  
  // Create the results.
  JsonArray retval;
  for(Int_t i=0; i< n; i++) {
    JsonObject t;
    for(int j=0;j<formulae.size();j++) {
      double v = formulae[j]->EvalInstance(i);
      if(formulae[j]->IsInteger()) { t.add(formulae[j]->GetName(),(int)v);}
      else t.add(formulae[j]->GetName(),v);
    }
    retval.add(t);    
  }
  // Clean up.
  for(int j=0;j<formulae.size();j++) {
    delete formulae[j]; formulae[j] = 0;
  }
  return retval;
}


JsonArray TreeReader::makeSimpleFArray(const std::string& formula)
{
  TTreeFormula ttf("tff",formula.c_str(),fTree);
  JsonArray arr;
  int n = ttf.GetNdata();
  std::cout << formula << " " << n << std::endl;
  for(int i=0;i<ttf.GetNdata();i++) {
    Double_t v = ttf.EvalInstance(i);
    std::cout << formula << " " << i << " " << v << std::endl;
    if(ttf.IsInteger()) arr.add(JsonElement((int)v));
    else arr.add(JsonElement(v));
  }
  return arr;
}



// XML elements are obsoleted. We do things with JSON around here, sonny!
//
// XmlElement TreeReader::getXml(const string& tagname, const string& leafname, int index, int second_index)
// {
//   TLeaf* leaf = fTree->GetLeaf(leafname.c_str());
//   return getXml(tagname,leaf,index,second_index);
// }
// 
// XmlElement TreeReader::getXml(const string& tagname, TLeaf* leaf, int index, int second_index)
// {
//   if(!leaf) return XmlElement(); // null element.
// 
//   string type = leaf->IsA()->GetName();
//   if((type == "TLeafD") || (type == "TLeafF")) {
//     return XmlElement(tagname,getVal(leaf,index,second_index));
//   } 
//   
//   return XmlElement(tagname,getInt(leaf,index,second_index));
// }
// 
// 
// XmlElement TreeReader::getXmlArray(const string& tagname, const string& leafname)
// {
//   TLeaf* leaf = fTree->GetLeaf(leafname.c_str());
//   if(!leaf) return XmlElement(); // null element.
// 
//   XmlElement e(tagname);
//   Int_t n = leaf->GetNdata();
//   
//   for(int i=0;i<n;++i) {
//     e << getStr(leaf,i);
//     if(i>0) e<< ",";
//   }
//   return e;
// }
// 
// 
// XmlElement TreeReader::getXmlArray(const string& tagname, const string& leafname, Int_t index)
// {
//   TLeaf* leaf = fTree->GetLeaf(leafname.c_str());
//   if(!leaf) return XmlElement(); // null element.
// 
//   XmlElement e(tagname);
//   Int_t n = leaf->GetLenStatic();
//   
//   for(int i=0;i<n;++i) {
//     if(i>0) e<< ",";
//     e << getStr(leaf,index,i);
//   }
//   return e;
// }




