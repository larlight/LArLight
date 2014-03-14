int tancurve(float adc)
{
  return atan(adc/50.)/TMath::Pi()*256+127;
}