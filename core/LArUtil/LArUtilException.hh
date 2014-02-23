/**
 * \file LArUtilException.hh
 *
 * \ingroup LArUtil
 * 
 * \brief Class def header for exception classes in LArUtil package
 *
 * @author kazuhiro
 */

/** \addtogroup LArUtil

    @{*/
#ifndef DBEXCEPTION_HH
#define DBEXCEPTION_HH

#include <iostream>
#include <exception>

namespace larutil {
  /**
     \class LArUtilException
     Generic (base) exception class
  */
  class LArUtilException : public std::exception{

  public:

    LArUtilException(std::string msg="") : std::exception(), _msg(msg)
    {}

    virtual ~LArUtilException() throw();
    virtual const char* msg() const throw() 
    {return _msg.c_str(); }

  private:

    std::string _msg;
  };

}
#endif
/** @} */ // end of doxygen group 

