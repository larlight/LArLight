#ifndef DATATYPECONSTANTS_HH
#define DATATYPECONSTANTS_HH
/**
 * \file DataTypeConstants.hh
 *
 * \ingroup SimpleTreeData
 * 
 * @author Kazu - Nevis 2013
 */

/** \addtogroup SimpleTreeData

    @{*/

/// Defines constants for data structure definition (system utility)
namespace DATA{
    
  // Maximum size of C-array
  const UShort_t kMaxTracks       = 300;    ///< Maximum number of tracks to be held in C-array
  const UShort_t kMaxTrackPoints  = 10000;  ///< Maximum number of track trajectory points to be held in C-array
  const UShort_t kMaxSpacePoints  = 10000;  ///< Maximum number of spacepoints to be held in C-array
  const UShort_t kMaxHits         = 20000;  ///< Maximum number of hits to be held in C-array
  const UShort_t kMaxPrimaries    = 20000;  ///< Maximum number of primary particles from generator
  const UShort_t kMaxClusters     = 1000;   ///< Maximum number of clusters
  const UChar_t  kMaxNeutrinos    = 100;    ///< Maximum number of neutrinos per event produced by a generator
  const UShort_t kMaxParticles    = 10000;  ///< Maximum number of particles from G4 tracking info
  const UInt_t   kMaxTrajectories = 0xffff; ///< Maximum number of particle's trajectory points that can be stored
  const UShort_t kMaxPMTFIFO      = 2000;   ///< Maximum number of discriminator fire
  const UInt_t   kMaxPMTWords     = 40 * 3000; ///< Maximum number of ADC samples to store (2 beamgate per channel)

  const std::string TREE_NAME = "anatree";

  /// Define identifier for a data container class to be loaded through storage_manager::get_data()
  enum DATA_TYPE{
    Event=0,
    UserInfo,           ///< dynamic data container
    MCNeutrino,         ///< LArSoft MCNeutrino
    GENIE_MCNeutrino,   ///< GENIE MCNeutrino
    MCTruth,            ///< LArSoft MCTruth
    CRY_MCTruth,        ///< Cry MCTruth info
    GENIE_MCTruth,      ///< GENIE MCTruth info
    MCParticle,         ///< LArSoft MCParticle
    MCTrajectory,       ///< LArSoft MCTrajectory
    FIFOChannel,        ///< LArSoft FIFOChannel
    Wire,               ///< LArSoft Wire
    Hit,                ///< LArSoft Hit
    CrawlerHit,         ///< ClusterCrawler Hit algo
    GausHit,            ///< Gaus Hit algo
    APAHit,             ///< APA Hit algo
    FFTHit,             ///< FFT Hit algo
    RFFHit,             ///< RFF Hit algo
    Cluster,            ///< LArSoft Cluster
    FuzzyCluster,       ///< Fuzzy Cluster
    DBCluster,          ///< DBCluster
    CrawlerCluster,     ///< ClusterCrawler
    HoughCluster,       ///< LArSoft Hough
    Seed,               ///< LArSoft Seed
    SpacePoint,         ///< LArSoft Spacepoints
    Track,              ///< LArSoft Track
    Bezier,             ///< Track (Bezier)
    Kalman3DSPS,        ///< Track (Kalman3DSPS)
    Kalman3DHit,        ///< Track (Kalman3DHit)
    Shower,             ///< LArSoft Shower
    Calorimetry,        ///< LArSoft Calorimetry
    DATA_TYPE_MAX
  };

  /// Define tree name
  const std::string DATA_TREE_NAME[DATA_TYPE_MAX] = {
    "event",
    "user",
    "mcnu",
    "genie_mcnu",
    "mctruth",
    "cry_mctruth",
    "genie_mctruth",
    "mcpart",
    "mcstep",
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
    "k3dsps",
    "k3dhit",
    "shower",
    "calo"
  };

}

#endif
/** @} */ // end of doxygen group 
