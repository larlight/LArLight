/**
 * \file RecoShowerStudy.hh
 *
 * \ingroup ShowerStudy
 * 
 * \brief Class def header for a class RecoShowerStudy
 *
 * @author arianaHackenburg
 */

/** \addtogroup ShowerStudy

    @{*/

#ifndef RECOSHOWERSTUDY_HH
#define RECOSHOWERSTUDY_HH

#include "ana_base.hh"
#include "DistToBoxWall.hh"
#include <vector>

namespace larlight {
  /**
     \class RecoShowerStudy
     User custom analysis class made by ah673
   */
  class RecoShowerStudy : public ana_base{
  
  public:

    RecoShowerStudy(){ _name="RecoShowerStudy"; _fout=0;};

    virtual ~RecoShowerStudy(){};

    virtual bool initialize();

	void PrepareTTree() ;

	void Clear() ;

	void Reset() ;

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

    protected:
	
	TTree* _reco_tree ;

	std::vector<double> _reco_energy ; 
	std::vector<double> _total_energy ; 

	std::vector<double> _shower_start ;
	std::vector<double> _direction ;

	int _count0  ;
	int _count1  ;
	
	double _dist_to_wall ;
	double _dist_along_traj ;

	double _dist_backwards_to_wall ; 
	
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
