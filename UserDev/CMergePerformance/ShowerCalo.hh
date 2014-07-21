/**
 * \file ShowerCalo.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class ShowerCalo
 *
 * @author SHELL_USER_NAME
 */

/** \addtogroup Analysis

    @{*/

#ifndef SHOWERCALO_HH
#define SHOWERCALO_HH

#include "ana_base.hh"
#include "ClusterParamsAlgNew.hh"
#include "McshowerLookback.hh"


namespace larlight {

  /**
     \class ShowerCalo
     User custom analysis class made by SHELL_USER_NAME
   */
  class ShowerCalo : public ana_base {
  
  public:

    /// Default constructor
    ShowerCalo() : ana_base() { 
      _name="ShowerCalo"; 
      _fout=0;
      _debug = true;
      _cluster_type = DATA::Cluster;
      _MCShower_mother_energy_cutoff = 0.02;
    }

    /// Default destructor
    virtual ~ShowerCalo(){};

    /** IMPLEMENT in ShowerCalo.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ShowerCalo.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

    /** Set Cluster Type to be read in: ClusterMerger makes Cluster
	instead of FuzzyCluster
    */
    void SetClusterType(DATA::DATA_TYPE type) {_cluster_type = type;}

    void SetDebug(bool flag) { _debug = flag; }

    protected:

    void GetHits( cluster* clus, event_hit* ev_hits,
		  larlight::DATA::DATA_TYPE hit_type);

    float GetEnergy( std::vector<larutil::PxHit> clusHits );

    void GetParams( std::vector<larutil::PxHit> clusHits );

    float Pi0Mass( float angle1, float Q1, float angle2, float Q2 );

    float dEdxBox( float dQdx );

    float dQdEBirks( float dQdx );

    void FillClusterParamsVector(event_cluster* ev_cluster,
				 event_hit* ev_hits);
    void PrepareHistos();

    void dEdx( std::vector<float> & dEdx_vec,
	       std::vector<float> & dE_vec );

    void dEdx(cluster* clus,
	      event_hit* ev_hits,
	      larlight::DATA::DATA_TYPE hit_type,
	      std::vector<float> & dEdx_vec);

    std::vector< std::vector<int> > FindBestClusters(event_mcshower* ev_mcshower,
						     event_cluster* ev_cluster,
						     event_hit* ev_hits,
						     const std::vector< ::cluster::ClusterParamsAlgNew> &_clusterparams);
    
    
    //Histograms created
    std::vector<TH2D*> hCharge_toQ;
    std::vector<TH1D*> hPurityPerMCShower;
    std::vector<TH1D*> hEffPerMCShower;
    std::vector<TH2D*> hClusQ_vs_ShowerQ;
    std::vector<TH2D*> hClusQ_vs_ShowerMCQ;
    std::vector<TH2D*> hClusE_vs_ShowerMCE;
    std::vector<TH2D*> hClusTOTQ_vs_ShowerQ;
    std::vector<TH2D*> hClusPur_vs_ShowerQ;
    std::vector<TH2D*> hClusEff_vs_ShowerQ;
    std::vector<TH1D*> hdEdx;
    std::vector<TH2D*> hdx;
    std::vector<TH1D*> hPi0Mass;

    //Variables used in module
    bool _debug;
    Double_t w2cm, t2cm;
    ///vector of CPAN objects (before, and overwritten after merging)
    std::vector< ::cluster::ClusterParamsAlgNew> _clusterparams;
    ::cluster::ClusterParamsAlgNew _params;
    std::vector<larutil::PxHit> _clusHits;
    std::vector<std::vector<unsigned short> > clus_idx_vec;
    //neural net path needed
    std::string myNeuralNetPath;
    DATA::DATA_TYPE _cluster_type;
    larlight::DATA::DATA_TYPE hit_type;
    //declare mcshowerlookback object only once, not once per event
    larlight::McshowerLookback _mcslb;
    //some maps that mcshowerlookback fills once per event
    std::map<UInt_t,UInt_t> _shower_idmap;
    std::map<UShort_t,larlight::simch> _simch_map;
    double _MCShower_mother_energy_cutoff;
    double _clusQfrac_over_totclusQ;
    double _clusQ_over_MCQ;
    double _tot_clus_charge;
    double _tot_clus_charge_fromKnownMCS;
    double _frac_clusQ;
    double _dom_MCS_Q;
    double _mother_energy;
    int _nhits;
    double _opening_angle;
    double _clusQfrac_from_unknown;
    GEO::View_t _plane;
    bool _after_merging;
    std::vector< std::vector< float > > _MCShower_best_clus; 
    std::vector< std::vector< float > > _MCShower_Qs; 
    std::vector< std::vector< float > > _BestClus_Qs; 
    std::vector< std::map<int, int> > _Shower_Clus_Map;
    std::vector<UInt_t> _MCShower_indices;
    std::vector<float> _dEdx_vec;   //hold dEdx deposited along each dx
    std::vector<float> _dE_vec;     //hold energy deposited along each dx
    std::vector<float> _dxTot_vec;
    int _evt_num;
    int L_max;

    //Birk's Law values
    double Ab, Wion, kB, E;
    //Box Model parameters:
    double _beta;
    double _alpha;
    //LAr properties
    double _Wion;
    double _rho;
    double _Efield;
    //Convert from Hit.Charge() to N_electrons
    double _Qtoe_Induction;
    double _Qtoe_Collection;
  };
}
#endif

/** @} */ // end of doxygen group 
