#ifndef GEOMETRY_CC
#define GEOMETRY_CC

#include "Geometry.hh"

namespace larutil {

  Geometry* Geometry::_me = 0;

  Geometry::Geometry(std::string file_name, std::string tree_name) : LArUtilBase(file_name,tree_name)
  {
    if(_file_name.empty())
      _file_name = Form("%s/LArUtil/dat/%s",getenv("LARLIGHT_CORE_DIR"),kFILENAME_GEOMETRY.c_str());
    if(_tree_name.empty())
      _tree_name = kTREENAME_GEOMETRY;
    _name = "Geometry";
    LoadData();
  }

  void Geometry::SetBranchAddress()
  {
    if(!_data_tree) {
      throw LArUtilException(Form("DataTree has to be specified prior to %s function call",__FUNCTION__));
      return;
    }
    
    std::string error_msg("");
    if(!(_data_tree->GetBranch("fDetLength")))      error_msg += "      fDetLength\n";
    if(!(_data_tree->GetBranch("fDetHalfWidth")))   error_msg += "      fDetHalfWidth\n";
    if(!(_data_tree->GetBranch("fDetHalfHeight")))  error_msg += "      fDetHalfHeight\n";

    if(!(_data_tree->GetBranch("fCryoLength")))     error_msg += "      fCryoLength\n";
    if(!(_data_tree->GetBranch("fCryoHalfWidth")))  error_msg += "      fCryoHalfWidth\n";
    if(!(_data_tree->GetBranch("fCryoHalfHeight"))) error_msg += "      fCryoHalfHeight\n";

    if(!(_data_tree->GetBranch("fChannelToPlaneMap")))     error_msg += "      fChannelToPlaneMap\n";
    if(!(_data_tree->GetBranch("fChannelToWireMap")))      error_msg += "      fChannelToWireMap\n";
    if(!(_data_tree->GetBranch("fPlaneWireToChannelMap"))) error_msg += "      fPlaneWireToChannelMap\n";

    if(!(_data_tree->GetBranch("fSignalType"))) error_msg += "      fSignalType\n";
    if(!(_data_tree->GetBranch("fViewType")))   error_msg += "      fViewType\n";
    if(!(_data_tree->GetBranch("fPlanePitch"))) error_msg += "      fPlanePitch\n";

    if(!(_data_tree->GetBranch("fFirstWireStartVtx"))) error_msg += "      fFirstWireStartVtx\n";
    if(!(_data_tree->GetBranch("fFirstWireEndVtx")))   error_msg += "      fFirstWireEndVtx\n";

    if(!(_data_tree->GetBranch("fWirePitch")))    error_msg += "      fWirePitch\n";
    if(!(_data_tree->GetBranch("fWireAngle")))    error_msg += "      fWireAngle\n";
    if(!(_data_tree->GetBranch("fOpChannelVtx"))) error_msg += "      fOpChannelVtx\n";
    
    if(!(_data_tree->GetBranch("fPlaneOriginVtx"))) error_msg += "      fPlaneOriginVtx\n";

    if(!error_msg.empty()) {

      throw LArUtilException(Form("Missing following TBranches...\n%s",error_msg.c_str()));

      return;
    }
    
    _data_tree->SetBranchAddress("fDetLength",&fDetLength);
    _data_tree->SetBranchAddress("fDetHalfWidth",&fDetHalfWidth);
    _data_tree->SetBranchAddress("fDetHalfHeight",&fDetHalfHeight);

    _data_tree->SetBranchAddress("fCryoLength",&fCryoLength);
    _data_tree->SetBranchAddress("fCryoHalfWidth",&fCryoHalfWidth);
    _data_tree->SetBranchAddress("fCryoHalfHeight",&fCryoHalfHeight);

    _data_tree->SetBranchAddress("fChannelToPlaneMap",&fChannelToPlaneMap);
    _data_tree->SetBranchAddress("fChannelToWireMap",&fChannelToWireMap);
    _data_tree->SetBranchAddress("fPlaneWireToChannelMap",&fPlaneWireToChannelMap);

    _data_tree->SetBranchAddress("fSignalType",&fSignalType);
    _data_tree->SetBranchAddress("fViewType",&fViewType);
    _data_tree->SetBranchAddress("fPlanePitch",&fPlanePitch);

    _data_tree->SetBranchAddress("fFirstWireStartVtx",&fFirstWireStartVtx);
    _data_tree->SetBranchAddress("fFirstWireEndVtx",&fFirstWireEndVtx);
    _data_tree->SetBranchAddress("fWirePitch",&fWirePitch);
    _data_tree->SetBranchAddress("fWireAngle",&fWireAngle);

    _data_tree->SetBranchAddress("fOpChannelVtx",&fOpChannelVtx);

    _data_tree->SetBranchAddress("fPlaneOriginVtx",&fPlaneOriginVtx);

  }


  UInt_t Geometry::Nwires(UInt_t p) const
  {
    if(Nplanes() <= p) {
      throw LArUtilException(Form("Invalid plane ID :%d",p));
      return larlight::DATA::INVALID_UINT;
    }

    return fPlaneWireToChannelMap->at(p).size();
  }

  UChar_t  Geometry::ChannelToPlane(const UInt_t ch) const
  {
    if(ch >= fChannelToPlaneMap->size()) {
      throw LArUtilException(Form("Invalid channel number: %d",ch));
      return larlight::DATA::INVALID_CHAR;
    }
    return fChannelToPlaneMap->at(ch);
  }

  UInt_t   Geometry::ChannelToWire(const UInt_t ch)const
  {
    if(ch >= fChannelToWireMap->size()) {
      throw LArUtilException(Form("Invalid channel number: %d",ch));
      return larlight::DATA::INVALID_CHAR;
    }
    return fChannelToWireMap->at(ch);
  }

  larlight::GEO::SigType_t Geometry::SignalType(const UInt_t ch) const
  {
    if(ch >= fChannelToPlaneMap->size()) {
      throw LArUtilException(Form("Invalid Channel number :%d",ch));
      return larlight::GEO::kMysteryType;
    }
    
    return fSignalType->at(fChannelToPlaneMap->at(ch));
  }

  larlight::GEO::SigType_t Geometry::PlaneToSignalType(const UChar_t plane) const
  {
    if(plane >= fSignalType->size()) {
      throw LArUtilException(Form("Invalid Plane number: %d",plane));
      return larlight::GEO::kMysteryType;
    }

    return fSignalType->at(plane);
  }

  larlight::GEO::View_t Geometry::View(const UInt_t ch) const
  {
    if(ch >= fChannelToPlaneMap->size()) {
      throw LArUtilException(Form("Invalid Channel number :%d",ch));
      return larlight::GEO::kUnknown;
    }
    
    return fViewType->at(fChannelToPlaneMap->at(ch));
  }

  larlight::GEO::View_t Geometry::PlaneToView(const UChar_t plane) const
  {
    if(plane >= fViewType->size()) {
      throw LArUtilException(Form("Invalid Plane number: %d",plane));
      return larlight::GEO::kUnknown;
    }

    return fViewType->at(plane);
  }

  std::set<larlight::GEO::View_t> const Geometry::Views() const
  {
    std::set<larlight::GEO::View_t> views;
    for(auto const v : *fViewType) views.insert(v);
    return views;
  }

  UInt_t Geometry::PlaneWireToChannel(const UInt_t plane,
				      const UInt_t wire) const
  {
    if(plane >= Nplanes() || fPlaneWireToChannelMap->at(plane).size() >= wire) {
      throw LArUtilException(Form("Invalid (plane, wire) = (%d, %d)",plane,wire));
      return larlight::DATA::INVALID_UINT;
    }
    return fPlaneWireToChannelMap->at(plane).at(wire);
  }

  UInt_t Geometry::NearestChannel(const Double_t worldLoc[3],
				  const UInt_t PlaneNo) const
  {
    return PlaneWireToChannel(PlaneNo,NearestWire(worldLoc,PlaneNo));
  }

  UInt_t Geometry::NearestChannel(const std::vector<Double_t> worldLoc,
				  const UInt_t PlaneNo) const
  {
    return PlaneWireToChannel(PlaneNo,NearestWire(worldLoc,PlaneNo));
  }

  UInt_t Geometry::NearestChannel(const TVector3 &worldLoc,
				  const UInt_t PlaneNo) const
  {
    return PlaneWireToChannel(PlaneNo,NearestWire(worldLoc,PlaneNo));
  }

  UInt_t Geometry::NearestWire(const Double_t worldLoc[3],
			       const UInt_t PlaneNo) const
  {
    return larlight::DATA::INVALID_UINT;
  }
  
  UInt_t Geometry::NearestWire(const std::vector<Double_t> worldLoc,
			       const UInt_t PlaneNo) const
  {
    return larlight::DATA::INVALID_UINT;
  }
  
  UInt_t Geometry::NearestWire(const TVector3 &worldLoc,
			       const UInt_t PlaneNo) const
  {
    return larlight::DATA::INVALID_UINT;
  }

  // distance between planes p1 < p2
  Double_t Geometry::PlanePitch(const UChar_t p1, const UChar_t p2) const
  {
    if( p1==p2 ) return 0;
    else if( (p1==0 && p2==1) || (p1==1 && p2==0) ) return fPlanePitch->at(0);
    else if( (p1==1 && p2==2) || (p1==2 && p2==1) ) return fPlanePitch->at(1);
    else if( (p1==0 && p2==2) || (p1==2 && p2==0) ) return fPlanePitch->at(2);
    else {
      throw LArUtilException("Plane number > 2 not supported!");
      return larlight::DATA::INVALID_DOUBLE;
    }
  }

  Double_t Geometry::WirePitch(const UInt_t  w1,
			       const UInt_t  w2,
			       const UChar_t plane) const
  {
    if( w1 > w2 && w1 >= fPlaneWireToChannelMap->at(plane).size() ) {
      throw LArUtilException(Form("Invalid wire number: %d",w1));
      return larlight::DATA::INVALID_DOUBLE;
    }
    if( w2 > w1 && w2 >= fPlaneWireToChannelMap->at(plane).size() ) {
      throw LArUtilException(Form("Invalid wire number: %d",w2));
      return larlight::DATA::INVALID_DOUBLE;
    }

    return ( w1 < w2 ? (w2-w1)*(fWirePitch->at(fViewType->at(plane))) : (w1-w2)*(fWirePitch->at(fViewType->at(plane))));
  }

  void Geometry::WireEndPoints(const UChar_t plane, 
			       const UInt_t wire, 
			       Double_t *xyzStart, Double_t *xyzEnd) const
  {
    xyzStart[0] = fFirstWireStartVtx->at(plane).at(0);
    xyzStart[1] = fFirstWireStartVtx->at(plane).at(1);
    xyzStart[2] = fFirstWireStartVtx->at(plane).at(2);
    xyzEnd[0]   = fFirstWireEndVtx->at(plane).at(0);
    xyzEnd[1]   = fFirstWireEndVtx->at(plane).at(1);
    xyzEnd[2]   = fFirstWireEndVtx->at(plane).at(2);

    Double_t angle_z = fWireAngle->at(fViewType->at(plane));
    if(angle_z > TMath::Pi()/2) angle_z -= TMath::Pi()/2;

    Double_t dz = wire * fWirePitch->at(fViewType->at(plane)) * TMath::Cos(angle_z);
    xyzStart[2] += dz;
    xyzEnd[2]   += dz;
  }
  
  bool Geometry::ChannelsIntersect(const UInt_t c1, 
				   const UInt_t c2, 
				   Double_t &y, Double_t &z) const
  {
    if(c1==c2){
      throw LArUtilException("Same channel does not intersect!");
      return false;
    }

    if( c1 >= fChannelToPlaneMap->size() || c2>= fChannelToPlaneMap->size() ) {
      throw LArUtilException(Form("Invalid channels : %d and %d",c1,c2));
      return false;
    }
    if( fViewType->at(fChannelToPlaneMap->at(c1)) == fViewType->at(fChannelToPlaneMap->at(c2)) ) {
      return false;
    }

    UInt_t w1 = fChannelToWireMap->at(c1);
    UInt_t w2 = fChannelToWireMap->at(c2);
    
    UChar_t p1 = fChannelToPlaneMap->at(c1);
    UChar_t p2 = fChannelToPlaneMap->at(c2);

    larlight::GEO::View_t v1 = fViewType->at(p1);
    larlight::GEO::View_t v2 = fViewType->at(p2);

    Double_t start1[3]={0.};
    Double_t start2[3]={0.};
    Double_t end1[3]={0.};
    Double_t end2[3]={0.};

    WireEndPoints(p1, w1, start1, end1);
    WireEndPoints(p2, w2, start2, end2);

    // if endpoint of one input wire is within range of other input wire in
    // BOTH y AND z, wires overlap
    bool overlapY = (ValueInRange(start1[1], start2[1], end2[1]) || ValueInRange(end1[1], start2[1], end2[1]));
    bool overlapZ = (ValueInRange(start1[2], start2[2], end2[2]) || ValueInRange(end1[2], start2[2], end2[2]));

    bool overlapY_rev = (ValueInRange(start2[1], start1[1], end1[1]) || ValueInRange(end2[1], start1[1], end1[1]));
    bool overlapZ_rev = (ValueInRange(start2[2], start1[2], end1[2]) || ValueInRange(end2[2], start1[2], end1[2]));
				       
    // override y overlap checks if a vertical plane exists:
      if( fWireAngle->at(v1) == TMath::Pi()/2 || fWireAngle->at(v2) == TMath::Pi()/2 ) {
      overlapY     = true;
      overlapY_rev = true;
    }
    
    //catch to get vertical wires, where the standard overlap might not work, Andrzej
    if(std::abs(start2[2] - end2[2]) < 0.01) overlapZ = overlapZ_rev;


    if(overlapY && overlapZ){
      IntersectionPoint(w1,w2, p1, p2,
			start1, end1,
			start2, end2,
			y, z);
      return true;
    }

    else if(overlapY_rev && overlapZ_rev){
      this->IntersectionPoint(w2, w1, p2, p1,
                              start2, end2,
                              start1, end1,
                              y, z);
      return true;
    }
    
    return false;

  }
  
  void Geometry::IntersectionPoint(const UInt_t  wire1,  const UInt_t  wire2,
				   const UChar_t plane1, const UChar_t plane2,
				   Double_t start_w1[3], Double_t end_w1[3],
				   Double_t start_w2[3], Double_t end_w2[3],
				   Double_t &y, Double_t &z) const
  {

    larlight::GEO::View_t v1 = fViewType->at(plane1);
    larlight::GEO::View_t v2 = fViewType->at(plane2);
    //angle of wire1 wrt z-axis in Y-Z plane...in radians
    Double_t angle1 = fWireAngle->at(v1);
    //angle of wire2 wrt z-axis in Y-Z plane...in radians
    Double_t angle2 = fWireAngle->at(v2);

    if(angle1 == angle2) return; //comparing two wires in the same plane...pointless.

     //coordinates of "upper" endpoints...(z1,y1) = (a,b) and (z2,y2) = (c,d)
    double a = 0.;
    double b = 0.;
    double c = 0.;
    double d = 0.;
    double angle = 0.;
    double anglex = 0.;

    // below is a special case of calculation when one of the planes is vertical.
    angle1 < angle2 ? angle = angle1 : angle = angle2;//get angle closest to the z-axis
    
    // special case, one plane is vertical 
    if(angle1 == TMath::Pi()/2 || angle2 == TMath::Pi()/2){
      if(angle1 == TMath::Pi()/2){
	
	anglex = (angle2-TMath::Pi()/2);
        a = end_w1[2];
        b = end_w1[1];
        c = end_w2[2];
        d = end_w2[1];
        // the if below can in principle be replaced by the sign of anglex (inverted)
        // in the formula for y below. But until the geometry is fully symmetric in y I'm
        // leaving it like this. Andrzej 
        if((anglex) > 0 ) b = start_w1[1];

      } 
      else if(angle2 == TMath::Pi()/2){
        anglex = (angle1-TMath::Pi()/2);
        a = end_w2[2];
        b = end_w2[1];
        c = end_w1[2];
        d = end_w1[1];
        // the if below can in principle be replaced by the sign of anglex (inverted)
        // in the formula for y below. But until the geometry is fully symmetric in y I'm
        // leaving it like this. Andrzej
        if((anglex) > 0 ) b = start_w2[1];
      }

      y = b + ((c-a) - (b-d)*tan(anglex))/tan(anglex);
      z = a;   // z is defined by the wire in the vertical plane
      
      return;
    }

    // end of vertical case 
    z = 0;y = 0;

    if(angle1 < (TMath::Pi()/2.0)){
      c = end_w1[2];
      d = end_w1[1];
      a = start_w2[2];
      b = start_w2[1];
    }
    else{
      c = end_w2[2];
      d = end_w2[1];
      a = start_w1[2];
      b = start_w1[1];
    }

    //Intersection point of two wires in the yz plane is completely
    //determined by wire endpoints and angle of inclination.
    z = 0.5 * ( c + a + (b-d)/TMath::Tan(angle) );
    y = 0.5 * ( b + d + (a-c)*TMath::Tan(angle) );

    return;

  }

  // Added shorthand function where start and endpoints are looked up automatically
  //  - whether to use this or the full function depends on optimization of your
  //    particular algorithm.  Ben J, Oct 2011
  //--------------------------------------------------------------------
  void Geometry::IntersectionPoint(const UInt_t  wire1,  const UInt_t  wire2,
				   const UChar_t plane1, const UChar_t plane2,
				   Double_t &y, Double_t &z) const

  {
    double WireStart1[3] = {0.};
    double WireStart2[3] = {0.};
    double WireEnd1[3]   = {0.};
    double WireEnd2[3]   = {0.};

    this->WireEndPoints(plane1, wire1, WireStart1, WireEnd1);
    this->WireEndPoints(plane2, wire2, WireStart2, WireEnd2);
    this->IntersectionPoint(wire1, wire2, plane1, plane2,
                            WireStart1, WireEnd1, 
			    WireStart2, WireEnd2, y, z);
  }

  UInt_t Geometry::GetClosestOpChannel(const Double_t *xyz) const
  {
    Double_t dist2      = 0;
    Double_t min_dist2  = larlight::DATA::INVALID_DOUBLE;
    UInt_t   closest_ch = larlight::DATA::INVALID_UINT;
    for(size_t ch=0; ch<fOpChannelVtx->size(); ++ch) {

      dist2 = 
	pow(xyz[0] - fOpChannelVtx->at(ch).at(0),2) + 
	pow(xyz[1] - fOpChannelVtx->at(ch).at(1),2) +
	pow(xyz[2] - fOpChannelVtx->at(ch).at(2),2); 
	
      if( dist2 < min_dist2 ) {
	
	min_dist2 = dist2;
	closest_ch = ch;

      }
    }

    return closest_ch;
  }

  UInt_t Geometry::GetClosestOpChannel(const Double_t *xyz, Double_t &dist) const
  {
    Double_t min_dist2  = larlight::DATA::INVALID_DOUBLE;
    UInt_t   closest_ch = larlight::DATA::INVALID_UINT;
    for(size_t ch=0; ch<fOpChannelVtx->size(); ++ch) {

      dist = 
	pow(xyz[0] - fOpChannelVtx->at(ch).at(0),2) + 
	pow(xyz[1] - fOpChannelVtx->at(ch).at(1),2) +
	pow(xyz[2] - fOpChannelVtx->at(ch).at(2),2); 
	
      if( dist < min_dist2 ) {
	
	min_dist2 = dist;
	closest_ch = ch;

      }
    }
    dist = sqrt(dist);
    return closest_ch;
  }

  void Geometry::GetOpChannelPosition(const UInt_t i, Double_t *xyz) const
  {
    if( i >= fOpChannelVtx->size() ) {
      throw LArUtilException(Form("Invalid PMT channel number: %d",i));
      xyz[0] = larlight::DATA::INVALID_DOUBLE;
      xyz[0] = larlight::DATA::INVALID_DOUBLE;
      xyz[0] = larlight::DATA::INVALID_DOUBLE;
      return;
    }

    xyz[0] = fOpChannelVtx->at(i).at(0);
    xyz[1] = fOpChannelVtx->at(i).at(1);
    xyz[2] = fOpChannelVtx->at(i).at(2);
    return;
  }

  const std::vector<Double_t>& Geometry::PlaneOriginVtx(UChar_t plane) const
  {
    if(plane >= fPlaneOriginVtx->size()) {
      throw LArUtilException(Form("Invalid plane number: %d",plane));
      fPlaneOriginVtx->push_back(std::vector<Double_t>(3,larlight::DATA::INVALID_DOUBLE));
      return fPlaneOriginVtx->at(this->Nplanes());
    }

    return fPlaneOriginVtx->at(plane);
  }

  void Geometry::PlaneOriginVtx(UChar_t plane, Double_t *vtx) const
  {
    vtx[0] = fPlaneOriginVtx->at(plane)[0];
    vtx[1] = fPlaneOriginVtx->at(plane)[1];
    vtx[2] = fPlaneOriginVtx->at(plane)[2];
  }  

}


#endif
