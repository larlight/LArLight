////////////////////////////////////////////////////////////////////////////
// \version $Id: OpHit.h,v 1.7 2010/03/01 21:32:44 bpage Exp $
//
// \brief Definition of OpHit object for LArSoft
//
// \author bjpjones@mit.edu
//         cschiu@mit.edu
//
////////////////////////////////////////////////////////////////////////////
#ifndef OPHIT_H
#define OPHIT_H

#include "data_base.hh"

namespace larlight {

  /// hits are 2D representations of charge deposited in the tdc/wire plane
  /// hits are assumed to be made from deconvoluted, unipolar signals
  class ophit : public data_base {

  public:
    ophit(); // Default constructor
    
  private:
    
    int                   fOpChannel;
    unsigned short        fFrame;
    double                fPeakTime;
    double                fPeakTimeAbs;
    double                fWidth;
    double                fArea;
    double                fAmplitude;
    double                fPE;
    double                fFastToTotal;
    
  public:
    
    ophit(int opchannel, 
	  double peaktime, 
	  double peaktimeabs,
	  unsigned short frame,
	  double width, 
	  double area, 
	  double peakheight, 
	  double pe,
	  double fasttototal);
    
    // Get Methods
    inline int    OpChannel()      const { return fOpChannel;      }
    unsigned short Frame()         const { return fFrame;          }
    inline double PeakTime()       const { return fPeakTime;       }
    inline double PeakTimeAbs()    const { return fPeakTimeAbs;    }
    inline double Width()          const { return fWidth;          }
    inline double Area()           const { return fArea;           }
    inline double Amplitude()      const { return fAmplitude;      }
    inline double PE()             const { return fPE;             }
    inline double FastToTotal()    const { return fFastToTotal;    }
    
    friend bool           operator <  (const ophit & a, const ophit & b);
    
  private:
    ClassDef(ophit,1)
  };

  class event_ophit : public std::vector<larlight::ophit>,
		      public event_base {

  public:

    /// default ctor
    event_ophit() : event_base(DATA::OpHit) {clear_data();}

    /// Default copy constructor
    event_ophit(const event_ophit& original) : std::vector<larlight::ophit>(original),
					       event_base(original)
    {}

    ~event_ophit() {}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}

  private:

    ClassDef(event_ophit,1)
  };
		      
}

#endif
