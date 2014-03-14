//
// Code for the Arachne Event Display
// Author: Nathaniel Tagg ntagg@otterbein.edu
// 
// Licence: this code is free for non-commertial use. Note other licences may apply to 3rd-party code.
// Any use of this code must include attribution to Nathaniel Tagg at Otterbein University, but otherwise 
// you're free to modify and use it as you like.
//

#include "JsonElement.h"

bool JsonElement::sfPrettyPrint = false;
int  JsonElement::sfDecimals = 2;


std::ostream& operator<< (std::ostream& out, const JsonElement& e)
{
  out << e.str();
  return out;
}


const std::string JsonElement::quotestring( const std::string& input )
{
  std::ostringstream ss;
  ss << "\"";
   for (auto iter = input.begin(); iter != input.end(); iter++) {
   //C++98/03:
   //for (std::string::const_iterator iter = input.begin(); iter != input.end(); iter++) {
       switch (*iter) {
           case '\\': ss << "\\\\"; break;
           case '"': ss << "\\\""; break;
           case '/': ss << "\\/"; break;
           case '\b': ss << "\\b"; break;
           case '\f': ss << "\\f"; break;
           case '\n': ss << "\\n"; break;
           case '\r': ss << "\\r"; break;
           case '\t': ss << "\\t"; break;
           default: ss << *iter; break;
       }
   }
   ss << "\"";
   return ss.str();
}


// ----------------------------------------------------------------------------------
// JsonObject
JsonObject& JsonObject::add(const std::string& key,const JsonElement& value)
{
  if(fElements++>0) {
    fContent << ",";
  }
  if(sfPrettyPrint) fContent << std::endl << "  ";
  fContent << "\"" << key << "\":";
  fContent << value.str();
  return *this;
}


JsonObject& JsonObject::add(const std::string& key,const JsonArray& value)
{
  if(fElements++>0) {
    fContent << ",";
  }
  if(sfPrettyPrint) fContent << std::endl << "  ";
  fContent << "\"" << key << "\":";
  fContent << value.str();
  return *this;
}

JsonObject& JsonObject::addBare(const std::string& key,const std::string& value)
{
  if(fElements++>0) {
    fContent << ",";
  }
  if(sfPrettyPrint) fContent << std::endl << "  ";
  fContent << "\"" << key << "\":";
  fContent << value;
  return *this;
}

const std::string JsonObject::str() const
{
  std::string out = "{";
  out += fContent.str();
  if(sfPrettyPrint) {out += "\n  ";};
  out += "}";
  return out;
}



// ----------------------------------------------------------------------------------
// JsonArray

JsonArray& JsonArray::add(const JsonElement& value)
{
  if(fElements++>0) {
    fContent << ",";
  }
  if(sfPrettyPrint) fContent << std::endl << "  ";
  fContent << value;
  return *this;  
}

JsonArray& JsonArray::add(const JsonObject& value)
{
  if(fElements++>0) {
    fContent << ",";
  }
  if(sfPrettyPrint) fContent << std::endl << "  ";
  fContent << value;
  return *this;  
}

const std::string JsonArray::str() const
{
  std::string out = "[";
  out += fContent.str();
  out += "]";
  return out;
        
        
}
 // Removes linker errors by demanding the templates get resolved.
 // see http://www.parashift.com/c++-faq-lite/templates.html
 // section 35.8 and 35.9
// template JsonObject& JsonObject::add <double>(const std::string&, const double&);
// template JsonObject& JsonObject::add <float> (const std::string&, const float&);
// template JsonObject& JsonObject::add <unsigned int>(const std::string&, const unsigned int&);
// template JsonObject& JsonObject::add <int>         (const std::string&, const int&);
// template JsonObject& JsonObject::add <std::string>         (const std::string&, const std::string&);

