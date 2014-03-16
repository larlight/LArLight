/**
 * \file LarlightArgoBackend.hh
 *
 * \ingroup LarlightArgoBackend
 * 
 * \brief Class def header for a class JsonComposer
 *
 * @author tagg
 */

/** \addtogroup LarlightArgoBackend

    @{*/
#ifndef JSONCOMPOSER_HH
#define JSONCOMPOSER_HH


#include <iostream>
#include <string>
#include "storage_manager.hh"
#include "JsonElement.h"

/**
   \class JsonComposer
   User defined class LarlightArgoBackend ... these comments are used to generate
   doxygen documentation!
 */
class JsonComposer{

public:

  /// Default constructor
  JsonComposer(){};

  /// Default destructor
  virtual ~JsonComposer(){};

  virtual std::string compose(larlight::storage_manager* data, const std::string& options="");
  
private:
  void  composeHeaderData();

  // Reco
  void  composeHits();
  void  composeClusters();
  void  composeSpacepoints();
  void  composeTracks();

  // Optical
  void  composeOpFlashes();
  void  composeOpHits();
  void  composeOpPulses();

  // Wires
  void  composeCalAvailability(); 
  void  composeRawAvailability(); 
  void  composeCal();
  void  composeRaw();

  // Other
  void composeAuxDets();

  
  // Monte carlo
  void  composeMC();

#ifndef __CINT__
  int inline static tanscale(float adc) 
  {
    return int(atan(adc/50.)/M_PI*256.) + 127;  
  }

  float inline static inv_tanscale(int y) 
  {
    return tan((y-127)*M_PI/256)*50.;
  }
#endif

  // temporary state:
  JsonObject       fOutput;
  larlight::storage_manager* fStorage; // NB temporary pointer
  std::string      fOptions;
  std::vector<unsigned char> fPalette;
  std::vector<unsigned char> fPaletteTrans;
  
};

#endif
/** @} */ // end of doxygen group 

