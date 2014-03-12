/**
 * \file DBException.hh
 *
 * \ingroup UBOnlineDBI
 * 
 * \brief Class def header for a class DBException
 *
 * @author kazuhiro
 */

/** \addtogroup UBOnlineDBI

    @{*/
#ifndef DBEXCEPTION_HH
#define DBEXCEPTION_HH

#include <iostream>
#include <exception>

namespace ubpsql {
  /**
     \class DBException
     Generic (base) exception class
  */
  class DBException : public std::exception{
    virtual const char* msg() const throw() 
    {return "Exception occurred";}
  };

  /**
     \class InvalidUser
     Exception for creating connection w/ invalid user
   */
  class InvalidUser : public DBException{
    virtual const char* msg() const throw()
    { return "Invalid DB Connection User Type"; }
  };

  /**
     \class FailedConnection
     Exception for failing to open a connection
   */
  class ConnectionError : public DBException{
    virtual const char* msg() const throw()
    { return "Failed to establish DB connection!"; }
  };
}
#endif
/** @} */ // end of doxygen group 

