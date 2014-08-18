/**
 * \file MCShowerClusterer.hh
 *
 * \ingroup CMTool
 * 
 * \brief This class takes in MCShower files, constructs fake hits (with 
 *  fixed width), clusters the hits together such that each MCShower has
 *  one cluster (in each view). EX: a single electron shower event will 
 *  have three clusters in its output, as there are 3 planes.
 *
 * @author davidkaleko
 */

/** \addtogroup ClusterRecoUtil

    @{*/

#ifndef MCSHOWERCLUSTERER_HH
#define MCSHOWERCLUSTERER_HH

#include "ana_base.hh"
#include "GeometryUtilities.hh"

namespace larlight {
  /**
     \class MCShowerMerger
     User custom analysis class made by davidkaleko
   */
  class MCShowerClusterer : public ana_base{
  
  public:

    /// Default constructor
    MCShowerClusterer(){ _name="MCShowerClusterer"; _fout=0; }

    /// Default destructor
    virtual ~MCShowerClusterer(){}

    /** IMPLEMENT in MCShowerClusterer.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in MCShowerClusterer.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in MCShowerClusterer.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();
    
  protected:

    size_t _nplanes;

    Double_t _hitwidth;

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
