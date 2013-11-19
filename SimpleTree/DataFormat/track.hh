#ifndef TRACK_HH
#define TRACK_HH
/**
 * \file track.hh
 *
 * \ingroup SimpleTreeData
 * 
 * @author Kazu - Nevis 2013
 */

/** \addtogroup SimpleTreeData

    @{*/

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
  virtual Bool_t set_address(TTree* t,Bool_t create=false);

  /// Setter method for a track information
  void add_track(UShort_t trackID, 
		 Float_t  startx, Float_t  starty,   Float_t  startz,   Float_t  startd,
		 Float_t  endx,   Float_t  endy,     Float_t  endz,     Float_t  endd,
		 Float_t  theta, Double_t mom,    Double_t len);

  //--- Getters for track-wise information ---//
  UShort_t        num_tracks() const { return _num_tracks; };
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
  const Double_t* startmom()  const { return _startmom;  };
  const Double_t* len()       const { return _len;       };

  /// Setter method for a trajectory point information
  void add_trajectory(UShort_t vtxID, 
		      Float_t  x,  Float_t  y,  Float_t  z,
		      Double_t momx,  Double_t momy,  Double_t momz);

  //--- Getter methods for trajectory points ---//
  Int_t num_points()       const { return _num_points; };
  const UShort_t*  vtxID() const { return _vtxID;      };
  const Float_t*   vtxx()  const { return _vtxx;       };
  const Float_t*   vtxy()  const { return _vtxy;       };
  const Float_t*   vtxz()  const { return _vtxz;       };
  const Double_t*  momx()  const { return _momx;       };
  const Double_t*  momy()  const { return _momy;       };
  const Double_t*  momz()  const { return _momz;       };

protected:

  // Tracks
  UShort_t _num_tracks;                 ///< Number of tracks
  UShort_t _trackID[DATA::kMaxTracks];  ///< Track ID
  Float_t  _startx[DATA::kMaxTracks];   ///< Start position X
  Float_t  _starty[DATA::kMaxTracks];   ///< Start position Y
  Float_t  _startz[DATA::kMaxTracks];   ///< Start position Z
  Float_t  _startd[DATA::kMaxTracks];   ///< Start distance to boundary
  Float_t  _endx[DATA::kMaxTracks];     ///< End position X
  Float_t  _endy[DATA::kMaxTracks];     ///< End position Y   
  Float_t  _endz[DATA::kMaxTracks];     ///< End position Z
  Float_t  _endd[DATA::kMaxTracks];     ///< End distance to boundary
  Float_t  _theta[DATA::kMaxTracks];    ///< Theta
  Float_t  _phi[DATA::kMaxTracks];      ///< Phi
  Double_t _startmom[DATA::kMaxTracks]; ///< Total momentum
  Double_t _len[DATA::kMaxTracks];      ///< Track length 

  // Trajectory points
  UShort_t  _num_points;                   ///< Number of trajectory points (for all tracks)
  UShort_t  _vtxID[DATA::kMaxTrackPoints]; ///< Array of associated track ID
  Float_t   _vtxx[DATA::kMaxTrackPoints];  ///< X coordinate of trajectory points
  Float_t   _vtxy[DATA::kMaxTrackPoints];  ///< Y coordinate of trajectory points
  Float_t   _vtxz[DATA::kMaxTrackPoints];  ///< Z coordinate of trajectory points
  Double_t  _momx[DATA::kMaxTrackPoints];  ///< Momentum X component at a trajectory point
  Double_t  _momy[DATA::kMaxTrackPoints];  ///< Momentum Y component at a trajectory point
  Double_t  _momz[DATA::kMaxTrackPoints];  ///< Momentum Z component at a trajectory point

};

#endif

/** @} */ // end of doxygen group 
