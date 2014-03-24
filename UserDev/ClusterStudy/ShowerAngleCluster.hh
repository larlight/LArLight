/**
 * \file ShowerAngleCluster.hh
 *
 * \ingroup ShowerAngleCluster
 * 
 * \brief Class def header for a class ShowerAngleCluster
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerAngleCluster

    @{*/

#ifndef SHOWERANGLECLUSTER_HH
#define SHOWERANGLECLUSTER_HH

#include "ana_base.hh"

#include "DataFormat-TypeDef.hh"
#include "ClusterRecoAlg-TypeDef.hh"

#include <TH1F.h>
#include <TTree.h>

#include "ClusterMergeAlg.hh"

namespace larlight {
  /**
     \class ShowerAngleCluster
     User custom analysis class made by kazuhiro
   */
  class ShowerAngleCluster : public larlight::ana_base{
  
  public:

    /// Default constructor
    ShowerAngleCluster();

    /// Default destructor
    virtual ~ShowerAngleCluster(){};

    /// Configure method
    void reconfigure();

    /// Initialize
    virtual bool initialize();

    /// Analyze
    virtual bool analyze(larlight::storage_manager* storage);

    /// Finalize
    virtual bool finalize();

    /// Set what kind of cluster to read-in
    void SetInputClusterType(DATA::DATA_TYPE type);

  private:

    double Get2DAngleForHit( const larlight::hit *starthit,
			     const std::vector <larlight::hit*> &hitlist);

    double Get2DAngleForHit( unsigned int wire, double time,
			     const std::vector <larlight::hit*> &hitlist);

    void ClearandResizeVectors(unsigned int nClusters);

 
    //HoughBaseAlg fHBAlg; 
    larreco::ClusterParamsAlg fCParAlg;

    larreco::ClusterMergeAlg *fCMergeAlg;


    //ClusterMergeAlg fCMergeAlg;
    //ClusterMatchAlg fCMatchAlg;
    bool fExternalStartPoints;
    //   double fWiretoCm,fTimetoCm,fWireTimetoCmCm;
    
    double fNTimes;
    
    larlight::cluster MainClusterLoop(const larlight::cluster &inCluster,
				      const std::vector<larlight::hit*> &hitlist, 
				      unsigned int iClustInput, 
				      unsigned int iClustOutput); 

    larlight::cluster MergeClusterLoop(const std::vector<larlight::hit*> &hitlist,
				       unsigned int iClustOutput);
    
    larutil::DetectorProperties *detp;
    larutil::LArProperties *larp;
    larutil::Geometry *geo;
    larutil::GeometryUtilities *gser;
   
    float fTimeTick; // time sample in us
    float fPresamplings;
    float fDriftVelocity;
     
    bool fForceRightGoing;

    //input parameter labels:
 
    larlight::DATA::DATA_TYPE fInputClusterType;
  
    std::vector <double> xangle;       // in cm, cm
   
    std::vector <double> lineslopetest;   // in wire, time
    std::vector <double>  lineinterctest;   
    std::vector<double>  fWireVertex,fTimeVertex;
    std::vector<double>  fWireEnd,fTimeEnd;
    std::vector<double> fVerticalness;
    std::vector< double > fErrors;
    unsigned int fNPlanes; // number of planes  

    TH1F *  fh_omega_single;
    TTree* ftree_cluster;
    bool matchflag;
    unsigned int  fMinHitListSize;
    
    DATA::DATA_TYPE _in_cluster_type;


  };
}
#endif

/** @} */ // end of doxygen group 
