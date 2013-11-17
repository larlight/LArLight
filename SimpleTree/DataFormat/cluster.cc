#ifndef CLUSTER_CC
#define CLUSTER_CC

#include "cluster.hh"


cluster::cluster(DATA::DATA_TYPE type) : data_base(type)
{

  if(type!=DATA::CrawlerCluster &&
     type!=DATA::DBCluster &&
     type!=DATA::FuzzyCluster &&
     type!=DATA::HoughCluster &&
     type!=DATA::Cluster)
    {
      print(MSG::ERROR,__FUNCTION__,
	    Form("Invalid data type for cluster: %d ... setting default DATA::Cluster type",type));
      _type = DATA::Cluster;
    }
  else
    _type = type;

  _name = DATA::DATA_TREE_NAME[type];
  initialize();

}

void cluster::add_cluster(UShort_t clusterID, UChar_t view,
			  Double_t charge,
			  Double_t dtdw,   Double_t dtdw_sigma,
			  Double_t dqdw,   Double_t dqdw_sigma,
			  Float_t  startx, Float_t starty, Float_t startz,
			  Float_t  startx_sigma, Float_t starty_sigma, Float_t startz_sigma,
			  Float_t  endx,   Float_t endy,   Float_t endz,
			  Float_t  endx_sigma,   Float_t endy_sigma,   Float_t endz_sigma)
{

  _clusterID[_no_clusters]  = clusterID;
  _view[_no_clusters]       = view;
  _charge[_no_clusters]     = charge;
  _dtdw[_no_clusters]       = dtdw;
  _dtdw_sigma[_no_clusters] = dtdw_sigma;
  _dqdw[_no_clusters]       = dqdw;
  _dqdw_sigma[_no_clusters] = dqdw_sigma;
  _startx[_no_clusters]     = startx;
  _starty[_no_clusters]     = starty;
  _startz[_no_clusters]     = startz;
  _endx[_no_clusters]       = endx;
  _endy[_no_clusters]       = endy;
  _endz[_no_clusters]       = endz;
  _startx_sigma[_no_clusters] = startx_sigma;
  _starty_sigma[_no_clusters] = starty_sigma;
  _startz_sigma[_no_clusters] = startz_sigma;
  _endx_sigma[_no_clusters] = endx_sigma;
  _endy_sigma[_no_clusters] = endy_sigma;
  _endz_sigma[_no_clusters] = endz_sigma;
  _no_clusters++;
}

void cluster::clear_event(bool all)
{

  // Clear data_base variables
  data_base::clear_event(all);

  for(UShort_t index = 0;
      (index < _no_clusters) || (all && index<DATA::kMaxClusters);
      ++index) {

    _clusterID[index]  = 0;
    _view[index]       = 0;
    _charge[index]     = 0;
    _dtdw[index]       = 0;
    _dtdw_sigma[index] = 0;
    _dqdw[index]       = 0;
    _dqdw_sigma[index] = 0;
    _startx[index]     = 0;
    _starty[index]     = 0;
    _startz[index]     = 0;
    _endx[index]       = 0;
    _endy[index]       = 0;
    _endz[index]       = 0;
    _startx_sigma[index] = 0;
    _starty_sigma[index] = 0;
    _startz_sigma[index] = 0;
    _endx_sigma[index] = 0;
    _endy_sigma[index] = 0;
    _endz_sigma[index] = 0;

  }
  _no_clusters=0;
}


void cluster::set_address(TTree* t)
{

  //Set address of the data_base variables
  data_base::set_address(t);

  if(t->GetBranch("no_clusters")) t->SetBranchAddress("no_clusters",&_no_clusters);
  else t->Branch("no_clusters",&_no_clusters,"no_clusters/s");

  if(t->GetBranch("clusterID")) t->SetBranchAddress("clusterID",_clusterID);
  else t->Branch("clusterID",_clusterID,"clusterID[no_clusters]/s");

  if(t->GetBranch("view")) t->SetBranchAddress("view",_view);
  else t->Branch("view",_view,"view[no_clusters]/b");

  if(t->GetBranch("charge")) t->SetBranchAddress("charge",_charge);
  else t->Branch("charge",_charge,"charge[no_clusters]/D");

  if(t->GetBranch("dtdw")) t->SetBranchAddress("dtdw",_dtdw);
  else t->Branch("dtdw",_dtdw,"dtdw[no_clusters]/D");

  if(t->GetBranch("dtdw_sigma")) t->SetBranchAddress("dtdw_sigma",_dtdw_sigma);
  else t->Branch("dtdw_sigma",_dtdw_sigma,"dtdw_sigma[no_clusters]/D");  

  if(t->GetBranch("dqdw")) t->SetBranchAddress("dqdw",_dqdw);
  else t->Branch("dqdw",_dqdw,"dqdw[no_clusters]/D");  

  if(t->GetBranch("dqdw_sigma")) t->SetBranchAddress("dqdw_sigma",_dqdw_sigma);
  else t->Branch("dqdw_sigma",_dqdw_sigma,"dqdw_sigma[no_clusters]/D");

  if(t->GetBranch("startx")) t->SetBranchAddress("startx",_startx);
  else t->Branch("startx",_startx,"startx[no_clusters]/F");

  if(t->GetBranch("starty")) t->SetBranchAddress("starty",_starty);
  else t->Branch("starty",_starty,"starty[no_clusters]/F");

  if(t->GetBranch("startz")) t->SetBranchAddress("startz",_startz);
  else t->Branch("startz",_startz,"startz[no_clusters]/F");

  if(t->GetBranch("startx_sigma")) t->SetBranchAddress("startx_sigma",_startx_sigma);
  else t->Branch("startx_sigma",_startx_sigma,"startx_sigma[no_clusters]/F");

  if(t->GetBranch("starty_sigma")) t->SetBranchAddress("starty_sigma",_starty_sigma);
  else t->Branch("starty_sigma",_starty_sigma,"starty_sigma[no_clusters]/F");

  if(t->GetBranch("startz_sigma")) t->SetBranchAddress("startz_sigma",_startz_sigma);
  else t->Branch("startz_sigma",_startz_sigma,"startz_sigma[no_clusters]/F");

  if(t->GetBranch("endx")) t->SetBranchAddress("endx",_endx);
  else t->Branch("endx",_endx,"endx[no_clusters]/F");

  if(t->GetBranch("endy")) t->SetBranchAddress("endy",_endy);
  else t->Branch("endy",_endy,"endy[no_clusters]/F");

  if(t->GetBranch("endz")) t->SetBranchAddress("endz",_endz);
  else t->Branch("endz",_endz,"endz[no_clusters]/F");

  if(t->GetBranch("endx_sigma")) t->SetBranchAddress("endx_sigma",_endx_sigma);
  else t->Branch("endx_sigma",_endx_sigma,"endx_sigma[no_clusters]/F");

  if(t->GetBranch("endy_sigma")) t->SetBranchAddress("endy_sigma",_endy_sigma);
  else t->Branch("endy_sigma",_endy_sigma,"endy_sigma[no_clusters]/F");

  if(t->GetBranch("endz_sigma")) t->SetBranchAddress("endz_sigma",_endz_sigma);
  else t->Branch("endz_sigma",_endz_sigma,"endz_sigma[no_clusters]/F");
  

}

#endif
