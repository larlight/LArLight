/**
 * \file ClusterViewerAlgo.hh
 *
 * \ingroup ClusterStudy
 * 
 * \brief Class def header for a class ClusterViewerAlgo
 *
 * @author kazuhiro/kaleko
 */

/** \addtogroup ClusterStudy

    @{*/

#ifndef CLUSTERVIEWERALGO_HH
#define CLUSTERVIEWERALGO_HH

#include "ViewerException.hh"
#include "Geometry.hh"
#include "GeometryUtilities.hh"
#include <TH2D.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TPad.h>

namespace cluster {
  /**
     \class ClusterViewerAlgo
     User custom analysis class made by kazuhiro/kaleko
  */
  class ClusterViewerAlgo {
    
  public:
    
    /// Default constructor
    ClusterViewerAlgo(std::string name="DeafultViewer");
    
    /// Default destructor
    virtual ~ClusterViewerAlgo(){};

    /// Resets settings
    void Reset();

    /**
       Function to check if it is ready to take data using AddXXX functions
       If raise is set to true, this function throws an exception.
    */
    bool ReadyTakeData(bool raise=false) const;

    /// Setter for x and y range ... must be called before any of AddXXX functions and/or after Reset()
    void SetRange(UChar_t plane, 
		  const std::pair<double,double> &xrange,
		  const std::pair<double,double> &yrange);

    /// Appender for hits
    void AddHits(const UChar_t plane, 
		 const std::vector<std::pair<double,double> > &hits_xy,
		 const std::vector<double> &hits_charge);

    /// Appender for clusters
    void AddCluster(const UChar_t plane,
		    const std::vector<std::pair<double,double> > &cluster_hits);

    /// Appender for clusters
    void AddCluster(const UChar_t plane,
		    const std::vector<std::pair<double,double> > &cluster_hits,
		    const std::pair<double,double> &cluster_start,
		    const std::pair<double,double> &cluster_end);

    /// Appender for clusters
    void AddCluster(const UChar_t plane,
		    const std::vector<std::pair<double,double> > &cluster_hits,
		    const std::vector<std::pair<double,double> > &cluster_polygon);

    /// Appender for clusters
    void AddCluster(const UChar_t plane,
		    const std::vector<std::pair<double,double> > &cluster_hits,
		    const std::pair<double,double> &cluster_start,
		    const std::pair<double,double> &cluster_end,
		    const std::vector<std::pair<double,double> > &cluster_polygon);
    
    /// A function to draw all clusters on a dedicated canvas
    void DrawAllClusters();

    /// A function to draw each cluster on a separate canvas than the one with all clusters
    void DrawOneCluster(UChar_t plane, size_t index);
    
    /// A function to draw two clusters on a separat canvas than the one with all clusters.
    void DrawTwoClusters(UChar_t plane, size_t index1, size_t index2);

    /// A function to count # clusters in the given plane
    size_t ClusterCount(UChar_t plane);
    
  protected:

    /// A utility function to create TH2D histogram
    TH2D* GetPlaneViewHisto(const UChar_t plane,
			    const std::string &name, 
			    const std::string &title) const;
    
    /// A utility function to create TGraph
    TGraph* GetPlaneViewGraph(const UChar_t plane,
			      const std::string &name, 
			      const std::string &title) const;

  protected:

    //--- Run control variables ---//
    /// Name ... has to be unique per algo instance
    std::string _name;
    /// Stores # of planes
    UChar_t _nplanes;
    /// Edge of wire vs. time view per plane
    std::vector<std::pair<double,double> > _xrange, _yrange;
    /// Boolean to confirm if range is set by a user
    std::vector<bool> _range_set;

    //---- Canvas ----//
    TCanvas* _cAllCluster;
    TCanvas* _cOneCluster;
    TCanvas* _cTwoClusters;

    //---- Things to be drawn (DATA) ----//

    /// 2D hit map (wire [cm] vs. time [cm]) ... index = plane
    std::vector<TH2D*> _hAllHits;
    /// 2D hit map (wire [cm] vs. time [cm]) ... 1st index = plane, 2nd index = cluster
    std::vector<std::vector<TH2D*> >   _hClusterHits;
    /// Cluster start point (wire [cm], time [cm]) ... 1st index = plane, 2nd index = cluster
    std::vector<std::vector<TGraph*> > _gClusterStart;
    /// Cluster end point (wire [cm], time [cm]) ... 1st index = plane, 2nd index = cluster
    std::vector<std::vector<TGraph*> > _gClusterEnd;
    /// Cluster polygon points in (wire [cm], time [cm]) ... 1st index = plane, 2nd index = cluster
    std::vector<std::vector<TGraph*> > _gClusterPolygon;


    //---- Utility variables for unit conversion ----//
    double _wire2cm, _time2cm;
  };
  
}
#endif

/** @} */ // end of doxygen group 
