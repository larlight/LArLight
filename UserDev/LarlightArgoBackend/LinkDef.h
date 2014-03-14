// This file contains the instructions that tell rootcint how to 
// build the Dictionary.cxx file.

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class JsonElement+;
#pragma link C++ class JsonFixed+;
#pragma link C++ class JsonSigFig+;
#pragma link C++ class JsonObject+;
#pragma link C++ class JsonArray+;

#pragma link C++ function FormulaMakeElement;

#pragma link C++ function FormulaMakeArray;
#pragma link C++ funciton test_fma;

#endif
