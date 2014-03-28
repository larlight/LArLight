////////////////////////////////////////////////////////////////////////
//  \file GeometryUtilities.cxx 
//
//  \brief Functions to calculate distances and angles in 3D and 2D
//
// andrzej.szelc@yale.edu
//
////////////////////////////////////////////////////////////////////////

#include "GeometryUtilities.hh"

namespace larutil{

  GeometryUtilities* GeometryUtilities::_me = 0;

  //--------------------------------------------------------------------
  GeometryUtilities::GeometryUtilities() 
  {
    _name = "GeometryUtilities";

    Reconfigure();
  }

  void GeometryUtilities::Reconfigure()
  {
    geom = (larutil::Geometry*)(larutil::Geometry::GetME());
    detp = (larutil::DetectorProperties*)(larutil::DetectorProperties::GetME());
    larp = (larutil::LArProperties*)(larutil::LArProperties::GetME());
    
    fNPlanes = geom->Nplanes();
    vertangle.resize(fNPlanes);
    for(UInt_t ip=0;ip<fNPlanes;ip++)
      vertangle[ip]=geom->WireAngleToVertical(geom->PlaneToView(ip)) - TMath::Pi()/2; // wire angle
        
    fWirePitch = geom->WirePitch(0,1,0);
    fTimeTick=detp->SamplingRate()/1000.; 
    fDriftVelocity=larp->DriftVelocity(larp->Efield(),larp->Temperature());
    
    fWiretoCm=fWirePitch;
    fTimetoCm=fTimeTick*fDriftVelocity;
    fWireTimetoCmCm=(fTimeTick*fDriftVelocity)/fWirePitch;
  }

  //--------------------------------------------------------------------
  GeometryUtilities::~GeometryUtilities() 
  {
    
  }
  

  //-----------------------------------------------------------------------------
  // omega0 and omega1 are calculated as:
  //  angle based on distances in wires and time - rescaled to cm.
  // tan(angle)*fMean_wire_pitch/(fTimeTick*fDriftVelocity);
  // as those calculated with Get2Dangle
  // writes phi and theta in degrees.
  /////////////////////////////////////
  Int_t GeometryUtilities::Get3DaxisN(Int_t iplane0,
				      Int_t iplane1,
				      Double_t omega0, 
				      Double_t omega1,
				      Double_t &phi,
				      Double_t &theta) const
  {
 
    Double_t l(0),m(0),n(0);
    Double_t ln(0),mn(0),nn(0);
    Double_t phis(0),thetan(0);
    //Double_t phin(0);//,phis(0),thetan(0);
    // pretend collection and induction planes. 
    // "Collection" is the plane with the vertical angle equal to zero. 
    // If both are non zero collection is the one with the negative angle. 
    UInt_t Cplane=0,Iplane=1;   
    //then angleC and angleI are the respective angles to vertical in these planes and slopeC, slopeI are the tangents of the track.
    Double_t angleC,angleI,slopeC,slopeI,omegaC,omegaI;
    omegaC = larlight::DATA::INVALID_DOUBLE;
    omegaI = larlight::DATA::INVALID_DOUBLE;
    // don't know how to reconstruct these yet, so exit with error.
    
    if(omega0==0 || omega1==0){
      phi=0;
      theta=-999;
      return -1;
    }
    
    
    //////////insert check for existence of planes.
    
    //check if backwards going track
    Double_t backwards=0;
    
    Double_t alt_backwards=0;
    
    ///// or?
    if(fabs(omega0)>(TMath::Pi()/2.0) && fabs(omega1)>(TMath::Pi()/2.0) ) {
      backwards=1;
    }
    
    if(fabs(omega0)>(TMath::Pi()/2.0) || fabs(omega1)>(TMath::Pi()/2.0) ) {
      alt_backwards=1;
    }
    
    
    
    
    if(vertangle[iplane0] == 0){   
      // first plane is at 0 degrees
      Cplane=iplane0;
      Iplane=iplane1;
      omegaC=omega0;
      omegaI=omega1;
    }
    else if(vertangle[iplane1] == 0){  
      // second plane is at 0 degrees
      Cplane = iplane1;
      Iplane = iplane0;
      omegaC=omega1;
      omegaI=omega0;
    }
    else if(vertangle[iplane0] != 0 && vertangle[iplane1] != 0){
      //both planes are at non zero degree - find the one with deg<0
      if(vertangle[iplane1] < vertangle[iplane0]){
	Cplane = iplane1;
	Iplane = iplane0;
	omegaC=omega1;
	omegaI=omega0;
      }
      else if(vertangle[iplane1] > vertangle[iplane0]){
	Cplane = iplane0;
	Iplane = iplane1;
	omegaC=omega0;
	omegaI=omega1;
      }
      else{
	//throw error - same plane.
	return -1;
      }	
      
    }
    slopeC=tan(omegaC);
    slopeI=tan(omegaI);
    //omega0=tan(omega0);
    //omega1=tan(omega1);
    angleC=vertangle[Cplane];
    angleI=vertangle[Iplane];
    
    //0 -1 factor depending on if one of the planes is vertical.
    bool nfact = !(vertangle[Cplane]);
    
    
    
    if(omegaC < TMath::Pi() && omegaC > 0 )
      ln=1;
    else
      ln=-1;
    
    //std::cout << " slopes, C:"<< slopeC << " " << (omegaC) << " I:" << slopeI << " " << omegaI <<std::endl;
    slopeI=tan(omegaI);
    
    //std::cout << "omegaC, angleC " << omegaC << " " << angleC << "cond: " << omegaC-angleC << " ln: " << ln << std::endl;
    
    l = 1;
    
    
    m = (1/(2*sin(angleI)))*((cos(angleI)/(slopeC*cos(angleC)))-(1/slopeI) 
			     +nfact*(  cos(angleI)/slopeC-1/slopeI  )     );
    
    n = (1/(2*cos(angleC)))*((1/slopeC)+(1/slopeI) +nfact*((1/slopeC)-(1/slopeI)));
    
    mn = (ln/(2*sin(angleI)))*((cos(angleI)/(slopeC*cos(angleC)))-(1/slopeI) 
			       +nfact*(  cos(angleI)/(cos(angleC)*slopeC)-1/slopeI  )     );
    
    nn = (ln/(2*cos(angleC)))*((1/slopeC)+(1/slopeI) +nfact*((1/slopeC)-(1/slopeI)));
    
    
    float Phi;
    float alt_Phi;
    // Direction angles
    if(fabs(angleC)>0.01)  // catch numeric error values 
      {
	phi=atan(n/l);
	//phin=atan(ln/nn);
	phis=asin(ln/TMath::Sqrt(ln*ln+nn*nn));
        
	if(fabs(slopeC+slopeI) < 0.001)
	  phis=0;
	else if(fabs(omegaI)>0.01 && (omegaI/fabs(omegaI) == -omegaC/fabs(omegaC) ) && ( fabs(omegaC) < 20*TMath::Pi()/180 || fabs(omegaC) > 160*TMath::Pi()/180   ) ) // angles have 
	  {phis = (fabs(omegaC) > TMath::Pi()/2) ? TMath::Pi() : 0;    //angles are 
	    
	  }
	
	
	
	if(nn<0 && phis>0 && !(!alt_backwards && fabs(phis)<TMath::Pi()/4 ) )   // do not go back if track looks forward and phi is forward
	  phis=(TMath::Pi())-phis;
	else if(nn<0 && phis<0 && !(!alt_backwards && fabs(phis)<TMath::Pi()/4 ) )
	  phis=(-TMath::Pi())-phis;
	
	
	// solve the ambiguities due to tangent periodicity
	Phi = phi > 0. ? (TMath::Pi()/2)-phi : fabs(phi)-(TMath::Pi()/2) ; 
	alt_Phi = phi > 0. ? (TMath::Pi()/2)-phi : fabs(phi)-(TMath::Pi()/2) ; 
	
	if(backwards==1){
	  if(Phi<0){ Phi=Phi+TMath::Pi();}
	  else if(Phi>0){Phi=Phi-TMath::Pi();}
	}
	
	bool alt_condition=( ( fabs(omegaC)>0.75*TMath::Pi() && fabs(omegaI)>0.166*TMath::Pi() )|| ( fabs(omegaI)>0.75*TMath::Pi() && fabs(omegaC)>0.166*TMath::Pi() ) );
	
	
	if((alt_backwards==1 && alt_condition)   || backwards==1 ){
	  if(alt_Phi<0){alt_Phi=alt_Phi+TMath::Pi();}
	  else if(alt_Phi>0){alt_Phi=alt_Phi-TMath::Pi();}
	}
	
      }
    else  // if plane is collection than Phi = omega
      {phi=omegaC;
	Phi=omegaC;
	phis=omegaC;
	alt_Phi=omegaC;
      }
    
    
    theta = acos( m / (sqrt(pow(l,2)+pow(m,2)+pow(n,2)) ) ) ;
    thetan = -asin ( mn / (sqrt(pow(l,2)+pow(mn,2)+pow(nn,2)) ) ) ;
    //Double_t thetah = acos( mn / (sqrt(pow(l,2)+pow(mn,2)+pow(nn,2)) ) ) ;
    //float Theta;
    //float alt_Theta = 0.;
    
    
    
    
    //if(Phi < 0)Theta = (TMath::Pi()/2)-theta;
    //if(Phi > 0)Theta = theta-(TMath::Pi()/2);
    
    //if(alt_Phi < 0)alt_Theta = (TMath::Pi()/2)-theta;
    //if(alt_Phi > 0)alt_Theta = theta-(TMath::Pi()/2);
    
    ////std::cout << "++++++++ GeomUtil " << Phi*180/TMath::Pi() << " " << Theta*180/TMath::Pi() << std::endl;
    //std::cout << "++++++++ GeomUtil_angles: Phi: " << alt_Phi*180/TMath::Pi() << " Theta: " << alt_Theta*180/TMath::Pi() << std::endl;
    
    //std::cout << "++++++++ GeomUtil_new_angles: Phi: " << phis*180/TMath::Pi() << " Theta: " << thetan*180/TMath::Pi() << std::endl;
    
    phi=phis*180/TMath::Pi();
    theta=thetan*180/TMath::Pi();
    
    
    return 0;   }
  
  //////////////////////////////
  //Calculate theta in case phi~0
  //returns theta in angles
  ////////////////////////////////
  Double_t GeometryUtilities::Get3DSpecialCaseTheta(Int_t iplane0,
						    Int_t iplane1,
						    Double_t dw0, 
						    Double_t dw1) const
  {
  
    Double_t splane,lplane;   // plane in which the track is shorter and longer.
    Double_t sdw,ldw; 
  
    if(dw0==0 && dw1==0)
      return -1;
  
    if(dw0 >= dw1 ) {
      lplane=iplane0; 
      splane=iplane1;
      ldw=dw0;
      sdw=dw1;
    }
    else {
      lplane=iplane1; 
      splane=iplane0;
      ldw=dw1;
      sdw=dw0;
    }
  
    Double_t top=(std::cos(vertangle[splane])-std::cos(vertangle[lplane])*sdw/ldw);
    Double_t bottom = tan(vertangle[lplane]*std::cos(vertangle[splane]) ); 
          bottom -= tan(vertangle[splane]*std::cos(vertangle[lplane]) )*sdw/ldw;
  
    Double_t tantheta=top/bottom;
  
    return atan(tantheta)*vertangle[lplane]/std::abs(vertangle[lplane])*180./TMath::Pi();
  }

  /////////////////////////////////////////////////////////
  //Calculate 3D pitch in beam coordinates
  // 
  /////////////////////////////////////////////////////////
  Double_t GeometryUtilities::CalculatePitch(UInt_t iplane,
					     Double_t phi,
					     Double_t theta) const
  {
    
    Double_t pitch = -1.;
    
    if(geom->PlaneToView(iplane) == larlight::GEO::kUnknown || 
       geom->PlaneToView(iplane) == larlight::GEO::k3D){
      print(larlight::MSG::ERROR,__FUNCTION__,
	    Form("Warning :  no Pitch foreseen for view %d", geom->PlaneToView(iplane)));
      return pitch;
    }
    else{
     
      Double_t pi=TMath::Pi();
      
      Double_t fTheta=pi/2-theta;
     
      Double_t fPhi=-(phi+pi/2);
      //Double_t fPhi=pi/2-phi;
      //if(fPhi<0)
      //	fPhi=phi-pi/2;
     
      //fTheta=TMath::Pi()/2;
 
     
     
      for(UInt_t i = 0; i < geom->Nplanes(); ++i) {
	if(i == iplane){
	  Double_t wirePitch = geom->WirePitch(0,1,i);
	  Double_t angleToVert =0.5*TMath::Pi() - geom->WireAngleToVertical(geom->PlaneToView(i));
	  
	      // 	//    //std::cout <<" %%%%%%%%%%  " << i << " angle " 
	      // 				       << angleToVert*180/pi << " " 
	      // 				       << geom->Plane(i).Wire(0).ThetaZ(false)*180/pi 
	      // 				       <<" wirePitch " << wirePitch
	      // 				       <<"\n %%%%%%%%%%  " << fTheta << " " << fPhi<< std::endl;
	      // 	   
	  
	  Double_t cosgamma = TMath::Abs(TMath::Sin(angleToVert)*TMath::Cos(fTheta)
				       +TMath::Cos(angleToVert)*TMath::Sin(fTheta)*TMath::Sin(fPhi));
	  
	  if (cosgamma>0) pitch = wirePitch/cosgamma;     
	} // end if the correct view
      } // end loop over planes
    } // end if a reasonable view
   
    return pitch;
  }



  /////////////////////////////////////////////////////////
  //Calculate 3D pitch in polar coordinates
  // 
  /////////////////////////////////////////////////////////
  Double_t GeometryUtilities::CalculatePitchPolar(UInt_t iplane,
						  Double_t phi,
						  Double_t theta) const
  {

    Double_t pitch = -1.;
  
    if(geom->PlaneToView(iplane) == larlight::GEO::kUnknown || 
       geom->PlaneToView(iplane) == larlight::GEO::k3D){
      print(larlight::MSG::ERROR,__FUNCTION__,
	    Form("Warning :  no Pitch foreseen for view %d", geom->PlaneToView(iplane)));
      return pitch;
    }
    else{
        
      Double_t fTheta=theta;
      Double_t fPhi=phi;  
     
     
      //fTheta=TMath::Pi()/2;
     
     
     
      for(UInt_t i = 0; i < geom->Nplanes(); ++i){
	if(i == iplane){
	  Double_t wirePitch = geom->WirePitch(0,1,i);
	  Double_t angleToVert =0.5*TMath::Pi() - geom->WireAngleToVertical(geom->PlaneToView(i));
	  
	  // 	    //std::cout <<" %%%%%%%%%%  " << i << " angle " 
	  // 				       << angleToVert*180/pi << " " 
	  // 				       << geom->Plane(i).Wire(0).ThetaZ(false)*180/pi 
	  // 				       <<" wirePitch " << wirePitch
	  // 				       <<"\n %%%%%%%%%%  " << fTheta << " " << fPhi<< std::endl;
	  
	  
	  Double_t cosgamma = TMath::Abs(TMath::Sin(angleToVert)*TMath::Cos(fTheta)
				       +TMath::Cos(angleToVert)*TMath::Sin(fTheta)*TMath::Sin(fPhi));
	  
	  if (cosgamma>0) pitch = wirePitch/cosgamma;     
	} // end if the correct view
      } // end loop over planes
    } // end if a reasonable view
   
    return pitch;
  }




  /////////////////////////////////////////////////////////
  //Calculate 2D slope 
  // in "cm" "cm" coordinates
  /////////////////////////////////////////////////////////
  Double_t GeometryUtilities::Get2Dslope(Double_t wireend,
					 Double_t wirestart,
					 Double_t timeend,
					 Double_t timestart) const
  {
	
    return GeometryUtilities::Get2Dslope((wireend-wirestart)*fWiretoCm,(timeend-timestart)*fTimetoCm);
  
  }

  /////////////////////////////////////////////////////////
  //Calculate 2D slope 
  // in "cm" "cm" coordinates
  /////////////////////////////////////////////////////////
  double GeometryUtilities::Get2Dslope(const larutil::PxPoint *endpoint,
				       const larutil::PxPoint *startpoint) const
  {
    return Get2Dslope(endpoint->w - startpoint->w,endpoint->t - startpoint->t);
  
  }

  /////////////////////////////////////////////////////////
  //Calculate 2D slope 
  // in wire time coordinates coordinates
  // 
  /////////////////////////////////////////////////////////
  Double_t GeometryUtilities::Get2Dslope(Double_t dwire,
					 Double_t dtime) const
  {
 
    //return omega;
 
    return tan(Get2Dangle(dwire,dtime))/fWireTimetoCmCm;

  }


  /////////////////////////////////////////////////////////
  //Calculate 2D angle 
  // in "cm" "cm" coordinates
  /////////////////////////////////////////////////////////
  Double_t GeometryUtilities::Get2Dangle(Double_t wireend,
					 Double_t wirestart,
					 Double_t timeend,
					 Double_t timestart) const
  {

    return Get2Dangle((wireend-wirestart)*fWiretoCm,(timeend-timestart)*fTimetoCm);
  
  }
  
  /////////////////////////////////////////////////////////
  //Calculate 2D angle 
  // in "cm" "cm" coordinates, endpoint and startpoint are assumed to be in cm/cm space
  /////////////////////////////////////////////////////////
  Double_t GeometryUtilities::Get2Dangle(const larutil::PxPoint *endpoint,
					 const larutil::PxPoint *startpoint) const
  {
    return Get2Dangle(endpoint->w - startpoint->w, endpoint->t - startpoint->t);
  
  }
			
  ////////////////////////////
  //Calculate 2D angle 
  // in "cm" "cm" coordinates
  ////////////////////////////
  Double_t GeometryUtilities::Get2Dangle(Double_t dwire,
					 Double_t dtime) const
  {
 
      
    Double_t BC,AC;
    Double_t omega;
 
    BC = ((Double_t)dwire); // in cm
    AC = ((Double_t)dtime); //in cm 
    omega = std::asin(  AC/std::sqrt(pow(AC,2)+pow(BC,2)) );
    if(BC<0)  // for the time being. Will check if it works for AC<0
      { 
	if(AC>0){
	  omega= TMath::Pi()-std::abs(omega);  //
	}
	else if(AC<0){
	  omega=-TMath::Pi()+std::abs(omega);
	}
	else {
	  omega=TMath::Pi();
	}
      } 
    //return omega;
    return omega; //*fWirePitch/(fTimeTick*fDriftVelocity);

  }

  //accepting phi and theta in degrees
  // returning in radians.
  
   double  GeometryUtilities::Get2DangleFrom3D(unsigned int plane,double phi, double theta) const
   {
   TVector3 dummyvector(cos(theta*TMath::Pi()/180.)*sin(phi*TMath::Pi()/180.)  ,sin(theta*TMath::Pi()/180.)  ,  cos(theta*TMath::Pi()/180.)*cos(phi*TMath::Pi()/180.));
    
    return Get2DangleFrom3D(plane,dummyvector);
     
   }
   
   
   // accepting TVector3
   // returning in radians as is customary,
   
   double  GeometryUtilities::Get2DangleFrom3D(unsigned int plane,TVector3 dir_vector) const
  {
   double alpha= 0.5*TMath::Pi()-geom->WireAngleToVertical(geom->PlaneToView(plane)); 
   // create dummy  xyz point in middle of detector and another one in unit length.
   // calculate correspoding points in wire-time space and use the differnces between those to return 2D a
   // angle
 //  std::cout << " alpha angle " <<alpha*180/TMath::Pi() <<std::endl;
   
   TVector3 start(geom->DetHalfWidth(),0.,geom->DetLength()/2.);
   TVector3 end=start+dir_vector;
   
   
    //the wire coordinate is already in cm. The time needs to be converted.
   larutil::PxPoint startp(plane,(geom->DetHalfHeight()*sin(fabs(alpha))+start[2]*cos(alpha)+start[1]*sin(alpha)),start[0]);
   
   larutil::PxPoint endp(plane,(geom->DetHalfHeight()*sin(fabs(alpha))+end[2]*cos(alpha)+end[1]*sin(alpha)),end[0]);
   
   //std::cout <<" points " << startp.w << "," << startp.t << "   "<< endp.w << "," << endp.t << std::endl;
   double angle=Get2Dangle(&endp,&startp);
   
   //std::cout << "calculated angle " << angle*180/TMath::Pi() << std::endl;
   
   return angle;
    
  }
  
  
  
  
  
  //////////////////////////////////////
  //Calculate 2D distance 
  // in "cm" "cm" coordinates
  ////////////////////////////////////////
  Double_t GeometryUtilities::Get2DDistance(Double_t wire1,
					    Double_t time1,
					    Double_t wire2,
					    Double_t time2) const
  {
    
    return TMath::Sqrt( pow((wire1-wire2)*fWiretoCm,2)+pow((time1-time2)*fTimetoCm,2) );	
  
  }

  
   double GeometryUtilities::Get2DDistance(const larutil::PxPoint *point1,
					   const larutil::PxPoint *point2) const	
  {
    return TMath::Sqrt( pow((point1->w - point2->w),2)+pow((point1->t - point2->t),2) );	
  }

  
  ////////////////////////////
  //Calculate 2D distance, using 2D angle 
  // in "cm" "cm" coordinates
  ////////////////////////////
  Double_t GeometryUtilities::Get2DPitchDistance(Double_t angle,
						 Double_t inwire,
						 Double_t wire) const
  {
    Double_t radangle = TMath::Pi()*angle/180;
    if(std::cos(radangle)==0)
      return 9999;
    return std::abs((wire-inwire)/std::cos(radangle))*fWiretoCm; 
  }


  //----------------------------------------------------------------------------
  Double_t GeometryUtilities::Get2DPitchDistanceWSlope(Double_t slope,
						       Double_t inwire,
						       Double_t wire) const
  {
  
    return std::abs(wire-inwire)*TMath::Sqrt(1+slope*slope)*fWiretoCm; 
   
  }




  ///////////////////////////////////
  //Calculate wire,time coordinates of the Hit projection onto a line
  // 
  ///////////////////////////////////
  Int_t GeometryUtilities::GetPointOnLine(Double_t slope,
					  Double_t intercept,
					  Double_t wire1,
					  Double_t time1,
					  Double_t &wireout,
					  Double_t &timeout) const
  {
    Double_t invslope=0;
      
    if(slope)	
      {
	invslope=-1./slope;
      }
  
    Double_t ort_intercept=time1-invslope*(Double_t)wire1;
    
    if((slope-invslope)!=0)
      wireout=(ort_intercept - intercept)/(slope-invslope); 
    else
      wireout=wire1;
    timeout=slope*wireout+intercept;   
    
    return 0;
  }
    
  //////////////////////////////////
  //Calculate wire,time coordinates of the Hit projection onto a line
  //  all points are assumed to be in cm/cm space.
  ///////////////////////////////////
  int GeometryUtilities::GetPointOnLine(Double_t slope,
					const larutil::PxPoint *startpoint,
					const larutil::PxPoint *point1,
					larutil::PxPoint &pointout) const
  {
    
    double intercept=startpoint->t - slope * startpoint->w;  
    
    
    return GetPointOnLine(slope,intercept,point1,pointout);
    
  }
  
  
  ///////////////////////////////////
  //Calculate wire,time coordinates of the Hit projection onto a line
  //  all points assumed to be in cm/cm space.
  ///////////////////////////////////
  int GeometryUtilities::GetPointOnLine(double slope,
					double intercept,
					const larutil::PxPoint *point1,
					larutil::PxPoint &pointout) const
  {
    double invslope=0;
      
    if(slope)	
      {
	invslope=-1./slope*fWireTimetoCmCm*fWireTimetoCmCm;
      }
  
    double ort_intercept=point1->t - invslope * point1->w;
    
    if((slope-invslope)!=0)
      pointout.w = (ort_intercept - intercept)/(slope-invslope); 
    else
      pointout.w = point1->w;
    
    pointout.t = slope * pointout.w + intercept;   
    
    return 0;
  }
    
  ///////////////////////////////////
  //Calculate wire,time coordinates of the Hit projection onto a line
  // 
  ///////////////////////////////////    
  Int_t GeometryUtilities::GetPointOnLine(Double_t slope,
					  Double_t wirestart,
					  Double_t timestart,
					  Double_t wire1,
					  Double_t time1,
					  Double_t &wireout,
					  Double_t &timeout) const
  {
    Double_t intercept=timestart-slope*(Double_t)wirestart;
  
    return GetPointOnLine(slope,intercept,wire1,time1,wireout,timeout);
  }

  ///////////////////////////////////
  //Calculate wire,time coordinates of the Hit projection onto a line
  // 
  ///////////////////////////////////
  Int_t GeometryUtilities::GetPointOnLineWSlopes(Double_t slope,
						 Double_t intercept,
						 Double_t ort_intercept,
						 Double_t &wireout,
						 Double_t &timeout) const
  {
    Double_t invslope=0;
  
    if(slope)	
	{
		invslope=-1./slope;
	}
    
    invslope*=fWireTimetoCmCm*fWireTimetoCmCm;
  	
    wireout=(ort_intercept - intercept)/(slope-invslope); 
    timeout=slope*wireout+intercept; 
  
    
    wireout/=fWiretoCm;
    timeout/=fTimetoCm;
    
    return 0;  
  }    

  
  ///////////////////////////////////
  //Calculate wire,time coordinates of the Hit projection onto a line
  // slope should be in cm/cm space. PxPoint should be in cm/cm space.
  ///////////////////////////////////
  Int_t GeometryUtilities::GetPointOnLineWSlopes(double slope,
						 double intercept,
						 double ort_intercept,
						 larutil::PxPoint &pointonline) const
  {
    Double_t invslope=0;
  
    if(slope)	

	invslope=-1./slope;

    invslope *= fWireTimetoCmCm * fWireTimetoCmCm;
  	
    pointonline.w = (ort_intercept - intercept)/(slope-invslope); 
    pointonline.t = slope * pointonline.w + intercept; 
    return 0;  
  }    
  
  
  
  ///////////////////////////////////
  //Find hit closest to wire,time coordinates
  // 
  ////////////////////////////////////////////////
  const larlight::hit* GeometryUtilities::FindClosestHit(const std::vector<larlight::hit*> &hitlist,
							 UInt_t wirein,
							 Double_t timein) const
  {
    return hitlist.at(FindClosestHitIndex(hitlist,wirein,timein));
  }
  
  
  //Find hit closest to wire,time coordinates
  // 
  ////////////////////////////////////////////////
  UInt_t GeometryUtilities::FindClosestHitIndex(const std::vector<larlight::hit*> &hitlist,
						UInt_t wirein,
						Double_t timein) const
  {
  
    Double_t min_length_from_start=larlight::DATA::INVALID_DOUBLE;
    UInt_t index=larlight::DATA::INVALID_UINT;
   
    UInt_t plane,wire;
   
   
    for(UInt_t ii=0; ii<hitlist.size();ii++){

      Double_t time = hitlist.at(ii)->PeakTime();  
      GetPlaneAndTPC(hitlist.at(ii),plane,wire);
      
      Double_t dist_mod=Get2DDistance(wirein,timein,wire,time);

      if(dist_mod<min_length_from_start){
	//wire_start[plane]=wire;
	//time_start[plane]=time;
	index = ii;
	min_length_from_start=dist_mod;
      }	
      
    } 
  
    return index;
  }
  
  
//     //Find hit closest to wire,time coordinates
//   // 
//   ////////////////////////////////////////////////
//   art::Ptr< recob::Hit > GeometryUtilities::FindClosestHitEvdPtr(std::vector<art::Ptr< recob::Hit > > hitlist,
// 						 UInt_t wirein,
// 						 Double_t timein) const
//   {
//   
//     Double_t min_length_from_start=99999;
//     art::Ptr< recob::Hit > nearHit;
//    
//     UInt_t plane,tpc,wire,cstat;
//    
//    
//     for(UInt_t ii=0; ii<hitlist.size();ii++){
//       recob::Hit * theHit = const_cast<recob::Hit *>(hitlist[ii].get());
//       Double_t time = theHit->PeakTime() ;  
//       GetPlaneAndTPC(theHit,plane,cstat,tpc,wire);
//     
//       Double_t dist_mod=Get2DDistance(wirein,timein,wire,time);
// 
//       if(dist_mod<min_length_from_start){
// 	//wire_start[plane]=wire;
// 	//time_start[plane]=time;
// 	nearHit=(hitlist[ii]);
// 	min_length_from_start=dist_mod;
//       }	
// 
//     } 
//   
//     return nearHit;    
//   }
//   
//   
//   
  
  
  
  //////////////////////////////////////////////////////////
  Int_t GeometryUtilities::GetProjectedPoint(const PxPoint *p0, 
					     const PxPoint *p1, 
					     PxPoint &pN) const
  {

    //determine third plane number
    for(UInt_t i = 0; i < fNPlanes; ++i){
      if(i == p0->plane || i == p1->plane)
	continue;   
      pN.plane = i;
    }
  
    // Assuming there is no problem ( and we found the best pair that comes close in time )
    // we try to get the Y and Z coordinates for the start of the shower. 
    UInt_t chan1 = geom->PlaneWireToChannel(p0->plane,p0->w);
    UInt_t chan2 = geom->PlaneWireToChannel(p1->plane,p1->w);
 
    Double_t pos[3]={0.};
    geom->PlaneOriginVtx(p0->plane, pos);
 
    Double_t x=(p0->t - detp->TriggerOffset())*fTimetoCm+pos[0];
 
    Double_t y,z;
    if(! geom->ChannelsIntersect(chan1,chan2,y,z) )
      return -1;
 
 
    pos[1]=y;
    pos[2]=z;
    pos[0]=x;
    
    pN=Get2DPointProjection(pos, pN.plane);
       
    return 0;  
  }


  //////////////////////////////////////////////////////////
  Int_t GeometryUtilities::GetYZ(const PxPoint *p0,
				 const PxPoint *p1,
				 Double_t* yz) const
  {
    Double_t y,z;
  
    UInt_t chan1 = geom->PlaneWireToChannel(p0->plane, p0->w);
    UInt_t chan2 = geom->PlaneWireToChannel(p1->plane, p1->w);

    if(! geom->ChannelsIntersect(chan1,chan2,y,z) )
      return -1;
  
    yz[0]=y;
    yz[1]=z;
  
    return 0;
  }

  //////////////////////////////////////////////////////////////
  
  PxPoint GeometryUtilities::Get2DPointProjection(Double_t *xyz, Int_t plane) const{
  
    PxPoint pN(0,0,0);
    
    Double_t pos[3];
    geom->PlaneOriginVtx(plane,pos);
    Double_t drifttick=(xyz[0]/fDriftVelocity)*(1./fTimeTick);
      
    pos[1]=xyz[1];
    pos[2]=xyz[2];

    ///\todo: this should use the cryostat and tpc as well in the NearestWire method
    
    pN.w = geom->NearestWire(pos, plane);
    pN.t=drifttick-(pos[0]/fDriftVelocity)*(1./fTimeTick)+detp->TriggerOffset();  
    pN.plane=plane;
    
    return pN;
     
   }
  
  
    //////////////////////////////////////////////////////////////
    // for now this returns the vlause in CM/CM space.
    // this will become the default, but don't want to break the code that depends on the 
    // previous version. A.S. 03/26/14
    //////////////////////////////////////
  
  PxPoint GeometryUtilities::Get2DPointProjectionCM(std::vector< double > xyz, int plane) const{
  
    PxPoint pN(0,0,0);
    
    Double_t pos[3];
        
      
    pos[1]=xyz[1];
    pos[2]=xyz[2];

    ///\todo: this should use the cryostat and tpc as well in the NearestWire method
    
    pN.w = geom->NearestWire(pos, plane)*fWiretoCm;
    pN.t=xyz[0];  
    pN.plane=plane;
    
    return pN;
     
   }
  
  
  

  Double_t GeometryUtilities::GetTimeTicks(Double_t x, Int_t plane) const{
  
   
    
    Double_t pos[3];
    geom->PlaneOriginVtx(plane,pos);
    Double_t drifttick=(x/fDriftVelocity)*(1./fTimeTick);
    
    return drifttick-(pos[0]/fDriftVelocity)*(1./fTimeTick)+detp->TriggerOffset();  
    
     
   }
  
  
  
  
  //----------------------------------------------------------------------
  // provide projected wire pitch for the view // copied from track.cxx and modified
  Double_t GeometryUtilities::PitchInView(UInt_t plane,
					  Double_t phi,
					  Double_t theta) const
  {
    Double_t dirs[3] = {0.};
    GetDirectionCosines(phi,theta,dirs); 
    
    /// \todo switch to using new Geometry::WireAngleToVertical(geo::View_t) 
    /// \todo and Geometry::WirePitch(geo::View_t) methods
    Double_t wirePitch   = 0.;
    Double_t angleToVert = 0.;
   
    wirePitch = geom->WirePitch(0,1,plane);
    angleToVert = geom->WireAngleToVertical(geom->PlaneToView(plane)) - 0.5*TMath::Pi();
         
    //(sin(angleToVert),std::cos(angleToVert)) is the direction perpendicular to wire
    //fDir.front() is the direction of the track at the beginning of its trajectory
    Double_t cosgamma = TMath::Abs(TMath::Sin(angleToVert)*dirs[1] + 
				      TMath::Cos(angleToVert)*dirs[2]);
   
    //   //std::cout << " ---- cosgamma: " << angleToVert*180/TMath::Pi() << " d's: " << dirs[1]
    //  << " " << dirs[2] << " ph,th " << phi << " " << theta << std::endl; 
    if(cosgamma < 1.e-5) 
      throw LArUtilException("cosgamma is basically 0, that can't be right");
    
    return wirePitch/cosgamma;
  }

  
  //////////////////////////////////////////////////
  void GeometryUtilities::GetDirectionCosines(Double_t phi,
					      Double_t theta,
					      Double_t *dirs) const
  {
    theta*=(TMath::Pi()/180);
    phi*=(TMath::Pi()/180); // working on copies, it's ok.
    dirs[0]=TMath::Cos(theta)*TMath::Sin(phi);
    dirs[1]=TMath::Sin(theta);
    dirs[2]=TMath::Cos(theta)*TMath::Cos(phi);
   
  }


  //////////////////////////////////////////
  Int_t GeometryUtilities::GetPlaneAndTPC(const larlight::hit* h,
					  UInt_t &p,
					  UInt_t &w) const
  {
    p  = geom->ChannelToPlane(h->Channel());
    w  = geom->ChannelToWire(h->Channel());
    return 0;
  }

  
  void GeometryUtilities::SelectLocalHitlist(const std::vector< larlight::hit*> &hitlist, 
					     std::vector<UInt_t> &hitlistlocal_index,
					     Double_t  wire_start,Double_t time_start, 
					     Double_t linearlimit,   Double_t ortlimit, 
					     Double_t lineslopetest)
  {
    
    Double_t locintercept=time_start-wire_start*lineslopetest;
    
    for(size_t i=0; i<hitlist.size(); ++i) {

      Double_t time = hitlist.at(i)->PeakTime();
      UInt_t plane,wire;
      GetPlaneAndTPC(hitlist.at(i),plane,wire);
      
      Double_t wonline=wire,tonline=time;
      //gser.GetPointOnLine(lineslopetest,lineinterctest,wire,time,wonline,tonline);
      GetPointOnLine(lineslopetest,locintercept,wire,time,wonline,tonline);
      
      //calculate linear distance from start point and orthogonal distance from axis
      Double_t lindist=Get2DDistance(wonline,tonline,wire_start,time_start);
      Double_t ortdist=Get2DDistance(wire,time,wonline,tonline);
      
      ////////std::cout << " w,t: " << wire << " " << time << " ws,ts " << wonline << " "<< tonline <<" "<< lindist << " " << ortdist << std::endl;
      
      if(lindist<linearlimit && ortdist<ortlimit)

	{ hitlistlocal_index.push_back((UInt_t)i);
	  //std::cout << " w,t: " << wire << " " << time << " calc time: " << wire*lineslopetest + locintercept  << " ws,ts " << wonline << " "<< tonline <<" "<< lindist << " " << ortdist  << " plane: " << plane << std::endl;
	}
      
      
    }
  }

  void GeometryUtilities::SelectLocalHitlist(const std::vector< larlight::hit*> &hitlist, 
					     std::vector<larlight::hit*> &hitlistlocal,
					     Double_t  wire_start,Double_t time_start, 
					     Double_t linearlimit,   Double_t ortlimit, 
					     Double_t lineslopetest)
  {
    
    Double_t locintercept=time_start-wire_start*lineslopetest;
    
    for(size_t i=0; i<hitlist.size(); ++i) {

      Double_t time = hitlist.at(i)->PeakTime();
      UInt_t plane,wire;
      GetPlaneAndTPC(hitlist.at(i),plane,wire);
      
      Double_t wonline=wire,tonline=time;
      //gser.GetPointOnLine(lineslopetest,lineinterctest,wire,time,wonline,tonline);
      GetPointOnLine(lineslopetest,locintercept,wire,time,wonline,tonline);
      
      //calculate linear distance from start point and orthogonal distance from axis
      Double_t lindist=Get2DDistance(wonline,tonline,wire_start,time_start);
      Double_t ortdist=Get2DDistance(wire,time,wonline,tonline);
      
      ////////std::cout << " w,t: " << wire << " " << time << " ws,ts " << wonline << " "<< tonline <<" "<< lindist << " " << ortdist << std::endl;
      
      if(lindist<linearlimit && ortdist<ortlimit)

	{ hitlistlocal.push_back(hitlist.at(i));
	  //std::cout << " w,t: " << wire << " " << time << " calc time: " << wire*lineslopetest + locintercept  << " ws,ts " << wonline << " "<< tonline <<" "<< lindist << " " << ortdist  << " plane: " << plane << std::endl;
	}
      
      
    }
    
  }

  void GeometryUtilities::SelectLocalHitlist(const std::vector<larutil::PxHit> &hitlist, 
					     std::vector <const larutil::PxHit*> &hitlistlocal,
					     larutil::PxPoint &startHit,
					     Double_t& linearlimit,   
					     Double_t& ortlimit, 
					     Double_t& lineslopetest,
					     larutil::PxHit &averageHit) {

    
    double locintercept=startHit.t - startHit.w * lineslopetest;

    double timesum = 0;
    double wiresum = 0;
    for(size_t i=0; i<hitlist.size(); ++i) {

      
      larutil::PxPoint hitonline;
      
      GetPointOnLine( lineslopetest, locintercept, (const larutil::PxHit*)(&hitlist.at(i)), hitonline );
       
      //calculate linear distance from start point and orthogonal distance from axis
      Double_t lindist=Get2DDistance((const larutil::PxPoint*)(&hitonline),(const larutil::PxPoint*)(&startHit));
      Double_t ortdist=Get2DDistance((const larutil::PxPoint*)(&hitlist.at(i)),(const larutil::PxPoint*)(&hitonline));
      
      
      if(lindist<linearlimit && ortdist<ortlimit)  {
	hitlistlocal.push_back((const larutil::PxHit*)(&(hitlist.at(i))));
        timesum += hitlist.at(i).t;
	wiresum += hitlist.at(i).w;
      }
      
      
    }

    averageHit.plane = startHit.plane;
    if(hitlistlocal.size())
    {
      averageHit.w = wiresum/(double)hitlistlocal.size();
      averageHit.t = timesum/((double) hitlistlocal.size());
    }
  }
  


} // namespace
