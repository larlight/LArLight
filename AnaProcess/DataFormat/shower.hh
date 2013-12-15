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
    shower() : data_base(), fClusters_v() {clear_data();};
    
    /// Copy constructor
    shower(const shower& original) : data_base(original),
				     fID(original.fID),
				     fDCosStart(original.fDCosStart),
				     fSigmaDCosStart(original.fSigmaDCosStart),
				     fMaxWidthX(original.fMaxWidthX),
				     fMaxWidthY(original.fMaxWidthY),
				     fDistanceMaxWidth(original.fDistanceMaxWidth),
				     fTotalCharge(original.fTotalCharge),
				     fClusters_v(original.fClusters_v)
    {};
    
    /// Default destructor
    virtual ~shower(){}
    
    virtual void clear_data();
    
    // Set methods
    void set_id            (int id)              { fID = id;                }
    void set_total_charge  (double q)            { fTotalCharge = q;        }
    void set_direction     (TVector3 dir)        { fDCosStart = dir;        }
    void set_direction_err (TVector3 dir_e)      { fSigmaDCosStart = dir_e; }
    void set_max_width     (double x, double y) 
    { fMaxWidthX=x; fMaxWidthY=y; }
    void set_distance_max_width (double d)       { fDistanceMaxWidth = d;   }
    void add_cluster       (larlight::cluster c) { fClusters_v.push_back(c);}

    // Get Methods 
    inline double TotalCharge()      const { return fTotalCharge;      }
    inline int    ID()               const { return fID;               }
    inline double MaxWidthX()        const { return fMaxWidthX;           }
    inline double MaxWidthY()        const { return fMaxWidthY;           }
    inline double DistanceMaxWidth() const { return fDistanceMaxWidth; }
    inline const TVector3& Direction()    const { return fDCosStart;          }
    inline const TVector3& DirectionErr() const { return fSigmaDCosStart;     }
    inline const std::vector<larlight::cluster>& Clusters() const { return fClusters_v; }

  protected:

    int      fID;
    TVector3 fDCosStart;             ///< direction cosines at start of shower
    TVector3 fSigmaDCosStart;        ///< uncertainting on initial direction cosines
    double   fMaxWidthX;             ///< maximum width of the prong in the x(0)
    double   fMaxWidthY;             ///< maximum width of the prong in the y(0)
    double   fDistanceMaxWidth;      ///< distance from the start of the prong to its maximum width
    double   fTotalCharge;           ///< total charge of hits in the shower
    std::vector<larlight::cluster> fClusters_v;  ///< associated clusters

  private:
    
    ////////////////////////
    ClassDef(shower,3)
    ////////////////////////
      
  };
  
  /**
     \class event_shower
     A collection storage class of multiple showers.
  */
  class event_shower : public data_base {
    
  public:
    
    /// Default constructor
    event_shower() : data_base(), fShower_v() {clear_data();};
    
    /// Default copy constructor
    event_shower(const event_shower& original) : data_base(original), fShower_v(original.fShower_v)
    {};
    
    /// Default destructor
    ~event_shower(){};
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){data_base::clear_data(); fShower_v.clear();}

    /// Method to add shower
    inline void add_shower(const shower v) { fShower_v.push_back(v); };

    /// Method to get a vector of shower
    inline const std::vector<larlight::shower>& GetShowerCollection() const { return fShower_v; }

  protected:
    
    std::vector<larlight::shower> fShower_v;
    
  private:
    
    ////////////////////////
    ClassDef(event_shower,1)
    ////////////////////////
  };
};
#endif

/** @} */ // end of doxygen group 
