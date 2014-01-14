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

namespace larlight {
  /**
     \class spacepoint
     LArSoft SpacePoint class equivalent data container
  */
  class spacepoint : public data_base {
    
  public:
    
    /// Default constructor
    spacepoint() : data_base() {clear_data();}
    
    spacepoint(Int_t id, 
	       Double_t x,  Double_t y,  Double_t z,
	       Double_t ex, Double_t ey, Double_t ez,
	       Double_t chi2) : data_base()
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
    virtual ~spacepoint(){}
    
    Int_t           ID()     const { return fID;     }
    const Double_t* XYZ()    const { return fXYZ;    }
    const Double_t* ErrXYZ() const { return fErrXYZ; }
    Double_t        Chisq()  const { return fChisq;  }
    
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
    
    Int_t                        fID;        ///< SpacePoint ID
    Double_t                     fXYZ[3];    ///< position of SpacePoint in xyz
    Double_t                     fErrXYZ[6]; ///< Error matrix (triangular).
    Double_t                     fChisq;     ///< Chisquare. 
    
  private:
    
    ////////////////////////
    ClassDef(spacepoint,1)
    ////////////////////////
      
  };
  
  /**
     \class event_sps
     A collection storage class of multiple spacepoints
  */
  class event_sps : public std::vector<larlight::spacepoint>,
		    public data_base {
    
  public:
    
    /// Default constructor
    event_sps() : std::vector<larlight::spacepoint>(), data_base() {clear_data();}
    
    /// Default copy constructor
    event_sps(const event_sps& original) : std::vector<larlight::spacepoint>(original),
					   data_base(original)
    {}
    
    /// Default destructor
    ~event_sps(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){data_base::clear_data(); clear();}
    
    /**
       A utility function to obtain maximum & minimum of spacepoints' vertex along the specified axis.
       NOTE: the initial max/min values matter. It only modifies max/min if it finds values that is 
       above/below those initial values.
    */
    void get_axis_range (Double_t &max, Double_t &min, const int axis) const;
    
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
    ClassDef(event_sps,2)
    ////////////////////////
  };
}
#endif

/** @} */ // end of doxygen group 
