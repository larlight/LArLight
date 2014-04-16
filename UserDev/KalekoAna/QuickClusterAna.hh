/**
 * \file QuickClusterAna.hh
 *
 * \ingroup KalekoAna
 * 
 * \brief Class def header for a class QuickClusterAna
 *
 * @author davidkaleko
 */

/** \addtogroup KalekoAna

    @{*/

#ifndef QUICKCLUSTERANA_HH
#define QUICKCLUSTERANA_HH

#include "ana_base.hh"
#include "ClusterParamsExecutor.hh"
namespace kaleko {
  /**
     \class QuickClusterAna
     User custom analysis class made by davidkaleko
   */
  class QuickClusterAna : public larlight::ana_base{
  
  public:

    /// Default constructor
    QuickClusterAna(){ _name="QuickClusterAna"; _fout=0;};

    /// Default destructor
    virtual ~QuickClusterAna(){};

    /** IMPLEMENT in QuickClusterAna.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in QuickClusterAna.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(larlight::storage_manager* storage);

    /** IMPLEMENT in QuickClusterAna.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    //getter for the tree holding figures of merit
    TTree* GetMCShowerTree() const {return _ana_tree;}
    

    protected:


    void PrepareTTree();

    void ClearTTreeVars();

    TTree* _ana_tree;

    void FillClusterParamsVector(larlight::storage_manager* storage);

    void CalculateTTreeVars(larlight::event_cluster* ev_cluster, 
			    larlight::event_mcshower* ev_mcshower, 
			    larlight::event_hit* ev_hits, 
			    const std::vector<cluster::ClusterParamsAlgNew> &_clusterparams);

    bool DKShittyClusterTest(const larlight::mcshower &main_shower,
			     const larlight::cluster &i_cluster);

    std::vector<cluster::ClusterParamsAlgNew> _clusterparams;
    

    //neural net path needed
    std::string myNeuralNetPath;

    //variables to go into the ttree
    //    std::vector<int> _nhits;
    //    std::vector<double> _cluscharge;
    //    std::vector<double> _mc_mother_energy;
    int _nhits;
    double _cluscharge;
    double _mc_mother_energy;
    double _mc_angle_2d;
    int _view;
    bool _dk_good_clus;


    double _sum_charge;                 ///< Sum charge of hits in ADC
    double _mean_charge;                ///< Mean (average) charge of hits in ADC
    double _mean_x;                     ///< Mean of hits along x, peaks only
    double _mean_y;                     ///< Mean of hits along y, peaks only
    double _rms_x;                      ///< rms of hits along x (wires)
    double _rms_y;                      ///< rms of hits along y, (time)     
    double _charge_wgt_x;               ///< Mean of hits along x, charge weighted
    double _charge_wgt_y;               ///< Mean of hits along y, charge weighted  
    double _cluster_angle_2d;           ///< Linear best fit to high-charge hits in the cluster
    double _angle_2d;                   ///< Angle of axis in wire/hit view      
    double _opening_angle;              ///< Width of angular distubtion wrt vertx 
    double _opening_angle_charge_wgt;   ///< Same for charge_wgt      
    double _closing_angle;              ///< Width of angular distubtion wrt endpoint   
    double _closing_angle_charge_wgt;   ///< Same for charge_wgt          
    double _eigenvalue_principal;       ///< the principal eigenvalue from PCA   
    double _eigenvalue_secondary;       ///< the secondary eigenvalue from PCA   
    double _verticalness;               ///< ???
    double _length;			
    double _width;			
    double _hit_density_1D;		
    double _hit_density_2D;		
    double _multi_hit_wires;		
    double _N_Wires;			
    double _modified_hit_density;	
    double _N_Hits;			
    double _N_Hits_HC;			
    int    _direction;			
    double _showerness;                 ///< got heart
    double _trackness;                  ///< got soul
    double _offaxis_hits;               ///< got brain     

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
