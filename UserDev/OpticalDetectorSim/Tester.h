/**
 * \file Tester.h
 *
 * \ingroup OpticalDetector
 * 
 * \brief Class def header for a class Tester
 *
 * @author kazuhiro
 */

/** \addtogroup OpticalDetector

    @{*/
#ifndef TESTER_H
#define TESTER_H

#include "DataFormat-TypeDef.hh"
#include "Analysis-TypeDef.hh"
#include "UBOpticalADC.h"
#include "ChConfig.h"
#include "WFAlgoArray.h"
#include "WFAlgoAnalyticalSPE.h"
#include "WFAlgoPedestal.h"

namespace opdet {
  /**
     \class Tester
     User defined class Tester ... these comments are used to generate
     doxygen documentation!
  */
  class Tester : public ::larlight::ana_base {
    
  public:
    
    /// Default constructor
    Tester();
    
    /// Default destructor
    virtual ~Tester(){}

    virtual bool initialize();

    virtual bool analyze(::larlight::storage_manager* storage);

    virtual bool finalize();

  protected:

    UBOpticalADC fGen;
    
  };
}

#endif
/** @} */ // end of doxygen group 

