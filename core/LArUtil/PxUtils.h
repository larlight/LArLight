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



#endif
