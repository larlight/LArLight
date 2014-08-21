/**
 * \file preco_exception.hh
 *
 * \ingroup FEMPulseReco
 * 
 * \brief Class def header for exception classes in FEMPulseReco package
 *
 * @author kazuhiro
 */

/** \addtogroup PulseReco

    @{*/
#ifndef PRECOEXCEPTION_HH
#define PRECOEXCEPTION_HH

#include <iostream>
#include <exception>

namespace optreco {
  /**
     \class preco_exception
     Generic (base) exception class
  */
  class preco_exception : public std::exception{

  public:

    preco_exception(std::string msg="") : std::exception()
    {
      _msg = "\033[93m";
      _msg += msg;
      _msg += "\033[00m";
    }

    virtual ~preco_exception() throw(){};

    virtual const char* what() const throw() 
    { return _msg.c_str(); }

  private:

    std::string _msg;
  };

}
#endif
/** @} */ // end of doxygen group 

