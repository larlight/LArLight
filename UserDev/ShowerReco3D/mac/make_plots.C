make_plots()
{


TFile  * elecs=new TFile("out_electrons.root","READ");
TFile  * gammas=new TFile("out_gammas.root","READ");


TTree * tels=(TTree *)elecs->Get("shower_tree");
tels->SetName("elshower");
TTree * tgams=(TTree *)gammas->Get("shower_tree");
tgams->SetName("gamshower");

TTree * mctels=(TTree *)elecs->Get("mctruth_tree");
mctels->SetName("elmc");
TTree * mctgams=(TTree *)gammas->Get("mctruth_tree");
mctgams->SetName("gammc");

tels->AddFriend("elmc");
tgams->AddFriend("gammc");


TCanvas * c1 = new TCanvas(" dE/dx plot ","dE/dx plot");

tels->SetLineColor(kBlue);
tels->SetLineWidth(2);
tels->Draw("fdEdx[2]>>(60,0,12)","fdEdx[2]>0");


tgams->SetLineColor(kRed);
tgams->SetLineWidth(2);
tgams->Draw("fdEdx[2]","fdEdx[2]>0","same");



TCanvas * c2 = new TCanvas(" x_reco ","x_reco");

tels->Draw("fparticle[0].ftrajectory[0]._position.X()");


}