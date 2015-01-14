
{
  gSystem->Load("liblar1Osc.so");


  TString directory = "/media/cadams/data_linux/eff_70/";
  // TString directory = "/media/cadams/data_linux/eff_60/";
  // TString directory = "/media/cadams/data_linux/eff_50/";
  // TString directory = "/media/cadams/data_linux/two_horns/";

  // LAr1-ND @ 100 meters
  lar1::Reprocessing ntuple_100m_nu              (directory + "combined_ntuple_100m_nu.root");           
  lar1::Reprocessing ntuple_100m_nu_fosc         (directory + "combined_ntuple_100m_nu_fosc.root");      
  // ntuple_100m_nu.UseTwoHornConfig();
  ntuple_100m_nu.Loop("nue",0,0);
  // ntuple_100m_nu.Loop("numu",0,0);
  // ntuple_100m_nu_fosc.UseTwoHornConfig();
  ntuple_100m_nu_fosc.Loop("fosc",0,0);

  // MicroBooNE @ 470m
  lar1::Reprocessing ntuple_470m_nu              (directory + "combined_ntuple_470m_nu.root");           
  lar1::Reprocessing ntuple_470m_nu_fosc         (directory + "combined_ntuple_470m_nu_fosc.root");
  // ntuple_470m_nu.UseTwoHornConfig();
  ntuple_470m_nu.Loop("nue",1,4);
  // ntuple_470m_nu.Loop("numu",1,4);
  // ntuple_470m_nu.UseTwoHornConfig();
  ntuple_470m_nu_fosc.Loop("fosc",1,4);

  //T600 @ 600m on axis
  lar1::Reprocessing ntuple_600m_onaxis_nu       (directory + "combined_ntuple_600m_onaxis_nu.root");           
  lar1::Reprocessing ntuple_600m_onaxis_nu_fosc  (directory + "combined_ntuple_600m_onaxis_nu_fosc.root");      
  ntuple_600m_onaxis_nu.Loop("nue",2,5);
  // ntuple_600m_onaxis_nu.UseTwoHornConfig();
  // // ntuple_600m_onaxis_nu.Loop("numu",2,5);
  // ntuple_600m_onaxis_nu_fosc.UseTwoHornConfig();
  ntuple_600m_onaxis_nu_fosc.Loop("fosc",2,5);


}
  //;
