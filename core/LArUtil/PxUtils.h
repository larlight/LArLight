#include <iostream>
#ifndef UTIL_PXUTILS_H
#define UTIL_PXUTILS_H


namespace larutil{
    class PxPoint {
      public:
        double w; // wire distance in cm
        double t; // time distance in cm (drift distance)
        unsigned int plane; // plane 0, 1, 2
       
        PxPoint(){
            plane=0;
            w=0;
            t=0;
            std::cout<< "This is the default point ctor." << std::endl;
        }
        
        PxPoint(Int_t pp,Double_t ww,Double_t tt){
 	plane=pp;
 	w=ww;
 	t=tt;
       }
        
        ~PxPoint(){}
    };

    class PxHit : public PxPoint {

    public:

      double charge; //charge

        PxHit(){
            PxPoint();
            charge = 0;
            std::cout << "This is the $^_*&ing default ctor." << std::endl;
        }
        ~PxHit(){}
    };
    
    
        //helper class needed for the seeding
    class PxLine {
    public:
      
      PxPoint pt0() { return PxPoint(plane,w0,t0); }
      PxPoint pt1() { return PxPoint(plane,w1,t1); }
      
      Double_t w0; ///<defined to be the vertex w-position
      Double_t t0; ///<defined to be the vertex t-position
      Double_t w1; ///<defined to be the ending w-position (of line or seed depending)
      Double_t t1; ///<defined to be the ending t-position (of line or seed depending)
      UInt_t plane;
   
      PxLine(Int_t pp,Double_t ww0,Double_t tt0, Double_t ww1, Double_t tt1){
	plane=pp;
	w0=ww0;
	t0=tt0;
	w1=ww1;
	t1=tt1;
      }
    
      PxLine(){
	plane=0;
	w0=0;
	t0=0;
	w1=0;
	t1=0;
      }
    
    };
    
}

namespace cluster{

    class cluster_params
    {
    public:
      cluster_params(){}
      ~cluster_params(){}

    public:
      double mean_charge;                // Mean (average) charge of hits in ADC
      double mean_x;                     // Mean of hits along x, peaks only
      double mean_y;                     // Mean of hits along y, peaks only
      double rms_x;                      // rms of hits along x (wires)
      double rms_y;                      // rms of hits along y, (time)
      double charge_wgt_x;               // Mean of hits along x, charge weighted
      double charge_wgt_y;               // Mean of hits along y, charge weighted
      double angle_2d;                   // Angle of axis in wire/hit view
      double opening_angle;              // Width of angular distubtion wrt vertx
      double opening_angle_highcharge;   // Same for high charge 
      double closing_angle;              // Width of angular distubtion wrt endpoint
      double closing_angle_highcharge;   // Same for high charge 
      double eigenvalue_principal;       // the principal eigenvalue from PCA
      double eigenvalue_secondary;       // the secondary eigenvalue from PCA
      double verticalness;               // ???

      double length;
      double width;

      double hit_density_1D;
      double hit_density_2D;
      double multi_hit_wires;
      double N_Wires;
      double modified_hit_density;



      int    direction;                  // +1 means shower is "forward - start wire < end wire"
                                         // -1 is backwards, 0 is undecided
      double showerness;
      double trackness;                  // got soul                                   
      
      double offaxis_hits;

    };
    
    
    
    
}

#endif
