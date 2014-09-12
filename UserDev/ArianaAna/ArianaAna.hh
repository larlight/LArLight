/**
 * \file ArianaAna.hh
 *
 * \ingroup ArianaAna
 * 
 * \brief Class def header for a class ArianaAna
 *
 * @author ah673
 */

/** \addtogroup ArianaAna

    @{*/

#ifndef ARIANAANA_HH
#define ARIANAANA_HH

#include "ana_base.hh"

namespace larlight {
  /**
     \class ArianaAna
     User custom analysis class made by ah673
   */
  class ArianaAna : public ana_base{
  
  public:

    /// Default constructor
    ArianaAna() : ana_base() { _name="ArianaAna"; 
	//set h1 to nullptr to avoid problems with declaration of hist at the end.
		h1= nullptr ; }

    /// Default destructor
    virtual ~ArianaAna(){};

    /** IMPLEMENT in ArianaAna.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ArianaAna.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ArianaAna.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    protected:
		TH1D * h1 ;
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
