/**
 * \file ShowerDistanceStudy.hh
 *
 * \ingroup ShowerStudy
 * 
 * \Efficiency and energy containment study for mcshower 
 *
 * @author arianaHackenburg 
 */

/** \addtogroup ShowerStudy

    @{*/

#ifndef SHOWERDISTANCESTUDY_HH
#define SHOWERDISTANCESTUDY_HH

#include "ana_base.hh"
#include <vector>

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

	void Clear() ;

	void Reset() ;	

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

    protected:

	TTree * _ana_tree ;

	double _motherEnergy ;
	double _daughterEnergy ;

	double _dist_ToWall ;
	double _dist_AlongTraj ;
	
	double _pi0_mass ;


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
