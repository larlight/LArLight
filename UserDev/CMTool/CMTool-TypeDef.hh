//
// type-def file for ROOT
//

#ifndef CMTOOL_TYPEDEF_HH
#define CMTOOL_TYPEDEF_HH

// Header files included here

#include "CMTException.hh"
#include "CMergeBookKeeper.hh"
#include "CMatchBookKeeper.hh"
#include "CMAlgoBase.hh"
#include "CBoolAlgoBase.hh"
#include "CFloatAlgoBase.hh"
#include "CPriorityAlgoBase.hh"
#include "CMManagerBase.hh"
#include "CMergeManager.hh"
#include "CMatchManager.hh"

#include "CPAlgoArray.hh"
#include "CPAlgoNHits.hh"
#include "CPAlgoQSum.hh"
#include "CPAlgoPolyArea.hh"
#include "CPAlgoIgnoreTracks.hh"
//ADD_CPALGO_HEADER

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
#include "CBAlgoCenterOfMassSmall.hh"
#include "CBAlgoStartNearEnd.hh"
#include "CBAlgoStartTrack.hh"
#include "CBAlgoProhibitAllTracks.hh"
#include "CBAlgoPolyShortestDist.hh"
#include "CBAlgoAngleIncompat.hh"
#include "CBAlgoMergeTinyWithBig.hh"
#include "CBAlgoProhibitBigClusters.hh"
//ADD_CBALGO_HEADER

#include "CFAlgoArray.hh"
#include "CFAlgoQRatio.hh"
#include "CFAlgoTimeOverlap.hh"
#include "CFAlgoZOverlap.hh"
#include "CFAlgoStartPointMatch.hh"
#include "CFAlgoStartPointCompat.hh"
#include "CFAlgoStartTimeCompat.hh"
#include "CFAlgo3DAngle.hh"
#include "CFAlgoWireOverlap.hh"
#include "CFAlgoVolumeOverlap.hh"
//ADD_CFALGO_HEADER

#include "MatchViewer.hh"
//ADD_NEW_HEADER ... do not change this comment line

#include "ViewerException.hh"
#include "ClusterViewerAlgo.hh"
#include "ClusterViewer.hh"
#include "ClusterMerger.hh"
#include "ClusterMatcher.hh"
#include "MCShowerQuality.hh"
#include "MergeViewer.hh"
#include "MCShowerClusterViewer.hh"

// Class forward declaration here
namespace cmtool {

  class CMTException;
  class CMAlgoBase;
  class CBoolAlgoBase;
  class CFloatAlgoBase;
  class CMergeBookKeeper;
  class CMatchBookKeeper;
  class CMManagerBase;
  class CMergeManager;

  class CPAlgoArray;
  class CPAlgoNHits;
  class CPAlgoQSum;
  class CPAlgoPolyArea;
  class CPAlgoIgnoreTracks;
//ADD_CPALGO_CLASS ... do not change this comment line

  class CBAlgoArray;
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
  class CBAlgoCenterOfMass;
  class CBAlgoCenterOfMassSmall;
  class CBAlgoMergeAll;
  class CBAlgoOutOfConeSeparate;
  class CBAlgoTrackSeparate;
  class CBAlgoAngleSeparate;
//ADD_CBALGO_CLASS ... do not change this comment line

  class CFAlgoArray;
  class CFAlgoQRatio;
  class CFAlgoTimeOverlap;
  class CFAlgoZOverlap;
  class CFAlgoStartPointMatch;
  class CFAlgoStartPointCompat;
  class CFAlgoStartTimeCompat;
  class CFAlgo3DAngle;
  class CFAlgoWireOverlap;
  class CFAlgoVolumeOverlap;
//ADD_CFALGO_CLASS ... do not change this comment line

}

namespace cluster {
  class ViewerException;
  class ClusterViewerAlgo;
}

namespace larlight {

  class ClusterMerger;
  class ClusterMatcher;
  class MCShowerQuality;

  class ClusterViewer;
  class MergeViewer;
  class MCShowerClusterCiewer;
  class MatchViewer;
//ADD_ANA_CLASS ... do not change this comment line
}


//ADD_EMPTY_CLASS ... do not change this comment line
#endif





























