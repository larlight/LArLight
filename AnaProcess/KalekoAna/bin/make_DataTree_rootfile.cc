
#include "KalekoAna-TypeDef.hh"

int main(){

  KalekoTrackAna k;
  std::string fname("../../../kaleko_input_files_LArLight/v3_ang0_dsout_reduced_Kalman3DSPS.root");
  int ang=0;
  int nevents=10;
  int bins =100;
  return k.EventLoop(fname,ang,nevents,bins);
}
