/**
 * \file DataTypeConstants.hh
 *
 * \ingroup DataFormat
 * 
 * \brief defines data format related constants used in this framework
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/
#ifndef DATATYPECONSTANTS_HH
#define DATATYPECONSTANTS_HH

/// Defines constants for data structure definition (system utility)
namespace DATA{
    
  // Maximum size of C-array
  const UShort_t kMaxTracks      = 300;   ///< Maximum number of tracks to be held in C-array
  const UShort_t kMaxTrackPoints = 10000; ///< Maximum number of track trajectory points to be held in C-array
  const UShort_t kMaxSpacePoints = 10000; ///< Maximum number of spacepoints to be held in C-array
  const UShort_t kMaxHits        = 20000; ///< Maximum number of hits to be held in C-array
  const UShort_t kMaxPrimaries   = 20000; ///< Maximum number of primary particles from generator
  const UShort_t kMaxClusters    = 1000;  ///< Maximum number of clusters

  /// Define identifier for a data container class to be loaded through storage_manager::get_data()
  enum DATA_TYPE{
    Event=0,
    UserInfo,           ///< dynamic data container
    MCTruth,            ///< MCTruth
    CRY_MCTruth,         ///< Cry MCTruth info
    GENIE_MCTruth,       ///< GENIE MCTruth info
    FIFOChannel,        ///< FIFOChannel
    Wire,               ///< Wire
    Hit,                ///< Hit
    CrawlerHit,         ///< ClusterCrawler Hit algo
    GausHit,            ///< Gaus Hit algo
    APAHit,             ///< APA Hit algo
    FFTHit,             ///< FFT Hit algo
    RFFHit,             ///< RFF Hit algo
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
    "mctruth",
    "cry_mctruth",
    "genie_mctruth",
    "pmt",
    "wire",
    "hit",
    "crawlerhit",
    "gaushit",
    "apahit",
    "ffthit",
    "rffhit",
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

#endif
