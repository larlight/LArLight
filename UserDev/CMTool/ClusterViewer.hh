/**
 * \file ClusterViewer.hh
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class ClusterViewer
 *
 * @author kazuhiro/kaleko
 */

/** \addtogroup CMTool

    @{*/

#ifndef CLUSTERVIEWER_HH
#define CLUSTERVIEWER_HH

#include "ClusterViewerAlgo.hh"
#include "GeometryUtilities.hh"
#include "ana_base.hh"
#include <TH2D.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TPad.h>

namespace larlight{
  /**
     \class ClusterViewer
     User custom analysis class made by kazuhiro/kaleko
  */
  class ClusterViewer : public ana_base{
    
  public:
    
    /// Default constructor
    ClusterViewer();
    
    /// Default destructor
    virtual ~ClusterViewer(){};
    
    /** IMPLEMENT in ClusterViewer.cc!
	Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();
    
    /** IMPLEMENT in ClusterViewer.cc! 
	Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);
    
    /** IMPLEMENT in ClusterViewer.cc! 
	Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    /// Setter for a cluster type to be viewed
    virtual void SetClusterType(DATA::DATA_TYPE type);

    /// Function to get number of clusters in the given plane
    size_t ClusterCount(UChar_t plane) {return _algo.ClusterCount(plane); }
    
    /// Function to draw a main canvas (where cluster hits & all-hits are shown)
    void DrawAllClusters() {_algo.DrawAllClusters();}

    /// Function to draw a secondary canvas (where individual cluster is shown)
    void DrawOneCluster(UChar_t plane, size_t index) {_algo.DrawOneCluster(plane,index);}

    /// Function to draw a tertiary canvas w/ two clusters on it
    void DrawTwoClusters(UChar_t plane, size_t index1, size_t index2) {_algo.DrawTwoClusters(plane,index1,index2);}

  protected:
    /// Type of cluster to view
    DATA::DATA_TYPE cluster_type;

    /// Actual viewer code
    ::cluster::ClusterViewerAlgo _algo;
  };
  
}
#endif

/** @} */ // end of doxygen group 
