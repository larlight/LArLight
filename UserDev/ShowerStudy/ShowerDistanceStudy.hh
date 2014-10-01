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

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

    protected:

	TTree * _ana_tree ;

	double _mother1Energy ;
	double _mother2Energy ;
	double _daughter1_Energy ;
	double _daughter2_Energy ;

	double _motherMomentumX ;	
	double _motherMomentumY ;	
	double _motherMomentumZ ;	

	double _dist1_ToWall ;
	double _dist1_AlongTraj ;
	
	double _dist2_ToWall ;
	double _dist2_AlongTraj ;

	double _reco_daughter1_Energy ;

	std::vector<double> _reco_start1 ;
	std::vector<double> _direction1 ;

	std::vector<double> _reco_start2 ;
	std::vector<double> _direction2 ;

	double _reco_dist1_ToWall ;
	double _reco_dist2_ToWall ;
	double _reco_dist1_AlongTraj ;
	double _reco_dist2_AlongTraj ;


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
