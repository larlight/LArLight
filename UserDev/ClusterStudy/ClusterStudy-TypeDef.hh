//
// type-def file for ROOT
//

#ifndef CLUSTERSTUDY_TYPEDEF_HH
#define CLUSTERSTUDY_TYPEDEF_HH

// Header files included here

#include "CMTException.hh"
#include "CMergeBookKeeper.hh"
#include "CMatchBookKeeper.hh"
#include "CBoolAlgoBase.hh"
#include "CFloatAlgoBase.hh"
#include "CMergeManager.hh"
#include "CBAlgoArray.hh"

#include "CBAlgoShortestDist.hh"
#include "CBAlgoPolyContain.hh"
#include "CBAlgoPolyOverlap.hh"
#include "CBAlgoAngleAlign.hh"
#include "CBAlgoFake.hh"
#include "CBAlgoStartInPoly.hh"
#include "CBAlgoPolyHitOverlap.hh"
#include "CBAlgoStartInCone.hh"
#include "CBAlgoTrackSeparate.hh"
#include "CBAlgoAngleSeparate.hh"
#include "CBAlgoOutOfConeSeparate.hh"
#include "CBAlgoMergeAll.hh"
#include "CBAlgoCenterOfMass.hh"
#include "CBAlgoStartNearEnd.hh"
#include "CBAlgoStartTrack.hh"
#include "CBAlgoProhibitAllTracks.hh"
#include "CBAlgoPolyShortestDist.hh"
#include "CBAlgoAngleIncompat.hh"
#include "CBAlgoMergeTinyWithBig.hh"
#include "CBAlgoProhibitBigClusters.hh"
//ADD_NEW_HEADER ... do not change this comment line

#include "ViewerException.hh"
#include "ClusterViewerAlgo.hh"
#include "ClusterViewer.hh"
#include "ClusterMerger.hh"
#include "MCShowerQuality.hh"
#include "MergeViewer.hh"
#include "MCShowerClusterViewer.hh"

// Class forward declaration here
namespace cmtool {

  class CMTException;
  class CBoolAlgoBase;
  class CBAlgoArray;
  class CMergeBookKeeper;
  class CMergeManager;

  class CBAlgoAngleCompat;
  class CBAlgoShortestDist;
  class CBAlgoPolyContain;
  class CBAlgoPolyOverlap;
  class CBAlgoAngleAlign;
  class CBAlgoFake;
  class CBAlgoStartInPoly;
  class CBAlgoPolyHitOverlap;
  class CBAlgoStartInCone;
  class CBAlgoStartNearEnd;
  class CBAlgoStartTrack;
  class CBAlgoProhibitAllTracks;
  class CBAlgoPolyShortestDist;
  class CBAlgoAngleIncompat;
  class CBAlgoMergeTinyWithBig;
  class CBAlgoProhibitBigClusters;
//ADD_CMALGO_CLASS ... do not change this comment line

  class ViewerException;
  class ClusterViewerAlgo;
  class ClusterTrackSeparate;
  class ClusterAngleSeparate;
  class ClusterOutOfConeSeparate;
  class ClusterMergeAll;
  class ClusterCenterOfMass;
}

namespace larlight {

  class ClusterMerger;
  class MCShowerQuality;

  class ClusterViewer;
  class MergeViewer;
  class MCShowerClusterCiewer;
//ADD_ANA_CLASS ... do not change this comment line
}

namespace cmtool {
  class CFloatAlgoBase;
  class CMatchBookKeeper;
//ADD_MATCH_CLASS ... do not change this comment line
}

//ADD_EMPTY_CLASS ... do not change this comment line
#endif
















