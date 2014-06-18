/**
 * \file tmpClusterAna.hh
 *
 * \ingroup KalekoAna
 * 
 * \brief Class def header for a class tmpClusterAna
 *
 * @author davidkaleko
 */

/** \addtogroup KalekoAna

    @{*/

#ifndef TMPCLUSTERANA_HH
#define TMPCLUSTERANA_HH

#include "ana_base.hh"
#include "Geometry.hh"

namespace kaleko {
  /**
     \class tmpClusterAna
     User custom analysis class made by davidkaleko
   */
  class tmpClusterAna : public larlight::ana_base{
  
  public:

    /// Default constructor
    tmpClusterAna(){ _name="tmpClusterAna"; _fout=0;};

    /// Default destructor
    virtual ~tmpClusterAna(){};

    /** IMPLEMENT in tmpClusterAna.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in tmpClusterAna.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(larlight::storage_manager* storage);

    /** IMPLEMENT in tmpClusterAna.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    //getter for the tree holding figures of merit
    TTree* GetTree() const {return _ana_tree;}
    

    protected:


    void PrepareTTree();

    void ClearTTreeVars();

    TTree* _ana_tree;

    void CalculateShit(larlight::storage_manager* storage);

    //variables to go into the ttree
    //filled once per event
    int _n_clusters;
    int _n_hits_tiny_clusters;
    int _n_hits_big_clusters;
    double _qtot_tiny_clusters;
    double _qtot_big_clusters;

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
