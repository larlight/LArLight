/**
 * \file FhiclLiteException.h
 *
 * \ingroup FhiclLite
 * 
 * \brief Class def header for exception classes in FhiclLite package
 *
 * @author kazuhiro
 */

/** \addtogroup FhiclLite

    @{*/
#ifndef TIMESERVICEEXCEPTION_H
#define TIMESERVICEEXCEPTION_H

#include <string>
#include <exception>

namespace fhicl {
  /**
     \class FhiclLiteException
     Simple exception class for FhiclLite
  */
  class FhiclLiteException : public std::exception{

  public:

    FhiclLiteException(std::string msg="") : std::exception(), _msg(msg)
    {}

    virtual ~FhiclLiteException() throw(){};
    virtual const char* what() const throw() 
    {return _msg.c_str(); }

  private:

    std::string _msg;
  };

}
#endif
/** @} */ // end of doxygen group 

