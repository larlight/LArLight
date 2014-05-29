/**
 * \file MergeViewer.hh
 *
 * \ingroup ClusterStudy
 * 
 * \brief Class def header for a class MergeViewer
 *
 * @author kazuhiro/kaleko
 */

/** \addtogroup ClusterStudy

    @{*/

#ifndef MERGEVIEWER_HH
#define MERGEVIEWER_HH

#include "ClusterViewerAlgo.hh"
#include "ClusterMerger.hh"

namespace larlight{
  /**
     \class MergeViewer
     User custom analysis class made by kazuhiro/kaleko
  */
  class MergeViewer : public ClusterMerger {
    
  public:
    
    /// Default constructor
    MergeViewer();
    
    /// Default destructor
    virtual ~MergeViewer(){};
    
    /** IMPLEMENT in MergeViewer.cc!
	Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();
    
    /** IMPLEMENT in MergeViewer.cc! 
	Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);
    
    /** IMPLEMENT in MergeViewer.cc! 
	Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    /// Function to get number of clusters in the given plane
    size_t ClusterCount(UChar_t plane) {return _algo.ClusterCount(plane); }
    
    /// Function to draw a main canvas (where cluster hits & all-hits are shown)
    void DrawAllClusters() {_algo.DrawAllClusters();}

    /// Function to draw a secondary canvas (where individual cluster is shown)
    void DrawOneCluster(UChar_t plane, size_t index) {
      _algo.DrawOneCluster(plane,index);
      if(_print_on_DrawOneCluster) PrintClusterInfo(plane,index);
    }

    /// Function to draw a tertiary canvas w/ two clusters on it
    void DrawTwoClusters(UChar_t plane, size_t index1, size_t index2) {_algo.DrawTwoClusters(plane,index1,index2);}

    /// Function to disable drawing of polygon if you want
    void SetDrawPolygon(bool flag) { _draw_polygon = flag; }

    /// Function to print cluster params to screen if user wants to 
    void PrintClusterInfo(UChar_t plane, size_t index);

    /// Function to set whether user wants to print cluster info
    void SetPrintClusterInfo(bool flag) { _print_on_DrawOneCluster = flag; }

  protected:

    ::cluster::ClusterViewerAlgo _algo;

    bool _draw_polygon;

    bool _print_on_DrawOneCluster;
  };
  
}
#endif

/** @} */ // end of doxygen group 