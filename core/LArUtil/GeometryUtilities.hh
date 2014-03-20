////////////////////////////////////////////////////////////////////////
// \file GeometryUtilities.h
//
// \brief Functions to calculate distances and angles in 3D and 2D
//
// \author andrzej.szelc@yale.edu
//
////////////////////////////////////////////////////////////////////////
#ifndef UTIL_GEOMETRYUTILITIES_H
#define UTIL_GEOMETRYUTILITIES_H

#include <TMath.h>

#include "DataFormat-TypeDef.hh"
#include "Geometry.hh"
#include "LArProperties.hh"
#include "DetectorProperties.hh"
#include "PxUtils.h"

///General LArSoft Utilities
namespace larutil{
  
  class PxPoint;
  class PxLine;
  
  class GeometryUtilities : public larlight::larlight_base {
    
  private:
    /// Default constructor = private for singleton
    GeometryUtilities();
    
    static GeometryUtilities* _me;
    
  public:
    
    /// Singleton getter
    static const GeometryUtilities* GetME(){
      if(!_me) _me = new GeometryUtilities;
      return _me;
    }
    
    /// Default destructor
    ~GeometryUtilities();
    
    
    Int_t Get3DaxisN(Int_t iplane0,
		   Int_t iplane1,
		   Double_t omega0, 
		   Double_t omega1,
		   Double_t &phi,
		   Double_t &theta) const;
    
    Double_t CalculatePitch(UInt_t iplane0,
			  Double_t phi,
			  Double_t theta) const;
			  
    Double_t CalculatePitchPolar(UInt_t iplane0,
			       Double_t phi,
			       Double_t theta) const;
    
    Double_t Get3DSpecialCaseTheta(Int_t iplane0,
				 Int_t iplane1,
				 Double_t dw0, 
				 Double_t dw1) const;
        
    
    Double_t Get2Dangle(Double_t deltawire,
		      Double_t deltatime) const;

		      
		      
    Double_t Get2Dangle(Double_t wireend,
		      Double_t wirestart,
		      Double_t timeend,
		      Double_t timestart) const;
   
    double Get2Dangle(larutil::PxPoint endpoint,
		        larutil::PxPoint startpoint) const;
   		      
		      
		      
    Double_t Get2Dslope(Double_t deltawire,
		      Double_t deltatime) const;
		      
    Double_t Get2Dslope(Double_t wireend,
		      Double_t wirestart,
		      Double_t timeend,
		      Double_t timestart) const;
    
    double Get2Dslope(larutil::PxPoint endpoint,
		        larutil::PxPoint startpoint) const;		      
		      
    Double_t Get2DDistance(Double_t wire1,
			 Double_t time1,
			 Double_t wire2,
			 Double_t time2) const;
  
    double Get2DDistance(larutil::PxPoint point1,
			  larutil::PxPoint point2) const;			 
			 
			 
    Double_t Get2DPitchDistance(Double_t angle,
			      Double_t inwire,
			      Double_t wire) const;
    
    Double_t Get2DPitchDistanceWSlope(Double_t slope,
				    Double_t inwire,
				    Double_t wire) const;
    
    Int_t GetPointOnLine(Double_t slope,
		       Double_t intercept,
		       Double_t wire1,
		       Double_t time1,
		       Double_t &wireout,
		       Double_t &timeout) const;
    
    Int_t GetPointOnLine(Double_t slope,
		       Double_t wirestart,
		       Double_t timestart,
		       Double_t wire1,
		       Double_t time1,
		       Double_t &wireout,
		       Double_t &timeout) const;
    
    int GetPointOnLine(Double_t slope,
		       larutil::PxPoint startpoint,
		       larutil::PxPoint point1,
		       larutil::PxPoint pointout) const;		       
		       
    int GetPointOnLine(double slope,
	               double intercept,
			larutil::PxPoint point1,
			larutil::PxPoint pointout) const;
		       
    Int_t GetPointOnLineWSlopes(Double_t slope,
			      Double_t intercept,
			      Double_t ort_intercept,
			      Double_t &wireout,
			      Double_t &timeout) const;
    
    Int_t GetPointOnLineWSlopes(double slope,
			      double intercept,
			      double ort_intercept,
			      larutil::PxPoint &pointonline) const;		      
			      
			      
    const larlight::hit* FindClosestHit(const std::vector<larlight::hit*> &hitlist,
					UInt_t wire,
					Double_t time) const;
    
    UInt_t FindClosestHitIndex(const std::vector<larlight::hit*> &hitlist,
			       UInt_t wirein,
			       Double_t timein) const;			       
			 
			       
			       
			       
    PxPoint Get2DPointProjection(Double_t *xyz,Int_t plane) const;			       
	
    Double_t GetTimeTicks(Double_t x, Int_t plane) const;
    
    
    Int_t GetPlaneAndTPC(const larlight::hit* h,
			 UInt_t &p,
			 UInt_t &w) const;

    Int_t GetProjectedPoint(PxPoint p0,
			    PxPoint p1,
			    PxPoint &pN) const;

    Int_t GetYZ(PxPoint p0,
		PxPoint p1, 
		Double_t* yz) const;
    
    Double_t PitchInView(UInt_t plane,
			 Double_t phi,
			 Double_t theta) const;
    
    void GetDirectionCosines(Double_t phi,
			     Double_t theta,
			     Double_t *dirs) const;
			     
    void SelectLocalHitlist(const std::vector<larlight::hit*>& hitlist, 
			    std::vector <larlight::hit*> &hitlistlocal_index,
			    Double_t wire_start,
			    Double_t time_start, 
			    Double_t linearlimit,   
			    Double_t ortlimit, 
			    Double_t lineslopetest);

    void SelectLocalHitlist(const std::vector<larlight::hit*>& hitlist, 
			    std::vector <UInt_t> &hitlistlocal_index,
			    Double_t wire_start,
			    Double_t time_start, 
			    Double_t linearlimit,   
			    Double_t ortlimit, 
			    Double_t lineslopetest);
	
   Double_t TimeToCm() {return fTimetoCm;};
   Double_t WireToCm() {return fWiretoCm;};			     
    
  private:

    larutil::Geometry* geom;
    larutil::DetectorProperties* detp;
    larutil::LArProperties* larp;
    
    std::vector< Double_t > vertangle;  //angle wrt to vertical
    Double_t fWirePitch;
    Double_t fTimeTick;
    Double_t fDriftVelocity;
    UInt_t fNPlanes;
    Double_t fWiretoCm;
    Double_t fTimetoCm;
    Double_t fWireTimetoCmCm;
    
    }; // class GeometryUtilities

//     //helper class needed for the endpoint finding
//     class PxPoint {
//     public:
//       Double_t w;
//       Double_t t;
//       UInt_t plane;
//    
//       PxPoint(){
// 	plane=0;
// 	w=0;
// 	t=0;
//       }
//       
//       PxPoint(Int_t pp,Double_t ww,Double_t tt){
// 	plane=pp;
// 	w=ww;
// 	t=tt;
//       }
//     
//     };
    
    
//     //helper class needed for the seeding
//     class PxLine {
//     public:
//       
//       PxPoint pt0() { return PxPoint(plane,w0,t0); }
//       PxPoint pt1() { return PxPoint(plane,w1,t1); }
//       
//       Double_t w0; ///<defined to be the vertex w-position
//       Double_t t0; ///<defined to be the vertex t-position
//       Double_t w1; ///<defined to be the ending w-position (of line or seed depending)
//       Double_t t1; ///<defined to be the ending t-position (of line or seed depending)
//       UInt_t plane;
//    
//       PxLine(Int_t pp,Double_t ww0,Double_t tt0, Double_t ww1, Double_t tt1){
// 	plane=pp;
// 	w0=ww0;
// 	t0=tt0;
// 	w1=ww1;
// 	t1=tt1;
//       }
//     
//       PxLine(){
// 	plane=0;
// 	w0=0;
// 	t0=0;
// 	w1=0;
// 	t1=0;
//       }
//     
//     };
  
} //namespace larutils
#endif // UTIL_DETECTOR_PROPERTIES_H
