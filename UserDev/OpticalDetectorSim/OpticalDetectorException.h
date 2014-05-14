/**
 * \file OpticalDetectorException.h
 *
 * \ingroup OpticalDetector
 * 
 * \brief Class def header for exception classes in OpticalDetector package
 *
 * @author kazuhiro
 */

/** \addtogroup OpticalDetector

    @{*/
#ifndef OPTICALDETECTOREXCEPTION_H
#define OPTICALDETECTOREXCEPTION_H

#include <string>
#include <exception>

namespace opdet {
  /**
     \class OpticalDetectorException
     Simple exception class for OpticalDetector
  */
  class OpticalDetectorException : public std::exception{

  public:

    OpticalDetectorException(std::string msg="") : std::exception(), _msg(msg)
    {}

    virtual ~OpticalDetectorException() throw(){};
    virtual const char* what() const throw() 
    {return _msg.c_str(); }

  private:

    std::string _msg;
  };

}
#endif
/** @} */ // end of doxygen group 

