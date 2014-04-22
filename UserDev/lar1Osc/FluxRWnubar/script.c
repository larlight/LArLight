{
  
  gROOT->ProcessLine(".L FluxRWnubar.cxx+g");

  FluxRWnubar rw;

  rw.GetWeightHist(4,1)->Draw();
  c1->Print("mu.gif");
  rw.GetWeightHist(4,2)->Draw();
  c1->Print("pi.gif");
  rw.GetWeightHist(4,3)->Draw();
  c1->Print("k0.gif");
  rw.GetWeightHist(4,4)->Draw();
  c1->Print("kc.gif");
}
