#ifndef MESSENGER_CC
#define MESSENGER_CC

#include "messenger.hh"

Message* Message::me = 0;

void Message::send(MSG::Level level, std::string msg)
{
  std::cout 
    << MSG::ColorPrefix[level].c_str()
    << MSG::StringPrefix[level].c_str()
    << "\033[0m"
    << msg.c_str()
    << std::endl;
}

void Message::send(MSG::Level level, std::string where, std::string msg)
{
  std::cout 
    << MSG::ColorPrefix[level].c_str()
    << MSG::StringPrefix[level].c_str()
    << "\033[0m"
    << "\033[95m"
    << "<"
    << where.c_str()
    << "> "
    << "\033[0m"
    << msg.c_str()
    << std::endl;
}

#endif
