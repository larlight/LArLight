#ifndef TMPPI0MASSSTUDY_CC
#define TMPPI0MASSSTUDY_CC

#include "TmpPi0MassStudy.hh"

namespace larlight {

  bool TmpPi0MassStudy::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D"). 
    //
    ana_tree = 0;

    PrepareAnaTree();

    return true;
  }
  
  bool TmpPi0MassStudy::analyze(storage_manager* storage) {
  
    //
    // Do your event-by-event analysis here. This function is called for 
    // each event in the loop. You have "storage" pointer which contains 
    // event-wise data. To see what is available, check the "Manual.pdf":
    //
    // http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
    // 
    // Or you can refer to Base/DataFormatConstants.hh for available data type
    // enum values. Here is one example of getting PMT waveform collection.
    //
    // event_fifo* my_pmtfifo_v = (event_fifo*)(storage->get_data(DATA::PMFIFO));
    //
    // if( event_fifo )
    //
    //   std::cout << "Event ID: " << my_pmtfifo_v->event_id() << std::endl;
    //

    larlight::event_mctruth* ev_mctruth = (larlight::event_mctruth*)storage->get_data(larlight::DATA::MCTruth);
    if(!ev_mctruth) {
      print(larlight::MSG::ERROR,__FUNCTION__,Form("Did not find specified data product, MCTruth!"));
      return false;
    }
    
    //get mother pi0 energy from mctruth
    _MC_energy = ev_mctruth->at(0).GetParticles().at(0).Trajectory().at(0).E();

    _mass = ComputePi0Mass::GetInstance()->GetMass();

    if(_mass > 0) { ana_tree->Fill(); }

    return true;
  }

  bool TmpPi0MassStudy::finalize() {

    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
    //
    // Say you made a histogram pointer h1 to store. You can do this:
    //
    // if(_fout) { _fout->cd(); h1->Write(); }
    //
    // else 
    //   print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    //

    if(_fout) { _fout->cd(); ana_tree->Write(); }

    if(ana_tree) delete ana_tree;
  
    return true;
  }


  void TmpPi0MassStudy::PrepareAnaTree(){

    if(!ana_tree) {
      
      ana_tree = new TTree("ana_tree","");

      ana_tree->Branch("mass",&_mass,"mass/F");
      ana_tree->Branch("mc_E",&_MC_energy,"mc_E/F");

    }

  }
}
#endif
