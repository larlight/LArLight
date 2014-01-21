/**
 * \file track.hh
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for track data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef TRACK_HH
#define TRACK_HH

#include "data_base.hh"
#include "TVector3.h"
#include "TMatrixD.h"

namespace larlight {
  /**
     \class track
     LArSoft Track class equivalent data container
  */
  class track : public data_base {
    
  public:
    
    /// Default constructor
    track(DATA::DATA_TYPE type=DATA::Track);
    
    /// Copy constructor
    track(const track& original);
    
    /// Default destructor
    virtual ~track(){}
    
    inline Int_t     track_id      ()               const { return fID;                  }
    inline size_t    n_point       ()               const { return fDir.size();          }
    inline size_t    n_covariance  ()               const { return fCov.size();          }
    inline size_t    n_momentum    ()               const { return fFitMomentum.size();  }
    inline size_t    n_dQdx        ()               const { return fdQdx.size();         }
    
    inline const TVector3& direction_at  (UInt_t p) const { return fDir.at(p);           }
    inline const TVector3& vertex_at     (UInt_t p) const { return fXYZ.at(p);           }
    inline       Double_t    momentum_at (UInt_t p) const { return fFitMomentum.at(p);   }
    inline const TMatrixD& covariance_at (UInt_t p) const { return fCov.at(p);           }
    
    inline void set_track_id   (const Int_t id)     { fID = id;                  }
    inline void add_vertex     (const TVector3 v) { fXYZ.push_back(v);         }
    inline void add_direction  (const TVector3 v) { fDir.push_back(v);         }
    inline void add_momentum   (const Double_t   v) { fFitMomentum.push_back(v); }
    inline void add_covariance (const TMatrixD m) { fCov.push_back(m);         }
    
    virtual void clear_data();
    
    /**
       A utility function to obtain maximum & minimum of spacepoints' vertex along the specified axis.
       NOTE: the initial max/min values matter. It only modifies max/min if it finds values that is 
       above/below those initial values.
    */
    void get_axis_range (Double_t &max, Double_t &min, const Int_t axis) const;
    
    /**
       A utility function to obtain maximum & minimum of spacepoints' vertex along all axis.
       NOTE: the initial max/min values matter. It only modifies max/min if it finds values that is
       above/below those initial values.
    */
    void get_axis_range (Double_t &xmax, Double_t &xmin,
			 Double_t &ymax, Double_t &ymin,
			 Double_t &zmax, Double_t &zmin) const;  
    
    
  protected:
    
    /// track's ID
    Int_t fID;            
    
    /// position of points along the track
    std::vector<TVector3> fXYZ;
    
    /// direction at each point along the track
    std::vector<TVector3> fDir;
    
    /** 
	covariance matrix of positions
	possibly only end points are stored
    */
    std::vector<TMatrixD> fCov;
    
    /**
       charge deposition per unit length at points
       along track outer vector index is over view
    */
    std::vector< std::vector <Double_t> > fdQdx;
    
    /**
       momentum at start and end of track
       determined from a fit. Intermediate points
       can be added if desired
    */
    std::vector<Double_t> fFitMomentum;
    
  private:
    
    
    ////////////////////////
    ClassDef(track,2)
    ////////////////////////
      
  };

  /**
     \class event_track 
     A collection storage class of multiple tracks.
  */
  class event_track : public std::vector<larlight::track>, public data_base {
    
  public:
    
    /// Default constructor
    event_track(DATA::DATA_TYPE type=DATA::Track);
    
    /// Default copy constructor
    event_track(const event_track& original) : std::vector<larlight::track>(original),
					       data_base(original)
    {}
    
    /// Default destructor
    ~event_track(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){data_base::clear_data(); clear();}
    
    /**
       A utility function to obtain maximum & minimum of spacepoints' vertex along the specified axis.
       NOTE: the initial max/min values matter. It only modifies max/min if it finds values that is 
       above/below those initial values.
    */
    void get_axis_range (Double_t &max, Double_t &min, const Int_t axis) const;
    
    /**
       A utility function to obtain maximum & minimum of spacepoints' vertex along all axis.
       NOTE: the initial max/min values matter. It only modifies max/min if it finds values that is
       above/below those initial values.
    */
    void get_axis_range (Double_t &xmax, Double_t &xmin,
			 Double_t &ymax, Double_t &ymin,
			 Double_t &zmax, Double_t &zmin) const;  
    
  private:
    
    ////////////////////////
    ClassDef(event_track,2)
    ////////////////////////
  };
}
#endif

/** @} */ // end of doxygen group 
