/**
 * \file RecoUtilException.hh
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for exception classes in ClusterRecoUtil package
 *
 * @author kazuhiro
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef RECOUTILEXCEPTION_HH
#define RECOUTILEXCEPTION_HH

#include <iostream>
#include <exception>

namespace cluster {
  /**
     \class RecoUtilException
     Generic (base) exception class
  */
  class RecoUtilException : public std::exception{

  public:

    RecoUtilException(std::string msg="") : std::exception(), _msg(msg)
    {}

    virtual ~RecoUtilException() throw(){};
    virtual const char* what() const throw() 
    {return _msg.c_str(); }

  private:

    std::string _msg;
  };

}
#endif
/** @} */ // end of doxygen group 

