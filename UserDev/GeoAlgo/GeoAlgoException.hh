/**
 * \file GeoAlgoException.hh
 *
 * \ingroup GeoAlgo
 * 
 * \brief Class def header for exception classes in GeoAlgo package
 *
 * @author davidkaleko
 */

/** \addtogroup GeoAlgo

    @{*/
#ifndef GEOALGOEXCEPTION_HH
#define GEOALGOEXCEPTION_HH

#include <iostream>
#include <exception>

namespace geoalgo {
  /**
     \class GeoAlgoException
     Generic (base) exception class
  */
  class GeoAlgoException : public std::exception{

  public:
    
    GeoAlgoException(std::string msg="") : std::exception(), _msg(msg)
    {}
    
    virtual ~GeoAlgoException() throw(){};
    virtual const char* what() const throw() 
    {return _msg.c_str(); }

  private:
    
    std::string _msg;
  };

}
#endif
/** @} */ // end of doxygen group 

