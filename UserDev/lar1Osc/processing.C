
{
  gSystem->Load("liblar1Osc.so");

  TString directory = "/Users/jzennamo/Desktop/LArLite/Files/";
  //lar1::NtupleReprocessing ntuple_100m_long_nu(directory + "combined_ntuple_100m_long_nu.root");      
  //lar1::NtupleReprocessing ntuple_100m_long_nu_fosc(directory + "combined_ntuple_100m_long_nu_fosc.root"); 

  //lar1::NtupleReprocessing ntuple_100m_nu(directory + "combined_ntuple_100m_nu.root");           
  //lar1::NtupleReprocessing ntuple_100m_nu_fosc(directory + "combined_ntuple_100m_nu_fosc.root");      
 
  // lar1::NtupleReprocessing ntuple_470m_nu(directory + "combined_ntuple_470m_nu.root");           
  //lar1::NtupleReprocessing ntuple_470m_nu_fosc(directory + "combined_ntuple_470m_nu_fosc.root");
 
  lar1::NtupleReprocessing ntuple_700m_nu(directory + "combined_ntuple_700m_nu.root");
  lar1::NtupleReprocessing ntuple_700m_nu_fosc(directory + "combined_ntuple_700m_nu_fosc.root");

  // For each ntuple, do the nue, numu, fosc processing loops.
  // Grouping them by iDet:

  //@ 100 meters
  //  ntuple_100m_nu.Loop("nue",0);
  // ntuple_100m_nu.Loop("numu",0);
  //ntuple_100m_nu_fosc.Loop("fosc",0);

  //@ MicroBooNE
  //  ntuple_470m_nu.Loop("nue",1);
  //  ntuple_470m_nu.Loop("numu",1);
  //  ntuple_470m_nu_fosc.Loop("fosc",1);

  //T600 @ 600m on axis
  ntuple_700m_nu.Loop("nue",6);
  ntuple_700m_nu.Loop("numu",6);
  ntuple_700m_nu_fosc.Loop("fosc",6);


}
