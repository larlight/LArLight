/**
 * \file DBBase.hh
 *
 * \ingroup UBOnlineDBI
 * 
 * \brief Base class for DBI package
 *
 * @author kazuhiro
 */

/** \addtogroup UBOnlineDBI

    @{*/
#ifndef DBBASE_HH
#define DBBASE_HH

#include <iostream>
#include <sstream>

namespace ubpsql {

  namespace MSG{

    /// Message level ENUM
    enum MSGLevel_t {
      kDEBUG=0,
      kINFO,
      kNORMAL,
      kWARNING,
      kERROR
    };
  }
  
  /**
     \class DBBase
     Base class for other classes in DB package
  */
  
  class DBBase{
    
  public:
    
    /// Default constructor
    DBBase(){ _msg_level=MSG::kNORMAL;}
    
    /// Default destructor
    virtual ~DBBase(){}

    /// Message level setter
    void SetVerbosity(MSG::MSGLevel_t level)
    {_msg_level = level; }

    /// debug level message attribute
    void Print(const MSG::MSGLevel_t level,
	       const std::string& msg1,
	       const std::string& msg2="") const;
  protected:

    MSG::MSGLevel_t _msg_level;

  };
}

#endif
/** @} */ // end of doxygen group 

