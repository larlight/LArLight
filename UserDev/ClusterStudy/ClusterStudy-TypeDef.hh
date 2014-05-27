//
// type-def file for ROOT
//

#ifndef CLUSTERSTUDY_TYPEDEF_HH
#define CLUSTERSTUDY_TYPEDEF_HH

// Header files included here

#include "CBookKeeper.hh"
#include "CBoolAlgoBase.hh"
#include "CMergeManager.hh"
#include "CMAlgoShortestDist.hh"
#include "CMAlgoPolyContain.hh"
#include "CMAlgoPolyOverlap.hh"
#include "CMAlgoAngleAlign.hh"
#include "CMAlgoFake.hh"
#include "CMAlgoStartInPoly.hh"
#include "CMAlgoPolyHitOverlap.hh"
#include "CMAlgoStartInCone.hh"

#include "ViewerException.hh"
#include "ClusterViewerAlgo.hh"
#include "ClusterViewer.hh"
#include "ClusterMerger.hh"
#include "MCShowerQuality.hh"
#include "MergeViewer.hh"
#include "MCShowerClusterViewer.hh"
#include "CMAlgoArray.hh"
#include "CMAlgoTrackSeparate.hh"
#include "CMAlgoAngleSeparate.hh"
#include "CMAlgoOutOfConeSeparate.hh"
#include "CMAlgoMergeAll.hh"
#include "CMAlgoStartNearEnd.hh"
#include "CMAlgoStartTrack.hh"
//ADD_NEW_HEADER ... do not change this comment line

// Class forward declaration here
namespace cluster {

  class CBookKeeper;

  class CBoolAlgoBase;
  class CMAlgoArray;

  class CMergeManager;
  class CMAlgoAngleCompat;

  class CMAlgoShortestDist;
  class CMAlgoPolyContain;
  class CMAlgoPolyOverlap;
  class CMAlgoAngleAlign;
  class CMAlgoFake;
  class CMAlgoStartInPoly;
  class CMAlgoPolyHitOverlap;
  class CMAlgoStartInCone;
  class CMAlgoStartNearEnd;
  class CMAlgoStartTrack;
//ADD_CMALGO_CLASS ... do not change this comment line

  class ViewerException;
  class ClusterViewerAlgo;
  class ClusterTrackSeparate;
  class ClusterAngleSeparate;
  class ClusterOutOfConeSeparate;
  class ClusterMergeAll;
}

namespace larlight {

  class ClusterMerger;
  class MCShowerQuality;

  class ClusterViewer;
  class MergeViewer;
  class MCShowerClusterCiewer;
//ADD_ANA_CLASS ... do not change this comment line
}

//ADD_EMPTY_CLASS ... do not change this comment line
#endif








