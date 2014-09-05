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

namespace larlight{
  /**
     \class hit
     LArSoft Hit class equivalent data container
  */
  class hit : public data_base {
    
  public:
    
    /// Default constructor
    hit(DATA::DATA_TYPE type=DATA::Hit);
    
    /// Copy constructor
    hit(const hit& original);
    
    /// Default destructor
    virtual ~hit(){}
    
    void clear_data();
    
    // Set methods
    void set_waveform  (const std::vector<Double_t>& wf) { fHitSignal = wf;}
    void set_times     (Double_t start, Double_t peak, Double_t end)
    {fStartTime = start; fPeakTime = peak; fEndTime = end;}
    void set_times_err (Double_t start, Double_t peak, Double_t end)
    {fSigmaStartTime = start; fSigmaPeakTime = peak; fSigmaEndTime = end;}
    void set_charge    (Double_t q, Double_t q_max)
    {fCharge=q; fMaxCharge = q_max;}
    void set_charge_err(Double_t q, Double_t q_max)
    {fSigmaCharge=q; fSigmaMaxCharge=q_max;}
    void set_multiplicity(Int_t m){ fMultiplicity=m;}
    void set_channel (UInt_t ch){ fChannel=ch;}
    void set_wire    (UInt_t wire) { fWire=wire;}
    void set_fit_goodness(Double_t v){ fGoodnessOfFit = v;}
    void set_view(GEO::View_t v){ fView = v;}
    void set_sigtype(GEO::SigType_t t) {fSignalType=t;}
    
    // Get Methods 
    const std::vector<Double_t>& HitSignal()      const { return fHitSignal;      }  
    Double_t          StartTime()                 const { return fStartTime;      }  
    Double_t          EndTime()                   const { return fEndTime;        }
    Double_t          PeakTime()                  const { return fPeakTime;       }
    Double_t          SigmaStartTime()            const { return fSigmaStartTime; }
    Double_t          SigmaEndTime()              const { return fSigmaEndTime;   }
    Double_t          SigmaPeakTime()             const { return fSigmaPeakTime;  }
    Int_t             Multiplicity()              const { return fMultiplicity;   }
    UInt_t          Channel()                   const { return fChannel;        }
    UInt_t          Wire()                      const { return fWire;           }
    Double_t          Charge(bool max=false)      const { return ( max ? fMaxCharge : fCharge);}
    Double_t          SigmaCharge(bool max=false) const { return ( max ? fSigmaMaxCharge : fSigmaCharge ); }
    Double_t          GoodnessOfFit()             const { return fGoodnessOfFit;  }
    GEO::SigType_t  SignalType()                const { return fSignalType;     }
    GEO::View_t     View()                      const { return fView;           }
    
  protected:
    
    std::vector<Double_t>   fHitSignal;      ///< vector of ADC values within the hit window
    Double_t                fStartTime;      ///< initial tdc tick for hit 
    Double_t                fSigmaStartTime; ///< uncertainty on initial tick
    Double_t                fPeakTime;       ///< tdc for the peak charge deposition
    Double_t                fSigmaPeakTime;  ///< uncertainty for tdc of the peak
    Double_t                fEndTime;        ///< final tdc tick for hit
    Double_t                fSigmaEndTime;   ///< uncertainty on final tick
    Double_t                fCharge;         ///< total charge deposited for hit
    Double_t                fSigmaCharge;    ///< uncertainty in total charge deposited
    Double_t                fMaxCharge;      ///< maximum ADC value in hit window
    Double_t                fSigmaMaxCharge; ///< maximum ADC value in hit window
    Int_t                   fMultiplicity;   ///< how many hits could this one be shared with
    Double_t                fGoodnessOfFit;  ///< how well do we believe we know this hit?
    GEO::View_t             fView;           ///< view for the plane of the hit
    GEO::SigType_t          fSignalType;     ///< signal type for the plane of the hit
    UInt_t                  fChannel;        ///< channel number
    UInt_t                  fWire;           ///< wire number
  private:
    
    ////////////////////////
    ClassDef(hit,3)
    ////////////////////////
      
  };
  
  /**
     \class event_hit
     A collection storage class of multiple hits.
  */
  class event_hit : public std::vector<larlight::hit>, 
		    public event_base {
    
  public:
    
    /// Default constructor
    event_hit(DATA::DATA_TYPE type=DATA::Hit);
    
    /// Default copy constructor
    event_hit(const event_hit& original) : std::vector<larlight::hit>(original), event_base(original)
    {}
    
    /// Default destructor
    ~event_hit(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}

    /**
       A utility function to obtain maximum & minimum of hits' vertex
       NOTE: the initial max/min values matter. It only modifies max/min if it finds values that is 
       above/below those initial values.
    */
    void get_axis_range (std::vector<Double_t> &chmax,   std::vector<Double_t> &chmin,
			 std::vector<Double_t> &wiremax, std::vector<Double_t> &wiremin,
			 std::vector<Double_t> &timemax, std::vector<Double_t> &timemin) const;

    /**
       Yet another utility function with an optional input to specify index of hits to be considered.
     */
    void get_axis_range (std::vector<Double_t> &chmax,   std::vector<Double_t> &chmin,
			 std::vector<Double_t> &wiremax, std::vector<Double_t> &wiremin,
			 std::vector<Double_t> &timemax, std::vector<Double_t> &timemin,
			 const std::vector<unsigned short>& hit_index) const;
    
  private:
    
    ////////////////////////
    ClassDef(event_hit,4)
    ////////////////////////
  };
}
#endif

/** @} */ // end of doxygen group 
