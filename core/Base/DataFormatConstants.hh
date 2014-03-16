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
#include <limits>
#include <climits>
#include <Rtypes.h>

/// Namespace of everything in this framework
namespace larlight{
  
  /// Defines constants for data structure definition (system utility)
  namespace DATA{

    const UChar_t  INVALID_UCHAR  = std::numeric_limits<UChar_t>::max();
    const Char_t   INVALID_CHAR   = std::numeric_limits<Char_t>::max();
    const UShort_t INVALID_USHORT = std::numeric_limits<UShort_t>::max();
    const Short_t  INVALID_SHORT  = std::numeric_limits<Short_t>::max();
    const UInt_t   INVALID_UINT   = std::numeric_limits<UInt_t>::max();
    const Int_t    INVALID_INT    = std::numeric_limits<Int_t>::max();

    const Double_t INVALID_DOUBLE = std::numeric_limits<Double_t>::max();
    const Double_t INVALID_FLOAT  = std::numeric_limits<Float_t>::max();
    
    /// Define identifier for a data container class to be loaded through storage_manager::get_data()
    enum DATA_TYPE{
      Event=0,
      MCTruth,            ///< MCTruth 
      MCParticle,         ///< MCParticle
      MCTrajectory,       ///< MCTrajectory
      MCNeutrino,         ///< MCNeutrino
      MCShower,           ///< LArSoft utility class MCShower
      SimChannel,         ///< SimChannel
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
      Vertex,             ///< Vertex
      FeatureVertex,      ///< FeatureVertex
      HarrisVertex,       ///< HarrisVertex
      EndPoint2D,         ///< EndPoint2D
      FeatureEndPoint2D,  ///< EndPoint2D from Feature Vertex Finder
      HarrisEndPoint2D,   ///< EndPoint2D from Harris Vertex Finder 
      Calorimetry,        ///< Calorimetry
      UserInfo,           ///< dynamic data container ... this must be the last valid enum element

      /*
	Following enums are LArLight original except FIFOChannel which
	is meant to be compatible with LArSoft FIFOChannel
      */
      FIFO,               ///< LArSoft waveform 
      PMTFIFO,            ///< PMT channel waveform data
      TPCFIFO,            ///< TPC channel waveform data
      Pulse,              ///< Generic event_pulse
      PMTPulse_ThresWin,  ///< Reconstructed event_pulse for PMT waveform
      PMTPulse_FixedWin,  ///< Reconstructed event_pulse for PMT waveform
      TPCPulse_ThresWin,  ///< reconstructed event_pulse for TPC waveform
      TPCPulse_FixedWin,  ///< reconstructed event_pulse for TPC waveform
      Trigger,            ///< Trigger data

      DATA_TYPE_MAX
    };
    
    /// Define tree name
    const std::string DATA_TREE_NAME[DATA_TYPE_MAX] = {
      "event",
      "mctruth",
      "mcpart",
      "mctrajectory",
      "mcnu",
      "mcshower",
      "simch",
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
      "vertex",
      "feature_vtx",
      "harris_vtx",
      "end2d",
      "feature_end2d",
      "harris_end2d",
      "calorimetry",
      "user",
      "fifo",
      "pmtfifo",
      "tpcfifo",
      "pulse",
      "pmt_threswin",
      "pmt_fixedwin",
      "tpc_threswin",
      "tpc_fixedwin",
      "trigger"
    };
    
  }
  
}
#endif
/** @} */ // end of doxygen group
