#ifndef ANALYSISALGCONSTANTS_HH
#define ANALYSISALGCONSTANTS_HH

#include <vector>
namespace calo {

  static const double kCalAmpConstants_ARGONEUT[2]  = {0.588726e-3 , 1.18998e-3};
  static const double kCalAreaConstants_ARGONEUT[2] = {0.773887e-2 , 1.78186e-2};
  static const bool kCaloUseModBox_ARGONEUT = false;

  static const double kCalAmpConstants_UB[3]  = {2.677e-3, 2.804e-3, 5.716e-3};
  static const double kCalAreaConstants_UB[3] = {0, 0, 0};
  static const bool kCaloUseModBox_UB = false;
}

#endif
