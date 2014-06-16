/**
 * \file MCShowerClusterViewer.hh
 *
 * \ingroup ClusterStudy
 * 
 * \brief Class def header for a class MCShowerClusterViewer
 *
 * @author kazuhiro/kaleko
 */

/** \addtogroup ClusterStudy

    @{*/

#ifndef MCSHOWERCLUSTERVIEWER_HH
#define MCSHOWERCLUSTERVIEWER_HH

#include "ClusterViewerAlgo.hh"
#include "GeometryUtilities.hh"
#include "ana_base.hh"
#include <TH2D.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TPad.h>

namespace larlight{
  /**
     \class MCShowerClusterViewer
     User custom analysis class made by kazuhiro/kaleko
  */
  class MCShowerClusterViewer : public ana_base{
    
  public:
    
    /// Default constructor
    MCShowerClusterViewer();
    
    /// Default destructor
    virtual ~MCShowerClusterViewer(){};
    
    /** IMPLEMENT in MCShowerClusterViewer.cc!
	Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();
    
    /** IMPLEMENT in MCShowerClusterViewer.cc! 
	Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);
    
    /** IMPLEMENT in MCShowerClusterViewer.cc! 
	Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    /// Function to get number of clusters in the given plane
    size_t ClusterCount(UChar_t plane) {return _algo.ClusterCount(plane); }
    
    /// Function to draw a main canvas (where cluster hits & all-hits are shown)
    void DrawAllClusters();

    /// Function to draw a secondary canvas (where individual cluster is shown)
    void DrawOneCluster(UChar_t plane, size_t index);

  protected:
    /// Type of cluster to view
    DATA::DATA_TYPE cluster_type;

    /// Actual viewer code
    ::cluster::ClusterViewerAlgo _algo;

  };
  
}
#endif

/** @} */ // end of doxygen group 
