/**
 * \file ShowerDistanceStudy.hh
 *
 * \ingroup ShowerStudy
 * 
 * \brief Class def header for a class ShowerDistanceStudy
 *
 * @author arianaHackenburg 
 */

/** \addtogroup ShowerStudy

    @{*/

#ifndef SHOWERDISTANCESTUDY_HH
#define SHOWERDISTANCESTUDY_HH

#include "ana_base.hh"

namespace larlight {
  /**
     \class ShowerDistanceStudy
     User custom analysis class made by arianaHackenburg
   */
  class ShowerDistanceStudy : public ana_base{
  
  public:

    /// Default constructor
    ShowerDistanceStudy(){ _name="ShowerDistanceStudy"; _fout=0;};

    /// Default destructor
    virtual ~ShowerDistanceStudy(){};

	void PrepareTTree() ;

    virtual bool initialize();

    /** IMPLEMENT in ShowerDistanceStudy.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ShowerDistanceStudy.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    protected:

	TTree * _ana_tree ;

	double _motherEnergy ;
	double _daughter1_Energy ;
	double _daughter2_Energy ;

	double _dist1_ToWall ;
	double _dist1_AlongTraj ;
	
	double _dist2_ToWall ;
	double _dist2_AlongTraj ;

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
