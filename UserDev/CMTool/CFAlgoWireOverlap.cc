#ifndef CFALGOWIREOVERLAP_CXX
#define CFALGOWIREOVERLAP_CXX

#include "CFAlgoWireOverlap.hh"
#include <algorithm>

namespace cmtool {

  //-------------------------------------------------------
  CFAlgoWireOverlap::CFAlgoWireOverlap() : CFloatAlgoBase()
  //-------------------------------------------------------
  {
    _w2cm = larutil::GeometryUtilities::GetME()->WireToCm();
    _t2cm = larutil::GeometryUtilities::GetME()->TimeToCm();
    SetVerbose(false);
  }

  //-----------------------------
  void CFAlgoWireOverlap::Reset()
  //-----------------------------
  {
  }

  //----------------------------------------------------------------------------------------------
  float CFAlgoWireOverlap::Float(const std::vector<const cluster::ClusterParamsAlg*> &clusters)
  //----------------------------------------------------------------------------------------------
  {
    // Code-block by Kazu starts
    // This ensures all entries in "clusters" pointer vector are valid pointers.
    // You may take out this block if you want to allow matching using clusters from only 2 planes.
    // But make sure you handle the case of null pointer
    for(auto const& ptr : clusters) if(!ptr) return -1;
    // Code-block by Kazu ends

    //This algorithm now works for 3 planes: find 3Dstart point from first 2 planes and find
    //How well that agrees with 3rd plane's start point location.

    //So first, make sure clusters vector has only 3 elements. If not return -1
    if ( clusters.size() != 3 )
      return -1;

    //Get hits for all 3 clusters
    std::vector<larutil::PxHit> hits0 = clusters.at(0)->GetHitVector();
    std::vector<larutil::PxHit> hits1 = clusters.at(1)->GetHitVector();
    std::vector<larutil::PxHit> hits2 = clusters.at(2)->GetHitVector();
    
    //Get Wire-Range for all 3 clusters
    int startWire0 = 9999, endWire0 = 0;
    int startWire1 = 9999, endWire1 = 0;
    int startWire2 = 9999, endWire2 = 0;
    for (auto& hit: hits0){
      if ( int(hit.w / _w2cm) < startWire0 ) { startWire0 = int(hit.w / _w2cm); }
      if ( int(hit.w / _w2cm) > endWire0 )   { endWire0   = int(hit.w / _w2cm); }
    }
    for (auto& hit: hits1){
      if ( int(hit.w / _w2cm) < startWire1 ) { startWire1 = int(hit.w / _w2cm); }
      if ( int(hit.w / _w2cm) > endWire1 )   { endWire1   = int(hit.w / _w2cm); }
    }
    for (auto& hit: hits2){
      if ( int(hit.w / _w2cm) < startWire2 ) { startWire2 = int(hit.w / _w2cm); }
      if ( int(hit.w / _w2cm) > endWire2 )   { endWire2   = int(hit.w / _w2cm); }
    }

    //Now get start & end points of all these wires
    Double_t xyzStart0WireStart[3] = {0., 0., 0.}; //xyz array info of start point for smallest wire number on plane 0
    Double_t xyzStart0WireEnd[3]   = {0., 0., 0.}; 
    Double_t xyzEnd0WireStart[3] = {0., 0., 0.}; 
    Double_t xyzEnd0WireEnd[3]   = {0., 0., 0.}; 
    Double_t xyzStart1WireStart[3] = {0., 0., 0.}; //xyz array info of start point for smallest wire number on plane 1
    Double_t xyzStart1WireEnd[3]   = {0., 0., 0.}; 
    Double_t xyzEnd1WireStart[3] = {0., 0., 0.}; 
    Double_t xyzEnd1WireEnd[3]   = {0., 0., 0.}; 
    Double_t xyzStart2WireStart[3] = {0., 0., 0.}; //xyz array info of start point for smallest wire number on plane 2
    Double_t xyzStart2WireEnd[3]   = {0., 0., 0.}; 
    Double_t xyzEnd2WireStart[3] = {0., 0., 0.}; 
    Double_t xyzEnd2WireEnd[3]   = {0., 0., 0.}; 

    if (_verbose) {
      std::cout << "Wire Ranges:" << std::endl;
      std::cout << "U-Plane: [ " << startWire0 << ", " << endWire0 << "]" << std::endl;
      std::cout << "V-Plane: [ " << startWire1 << ", " << endWire1 << "]" << std::endl;
      std::cout << "Y-Plane: [ " << startWire2 << ", " << endWire2 << "]" << std::endl;
      std::cout << std::endl;
    }

    larutil::Geometry::GetME()->WireEndPoints(0, startWire0, xyzStart0WireStart, xyzStart0WireEnd);
    larutil::Geometry::GetME()->WireEndPoints(0, endWire0, xyzEnd0WireStart, xyzEnd0WireEnd);
    larutil::Geometry::GetME()->WireEndPoints(1, startWire1, xyzStart1WireStart, xyzStart1WireEnd);
    larutil::Geometry::GetME()->WireEndPoints(1, endWire1, xyzEnd1WireStart, xyzEnd1WireEnd);
    larutil::Geometry::GetME()->WireEndPoints(2, startWire2, xyzStart2WireStart, xyzStart2WireEnd);
    larutil::Geometry::GetME()->WireEndPoints(2, endWire2, xyzEnd2WireStart, xyzEnd2WireEnd);
    //check that z-positions for plane2 are the same...if not error!
    //if ( xyzStart2WireStart[2] != xyzStart2WireEnd[2] ) { std::cout << "Y-wire does not have same Z start and end..." << std::endl; }
    double zMin = xyzStart2WireStart[2];
    //if ( xyzEnd2WireStart[2] != xyzEnd2WireEnd[2] ) { std::cout << "Y-wire does not have same Z start and end..." << std::endl; }
    double zMax = xyzEnd2WireStart[2];

    //Plane U == Plane 0
    //Plane V == Plane 1
    //Plane Y == Plane 2

    //Each line can be described by function y = s*z + b (x,y,z coordinates same as uBooNE coord.)
    //Slopes known: Pl0 is +60 clockwise from vertical. Pl1 is -60, counterclockwise from vertical. Looking at TPC with beam from left
    double slopeU = tan(30*3.14/180.);
    double slopeV = tan(-30*3.14/180.);
    if (_verbose){
      std::cout << "Slope U plane: " << slopeU << std::endl;
      std::cout << "Slope V plane: " << slopeV << std::endl;
    }
    //Find intercepts:
    double bUup = xyzStart0WireStart[1] - xyzStart0WireStart[2] * slopeU; 
    double bUdn   = xyzEnd0WireStart[1] - xyzEnd0WireStart[2] * slopeU; 
    double bVdn = xyzStart1WireStart[1] - xyzStart1WireStart[2] * slopeV; 
    double bVup   = xyzEnd1WireStart[1] - xyzEnd1WireStart[2] * slopeV; 
    //make sure we know which line is above which
    if ( bUup < bUdn ) { std::cout << "Uup and Udn are mixed up!" << std::endl; }
    if ( bVdn > bVup ) { std::cout << "Vup and Vdn are mixed up!" << std::endl; }
    //Pl2 lines are vertical...slope is infinite and no intercept.

    //Find intercepts of U wire-ranges with Y plane (easy since vertical)
    //For now assume wire-ranges go to infinity, worry about TPC constraints later

    //Plug in Y-plane zMin and zMax coordinates into equations for U/V wire lines
    double VdnZmin = slopeV * zMin + bVdn; 
    double VdnZmax = slopeV * zMax + bVdn; 
    double VupZmin = slopeV * zMin + bVup; 
    double VupZmax = slopeV * zMax + bVup; 
    double UdnZmin = slopeU * zMin + bUdn; 
    double UdnZmax = slopeU * zMax + bUdn; 
    double UupZmin = slopeU * zMin + bUup; 
    double UupZmax = slopeU * zMax + bUup;

    if (_verbose){
      std::cout << "Y-Plane and U-Plane points [Z,Y]:" << std::endl;
      std::cout << "\t\t[ " << zMin << ", " << UdnZmin << "]" << std::endl;
      std::cout << "\t\t[ " << zMin << ", " << UupZmin << "]" << std::endl;
      std::cout << "\t\t[ " << zMax << ", " << UupZmax << "]" << std::endl;
      std::cout << "\t\t[ " << zMax << ", " << UdnZmax << "]" << std::endl;
      std::cout << "Y-Plane and V-Plane points [Z,Y]:" << std::endl;
      std::cout << "\t\t[ " << zMin << ", " << VdnZmin << "]" << std::endl;
      std::cout << "\t\t[ " << zMin << ", " << VupZmin << "]" << std::endl;
      std::cout << "\t\t[ " << zMax << ", " << VupZmax << "]" << std::endl;
      std::cout << "\t\t[ " << zMax << ", " << VdnZmax << "]" << std::endl;
    }
    //We now have Two polygons:
    //One is the intersection of Y-Plane wires with U-Plane wires
    //The other the intersection of planes Y and V.
    //The intersection points of these two polygons is the 
    //overall intersection Area of the 3 clusters on the Y-Z plane.

    //Go through all siegment intersections. If one is found add to 
    //list of points.
    //Create a list of points for polygon
    std::vector< std::pair<float,float> > WireIntersection;
    double zInt; // temporary holder for z-intersection point of oblique sides
    //Check: Vup and Uup, Vup and Uright, Vup and Uleft, Vup and Udn.
    //Intersection between Vup and Uup: if within zMin, zMax then ok!
    zInt = (bUup-bVup)/(slopeV-slopeU);
    if ( (zInt > zMin) and (zInt < zMax) )
      WireIntersection.push_back( std::make_pair( zInt, slopeV*zInt+bVup ) );
    //Intersection between Vup and Uright:
    if ( (VupZmax < UupZmax) and (VupZmax > UdnZmax) )
      WireIntersection.push_back( std::make_pair( zMax, VupZmax ) );
    //Intersection between Vup and Uleft:
    if ( (VupZmin < UupZmin) and ( VupZmin > UdnZmin) )
      WireIntersection.push_back( std::make_pair( zMin, VupZmin ) );
    //Intersection between Vup and Udn:
    zInt = (bUdn-bVup)/(slopeV-slopeU);
    if ( (zInt > zMin) and (zInt < zMax) )
      WireIntersection.push_back( std::make_pair( zInt, slopeV*zInt+bVup ) );
    
    //Check: Vdn and Uup, Uright, Uleft, Udn:
    //Intersection between Vdn and Uup:
    zInt = (bUup-bVdn)/(slopeV-slopeU);
    if ( (zInt > zMin) and (zInt < zMax) )
      WireIntersection.push_back( std::make_pair( zInt, slopeV*zInt+bVdn ) );
    //Intersection between Vdn and Uright:
    if ( (VdnZmax < UupZmax) and (VdnZmax > UdnZmax) )
      WireIntersection.push_back( std::make_pair( zMax, VdnZmax ) );
    //Intersection between Vdn and Uleft:
    if ( (VdnZmin < UupZmin) and ( VdnZmin > UdnZmin) )
      WireIntersection.push_back( std::make_pair( zMin, VdnZmin ) );
    //Intersection between Vdn and Udn:
    zInt = (bUdn-bVdn)/(slopeV-slopeU);
    if ( (zInt > zMin) and (zInt < zMax) )
      WireIntersection.push_back( std::make_pair( zInt, slopeV*zInt+bVdn ) );
      
    //Check: Vright and Uup, Udn:
    //Intersection between Vright and Uup:
    if ( (UupZmax < VupZmax) and ( UupZmax > VdnZmax) )
      WireIntersection.push_back( std::make_pair( zMax, UupZmax ) );
    //Intersection between Vright and Udn:
    if ( (UdnZmax < VupZmax) and ( UdnZmax > VdnZmax) )
      WireIntersection.push_back( std::make_pair( zMax, UdnZmax ) );

    //Check Vleft and Uup, Udn:
    //Intersection between Vleft and Uup:
    if ( (UupZmin < VupZmin) and ( UupZmin > VdnZmin) )
      WireIntersection.push_back( std::make_pair( zMin, UupZmin ) );
    //Intersection between Vleft and Udn:
    if ( (UdnZmin < VupZmin) and ( UdnZmin > VdnZmin) )
      WireIntersection.push_back( std::make_pair( zMin, UdnZmin ) );

    //If length is 0 then no intersection...return -1
    if ( WireIntersection.size() == 0 ){
      if (_verbose) { std::cout << "No intersection..." << std::endl << std::endl; }
      return -1;
    }

    //Now our polygon is complete...
    //need to disentangle in case points added in incorrect order
    //then calculate area
    Polygon2D WirePolygon(WireIntersection);    
    //Check order
    WirePolygon.UntanglePolygon();

    if (_verbose){
      std::cout << "Intersection Polygon Coordinates [Z,Y]: " << std::endl;
      for (unsigned int s=0; s < WirePolygon.Size(); s++)
	std::cout << "\t\t[ " << WirePolygon.Point(s).first << ", " << WirePolygon.Point(s).second << "]" << std::endl;
      std::cout << std::endl;
    }
    
    //return polygon area -> larger = better!
    if (_verbose) { std::cout << "Intersection area: " << WirePolygon.Area() << std::endl << std::endl; }    
    return  WirePolygon.Area();
  }

  //------------------------------
  void CFAlgoWireOverlap::Report()
  //------------------------------
  {

  }
    
}
#endif
