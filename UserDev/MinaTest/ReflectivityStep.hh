/**
 * \file ReflectivityStep.hh
 *
 * \ingroup MinaTest
 * 
 * \brief Class def header for a class ReflectivityStep
 *
 * @author ehimwich
 */

/** \addtogroup MinaTest

    @{*/
#ifndef REFLECTIVITYSTEP_HH
#define REFLECTIVITYSTEP_HH

#include <iostream>
#include "TObject.h"
#include <vector>

/**
   \class ReflectivityStep
   User defined class ReflectivityStep ... these comments are used to generate
   doxygen documentation!
 */
class ReflectivityStep : public TObject{

public:

  /// Default constructor
  ReflectivityStep(){ clear(); };

  /// Default destructor
  virtual ~ReflectivityStep(){};

  void clear() {
    DelMom.clear();
    Mom.clear();
    DirMom.clear();
    Number = 0;
    Material = "None";
    NextMaterial = "None";

  };

  void SetStep(int _num, std::vector<double> _delMom, std::vector<double> _mom, std::vector<double> _dirMom, std::string _material, std::string _nextMaterial){
    Mom = _mom;
    DelMom = _delMom;
    DirMom = _dirMom;
    Material = _material;
    NextMaterial = _nextMaterial;
    Number = _num;
    
  };

  int fNumber() const {return Number;}
  std::vector<double> fDelMom() const {return DelMom;}
  std::vector<double> fMom() const {return Mom;}
  std::vector<double> fDirMom() const {return DirMom;}
  std::string fMaterial() const {return Material;}
  std::string fNextMaterial() const {return NextMaterial;}


protected:

  int Number;
  std::vector<double> DelMom;
  std::vector<double> Mom;
  std::vector<double> DirMom;
  std::string Material;
  std::string NextMaterial;

  ClassDef(ReflectivityStep, 1);

};

#endif
/** @} */ // end of doxygen group 

