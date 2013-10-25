/**
 * \file hit.hh
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for hit data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef HIT_HH
#define HIT_HH

#include "data_base.hh"
/**
   \class hit
   LArSoft Hit class equivalent data container
*/
class hit : public data_base {

public:

  /// Default constructor
  hit() : data_base() , fHitSignal() {clear_data();};

  /// Copy constructor
  hit(const hit& original);

  /// Default destructor
  virtual ~hit(){};

  void clear_data();

  // Set methods
  void set_waveform  (const std::vector<double>& wf) { fHitSignal = wf;};
  void set_times     (double start, double peak, double end)
  {fStartTime = start; fPeakTime = peak; fEndTime = end;};
  void set_times_err (double start, double peak, double end)
  {fSigmaStartTime = start; fSigmaPeakTime = peak; fSigmaEndTime = end;};
  void set_charge    (double q, double q_max)
  {fCharge=q; fMaxCharge = q_max;};
  void set_charge_err(double q, double q_max)
  {fSigmaCharge=q; fSigmaMaxCharge=q_max;};
  void set_multiplicity(int m){ fMultiplicity=m;};
  void set_channel (UInt_t ch){ fChannel=ch;};
  void set_fit_goodness(double v){ fGoodnessOfFit = v;};
  void set_view(GEO::View_t v){ fView = v;};
  void set_sigtype(GEO::SigType_t t) {fSignalType=t;};

  // Get Methods 
  const std::vector<double>& HitSignal()      const { return fHitSignal;      };  
  double          StartTime()                 const { return fStartTime;      };  
  double          EndTime()                   const { return fEndTime;        };
  double          PeakTime()                  const { return fPeakTime;       };
  double          SigmaStartTime()            const { return fSigmaStartTime; };
  double          SigmaEndTime()              const { return fSigmaEndTime;   };
  double          SigmaPeakTime()             const { return fSigmaPeakTime;  };
  int             Multiplicity()              const { return fMultiplicity;   };
  UInt_t        Channel()                   const { return fChannel;        };
  double          Charge(bool max=false)      const { return ( max ? fMaxCharge : fCharge);};
  double          SigmaCharge(bool max=false) const { return ( max ? fSigmaMaxCharge : fSigmaCharge ); };
  double          GoodnessOfFit()             const { return fGoodnessOfFit;  };
  GEO::SigType_t  SignalType()                const { return fSignalType;     };
  GEO::View_t     View()                      const { return fView;           };

protected:

  std::vector<double>     fHitSignal;      ///< vector of ADC values within the hit window
  double                  fStartTime;      ///< initial tdc tick for hit 
  double                  fSigmaStartTime; ///< uncertainty on initial tick
  double                  fPeakTime;       ///< tdc for the peak charge deposition
  double                  fSigmaPeakTime;  ///< uncertainty for tdc of the peak
  double                  fEndTime;        ///< final tdc tick for hit
  double                  fSigmaEndTime;   ///< uncertainty on final tick
  double                  fCharge;         ///< total charge deposited for hit
  double                  fSigmaCharge;    ///< uncertainty in total charge deposited
  double                  fMaxCharge;      ///< maximum ADC value in hit window
  double                  fSigmaMaxCharge; ///< maximum ADC value in hit window
  int                     fMultiplicity;   ///< how many hits could this one be shared with
  double                  fGoodnessOfFit;  ///< how well do we believe we know this hit?
  GEO::View_t             fView;           ///< view for the plane of the hit
  GEO::SigType_t          fSignalType;     ///< signal type for the plane of the hit
  UInt_t                fChannel;        ///< channel number

private:

  ////////////////////////
  ClassDef(hit,1)
  ////////////////////////

};

/**
 \class event_hit
 A collection storage class of multiple hits.
*/
class event_hit : public data_base {
  
public:

  /// Default constructor
  event_hit() : data_base(), fHit_v() {clear_data();};
  
  /// Default copy constructor
  event_hit(const event_hit& original) : data_base(original), fHit_v(original.fHit_v)
  {};

  /// Default destructor
  ~event_hit(){};
  
  /// Method to clear currently held data contents in the buffer
  virtual void clear_data(){data_base::clear_data(); fHit_v.clear();};
  
  /// Method to add a hit
  inline void  add_hit(const hit h) {fHit_v.push_back(h);};

  /// Method to get an array of hit
  inline const std::vector<hit>& GetHitCollection() { return fHit_v; };

private:

  std::vector<hit> fHit_v;

  ////////////////////////
  ClassDef(event_hit,1)
  ////////////////////////
};

#endif

/** @} */ // end of doxygen group 
