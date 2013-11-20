#ifndef TRACKVIEWER_CC
#define TRACKVIEWER_CC

#include "TrackViewer.hh"

//################################################################
TrackViewer::TrackViewer() : ana_base(), _hRecoTrack_v()
//################################################################
{
  // Class name
  _name = "TrackViewer";
  // Set initialization values for pointers
  _fout     = 0;
  _hRecoSPS = 0;
  _hMCStep  = 0;
  _track    = 0;
  fDataType = DATA::Kalman3DSPS;
}

//################################################################
bool TrackViewer::initialize()
//################################################################
{

  return true;
}

//################################################################
bool TrackViewer::analyze(storage_manager* storage)
//################################################################
{

  // Clean up histograms if they already exist (from previous event)
  if(_hMCStep)  {delete _hMCStep;  _hMCStep  = 0;};
  if(_hRecoSPS) {delete _hRecoSPS; _hRecoSPS = 0;};
  for(auto h : _hRecoTrack_v) {delete h; h=0;};

  _hRecoTrack_v.clear();
  _track = 0;

  //
  // Obtain event-wise data object pointers
  //
  const sps*    my_sps    = (sps*)(storage->get_data(DATA::SpacePoint));
  const track*  my_track  = (track*)(storage->get_data(fDataType));
  const mcstep* my_mcstep = (mcstep*)(storage->get_data(DATA::MCTrajectory));

  // Define utility variables to hold max/min of each axis range
  Float_t xmax, ymax, zmax;
  xmax = ymax = zmax = -1.;
  Float_t xmin, ymin, zmin;
  xmin = ymin = zmin = 999999.;

  // Loop over vertex points
  if(my_sps)

    for(size_t i=0; i<my_sps->num_sps(); ++i) {

      xmax = std::max((my_sps->vtxx()[i]),xmax);
      xmin = std::min((my_sps->vtxx()[i]),xmin);
      ymax = std::max((my_sps->vtxy()[i]),ymax);
      ymin = std::min((my_sps->vtxy()[i]),ymin);
      zmax = std::max((my_sps->vtxz()[i]),zmax);
      zmin = std::min((my_sps->vtxz()[i]),zmin);

    }

  if(my_track)

    for(size_t i=0; i<my_track->num_points(); ++i) {

      xmax = std::max((my_track->vtxx()[i]),xmax);
      xmin = std::min((my_track->vtxx()[i]),xmin);
      ymax = std::max((my_track->vtxy()[i]),ymax);
      ymin = std::min((my_track->vtxy()[i]),ymin);
      zmax = std::max((my_track->vtxz()[i]),zmax);
      zmin = std::min((my_track->vtxz()[i]),zmin);      

    }

  // Proceed only if minimum/maximum are set to some values other than the defaults
  if(xmax == -1) {

    print(MSG::WARNING,__FUNCTION__,
	  "Did not find any reconstructed spacepoint or track. Skipping this event...");

    return true;
  }

  //
  // Make & fill vertex histograms
  //

  // Spacepoints
  if(my_sps) {
    
    /// Note: for a plotting style, it is convenient to have a (x, y, z) labeled as (z, x, y).
    _hRecoSPS = Prepare3DHisto("_hRecoSPS", zmin, zmax, xmin, xmax, ymin, ymax);

    for(size_t i=0; i<my_sps->num_sps(); ++i)

      _hRecoSPS->Fill(my_sps->vtxz()[i], my_sps->vtxx()[i], my_sps->vtxy()[i]);
    
    _hRecoSPS->SetMarkerStyle(23);
    _hRecoSPS->SetMarkerColor(kBlue);
  }

  // Tracks
  if(my_track) {
    
    Int_t id=-1;
    TH3D* h=0;
    for(size_t i=0; i<my_track->num_points(); ++i){
      
      if(id<0 || id != my_track->vtxID()[i]) {

	id = my_track->vtxID()[i];

	if(h) _hRecoTrack_v.push_back(h);
	
	h  = Prepare3DHisto(Form("_hRecoTrack_%03d",(int)(id)),zmin,zmax,xmin,xmax,ymin,ymax);
	h->SetMarkerStyle(22);
	h->SetMarkerColor(kRed);

      }
      
      h->Fill(my_track->vtxz()[i],my_track->vtxx()[i],my_track->vtxy()[i]);
	      
    }
    if(h) _hRecoTrack_v.push_back(h);
    _track = (track*)my_track;
  }

  return true;  
  // MC trajectory points
  if(my_mcstep){

    for(size_t i=0; i<my_mcstep->num_steps(); ++i) {

      if(my_mcstep->trackID()[i]>1) break;
      if(my_mcstep->trackID()[i]==0) continue;

      if(!_hMCStep) {

	_hMCStep = Prepare3DHisto("_hMCStep",
				  zmin, zmax, xmin, xmax, ymin, ymax);
	_hMCStep->SetMarkerStyle(20);
	_hMCStep->SetMarkerColor(kCyan);
      }
	
      _hMCStep->Fill(my_mcstep->vtxx()[i],
		     my_mcstep->vtxy()[i],
		     my_mcstep->vtxz()[i]);
    }
  }
  
  return true;
};

//################################################################
TH3D* TrackViewer::Prepare3DHisto(std::string name, 
				  double xmin, double xmax,
				  double ymin, double ymax,
				  double zmin, double zmax)
//################################################################
{

  TH3D* h=0;

  h = new TH3D(name.c_str(),"3D Viewer; Z; X; Y",
	       50,  xmin, xmax,
	       50,  ymin, ymax,
	       300, zmin, zmax);

  return h;
}

bool TrackViewer::finalize() {

  // This function is called at the end of event loop.
  // Do all variable finalization you wish to do here.
  // If you need, you can store your ROOT class instance in the output
  // file. You have an access to the output file through "_fout" pointer.

  return true;
}

#endif
