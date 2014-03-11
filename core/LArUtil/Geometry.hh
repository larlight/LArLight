/**
 * \file Geometry.hh
 *
 * \ingroup LArUtil
 * 
 * \brief Class def header for a class Geometry
 *
 * @author kazuhiro
 */

/** \addtogroup LArUtil

    @{*/
#ifndef GEOMETRY_HH
#define GEOMETRY_HH

#include "LArUtilBase.hh"
#include <TMath.h>
#include <TVector3.h>
#include <set>

namespace larutil {
  /**
     \class Geometry
  */
  class Geometry : public LArUtilBase{
    
  private:

    /// Singleton constructor
    Geometry(std::string file_name="",
	     std::string tree_name="");

    static Geometry* _me;
    
  public:

    /// Singleton getter
    static const Geometry* GetME(std::string file_name="", std::string tree_name="") 
    {
      if(!_me) _me = new Geometry(file_name,tree_name);

      return _me;
    }
    
    /// Default destructor
    virtual ~Geometry(){};

    virtual void SetBranchAddress();    

    //--- LArSoft Implementation ---//

    /// Number of readout channels in the detector 
    UInt_t     Nchannels() const
    { return fChannelToWireMap->size(); }

    /// Number of OpChannels in the detector
    UInt_t NOpChannels() const
    { return fOpChannelVtx->size(); }

    /// Number of views (different wire orientations) in the detector
    UInt_t Nviews() const
    { return fWirePitch->size(); }

    /// Number of wire planes in TPC "tpc" of cryostat "cstat".
    UInt_t Nplanes() const
    { return fPlanePitch->size(); }

    /// Number of wires in plane "p" of TPC "tpc" of cryostat "cstat".
    UInt_t Nwires(UInt_t p) const;
    
    /// convert channel number to list of possible WireIDs
    //larlight::GEO::WireID ChannelToWire(UInt_t const channel) const; 

    /// return the signal type for a given channel
    larlight::GEO::SigType_t SignalType(const UInt_t channel)  const; 

    /// return the view type for a given channel
    larlight::GEO::View_t View(const UInt_t channel)  const; 

    /// return vector of possible views in the detector
    std::set<larlight::GEO::View_t>  const Views() const; 

    /// convert plane, wire to channel 
    UInt_t   PlaneWireToChannel(const UInt_t plane,
				const UInt_t wire) const;
    
    /// convert channel to plane
    UChar_t  ChannelToPlane(const UInt_t ch) const;

    /// Convert channel to wire
    UInt_t   ChannelToWire(const UInt_t ch)const;
    
    /// find the nearest channel to input world coordinates
    UInt_t   NearestChannel(const Double_t worldLoc[3],
			    const UInt_t   PlaneNo) const;

    /// find the nearest channel to input world coordinates 
    UInt_t   NearestChannel(const std::vector<Double_t> worldLoc,
			    const UInt_t PlaneNo) const;

    /// find the nearest channel to input world coordinates
    UInt_t   NearestChannel(const TVector3& worldLoc,
			    const UInt_t    PlaneNo) const;

    /// nearest wire to input world coordinates
    UInt_t NearestWire(const Double_t worldLoc[3],
		       const UInt_t   PlaneNo) const;

    /// nearest wire to input world coordinate
    UInt_t NearestWire(std::vector<Double_t> worldLoc,
		       const UInt_t  PlaneNo) const;

    /// nearest wire to input world coordinates
    UInt_t NearestWire(const TVector3& worldLoc,
		       const UInt_t PlaneNo) const;

    /// half width of the TPC
    Double_t   DetHalfWidth() const
    { return fDetHalfWidth; }

    /// half height of the TPC 
    Double_t   DetHalfHeight() const
    { return fDetHalfHeight; }

    /// length of the TPC 
    Double_t   DetLength() const
    { return fDetLength; }

    /// half width of the cryostat
    Double_t   CryostatHalfWidth() const
    { return fCryoHalfWidth; }

    /// half height of the cryostat
    Double_t   CryostatHalfHeight() const
    { return fCryoHalfHeight; }

    /// length of the cryostat 
    Double_t   CryostatLength() const
    { return fCryoLength; }

    /// boundaries of cryostat, 3 pairs of +/- coord
    //void     CryostatBoundaries(Double_t* boundaries) const;

  private:

    Double_t fDetLength;
    Double_t fDetHalfWidth;
    Double_t fDetHalfHeight;

    Double_t fCryoLength;
    Double_t fCryoHalfWidth;
    Double_t fCryoHalfHeight;

  public:

    /// distance between planes p1 < p2
    Double_t   PlanePitch(const UChar_t p1 = 0,const UChar_t p2 = 1) const;

    /// distance between wires on the same plane w1 < w2
    Double_t   WirePitch(const UInt_t  w1 = 0,                   
			 const UInt_t  w2 = 1, 
			 const UChar_t plane = 0) const;

    /// assumes all planes in a view have the same pitch
    Double_t   WirePitch(const larlight::GEO::View_t view) const
    { return fWirePitch->at(view); }

    /// assumes all wires in the view have the same angle
    Double_t   WireAngleToVertical(larlight::GEO::View_t view) const
    { return fWireAngle->at(view); }

    /// volume box
    void  WorldBox(Double_t* xlo,
		   Double_t* xhi,
		   Double_t* ylo,
		   Double_t* yhi,
		   Double_t* zlo,
		   Double_t* zhi)  const; 

    /**
       The following functions are utilized to determine if two wires
       in the TPC intersect or not, and if they do then
       determine the coordinates of the intersection.
       Starting point of wire is end with lower z-coordinate.
    */
    bool ValueInRange(const Double_t value, Double_t min, Double_t max) const
    {  if(min>max) std::swap(min,max); return ( (min <= value) && (value <= max) ); }

    void WireEndPoints(const UChar_t plane,
                       const UInt_t wire,
                       Double_t *xyzStart,
                       Double_t *xyzEnd) const;

    bool ChannelsIntersect(const UInt_t c1, const UInt_t c2, Double_t &y, Double_t &z) const;
    
    void IntersectionPoint(const UInt_t  wire1,  const UInt_t  wire2,
			   const UChar_t plane1, const UChar_t plane2,
			   Double_t start_w1[3], Double_t end_w1[3],
			   Double_t start_w2[3], Double_t end_w2[3],
			   Double_t &y, Double_t &z) const;

    void IntersectionPoint(const UInt_t  wire1,  const UInt_t  wire2,
			   const UChar_t plane1, const UChar_t plane2,
			   Double_t &y, Double_t &z) const;

    
    /// Return gdml string which gives sensitive opdet name
    std::string OpDetGeoName(UInt_t c=0) const;

    /// Find the nearest OpChannel to some point, in the appropriate cryostat 
    UInt_t  GetClosestOpChannel(const Double_t * xyz) const;

    /// Find the nearest OpChannel to some point, in the appropriate cryostat 
    UInt_t  GetClosestOpChannel(const Double_t * xyz, Double_t &dist) const;

    /// Return optical channel vertex
    void GetOpChannelPosition(const UInt_t i, Double_t *xyz) const;

  private:
    
    std::vector<UChar_t>                *fChannelToPlaneMap;
    std::vector<UShort_t>               *fChannelToWireMap;
    std::vector<std::vector<UShort_t> > *fPlaneWireToChannelMap;

    // Vectors with length = # planes
    std::vector<larlight::GEO::SigType_t> *fSignalType;
    std::vector<larlight::GEO::View_t>    *fViewType;
    std::vector<Double_t> *fPlanePitch;
    std::vector<std::vector<Double_t> > *fFirstWireStartVtx;
    std::vector<std::vector<Double_t> > *fFirstWireEndVtx;

    // Vectors with length = view
    std::vector<Double_t> *fWirePitch;
    std::vector<Double_t> *fWireAngle;

    std::vector<std::vector<Float_t> > *fOpChannelVtx;

  };
}

#endif
/** @} */ // end of doxygen group 

