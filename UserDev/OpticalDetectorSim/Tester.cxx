#ifndef TESTER_CXX
#define TESTER_CXX

#include "Tester.h"

namespace opdet {

  Tester::Tester()
  {

  }

  bool Tester::initialize() {

    return true;
  }

  bool Tester::analyze(::larlight::storage_manager* storage) {

    ::larlight::event_tpcfifo* wfs = (::larlight::event_tpcfifo*)(storage->get_data(larlight::DATA::TPCFIFO));

    wfs->reserve(kNChannels * 2);

    ::util::ElecClock clock = ::util::TimeService::GetME().OpticalClock();

    std::vector<double> photons(1e4,0);

    for(size_t i=0; i<kNChannels; ++i) {

      wfs->push_back(::larlight::tpcfifo());
      wfs->push_back(::larlight::tpcfifo());

      wfs->at(i*2).set_channel_number(i);
      wfs->at(i*2+1).set_channel_number(i);

      fGen.Reset();
      fGen.SetTimeInfo(clock,1.6e6*4);
      for(size_t i=0; i<1e4; i++)
	
	photons.at(i) = RandomServer::GetME().Gaus(3.2e6,1600);

      fGen.SetPhotons(photons);

      fGen.GenWaveform(i,
		       wfs->at(i*2),
		       wfs->at(i*2+1));
    }

    return true;
  }

  bool Tester::finalize() {

    return true;
  }
  
}

#endif
