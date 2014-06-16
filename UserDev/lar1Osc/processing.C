
{
  gSystem->Load("liblar1Osc.so");
  TString directory = "~/Desktop/LAr1ND/lar1Repos/lar1/ntuples_2/";
  // lar1::NtupleReprocessing ntuple_100m_long_nu(directory + "combined_ntuple_100m_long_nu.root");      
  // lar1::NtupleReprocessing ntuple_100m_long_nu_fosc(directory + "combined_ntuple_100m_long_nu_fosc.root"); 
  lar1::NtupleReprocessing ntuple_100m_nu(directory + "combined_ntuple_100m_nu.root");           
  lar1::NtupleReprocessing ntuple_100m_nu_fosc(directory + "combined_ntuple_100m_nu_fosc.root");      
  // lar1::NtupleReprocessing ntuple_100m_nubar(directory + "combined_ntuple_100m_nubar.root");        
  // lar1::NtupleReprocessing ntuple_100m_nubar_fosc(directory + "combined_ntuple_100m_nubar_fosc.root");   
  lar1::NtupleReprocessing ntuple_470m_nu(directory + "combined_ntuple_470m_nu.root");           
  lar1::NtupleReprocessing ntuple_470m_nu_fosc(directory + "combined_ntuple_470m_nu_fosc.root");
  // lar1::NtupleReprocessing ntuple_470m_nubar(directory + "combined_ntuple_470m_nubar.root");
  // lar1::NtupleReprocessing ntuple_470m_nubar_fosc(directory + "combined_ntuple_470m_nubar_fosc.root");
  lar1::NtupleReprocessing ntuple_700m_nu(directory + "combined_ntuple_700m_nu.root");
  lar1::NtupleReprocessing ntuple_700m_nu_fosc(directory + "combined_ntuple_700m_nu_fosc.root");
  // lar1::NtupleReprocessing ntuple_700m_nubar(directory + "combined_ntuple_700m_nubar.root");
  // lar1::NtupleReprocessing ntuple_700m_nubar_fosc(directory + "combined_ntuple_700m_nubar_fosc.root");

  // For each ntuple, do the nue, numu, fosc processing loops.
  // Grouping them by iDet:

  ntuple_100m_nu.Loop("nue",0);
  // ntuple_100m_nu.Loop("numu",0);
  ntuple_100m_nu_fosc.Loop("fosc",0);
  // ntuple_100m_nubar.Loop("nue",0);
  // ntuple_100m_nubar.Loop("numu",0);
  // ntuple_100m_nubar_fosc.Loop("fosc",0);

  // ntuple_470m_nu.Loop("nue",1);
  // ntuple_470m_nu.Loop("numu",1);
  // ntuple_470m_nu_fosc.Loop("fosc",1);
  // ntuple_470m_nubar.Loop("nue",1);
  // ntuple_470m_nubar.Loop("numu",1);
  // ntuple_470m_nubar_fosc.Loop("fosc",1);

  // ntuple_700m_nu.Loop("nue",2);
  // ntuple_700m_nu.Loop("numu",2);
  // ntuple_700m_nu_fosc.Loop("fosc",2);
  // ntuple_700m_nubar.Loop("nue");
  // ntuple_700m_nubar.Loop("numu",2);
  // ntuple_700m_nubar_fosc.Loop("fosc",2);

  // ntuple_700m_nu.Loop("nue",4);
  // ntuple_700m_nu.Loop("numu",4);
  // ntuple_700m_nu_fosc.Loop("fosc",4);
  // ntuple_700m_nubar.Loop("nue");
  // ntuple_700m_nubar.Loop("numu",4);
  // ntuple_700m_nubar_fosc.Loop("fosc",4);

  // ntuple_100m_long_nu.Loop("nue",5);
  // ntuple_100m_long_nu.Loop("numu",5);
  // ntuple_100m_long_nu_fosc.Loop("fosc",5);


}