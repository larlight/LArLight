


plotMaker(){

	//Plot all three, 470 +700, 700 in nu mode

/*
	const Int_t N = 7;
	Int_t colors[N] = {1, 8, 9, 41, 46, 39, 12};
	TString config[N] = {"100m, 470m, 700m", "470m, 700m", "100m, 700m", "100m, 470m", "700m", "470m", "100m"};
	char fileSource[200]="/uboone/app/users/cadams/frozen/CombinedFitting/reprocessing/Combined_ntuples_weight/";
	TString fileNameRoot = fileSource;
	TString fileExtension[N] = {"nubar_100_470_700", "nubar_470_700", "nubar_100_700", "nubar_100_470", "nubar_700", "nubar_470", "nubar_100"};
    TString running = "#nubar running";
    TString datapot = "10#times10^{20} POT";
    TString pngName = "nubar_5sigma_all.png";	
*/    

    const Int_t N = 7;
	Int_t colors[N] = {1, 8, 9, 41, 46, 39, 12};
	TString config[N] = {"100m, 470m, 700m", "470m, 700m", "100m, 700m", "100m, 470m", "700m", "470m", "100m"};
	char fileSource[200]="/uboone/app/users/cadams/frozen/CombinedFitting/reprocessing/Combined_ntuples_weight/";
	TString fileNameRoot = fileSource;
	TString fileExtension[N] = {"nu_100_470_700", "nu_470_700", "nu_100_700", "nu_100_470", "nu_700", "nu_470", "nu_100"};
    TString running = "#nu running";
    TString datapot = "6.6#times10^{20} POT";
    TString pngName = "nu_5sigma_all.png";


	//Plot all three, 470 +700, 700 in nubar mode
/*	
	const Int_t N = 3;
		Int_t * colors = {1, 8, 9, 41, 46, 39};

	TString config[N] = {"100m, 470m, 700m", "470, 700", "700"};
	char fileSource[200]="/uboone/app/users/cadams/frozen/CombinedFitting/reprocessing/Combined_ntuples_weight/";
	TString fileNameRoot = fileSource;
	TString fileExtension[N] = {"nubar_100_470_700", "nubar_470_700", "nubar_700"};
    TString running = "#nubar running";
    TString datapot = "10#times10^{20} POT";
*/
	//Plot all single detectors in nu:
/*	
	const Int_t N = 3;
		Int_t * colors = {1, 8, 9, 41, 46, 39};

	TString config[N] = {"100", "470", "700"};
	char fileSource[200]="/uboone/app/users/cadams/frozen/CombinedFitting/reprocessing/Combined_ntuples_weight/";
	TString fileNameRoot = fileSource;
	TString fileExtension[N] = {"nu_100", "nu_470", "nu_700"};
    TString running = "#nu running";
    TString datapot = "6.6#times10^{20} POT";
*/
	//Plot all single detectors in nubar:
/*
	const Int_t N = 3;
	Int_t * colors = {1, 8, 9, 41, 46, 39};
	TString config[N] = {"100", "470", "700"};
	char fileSource[200]="/uboone/app/users/cadams/frozen/CombinedFitting/reprocessing/Combined_ntuples_weight/";
	TString fileNameRoot = fileSource;
	TString fileExtension[N] = {"nubar_100", "nubar_470", "nubar_700"};
    TString running = "#nubar running";
    TString datapot = "10#times10^{20} POT";
*/

////----------------------------------
//End of input section.  Starting plot routine.
	
	//grid boundaries
    Double_t dm2min = 0.01;                       //eV**2
    Double_t dm2max = 100.;                       //eV**2
    Double_t sin22thmin = 0.0001;
    Double_t sin22thmax = 1.0;
    

	//TGraph * sens90 = (TGraph*) file1 -> Get("Graph;1");
    //TGraph * sens3s = (TGraph*) file1 -> Get("Graph;2");
    TGraph * sens5s[N]; 
	std::cout << "Using plots from folder " << fileNameRoot << std::endl;
    for (int i = 0; i < N; i++){
    	    std::cout << "Reading plot from " << fileExtension[i] << std::endl;
    	TFile *file1 = new TFile(fileNameRoot+fileExtension[i]+"_chi2.root");
    	sens5s[i] = (TGraph*) file1 -> Get("Graph;3");
    	sens5s[i] -> SetLineColor(colors[i]);
    	sens5s[i] -> SetLineWidth(2);
    	if (i > 3) sens5s[i] -> SetLineStyle(2);
    }

    //======================================================
    printf("\nDrawing sensitivity curves...\n");

    cout<<"Drawing LSND intervals...\n\n";

    TCanvas* c3 = new TCanvas("c3","Sensitivity",700,700);
    c3->SetLogx();
    c3->SetLogy();

    TH2D* hr1=new TH2D("hr1","hr1",500,sin22thmin,sin22thmax,500,dm2min,dm2max);
    hr1->Reset();
    hr1->SetFillColor(0);
    hr1->SetTitle(";sin^{2}2#theta_{#mu#mu};#Deltam^{2} (eV^{2})");
    hr1->GetXaxis()->SetTitleOffset(.9);
    hr1->GetYaxis()->SetTitleOffset(.9);
    hr1->GetXaxis()->SetTitleSize(0.05);
    hr1->GetYaxis()->SetTitleSize(0.05);
    hr1->SetStats(kFALSE);
    hr1->Draw();
    gROOT->ProcessLine(".x ./lsnd_plot.c(c3)");

    //======================================================

    TLegend* legt=new TLegend(0.55,0.7-N*0.026,0.8,0.9);
    legt->SetFillStyle(0);
    legt->SetFillColor(0);
    legt->SetBorderSize(0);
    legt->SetTextSize(0.025);
    legt->AddEntry("",running,"");
    legt->AddEntry("",datapot,"");
    
    
    //for (int i=0; i<N; i++) legt->AddEntry("",config[i],"");

	for (int i=0; i<N; i++){
	    sens5s[i]->Draw("l");
    	legt->AddEntry(sens5s[i],"5#sigma "+config[i],"l");
	}
    legt->Draw();

    TLegend* leg3=new TLegend(0.2,0.2,0.4,0.27);
    leg3->SetFillStyle(0);
    leg3->SetFillColor(0);
    leg3->SetBorderSize(0);
    leg3->SetTextSize(0.04);
    TGraph *gdummy1 = new TGraph();
    gdummy1->SetFillColor(29);
    TGraph *gdummy2 = new TGraph();
    gdummy2->SetFillColor(38);
    leg3->AddEntry(gdummy2,"LSND 90% CL","F");
    leg3->AddEntry(gdummy1,"LSND 99% CL","F");
    leg3->Draw();

    TImage *img = TImage::Create();
    img -> FromPad(c3);
	img -> WriteImage(pngName);

		   

    cout<<"\nEnd of routine.\n";
    
}