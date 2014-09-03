/**
 * \file QuickMCShowerAna.hh
 *
 * \ingroup KalekoAna
 * 
 * \brief Class def header for a class QuickMCShowerAna
 *
 * @author davidkaleko
 */

/** \addtogroup KalekoAna

    @{*/

#ifndef QUICMCSHOWERANA_HH
#define QUICMCSHOWERANA_HH

#include "ana_base.hh"

namespace kaleko {
  /**
     \class QuickMCShowerAna
     User custom analysis class made by davidkaleko
   */
  class QuickMCShowerAna : public larlight::ana_base{
  
  public:

    /// Default constructor
    QuickMCShowerAna(){ _name="QuickMCShowerAna"; _fout=0;};

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
    

    protected:

    void PrepareTTree();

    void ClearTTreeVars();

    TTree* _ana_tree;

    double _mom_energy;
    double _reco_energy;

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
