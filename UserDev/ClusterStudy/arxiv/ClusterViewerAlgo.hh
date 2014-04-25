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

#include "ana_base.hh"
#include <TH2D.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TPad.h>

namespace larlight{
  /**
     \class ClusterViewerAlgo
     User custom analysis class made by kazuhiro/kaleko
  */
  class ClusterViewerAlgo : public ana_base{
    
  public:
    
    /// Default constructor
    ClusterViewerAlgo();
    
    /// Default destructor
    virtual ~ClusterViewerAlgo(){};

    /** IMPLEMENT in ClusterViewerAlgo.cc! 
	Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);
    
    /// A utility function to (re)create Th3D histogram of a specified boundary & name
    TH2D* Prepare2DHisto(std::string name, 
			 double wiremin, double wiremax,
			 double timemin, double timemax);
    
    TGraph* PrepareGraph();
    
    /// Getter for Cluster histogram vector
    /// length of this vector should be the number of clusters in that view
    const std::vector<TH2D*>  GetClusterHits(int plane) const {
      if(plane>_hClusterGraph_start.size())
	
	throw cluster::RecoUtilException(Form("Plane %d does not exist (max=%zu)",
					      plane,
					      _hClusterGraph_start.size())
					 );

      return _hClusterHits.at(plane);

    };
    
    /// Getter for hit TH2D histo, weighted by charge
    const TH2D*  GetHisto_Hits (int plane) const {

      if(plane>_hClusterGraph_start.size())

	throw cluster::RecoUtilException(Form("Plane %d does not exist (max=%zu)",
					      plane,
					      _hClusterGraph_start.size())
					 );
      return _hAllHits.at(plane);
    };
    
    const std::vector<TGraph*>  GetClusterGraph_Reco (int plane, bool start) const {

      if(plane>_hClusterGraph_start.size())

	throw cluster::RecoUtilException(Form("Plane %d does not exist (max=%zu)",
					      plane,
					      _hClusterGraph_start.size())
					 );
	
      if(start)
	return _hClusterGraph_start.at(plane);
      else
	return _hClusterGraph_end.at(plane);
      
    };
    
  protected:
    /// Type of cluster to view
    DATA::DATA_TYPE cluster_type;

    /// Main canvas
    TCanvas* _c1;
    /// Main pad
    TPad*    _p1;
    /// MC step histogram
    //  TH3D*    _hMCStep;
    /// Histogram array for Reconstructed clusters' hits (time v. wire) for planes
    /// length of this vector is the # of planes, and the inner vector's length is # of clusters in that plane
    std::vector<std::vector<TH2D*> > _hClusterHits;

    /// Hit histograms to sit next to cluster ones,
    std::vector<TH2D*> _hAllHits;

    std::vector<std::vector<TGraph*> > _hClusterGraph_start;
    std::vector<std::vector<TGraph*> > _hClusterGraph_end;


    std::vector<std::pair<double,double> > _xrange, _yrange;
  };
  
}
#endif

/** @} */ // end of doxygen group 
