#ifndef CFALGOSTARTPOINTCOMPAT_CXX
#define CFALGOSTARTPOINTCOMPAT_CXX

#include "CFAlgoStartPointCompat.hh"

namespace cmtool {

  //-------------------------------------------------------
  CFAlgoStartPointCompat::CFAlgoStartPointCompat() : CFloatAlgoBase()
  //-------------------------------------------------------
  {
    _w2cm = larutil::GeometryUtilities::GetME()->WireToCm();
    _t2cm = larutil::GeometryUtilities::GetME()->TimeToCm();
    UseTime(true);
  }

  //-----------------------------
  void CFAlgoStartPointCompat::Reset()
  //-----------------------------
  {

  }

  //----------------------------------------------------------------------------------------------
  float CFAlgoStartPointCompat::Float(const std::vector<const cluster::ClusterParamsAlgNew*> &clusters)
  //----------------------------------------------------------------------------------------------
  {

    //This algorithm now works for 3 planes: find 3Dstart point from first 2 planes and find
    //How well that agrees with 3rd plane's start point location.

    //So first, make sure clusters vector has only 3 elements. If not return -1
    if ( clusters.size() != 3 )
      return -1;

    //Find 3D start point from start point on first 2 planes:
    //For now convert start point wire in cm back to wire number
    //Round to integer (sometimes output is double...why???)
    int startWire1 = int( clusters.at(0)->GetParams().start_point.w / _w2cm );
    double startTime1 = clusters.at(0)->GetParams().start_point.t;
    unsigned char Pl1 = clusters.at(0)->GetParams().start_point.plane;
    int startChan1 = larutil::Geometry::GetME()->PlaneWireToChannel(Pl1, startWire1);
    int startWire2 = int( clusters.at(1)->GetParams().start_point.w / _w2cm );
    double startTime2 = clusters.at(1)->GetParams().start_point.t;
    unsigned char Pl2 = clusters.at(1)->GetParams().start_point.plane;
    int startChan2 = larutil::Geometry::GetME()->PlaneWireToChannel(Pl2, startWire2);
    int startWire3 = int( clusters.at(2)->GetParams().start_point.w / _w2cm );
    double startTime3 = clusters.at(2)->GetParams().start_point.t;
    unsigned char Pl3 = clusters.at(2)->GetParams().start_point.plane;
    int startChan3 = larutil::Geometry::GetME()->PlaneWireToChannel(Pl3, startWire3);


    //Get Intersections in pairs:
    //y and z indicate detector coordinate and numbers indicate planes
    //used to generate that intersection point
    double yS12, zS12, yS13, zS13, yS23, zS23;

    larutil::Geometry::GetME()->IntersectionPoint( startWire1, startWire2,
						   Pl1, Pl2,
						   yS12, zS12);

    larutil::Geometry::GetME()->IntersectionPoint( startWire1, startWire3,
						   Pl1, Pl3,
						   yS13, zS13);

    larutil::Geometry::GetME()->IntersectionPoint( startWire2, startWire3,
						   Pl2, Pl3,
						   yS23, zS23);

    //assume X coordinate for these start-points is 0
    //i.e. only focus on projection onto wire-plane
    //then check if the start point reconstructe from
    //planes A and B falls within the wire-range of
    //the cluster on plane C
    //For compatibility also, check that the start-point-time
    //is within the time-range of the cluster on plane C
    Double_t Start1[3] = {-150., yS12, zS12};
    Double_t Start2[3] = {-150., yS13, zS13};
    Double_t Start3[3] = {-150., yS23, zS23};
    UInt_t WireStart1, WireStart2, WireStart3;
    try { WireStart1 = larutil::Geometry::GetME()->NearestChannel( Start1, Pl1); }
    catch ( ::larutil::LArUtilException &e) {
      std::cout << e.what() << std::endl;
      std::cout << "Exception caught!" << std::endl;
      WireStart1 = 9999;
      }
    try { WireStart2 = larutil::Geometry::GetME()->NearestChannel( Start2, Pl2); }
    catch ( ::larutil::LArUtilException &e ) {
      std::cout << e.what() << std::endl;
      std::cout << "Exception caught!" << std::endl;
      WireStart1 = 9999;
      }
    try { WireStart3 = larutil::Geometry::GetME()->NearestChannel( Start3, Pl3); }
    catch ( ::larutil::LArUtilException &e) {
      std::cout << e.what() << std::endl;
      std::cout << "Exception caught!" << std::endl;
      WireStart1 = 9999;
      }

    //Now Get Hit-Range for Clusters
    std::vector<larutil::PxHit> hits1 = clusters.at(0)->GetHitVector();
    std::vector<larutil::PxHit> hits2 = clusters.at(1)->GetHitVector();
    std::vector<larutil::PxHit> hits3 = clusters.at(2)->GetHitVector();
    //define variables for min/max time/wire of each cluster
    double minWire1, minWire2, minWire3 = 99999;
    double maxWire1, maxWire2, maxWire3 = 0;
    double minTime1, minTime2, minTime3 = 99999;
    double maxTime1, maxTime2, maxTime3 = 0;
    int hitWireTMP = 0;
    double hitTimeTMP = 0;
    for (auto& h: hits1){
      hitWireTMP = int(h.w/_w2cm);
      hitTimeTMP = int(h.t/_t2cm);
      if ( hitWireTMP < minWire1 ) { minWire1 = hitWireTMP; }
      if ( hitWireTMP > maxWire1 ) { maxWire1 = hitWireTMP; }
      if ( hitTimeTMP < minTime1 ) { minTime1 = hitTimeTMP; }
      if ( hitTimeTMP > maxTime1 ) { maxTime1 = hitTimeTMP; }
    }
    for (auto& h: hits2){
      hitWireTMP = int(h.w/_w2cm);
      hitTimeTMP = int(h.t/_t2cm);
      if ( hitWireTMP < minWire2 ) { minWire2 = hitWireTMP; }
      if ( hitWireTMP > maxWire2 ) { maxWire2 = hitWireTMP; }
      if ( hitTimeTMP < minTime2 ) { minTime2 = hitTimeTMP; }
      if ( hitTimeTMP > maxTime2 ) { maxTime2 = hitTimeTMP; }
    }
    for (auto& h: hits3){
      hitWireTMP = int(h.w/_w2cm);
      hitTimeTMP = int(h.t/_t2cm);
      if ( hitWireTMP < minWire3 ) { minWire3 = hitWireTMP; }
      if ( hitWireTMP > maxWire3 ) { maxWire3 = hitWireTMP; }
      if ( hitTimeTMP < minTime3 ) { minTime3 = hitTimeTMP; }
      if ( hitTimeTMP > maxTime3 ) { maxTime3 = hitTimeTMP; }
    }

    if ( _verbose ){
      std::cout << "Wire Start Numbers: " << std::endl;
      std::cout << "\t" << startWire1 << std::endl;
      std::cout << "\t" << startWire2 << std::endl;
      std::cout << "\t" << startWire3 << std::endl;
      std::cout << std::endl;
    }

    if ( _verbose ){
      std::cout << "Intersection Pl1-Pl3: ( " << yS13 << ", " << zS13 << " )" << std::endl;  
      std::cout << "Intersection Pl1-Pl2: ( " << yS12 << ", " << zS12 << " )" << std::endl;  
      std::cout << "Intersection Pl2-Pl3: ( " << yS23 << ", " << zS23 << " )" << std::endl;  
    }

    //Parameter used for evaluation is whether the start-point of any reconstructed start point from 2 planes
    //is in the start-end time-wire range of the other plane's cluster
    double compat = -1;

    if ( (WireStart1 > minWire1) and (WireStart1 < maxWire1) ){
      if (_verbose) { std::cout << "Plane 1 Match!" << std::endl; }
      compat = 1;
    }
    if ( (WireStart2 > minWire2) and (WireStart2 < maxWire2) ){
      if (_verbose) { std::cout << "Plane 2 Match!" << std::endl; }
      compat = 1;
    }
    if ( (WireStart3 > minWire3) and (WireStart3 < maxWire3) ){
      if (_verbose) { std::cout << "Plane 3 Match!" << std::endl; }
      compat = 1;
    }

    return compat;
  }

  //------------------------------
  void CFAlgoStartPointCompat::Report()
  //------------------------------
  {

  }
    
}
#endif
