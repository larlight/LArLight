#include <iostream>
#include <fstream>
#include <string>
#include "TCanvas.h"

#include "readGFData.C"

// #include "readGFData.C"

void lsnd_plot (TCanvas* c)
{
  c->cd();
  const char* data_dir = "lsnd_data/";
  Double_t  dm2BF[] = {1.2};
  Double_t sin22thBF[] = {0.003};

  const Int_t NDATAFILES = 11;
  const char * file_list[NDATAFILES] = {"llreg_608_1.vec",
				       "llreg_608_2.vec",
				       "llreg_608_3.vec",
				       "llreg_607_1.vec",
				       "llreg_607_2.vec",
				       "llreg_607_3.vec",
				       "llreg_607_4.vec",
				       "llreg_607_5.vec",
				       "llreg_607_6.vec",
				       "llreg_607_7.vec",
				       "llreg_607_8.vec"};

  Int_t graph_color[NDATAFILES] = {29, 29, 29, 38, 38, 38, 38, 38, 38, 38, 38};

  Int_t    nlines;
  Double_t x[500],y[500];
  Double_t dummy, dummy_old;
  TGraph* gr[NDATAFILES];
  for (Int_t ifile = 0; ifile<NDATAFILES; ifile++) {
    nlines = 0;
    for (Int_t i=0;i<500;i++){x[i]=0.0;y[i]=0.0;}
    char  filename[100];
	strcpy(filename, data_dir);
	//printf("%s\n",filename);
	strcat(filename, file_list[ifile]);
	//printf("%s\n",filename);
	std::cout << std::endl;
    ifstream datafile;
    datafile.open(filename, ios_base::in);
    //check if the file is open: 
    if (!datafile.is_open() ) {std::cerr << "lsnd_plot.C: file not opened" <<std::endl; return;}
    //else {std::cout << "Successfully opened " << filename << std::endl;}
    while (!datafile.eof()) {
      datafile >> dummy; 
      datafile >> dummy; 
      datafile >> x[nlines]; 
      datafile >> y[nlines];
      nlines++;
      if (dummy == dummy_old) nlines--; //if last row was empty
      dummy_old = dummy;
    }

    gr[ifile] = new TGraph(nlines,x,y);
    datafile.close();
  }
  std::cout << "Finished reading data files" << std::endl;
  for (Int_t ifile = 0; ifile<NDATAFILES; ifile++) {
    // Int_t graph_color[NDATAFILES] = {29, 29, 29, 38, 38, 38, 38, 38, 38, 38, 38};
    gr[ifile]->SetFillColor(graph_color[ifile]);
    gr[ifile]->Draw("LF");
  }
  //Add the best fit point;
  TGraph * bfPoint = new TGraph(1, sin22thBF, dm2BF);
  bfPoint -> SetLineColor(2);
  bfPoint -> SetMarkerStyle(3);
  bfPoint -> SetMarkerColor(1);
  bfPoint -> Draw("LP");

  plotData(c);

  return;
}

void lsnd_plot(){
	TCanvas * d = new TCanvas("LSND Region", "LSND Region", 600, 600);
  d->SetLogx();
  d->SetLogy();
  
  TH2D* hr1=new TH2D("hr1","hr1",500,0.0001,1,500,0.01,100);
  hr1->Reset();
  hr1->SetFillColor(0);
  hr1->SetTitle(";sin^{2}2#theta_{#mue};#Deltam^{2} (eV^{2})");
  hr1->GetXaxis()->SetTitleOffset(1.1);
  hr1->GetYaxis()->SetTitleOffset(1.2);
  hr1->GetXaxis()->SetTitleSize(0.05);
  hr1->GetYaxis()->SetTitleSize(0.05);
  hr1->SetStats(kFALSE);
  hr1->Draw();
  
  lsnd_plot(d);

	return;
}
