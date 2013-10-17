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

/**
   \class track
   LArSoft Track class equivalent data container
*/
class track : public data_base {

public:
  
  /// Default constructor
  track();

  /// Copy constructor
  track(const track& original);

  /// Default destructor
  virtual ~track(){};
  
  inline int             track_id      ()               const { return fID;                  }
  inline const size_t    n_point       ()               const { return fDir.size();          }
  inline const size_t    n_covariance  ()               const { return fCov.size();          }
  inline const size_t    n_momentum    ()               const { return fFitMomentum.size();  }
  inline const size_t    n_dQdx        ()               const { return fdQdx.size();         }

  inline const TVector3& direction_at  (unsigned int p) const { return fDir.at(p);           }
  inline const TVector3& vertex_at     (unsigned int p) const { return fXYZ.at(p);           }
  inline const double    momentum_at   (unsigned int p) const { return fFitMomentum.at(p);   }
  inline const TMatrixD& covariance_at (unsigned int p) const { return fCov.at(p);           }

  inline void set_track_id   (const int id)     { fID = id;                  };
  inline void add_vertex     (const TVector3 v) { fXYZ.push_back(v);         };
  inline void add_direction  (const TVector3 v) { fDir.push_back(v);         };
  inline void add_momentum   (const double   v) { fFitMomentum.push_back(v); };
  inline void add_covariance (const TMatrixD m) { fCov.push_back(m);         };

  virtual void clear_data();

protected:

  /// track's ID
  int fID;            

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
  std::vector< std::vector <double> > fdQdx;
  
  /**
     momentum at start and end of track
     determined from a fit. Intermediate points
     can be added if desired
  */
  std::vector<double> fFitMomentum;

private:
  

  ////////////////////////
  ClassDef(track,1)
  ////////////////////////

};

/**
 \class event_track 
 A collection storage class of multiple tracks.
*/
class event_track : public data_base {
  
public:

  /// Default constructor
  event_track() : data_base() {clear_data();};
  
  /// Default copy constructor
  event_track(const event_track& original) : data_base(original),
					     fTrack_v(original.fTrack_v)
  {};

  /// Default destructor
  ~event_track(){};
  
  /// Method to clear currently held data contents in the buffer
  virtual void clear_data(){data_base::clear_data(); fTrack_v.clear();};

  /// Method to add a track
  inline void  add_track(const track t)                 { fTrack_v.push_back(t);  };

  /// Method to get an array of tracks
  inline const std::vector<track>& GetTrackCollection() { return fTrack_v;        };

private:

  std::vector<track> fTrack_v;

  ////////////////////////
  ClassDef(event_track,1)
  ////////////////////////
};

#endif

/** @} */ // end of doxygen group 
