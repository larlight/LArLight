#ifndef RECORDCOMPOSER_H
#define RECORDCOMPOSER_H



#include "JsonElement.h"
#include "TreeReader.h"
#include "TObject.h"
#include <math.h>
#include <vector>

class TTree;
class TreeElementLooter;
class TLorentzVector;

class RecordComposer {
public:
  RecordComposer(JsonObject& output, TTree* tree, Long64_t jentry, const std::string options);
  ~RecordComposer();
  void compose();
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


  // Utility functions.
  std::vector<std::string> findLeafOfType(std::string pattern);
  
  class association { public:
    association() {};
    // Holds an Assn object that links a to b.
    std::string a;
    std::string b;
    std::vector< std::vector<int> > a_to_b;
    std::vector< std::vector<int> > b_to_a;
  };

  std::vector<association> getAssociation(std::string pattern);
  
  void        hsvToRgb(unsigned char* out, float h, float s, float v);
  std::string stripdots(const std::string& s);
  JsonObject  GetClusterWireAndTDC(TreeElementLooter& l, int row);
  void        wireOfChannel(int channel, int& plane, int& wire);
  int         pointOffLine(const TLorentzVector& x0, const TLorentzVector& pv, const TLorentzVector& x, double tol);
  
  
  // Mapping from any adc value onto an 8-bit integer for crude color purposes.
  
  int inline static tanscale(float adc) 
  {
    return int(atan(adc/50.)/M_PI*256.) + 127;  
  }

  float inline static inv_tanscale(int y) 
  {
    return tan((y-127)*M_PI/256)*50.;
  }
  
  
  JsonObject& fOutput; // Top-level output object
  TTree* fTree;
  Long64_t fEntry;
  std::string fOptions;
  TreeReader ftr;
  static std::string sfFileStoragePath;
  static std::string sfUrlToFileStorage;
  
  std::vector<unsigned char> fPalette;
  std::vector<unsigned char> fPaletteTrans;
};



#endif 


