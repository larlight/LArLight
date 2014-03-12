/**
 * \file TrackViewer.hh
 *
 * \ingroup TrackStudy
 * 
 * \brief Class def header for a class TrackViewer
 *
 * @author kazuhiro
 */

/** \addtogroup TrackStudy

    @{*/

#ifndef TRACKVIEWER_HH
#define TRACKVIEWER_HH

#include "ana_base.hh"
#include <TH3D.h>
#include <TCanvas.h>
#include <TPad.h>

namespace larlight {
  
  /**
     \class TrackViewer
     User custom analysis class made by kazuhiro
  */
  class TrackViewer : public ana_base{
    
  public:
    
    /// Default constructor
    TrackViewer();
    
    /// Default destructor
    virtual ~TrackViewer(){};
    
    /** IMPLEMENT in TrackViewer.cc!
	Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();
    
    /** IMPLEMENT in TrackViewer.cc! 
	Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);
    
    /** IMPLEMENT in TrackViewer.cc! 
	Finalize method to be called after all events processed.
    */
    virtual bool finalize();
    
    /// Set the track type to plot
    DATA::DATA_TYPE fDataType;
    void set_data_type(DATA::DATA_TYPE type) { fDataType = type;};
    
    /// A utility function to (re)create Th3D histogram of a specified boundary & name
    TH3D* Prepare3DHisto(std::string name, 
			 double xmin, double xmax,
			 double ymin, double ymax,
			 double zmin, double zmax);
    
    /// Getter for spacepoint histogram
    const TH3D* GetHisto_SPS  () const {return _hRecoSPS;};
    
    /// Getter for MC points
    const TH3D* GetHisto_MC   () const {return _hMCStep;};
    
    /// Getter for Track histogram vector
    const std::vector<TH3D*>  GetHisto_Reco () const {return _hRecoTrack_v;};
    
    /// Getter for track data object vector
    const std::vector<larlight::track>  GetData_Reco  () const {return _track_v;};
    
  protected:
    /// Main canvas
    TCanvas* _c1;
    /// Main pad
    TPad*    _p1;
    /// Spacepoints histogram
    TH3D*    _hRecoSPS;
    /// MC step (primary particle only) histogram
    TH3D*    _hMCStep;
    /// Histogram array for Reconstructed tracks' trajectory points 
    std::vector<TH3D*> _hRecoTrack_v;
    /// track data product vector
    std::vector<track> _track_v;
    
  };
};
#endif
  
/** @} */ // end of doxygen group 
  
