/**
 * \file NCfilter.hh
 *
 * \ingroup NCfilter
 * 
 * \brief Class def header for a class NCfilter
 *
 * @author ryan
 */

/** \addtogroup NCfilter

    @{*/

#ifndef NCFILTER_HH
#define NCFILTER_HH

#include "ana_base.hh"
#include "LArUtilBase.hh"



namespace larlight {
  /**
     \class NCfilter
     User custom analysis class made by ryan
   */
  class NCfilter : public ana_base{
  
  public:

    /// Default constructor
    NCfilter() : ana_base() { _name="NCfilter"; }

    /// Default destructor
    virtual ~NCfilter(){};

    /** IMPLEMENT in NCfilter.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in NCfilter.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in NCfilter.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    protected:

    private: 
	std::pair<double,double> HighQSlope(std::vector<unsigned int> hitindex , larlight::event_hit *const hits);
	std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>> ForceRegions(larlight::event_hit *const hits, std::vector<std::pair<double,double>> APP);

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
