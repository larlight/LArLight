////////////////////////////////////////////////////////////////////////
//
//  SimpleClock.cxx
//
////////////////////////////////////////////////////////////////////////

#ifndef SIMPLECLOCK_CXX
#define SIMPLECLOCK_CXX

#include "SimpleClock.h"

namespace trigger{

  /*
  //##########################################
  SimpleClock(fhicl::ParameterSet const& pset)
    : _beam_sample(kINVALID_SAMPLE),
      _beam_frame(kINVALID_FRAME),
      _trigger_sample(kINVALID_SAMPLE),
      _trigger_frame(kINVALID_FRAME)
  //##########################################
  {
    SetFrameSize(pset.get<unsigned int> ("FrameSize"));
    SetClockSpeed(pset.get<double>       ("ClockSpeed"));
    SetMCStart(pset.get<double>       ("MCStartT",0.));
  }
  */

  //###############################################
  SimpleClock::SimpleClock(unsigned int frame_size,
			   double clock_speed,
			   double mc_start) 
    : _beam_sample(kINVALID_SAMPLE),
      _beam_frame(kINVALID_FRAME),
      _trigger_sample(kINVALID_SAMPLE),
      _trigger_frame(kINVALID_FRAME)
  //###############################################
  {    
    
    _frame_size  = frame_size;
    _clock_speed = clock_speed;
    _mc_start    = mc_start;

    ReportConfig();
  }

  //####################################
  void SimpleClock::ReportConfig() const
  //####################################
  {
    std::ostringstream msg;

    msg
      << std::endl
      << " SimpleClock Configuration:              " << std::endl
      << "---------------------------------------------" << std::endl;

    msg << Form(" Frame size: %d",_frame_size) << std::endl
	<< Form(" MC Start Time: %g",_mc_start) << std::endl
	<< std::endl;

    //mf::LogInfo(__FUNCTION__) << msg.str();
    std::cout << msg.str() << std::endl;

  }

  //###########################################################
  void SimpleClock::SetTriggerData(const trigdata::Trigger &td)
  //###########################################################
  {
    _trigger_sample = td.TriggerSample();
    _trigger_frame  = td.TriggerFrame();
    _beam_sample    = td.BeamGateSample();
    _beam_frame     = td.BeamGateFrame();
  }

  //####################################
  void SimpleClock::CheckDataSet() const
  //####################################
  {
    if(_trigger_sample == kINVALID_SAMPLE)

      throw std::invalid_argument("Data must be set prior to call this function!");
      std::exit(1);
  }

  //########################################################################################################
  void SimpleClock::Period2SampleFrame(const double period, unsigned int &sample, unsigned int &frame) const
  //########################################################################################################
  {
    unsigned int tick( period / TickPeriod() );
    Tick2SampleFrame(tick, sample, frame);
  }
  
  //##########################################################################################################
  void SimpleClock::Tick2SampleFrame(const unsigned int tick, unsigned int &sample, unsigned int &frame) const
  //##########################################################################################################
  {
    frame  = tick / FrameTick();
    sample = tick - frame * FrameTick();
  }

  //############################################################################################
  double SimpleClock::TimeFromTrigger(const unsigned int sample, const unsigned int frame) const
  //############################################################################################
  {
    return ( (TickFromTrigger(sample,frame)) * TickPeriod() );
  }

  //#########################################################################################
  int SimpleClock::TickFromTrigger(const unsigned int sample, const unsigned int frame) const
  //#########################################################################################
  {
    CheckDataSet();
    return ( int(frame - _trigger_frame) * int(_frame_size) + int(sample - _trigger_sample) );
  }

  //#############################################################################################
  double SimpleClock::TimeFromBeamGate(const unsigned int sample, const unsigned int frame) const
  //#############################################################################################
  {
    return ( (TickFromBeamGate(sample,frame)) * TickPeriod() );
  }
  
  //##########################################################################################
  int SimpleClock::TickFromBeamGate(const unsigned int sample, const unsigned int frame) const
  //##########################################################################################
  {
    CheckDataSet();
    return ( int(frame - _beam_frame) * int(_frame_size) + int(sample - _beam_sample) );
  }
    
  //##########################################################################################
  unsigned int SimpleClock::Sample(const SimpleClock& clock, const unsigned int sample) const
  //##########################################################################################
  {
    return (unsigned int)( clock.TickPeriod() * sample / TickPeriod() );
  }

  //############################################################
  unsigned int SimpleClock::MCFrame(const double time_sec) const
  //############################################################
  {
    if(!_frame_size) return 0;

    if(time_sec < _mc_start)  {

      throw std::invalid_argument(Form("World began @ %g: cannot convert the given time stamp %g",
				       _mc_start,time_sec));
      std::exit(1);

    }

    return (unsigned int)((time_sec - _mc_start) / FramePeriod());
    
  }

  //#############################################################
  unsigned int SimpleClock::MCSample(const double time_sec) const
  //#############################################################
  {

    double time_in_frame = time_sec - FramePeriod() * MCFrame(time_sec);
    
    return (unsigned int)(time_in_frame * _clock_speed);

  }

}

#endif
