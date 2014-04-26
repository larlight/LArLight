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

#pragma link C++ class cluster::CMergeAlgoBase+;
#pragma link C++ class std::vector<cluster::CMergeAlgoBase*>+;

#pragma link C++ class cluster::CBookKeeper+;
#pragma link C++ class std::vector<unsigned short>+;
#pragma link C++ class std::vector<std::vector<unsigned short> >+;

#pragma link C++ class cluster::CMAlgoShortestDist+;
#pragma link C++ class cluster::CMAlgoPolyContain+;
#pragma link C++ class cluster::CMAlgoPolyOverlap+;
#pragma link C++ class cluster::CMAlgoAngleCompat+;
#pragma link C++ class cluster::CMAlgoFake+;
#pragma link C++ class cluster::CMAlgoStartInPoly+;
#pragma link C++ class cluster::CMAlgoPolyHitOverlap+;
#pragma link C++ class cluster::CMAlgoStartInCone+;

#pragma link C++ class larlight::ClusterMerger+;




//ADD_NEW_CLASS ... do not change this line
#endif














