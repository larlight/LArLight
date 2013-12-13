/**
 * \file ClusterViewer.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class ClusterViewer
 *
 * @author kazuhiro/kaleko
 */

/** \addtogroup Analysis

    @{*/

#ifndef CLUSTERVIEWER_HH
#define CLUSTERVIEWER_HH

#include "ana_base.hh"
#include <TH2D.h>
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
    
    /// A utility function to (re)create Th3D histogram of a specified boundary & name
    TH2D* Prepare2DHisto(std::string name, 
			 double wiremin, double wiremax,
			 double timemin, double timemax);
    
    /// Getter for MC points
    //  const TH2D* GetHisto_MC   () const {return _hMCStep;};
    
    /// Getter for Cluster histogram vector
    /// length of this vector should be the number of clusters in that view
    //need to turn these 3 functions into 1... scared of array of vector
    const std::vector<TH2D*>  GetHisto_Reco (int view) const {
      if(view==0)
	return _hRecoCluster_v_0;
      else if(view==1)
	return _hRecoCluster_v_1;
      else if(view==2)
	return _hRecoCluster_v_2;
      else {
	std::cout<<"*******************you screwed something up. view should be 0 1 or 2!"<<std::endl;
	std::cout<<"returning _hRecoCluster_v_0 because i don't know what else to return"<<std::endl;
	return _hRecoCluster_v_0;
      }
    };

    /// Getter for cluster data object vector
    /// length of this vector is the total # of clusters in the event
    const std::vector<cluster>  GetData_Reco  () const {return _cluster_v;};
    
  protected:
    /// Main canvas
    TCanvas* _c1;
    /// Main pad
    TPad*    _p1;
    /// MC step histogram
    //  TH3D*    _hMCStep;
    /// Histogram array for Reconstructed clusters' hits (time v. wire) for 0th view
    /// length of this vector is the # of clusters in that particular view
    std::vector<TH2D*> _hRecoCluster_v_0;
    /// Histogram array for Reconstructed clusters' hits (time v. wire) for 1st view
    std::vector<TH2D*> _hRecoCluster_v_1;
    /// Histogram array for Reconstructed clusters' hits (time v. wire) for 2nd view
    std::vector<TH2D*> _hRecoCluster_v_2;
    /// cluster data product vector
    std::vector<cluster> _cluster_v;
    
    GEO::View_t iview;
    std::vector<hit> ihit_v;
  };
}
#endif

/** @} */ // end of doxygen group 
