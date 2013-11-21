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

  _clusterID[_num_clusters]  = clusterID;
  _view[_num_clusters]       = view;
  _charge[_num_clusters]     = charge;
  _dtdw[_num_clusters]       = dtdw;
  _dtdw_sigma[_num_clusters] = dtdw_sigma;
  _dqdw[_num_clusters]       = dqdw;
  _dqdw_sigma[_num_clusters] = dqdw_sigma;
  _startx[_num_clusters]     = startx;
  _starty[_num_clusters]     = starty;
  _startz[_num_clusters]     = startz;
  _endx[_num_clusters]       = endx;
  _endy[_num_clusters]       = endy;
  _endz[_num_clusters]       = endz;
  _startx_sigma[_num_clusters] = startx_sigma;
  _starty_sigma[_num_clusters] = starty_sigma;
  _startz_sigma[_num_clusters] = startz_sigma;
  _endx_sigma[_num_clusters] = endx_sigma;
  _endy_sigma[_num_clusters] = endy_sigma;
  _endz_sigma[_num_clusters] = endz_sigma;
  _num_clusters++;
}

void cluster::clear_event(bool all)
{

  // Clear data_base variables
  data_base::clear_event(all);

  for(UShort_t index = 0;
      (index < _num_clusters) || (all && index<DATA::kMaxClusters);
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
  _num_clusters=0;
}


Bool_t cluster::set_address(TTree* t,Bool_t create)
{
  print(MSG::DEBUG,__PRETTY_FUNCTION__,Form("%s called...",_name.c_str()));
  //Set address of the data_base variables
  Bool_t exist = data_base::set_address(t,create);

  if(t->GetBranch(Form("num_%s",_name.c_str()))) t->SetBranchAddress(Form("num_%s",_name.c_str()),&_num_clusters);
  else {
    exist = false;
    if(create) t->Branch(Form("num_%s",_name.c_str()),&_num_clusters,Form("num_%s/s",_name.c_str()));
  }

  if(t->GetBranch(Form("%s_clusterID",_name.c_str()))) t->SetBranchAddress(Form("%s_clusterID",_name.c_str()),_clusterID);
  else if(create) t->Branch(Form("%s_clusterID",_name.c_str()),_clusterID,Form("%s_clusterID[num_%s]/s",_name.c_str(),_name.c_str()));
  
  if(t->GetBranch(Form("%s_view",_name.c_str()))) t->SetBranchAddress(Form("%s_view",_name.c_str()),_view);
  else if(create) t->Branch(Form("%s_view",_name.c_str()),_view,Form("%s_view[num_%s]/b",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_charge",_name.c_str()))) t->SetBranchAddress(Form("%s_charge",_name.c_str()),_charge);
  else if(create) t->Branch(Form("%s_charge",_name.c_str()),_charge,Form("%s_charge[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_dtdw",_name.c_str()))) t->SetBranchAddress(Form("%s_dtdw",_name.c_str()),_dtdw);
  else if(create) t->Branch(Form("%s_dtdw",_name.c_str()),_dtdw,Form("%s_dtdw[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_dtdw_sigma",_name.c_str()))) t->SetBranchAddress(Form("%s_dtdw_sigma",_name.c_str()),_dtdw_sigma);
  else if(create) t->Branch(Form("%s_dtdw_sigma",_name.c_str()),_dtdw_sigma,Form("%s_dtdw_sigma[num_%s]/D",_name.c_str(),_name.c_str()));  

  if(t->GetBranch(Form("%s_dqdw",_name.c_str()))) t->SetBranchAddress(Form("%s_dqdw",_name.c_str()),_dqdw);
  else if(create) t->Branch(Form("%s_dqdw",_name.c_str()),_dqdw,Form("%s_dqdw[num_%s]/D",_name.c_str(),_name.c_str()));  

  if(t->GetBranch(Form("%s_dqdw_sigma",_name.c_str()))) t->SetBranchAddress(Form("%s_dqdw_sigma",_name.c_str()),_dqdw_sigma);
  else if(create) t->Branch(Form("%s_dqdw_sigma",_name.c_str()),_dqdw_sigma,Form("%s_dqdw_sigma[num_%s]/D",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_startx",_name.c_str()))) t->SetBranchAddress(Form("%s_startx",_name.c_str()),_startx);
  else if(create) t->Branch(Form("%s_startx",_name.c_str()),_startx,Form("%s_startx[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_starty",_name.c_str()))) t->SetBranchAddress(Form("%s_starty",_name.c_str()),_starty);
  else if(create) t->Branch(Form("%s_starty",_name.c_str()),_starty,Form("%s_starty[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_startz",_name.c_str()))) t->SetBranchAddress(Form("%s_startz",_name.c_str()),_startz);
  else if(create) t->Branch(Form("%s_startz",_name.c_str()),_startz,Form("%s_startz[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_startx_sigma",_name.c_str()))) t->SetBranchAddress(Form("%s_startx_sigma",_name.c_str()),_startx_sigma);
  else if(create) t->Branch(Form("%s_startx_sigma",_name.c_str()),_startx_sigma,Form("%s_startx_sigma[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_starty_sigma",_name.c_str()))) t->SetBranchAddress(Form("%s_starty_sigma",_name.c_str()),_starty_sigma);
  else if(create) t->Branch(Form("%s_starty_sigma",_name.c_str()),_starty_sigma,Form("%s_starty_sigma[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_startz_sigma",_name.c_str()))) t->SetBranchAddress(Form("%s_startz_sigma",_name.c_str()),_startz_sigma);
  else if(create) t->Branch(Form("%s_startz_sigma",_name.c_str()),_startz_sigma,Form("%s_startz_sigma[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_endx",_name.c_str()))) t->SetBranchAddress(Form("%s_endx",_name.c_str()),_endx);
  else if(create) t->Branch(Form("%s_endx",_name.c_str()),_endx,Form("%s_endx[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_endy",_name.c_str()))) t->SetBranchAddress(Form("%s_endy",_name.c_str()),_endy);
  else if(create) t->Branch(Form("%s_endy",_name.c_str()),_endy,Form("%s_endy[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_endz",_name.c_str()))) t->SetBranchAddress(Form("%s_endz",_name.c_str()),_endz);
  else if(create) t->Branch(Form("%s_endz",_name.c_str()),_endz,Form("%s_endz[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_endx_sigma",_name.c_str()))) t->SetBranchAddress(Form("%s_endx_sigma",_name.c_str()),_endx_sigma);
  else if(create) t->Branch(Form("%s_endx_sigma",_name.c_str()),_endx_sigma,Form("%s_endx_sigma[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_endy_sigma",_name.c_str()))) t->SetBranchAddress(Form("%s_endy_sigma",_name.c_str()),_endy_sigma);
  else if(create) t->Branch(Form("%s_endy_sigma",_name.c_str()),_endy_sigma,Form("%s_endy_sigma[num_%s]/F",_name.c_str(),_name.c_str()));

  if(t->GetBranch(Form("%s_endz_sigma",_name.c_str()))) t->SetBranchAddress(Form("%s_endz_sigma",_name.c_str()),_endz_sigma);
  else if(create) t->Branch(Form("%s_endz_sigma",_name.c_str()),_endz_sigma,Form("%s_endz_sigma[num_%s]/F",_name.c_str(),_name.c_str()));

  return exist;
}

#endif
