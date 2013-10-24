/**
 * \file spacepoint.hh
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for spacepoint data container
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
   \class spacepoint
   LArSoft SpacePoint class equivalent data container
*/
class spacepoint : public data_base {

public:

  /// Default constructor
  spacepoint() : data_base() {clear_data();};

  spacepoint(int id, 
	     double x,  double y,  double z,
	     double ex, double ey, double ez,
	     double chi2) : data_base()
  {
    clear_data();
    fID=id;
    fXYZ[0]=x;
    fXYZ[1]=y;
    fXYZ[2]=z;
    fErrXYZ[0]=ex;
    fErrXYZ[1]=ey;
    fErrXYZ[2]=ez;
    fChisq=chi2;
  }
  
  /// Copy constructor
  spacepoint(const spacepoint& original) : data_base(original),
					   fID(original.fID),
					   fChisq(original.fChisq)
  {
    fXYZ[0]=original.fXYZ[0];
    fXYZ[1]=original.fXYZ[1];
    fXYZ[2]=original.fXYZ[2];
    fErrXYZ[0]=original.fErrXYZ[0];
    fErrXYZ[1]=original.fErrXYZ[1];
    fErrXYZ[2]=original.fErrXYZ[2];
  }

  /// Default destructor
  virtual ~spacepoint(){};

  int           ID()     const { return fID;     };
  const double* XYZ()    const { return fXYZ;    };
  const double* ErrXYZ() const { return fErrXYZ; };
  double        Chisq()  const { return fChisq;  };

  /// Method to clear data member
  void clear_data() {
    data_base::clear_data();
    fID = -1;
    fXYZ[0]=-99999;
    fXYZ[1]=-99999;
    fXYZ[2]=-99999;
    fErrXYZ[0]=-99999;
    fErrXYZ[1]=-99999;
    fErrXYZ[2]=-99999;
    fChisq=-1;
  }

protected:

  int                        fID;        ///< SpacePoint ID
  double                     fXYZ[3];    ///< position of SpacePoint in xyz
  double                     fErrXYZ[6]; ///< Error matrix (triangular).
  double                     fChisq;     ///< Chisquare. 

private:

  ////////////////////////
  ClassDef(spacepoint,1)
  ////////////////////////

};

/**
 \class event_sps
 A collection storage class of multiple spacepoints
*/
class event_sps : public data_base {
  
public:

  /// Default constructor
  event_sps() : data_base() {clear_data();};
  
  /// Default copy constructor
  event_sps(const event_sps& original) : data_base(original),
					 fSpacePoint_v(original.fSpacePoint_v)
  {};

  /// Default destructor
  ~event_sps(){};
  
  /// Method to clear currently held data contents in the buffer
  virtual void clear_data(){data_base::clear_data(); fSpacePoint_v.clear();};
  
  /// Method to add a spacepoint
  inline void  add_sps(const spacepoint pt)                { fSpacePoint_v.push_back(pt); };

  /// Method to get an array of spacepoints
  inline const std::vector<spacepoint>& GetSPSCollection() { return fSpacePoint_v;        };

  /**
     A utility function to obtain maximum & minimum of spacepoints' vertex along the specified axis.
     NOTE: the initial max/min values matter. It only modifies max/min if it finds values that is 
     above/below those initial values.
  */
  void get_axis_range (double &max, double &min, const int axis) const;

  /**
     A utility function to obtain maximum & minimum of spacepoints' vertex along all axis.
     NOTE: the initial max/min values matter. It only modifies max/min if it finds values that is
     above/below those initial values.
  */
  void get_axis_range (double &xmax, double &xmin,
		       double &ymax, double &ymin,
		       double &zmax, double &zmin) const;

private:

  std::vector<spacepoint> fSpacePoint_v;

  ////////////////////////
  ClassDef(event_sps,1)
  ////////////////////////
};

#endif

/** @} */ // end of doxygen group 
