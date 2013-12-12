#ifndef PMTFIFO_HH
#define PMTFIFO_HH

#include "data_base.hh"
namespace larlight {

  class pmtfifo : public data_base {
    
  public:
    
    pmtfifo() : data_base(), fWaveform() {clear_data();};
    
    pmtfifo(UShort_t ch, int32_t frame, int32_t sample, 
	    PMT::DISCRIMINATOR disc,
	    const std::vector<UShort_t> wf) : data_base(), 
					      fWaveform(wf),
					      fCh(ch),
					      fFrame(frame),
					      fSample(sample),
					      fDiscID(disc)
    {};
    
    pmtfifo(const pmtfifo& original) : data_base(original),
				       fWaveform(original.fWaveform),
				       fCh(original.fCh),
				       fFrame(original.fFrame),
				       fSample(original.fSample),
				       fDiscID(original.fDiscID)
    {};
    
    void clear_data();
    
    inline void append_adc(const UShort_t adc){fWaveform.push_back(adc);};
    
    inline void set_waveform(const std::vector<UShort_t> wf){fWaveform=wf;};
    
    const std::vector<UShort_t>& waveform() const {return fWaveform;};
    
    UShort_t channel() const {return fCh;};
    
    UInt_t frame()   const {return fFrame;};
    
    UInt_t sample()  const {return fSample;};
    
    PMT::DISCRIMINATOR disc_id() const {return fDiscID;};
    
    virtual ~pmtfifo(){};
    
  protected:
    
    std::vector<UShort_t> fWaveform;
    UShort_t fCh;
    UInt_t fFrame;
    UInt_t fSample;
    PMT::DISCRIMINATOR fDiscID;
    
  private:
    
    ////////////////////////
    ClassDef(pmtfifo,1)
    ////////////////////////
      
  };
  
  class event_pmt : public data_base {

  public:
    
    event_pmt() : data_base(), fFIFO_v() {clear_data();};
    
    event_pmt(const event_pmt& original) : data_base(original),
					 fFIFO_v(original.fFIFO_v)
    {};
    
    virtual ~event_pmt(){};
    
    void clear_data() {data_base::clear_data(); fFIFO_v.clear();};
    
    inline void add_fifo(const pmtfifo fifo) {fFIFO_v.push_back(fifo);};
    
    const std::vector<larlight::pmtfifo>& GetFIFOCollection() const {return fFIFO_v;};
    
  protected:
    
    std::vector<pmtfifo> fFIFO_v;
    
  private:
    
    ////////////////////////
    ClassDef(event_pmt,2)
    ////////////////////////
      
  };
};
#endif

