#ifndef TRAININGMODULE_CC
#define TRAININGMODULE_CC

#include "TrainingModule.hh"

void TrainingModule::configureHiddenLayers(int numLayers, ...){

  fHiddenLayerLength.clear();
  fNumHiddenLayers = numLayers;
  fHiddenLayerLength.reserve(numLayers);

  int i;
  va_list vl;
  va_start(vl,numLayers);
  for (i=1;i<numLayers;i++)
  {
    fHiddenLayerLength.push_back(va_arg(vl,int));
  }
  va_end(vl);
  return;
}

void TrainingModule::saveFANNToFile(std::string s){
  if (s != "")
    fann_save(ann, s.c_str());
  else 
    fann_save(ann, fOutputFileName.c_str());

  // in
  return;
}

template <class T>
void TrainingModule::trainOnData(T * data, T * truth){
  fann_train(ann, data, truth);
}

#endif
