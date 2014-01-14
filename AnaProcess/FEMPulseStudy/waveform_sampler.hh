/**
 * \file waveform_sampler.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class waveform_sampler
 *
 * @author kazuhiro
 */

/** \addtogroup Analysis

    @{*/

#ifndef WAVEFORM_SAMPLER_HH
#define WAVEFORM_SAMPLER_HH

#include <TH2D.h>
#include <set>
#include <map>

#include "ana_base.hh"
#include "pulse_selector_simple.hh"

namespace larlight {

  /**
     \class waveform_sampler
     User custom analysis class made by kazuhiro
  */
  class waveform_sampler : public ana_base{
    
  public:
    
    /// Default constructor
    waveform_sampler();
    
    /// Default destructor
    virtual ~waveform_sampler(){};
    
    /** IMPLEMENT in waveform_sampler.cc!
	Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();
    
    /** IMPLEMENT in waveform_sampler.cc! 
	Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);
    
    /** IMPLEMENT in waveform_sampler.cc! 
	Finalize method to be called after all events processed.
    */
    virtual bool finalize();
    
    void set_selection(pulse_selector_base* algo){_selector=algo;};
    
    void set_waveform_length(int length){_wf_length=length;};
    
    TH2D* get_waveform(UShort_t ch){ 
      
      if(_hWF_map.find(ch)==_hWF_map.end()) return 0; 
      return _hWF_map[ch];
      
    };
    
  protected:
    
    void clear_event();
    
    void sample_waveform(const fifo *wf);
    
    std::map<UShort_t,std::map<UInt_t,std::set<UInt_t> > > _sel_info;
    
    pulse_selector_base* _selector;
    
    std::map<UShort_t,TH2D*> _hWF_map;
    
    int _wf_length;
    
  };
}
#endif

/** @} */ // end of doxygen group 
