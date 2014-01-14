/**
 * \file DataFormatConstants.hh
 *
 * \ingroup DataFormat
 * 
 * \brief defines constants used in DataFormat
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Base

    @{*/

#ifndef DATAFORMATCONSTANTS_HH
#define DATAFORMATCONSTANTS_HH
#include <string>

/// Namespace of everything in this framework
namespace larlight{
  
  /// Defines constants for data structure definition (system utility)
  namespace DATA{
    
    /// Define identifier for a data container class to be loaded through storage_manager::get_data()
    enum DATA_TYPE{
      Event=0,
      MCTruth,            ///< MCTruth
      PMTFIFO,            ///< LArSoft PMT waveform (FIFOChannel)
      TPCFIFO,            ///< Equivalent FIFOChannel for TPC
      TRIGGER,            ///< Trigger data
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
      ShowerAngleCluster, ///< ShowerAngleCluster
      Seed,               ///< Seed
      SpacePoint,         ///< Spacepoints
      Track,              ///< Track
      Bezier,             ///< Track (Bezier)
      Kalman3DSPS,        ///< Track (Kalman3DSPS)
      Kalman3DHit,        ///< Track (Kalman3DHit)
      Shower,             ///< Shower
      Calorimetry,        ///< Calorimetry
      UserInfo,           ///< dynamic data container ... this must be the last valid enum element
      DATA_TYPE_MAX
    };
    
    /// Define tree name
    const std::string DATA_TREE_NAME[DATA_TYPE_MAX] = {
      "event",
      "mc",
      "pmtfifo",
      "tpcfifo",
      "trigger",
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
      "showeranglecluster",
      "seed",
      "sps",
      "track",
      "bezier",
      "kalman3dsps",
      "kalman3dhit",
      "shower",
      "calorimetry",
      "user"
    };
    
  }
  
}
#endif
/** @} */ // end of doxygen group
