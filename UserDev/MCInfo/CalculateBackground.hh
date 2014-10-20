/**
 * \file CalculateBackground.hh
 *
 * \ingroup MCInfo
 * 
 * \brief Class def header for a class CalculateBackground
 *
 * @author David Caratelli
 */

/** \addtogroup MCInfo

    @{*/

#ifndef CALCULATEBACKGROUND_HH
#define CALCULATEBACKGROUND_HH

#include "ana_base.hh"
#include "MCgetter.hh"
#include "TrajectoryInVolume.hh"
#include "PointToLineDist.hh"
#include "Geometry.hh"
#include <string>

namespace larlight {
  /**
     \class CalculateBackground
     User custom analysis class made by david
   */
  class CalculateBackground : public ana_base{
  
  public:

    /// Default constructor
    CalculateBackground(){ _name="CalculateBackground"; _fout=0; _verbose=false; SetProperties(); };

    /// Default destructor
    virtual ~CalculateBackground(){};

    /** IMPLEMENT in CalculateBackground.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in CalculateBackground.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in CalculateBackground.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void SetVerbose(bool on) { _verbose = on; }

    /// Function to set physics process to be analyzed
    void SetProcess(std::vector<int> PDGs, std::vector<std::string> procs);

    /// Set Energy threshold for particles to be considered
    void SetECut(double E) { _MCgetter.SetECut(E); _Ecut = E; }

    /// Set-up Background Histogram Information
    void SetBackgroundHistogram(int nbins, float xmin, float xmax);
    /// NormalizeBackground
    void NormalizeBackgroundHistogram();
    /// Write Background Histogram
    void WriteBackgroundHistogram();
    /// Fill Background Histogram
    void FillBackgroundHistogram();


    /// Set PDG lsit for MCgetter
    void SetPDGlist(std::vector<int> pdgs) { _MCgetter.getAllPDGs(pdgs); }

    /// prepare TTree
    void prepareTree();
    /// Set All Properties
    void SetProperties();
    /// Set POTs:
    void setPOT(double pot) { _POT = pot; }
    /// Set pps"
    void setpps(double pps) { _pps = pps; }
    /// Set beam Time
    void setBeamTime(double beamtime) { _beamT = beamtime; }
    /// Set event readout time
    void setEventTime(double eventtime) { _eventT = eventtime; }

    protected:

    /// Process to be searched
    std::vector<std::pair<int,std::string> > _process;

    /// verbose
    bool _verbose;

    /// double Energy cut
    double _Ecut;

    /// Event number
    int _evtN;

    /// MCgetter to make mc particle map
    MCgetter _MCgetter;

    /// GeoAlg for TPC containment
    TrajectoryInVolume _inTPCAlgo;
    /// GeoAlg for point to line dist
    PointToLineDist _pointDist;

    /// Variables useful to normalize events
    /// POT requested: duration of experiment
    double _POT; 
    /// pulses-per-spill (one spill = one event)
    double _pps;
    /// event readout time
    double _eventT;
    /// beam time
    double _beamT;
    /// Number of events in file
    double _Nevents;


    /// Tree for analysis
    TTree* _tree;
    //variables for analysis
    double _Energy;
    int    _inTPC;
    double _AncestorE;
    int    _AncestorPDG;
    double _AncestorDist;
    double _MotherE;
    int    _MotherPDG;
    double _MotherDist;
    double _StartX;
    double _StartY;
    double _StartZ;
    double _EndX;
    double _EndY;
    double _EndZ;
    std::string Process;
    std::string ProcHist;

    // Variable for Background Histogram
    TH1D*  _hBackground;
    int    _Nbins;
    double _Emin;
    double _Emax;
    

  };
}
#endif

//**************************************************************************
// 
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group 
