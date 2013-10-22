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
/**
   \class hit
   LArSoft Wire class equivalent data container
*/
class wire : public data_base {

public:

  /// Default constructor
  wire() : data_base(), fSignal() {clear_data();};

  /// Alternative constructor
  wire(const std::vector<float> wf,
	     const uint32_t ch,
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
  void set_signal(const std::vector<float> wf) { fSignal=wf; };
  /// Setter for a channel number
  void set_channel(uint32_t    ch)   { fChannel = ch;   };
  /// Setter for view type
  void set_view   (GEO::View_t v)    { fView = v;       };
  /// Setter for signal type
  void set_sigtype(GEO::SigType_t s) { fSignalType = s; };

  // Get Methods
  /// Getter for waveform
  const std::vector<float>&  Signal()     const { return fSignal;        };
  /// Getter for view type
  GEO::View_t                View()       const { return fView;          };
  /// Getter for signal type
  GEO::SigType_t             SignalType() const { return fSignalType;    };
  /// Getter for channel number
  uint32_t                   Channel()    const { return fChannel;       };

protected:

  std::vector<float>      fSignal;     ///< the calibrated signal waveform
  GEO::View_t             fView;       ///< view corresponding to the plane of this wire
  GEO::SigType_t          fSignalType; ///< signal type of the plane for this wire
  uint32_t                fChannel;    ///< channel number

private:

  ////////////////////////
  ClassDef(wire,1)
  ////////////////////////

};

/**
 \class event_wire
 A collection storage class of multiple wires.
*/
class event_wire : public data_base {
  
public:

  /// Default constructor
  event_wire() : data_base(), fWire_v() {clear_data();};
  
  /// Default copy constructor
  event_wire(const event_wire& original) : data_base(original), fWire_v(original.fWire_v)
  {};

  /// Default destructor
  ~event_wire(){};
  
  /// Method to clear currently held data contents in the buffer
  virtual void clear_data(){data_base::clear_data(); fWire_v.clear();};
  
  /// Method to add a track
  inline void  add_wire(const wire w) {fWire_v.push_back(w);};

  /// Method to get an array of tracks
  inline const std::vector<wire>& GetWireCollection() { return fWire_v; };

private:

  std::vector<wire> fWire_v;

  ////////////////////////
  ClassDef(event_wire,1)
  ////////////////////////
};

#endif

/** @} */ // end of doxygen group 
