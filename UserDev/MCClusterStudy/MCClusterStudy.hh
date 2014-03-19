/**
 * \file MCClusterStudy.hh
 *
 * \ingroup MCClusterStudy
 * 
 * \brief Class def header for a class MCClusterStudy
 *
 * @author davidkaleko
 */

/** \addtogroup MCClusterStudy

    @{*/

#ifndef MCCLUSTERSTUDY_HH
#define MCCLUSTERSTUDY_HH

#include "ana_base.hh"

//this include works because I modified the GNUMakeFile to add LArUtil
// to INCFLAGS, so compiler can find this hh file
#include "GeometryUtilities.hh"

namespace larlight {
  /**
     \class MCClusterStudy
     User custom analysis class made by davidkaleko
   */
  class MCClusterStudy : public ana_base{
  
  public:

    /// Default constructor
    MCClusterStudy() : ana_base() { _name="MCClusterStudy"; }

    /// Default destructor
    virtual ~MCClusterStudy(){};

    /** IMPLEMENT in MCClusterStudy.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in MCClusterStudy.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in MCClusterStudy.cc! 
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
