/**
 * \file WireViewer.hh
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class WireViewer
 *
 * @author davidkaleko
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef WIREVIEWER_HH
#define WIREVIEWER_HH

#include <iostream>
#include "ana_base.hh"
#include "Geometry.hh"
#include "GeometryUtilities.hh"
#include <TH2D.h>


namespace cluster{
  /**
     \class WireViewer
     User defined class WireViewer ... these comments are used to generate
     doxygen documentation!
  */
  class WireViewer{
    
  public:
    
    /// Default constructor
    WireViewer();
    
    /// Default destructor
    virtual ~WireViewer(){};

    void FilterBaseline(bool doit){ fFilterBaseline=doit;}
    
    void GenerateHisto(const larlight::event_wire *ewire, int plane);

    TH2D* GetWireView() { return hWireView; }

  protected:

    TH2D* hWireView;

    Float_t fThresholdSigma;

    size_t  fNSampleSigma;

    bool  fFilterBaseline;

  };
  
}
#endif
/** @} */ // end of doxygen group 

  
