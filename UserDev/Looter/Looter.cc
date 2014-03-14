#ifndef LOOTER_CC
#define LOOTER_CC

#include <cassert>
#include "Looter.hh"

#include "TreeElementLooter.h"
#include "hit.hh"
#include "cluster.hh"

#include "TFile.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TTreeFormula.h"





using namespace std;

larlight::DATA::DATA_TYPE associationNameToType(const std::string& name);
larlight::DATA::DATA_TYPE nameToType(const std::string& name);


Looter::Looter(std::string filename)
  : _good(true)
  , _filename(filename)
  , _ftr(0)
{
  _file = new TFile(_filename.c_str());
  if(_file->IsZombie()) {
    std::cout << "Could not open input file " << filename << " for reading." << std::endl;
    _good = false;
    return;
  }
  
  _tree = dynamic_cast<TTree*>(_file->Get("Events"));
  if(!_tree) {
    std::cout << "Input file " << filename << " does not have an Events tree" << std::endl;
    _good = false;
    return;    
  }
  
  _ftr = new TreeReader(_tree);
}

Looter::~Looter()
{
  if(_ftr) delete _ftr;
  if(_file) delete _file;
}

int Looter::loot(larlight::storage_manager& storage, Long64_t start_event, Long64_t nevents)
{
  if(!_good) return 0;
  
  Long64_t n = _tree->GetEntries() - start_event;
  if(nevents>0 && nevents<n) n = nevents;
  
  // Set branches to read here.
  _tree->SetBranchStatus("*",1);  // By default, read all.
  _tree->SetBranchStatus("raw::RawDigits*",0); // Speed!
  _tree->SetBranchStatus("recob::Wires*"  ,0); // Speed!
  
  int events_read =0;
  for(_entry = start_event;_entry < n; _entry ++) {
    Int_t bytesRead = _tree->GetEntry(_entry);
    if(bytesRead<0) {
      std::cout << "Error: I/O error on GetEntry trying to read entry " << _entry;
      return events_read;
    }
    if(bytesRead==0) {
      cout << "Error: Nonexistent entry reported by GetEntry trying to read entry " << _entry;
      return events_read;
    }
    
    _run    = _ftr->getF("EventAuxiliary.id_.subRun_.run_.run_");
    _subrun = _ftr->getF("EventAuxiliary.id_.subRun_.subRun_");
    _event  = _ftr->getF("EventAuxiliary.id_.event_");

    read_hits(storage);
    read_clusters(storage);
    
    
    // Only when we are done..
    storage.next_event();    
    events_read++;
  }
  
  return events_read;  
}


void Looter::read_hits(larlight::storage_manager& storage)
{
  vector<string> leafnames = findLeafOfType("vector<recob::Hit>>");
  if(leafnames.size()==0) {
    std::cout << "No hit branch found in file.";
    return;
  } 
    
  // For now, just pick the first one.
  leafnames.resize(1); // throw them all away.
  
  for(size_t iname = 0; iname<leafnames.size(); iname++) {
    std::string name = leafnames[iname];
    std::cout << "Looking at hits object " << (name+"obj_").c_str() << endl;   
  
    larlight::event_hit* outHits = (larlight::event_hit*)(storage.get_data(larlight::DATA::Hit));
    outHits->set_run(_run);
    outHits->set_subrun(_subrun);
    outHits->set_event_id(_event);
    
    TLeaf* l = _tree->GetLeaf((name+"obj_").c_str());
    if(!l) continue;
    int nhits = l->GetLen();
    cout << "nhits: " << nhits << endl;
    
    TLeaf* lkey           = _tree->GetLeaf((name+"obj.fRawDigit.key_"   ).c_str());
    TLeaf* lwire          = _tree->GetLeaf((name+"obj.fWireID.Wire"     ).c_str());
    // TLeaf* lplane         = _tree->GetLeaf((name+"obj.fWireID.Plane"    ).c_str());
    TLeaf* lview          = _tree->GetLeaf((name+"obj.fView"            ).c_str());
    TLeaf* lmult          = _tree->GetLeaf((name+"obj.fMultiplicity"    ).c_str());
    TLeaf* lcharge        = _tree->GetLeaf((name+"obj.fCharge"          ).c_str());
    TLeaf* lmaxcharge     = _tree->GetLeaf((name+"obj.fMaxCharge"       ).c_str());
    TLeaf* lsigcharge     = _tree->GetLeaf((name+"obj.fSigmaCharge"     ).c_str());
    TLeaf* lsigmaxcharge  = _tree->GetLeaf((name+"obj.fSigmaMaxCharge"  ).c_str());
    TLeaf* lpeaktime      = _tree->GetLeaf((name+"obj.fPeakTime"        ).c_str());
    TLeaf* lsigtime       = _tree->GetLeaf((name+"obj.fSigmaPeakTime"   ).c_str());
    TLeaf* lstarttime     = _tree->GetLeaf((name+"obj.fStartTime"       ).c_str());
    TLeaf* lendtime       = _tree->GetLeaf((name+"obj.fEndTime"         ).c_str());
    TLeaf* lsigstarttime  = _tree->GetLeaf((name+"obj.fSigmaStartTime"  ).c_str());
    TLeaf* lsigendtime    = _tree->GetLeaf((name+"obj.fSigmaEndTime"    ).c_str());
    TLeaf* lgof           = _tree->GetLeaf((name+"obj.fGoodnessOfFit"   ).c_str());
    TLeaf* lsigtype       = _tree->GetLeaf((name+"obj.fSignalType"      ).c_str());
    for(int i=0;i<nhits;i++) {
      larlight::hit h;
      // h.set_hit_id(i);
      //  h.set_waveform(const std::vector<Double_t>& wf);
      h.set_times(_ftr->getVal(lstarttime,i),
                  _ftr->getVal(lpeaktime,i),
                  _ftr->getVal(lendtime,i));
      h.set_times_err(_ftr->getVal(lsigstarttime,i),
                      _ftr->getVal(lsigtime,i),
                      _ftr->getVal(lsigendtime,i));
      h.set_charge  (_ftr->getVal(lcharge,i),_ftr->getVal(lmaxcharge,i));
      h.set_charge_err(_ftr->getVal(lsigcharge,i),_ftr->getVal(lsigmaxcharge,i));
      h.set_multiplicity(_ftr->getInt(lmult,i));
      h.set_channel (_ftr->getInt(lkey,i)); // Not sure of this one.
      int wire = _ftr->getInt(lwire,i);
      if(wire>=0) h.set_wire(wire);
      h.set_fit_goodness(_ftr->getVal(lgof,i));
      h.set_view((larlight::GEO::View_t)(_ftr->getInt(lview,i)));
      h.set_sigtype((larlight::GEO::SigType_t)(_ftr->getInt(lsigtype,i)));
      outHits->push_back(h);
    }

   }
}

void Looter::read_clusters(larlight::storage_manager& storage)
{
  vector<string> leafnames = findLeafOfType("vector<recob::Cluster>>");
  if(leafnames.size()==0) {
    cout << "No cluster branch found in file." << endl;
    return;
  } 
  
  leafnames.resize(1); // Truncate to 1 for now.
  for(size_t iname = 0; iname<leafnames.size(); iname++) {    
    std::string name = leafnames[iname];
    larlight::DATA::DATA_TYPE type = larlight::DATA::Cluster;
    // FIXME Change type based on name.
    std::cout << "Looking at cluster object " << (name+"obj_").c_str() << endl;
    TLeaf* l = _tree->GetLeaf((name+"obj_").c_str());
    if(!l) continue;
    int nclusters = l->GetLen();
    cout << "clusters: " << nclusters << endl;
    TreeElementLooter startPos     (_tree,name+"obj.fStartPos");
    TreeElementLooter endPos       (_tree,name+"obj.fEndPos");
    TreeElementLooter sigmaStartPos(_tree,name+"obj.fSigmaStartPos");
    TreeElementLooter sigmaEndPos  (_tree,name+"obj.fSigmaEndPos");


    // Attempt to find association data to hits.
    std::map<int,std::vector<unsigned short> > map_to_hits;
    //
    // I would like to also put this stuff here, instead of just in the hits area.
    // Also, I really should support multiple hit lists...
    //
    
    vector<string> assnames = findLeafOfType("art::Wrapper<art::Assns<recob::Cluster,recob::Hit");
    for(size_t iass=0;iass<assnames.size(); iass++) {
      std::string assname = assnames[iass];
      cout << "Finding association to clusters " << assname << endl;
      size_t u1 = assname.find_first_of("_");
      size_t v1 = name.find_first_of("_");
      std::cout << "Looking for match between " << name << " and " << assname << endl;
      if(assname.substr(u1+1)!=name.substr(v1+1)) {
        continue;
      }
      std::cout << "---> Got a match." << endl;
    
      // Attempt to pull association data.
      TTreeFormula forma("a",std::string(assname+".obj.ptr_data_1_.second").c_str(),_tree);
      TTreeFormula formb("b",std::string(assname+".obj.ptr_data_2_.second").c_str(),_tree);
      int n  = forma.GetNdata();
               formb.GetNdata(); // need this line to goose formula into evaluating
      // cout << shortname << "  Association formula has " << n << " entries" << endl;
      // cout << shortname << " Association formula b has " << formb.GetNdata() << " entries" << endl;
      for(Int_t i=0;i<n;i++) {
        int cluster_id = forma.EvalInstance(i);
        int hit_id     = formb.EvalInstance(i);
        map_to_hits[cluster_id].push_back(hit_id);
      }    
    }
    
    // 
    // Actually build it:
    //
    larlight::event_cluster* clusters = dynamic_cast<larlight::event_cluster*>(storage.get_data(type));
    assert(clusters);
    clusters->set_run(_run);
    clusters->set_subrun(_subrun);
    clusters->set_event_id(_event);
    
    for(int i=0;i<nclusters;i++) {
      larlight::cluster c;
      c.set_charge   (_ftr->getVal(name+"obj.fTotalCharge",i));
      c.set_dtdw     (_ftr->getVal(name+"obj.fdTdW",i));
      c.set_dqdw     (_ftr->getVal(name+"obj.fdTdW",i));
      c.set_dtdw_err (_ftr->getVal(name+"obj.fSigmadTdW",i));
      c.set_dqdw_err (_ftr->getVal(name+"obj.fSigmadQdW",i));
      c.set_id       (_ftr->getVal(name+"obj.fID",i));
      c.set_view     ((larlight::GEO::View_t)_ftr->getVal(name+"obj.fView",i));
      c.set_start_vtx     (*startPos.get<vector<double> >(i));
      c.set_end_vtx       (*endPos.get<vector<double> >(i));
      c.set_start_vtx_err (*sigmaStartPos.get<vector<double> >(i));
      c.set_end_vtx_err   (*sigmaEndPos.get<vector<double> >(i));

      // add associations
      c.add_association(larlight::DATA::Hit,map_to_hits[i]); // Fixme
      clusters->push_back(c);
    }
  } 
}


std::vector<std::string> Looter::findLeafOfType(std::string pattern)
{
  /// Look in the tree. Try to find a leafelement that matches 'pattern'.
  /// Return the full name of that leaf.
  vector<string> retval;
  
  // Strip whitespace from pattern.
  pattern.erase(std::remove_if(pattern.begin(), pattern.end(), ::isspace), pattern.end());
  
  TObjArray* list = _tree->GetListOfLeaves();
  for(int i=0;i<list->GetEntriesFast();i++) {
    TObject* o = list->At(i);
    TLeaf* le = (TLeaf*)o;
    std::string name = le->GetTypeName();
    // Strip whitespace from pattern.
    name.erase(std::remove_if(name.begin(), name.end(), ::isspace), name.end());
    size_t found = name.find(pattern);
    if(found != std::string::npos) {
      // Return this match.
      retval.push_back(le->GetName());
    }
  }
  return retval;
}


larlight::DATA::DATA_TYPE associationNameToType(const std::string& name)
{
  using namespace larlight::DATA;
  return larlight::DATA::DATA_TYPE_MAX;
  
}

larlight::DATA::DATA_TYPE nameToType(const std::string& name)
{
  using namespace larlight;
  using std::string;
  if(string::npos != name.find_first_of("recob::Hits")) {
    if(string::npos != name.find_first_of("recob::Hits_ffthit"))     return larlight::DATA::FFTHit;
    if(string::npos != name.find_first_of("recob::Hits_apahit"))     return DATA::APAHit;     // FIXME?
    if(string::npos != name.find_first_of("recob::Hits_gaushit"))    return DATA::GausHit;
    if(string::npos != name.find_first_of("recob::Hits_crawlerhit")) return DATA::CrawlerHit;
    else return DATA::Hit;
  }

  if(std::string::npos != name.find_first_of("recob::Clusters")) {
    if(std::string::npos != name.find_first_of("recob::Clusters_dbcluster")) return DATA::DBCluster;
    if(std::string::npos != name.find_first_of("recob::Clusters_crawler")) return DATA::CrawlerCluster; // FIXME?
    if(std::string::npos != name.find_first_of("recob::Clusters_hough")) return DATA::HoughCluster;
    if(std::string::npos != name.find_first_of("recob::Clusters_showeranglecluster")) return DATA::ShowerAngleCluster;

    else return DATA::Cluster;
  }
  return DATA::DATA_TYPE_MAX;
    
  // if(std::npos != name.find_first_of("recob::Clusters") {
  //   if(std::npos != name.find_first_of("recob::Clusters_dbcluster")) return DBCluster;
  //   // FIXME: need to check these others.
  //   else return Cluster;
  // }
  //   MCTruth,            ///< MCTruth
  //         MCParticle,         ///< MCParticle
  //         MCTrajectory,       ///< MCTrajectory
  //         MCNeutrino,         ///< MCNeutrino
  //         Wire,               ///< Wire
  //         FFTHit,             ///< FFT Hit algo
  //         Cluster,            ///< Cluster
  //         FuzzyCluster,       ///< Fuzzy Cluster
  //         DBCluster,          ///< DBCluster
  //         CrawlerCluster,     ///< Crawler
  //         HoughCluster,       ///< Hough
  //         ShowerAngleCluster, ///< ShowerAngleCluster
  //         Seed,               ///< Seed
  //         SpacePoint,         ///< Spacepoints
  //         Track,              ///< Track
  //         Bezier,             ///< Track (Bezier)
  //         Kalman3DSPS,        ///< Track (Kalman3DSPS)
  //         Kalman3DHit,        ///< Track (Kalman3DHit)
  //         Shower,             ///< Shower
  //         Vertex,             ///< Vertex
  //         FeatureVertex,      ///< FeatureVertex
  //         HarrisVertex,       ///< HarrisVertex
  //         EndPoint2D,         ///< EndPoint2D
  //         FeatureEndPoint2D,  ///< EndPoint2D from Feature Vertex Finder
  //         HarrisEndPoint2D,   ///< EndPoint2D from Harris Vertex Finder
  //         Calorimetry,        ///< Calorimetry
  //         UserInfo,           ///< dynamic data container ... this must be the last valid enum element
    
}

#endif
