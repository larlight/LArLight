/**
 * \file UBTriggerAlgo.h
 *
 * \ingroup TriggerSim
 * 
 * \brief Class def header for a class UBTriggerAlgo
 *
 * @author kterao
 */

/** \addtogroup TriggerSim

    @{*/

#ifndef UBTRIGGERALGO_H
#define UBTRIGGERALGO_H

// STL
#include <map>
#include <vector>

//LArLight
#include "TriggerData.h"
#include "ElecClock.h"

namespace trigger
{
  /**
    \class UBTriggerAlgo
    MicroBooNE trigger simulation algorithm. This algorithm instance takes same configuration
    parameters as the real trigger electronics module. It is equiped with setter functions for
    PC, Calib, Ext, BNB, NuMI, PMT-Cosmic, and PMT-Beam trigger inputs, and assess the same
    trigger logic on those inputs (if the author did not make a mistake). 9 trigger conditions
    can be configured while only p0, p1, and p8 are implemented at this point as that is the
    case also for the real trigger module (as of 04/16/14).
    */
  class UBTriggerAlgo {

  private:
    
    /// Utility data type to store time window
    typedef std::pair<util::ElecClock,util::ElecClock> time_window_t;

  public:

    /// Default constructor with fhicl parameters
    //UBTriggerAlgo(fhicl::ParameterSet const& pset);
    UBTriggerAlgo();

    /// Default destructor
    ~UBTriggerAlgo(){}

    /// Method to report the current configuration
    void ReportConfig() const;

    /// Method to clear input trigger information
    void ClearInputTriggers();

    /// Function to process algorithm
    void ProcessTrigger(std::vector<raw::Trigger> &triggers);

    /// Function to print out current list of candidate triggers
    void ShowCandidateTriggers() const;

    /// Given trigger time, return TPC readout start time
    util::ElecClock ReadOutStartTimeTPC(const util::ElecClock& time) const;

    /// Given trigger time, return PMT readout start time
    util::ElecClock ReadOutStartTimeOptical(const util::ElecClock& time) const;

    /// Given trigger time, return BNB beam gate window start time
    util::ElecClock BNBStartTime(const util::ElecClock& time) const;

    /// Given trigger time, return NuMI beam gate window start time
    util::ElecClock NuMIStartTime(const util::ElecClock& time) const;
    
  public: // Change this to protected for LArSoft and public for LArLight

    /// Function to set an individual trigger mask
    void SetMask(unsigned char index, uint32_t mask);

    /// Function to set an individual trigger prescale
    void SetPrescale(unsigned char index, bool prescale);

    /// Function to set trigger masks for 8 trigger conditions
    void SetMask(const std::vector<unsigned short> &mask);

    /// Function to set prescales for 8 trigger conditions
    void SetPrescale(const std::vector<bool> &prescale);

    /// Function to set Debug mode
    void SetDebugMode(bool debug)
    {_debug_mode = debug; }

    /// Function to set deadtime in # of frames
    void SetDeadtime(unsigned short deadtime) 
    {_deadtime = deadtime;}

    /// Function to set BNB related parameters
    void SetBNBParams(unsigned short width,
		      unsigned short delay,
		      unsigned short cosmic_min,
		      unsigned short cosmic_max);
    
    /// Function to set NUMI related parameters
    void SetNuMIParams(unsigned short width,
		       unsigned short delay,
		       unsigned short cosmic_min,
		       unsigned short cosmic_max);
    
    /// Function to add calibration trigger with G4 time input
    void AddTriggerCalib(const util::ElecClock& time);

    /// Function to add External trigger with G4 time input
    void AddTriggerExt(const util::ElecClock& time);

    /// Function to add PC trigger with G4 time input
    void AddTriggerPC(const util::ElecClock& time);

    /// Functon to add BNB beam gate input
    void AddTriggerBNB(const util::ElecClock& time);

    /// Function to add NuMI beam gate input
    void AddTriggerNuMI(const util::ElecClock& time);

    /// Function to add PMT trigger input from G4 time
    void AddTriggerPMTCosmic(const util::ElecClock& time);

    /// Function to add PMT trigger input from G4 time
    void AddTriggerPMTBeam(const util::ElecClock& time);

  protected:
    
    /// Function to raise an error
    void RaiseTriggerException(std::string msg) const;

    /// Function to send a string to stdout stream
    void Report(const std::string &msg) const;

    /// Function to append new trigger candidate to _candidates
    void AddTrigger(const raw::Trigger &new_trigger);

    /// Function to combine two trigger objects
    const raw::Trigger CombineTriggers(const raw::Trigger &trigger1, 
				       const raw::Trigger &trigger2);

    /**
       Utility function to check if the given time stamp is inside the provided time_window_t
       assuming sample number provided and used in time_window_t is in trigger clock unit
    */
    bool InWindow(const util::ElecClock &time,
		  const time_window_t  &window) const
    { return ( time <= window.second && time >= window.first ); }

  protected:

    /// stores CANDIDATE readout trigger timestamps 
    std::map<unsigned int, std::map<unsigned int,raw::Trigger> > _candidates;

    /// Various clocks
    util::ElecClock _tpc_clock;
    util::ElecClock _pmt_clock;
    util::ElecClock _trig_clock;
    
    unsigned int   _trigger_counter; ///< Trigger counter
    bool           _debug_mode; ///< Verbose mode for debugging
    unsigned short _deadtime;   ///< Trigger dead time

    std::vector<unsigned short> _mask;     ///< Masks for 8 trigger conditions
    std::vector<bool>           _prescale; ///< Prescales for 8 trigger conditions

    short _readout_frame_offset;  ///< Offset from the triggered frame to the beginning of readout window
    short _tpc_readout_offset;    ///< Offset from trigger time to TPC readout window start in sample number
    
    unsigned short _bnb_gate_width;        ///< BNB trigger gate width
    unsigned short _bnb_delay;             ///< BNB trigger delay
    unsigned short _bnb_cosmic_allow_min;  ///< BNB cosmic allow window start after BNB
    unsigned short _bnb_cosmic_allow_max;  ///< BNB cosmic allow window end after BNB

    unsigned short _numi_gate_width;       ///< BNB trigger gate width
    unsigned short _numi_delay;            ///< NUMI trigger delay
    unsigned short _numi_cosmic_allow_min; ///< NUMI cosmic allow window start after NUMI
    unsigned short _numi_cosmic_allow_max; ///< NUMI cosmic allow window end after NUMI

    /// Predefined BNB trigger timings ... not cleared by ClearInputTriggers
    std::vector<util::ElecClock> _bnb_timings;

    /// Predefined NuMI trigger timings ... not cleared by ClearInputTriggers
    std::vector<util::ElecClock> _numi_timings;

    ///< Predefined External trigger timings ... not cleared by ClearInputTriggers
    std::vector<util::ElecClock> _ext_timings;

    ///< Predefined PC trigger timings ... not cleared by ClearInputTriggers
    std::vector<util::ElecClock> _pc_timings;

    ///< Predefined Calibration trigger timings ... not cleared by ClearInputTriggers
    std::vector<util::ElecClock> _calib_timings;

  }; // class UBTriggerAlgo
  
} //namespace cluster
#endif
/** @} */ // end of doxygen group 
