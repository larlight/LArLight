/**
 * \file wire.hh
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for wire data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef WIRE_HH
#define WIRE_HH

#include "data_base.hh"

namespace larlight {
  /**
     \class wire
     LArSoft Wire class equivalent data container
  */
  class wire : public data_base {
    
  public:
    
    /// Default constructor
    wire() : data_base(), fSignal() {clear_data();};
    
    /// Alternative constructor
    wire(const std::vector<Float_t> wf,
	 const UInt_t ch,
	 const GEO::View_t view,
	 const GEO::SigType_t sig) 
      : data_base(), 
	fSignal(wf),
	fView(view),
	fSignalType(sig),
	fChannel(ch)
    {};
    
    /// Copy constructor
    wire(const wire& original) : data_base(original),
				 fSignal(original.fSignal),
				 fView(original.fView),
				 fSignalType(original.fSignalType),
				 fChannel(original.fChannel)
    {};
    
    /// Default destructor
    virtual ~wire(){};
    
    void clear_data();
    
    /// Setter for an waveform
    void set_signal(const std::vector<Float_t> wf) { fSignal=wf; }
    /// Setter for a channel number
    void set_channel(UInt_t    ch)   { fChannel = ch;   }
    /// Setter for view type
    void set_view   (GEO::View_t v)    { fView = v;       }
    /// Setter for signal type
    void set_sigtype(GEO::SigType_t s) { fSignalType = s; }
    
    // Get Methods
    /// Getter for waveform
    const std::vector<Float_t>&  Signal()     const { return fSignal;        }
    /// Getter for view type
    GEO::View_t                View()       const { return fView;          }
    /// Getter for signal type
    GEO::SigType_t             SignalType() const { return fSignalType;    }
    /// Getter for channel number
    UInt_t                   Channel()    const { return fChannel;       }
    
  protected:
    
    std::vector<Float_t> fSignal;     ///< the calibrated signal waveform
    GEO::View_t        fView;       ///< view corresponding to the plane of this wire
    GEO::SigType_t     fSignalType; ///< signal type of the plane for this wire
    UInt_t             fChannel;    ///< channel number
    
  private:
    
    ////////////////////////
    ClassDef(wire,1)
    ////////////////////////
      
  };

  /**
     \class event_wire
     A collection storage class of multiple wires.
  */
  class event_wire : std::vector<larlight::wire>,
		     public data_base {
    
  public:
    
    /// Default constructor
    event_wire() : std::vector<larlight::wire>(), data_base() {clear_data();}
    
    /// Default copy constructor
    event_wire(const event_wire& original) : std::vector<larlight::wire>(original), data_base(original)
    {}
    
    /// Default destructor
    ~event_wire(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){data_base::clear_data(); clear();}
    
  private:
    
    ////////////////////////
    ClassDef(event_wire,2)
    ////////////////////////
  };
};
#endif

/** @} */ // end of doxygen group 
