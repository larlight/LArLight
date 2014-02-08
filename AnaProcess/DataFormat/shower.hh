/**
 * \file shower.hh
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for shower data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef SHOWER_HH
#define SHOWER_HH

#include "data_base.hh"
#include "cluster.hh"
#include <TVector3.h>
namespace larlight{
  /**
     \class shower
     LArSoft Shower class equivalent data container
  */
  class shower : public data_base {
    
  public:
    
    /// Default constructor
    shower(DATA::DATA_TYPE type=DATA::Shower);
    
    /// Copy constructor
    shower(const shower& original) : data_base(original),
				     fID(original.fID),
				     fDCosStart(original.fDCosStart),
				     fSigmaDCosStart(original.fSigmaDCosStart),
				     fMaxWidthX(original.fMaxWidthX),
				     fMaxWidthY(original.fMaxWidthY),
				     fDistanceMaxWidth(original.fDistanceMaxWidth),
				     fTotalCharge(original.fTotalCharge)
    {}
    
    /// Default destructor
    virtual ~shower(){}
    
    virtual void clear_data();
    
    // Set methods
    void set_id            (Int_t id)              { fID = id;                }
    void set_total_charge  (Double_t q)            { fTotalCharge = q;        }
    void set_direction     (TVector3 dir)        { fDCosStart = dir;        }
    void set_direction_err (TVector3 dir_e)      { fSigmaDCosStart = dir_e; }
    void set_max_width     (Double_t x, Double_t y) 
    { fMaxWidthX=x; fMaxWidthY=y; }
    void set_distance_max_width (Double_t d)       { fDistanceMaxWidth = d;   }

    // Get Methods 
    inline Double_t TotalCharge()      const { return fTotalCharge;      }
    inline Int_t    ID()               const { return fID;               }
    inline Double_t MaxWidthX()        const { return fMaxWidthX;           }
    inline Double_t MaxWidthY()        const { return fMaxWidthY;           }
    inline Double_t DistanceMaxWidth() const { return fDistanceMaxWidth; }
    inline const TVector3& Direction()    const { return fDCosStart;          }
    inline const TVector3& DirectionErr() const { return fSigmaDCosStart;     }

  protected:

    Int_t    fID;
    TVector3 fDCosStart;             ///< direction cosines at start of shower
    TVector3 fSigmaDCosStart;        ///< uncertainting on initial direction cosines
    Double_t fMaxWidthX;             ///< maximum width of the prong in the x(0)
    Double_t fMaxWidthY;             ///< maximum width of the prong in the y(0)
    Double_t fDistanceMaxWidth;      ///< distance from the start of the prong to its maximum width
    Double_t fTotalCharge;           ///< total charge of hits in the shower

  private:
    
    ////////////////////////
    ClassDef(shower,3)
    ////////////////////////
      
  };
  
  /**
     \class event_shower
     A collection storage class of multiple showers.
  */
  class event_shower : public std::vector<larlight::shower>, public data_base {
    
  public:
    
    /// Default constructor
    event_shower(DATA::DATA_TYPE type=DATA::Shower);
    
    /// Default copy constructor
    event_shower(const event_shower& original) : std::vector<larlight::shower>(original), data_base(original)
    {}
    
    /// Default destructor
    ~event_shower(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){data_base::clear_data(); clear();}

  protected:
    
  private:
    
    ////////////////////////
    ClassDef(event_shower,2)
    ////////////////////////
  };
}
#endif

/** @} */ // end of doxygen group 
