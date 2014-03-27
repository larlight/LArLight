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

void TrainingModule::init(){

  if (ann){
    std::cerr << "ERROR: ann is already initialized, aborting.\n";
    return; 
  }

  unsigned int * layers;
  layers = new unsigned int[fNumHiddenLayers + 2];
  layers[0] = fFeatureVectorLength;
  for (unsigned int i = 1; i <= fNumHiddenLayers; i++)
    layers[i] = fHiddenLayerLength[i-1];
  layers[fNumHiddenLayers+1] = fOutputVectorLength;
  ann = fann_create_standard_array(fFeatureVectorLength, layers); 
  return;
}

void TrainingModule::trainOnData(float * data, float * truth){
  fann_train(ann, data, truth);
}

#endif
