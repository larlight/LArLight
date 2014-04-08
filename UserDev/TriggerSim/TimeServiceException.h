/**
 * \file TimeServiceException.hh
 *
 * \ingroup TimeService
 * 
 * \brief Class def header for exception classes in TimeService package
 *
 * @author kazuhiro
 */

/** \addtogroup TimeService

    @{*/
#ifndef TIMESERVICEEXCEPTION_H
#define TIMESERVICEEXCEPTION_H

#include <string>
#include <exception>

namespace util {
  /**
     \class TimeServiceException
     Simple exception class for TimeService
  */
  class TimeServiceException : public std::exception{

  public:

    TimeServiceException(std::string msg="") : std::exception(), _msg(msg)
    {}

    virtual ~TimeServiceException() throw(){};
    virtual const char* msg() const throw() 
    {return _msg.c_str(); }

  private:

    std::string _msg;
  };

}
#endif
/** @} */ // end of doxygen group 

