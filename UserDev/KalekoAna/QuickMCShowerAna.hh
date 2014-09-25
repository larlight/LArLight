/**
 * \file QuickMCShowerAna.hh
 *
 * \ingroup KalekoAna
 * 
 * \brief This class will take in mcshowers to get the true 3D angle of a shower, then take in reco showers to get the reco 3D angle of a shower, then compare the two.
 * It uses MCShowerMatchAlg for events with >1 mcshowers, to match reco shower to mcshower.
 *
 * @author davidkaleko
 */

/** \addtogroup KalekoAna
    
    @{*/

#ifndef QUICMCSHOWERANA_HH
#define QUICMCSHOWERANA_HH

#include "ana_base.hh"
#include "MCShowerMatchAlg.hh"
#include "CRUHelper.hh"

namespace kaleko {
  /**
     \class QuickMCShowerAna
     User custom analysis class made by davidkaleko
  */
  class QuickMCShowerAna : public larlight::ana_base{
    
  public:
    
    /// Default constructor
    QuickMCShowerAna(){ _name="QuickMCShowerAna"; _fout=0; _true_electrons_false_pi0=true;};
    
    /// Default destructor
    virtual ~QuickMCShowerAna(){};
    
    /** IMPLEMENT in QuickMCShowerAna.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();
    
    /** IMPLEMENT in QuickMCShowerAna.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(larlight::storage_manager* storage);
    
    /** IMPLEMENT in QuickMCShowerAna.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();
    
    //getter for the tree holding figures of merit
    TTree* GetTree() const {return _ana_tree;}
    
    void SetElectrons(bool flag) { _true_electrons_false_pi0 = flag; }
    
    void SetClusterType(::larlight::DATA::DATA_TYPE type) { _cluster_type = type; }
    
  protected:
    
    void PrepareTTree();
    
    void ClearTTreeVars();
    
    bool _true_electrons_false_pi0;
    
    ::larlight::DATA::DATA_TYPE _cluster_type;
    
    ::larlight::MCShowerMatchAlg _mcshower_match_alg;
    std::vector<unsigned int> cluster_indices;
    size_t mcshower_index;
    double _correctness;
    

    TTree* _ana_tree;
    
    int _event_id;
    double _mom_energy;
    double _daught_energy;
    double _reco_energy;
    std::vector<double> _mc_shower_direction;
    std::vector<double> _reco_shower_direction;
    double _dot_prod;
    double _acos_dot_prod;
    std::vector<double> _mc_shower_start_vtx;
    std::vector<double> _reco_shower_start_vtx;
    double _dist;
    
    std::vector<double> _2Dclusterangle_perplane;
    std::vector<double> _2Dclusterangle_FROM3DMC_perplane;
    int _best_plane;
    
    
    ::cluster::CRUHelper _cru_helper;
    
    larutil::GeometryUtilities *fGSer;
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
