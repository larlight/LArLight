{
  
  gROOT->ProcessLine(".L FluxRW.cxx+g");

  FluxRW rw;

  rw.GetWeightHist(1,1)->Draw();
  c1->Print("mu.gif");
  rw.GetWeightHist(1,2)->Draw();
  c1->Print("pi.gif");
  rw.GetWeightHist(1,3)->Draw();
  c1->Print("k0.gif");
  rw.GetWeightHist(1,4)->Draw();
  c1->Print("kc.gif");
}
