/**
 * \file TrackViewer.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class TrackViewer
 *
 * @author kazuhiro
 */

/** \addtogroup Analysis

    @{*/

#ifndef TRACKVIEWER_HH
#define TRACKVIEWER_HH

#include "ana_base.hh"
#include <TH3D.h>
#include <TCanvas.h>
#include <TPad.h>
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

  /// A utility function to (re)create Th3D histogram of a specified boundary & name
  TH3D* Prepare3DHisto(std::string name, 
		       double xmin, double xmax,
		       double ymin, double ymax,
		       double zmin, double zmax);

  /// A method to draw currently created histograms (which are made by analyze() function)
  void DrawCanvas();

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

};

#endif

/** @} */ // end of doxygen group 
