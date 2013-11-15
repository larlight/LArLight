/**
 * \file constants.hh
 *
 * \ingroup SimpleTreeBase
 * 
 * \brief defines basic constants used in this framework
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup SimpleTreeBase

    @{*/

#ifndef CONSTANTS_HH
#define CONSTANTS_HH
#include <string>
#include "types.hh"

/// Defines constants for data structure definition (system utility)
namespace DATA{

  /// Define identifier for a data container class to be loaded through storage_manager::get_data()
  enum DATA_TYPE{
    Event=0,
    UserInfo,           ///< dynamic data container
    MCTruth,            ///< MCTruth
    FIFOChannel,        ///< FIFOChannel
    Wire,               ///< Wire
    Hit,                ///< Hit
    CrawlerHit,         ///< ClusterCrawler Hit algo
    GausHit,            ///< Gaus Hit algo
    APAHit,             ///< APA Hit algo
    FFTHit,             ///< FFT Hit algo
    Cluster,            ///< Cluster
    FuzzyCluster,       ///< Fuzzy Cluster
    DBCluster,          ///< DBCluster
    CrawlerCluster,     ///< Crawler
    HoughCluster,       ///< Hough
    Seed,               ///< Seed
    SpacePoint,         ///< Spacepoints
    Track,              ///< Track
    Bezier,             ///< Track (Bezier)
    Kalman3DSPS,        ///< Track (Kalman3DSPS)
    Kalman3DHit,        ///< Track (Kalman3DHit)
    Shower,             ///< Shower
    Calorimetry,        ///< Calorimetry
    DATA_TYPE_MAX
  };

  /// Define tree name
  const std::string DATA_TREE_NAME[DATA_TYPE_MAX] = {
    "event",
    "user",
    "mc",
    "pmt",
    "wire",
    "hit",
    "crawlerhit",
    "gaushit",
    "apahit",
    "ffthit",
    "cluster",
    "fuzzycluster",
    "dbcluster",
    "crawlercluster",
    "houghcluster",
    "seed",
    "sps",
    "track",
    "bezier",
    "kalman3dsps",
    "kalman3dhit",
    "shower",
    "calorimetry"
  };

}

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

#endif
/** @} */ // end of doxygen group
