//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class Polygon+;

#pragma link C++ namespace cluster+;

#pragma link C++ class std::vector<const larutil::PxHit*>+;

#pragma link C++ class cluster::ClusterParamsAlgNew+;
#pragma link C++ class std::vector<cluster::ClusterParamsAlgNew>+;

#pragma link C++ class cluster::cluster_params+;
#pragma link C++ class std::vector<cluster::cluster_params>+;

#pragma link C++ class cluster::ClusterParamsExecutor+;
#pragma link C++ class std::vector<cluster::ClusterParamsExecutor>+;

#pragma link C++ class cluster::RecoUtilException+;
#pragma link C++ class cluster::SimChannelViewer+;
#pragma link C++ class cluster::WireViewer+;
#pragma link C++ class cluster::ShowerReco2DAlgo+;
#pragma link C++ class cluster::CMergeManager+;

#pragma link C++ class cluster::CMergeAlgoBase+;
#pragma link C++ class std::vector<cluster::CMergeAlgoBase*>+;

#pragma link C++ class cluster::CBookKeeper+;
#pragma link C++ class cluster::CMAlgoShortestDist+;
//ADD_NEW_CLASS ... do not change this line
#endif









