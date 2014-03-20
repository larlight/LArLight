/**
 * \file MCShowerStudy.hh
 *
 * \ingroup MCShowerStudy
 * 
 * \brief Class def header for a class MCShowerStudy
 *
 * @author davidkaleko
 */

/** \addtogroup MCShowerStudy

    @{*/

#ifndef MCSHOWERSTUDY_HH
#define MCSHOWERSTUDY_HH

#include "ana_base.hh"

//this include works because I modified the GNUMakeFile to add LArUtil
// to INCFLAGS, so compiler can find this hh file
#include "GeometryUtilities.hh"

namespace larlight {
  /**
     \class MCShowerStudy
     User custom analysis class made by davidkaleko
   */
  class MCShowerStudy : public ana_base{
  
  public:

    /// Default constructor
    MCShowerStudy() : ana_base() { _name="MCShowerStudy"; }

    /// Default destructor
    virtual ~MCShowerStudy(){};

    /** IMPLEMENT in MCShowerStudy.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in MCShowerStudy.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in MCShowerStudy.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    TTree* GetMCClusTree() const {return _mcclus_tree;}

    protected:

    void PrepareTTree();

    TTree* _mcclus_tree;


    //this i should get from geometry service but didn't find immediately
    double _wire_2_cm;
    double _time_2_cm;

    // variables that go into the analysis ttree
    double _refinestartend_FOM;

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
