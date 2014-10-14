/**
 * \file LEEEventSelection.hh
 *
 * \ingroup UBSensInterface
 * 
 * \brief This is a filter that reads in LArLight format MC generated
 * for the low energy excess sensitivity studies, determines if the event
 * passes event selection cuts, and returns false (filters out) if the
 * event does not pass. E.G. if mcshower mother vertex is outside of 
 * the detector fiducial volume, do return false.
 * Could also imagine if the shower is < X% contained, return false. etc.
 *
 * @author davidkaleko
 */

/** \addtogroup UBSensInterface

    @{*/

#ifndef LEEEVENTSELECTION_HH
#define LEEEVENTSELECTION_HH

#include "ana_base.hh"
#include "DistToBoxWall.hh"

namespace larlight {
  /**
     \class LEEEventSelection
     User custom analysis class made by davidkaleko
   */
  class LEEEventSelection : public ana_base{
  
  public:

    /// Default constructor
    LEEEventSelection(){ 
      _name="LEEEventSelection"; 
      _fid_cut_dist = 17.;
      _fout=0;
    };

    /// Default destructor
    virtual ~LEEEventSelection(){};

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

    void SetFiducialCutDistance(double kaleko){ _fid_cut_dist = kaleko; }

  protected:

    double _fid_cut_dist;

    geoalgo::DistToBoxWall _dtbw;

  };
}
#endif
/** @} */ // end of doxygen group 
