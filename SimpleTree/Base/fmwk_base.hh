/**
 * \file fmwk_base.hh
 *
 * \ingroup SimpleTreeBase
 * 
 * \brief Class definition file of fmwk_base
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup SimpleTreeBase

    @{*/

#ifndef FMWK_BASE_HH
#define FMWK_BASE_HH

#include "types.hh"
#include "constants.hh"
#include "messenger.hh"

/**
 \class fmwk_base
 Very base class of all everything else defined in this framework.
*/
class fmwk_base {

public:

  /// Default constructor
  fmwk_base() : _name("fmwk_base")
  {set_verbosity(MSG::NORMAL);}
  
  /// Default copy constructor
  fmwk_base(const fmwk_base &original) : _name(original._name){};

  /// Default destructor
  virtual ~fmwk_base(){};

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
