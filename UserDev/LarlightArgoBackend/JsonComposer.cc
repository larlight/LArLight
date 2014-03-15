#ifndef LARLIGHTARGOBACKEND_CC
#define LARLIGHTARGOBACKEND_CC

#include "storage_manager.hh"
#include "data_base.hh"
#include "JsonComposer.hh"
#include "JsonElement.h"
#include <cassert>

using namespace larlight;
using std::cout;
using std::endl;

std::string JsonComposer::compose(storage_manager* data, const std::string& options)
{
  assert(data);
  fStorage = data;
  fOptions = options;
  fOutput = JsonObject(); // Reset.
  int doCal = 1;
  int doRaw = 1;
  if( std::string::npos != fOptions.find("_NOCAL_")) doCal = 0;
  if( std::string::npos != fOptions.find("_NORAW_")) doRaw = 0;
    
  composeHeaderData();

  // Wire data.
  // Start first so background image conversion tasks can be started as we build the rest.
  if(!doCal) composeCalAvailability(); // just look at branch names.
  if(!doRaw) composeRawAvailability();
  if(doCal) composeCal();
  if(doRaw) composeRaw();

  //reco
  composeHits();
  composeClusters();
  composeSpacepoints();
  composeTracks();
  
  // Optical
  composeOpPulses();
  composeOpFlashes();
  composeOpHits();
  
  composeAuxDets();
  
  composeMC();
  return fOutput.str();
}

void JsonComposer::composeHeaderData()
{
  // Header data. Alas, this is the stuff that's nearly impossible to get!  
  
  // Find a record, any record.
  std::vector<larlight::DATA::DATA_TYPE> list = fStorage->list_data_types();
  if(list.size()==0) return;
  
  // Get something, anything.
  event_base* thing = fStorage->get_data(list[0]);
  
  JsonObject header;
  header.add("run",thing->run());
  header.add("subrun",thing->subrun());
  header.add("event",thing->event_id());
    
  // Add my own things. 
  // FIXME: this should come from the event data, not be hardcoded, but this will have to do for the moment.
  header.add("TDCStart",0);
  header.add("TDCEnd",3200);
  
  fOutput.add("header",header);
}

// Reco
void  JsonComposer::composeHits()
{
  JsonObject reco_list;
  for(size_t itype = DATA::Hit; itype< DATA::Cluster; itype++) {
    DATA::DATA_TYPE type = (DATA::DATA_TYPE)itype;
    event_hit* hits = dynamic_cast<event_hit*>(fStorage->get_data((DATA::DATA_TYPE)itype));
    if(hits==0) continue;
    std::string name = DATA::DATA_TREE_NAME[itype];
    std::cout << "Looking at hits object " << name << std::endl;   
  
    int nhits = hits->size();
    cout << "nhits: " << nhits << endl;
    std::vector<JsonObject> v;
    
    for(int i=0;i<nhits;i++){
       larlight::hit& h = (*hits)[i];
       JsonObject j;
       j.add("wire",    h.Wire());
       j.add("plane",   2-h.View());
       j.add("view",    h.View());
       j.add("m",       h.Multiplicity());
       j.add("q",       h.Charge());
       j.add("\u03C3q", h.SigmaCharge());
       j.add("t",       h.PeakTime());
       j.add("\u03C3t", h.SigmaPeakTime());
       j.add("t1",      h.StartTime());
       j.add("t2",      h.EndTime());
       v.push_back(j);
    }    
    
    // Look for cluster associations.
    for(size_t itype2 = DATA::Cluster; itype2< DATA::Seed; itype2++) {
      event_cluster* clusters = dynamic_cast<event_cluster*>(fStorage->get_data((DATA::DATA_TYPE)itype2));
      if(!clusters) continue;
      std::string clusname("clusid_");
      clusname += DATA::DATA_TREE_NAME[itype2];

      size_t nclus = clusters->size();
      cout << clusname << "  " << nclus <<  endl;

      for(size_t iclus=0;iclus<nclus;iclus++) {
        const larlight::cluster& clus = (*clusters)[iclus];
        size_t nass = clus.size_association(type);
        if(nass) {
          std::vector<unsigned short> ass = clus.association(type);
          for(size_t iass=0;iass<ass.size();iass++) {
            size_t ihit = ass[iass];
            if(v.size()>ihit) v[ihit].add(clusname,(int)iclus);
          }
        }        
      }
    }
    
    JsonArray arr;
    for(int i=0;i<v.size();i++) arr.add(v[i]);
        
    reco_list.add(name,arr);
  }
  fOutput.add("hits",reco_list);
}
  
void  JsonComposer::composeClusters()
{}
  
void  JsonComposer::composeSpacepoints()
{}
  
void  JsonComposer::composeTracks()
{}

// Optical
void  JsonComposer::composeOpFlashes()
{}
  
void  JsonComposer::composeOpHits()
{}

void  JsonComposer::composeOpPulses()
{}

// Wires
void  JsonComposer::composeCalAvailability()
{}
  
void  JsonComposer::composeRawAvailability()
{}
  
void  JsonComposer::composeCal()
{}
  
void  JsonComposer::composeRaw()
{}

// Other
void JsonComposer::composeAuxDets()
{}


// Monte carlo
void  JsonComposer::composeMC()
{}



#endif
