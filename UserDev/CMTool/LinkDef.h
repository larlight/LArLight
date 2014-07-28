//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class std::vector<unsigned int>+;
#pragma link C++ class std::vector<std::vector<unsigned int> >+;
#pragma link C++ class std::vector<unsigned short>+;
#pragma link C++ class std::vector<std::vector<unsigned short> >+;

#pragma link C++ class cluster::ClusterViewerAlgo+;
#pragma link C++ class cluster::ViewerException+;
#pragma link C++ class larlight::ClusterViewer+;
#pragma link C++ class larlight::MergeViewer+;
#pragma link C++ class larlight::MCShowerClusterViewer+;

#pragma link C++ class cmtool::CMTException+;
#pragma link C++ class cmtool::CMergeBookKeeper+;
#pragma link C++ class cmtool::CMatchBookKeeper+;
#pragma link C++ class cmtool::CMAlgoBase+;
#pragma link C++ class cmtool::CBoolAlgoBase+;
#pragma link C++ class cmtool::CFloatAlgoBase+;
#pragma link C++ class cmtool::CPriorityAlgoBase+;
#pragma link C++ class cmtool::CMManagerBase;
#pragma link C++ class cmtool::CMergeManager+;
#pragma link C++ class cmtool::CMatchManager+;

#pragma link C++ class cmtool::CPAlgoArray+;
#pragma link C++ class cmtool::CPAlgoNHits+;
#pragma link C++ class cmtool::CPAlgoQSum+;
#pragma link C++ class cmtool::CPAlgoPolyArea+;
#pragma link C++ class cmtool::CPAlgoIgnoreTracks+;
//ADD_CPALGO_CLASS ... do not change this line

#pragma link C++ class cmtool::CBAlgoArray+;
#pragma link C++ class cmtool::CBAlgoAngleCompat+;
#pragma link C++ class cmtool::CBAlgoShortestDist+;
#pragma link C++ class cmtool::CBAlgoPolyContain+;
#pragma link C++ class cmtool::CBAlgoPolyOverlap+;
#pragma link C++ class cmtool::CBAlgoAngleAlign+;
#pragma link C++ class cmtool::CBAlgoFake+;
#pragma link C++ class cmtool::CBAlgoStartInPoly+;
#pragma link C++ class cmtool::CBAlgoPolyHitOverlap+;
#pragma link C++ class cmtool::CBAlgoStartInCone+;
#pragma link C++ class cmtool::CBAlgoArray+;
#pragma link C++ class cmtool::CBAlgoTrackSeparate+;
#pragma link C++ class cmtool::CBAlgoAngleSeparate+;
#pragma link C++ class cmtool::CBAlgoOutOfConeSeparate+;
#pragma link C++ class cmtool::CBAlgoMergeAll+;
#pragma link C++ class cmtool::CBAlgoCenterOfMass+;
#pragma link C++ class cmtool::CBAlgoCenterOfMassSmall+;
#pragma link C++ class cmtool::CBAlgoStartNearEnd+;
#pragma link C++ class cmtool::CBAlgoStartTrack+;
#pragma link C++ class cmtool::CBAlgoProhibitAllTracks+;
#pragma link C++ class cmtool::CBAlgoPolyShortestDist+;
#pragma link C++ class cmtool::CBAlgoAngleIncompat+;
#pragma link C++ class cmtool::CBAlgoMergeTinyWithBig+;
#pragma link C++ class cmtool::CBAlgoProhibitBigClusters+;
//ADD_CBALGO_CLASS ... do not change this line

#pragma link C++ class cmtool::CFAlgoArray+;
#pragma link C++ class cmtool::CFAlgoQRatio+;
#pragma link C++ class cmtool::CFAlgoTimeOverlap+;
#pragma link C++ class cmtool::CFAlgoZOverlap+;
#pragma link C++ class cmtool::CFAlgoStartPointMatch+;
#pragma link C++ class cmtool::CFAlgoStartPointCompat+;
#pragma link C++ class cmtool::CFAlgo3DAngle+;
//ADD_CFALGO_CLASS ... do not change this line


#pragma link C++ class larlight::ClusterMerger+;
#pragma link C++ class larlight::ClusterMatcher+;
#pragma link C++ class larlight::MCShowerQuality+;
#pragma link C++ class larlight::MatchViewer+;
//ADD_NEW_CLASS ... do not change this line
#endif









































