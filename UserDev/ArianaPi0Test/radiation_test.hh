/**
 * \file radiation_test.hh
 *
 * \ingroup ArianaPi0Test
 * 
 * \brief Class def header for a class radiation_test
 *
 * @author ah673
 */

/** \addtogroup ArianaPi0Test

    @{*/

#ifndef RADIATION_TEST_HH
#define RADIATION_TEST_HH

#include "ana_base.hh"

namespace larlight {
  /**
     \class radiation_test
     User custom analysis class made by ah673
   */
  class radiation_test : public ana_base{
  
  public:

    radiation_test(){ _name="radiation_test"; _fout=0;};

    virtual ~radiation_test(){};

	TTree* GetTree() const {return _ana_tree;}

    void PrepareTTree();

	void Clear();

	void MinDistVtxToBoundary(const double x, const double y, const double z, double& x_dist, double& y_dist, double& z_dist );

  /// Getter for a distance from a point to the box boundary
  /// Second argument should be a unit vector
    double DistanceToWall(std::vector<double> const& point,
              std::vector<double> const& dir)  ;
	
	virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

    protected:

	TTree* _ana_tree;

	TH1D* h1 ;

	double _pi0_angle ;
	double _new_pi0_angle ;
	double _new_pi0_mass ;

	double _LAr_radLength =20;
	double s = 0;
	
	std::vector<double> _pi0_mass_wo_cut  ;
	std::vector<double> _pi0_mass_1length  ;
	std::vector<double> _pi0_mass_2lengths  ;
	std::vector<double> _pi0_mass_3lengths  ;
	std::vector<double> _pi0_mass_4lengths  ;
	std::vector<double> _pi0_mass_5lengths  ;
	std::vector<double> _pi0_mass_morelengths  ;
	
	//Energy containment for n rad lengths of gamma contianed within detector
	double _energy_containment ;

	std::vector<double> _Econ0 ;
	std::vector<double> _Econ1 ;
	std::vector<double> _Econ2 ;
	std::vector<double> _Econ3 ;
	std::vector<double> _Econ4 ;
	std::vector<double> _Econ5 ;
	std::vector<double> _Econ_more ;

	double mother1_E ;
	double mother2_E ;

	double _daugh1_E ;

	double _vertex_X ;
	double _vertex_Y ;
	double _vertex_Z ;

	double x_new1, y_new1, z_new1 ;
	double x_new2, y_new2,  z_new2 ;

	//What is wrong with the energy containment radiation lengths cut?????
	double x_dist, y_dist, z_dist ;
	
	std::vector<double> v_x_dist ;
	std::vector<double> v_y_dist ;
	std::vector<double> v_z_dist ;

	int count0 = 0;
	int count1 = 0;
	int count2 = 0;
 	int count3 = 0;
	int count4 = 0;
	int count5 = 0;
	int count6 = 0;

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
