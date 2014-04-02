/**
 * \file SimChannelViewer.hh
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class SimChannelViewer
 *
 * @author davidkaleko
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef SIMCHANNELVIEWER_HH
#define SIMCHANNELVIEWER_HH


#include <iostream>
#include "ana_base.hh"
#include "Geometry.hh"
#include "GeometryUtilities.hh"
#include "DetectorProperties.hh"
#include <TH2D.h>


namespace cluster{

  /**
     \class SimChannelViewer
     User defined class SimChannelViewer ... these comments are used to generate
     doxygen documentation!
  */
  
  class SimChannelViewer{
    
  public:
    
    /// Default constructor
    SimChannelViewer();
    
    /// Default destructor
    virtual ~SimChannelViewer(){};

    void GenerateHisto(const larlight::event_simch *esch, int plane);


    TH2D* GetSimChView() { return hSimChView; }


  protected:

    TH2D* hSimChView;

    std::map<UShort_t, std::vector< larlight::ide > > fTDCIDEs;
};

  
}
#endif
/** @} */ // end of doxygen group 

