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

    const UChar_t  INVALID_UCHAR=0xff;
    const Char_t   INVALID_CHAR=0x7f;
    const UShort_t INVALID_USHORT=0xffff;
    const Short_t  INVALID_SHORT=0x7fff;
    const UInt_t   INVALID_UINT=0xffffffff;
    const Int_t    INVALID_INT=0x7fffffff;
    
    /// Define identifier for a data container class to be loaded through storage_manager::get_data()
    enum DATA_TYPE{
      Event=0,
      MCTruth,            ///< MCTruth
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
      "mc",
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
      "user",
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
