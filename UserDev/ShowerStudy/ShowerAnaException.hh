/**
 * \file ShowerAnaException.hh
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for exception classes in ClusterRecoUtil package
 *
 * @author kazuhiro
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef SHOWERANAEXCEPTION_HH
#define SHOWERANAEXCEPTION_HH

#include <iostream>
#include <exception>

namespace showerana {
  /**
     \class ShowerAnaException
     Generic (base) exception class
  */
  class ShowerAnaException : public std::exception{

  public:
    
    ShowerAnaException(std::string msg="") : std::exception(), _msg(msg)
    {}
    
    virtual ~ShowerAnaException() throw(){};
    virtual const char* what() const throw() 
    {return _msg.c_str(); }

  private:
    
    std::string _msg;
  };

}
#endif
/** @} */ // end of doxygen group 

