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

#define PI 3.14159265

#include "ana_base.hh"
#include "DivReg.hh"
#include "ForceRegions.hh"
#include "Quality.hh"
#include "CRUHelper.hh"
#include "PlaneCheckMatch.hh"
#include "LookBack.hh"
#include "LArUtilBase.hh"
#include<algorithm> // For sort()




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

	::cluster::DivReg fDivReg;
        ::cluster::CRUHelper fCRUHelper;
        ::cluster::ForceRegions fForceRegions;
        ::cluster::Quality fQuality;
        ::cluster::PlaneCheckMatch fPlaneCheckMatch;
        ::cluster::LookBack fLookBack;

	double radius_of_interest = 2.0;


    private: 
	std::pair<double,double> HighQSlope(std::vector<unsigned int> hitindex ,larlight::event_hit const* hits);
	std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>> CheckQuality(larlight::event_hit *const hits,std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>> protocluster, std::vector<bool> Flags);
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
