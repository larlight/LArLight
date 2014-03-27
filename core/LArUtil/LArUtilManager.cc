#ifndef LARUTILMANAGER_CC
#define LARUTILMANAGER_CC

#include "LArUtilManager.hh"

namespace larutil {

  bool LArUtilManager::Reconfigure(larlight::GEO::DetId_t type)
  {

    if(type == LArUtilConfig::Detector()) return true;

    bool status = LArUtilConfig::SetDetector(type);

    if(!status) return status;

    return ReconfigureUtilities();

  }

  bool LArUtilManager::ReconfigureUtilities()
  {
    bool status = true;

    // Geometry
    larlight::Message::send(larlight::MSG::NORMAL,__FUNCTION__,"Reconfiguring Geometry");
    Geometry* geom = (Geometry*)(Geometry::GetME(false));
    geom->SetFileName(Form("%s/LArUtil/dat/%s",
			   getenv("LARLIGHT_CORE_DIR"),
			   kUTIL_DATA_FILENAME[LArUtilConfig::Detector()].c_str()));
    geom->SetTreeName(kTREENAME_GEOMETRY);
    status = status && geom->LoadData(true);

    // LArProperties
    larlight::Message::send(larlight::MSG::NORMAL,__FUNCTION__,"Reconfiguring LArProperties");
    LArProperties* larp = (LArProperties*)(LArProperties::GetME(false));
    larp->SetFileName(Form("%s/LArUtil/dat/%s",
			   getenv("LARLIGHT_CORE_DIR"),
			   kUTIL_DATA_FILENAME[LArUtilConfig::Detector()].c_str()));
    larp->SetTreeName(kTREENAME_LARPROPERTIES);
    status = status && larp->LoadData(true);

    // DetectorProperties
    larlight::Message::send(larlight::MSG::NORMAL,__FUNCTION__,"Reconfiguring DetectorProperties");
    DetectorProperties* detp = (DetectorProperties*)(DetectorProperties::GetME(false));
    detp->SetFileName(Form("%s/LArUtil/dat/%s",
			   getenv("LARLIGHT_CORE_DIR"),
			   kUTIL_DATA_FILENAME[LArUtilConfig::Detector()].c_str()));
    detp->SetTreeName(kTREENAME_DETECTORPROPERTIES);
    status = status && detp->LoadData(true);

    if(status){

      // GeometryUtilities
      larlight::Message::send(larlight::MSG::NORMAL,__FUNCTION__,"Reconfiguring GeometryUtilities...");
      GeometryUtilities* gser = (GeometryUtilities*)(GeometryUtilities::GetME());
      gser->Reconfigure();

    }

    return status;
  }

}

#endif
