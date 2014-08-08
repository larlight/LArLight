/**
 * \file CoreyShowerStudy.hh
 *
 * \ingroup CoreyShowerStudy
 * 
 * \brief Class def header for a class CoreyShowerStudy
 *
 * @author cja33
 */

/** \addtogroup CoreyShowerStudy

    @{*/
#ifndef COREYSHOWERSTUDY_HH
#define COREYSHOWERSTUDY_HH

#include <iostream>
#include "ana_base.hh"
#include "TVector3.h"
/**
   \class CoreyShowerStudy
   User defined class CoreyShowerStudy ... these comments are used to generate
   doxygen documentation!
 */
namespace shower{
  class CoreyShowerStudy{

  public:

    /// Default constructor
    CoreyShowerStudy(){
      contain50VsEnergy = new TH2D("contain50VsEnergy","contain50VsEnergy",
                                   50,0,2,nbins,0,250);
      contain75VsEnergy = new TH2D("contain75VsEnergy","contain75VsEnergy",
                                   50,0,2,nbins,0,250);
      contain90VsEnergy = new TH2D("contain90VsEnergy","contain90VsEnergy",
                                   50,0,2,nbins,0,250);
    };

    /// Default destructor
    virtual ~CoreyShowerStudy(){};

    int LoadAllShowers(const larlight::event_mcshower *showers);

    TH1F * GetChargeProfile(){return chargeProfile;}

    TH2D * GetContain50VsEnergy(){return contain50VsEnergy;}
    TH2D * GetContain75VsEnergy(){return contain75VsEnergy;}
    TH2D * GetContain90VsEnergy(){return contain90VsEnergy;}
  private:

    TH1F * chargeProfile;
    TH2D * contain50VsEnergy;
    TH2D * contain75VsEnergy;
    TH2D * contain90VsEnergy;

    const static int nbins = 100;

  };
}
#endif
/** @} */ // end of doxygen group 

