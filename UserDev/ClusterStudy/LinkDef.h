//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class cluster::ClusterViewerAlgo+;
#pragma link C++ class cluster::ViewerException+;
#pragma link C++ class larlight::ClusterViewer+;
#pragma link C++ class larlight::MergeViewer+;
#pragma link C++ class larlight::MCShowerClusterViewer+;

#pragma link C++ class cluster::CMergeManager+;

#pragma link C++ class cluster::CBoolAlgoBase+;
#pragma link C++ class cluster::CMAlgoArray+;

#pragma link C++ class cluster::CBookKeeper+;
#pragma link C++ class std::vector<unsigned short>+;
#pragma link C++ class std::vector<std::vector<unsigned short> >+;
#pragma link C++ class cluster::CMAlgoAngleCompat+;
#pragma link C++ class cluster::CMAlgoShortestDist+;
#pragma link C++ class cluster::CMAlgoPolyContain+;
#pragma link C++ class cluster::CMAlgoPolyOverlap+;
#pragma link C++ class cluster::CMAlgoAngleAlign+;
#pragma link C++ class cluster::CMAlgoFake+;
#pragma link C++ class cluster::CMAlgoStartInPoly+;
#pragma link C++ class cluster::CMAlgoPolyHitOverlap+;
#pragma link C++ class cluster::CMAlgoStartInCone+;
#pragma link C++ class cluster::CMAlgoArray+;
#pragma link C++ class cluster::CMAlgoTrackSeparate+;
#pragma link C++ class cluster::CMAlgoAngleSeparate+;
#pragma link C++ class cluster::CMAlgoOutOfConeSeparate+;
#pragma link C++ class cluster::CMAlgoMergeAll+;
#pragma link C++ class cluster::CMAlgoCenterOfMass+;
#pragma link C++ class cluster::CMAlgoStartNearEnd+;
#pragma link C++ class cluster::CMAlgoStartTrack+;
#pragma link C++ class cluster::CMAlgoProhibitAllTracks+;
#pragma link C++ class cluster::CMAlgoPolyShortestDist+;
#pragma link C++ class cluster::CMAlgoAngleIncompat+;
#pragma link C++ class cluster::CMAlgoMergeTinyWithBig+;
#pragma link C++ class cluster::CMAlgoProhibitBigClusters+;
//ADD_CMALGO_CLASS ... do not change this line

#pragma link C++ class larlight::ClusterMerger+;
#pragma link C++ class larlight::MCShowerQuality+;

//ADD_NEW_CLASS ... do not change this line
#endif


























