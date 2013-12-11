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
    ShowerAngleCluster, ///< ShowerAngleCluster
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
    "showeranglecluster",
    "seed",
    "sps",
    "track",
    "bezier",
    "kalman3dsps",
    "kalman3dhit",
    "shower",
    "calorimetry"
  };

  /// Invalid (or initialized) channel number  
  const DATA::ch_number_t INVALID_CH=0xffff;

}

/// Defines constants for PMT related electronics
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

/// Defines constants for MC truth information
namespace MC{

  /// MC generator type
  enum Origin_t{
    kUnknown,           ///< ???
    kBeamNeutrino,      ///< Beam neutrinos
    kCosmicRay,         ///< Cosmic rays
    kSuperNovaNeutrino, ///< Supernova neutrinos
    kSingleParticle     ///< single particles thrown at the detector
  };
}

/// Defines constants for geometry
namespace GEO{

  /// Enumerate the possible plane projections
  enum View_t {
    kU,       ///< planes which measure U
    kV,       ///< planes which measure V
    kW,       ///< soon to be deprecated, planes which measure W (third view for Bo, MicroBooNE, etc)
    kZ=kW,    ///< planes which measure Z direction (ie wires are vertical)
    k3D,      ///< 3 dimensional objects, potentially hits, clusters, prongs, etc
    kUnknown  ///< unknown view
  };

  /// Enumerate the possible signal types
  enum SigType_t {
    kInduction,   ///< signal from induction planes
    kCollection,  ///< signal from collection planes
    kMysteryType  ///< who knows?
  };

}

#endif
/** @} */ // end of doxygen group
