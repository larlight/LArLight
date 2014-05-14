/**
 * \file SimpleChConfig.h
 *
 * \ingroup OpticalDetector
 * 
 * \brief Class def header for a class SimpleChConfig
 *
 * @author kazuhiro
 */

/** \addtogroup OpticalDetector

    @{*/
#ifndef SIMPLECHCONFIG_H
#define SIMPLECHCONFIG_H

#include <vector>
#include "OpticalDetectorException.h"
#include "OpticalDetectorConstants.h"
namespace opdet {
  /**
     \class SimpleChConfig
     User defined class SimpleChConfig ... these comments are used to generate
     doxygen documentation!
  */
  class SimpleChConfig{
    
  protected:
    
    /// Default constructor
    SimpleChConfig();
    
    /// Default destructor
    virtual ~SimpleChConfig(){};

  public:

    /// Parameter getter for a single channel
    float GetParameter(const ChConfigType_t type, 
		       const unsigned short ch) const;

    /// Parameter getter for whole channels
    const std::vector<float>& GetParameter(const ChConfigType_t type) const;

  protected:
    
    std::vector<std::vector<float> > fParams;

    std::vector<float>  fDefault;
    
  };
}

#endif
/** @} */ // end of doxygen group 

