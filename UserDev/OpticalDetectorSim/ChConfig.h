/**
 * \file ChConfig.h
 *
 * \ingroup OpticalDetector
 * 
 * \brief Class def header for a class ChConfig
 *
 * @author kazuhiro
 */

/** \addtogroup OpticalDetector

    @{*/
#ifndef CHCONFIG_H
#define CHCONFIG_H

#include "SimpleChConfig.h"
namespace opdet {
  /**
     \class ChConfig
     User defined class ChConfig ... these comments are used to generate
     doxygen documentation!
  */
  class ChConfig : public SimpleChConfig{
    
  private:
    
    /// Default constructor
    ChConfig() : SimpleChConfig() {};
    
    /// Default destructor
    virtual ~ChConfig(){};

    /// Singleton pointer
    static ChConfig* _me;

  public:

    /// Singleton instance getter
    static const ChConfig& GetME()
    {
      if(!_me) _me = new ChConfig;
      return *_me;
    }

  };
}

#endif
/** @} */ // end of doxygen group 

