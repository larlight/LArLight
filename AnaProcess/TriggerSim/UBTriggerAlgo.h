////////////////////////////////////////////////////////////////////////
// \file UBTriggerAlgo.h
//
// \brief UBTriggerAlgo class header file
//
// \author kazuhiro@nevis.columbia.edu
//
////////////////////////////////////////////////////////////////////////

#ifndef UBTRIGGERALGO_H
#define UBTRIGGERALGO_H

// ART includes
//#include "art/Persistency/Common/Ptr.h" 
//#include "fhiclcpp/ParameterSet.h"
//#include "messagefacility/MessageLogger/MessageLogger.h"

// LArSoft
//#include "Simulation/BeamGateInfo.h"
//#include "OpticalDetectorData/PMTTrigger.h"
//#include "UBTriggerTypes.h"

// Uboonecode
//#include "uboone/TriggerData/TriggerData.h"
//#include "uboone/ClockManager/SimpleClock.h"

// STL
#include <map>
#include <vector>
#include <iostream>

// ROOT
#include <TString.h>

//LArLight
#include "UBTriggerTypes.h"
#include "TriggerData.h"
#include "SimpleClock.h"

namespace trigger
{
  
  struct time_window_t {
    
    unsigned int start_sample;
    unsigned int start_frame;
    unsigned int duration;

    time_window_t() {
      start_sample = 0;
      start_frame  = 0;
      duration     = 0;
    }

    time_window_t(unsigned int sample,
		  unsigned int frame,
		  unsigned int period)
    {
      start_sample = sample;
      start_frame  = frame;
      duration     = period;
    }
  };

  class UBTriggerAlgo {

  public:

    /// Default constructor with fhicl parameters
    //UBTriggerAlgo(fhicl::ParameterSet const& pset);
    UBTriggerAlgo();

    /// Default destructor
    virtual ~UBTriggerAlgo(){}

    /// Method to report the current configuration
    void ReportConfig() const;

    /// Method to clear input trigger information
    void ClearInputTriggers();

    /// Method to append PMT-Trigger information
    //void AppendTriggerPMT(const art::Ptr<optdata::PMTTrigger> pmt_trigger);

    /// Method to append BeamGateInfo for BNB beamgate
    //void AppendTriggerBNB(const art::Ptr<sim::BeamGateInfo> bgi);

    /// Method to append BeamGateInfo for NuMI beamgate
    //void AppendTriggerNuMI(const art::Ptr<sim::BeamGateInfo> bgi);

    /// Function to process algorithm
    void ProcessTrigger(std::vector<trigdata::Trigger> &triggers);

    /// Function to print out current list of candidate triggers
    void ShowCandidateTriggers() const;
    
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
    void AddTriggerCalib(double t);

    /// Function to add External trigger with G4 time input
    void AddTriggerExt(double t);

    /// Function to add PC trigger with G4 time input
    void AddTriggerPC(double t);

    /// Functon to add BNB beam gate input
    void AddTriggerBNB(double start);

    /// Function to add NuMI beam gate input
    void AddTriggerNuMI(double start);

    /// Function to add PMT trigger input from G4 time
    void AddTriggerPMTCosmic(double t);

    /// Function to add PMT trigger input from G4 time
    void AddTriggerPMTBeam(double t);

    /// Function to add PMT trigger using sample/frame numbers
    void AddTriggerPMTCosmic(unsigned int sample, unsigned int frame);

    /// Function to add PMT trigger using sample/frame numbers
    void AddTriggerPMTBeam(unsigned int sample, unsigned int frame);

  protected:
    
    /// Function to raise an error
    void RaiseTriggerException(std::string msg) const;

    /// Function to send a string to stdout stream
    void Report(const std::string &msg) const;

    /// Function to append new trigger candidate to _candidates
    void AddTrigger(const trigdata::Trigger &new_trigger);

    /// Function to combine two trigger objects
    const trigdata::Trigger CombineTriggers(const trigdata::Trigger &trigger1, 
					    const trigdata::Trigger &trigger2);

    /**
       Utility function to check if the given time stamp is inside the provided time_window_t
       assuming sample number provided and used in time_window_t is in trigger clock unit
    */
    bool InWindow(time_window_t window, unsigned int sample, unsigned int frame) const;

  protected:

    /// stores CANDIDATE readout trigger timestamps 
    std::map<unsigned int, std::map<unsigned int,trigdata::Trigger> > _candidates;

    /// Various clocks
    trigger::SimpleClock _tpc_clock;
    trigger::SimpleClock _pmt_clock;
    trigger::SimpleClock _trigger_clock;
    
    UInt_t   _trigger_counter; ///< Trigger counter
    bool     _debug_mode; ///< Verbose mode for debugging
    UShort_t _deadtime;   ///< Trigger dead time

    std::vector<UShort_t> _mask;     ///< Masks for 8 trigger conditions
    std::vector<bool>     _prescale; ///< Prescales for 8 trigger conditions
    
    UShort_t _bnb_gate_width;        ///< BNB trigger gate width
    UShort_t _bnb_delay;             ///< BNB trigger delay
    UShort_t _bnb_cosmic_allow_min;  ///< BNB cosmic allow window start after BNB
    UShort_t _bnb_cosmic_allow_max;  ///< BNB cosmic allow window end after BNB

    UShort_t _numi_gate_width;       ///< BNB trigger gate width
    UShort_t _numi_delay;            ///< NUMI trigger delay
    UShort_t _numi_cosmic_allow_min; ///< NUMI cosmic allow window start after NUMI
    UShort_t _numi_cosmic_allow_max; ///< NUMI cosmic allow window end after NUMI

    /// Predefined BNB trigger timings ... not cleared by ClearInputTriggers
    std::vector<double> _bnb_timings;

    /// Predefined NuMI trigger timings ... not cleared by ClearInputTriggers
    std::vector<double> _numi_timings;

    ///< Predefined External trigger timings ... not cleared by ClearInputTriggers
    std::vector<double> _ext_timings;

    ///< Predefined PC trigger timings ... not cleared by ClearInputTriggers
    std::vector<double> _pc_timings;

    ///< Predefined Calibration trigger timings ... not cleared by ClearInputTriggers
    std::vector<double> _calib_timings;

  }; // class ClusterMatchAlg
  
} //namespace cluster
#endif
