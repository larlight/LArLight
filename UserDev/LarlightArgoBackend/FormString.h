#ifndef FORMSTRING_HH
#define FORMSTRING_HH

#include <string>


/// \function FormString
/// \ingroup  Logging
/// \author   Nathaniel Tagg (tagg@minos.phy.tufts.edu)
/// \brief    Utility function to replace sprintf 
///  Use this instead of creating a temporary buffer. Returns
///  a standard string instead of const char*
///
/// Usage: myString = FormString(format,param,param,param...) 

std::string FormString(const char* fmt, ...);


#endif
