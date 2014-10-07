
{
  gSystem->Load("liblar1Osc.so");


  TString directory = "~/genie_RW/";

  //LAr1-ND @ 100 meters
  // lar1::Reprocessing ntuple_100m_nu              (directory + "combined_ntuple_100m_nu.root");           
  // lar1::Reprocessing ntuple_100m_nu_fosc         (directory + "combined_ntuple_100m_nu_fosc.root");      
  // ntuple_100m_nu.Loop("nue",0,0);
  // ntuple_100m_nu.Loop("numu",0,0);
  // ntuple_100m_nu_fosc.Loop("fosc",0,0);
  // 
  //LAr1-ND @ 150 meters
  // lar1::Reprocessing ntuple_150m_nu              (directory + "combined_ntuple_150m_nu.root");           
  // lar1::Reprocessing ntuple_150m_nu_fosc         (directory + "combined_ntuple_150m_nu_fosc.root");      
  // ntuple_150m_nu.Loop("nue",0,1);
  // ntuple_150m_nu.Loop("numu",0,1);
  // ntuple_150m_nu_fosc.Loop("fosc",0,1);

  //LAr1-ND @ 200 meters
  // lar1::Reprocessing ntuple_200m_nu              (directory + "combined_ntuple_200m_nu.root");           
  // lar1::Reprocessing ntuple_200m_nu_fosc         (directory + "combined_ntuple_200m_nu_fosc.root");      
  // ntuple_200m_nu.Loop("nue",0,3);
  // ntuple_200m_nu.Loop("numu",0,3);
  // ntuple_200m_nu_fosc.Loop("fosc",0,3);

  // MicroBooNE @ 470m
  lar1::Reprocessing ntuple_470m_nu              (directory + "combined_ntuple_470m_nu.root");           
  // lar1::Reprocessing ntuple_470m_nu_fosc         (directory + "combined_ntuple_470m_nu_fosc.root");
  // ntuple_470m_nu.Loop("nue",1,4);
  ntuple_470m_nu.Loop("numu",1,4);
  // ntuple_470m_nu_fosc.Loop("fosc",1,4);

  //T600 @ 600m on axis
  // lar1::Reprocessing ntuple_600m_onaxis_nu       (directory + "combined_ntuple_600m_onaxis_nu.root");           
  // lar1::Reprocessing ntuple_600m_onaxis_nu_fosc  (directory + "combined_ntuple_600m_onaxis_nu_fosc.root");      
  // ntuple_600m_onaxis_nu.Loop("nue",2,5);
  // ntuple_600m_onaxis_nu.Loop("numu",2,5);
  // ntuple_600m_onaxis_nu_fosc.Loop("fosc",2,5);

  //T600 @ 600m off axis
  // lar1::Reprocessing ntuple_600m_offaxis_nu      (directory + "combined_ntuple_600m_offaxis_nu.root");           
  // lar1::Reprocessing ntuple_600m_offaxis_nu_fosc (directory + "combined_ntuple_600m_offaxis_nu_fosc.root");      
  // ntuple_600m_offaxis_nu.Loop("nue",2,6,10000);
  // ntuple_600m_offaxis_nu.Loop("numu",2,6,10000);
  // ntuple_600m_offaxis_nu_fosc.Loop("fosc",2,6,10000);
}
