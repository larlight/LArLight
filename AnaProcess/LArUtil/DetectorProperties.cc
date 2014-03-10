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
			getenv("ANA_PROC_DIR"),
			kFILENAME_DETECTORPROPERTIES.c_str());
    if(_tree_name.empty())
      _tree_name = kTREENAME_DETECTORPROPERTIES;
    _name = "DetectorProperties";
    LoadData();
  }

  void DetectorProperties::SetBranchAddress()
  {

    if(!_data_tree) {
      throw LArUtilException(Form("DataTree has to be specified prior to %s function call",__FUNCTION__));
      return;
    }

    std::string error_msg("");
    if(!(_data_tree->GetBranch("fSamplingRate")))      error_msg += "      fSamplingRate\n";
    if(!(_data_tree->GetBranch("fTriggerOffset")))     error_msg += "      fTriggerOffset\n";
    if(!(_data_tree->GetBranch("fElectronsToADC")))    error_msg += "      fElectronsToADC\n";
    if(!(_data_tree->GetBranch("fNumberTimeSamples"))) error_msg += "      fNumberTimeSamples\n";
    if(!(_data_tree->GetBranch("fReadOutWindowSize"))) error_msg += "      fReadOutWindowSize\n";
    if(!(_data_tree->GetBranch("fTimeOffsetU")))       error_msg += "      fTimeOffsetU\n";
    if(!(_data_tree->GetBranch("fTimeOffsetV")))       error_msg += "      fTimeOffsetV\n";
    if(!(_data_tree->GetBranch("fTimeOffsetZ")))       error_msg += "      fTimeOffsetZ\n";
    if(!(_data_tree->GetBranch("fXTicksCoefficient"))) error_msg += "      fXTicksCoefficient\n";
    if(!(_data_tree->GetBranch("fXTicksOffsets")))     error_msg += "      fXTicksOffsets\n";
    if(!error_msg.empty()) {

      throw LArUtilException(Form("Missing following TBranches...\n%s",error_msg.c_str()));

      return;
    }

    _data_tree->SetBranchAddress("fSamplingRate",&fSamplingRate);
    _data_tree->SetBranchAddress("fTriggerOffset",&fTriggerOffset);
    _data_tree->SetBranchAddress("fElectronsToADC",&fElectronsToADC);
    _data_tree->SetBranchAddress("fNumberTimeSamples",&fNumberTimeSamples);
    _data_tree->SetBranchAddress("fReadOutWindowSize",&fReadOutWindowSize);
    _data_tree->SetBranchAddress("fTimeOffsetU",&fTimeOffsetU);
    _data_tree->SetBranchAddress("fTimeOffsetV",&fTimeOffsetV);
    _data_tree->SetBranchAddress("fTimeOffsetZ",&fTimeOffsetZ);
    _data_tree->SetBranchAddress("fXTicksCoefficient",&fXTicksCoefficient);
    _data_tree->SetBranchAddress("fXTicksOffsets",&fXTicksOffsets);

  }

}

#endif
