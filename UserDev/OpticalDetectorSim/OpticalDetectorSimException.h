/**
 * \file OpticalDetectorSimException.h
 *
 * \ingroup OpticalDetector
 * 
 * \brief Class def header for exception classes in OpticalDetector package
 *
 * @author kazuhiro
 */

/** \addtogroup OpticalDetector

    @{*/
#ifndef OPTICALDETECTORSIMEXCEPTION_H
#define OPTICALDETECTORSIMEXCEPTION_H

#include <string>
#include <exception>

namespace opdet {
  /**
     \class OpticalDetectorSimException
     Simple exception class for OpticalDetector
  */
  class OpticalDetectorSimException : public std::exception{

  public:

    OpticalDetectorSimException(std::string msg="") : std::exception(), _msg(msg)
    {}

    virtual ~OpticalDetectorSimException() throw(){};
    virtual const char* what() const throw() 
    {return _msg.c_str(); }

  private:

    std::string _msg;
  };

}
#endif
/** @} */ // end of doxygen group 

