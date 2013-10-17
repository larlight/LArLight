/**
 * \file constants.hh
 *
 * \ingroup Base
 * 
 * \brief defines basic constants used in this framework
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Base

    @{*/

#ifndef CONSTANTS_HH
#define CONSTANTS_HH
#include <string>
#include "types.hh"

/// Defines constants for Message utility
namespace MSG{

  /// Defines message level
  enum Level{
    DEBUG=0,      ///< Message level ... useful to debug a crash
    INFO,         ///< Debug info but not the lowest level
    NORMAL,       ///< Normal stdout
    WARNING,      ///< notify a user in the standard operation mode for an important finding.
    ERROR,        ///< notify a user when something is clearly wrong
    MSG_TYPE_MAX
  };

  const std::string ColorPrefix[MSG_TYPE_MAX] =
    {
      "\033[94m", ///< blue ... DEBUG   
      "\033[92m", ///< green ... INFO
      "\033[95m", ///< magenta ... NORMAL
      "\033[93m", ///< yellow ... WARNING
      "\033[91m"  ///< red ... ERROR
    };
  ///< Color coding of message

  const std::string StringPrefix[MSG_TYPE_MAX] =
    {
      "     [DEBUG]  ", ///< DEBUG message prefix
      "      [INFO]  ", ///< INFO message prefix
      "    [NORMAL]  ", ///< NORMAL message prefix
      "   [WARNING]  ", ///< WARNING message prefix 
      "     [ERROR]  "  ///< ERROR message prefix
    };
  ///< Prefix of message
}


namespace DATA{

  /// Define identifier for a data container class to be loaded through storage_manager::get_data()
  enum DATA_TYPE{
    Event=0,
    UserInfo,           ///< dynamic data container
    FIFOChannel,        ///< FIFOChannel
    Wire,               ///< Wire
    Hit,                ///< Hit
    Track,              ///< Track
    Shower,             ///< Shower
    Cluster,            ///< Cluster
    Calorimetry,        ///< Calorimetry
    DATA_TYPE_MAX
  };

  /// Define tree name
  const std::string DATA_TREE_NAME[DATA_TYPE_MAX] = {
    "event",
    "user_info",
    "fifo_channel",
    "wire",
    "hit",
    "track",
    "shower",
    "cluster",
    "calorimetry"
  };

  /// Invalid (or initialized) channel number  
  const DATA::ch_number_t INVALID_CH=0xffff;

}

namespace PMT{

  /// PMT discriminator type
  enum DISCRIMINATOR {
    BASE_DISC = 0, ///< lowest disc. threshold so-called disc. 0
    COSMIC_DISC,   ///< discriminator for cosmics
    MICHEL_DISC,   ///< discriminator for michel electrons (not used, just a place hlder)
    BEAM_DISC,     ///< discriminator for signal within the beam window
    BEAM_WIN,      ///< discriminator for beam window (forced N samples read-out)
    DISC_MAX
  };
}

#endif
/** @} */ // end of doxygen group
