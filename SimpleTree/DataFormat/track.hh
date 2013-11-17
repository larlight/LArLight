/**
 * \file track.hh
 *
 * \ingroup SimpleTreeData
 * 
 * @author Kazu - Nevis 2013
 */

/** \addtogroup SimpleTreeData

    @{*/

#ifndef TRACK_HH
#define TRACK_HH

#include "data_base.hh"

/**
   \class track
   Implementation of data_base for LArSoft Track data class. It holds two kinds of C-arrays: one is a track summary information created per track, and another C-array to hold the full step-wise trajectory points for each track. 
*/
class track : public data_base {

public:

  /// Default constructor
  track(DATA::DATA_TYPE type=DATA::Track);

  /// Default destructor
  virtual ~track(){};

  /**
     Implementation data_base::clear_event. 
     When the argument boolean "all" is set to true, clear ALL C-array data elements to the default value.
     When "all" is set to false (default), only filled entries are cleared.
     This way we save time to loop over array elements.
  */
  virtual void clear_event(bool all=false);

  /// Implementation of track data address setter
  virtual void set_address(TTree* t);

  /// Setter method for a track information
  void add_track(UShort_t trackID, 
		 Float_t  startx, Float_t  starty,   Float_t  startz,   Float_t  startd,
		 Float_t  endx,   Float_t  endy,     Float_t  endz,     Float_t  endd,
		 Float_t  theta,  Float_t  theta_xz, Float_t  theta_yz, 
		 Double_t mom,    Double_t len);

  //--- Getters for track-wise information ---//
  UShort_t        no_tracks() const { return _no_tracks; };
  const UShort_t* trackID()   const { return _trackID;   };
  const Float_t*  startx()    const { return _startx;    };
  const Float_t*  starty()    const { return _starty;    };
  const Float_t*  startz()    const { return _startz;    };
  const Float_t*  startd()    const { return _startd;    };
  const Float_t*  endx()      const { return _endx;      };
  const Float_t*  endy()      const { return _endy;      };
  const Float_t*  endz()      const { return _endz;      };
  const Float_t*  endd()      const { return _endd;      };
  const Float_t*  theta()     const { return _theta;     };
  const Float_t*  theta_xz()  const { return _theta_xz;  };
  const Float_t*  theta_yz()  const { return _theta_yz;  };
  const Double_t* mom()       const { return _mom;       };
  const Double_t* len()       const { return _len;       };

  /// Setter method for a trajectory point information
  void add_trajectory(UShort_t vtxID, 
		      Float_t  x,  Float_t  y,  Float_t  z,
		      Double_t px, Double_t py, Double_t pz);

  //--- Getter methods for trajectory points ---//
  Int_t no_points()       const { return _no_points; };
  const UShort_t* vtxID() const { return _vtxID;     };
  const Float_t*  vtxx()  const { return _vtxx;      };
  const Float_t*  vtxy()  const { return _vtxy;      };
  const Float_t*  vtxz()  const { return _vtxz;      };
  const Double_t* momx()  const { return _momx;      };
  const Double_t* momy()  const { return _momy;      };
  const Double_t* momz()  const { return _momz;      };

protected:

  // Tracks
  UShort_t _no_tracks;                 ///< Number of tracks
  UShort_t _trackID[DATA::kMaxTracks]; ///< Track ID
  Float_t _startx[DATA::kMaxTracks];   ///< Start position X
  Float_t _starty[DATA::kMaxTracks];   ///< Start position Y
  Float_t _startz[DATA::kMaxTracks];   ///< Start position Z
  Float_t _startd[DATA::kMaxTracks];   ///< Start distance to boundary
  Float_t _endx[DATA::kMaxTracks];     ///< End position X
  Float_t _endy[DATA::kMaxTracks];     ///< End position Y   
  Float_t _endz[DATA::kMaxTracks];     ///< End position Z
  Float_t _endd[DATA::kMaxTracks];     ///< End distance to boundary
  Float_t _theta[DATA::kMaxTracks];    ///< Theta
  Float_t _phi[DATA::kMaxTracks];      ///< Phi
  Float_t _theta_xz[DATA::kMaxTracks]; ///< Theta on XZ plane
  Float_t _theta_yz[DATA::kMaxTracks]; ///< Theta on YZ plane
  Double_t _mom[DATA::kMaxTracks];     ///< Total momentum
  Double_t _len[DATA::kMaxTracks];     ///< Track length 

  // Trajectory points
  UShort_t _no_points;                    ///< Number of trajectory points (for all tracks)
  UShort_t _vtxID[DATA::kMaxTrackPoints]; ///< Array of associated track ID
  Float_t  _vtxx[DATA::kMaxTrackPoints];  ///< X coordinate of trajectory points
  Float_t  _vtxy[DATA::kMaxTrackPoints];  ///< Y coordinate of trajectory points
  Float_t  _vtxz[DATA::kMaxTrackPoints];  ///< Z coordinate of trajectory points
  Double_t _momx[DATA::kMaxTrackPoints];  ///< X component of momentum at this point
  Double_t _momy[DATA::kMaxTrackPoints];  ///< Y component of momentum at this point
  Double_t _momz[DATA::kMaxTrackPoints];  ///< Z component of momentum at this point

};

#endif

/** @} */ // end of doxygen group 
