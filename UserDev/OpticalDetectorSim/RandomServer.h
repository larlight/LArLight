/**
 * \file RandomServer.h
 *
 * \ingroup OpticalDetector
 * 
 * \brief Class def header for a class RandomServer
 *
 * @author kazuhiro
 */

/** \addtogroup OpticalDetector

    @{*/
#ifndef RANDOMSERVER_H
#define RANDOMSERVER_H

#include <iostream>
#include <TRandom3.h>

namespace opdet {
  /**
     \class RandomServer
     User defined class RandomServer ... these comments are used to generate
     doxygen documentation!
  */
  class RandomServer : public TRandom3{
    
  private:
    
    /// Default constructor
    RandomServer(){};
    
    /// Default destructor
    virtual ~RandomServer(){};

    static RandomServer* _me;
    
  public:

    static RandomServer& GetME()
    {

      if(!_me) _me = new RandomServer;

      return *_me;
    }

  };
}

#endif
/** @} */ // end of doxygen group 

