#ifndef FRAMEWORKBASE_HH
#define FRAMEWORKBASE_HH
/**
 * \file FrameworkBase.hh
 *
 * \ingroup SimpleTreeBase
 * 
 * @author Kazu - Nevis 2013
 */

/** \addtogroup SimpleTreeBase
    Base package, pretty much empty. Defines message facility & framework base class.

    @{*/

#include "BasicConstants.hh"
#include "Message.hh"

/**
 \class FrameworkBase
 Very base class of all everything else defined in this framework.
*/
class FrameworkBase {

public:

  /// Default constructor
  FrameworkBase() : _name("FrameworkBase")
  {set_verbosity(MSG::NORMAL);}
  
  /// Default copy constructor
  FrameworkBase(const FrameworkBase &original) : _name(original._name){};

  /// Default destructor
  virtual ~FrameworkBase(){};

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

#endif

/** @} */ // end of doxygen group
