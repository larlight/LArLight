////////////////////////////////////////////////////////////////////////
//
//  UBTriggerAlgo source
//
////////////////////////////////////////////////////////////////////////

#ifndef UBTRIGGERALGO_CXX
#define UBTRIGGERALGO_CXX

#include "UBTriggerAlgo.h"

namespace trigger{

  /*
  //##################################################################
  UBTriggerAlgo::UBTriggerAlgo(fhicl::ParameterSet const& pset)
    : _tpc_clock(pset.get< fhicl::ParameterSet >("ClockTPC")),
      _pmt_clock(pset.get< fhicl::ParameterSet >("ClockPMT")),
      _trigger_clock(pset.get< fhicl::ParameterSet >("ClockTrigger"))
  //##################################################################
  {    
    _trigger_counter=0;

    // Debug flag
    SetDebugMode(pset.get<bool>("DebugMode"));

    // Deadtime 
    SetDeadtime(pset.get<uint16_t>("Deadtime") );

    // Mask
    SetMask(pset.get<std::vector<uint16_t> >("Mask"));

    // Prescale
    SetPrescale(pset.get<std::vector<bool> >("Prescale"));

    // BNB gate parameters
    SetBNBParams( pset.get<uint16_t>("GateWidthBNB"),
                  pset.get<uint16_t>("TrigDelayBNB"),
		  pset.get<uint16_t>("CosmicStartBNB"),
		  pset.get<uint16_t>("CosmicEndBNB") );

    // NuMI gate parameters
    SetNuMIParams( pset.get<uint16_t>("GateWidthNuMI"),
                   pset.get<uint16_t>("TrigDelayNuMI"),
		   pset.get<uint16_t>("CosmicStartNuMI"),
		   pset.get<uint16_t>("CosmicEndNuMI") );


    ClearPredefinedTriggers();

    // BNB trigger
    _bnb_timings = pset.get<std::vector<double> >("PermTriggerBNB",std::vector<double>())

    // NuMI trigger
    _numi_timings = pset.get<std::vector<double> >("PermTriggerNuMI",std::vector<double>())

    // Calib trigger
    _calib_timings = pset.get<std::vector<double> >("PermTriggerCalib",std::vector<double>())

    // Calib trigger
    _calib_timings = pset.get<std::vector<double> >("PermTriggerCalib",std::vector<double>())

    // Ext trigger
    _ext_timings = pset.get<std::vector<double> >("PermTriggerExt",std::vector<double>())

    // PC trigger
    _pc_timings = pset.get<std::vector<double> >("PermTriggerPC",std::vector<double>())

    ReportConfig();

    ClearInputTriggers();
  }
  */

  //##############################################################
  UBTriggerAlgo::UBTriggerAlgo() : _tpc_clock(3200,2.e6,0.),
				   _pmt_clock(102400,64.e6,0.),
				   _trigger_clock(25600,16.e6,0.),
				   _mask(9,0),
				   _prescale(9,false)
  //##############################################################
					      {
    // LArLight version constructor
    SetDeadtime(4);
    SetBNBParams(102,256,0,0);
    SetNuMIParams(102,256,0,0);
  }

  //######################################################
  void UBTriggerAlgo::Report(const std::string &msg) const
  //######################################################
  {
    std::cout << msg.c_str() << std::endl;
  }

  //##############################################################
  void UBTriggerAlgo::RaiseTriggerException(std::string msg) const
  //##############################################################
  {
    //throw cet::exception(__FUNCTION__) << msg.c_str();
    throw std::invalid_argument(msg.c_str());
    std::exit(1);
  }
  
  //#########################################################
  void UBTriggerAlgo::SetBNBParams(unsigned short width,
				   unsigned short delay,
				   unsigned short cosmic_min,
				   unsigned short cosmic_max) 
  //#########################################################
  {
    if(_bnb_cosmic_allow_max < _bnb_cosmic_allow_min) {
      RaiseTriggerException("BNB Cosmic allow window max is smaller than min!");
      _bnb_cosmic_allow_max = _bnb_cosmic_allow_min;
    }
    _bnb_gate_width       = width;
    _bnb_delay            = delay;
    _bnb_cosmic_allow_min = cosmic_min;
    _bnb_cosmic_allow_max = cosmic_max;
  }

  //######################################################
  void UBTriggerAlgo::SetNuMIParams(unsigned short width,
			       unsigned short delay,
			       unsigned short cosmic_min,
			       unsigned short cosmic_max) 
  //######################################################
  {
    if(_numi_cosmic_allow_max < _numi_cosmic_allow_min) {

      _numi_cosmic_allow_max = _numi_cosmic_allow_min;
    }    
    _numi_gate_width       = width;
    _numi_delay            = delay;
    _numi_cosmic_allow_min = cosmic_min;
    _numi_cosmic_allow_max = cosmic_max;
  }


  //#############################################################
  void UBTriggerAlgo::SetMask(unsigned char index, uint32_t mask)
  //#############################################################
  {
    if(index>9) RaiseTriggerException("Index >9 cannot be set!");
    _mask[index]=mask;
  }

  //#################################################################
  void UBTriggerAlgo::SetPrescale(unsigned char index, bool prescale)
  //#################################################################
  {
    if(index>9) RaiseTriggerException("Index >9 cannot be set!");
    _prescale[index]=prescale;
  }

  //##################################################################
  void UBTriggerAlgo::SetMask(const std::vector<unsigned short> &mask) 
  //##################################################################
  {
    if(_mask.size()!=9) {
      RaiseTriggerException(Form("Length of masks (=%zu) is invalid! Initializing to right length...",
				 mask.size()));
      _mask.resize(9,0);
    }else
      _mask = mask;

  }

  //##################################################################
  void UBTriggerAlgo::SetPrescale(const std::vector<bool> &prescale) 
  //##################################################################
  {
    if(_prescale.size()!=9) {
      RaiseTriggerException(Form("Length of prescales (=%zu) is invalid! Initializing to right length...",
				 prescale.size()));
      _prescale.resize(9,0);
    }else
      _prescale = prescale;

  }

  //######################################
  void UBTriggerAlgo::ClearInputTriggers()
  //######################################
  {
    _candidates.clear();

    for( auto const t : _bnb_timings ) AddTriggerBNB(t);

    for( auto const t : _numi_timings ) AddTriggerNuMI(t);

    for( auto const t : _calib_timings ) AddTriggerCalib(t);

    for( auto const t : _ext_timings ) AddTriggerExt(t);

    for( auto const t : _pc_timings ) AddTriggerPC(t);

  }

  //###########################################
  void UBTriggerAlgo::AddTriggerCalib(double t)
  //###########################################
  {
    
    // Calibration triggers
    AddTrigger(trigdata::Trigger(0,
				 _pmt_clock.MCSample(t),
				 _pmt_clock.MCFrame(t),
				 _pmt_clock.MCSample(t),
				 _pmt_clock.MCFrame(t),
				 ( (0x1) << trigger::kTriggerCalib )) 
	       );
  }

  //###########################################
  void UBTriggerAlgo::AddTriggerExt(double t)
  //###########################################
  {
    // External triggers
    AddTrigger(trigdata::Trigger(0,
				 _pmt_clock.MCSample(t),
				 _pmt_clock.MCFrame(t),
				 _pmt_clock.MCSample(t),
				 _pmt_clock.MCFrame(t),
				 ( (0x1) << trigger::kTriggerEXT ))
	       );
  }

  //#########################################
  void UBTriggerAlgo::AddTriggerPC(double t)
  //#########################################
  {
    // PC triggers
    AddTrigger(trigdata::Trigger(0,
				 _pmt_clock.MCSample(t),
				 _pmt_clock.MCFrame(t),
				 _pmt_clock.MCSample(t),
				 _pmt_clock.MCFrame(t),
				 ( (0x1) << trigger::kTriggerPC ))
	       );
  }

  //#######################################
  void UBTriggerAlgo::ReportConfig() const
  //#######################################
  {
    std::ostringstream msg;

    msg
      << std::endl
      << " UBTriggerAlgo Configuration:              " << std::endl
      << "---------------------------------------------" << std::endl;

    msg << " Debug Mode ... " << (_debug_mode ? "enabled!" : "disabled!") << std::endl
	<< std::endl;

    for(size_t i=0; i<_mask.size(); ++i)

      msg << Form("  Mask %zu     : %d", i, _mask[i]) << std::endl;
    
    msg << std::endl;

    for(size_t i=0; i<_prescale.size(); ++i)

      msg << Form("  Prescale %zu : %d", i, _prescale[i]) << std::endl;

    msg << std::endl;

    msg
      << Form(" Trigger Deadtime   : %d",_deadtime) << std::endl
      << std::endl
      << Form("  NuMI Trigger Delay : %d",_numi_delay) << std::endl
      << Form("  NuMI Cosmic Start  : %d",_numi_cosmic_allow_min) << std::endl
      << Form("  NuMI Cosmic End    : %d",_numi_cosmic_allow_max) << std::endl
      << std::endl
      << Form("  BNB Trigger Delay  : %d",_bnb_delay) << std::endl
      << Form("  BNB Cosmic Start   : %d",_bnb_cosmic_allow_min) << std::endl
      << Form("  BNB Cosmic End     : %d",_bnb_cosmic_allow_max) << std::endl
      << "---------------------------------------------" << std::endl
      << std::endl;

    //mf::LogInfo(__FUNCTION__) << msg.str();
    Report(msg.str());
  }

  //#######################################################################################
  const trigdata::Trigger UBTriggerAlgo::CombineTriggers(const trigdata::Trigger &trigger1, 
							 const trigdata::Trigger &trigger2)
  //#######################################################################################
  {

    if(_debug_mode)
      
      Report(Form("  Attempting to combine two triggers: (%d,%d) and (%d,%d)",
		  trigger1.TriggerSample(),
		  trigger1.TriggerFrame(),
		  trigger2.TriggerSample(),
		  trigger2.TriggerFrame()) );

    if( trigger1.TriggerNumber() != trigger2.TriggerNumber() ) {
      RaiseTriggerException("Cannot combine triggers with different trigger counters!");
      return trigdata::Trigger();
    }

    if( trigger1.TriggerFrame() != trigger2.TriggerFrame() ) {
      RaiseTriggerException("Cannot combine triggers in different frames!");
      return trigdata::Trigger();
    }

    auto const res_number = trigger1.TriggerNumber();
    auto const res_frame  = trigger1.TriggerFrame();

    auto const trig1_sample = trigger1.TriggerSample();
    auto const trig2_sample = trigger2.TriggerSample();
    if( _trigger_clock.Sample(_pmt_clock,trig1_sample) != _trigger_clock.Sample(_pmt_clock,trig2_sample) ){
      RaiseTriggerException("Cannot combine triggers in different trigger clock sample number!");
      return trigdata::Trigger();
    }

    auto const res_sample = (trig1_sample < trig2_sample ? trig1_sample : trig2_sample);

    // Construct beam gate region. Note that this is relevant only if either of
    // two candidates is beam. Else do nothing.
    unsigned int beam_frame  = 0;
    unsigned int beam_sample = 0;

    // Case1: both are beam ... not supported for now
    if( (trigger1.Triggered(trigger::kTriggerBNB) || trigger1.Triggered(trigger::kTriggerNuMI)) &&
	(trigger2.Triggered(trigger::kTriggerBNB) || trigger2.Triggered(trigger::kTriggerNuMI)) ) {
      RaiseTriggerException("Combining two beam gates not supported for now!");
      return trigdata::Trigger();
    }
    // Case2: only trigger 1 is beam
    else if( (trigger1.Triggered(trigger::kTriggerBNB) || trigger1.Triggered(trigger::kTriggerNuMI)) ) {
      
      beam_frame  = trigger1.BeamGateFrame();
      beam_sample = trigger1.BeamGateSample();

    }
    // Case3: only trigger 2 is beam
    else if( (trigger2.Triggered(trigger::kTriggerBNB) || trigger2.Triggered(trigger::kTriggerNuMI)) ) {

      beam_frame  = trigger2.BeamGateFrame();
      beam_sample = trigger2.BeamGateSample();

    }
    // Case4: neither is beam ... use trigger timing
    else{

      beam_frame  = res_frame;
      beam_sample = res_sample;

    }

    if(_debug_mode) Report(Form("    Combined beam timing @ (%d,%d)", beam_sample, beam_frame));

    // Combine trigger bits
    uint32_t res_bits = 0x0;
    for(unsigned char i=0; i < 4*(sizeof(res_bits)); ++i) {
      
      if( trigger1.Triggered(i) || trigger2.Triggered(i) )
	{
	  res_bits += (uint32_t)( (0x1) << i );
	  if(_debug_mode) Report(Form("    Combined bit %d ... now %d",i,res_bits));
	}
    }
    
    return trigdata::Trigger(res_number,
			     res_sample,
			     res_frame,
			     beam_sample,
			     beam_frame,
			     res_bits);

  }

  //##################################################################
  void UBTriggerAlgo::AddTrigger(const trigdata::Trigger &new_trigger)
  //##################################################################
  {
    unsigned int frame  = new_trigger.TriggerFrame();
    unsigned int sample = _trigger_clock.Sample(_pmt_clock,new_trigger.TriggerSample());

    if(_debug_mode) {

      Report(Form("Requested to add a new trigger @ (%d, %d)",
		  new_trigger.TriggerSample(),
		  new_trigger.TriggerFrame()));

    }

    auto frame_iter = _candidates.find(frame);

    if( frame_iter == _candidates.end() ) {

      _candidates[frame] = std::map<unsigned int,trigdata::Trigger>();
      _candidates[frame].insert(std::pair<unsigned int,trigdata::Trigger>(sample,new_trigger));
      
    }else{

      auto sample_iter = (*frame_iter).second.find(sample);

      if( sample_iter == (*frame_iter).second.end() )

	(*frame_iter).second.insert(std::pair<unsigned int,trigdata::Trigger>(sample,new_trigger));
      
      else {

	trigdata::Trigger combined_trigger = CombineTriggers(new_trigger, (*sample_iter).second);

	_candidates[frame][sample]=combined_trigger;

      }
    }
  }

  //
  // LArSoft interface functions ... comment out to compile for LArLight
  //
  /*
  //######################################################################
  void UBTriggerAlgo::AddTriggerBNB(const art::Ptr<sim::BeamGateInfo> bgi)
  //######################################################################
  {
    AddTriggerBNB(bgi->Start());
  }

  //######################################################################
  void UBTriggerAlgo::AddTriggerNuMI(const art::Ptr<sim::BeamGateInfo> bgi)
  //######################################################################
  {
    AddTriggerNuMI(bgi->Start());
  }

  //################################################################################
  void UBTriggerAlgo::AddTriggerPMT(const art::Ptr<optdata::PMTTrigger> pmt_trigger)
  //################################################################################
  {

    // Trigger bits
    uint32_t trig_bits=0x0;
    switch(pmt_trigger->Category()) {
    case optdata::kUndefined:
    case optdata::kHighGain:
    case optdata::kLowGain:
    case optdata::kFEMCosmicHighGain:
    case optdata::kFEMCosmicLowGain:
    case optdata::kFEMBeamHighGain:
    case optdata::kFEMBeamLowGain:
      RaiseTriggerException("Unknown type PMT trigger received!");
      return;
    case optdata::kBeamPMTTrigger:
      AddTriggerPMTBeam( pmt_trigger->TimeSlice(), pmt_trigger->Frame() );
      break;
    case optdata::kCosmicPMTTrigger:
      AddTriggerPMTCosmic( pmt_trigger->TimeSlice(), pmt_trigger->Frame() );
      break;
    }
  }
  */
  //
  // End of LArSoft interface functions 
  //

  //###############################################
  void UBTriggerAlgo::AddTriggerPMTCosmic(double t)
  //###############################################
  {
    AddTriggerPMTCosmic( _pmt_clock.MCSample(t), _pmt_clock.MCFrame(t) );
  }

  //###############################################
  void UBTriggerAlgo::AddTriggerPMTBeam(double t)
  //###############################################
  {
    AddTriggerPMTBeam( _pmt_clock.MCSample(t), _pmt_clock.MCFrame(t) );
  }

  //#################################################################
  void UBTriggerAlgo::AddTriggerPMTCosmic(unsigned int trig_sample, 
					     unsigned int trig_frame)
  //#################################################################
  {
    // Trigger bits
    unsigned int trig_bits=0x0;
    trig_bits += ( (0x1) << kPMTTriggerCosmic );
    trig_bits += ( (0x1) << kPMTTrigger );
    
    // Beam frame & sample number ... for PMT trigger, this is same as trigger frame/sample
    unsigned int beam_frame  = trig_frame;
    unsigned int beam_sample = trig_sample;

    // Create this trigger candidate object
    trigdata::Trigger trig_candidate(0,
				     trig_sample, trig_frame,
				     beam_sample, beam_frame,
				     trig_bits);

    // Add this trigger candidate
    AddTrigger(trig_candidate);
  }

  //################################################################
  void UBTriggerAlgo::AddTriggerPMTBeam(unsigned int trig_sample, 
					   unsigned int trig_frame)
  //################################################################
  {
   // Trigger bits
    unsigned int trig_bits=0x0;
    trig_bits += ( (0x1) << kPMTTriggerBeam );
    trig_bits += ( (0x1) << kPMTTrigger );
    
    // Beam frame & sample number ... for PMT trigger, this is same as trigger frame/sample
    unsigned int beam_frame  = trig_frame;
    unsigned int beam_sample = trig_sample;

    // Create this trigger candidate object
    trigdata::Trigger trig_candidate(0,
				     trig_sample, trig_frame,
				     beam_sample, beam_frame,
				     trig_bits);

    // Add this trigger candidate
    AddTrigger(trig_candidate);
  }

  //################################################################
  void UBTriggerAlgo::AddTriggerBNB(double start)
  //################################################################
  {
    // Trigger frame number
    unsigned int trig_frame  = _pmt_clock.MCFrame(start);
    // Trigger sample number
    unsigned int trig_sample = _pmt_clock.MCSample(start);

    // Beam frame and sample numbers
    unsigned int beam_timing = trig_sample + _pmt_clock.Sample(_trigger_clock,_bnb_delay);
    unsigned int beam_sample = 0;
    unsigned int beam_frame  = 0;
    _pmt_clock.Tick2SampleFrame( beam_timing, beam_sample, beam_frame );
    beam_frame += trig_frame;

    uint32_t trig_bits = ( (0x1) << trigger::kTriggerBNB );
 
    // Create this trigger candidate object
    trigdata::Trigger trig_candidate(0,
				     trig_sample, trig_frame,
				     beam_sample, beam_frame,
				     trig_bits);

    // Add this trigger candidate
    AddTrigger(trig_candidate);
  }

  //###############################################
  void UBTriggerAlgo::AddTriggerNuMI(double start)
  //###############################################
  {
    // Trigger frame number
    unsigned int trig_frame  = _pmt_clock.MCFrame(start);
    // Trigger sample number
    unsigned int trig_sample = _pmt_clock.MCSample(start);

    // Beam frame and sample numbers
    unsigned int beam_timing = trig_sample + _pmt_clock.Sample(_trigger_clock,_numi_delay);
    unsigned int beam_sample = 0;
    unsigned int beam_frame  = 0;
    _pmt_clock.Tick2SampleFrame( beam_timing, beam_sample, beam_frame );
    beam_frame += trig_frame;

    uint32_t trig_bits = ( (0x1) << trigger::kTriggerNuMI );

    // Create this trigger candidate object
    trigdata::Trigger trig_candidate(0,
				     trig_sample, trig_frame,
				     beam_sample, beam_frame,
				     trig_bits);

    // Add this trigger candidate
    AddTrigger(trig_candidate);

  }
  
  //###############################################
  void UBTriggerAlgo::ShowCandidateTriggers() const
  //###############################################
  {
    auto frame_iter = _candidates.begin();

    std::ostringstream msg;

    msg
      << "######################################################"
      << "Trigger Inputs"
      << "######################################################" << std::endl
      << std::endl
      << Form("  %-18s : %-18s : %-18s : %-18s : %-18s : %-18s : %-32s",
	      "Trig Sample(16MHz)",
	      "Trig Frame (16MHz)",
	      "Trig Sample(64MHz)",
	      "Trig Frame (64MHz)",
	      "Beam Sample(64Mhz)",
	      "Beam Frame (64Mhz)",
	      "Bits") << std::endl
      << std::endl;

    while(frame_iter!=_candidates.end()) {
      
      auto sample_iter = (*frame_iter).second.begin();

      while(sample_iter != (*frame_iter).second.end()) {

	unsigned int decision_sample = (*sample_iter).first;
	unsigned int decision_frame  = (*frame_iter).first;
	unsigned int trigger_sample  = (*sample_iter).second.TriggerSample();
	unsigned int trigger_frame   = (*sample_iter).second.TriggerFrame();
	unsigned int beam_sample     = (*sample_iter).second.BeamGateSample();
	unsigned int beam_frame      = (*sample_iter).second.BeamGateFrame();
	
	msg
	  << Form("  %-18d : %-18d : %-18d : %-18d : %-18d : %-18d : ",
		  decision_sample,
		  decision_frame,
		  trigger_sample,
		  trigger_frame,
		  beam_sample,
		  beam_frame);

	for(unsigned char k=0; k<32; ++k)

	  msg << ((*sample_iter).second.Triggered(k) ? 1 : 0);

	msg << std::endl;

	sample_iter++;
      }
      frame_iter++;
    }
    msg
      << std::endl
      << "######################################################"
      << "##############"
      << "######################################################" << std::endl
      << std::endl;

    Report(msg.str());
  }

  //#########################################################################
  void UBTriggerAlgo::ProcessTrigger(std::vector<trigdata::Trigger> &triggers)
  //#########################################################################
  {
    triggers.clear();

    if(_debug_mode)
      
      ShowCandidateTriggers();

    // Create deadtime and PMT-Trigger allow time windows
    trigger::time_window_t deadtime(0,0,0);

    trigger::time_window_t bnb_active(0,0,0);

    trigger::time_window_t numi_active(0,0,0);

    trigger::time_window_t bnb_gate(0,0,0);

    trigger::time_window_t numi_gate(0,0,0);

    auto const mask0 = _mask.at(0);
    auto const mask1 = _mask.at(1);
    auto const mask8 = _mask.at(8);
    auto const scale0 = _prescale.at(0);
    auto const scale1 = _prescale.at(1);
    auto const scale8 = _prescale.at(8);

    // Loop over candidate frames
    auto frame_iter = _candidates.begin();
    while( frame_iter != _candidates.end() ) {

      // Make sure sample exists if frame is found
      auto sample_iter = (*frame_iter).second.begin();
      
      if( sample_iter == (*frame_iter).second.end() ) {
	
	RaiseTriggerException("Logic error: found candidate frame but no associated sample & trigger!");
	triggers.clear();
	return;
      }    

      // Loop over samples
      while( sample_iter != (*frame_iter).second.end() ) {

	if(_debug_mode) Report(Form("\n  Processing candidate @ %d, %d",
				    (*sample_iter).first,
				    (*frame_iter).first));

	// If in deadtime, continue
	if( InWindow(deadtime, (*sample_iter).first, (*frame_iter).first) ) {
	  if(_debug_mode) Report(Form("    Skipping as deadtime is (%d,%d) => %d samples",
				      deadtime.start_sample,
				      deadtime.start_frame,
				      deadtime.duration));
	  sample_iter++;
	  continue;
	}

	auto const pmt0   = (*sample_iter).second.Triggered(trigger::kPMTTriggerCosmic);
	auto const pmt1   = (*sample_iter).second.Triggered(trigger::kPMTTriggerBeam);
	auto const numi   = (*sample_iter).second.Triggered(trigger::kTriggerNuMI);
	auto const bnb    = (*sample_iter).second.Triggered(trigger::kTriggerBNB);
	auto const calib  = (*sample_iter).second.Triggered(trigger::kTriggerCalib);
	auto const ext    = (*sample_iter).second.Triggered(trigger::kTriggerEXT);
	auto const pc     = (*sample_iter).second.Triggered(trigger::kTriggerPC);
	auto const active = ( InWindow(bnb_active, (*sample_iter).first, (*frame_iter).first) ||
			      InWindow(numi_active, (*sample_iter).first, (*frame_iter).first) );

	// If BNB or NuMI, open new beam trigger gate
	if(bnb) {
	  bnb_gate.start_sample = (*sample_iter).first;
	  bnb_gate.start_frame  = (*frame_iter).first;
	  bnb_gate.duration = _bnb_gate_width;
	}
	if(numi) {
	  numi_gate.start_sample = (*sample_iter).first;
	  numi_gate.start_frame  = (*frame_iter).first;
	  numi_gate.duration = _numi_gate_width;
	}
	
	auto const bnb_gate_active  = InWindow(bnb_gate,  (*sample_iter).first, (*frame_iter).first);
	auto const numi_gate_active = InWindow(numi_gate, (*sample_iter).first, (*frame_iter).first);

	if(_debug_mode)
	  {
	    std::ostringstream msg;
	    msg 
	      << Form("    PMT Cosmic? %s", (pmt0  ? "yes" : "no")) << std::endl
	      << Form("    PMT Beam?   %s", (pmt1  ? "yes" : "no")) << std::endl
	      << Form("    NuMI Gate?  %s", (numi  ? "yes" : "no")) << std::endl
	      << Form("    BNB Gate?   %s", (bnb   ? "yes" : "no")) << std::endl
	      << Form("    Calib?      %s", (calib ? "yes" : "no")) << std::endl
	      << Form("    External?   %s", (ext   ? "yes" : "no")) << std::endl
	      << Form("    PC?         %s", (pc    ? "yes" : "no")) << std::endl
	      << std::endl
	      << Form("    NuMI   Window? %s", (numi_gate_active  ? "yes" : "no")) << std::endl
	      << Form("    BNB    Window? %s", (bnb_gate_active ? "yes" : "no")) << std::endl
	      << Form("    Cosmic Window? %s", (active           ? "yes" : "no")) << std::endl;
	    Report(msg.str());
	  }



	// Evaludate trigger condition p0 ... this is for PMT Cosmic trigger
	// Equation follows Nevis FPGA code including similar notations
	bool p0 = ( (numi   && pmt0 && ((0x1) & (mask0 >> 0))           ) ||
		    (bnb    && pmt0 && ((0x1) & (mask0 >> 1))           ) ||
		    (active && pmt0 && ((0x1) & (mask0 >> 2)) && !scale0) ||
		    (          pmt0 && ((0x1) & (mask0 >> 3)) && !scale0) );

	// Evaluate trigger condition p1 ... this is for PMT Beam trigger
	// Equation follows Nevis FPGA code including similar notations
	bool p1 = ( (numi   && pmt1 && ((0x1) & (mask1 >> 0))           ) || 
		    (bnb    && pmt1 && ((0x1) & (mask1 >> 1))           ) ||
		    (active && pmt1 && ((0x1) & (mask1 >> 2)) && !scale1) ||
		    (          pmt1 && ((0x1) & (mask1 >> 3)) && !scale1) );

	// Evaluate trigger condition p8 ... this is for Beam trigger input
	// Equation follows Nevis FPGA code including similar notations
	bool p8 = ( (active && ext    && ((0x1) & (mask8 >> 0)) && !scale8) ||
		    (          ext    && ((0x1) & (mask8 >> 1)) && !scale8) ||
		    (active && pc     && ((0x1) & (mask8 >> 2))           ) ||
		    (          pc     && ((0x1) & (mask8 >> 3))           ) ||
		    (bnb_gate_active  && ((0x1) & (mask8 >> 4))           ) ||
		    (numi_gate_active && ((0x1) & (mask8 >> 5))           ) ||
		    (           calib && ((0x1) & (mask8 >> 6))           ) );

	if(_debug_mode) {
	  
	  std::ostringstream msg;

	  msg << Form("    p0 condition ... %s", (p0 ? "satisfied" : "not met")) << std::endl
	      << Form("    p1 condition ... %s", (p1 ? "satisfied" : "not met")) << std::endl
	      << Form("    p8 condition ... %s", (p8 ? "satisfied" : "not met")) << std::endl;
	  Report(msg.str());
	}


	// Ask OR condition of p0, p1, and p8
	if( p0 || p1 || p8 ) {
	  // New trigger found. 

	  // Assign deadtime
	  deadtime.start_sample = (*sample_iter).first;
	  deadtime.start_frame  = (*frame_iter).first;
	  deadtime.duration     = _deadtime * _trigger_clock.FrameTick();

	  // Assign active window for bnb/numi
	  if(bnb) {
	    unsigned int allow_duration     = _bnb_cosmic_allow_max - _bnb_cosmic_allow_min;
	    unsigned int allow_start_sample = 0;
	    unsigned int allow_start_frame  = 0;
	    _trigger_clock.Tick2SampleFrame( ((*sample_iter).first + _bnb_cosmic_allow_min),
					     allow_start_sample,
					     allow_start_frame );
	    allow_start_frame += (*frame_iter).first;
	    
	    bnb_active.start_sample = allow_start_sample;
	    bnb_active.start_frame  = allow_start_frame;
	    bnb_active.duration     = allow_duration;
	  }
	  if(numi) {
	    unsigned int allow_duration     = _numi_cosmic_allow_max - _numi_cosmic_allow_min;
	    unsigned int allow_start_sample = 0;
	    unsigned int allow_start_frame  = 0;
	    _trigger_clock.Tick2SampleFrame( ((*sample_iter).first + _numi_cosmic_allow_min),
					     allow_start_sample,
					     allow_start_frame );
	    allow_start_frame += (*frame_iter).first;
	    
	    numi_active.start_sample = allow_start_sample;
	    numi_active.start_frame  = allow_start_frame;
	    numi_active.duration     = allow_duration;
	  }
	  
	  // Store trigger object
	  triggers.push_back( trigdata::Trigger( _trigger_counter,
						 (*sample_iter).second.TriggerSample(),
						 (*sample_iter).second.TriggerFrame(),
						 (*sample_iter).second.BeamGateSample(),
						 (*sample_iter).second.BeamGateFrame(),
						 (*sample_iter).second.TriggerBits() ) );
	  _trigger_counter++;
	} // end if trigger found
	sample_iter++;
      } // end looping over samples
      frame_iter++;
    } // end looping over frames
    
  }

  //###############################################################################################
  bool UBTriggerAlgo::InWindow(time_window_t window, unsigned int sample, unsigned int frame) const
  //###############################################################################################
  {
    if( frame < window.start_frame ) return false;

    if( frame == window.start_frame ) {

      if( sample < window.start_sample ) return false;
      else return (sample < (window.start_sample + window.duration));

    } else {

      if(_trigger_clock.FrameTick() < window.start_sample)

	RaiseTriggerException("Logic error: found time window sample number larger than frame size!");

      unsigned int diff_tick = (_trigger_clock.FrameTick()) * (frame - window.start_frame) - window.start_sample + sample;
      return (diff_tick < window.duration);

    }
    
  }

}

#endif
