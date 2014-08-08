#ifndef COREYSHOWERSTUDY_CC
#define COREYSHOWERSTUDY_CC

#include "CoreyShowerStudy.hh"

namespace shower{
  int CoreyShowerStudy::LoadAllShowers(const larlight::event_mcshower * showers){
    std::cout << "Number of showers in this event is "
              << showers -> size() << std::endl;


    for (auto & shower : * showers){
      std::cout << "Axis of this shower is claimed to be:\n"
                << "\t p = (" 
                << shower.MotherMomentum()[0] << ", "
                << shower.MotherMomentum()[1] << ", "
                << shower.MotherMomentum()[2] << ", "
                << shower.MotherMomentum()[3] << ")\n"
                << "\t x = ("
                << shower.MotherPosition()[0] << ", "
                << shower.MotherPosition()[0] << ", "
                << shower.MotherPosition()[0] << ", "
                << shower.MotherPosition()[0] << ")\n";
    }
  
    // Get the axis of the primary shower, which is the first:
    TVector3 showerAxis(showers->front().MotherMomentum()[0],
                        showers->front().MotherMomentum()[1],
                        showers->front().MotherMomentum()[2]);

    TVector3 showerStartPoint(showers->front().MotherPosition()[0],
                              showers->front().MotherPosition()[1],
                              showers->front().MotherPosition()[2]);

    std::vector <float> parallelDist;
    std::vector <float> perpDist;
    std::vector <float> deposEnergy;

    parallelDist.reserve(showers->front().DaughterPoints().size());
    perpDist.reserve(showers->front().DaughterPoints().size());
    deposEnergy.reserve(showers->front().DaughterPoints().size());
    
    double maxPerpDist = 0;
    double maxParlDist = 0;

    for (auto & point : showers->front().DaughterPoints()){
      TVector3 thisPoint(point[0],
                         point[1],
                         point[2]);

      double t = showerAxis.Dot(showerStartPoint - thisPoint);
      t /= showerAxis.Mag2();

      TVector3 pointAlongLine = showerStartPoint + showerAxis*t;
      double this_perpDist = (thisPoint - pointAlongLine).Mag();
      double this_parlDist = (thisPoint - showerStartPoint).Mag();
      double this_deposEng = point[3];

      perpDist.push_back(this_perpDist);
      parallelDist.push_back(this_parlDist);
      deposEnergy.push_back(this_deposEng);

      if (maxParlDist < this_parlDist) maxParlDist = this_parlDist;
      if (maxPerpDist < this_perpDist) maxPerpDist = this_perpDist;

    }

    chargeProfile = new TH1F("chargeProfileHist", "Charge Profile Hist",
                              nbins, 0, maxParlDist);

    for (unsigned int i = 0; i < parallelDist.size(); i++ ){
      chargeProfile -> Fill(parallelDist[i], deposEnergy[i]);
    }

    // Find the spots that have contained 50%, 75%, 95% of the shower:
    double leng50 = 0;
    double leng75 = 0;
    double leng90 = 0;
    double totalE = chargeProfile->Integral();
    double sumE = 0;
    for (int i = 1; i <= nbins; i++){
      sumE = chargeProfile->Integral(1,i);
      if (sumE/totalE < 0.5) leng50 = i;
      if (sumE/totalE < 0.75) leng75 = i;
      if (sumE/totalE < 0.90) leng90 = i;
    }
    leng50 *= maxParlDist/nbins;
    leng75 *= maxParlDist/nbins;
    leng90 *= maxParlDist/nbins;

    std::cout << "Parameters are: \n"
              << "  50\%: " << leng50 << "\n"
              << "  75\%: " << leng75 << "\n"
              << "  90\%: " << leng90 << "\n"
              << "  E  : "  << showers ->front().MotherMomentum()[3] << "\n";

    contain50VsEnergy -> Fill(showers->front().MotherMomentum()[3],leng50);
    contain75VsEnergy -> Fill(showers->front().MotherMomentum()[3],leng75);
    contain90VsEnergy -> Fill(showers->front().MotherMomentum()[3],leng90);

    return 0;
  }
}
#endif
