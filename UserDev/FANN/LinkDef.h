//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace FANN;

#pragma link C++ class cluster::TrainingModule+;
#pragma link C++ class FANN::neural_net+;
//ADD_NEW_CLASS ... do not change this line
#endif
