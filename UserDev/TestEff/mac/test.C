{

  TH1D* h=new TH1D("h","",100,0,10);

  TCanvas* c=new TCanvas("c","",600,500);

  for(size_t i = 1; i<=h->GetNbinsX(); ++i) {

    h->SetBinContent(i,i);
    h->Draw();
    c->Update();
    gSystem->Sleep(10);
  }

}
