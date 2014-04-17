/**
 * \file UBTriggerTypes.h
 *
 * \ingroup TriggerSim
 * 
 * \brief Defining constants used in MicroBooNE TriggerSim
 *
 * @author kterao
 */

/** \addtogroup TriggerSim

    @{*/
#ifndef UBTRIGGERTYPES_H
#define UBTRIGGERTYPES_H

namespace trigger{

  /// Trigger bits for microboone trigger data product
  typedef enum _ubtrigger_t {
    kPMTTriggerBeam   = 0, ///< PMT-FEM issued beam trigger
    kPMTTriggerCosmic = 1, ///< PMT-FEM issued cosmic trigger
    kPMTTrigger   = 7,     ///< PMT-FEM issued any trigger
    kTriggerPC    = 8,     ///< DAQ issued trigger
    kTriggerEXT   = 9,     ///< EXT input to the trigger module front face
    kActive       = 10,    ///< Active flag which is enabled during cosmic-allow region 
    kTriggerBNB   = 11,    ///< BNB input to the trigger module front face
    kTriggerNuMI  = 12,    ///< NuMI input to the trigger module front face
    kVeto         = 13,    ///< Veto flag which is enabled through trigger module front face
    kTriggerCalib = 14     ///< Calib input to the trigger module front face
  } UBTrigger_t;


}
#endif
/** @} */ // end of doxygen group 
