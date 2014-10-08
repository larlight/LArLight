/**
 * \file CMTException.hh
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for exception classes in CMTool package
 *
 * @author kazuhiro
 */

/** \addtogroup CMTool

    @{*/
#ifndef CMTEXCEPTION_HH
#define CMTEXCEPTION_HH

#include <iostream>
#include <exception>

namespace cmtool {
  /**
     \class CMTException
     Generic (base) exception class
  */
  class CMTException : public std::exception{

  public:

    CMTException(std::string msg="") : std::exception(), _msg(msg)
    {}

    virtual ~CMTException() throw(){};
    virtual const char* what() const throw() 
    {return _msg.c_str(); }

  private:

    std::string _msg;
  };

}
#endif
/** @} */ // end of doxygen group 

