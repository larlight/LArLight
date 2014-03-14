#ifndef FORMULALOOTER_H_C7SY35FB
#define FORMULALOOTER_H_C7SY35FB

#include "JsonElement.h"

class TTree;

JsonElement FormulaMakeElement(TTree* tree, const std::string& formula);


JsonArray FormulaMakeArray(TTree* tree, const std::vector<std::pair< std::string,std::string> >& key_formula_pairs);

#ifndef __CINT__
// Syntactic sugar to wrap above call.
template <class ...B> 
JsonArray FormulaMakeArray(TTree* tree, const std::string& k, const std::string& f, B... argTail);
#endif

JsonArray test_fma(TTree* tree);


#endif /* end of include guard: FORMULALOOTER_H_C7SY35FB */
