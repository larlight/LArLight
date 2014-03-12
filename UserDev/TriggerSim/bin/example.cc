//
// Example C++ routine to run your analysis module, TriggerSim.
//

#include "TriggerSim-TypeDef.hh"
#include <iostream>

int main(int argc, char** argv){

  trigger::UBTriggerAlgo trig;
  trig.SetDebugMode(true);
  trig.ReportConfig();

  trig.AddTriggerCalib(0.0032 + 180.e-6);
  trig.AddTriggerExt(0.0032 + 120.e-6);

  trig.ClearInputTriggers();

  trig.AddTriggerBNB(0.0016 + 120.e-6);

  trig.AddTriggerNuMI(0.0032 + 120.e-6);

  //trig.ShowCandidateTriggers();

  std::vector<trigdata::Trigger> aho;
  trig.ProcessTrigger(aho);

  return 0;
}
