/**
 * \file spacepoint.hh
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for sps data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef SPACEPOINT_HH
#define SPACEPOINT_HH

#include "data_base.hh"
#include "TVector3.h"
#include "TMatrixD.h"

/**
   \class sps
   LArSoft Sps class equivalent data container
*/
class sps : public data_base {

public:

  /// Maximum number of space points to be stored
  static const size_t kMaxSPS = 1000;

public:

  /// Default constructor
  sps(DATA::DATA_TYPE type=DATA::SpacePoint);

  /// Default destructor
  virtual ~sps(){};

  /// Implementation of event-wise track data clear method
  virtual void clear_event();

  /// Implementation of track data address setter
  virtual void set_address(TTree* t);

  //--- Member getters ---//
  int           nsps()       const { return _nsps;       };
  const int*    spsId()      const { return _spsId;      };
  const double* spsx()       const { return _spsx;       };
  const double* spsy()       const { return _spsy;       };
  const double* spsz()       const { return _spsz;       };
  const double* spsx_sigma() const { return _spsx_sigma; };
  const double* spsy_sigma() const { return _spsy_sigma; };
  const double* spsz_sigma() const { return _spsz_sigma; };
  const double* chi2()       const { return _chi2;       };

  //--- Member setters ---//
  void nsps(int n)                 { _nsps=n;          };
  void spsId(int n, double v)      { _spsId[n]=v;      };
  void spsx(int n, double v)       { _spsx[n]=v;       };
  void spsy(int n, double v)       { _spsy[n]=v;       };
  void spsz(int n, double v)       { _spsz[n]=v;       };
  void spsx_sigma(int n, double v) { _spsx_sigma[n]=v; };
  void spsy_sigma(int n, double v) { _spsy_sigma[n]=v; };
  void spsz_sigma(int n, double v) { _spsz_sigma[n]=v; };
  void chi2(int n, double v)       { _chi2[n]=v;       };

protected:

  int    _nsps;                 ///< Number of space-points (SPS)
  int    _spsId[kMaxSPS];       ///< SPS ID
  double _spsx[kMaxSPS];        ///< SPS x coordinates
  double _spsy[kMaxSPS];        ///< SPS y coordinates
  double _spsz[kMaxSPS];        ///< SPS z coordinates
  double _spsx_sigma[kMaxSPS];  ///< SPS x coordinate errors
  double _spsy_sigma[kMaxSPS];  ///< SPS y coordinate errors
  double _spsz_sigma[kMaxSPS];  ///< SPS z coordinate errors
  double _chi2[kMaxSPS];        ///< SPS Chi2

};

#endif

/** @} */ // end of doxygen group 
