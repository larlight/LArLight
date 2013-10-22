#ifndef HIT_CC
#define HIT_CC

#include "hit.hh"

//##########################################################################
hit::hit(const hit &original) : data_base(original),
				fHitSignal(original.fHitSignal),
				fStartTime(original.fStartTime),
				fSigmaStartTime(original.fSigmaStartTime),
				fPeakTime(original.fPeakTime),
				fSigmaPeakTime(original.fSigmaPeakTime),
				fEndTime(original.fEndTime),
				fSigmaEndTime(original.fSigmaEndTime),
				fCharge(original.fCharge),
				fSigmaCharge(original.fSigmaCharge),
				fMaxCharge(original.fMaxCharge),
				fSigmaMaxCharge(original.fSigmaMaxCharge),
				fMultiplicity(original.fMultiplicity),
				fGoodnessOfFit(original.fGoodnessOfFit),
				fView(original.fView),
				fSignalType(original.fSignalType),
				fChannel(original.fChannel)
//##########################################################################
{};

//##########################################################################
void hit::clear_data()
//##########################################################################
{
  fHitSignal.clear();
  fStartTime = fPeakTime = fEndTime = -1;
  fSigmaStartTime = fSigmaPeakTime = fSigmaEndTime = -1;
  fCharge = fMaxCharge = -1;
  fSigmaCharge = fSigmaMaxCharge = -1;
  fMultiplicity = -1;
  fGoodnessOfFit = -1;
  fView = GEO::kUnknown;
  fSignalType = GEO::kMysteryType;
  fChannel = 0xffffffff;
}

#endif
