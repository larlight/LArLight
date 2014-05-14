//
// Example C++ routine to run your analysis module, OpticalDetector.
//

#include "OpticalDetector-TypeDef.hh"
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TGraph.h>
int main(int argc, char** argv){

  TFile fout("out.root","RECREATE");

  std::vector<float> wf(40,0);

  opdet::WFAlgoSPE algo;

  util::ElecClock clock = util::TimeService::GetME().OpticalClock();

  TH1D* h=0;

  TH2D* hCorr = new TH2D("hCorr","",50,-25,0,100,0,150);

  TGraph *gCorr = new TGraph(30);
  gCorr->SetName("gCorr");

  for(size_t i=0; i<30; ++i) {

    wf.clear();
    wf.resize(40,0);

    h = new TH1D(Form("h%02zu", i),
		 "",
		 wf.size(),0,wf.size());

    algo.Reset();
    algo.GenSPE(clock.TickPeriod() * 20, 200);
    algo.SetChannel(0);
    //algo.GenSPE(clock.TickPeriod() * 20, 200);

    
    std::cout<<(clock.TickPeriod() * 10 + clock.TickPeriod() / 10. * i)/clock.TickPeriod() << std::endl;
    algo.AddPhoton(clock.TickPeriod() * 10 + clock.TickPeriod() / 10. * i);

    algo.Process(wf,clock);


    double slope=0;
    double offset=0;
    bool found=false;
    for(size_t j=0; j<wf.size(); ++j) {

      h->SetBinContent(j+1,wf.at(j));

      if(!found && j<wf.size()-2 && wf.at(j)>10) {
	slope  = wf.at(j+1) - wf.at(j);
	offset = wf.at(j) - slope * j;
	std::cout
	  << "Found @ " << j
	  << " start = " << (-1 * offset / slope) 
	  << " diff = " << clock.TickPeriod()*((clock.TickPeriod() * 10 + clock.TickPeriod() / 10. * i)/clock.TickPeriod() - (-1 * offset / slope))
	  << std::endl;
	found=true;
	hCorr->Fill(clock.TickPeriod()*((clock.TickPeriod() * 10 + clock.TickPeriod() / 10. * i)/clock.TickPeriod() - (-1 * offset / slope)),
		    wf.at(j));
	gCorr->SetPoint(i,
			(-1)*(clock.TickPeriod()*((clock.TickPeriod() * 10 + clock.TickPeriod() / 10. * i)/clock.TickPeriod() - (-1 * offset / slope))),
			log(wf.at(j)));
      }
      if(found) std::cout<<wf.at(j)<< " ";
    }
    std::cout<<std::endl<<std::endl;

    fout.cd();
    h->Write();
  }

  fout.cd();
  hCorr->Write();
  gCorr->Write();

  fout.Close();
  return 0;
}
