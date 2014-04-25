/**
 * \file CutEff.hh
 *
 * \ingroup CutEff
 * 
 * \brief Class def header for a class CutEff
 *
 * @author andrzej
 */

/** \addtogroup CutEff

    @{*/

#ifndef CUTEFF_HH
#define CUTEFF_HH

#include <TH1D.h>

#include "ana_base.hh"
#include "ClusterParamsAlgNew.hh"


namespace larlight {
  /**
     \class CutEff
     User custom analysis class made by andrzej
   */
  class CutEff : public ana_base{
  
  public:

    /// Default constructor
    CutEff() : ana_base() { _name="CutEff"; }

    /// Default destructor
    virtual ~CutEff(){ //h1=0; 
          };

    /** IMPLEMENT in CutEff.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in CutEff.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in CutEff.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    protected:

    ::cluster::ClusterParamsAlgNew  fCPAlg; 

    larutil::GeometryUtilities  *fGSer;
    /// Example histogram to be saved in analysis root file (separate from data product)
    
    TTree * fMainTree;
    ///////////////////TTree variables
    
    void init_tree_vectors();
    
    int fRun;
    int fSubRun;
    int fEvent;
    int fNPlanes;
    
    
    int fNClusters;
    int fNParticles;
    
    
    //These are defined per MCparticle  
    std::vector< int>   fMCPDGstart   ;  
    std::vector< int>   fMCPDGisPrimary   ;
    std::vector<double>   fMCenergystart   ;
    std::vector< double >   fMCPhistart   ;
    std::vector<double >   fMCThetastart   ;
    std::vector<double >   fMCXOrig   ;
    std::vector<double >   fMCYOrig   ;
    std::vector<double >   fMCZOrig   ;
    
    
    //These are defined per cluster
    std::vector< int>   mcpdg   ;
    std::vector<double>   mcenergy   ;
    std::vector< double >   mcphi   ;
    std::vector<double >   mctheta   ;
    std::vector<double >   mcomega   ;
    std::vector< double >   mcx   ;
    std::vector<double >   mcy   ;
    std::vector<double >   mcz   ;
    
    std::vector< int>   mcdirection   ;
    // std::vector< double>   mcenergyfraction   ;
    std::vector<unsigned int>   mcwirevertex   ;
    std::vector<double>   mctimevertex   ;
    // std::vector< double>   mcdistfromorigin   ;
    
    
    ///////////// reconstructed quantities
    std::vector< int>   plane   ;
    std::vector<unsigned int>   fWireVertex   ;
    std::vector<double>   fTimeVertex   ;
    
    std::vector<unsigned int>   fWireLast   ;
    std::vector<double>   fTimeLast   ;
    
    std::vector<int>   fDirection   ;
    
    std::vector<double>   fOffAxis   ;
    std::vector<double>   fOffAxisNorm   ;
    std::vector<int>   fNhits   ;
    
    std::vector<double>   fHitDensity   ;
    std::vector<double>   fLength   ;
    std::vector<double>   fWidth   ;  
    
    //  fOffAxisNormHD   std::vector<double>   fOffAxisNormHD   ;
    //  fOnAxisNormHD   std::vector<double>   fOnAxisNormHD   ;
    
    
    std::vector<double>   fPrincipal   ;
    std::vector<double>   slope2D   ;
    std::vector<int>   fMultiHitWires   ;
    
    
    
    std::vector<double>   fsumCharge   ;
    std::vector<double>   fmeanCharge    ;
    std::vector<double>   frmsX    ;
    std::vector<double>   frmsY    ;
    std::vector<double>   fmeanX    ;
    std::vector<double>   fmeanY    ;
    std::vector<double>   fchargeWgtX    ;
    std::vector<double>   fchargeWgtY    ;
    std::vector<double>   fclusterAngle2d    ;
    std::vector<double>   fangle2d    ;
    std::vector<double>   fopenAngle    ;
    std::vector<double>   fopenAngleChargeWgt    ;
    std::vector<double>   fcloseAngle    ;
    std::vector<double>   fcloseAngleChargeWgt    ;
    std::vector<double>   fhitDensity1d    ;
    std::vector<double>   fhitDensity2d    ;
    std::vector<double>   fmodifiedHitDens    ;
    std::vector<double>   fShowerness;
    std::vector<double>   fTrackness;
    std::vector<double>   fModifiedMeanCharge;
    std::vector<double>   fChargeRMS;
    
    std::vector<bool>     fPassCut; // per cluster
    std::vector<bool>     fPassFANNCut; //per cluster
    std::vector<int>      fNshowersPerPlane;  // per plane
    std::vector<int>      fNFANNshowersPerPlane; //per plane
    
    double   fSmallestDist;  
    double   fSmallestReasonDist;  
    std::vector<double>   fClusterDist;  //per combination of clusters 
    
    int    fEventPassCut;
    int    fEventPassFANNCut;
    
    int    fEventPassCut2Planes;
    int    fEventPassFANNCut2Planes;
    
    int    fEventPassCut2Planes1Only;
    int    fEventPassFANNCut2Planes1Only;
    
    int    fEventPassCut2PlanesTimeMatched;
    int    fEventPassFANNCut2PlanesTimeMatched;
  
    int    fEventPassCut2PlanesMatched;
    int    fEventPassFANNCut2PlanesMatched;
    
    ////////////////////////////////// allhits loop.
    int    fEventPassAllHitsShowerness;
    int    fEventPassAllHitsChargerms;
    int    fEventPassAllHitsTPrincipal;
    std::vector<double>   fChargeRMSAll;
    std::vector<double>   fShowernessAll;
    std::vector<double>   fPrincipalAll;
    std::vector<int>   allplane;
       //any event that has two shower clusters one in eache plane that match in time.  
    TH1D* bothplaneShowerEventsandAllHitsPassed;
    TH1D* bothplaneShowerFANNEventsandAllHitsPassed;
    
    TH1D* notbothplaneShowerEventsandAllHitsPassed;
    TH1D* notbothplaneShowerFANNEventsandAllHitsPassed;
    
    TH1D* bothplaneShowerEventsandAllHitsPassedAndMinosMatched;
    TH1D* bothplaneShowerFANNEventsandAllHitsPassedAndMinosMatched;
    
    //TH1D* minos_matched_mcpdg; TH1D* minos_matched_recopdg; 
    int minos_matched_mcpdg;
    int minos_matched_recopdg;
    int    fEventMinosMatched;
    
    /////////////////////////////////////
    
    // one entry per event
    TH1D* allEvts;
    //one entry per cluster
    TH1D* allClusters;
    //any cluster that passes cuts
    TH1D* anyShowerClusters;
    //any event that has two shower clusters (no 2 plane req) 
    TH1D* anyShowerEvents;
    //any event that has at least two shower clusters one in each plane  
    TH1D* bothplaneShowerEvents;
         //any event that has two shower clusters one in eache plane that match in time.  
    TH1D* bothplaneShowerEventsTimematched;
    //any event that has at least two shower clusters one in each plane that match/reconstruct inside fid vol.  
    TH1D* bothplaneShowerEventsmatched;
    //any event that has only shower clusters one in eache plane   
    TH1D* bothplaneShowerEventOnlyOne;
    
    
    TH1D* anyShowerClusters_FANN;
    //any event that has two shower clusters (no 2 plane req) 
    TH1D* anyShowerEvents_FANN;
    //any event that has two shower clusters one in eache plane  
    TH1D* bothplaneShowerEvents_FANN;
     //any event that has two shower clusters one in eache plane that match in time.  
    TH1D* bothplaneShowerEventsTimematched_FANN;
    //any event that has two shower clusters one in eache plane that match/reconstruct inside fid vol.  
    TH1D* bothplaneShowerEventsmatched_FANN;
    //any event that has only shower clusters one in eache plane   
    TH1D* bothplaneShowerEventOnlyOne_FANN;
    
    
    //smallest distance between MeanY
    TH1D* smallest_dist; 
    TH1D* smallest_reason_dist; 
    //all distances between MeanY
    TH1D* cluster_dist_diff; 
    
    
//     TH1D* rmsX;
//     TH1D* rmsY;
//     TH1D* chargeWgtX; 
//     TH1D* chargeWgtY;
//     
    
   
//     TH1D* sumCharge;
//     TH1D* meanCharge;
//     TH1D* meanX;
//     TH1D* meanY; 
//     TH1D* rmsX;
//     TH1D* rmsY;
//     TH1D* chargeWgtX; 
//     TH1D* chargeWgtY;
//     TH1D* clusterAngle2d; 
//     TH1D* angle2d;
//     TH1D* openAngle, *openAngleChargeWgt;
//     TH1D* closeAngle, *closeAngleChargeWgt; 
//    
//     TH1D* hitDensity1d ;
//     TH1D* hitDensity2d ;
//     TH1D* modifiedHitDens ;
//     
//     TH1D* openAngle_sh, *openAngleChargeWgt_sh;
//     TH1D* closeAngle_sh, *closeAngleChargeWgt_sh; 
//    
//     TH1D* hitDensity1d_sh ;
//     TH1D* hitDensity2d_sh ;
//     TH1D* modifiedHitDens_sh ;
//     TH1D *multihitw_sh;
//     TH1D *eigenvalue_principal_sh;
//     TH1D *length_sh;
//     TH1D *width_sh;
//     
//     TH1D* openAngle_tr, *openAngleChargeWgt_tr;
//     TH1D* closeAngle_tr, *closeAngleChargeWgt_tr; 
//    
//     TH1D* hitDensity1d_tr ;
//     TH1D* hitDensity2d_tr ;
//     TH1D* modifiedHitDens_tr ;
//     TH1D *multihitw_tr;
//     TH1D *eigenvalue_principal_tr;
//     TH1D *length_tr;
//     TH1D *width_tr;
//     
//     
//     TH1D *showerness, *trackness ;
// 
//     /// mc reco quality plots
     TH1D *recostartwire, *recostarttime;
     TH1D *recostartwirenodir, *recostarttimenodir;
//     
    TH1D *reco2Dangle, *reco2DangleGood;
    TH1D *recoDir;
//    
//    /////testing histograms
//    TH1D *  Charge_e[2];
//     
//    TH1D *  Charge_p[2];
//     
//    TH1D *  Charge_mu[2];
//     
//    TH1D *  Charge_e_c;
//    TH1D *  Charge_p_c;
//    TH1D *  Charge_mu_c;
   std::vector< double > max_charge,max_charge_3;
    
   
   
  };
}
#endif

//**************************************************************************
// 
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group 
