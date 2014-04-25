/**
 * \file FANNService.hh
 *
 * \ingroup FANN
 * 
 * \brief Class def header for a class FANNService
 *
 * @author kazuhiro
 */

/** \addtogroup FANN

    @{*/
#ifndef FANNSERVICE_HH
#define FANNSERVICE_HH

#include <iostream>
#include "FANNModule.hh"
namespace cluster {
  /**
     \class FANNService
     A singleton FANNModule getter
  */
  class FANNService{
    
  private:
    
    /// Default constructor
    FANNService(){};

    /// Default destructor
    virtual ~FANNService(){};

    static FANNService* _me;

  public:

    static FANNService* GetME()
    {
      if(!_me) _me = new FANNService;
      return _me;
    }

    FANNModule& GetFANNModule() { return _module;}

  protected:

    FANNModule _module;
    
  };

}
#endif
/** @} */ // end of doxygen group 

