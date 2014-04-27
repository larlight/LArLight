#include "LorentzVectorLight.hh"

namespace lar1
{
  LorentzVectorLight::LorentzVectorLight(){
    fX = 0.0;
    fY = 0.0;
    fZ = 0.0;
    fT = 0.0;
    return;
  }
  LorentzVectorLight::LorentzVectorLight(float a, float b, float c, float d){
    fX = a;
    fY = b;
    fZ = c;
    fT = d;
    return;
  }
  LorentzVectorLight::LorentzVectorLight(TLorentzVector& vec){
    fX = vec.X();
    fY = vec.Y();
    fZ = vec.Z();
    fT = vec.T();
    return;
  }
  LorentzVectorLight::LorentzVectorLight(const TLorentzVector& vec){
    fX = vec.X();
    fY = vec.Y();
    fZ = vec.Z();
    fT = vec.T();
    return;
  }
}
