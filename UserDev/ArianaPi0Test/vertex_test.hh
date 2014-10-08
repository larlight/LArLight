/**
 * \file vertex_test.hh
 *
 * \ingroup ArianaPi0Test
 * 
 * \brief Class def header for a class vertex_test
 *
 * @author ah673
 */

/** \addtogroup ArianaPi0Test

    @{*/

#ifndef VERTEX_TEST_HH
#define VERTEX_TEST_HH

#include "ana_base.hh"

namespace larlight {
  /**
     \class vertex_test
     User custom analysis class made by ah673
   */
  class vertex_test : public ana_base{
  
  public:

    vertex_test(){ _name="vertex_test"; _fout=0;};

    virtual ~vertex_test(){};

	 TTree* GetTree() const {return _ana_tree;}

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

    protected:


     void PrepareTTree();
 
     void ClearTTreeVars();
 
     TTree* _ana_tree;

//	std::default_random_engine generatorX ;

	double _count ;

	double dist ;
	double _new_dist ;
	
	double _vertex_X ;
	double _vertex_Y ; 
	double _vertex_Z ; 

	double shower_energy; 

	double start_point_X ;
	double start_point_Y ;
	double start_point_Z ;

	int Mother_PDG ;

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
