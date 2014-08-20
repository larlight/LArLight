#ifndef LARLIGHTARGOBACKEND_CC
#define LARLIGHTARGOBACKEND_CC

#include "storage_manager.hh"
#include "data_base.hh"
#include "JsonComposer.hh"
#include "JsonElement.h"
#include "RootToJson.h"
#include <cassert>

#include <TSystem.h>
#include <TH1D.h>

using namespace larlight;
using std::cout;
using std::endl;

static int events_served = 0;
TTime  ggTimeStart;

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

  long eventTimeStart = gSystem->Now();

  JsonObject source;
  source.add("file","LArLight_file_not_available");
  source.add("selection","LArLight_selection_not_available");
  source.add("start",0);
  source.add("end",0);
  source.add("entry",0);
  source.add("options",options);
  source.add("numEntriesInFile",1e99);
  fOutput.add("source",source);



  JsonObject monitor;
  monitor.add("pid",gSystem->GetPid());
  monitor.add("events_served",events_served);
  SysInfo_t sysinfo;  gSystem->GetSysInfo(&sysinfo);
  // CpuInfo_t cpuinfo;  gSystem->GetCpuInfo(&cpuinfo);
  MemInfo_t meminfo;  gSystem->GetMemInfo(&meminfo);
  ProcInfo_t procinfo; gSystem->GetProcInfo(&procinfo);
  
  monitor.add("OS"           , sysinfo.fOS.Data());
  monitor.add("ComputerModel", sysinfo.fModel.Data());
  monitor.add("CpuType"      ,  sysinfo.fCpuType.Data());
  monitor.add("Cpus"         ,  sysinfo.fCpus);
  monitor.add("CpuSpeed"     ,  sysinfo.fCpuSpeed);
  monitor.add("PhysicalRam"  ,  sysinfo.fPhysRam);

  monitor.add("MemTotal"     ,  Form("%d MB",meminfo.fMemTotal));
  monitor.add("MemUsed"      ,  Form("%d MB",meminfo.fMemUsed));
  monitor.add("MemFree"      ,  Form("%d MB",meminfo.fMemFree));
  monitor.add("SwapTotal"    ,  Form("%d MB",meminfo.fSwapTotal));
  monitor.add("SwapUsed"     ,  Form("%d MB",meminfo.fSwapUsed));
  monitor.add("SwapFree"     ,  Form("%d MB",meminfo.fSwapFree));

  monitor.add("CpuTimeUser"     ,  procinfo.fCpuUser);
  monitor.add("CpuTimeSys"     ,   procinfo.fCpuSys);
  monitor.add("MemResident"    ,   Form("%f MB",procinfo.fMemResident/1000.));
  monitor.add("MemVirtual"     ,   Form("%f MB",procinfo.fMemVirtual/1000.));

  monitor.add("WallClockTime"  ,   ((long)(gSystem->Now() - ggTimeStart))/1000.);
  fOutput.add("backend_monitor",monitor);

    
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
 


  fOutput.add("ElapsedServerTime",((long)(gSystem->Now()) - eventTimeStart));
 
  JsonObject result;
  result.add("record",fOutput);
  result.add("serve_event_log","Boom!");
  return result.str();
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
  header.add("TDCEnd",9600);
  
  fOutput.add("header",header);
}

// Reco
void  JsonComposer::composeHits()
{
  JsonObject hist_list;
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
    
    // Hit histograms.
    TH1D timeProfile("timeProfile","timeProfile",960,0,9600);
    std::vector<TH1*> planeProfile;
    planeProfile.push_back(new TH1D("planeProfile0","planeProfile0",218,0,2398));
    planeProfile.push_back(new TH1D("planeProfile1","planeProfile1",218,0,2398));
    planeProfile.push_back(new TH1D("planeProfile2","planeProfile2",432,0,3456));
    
    for(int i=0;i<nhits;i++){
       larlight::hit& h = (*hits)[i];
       JsonObject j;
       double t = h.PeakTime();
       double q = h.Charge();
       int wire = h.Wire();
       int plane = 2-h.View();
       if(plane==2)timeProfile.Fill(t,q);
       if(plane>=0 && plane<3) planeProfile[plane]->Fill(wire,q);
       
       j.add("wire",    wire);
       j.add("plane",   plane);
       j.add("q",       q);
       j.add("t",       t);
       j.add("t1",      h.StartTime());
       j.add("t2",      h.EndTime());
       // j.add("view",    h.View());
       // j.add("m",       h.Multiplicity());
       // j.add("\u03C3q", h.SigmaCharge());
       // j.add("\u03C3t", h.SigmaPeakTime());
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
    
    JsonObject hists;
    hists.add("timeHist",TH1ToHistogram(&timeProfile));
    JsonArray jPlaneHists;
    jPlaneHists.add(TH1ToHistogram(planeProfile[0]));
    jPlaneHists.add(TH1ToHistogram(planeProfile[1]));
    jPlaneHists.add(TH1ToHistogram(planeProfile[2]));
    hists.add("planeHists",jPlaneHists);

    delete planeProfile[0];
    delete planeProfile[1];
    delete planeProfile[2];    
    hist_list.add(name,hists);
    
  }
  fOutput.add("hits",reco_list);
  fOutput.add("hit_hists",hist_list);
  
}
  
void  JsonComposer::composeClusters()
{
  JsonObject reco_list;
  
  for(size_t itype = DATA::Cluster; itype< DATA::Seed; itype++) {
    DATA::DATA_TYPE type = (DATA::DATA_TYPE)itype;
    event_cluster* clusters = dynamic_cast<event_cluster*>(fStorage->get_data(type));
    if(clusters==0) continue;
    std::string name = DATA::DATA_TREE_NAME[itype];
    std::cout << "Looking at cluster object " << name << std::endl;   
  
    int n = clusters->size();
    cout << "n: " << n << endl;
    JsonArray jClusters;
    
    for(int i=0;i<n;i++){
      const larlight::cluster& clus = (*clusters)[i];
      JsonObject jclus;

      jclus.add("totalCharge",  clus.Charge());
      jclus.add("dTdW"       ,  clus.dTdW());
      jclus.add("dQdW"       ,  clus.dQdW());
      jclus.add("sigmadTdW"  ,  clus.SigmadTdW());
      jclus.add("sigmadQdW"  ,  clus.SigmadQdW());
      jclus.add("ID"         ,  clus.ID());
      jclus.add("view"       ,  clus.View());
  
      jclus.add("startPos"      ,JsonObject().add("wire",clus.StartPos()[0]).add("tdc",clus.StartPos()[1]));
      jclus.add("endPos"        ,JsonObject().add("wire",clus.EndPos()[0]).add("tdc",clus.EndPos()[1]));
      jclus.add("sigmaStartPos" ,JsonObject().add("wire",clus.SigmaStartPos()[0]).add("tdc",clus.SigmaStartPos()[1]));
      jclus.add("sigmaEndPos"   ,JsonObject().add("wire",clus.SigmaEndPos()[0]).add("tdc",clus.SigmaEndPos()[1]));

      for(size_t itype2 = DATA::Hit; itype2< DATA::Cluster; itype2++) {
        DATA::DATA_TYPE type2 = (DATA::DATA_TYPE)itype2;
        event_hit* hits = dynamic_cast<event_hit*>(fStorage->get_data((DATA::DATA_TYPE)itype2));
        if(hits==0) continue;
        std::string name2 = DATA::DATA_TREE_NAME[itype2];
        size_t nass = clus.size_association(type2); // Might be more than one; just look at first one.
        if(nass) {
          std::vector<unsigned short> ass = clus.association(type2);
          jclus.add("hits_"+name2,ass);
        }
      }
      jClusters.add(jclus);
    }    
    
    reco_list.add(name,jClusters);
    
  }
  fOutput.add("clusters",reco_list);
  
  
}
  
void  JsonComposer::composeSpacepoints()
{
  JsonObject reco_list;
  
  for(size_t itype = DATA::SpacePoint; itype< DATA::Track; itype++) {
    DATA::DATA_TYPE type = (DATA::DATA_TYPE)itype;
    event_sps* sps = dynamic_cast<event_sps*>(fStorage->get_data(type));
    if(sps==0) continue;
    std::string name = DATA::DATA_TREE_NAME[type];
    std::cout << "Looking at spacepoint object " << name << std::endl;   
  
    int n = sps->size();
    cout << "n: " << n << endl;
    JsonArray jlist;
    
    for(int i=0;i<n;i++){
      const larlight::spacepoint& sp = (*sps)[i];
        JsonObject jsp;
      
        jsp.add("id"    , sp.ID());
        jsp.add("chisq" , sp.Chisq());
        JsonArray xyz;
        JsonArray errXyz;
        for(int j=0;j<3;j++) {
          xyz   .add(sp.XYZ()[j]);
          errXyz.add(sp.ErrXYZ()[j]);
        }      
        jsp.add("xyz",xyz);
        jsp.add("errXyz",errXyz);
      
        jlist.add(jsp);
    }    
    
    reco_list.add(name,jlist);
  }
  fOutput.add("spacepoints",reco_list);
}
  
void  JsonComposer::composeTracks()
{
  JsonObject reco_list;
  
  for(size_t itype = DATA::Track; itype< DATA::Shower; itype++) {
    DATA::DATA_TYPE type = (DATA::DATA_TYPE)itype;
    event_track* tracks = dynamic_cast<event_track*>(fStorage->get_data(type));
    if(tracks==0) continue;
    std::string name = DATA::DATA_TREE_NAME[type];
    std::cout << "Looking at track object " << name << std::endl;   
  
    int n = tracks->size();
    cout << "n: " << n << endl;
    JsonArray jlist;
    
    for(int i=0;i<n;i++){
      const larlight::track& trk = (*tracks)[i];
        JsonObject jtrk;
      
        jtrk.add("id"    , trk.track_id());
        int npoint = trk.n_point();
        JsonArray jpoints;
        for(int j=0;j<npoint;j++) {
          JsonObject jpoint;
          jpoint.add("x",    trk.vertex_at(j).x() );
          jpoint.add("y",    trk.vertex_at(j).y() );
          jpoint.add("z",    trk.vertex_at(j).z() );
          jpoint.add("vx",   trk.direction_at(j).x() );
          jpoint.add("vy",   trk.direction_at(j).y() );
          jpoint.add("vz",   trk.direction_at(j).z() );
          // jpoint.add("dQdx", trk.direction_at(j).y() );  // Unsupported in larlight?
          // jpoint.add("dQdy", trk.direction_at(j).y() );
          // jpoint.add("dQdz", trk.direction_at(j).y() );
          jpoint.add("P",   trk.momentum_at(j));
          jpoints.add(jpoint);  
        }
        jtrk.add("points",jpoints);      
        jlist.add(jtrk);
    }    
    
    reco_list.add(name,jlist);
  }
  fOutput.add("tracks",reco_list);
}

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
