/**
 * \file QuicMCShowerAna.hh
 *
 * \ingroup KalekoAna
 * 
 * \brief Class def header for a class QuicMCShowerAna
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
     \class QuicMCShowerAna
     User custom analysis class made by davidkaleko
   */
  class QuicMCShowerAna : public larlight::ana_base{
  
  public:

    /// Default constructor
    QuicMCShowerAna(){ _name="QuicMCShowerAna"; _fout=0;};

    /// Default destructor
    virtual ~QuicMCShowerAna(){};

    /** IMPLEMENT in QuicMCShowerAna.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in QuicMCShowerAna.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(larlight::storage_manager* storage);

    /** IMPLEMENT in QuicMCShowerAna.cc! 
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
