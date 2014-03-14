#ifndef TIMER_H
#define TIMER_H
// Simple class good as an egg timer.
//#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string>


class Timer {
 public:
  static double GetTimeOfDay();
  static double TimevalToTime(const struct timeval& t);
  static void   TimeToTimeval(double time, struct timeval& t);

 private:
  double         mStartTime;
  
 public:
  Timer ( double start ) { mStartTime = start; };
  Timer ( const struct timeval& t ) { mStartTime = TimevalToTime(t); };
  Timer ( long sec, long usec);

  Timer ( void ) { Reset(); };
  void   Reset ( void ) { mStartTime=GetTimeOfDay(); };
  bool   Test ( double inSecs );  // Have inSecs passed?
  double Count( void );           // How many secs have passed?
  void   Camp( double inSecs );   // Sit and do nothing until inSecs seconds have elapsed (since last reset)
  std::string AsString();

  double GetStartTime() { return mStartTime; };
};

#endif






