//
// Code for the Arachne Event Display
// Author: Nathaniel Tagg ntagg@otterbein.edu
// 
// Licence: this code is free for non-commertial use. Note other licences may apply to 3rd-party code.
// Any use of this code must include attribution to Nathaniel Tagg at Otterbein University, but otherwise 
// you're free to modify and use it as you like.
//

#include "Timer.h"
#include <math.h>
#include <time.h>

const double kms = 1e-6;

double Timer::GetTimeOfDay()
{
  static struct timeval now;
  gettimeofday(&now,NULL);  
  double time = double(now.tv_sec) + kms*double(now.tv_usec); 
  return time;  
}


double Timer::TimevalToTime(const struct timeval& t)
{
  double time = double(t.tv_sec) + kms*double(t.tv_usec); 
  return time;  
}

void Timer::TimeToTimeval(double time, struct timeval& t)
{
  t.tv_sec  = (long)floor(time);
  t.tv_usec = (long)((time-t.tv_sec)*1000000.);
}


////////////////////////
// Timer class.

Timer::Timer( long sec, long usec )
{ 
  struct timeval t; 
  t.tv_sec=sec; 
  t.tv_usec=usec; 
  mStartTime=TimevalToTime(t); 
};


double 
Timer::Count( void ) 
{
  double now = GetTimeOfDay();
  return now-mStartTime;
};

// Return TRUE if it has been longer than inSecs seconds since 
// the last timer reset.
bool 
Timer::Test( double inSecs )
{
  if( Count() > inSecs) return true;
  return false;
};


void
Timer::Camp( double inSecs )
{
  double timeRemaining = (inSecs - Count());
  while(timeRemaining>0) {
    struct timeval timeout;
    TimeToTimeval(timeRemaining,timeout);
    select(0, NULL, NULL, NULL, &timeout);
    timeRemaining = (inSecs - Count());
  }
  
}

std::string Timer::AsString()
{
  struct timeval t;
  struct tm      stm;
  TimeToTimeval(mStartTime,t);
  ::localtime_r(&t.tv_sec,&stm);
  char buffer[256];
  ::strftime(buffer, sizeof(buffer), "%a %b %d %H:%M:%S %Y", &stm);
  return std::string(buffer);
}
