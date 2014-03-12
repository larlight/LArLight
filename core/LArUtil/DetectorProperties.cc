#ifndef DETECTORPROPERTIES_CC
#define DETECTORPROPERTIES_CC

#include "DetectorProperties.hh"

namespace larutil {

  DetectorProperties* DetectorProperties::_me = 0;

  DetectorProperties::DetectorProperties(std::string file_name, std::string tree_name) 
    : LArUtilBase(file_name,tree_name)
  {
    if(_file_name.empty())
      _file_name = Form("%s/LArUtil/dat/%s",
			getenv("LARLIGHT_CORE_DIR"),
			kFILENAME_DETECTORPROPERTIES.c_str());
    if(_tree_name.empty())
      _tree_name = kTREENAME_DETECTORPROPERTIES;
    _name = "DetectorProperties";
    LoadData();
  }

  void DetectorProperties::ClearData()
  {
    fSamplingRate = larlight::DATA::INVALID_DOUBLE;
    fTriggerOffset = larlight::DATA::INVALID_DOUBLE;
    fElectronsToADC = larlight::DATA::INVALID_DOUBLE;
    fNumberTimeSamples = larlight::DATA::INVALID_DOUBLE;
    fReadOutWindowSize = larlight::DATA::INVALID_DOUBLE;

    fTimeOffsetU = larlight::DATA::INVALID_DOUBLE;
    fTimeOffsetV = larlight::DATA::INVALID_DOUBLE;
    fTimeOffsetZ = larlight::DATA::INVALID_DOUBLE;

    fXTicksCoefficient = larlight::DATA::INVALID_DOUBLE;
    fXTicksOffsets.clear();    

  }

  bool DetectorProperties::ReadTree()
  {

    ClearData();
    TChain *ch = new TChain(_tree_name.c_str());
    ch->AddFile(_file_name.c_str());

    std::string error_msg("");
    if(!(ch->GetBranch("fSamplingRate")))      error_msg += "      fSamplingRate\n";
    if(!(ch->GetBranch("fTriggerOffset")))     error_msg += "      fTriggerOffset\n";
    if(!(ch->GetBranch("fElectronsToADC")))    error_msg += "      fElectronsToADC\n";
    if(!(ch->GetBranch("fNumberTimeSamples"))) error_msg += "      fNumberTimeSamples\n";
    if(!(ch->GetBranch("fReadOutWindowSize"))) error_msg += "      fReadOutWindowSize\n";
    if(!(ch->GetBranch("fTimeOffsetU")))       error_msg += "      fTimeOffsetU\n";
    if(!(ch->GetBranch("fTimeOffsetV")))       error_msg += "      fTimeOffsetV\n";
    if(!(ch->GetBranch("fTimeOffsetZ")))       error_msg += "      fTimeOffsetZ\n";
    if(!(ch->GetBranch("fXTicksCoefficient"))) error_msg += "      fXTicksCoefficient\n";
    if(!(ch->GetBranch("fXTicksOffsets")))     error_msg += "      fXTicksOffsets\n";
    if(!error_msg.empty()) {

      throw LArUtilException(Form("Missing following TBranches...\n%s",error_msg.c_str()));

      return false;
    }

    ch->SetBranchAddress("fSamplingRate",&fSamplingRate);
    ch->SetBranchAddress("fTriggerOffset",&fTriggerOffset);
    ch->SetBranchAddress("fElectronsToADC",&fElectronsToADC);
    ch->SetBranchAddress("fNumberTimeSamples",&fNumberTimeSamples);
    ch->SetBranchAddress("fReadOutWindowSize",&fReadOutWindowSize);
    ch->SetBranchAddress("fTimeOffsetU",&fTimeOffsetU);
    ch->SetBranchAddress("fTimeOffsetV",&fTimeOffsetV);
    ch->SetBranchAddress("fTimeOffsetZ",&fTimeOffsetZ);
    ch->SetBranchAddress("fXTicksCoefficient",&fXTicksCoefficient);

    std::vector<Double_t> *pXTicksOffsets=nullptr;
    ch->SetBranchAddress("fXTicksOffsets",&pXTicksOffsets);

    ch->GetEntry(0);

    for(size_t i=0; i<pXTicksOffsets->size(); ++i)
      fXTicksOffsets.push_back(pXTicksOffsets->at(i));

    delete ch;
    return true;
  }

}

#endif
