/**
 * \file track.hh
 *
 * \ingroup SimpleTreeData
 * 
 * \brief Class def header for track data container
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
   Implementation of data_base for track data
*/
class track : public data_base {

public:
 
  /// Maximum track this class can hold
  static const size_t kMaxTrack = 1000;

public:
  
  /// Default constructor
  track(DATA::DATA_TYPE type=DATA::DATA_TYPE_MAX);

  /// Implementation of event-wise track data clear method
  virtual void clear_event();

  /// Implementation of track data address setter
  virtual void set_address(TTree* t);

  //--- Member getters ---//
  int           ntracks()  const { return _ntracks;  };
  const int*    trackId()  const { return _trackId;  };
  const double* startx()   const { return _startx;   };
  const double* starty()   const { return _starty;   };
  const double* startz()   const { return _startz;   };
  const double* startd()   const { return _startd;   };
  const double* endx()     const { return _endx;     };
  const double* endy()     const { return _endy;     };
  const double* endz()     const { return _endz;     };
  const double* endd()     const { return _endd;     };
  const double* theta()    const { return _theta;    };
  const double* theta_xz() const { return _theta_xz; };
  const double* theta_yz() const { return _theta_yz; };
  const double* mom()      const { return _mom;      };
  const double* len()      const { return _len;      };

  //--- Member setters ---//
  void ntracks(int n)            { _ntracks=n;     };
  void trackId(int i, int id)    { _trackId[i]=id; };
  void startx(int i, double v)   { _startx[i]=v;   };
  void starty(int i, double v)   { _starty[i]=v;   };
  void startz(int i, double v)   { _startz[i]=v;   };
  void startd(int i, double v)   { _startd[i]=v;   };
  void endx(int i, double v)     { _endx[i]=v;     };
  void endy(int i, double v)     { _endy[i]=v;     };
  void endz(int i, double v)     { _endz[i]=v;     };
  void endd(int i, double v)     { _endd[i]=v;     };
  void theta(int i, double v)    { _theta[i]=v;    };
  void theta_xz(int i, double v) { _theta_xz[i]=v; };
  void theta_yz(int i, double v) { _theta_yz[i]=v; };
  void mom(int i, double v)      { _mom[i]=v;      };
  void len(int i, double v)      { _len[i]=v;      };

protected:

  int    _ntracks;              ///< Number of tracks
  int    _trackId[kMaxTrack];   ///< Track ID
  double _startx[kMaxTrack];    ///< Start position X
  double _starty[kMaxTrack];    ///< Start position Y
  double _startz[kMaxTrack];    ///< Start position Z
  double _startd[kMaxTrack];    ///< Start distance to boundary
  double _endx[kMaxTrack];      ///< End position X
  double _endy[kMaxTrack];      ///< End position Y   
  double _endz[kMaxTrack];      ///< End position Z
  double _endd[kMaxTrack];      ///< End distance to boundary
  double _theta[kMaxTrack];     ///< Theta
  double _phi[kMaxTrack];       ///< Phi
  double _theta_xz[kMaxTrack];  ///< Theta on XZ plane
  double _theta_yz[kMaxTrack];  ///< Theta on YZ plane
  double _mom[kMaxTrack];       ///< Total momentum
  double _len[kMaxTrack];       ///< Track length 

};

#endif

/** @} */ // end of doxygen group 
