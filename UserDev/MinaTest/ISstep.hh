/**
 * \file ISstep.hh
 *
 * \ingroup MinaTest
 * 
 * \brief Class def header for a class ISstep
 *
 * @author ehimwich
 */

/** \addtogroup MinaTest

    @{*/
#ifndef ISSTEP_HH
#define ISSTEP_HH

#include <iostream>
#include <TObject.h>

/**
   \class ISstep
   User defined class ISstep ... these comments are used to generate
   doxygen documentation!
 */
class ISstep : public TObject{

public:

  /// Default constructor
  ISstep(){ clear(); };

  /// Default destructor
  virtual ~ISstep(){};
  
  void clear() {
    Size = Energy = 0.;
    Photons = Electrons = 0;
    Number = 0;
  };

  void SetStep(int _num, double _size, double _energy, int _photons, int _electrons) {
    Size = _size;
    Energy = _energy;
    Photons = _photons;
    Electrons = _electrons;
    Number = _num;
  };

  double fSize() const {return Size;}
  double fEnergy() const {return Energy;}
  int fPhotons() const {return Photons;}
  int fElectrons() const {return Electrons;}
  int fNumber() const {return Number;}

protected:

  double Size;
  double Energy;
  int Photons;
  int Electrons;
  int Number;

  ClassDef(ISstep, 1);
  

};

#endif
/** @} */ // end of doxygen group 

