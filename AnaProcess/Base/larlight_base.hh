/**
 * \file larlight_base.hh
 *
 * \ingroup Base
 * 
 * \brief Class definition file of larlight_base
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Base

    @{*/

#ifndef LARLIGHT_BASE_HH
#define LARLIGHT_BASE_HH

#include "types.hh"
#include "constants.hh"
#include "messenger.hh"

namespace larlight{
  
  /**
     \class larlight_base
     Very base class of all everything else defined in this framework.
  */
  class larlight_base {
    
  public:
    
    /// Default constructor
    larlight_base() : _name("larlight_base")
    {set_verbosity(MSG::NORMAL);}
    
    /// Default copy constructor
    larlight_base(const larlight_base &original) : _name(original._name){};
    
    /// Default destructor
    virtual ~larlight_base(){};
    
    /// Setter for the verbosity level 
    virtual void set_verbosity(MSG::Level level);
    
    /// Getter for the verbosity level
    inline MSG::Level get_verbosity() const {return _verbosity_level;};
    
    /// Getter for the class name
    inline const std::string class_name() const {return _name;};
    
    /// message print out method
    inline void print(MSG::Level level, std::string where, std::string msg)
    {if(_verbosity[level]) Message::send(level,where,msg);};
    
    /// message print out method
    inline void print(MSG::Level level, std::string msg)
    {if(_verbosity[level]) Message::send(level,msg);};
    
  protected:
    
    char _buf[200];                           ///< char buffer for message manipulation
    bool       _verbosity[MSG::MSG_TYPE_MAX]; ///< holder for enabled message levels
    MSG::Level _verbosity_level;              ///< holder for specified verbosity level
    std::string _name;                        ///< class name holder
    
  };
}
#endif

/** @} */ // end of doxygen group
