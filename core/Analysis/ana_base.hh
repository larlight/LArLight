/**
 * \file ana_base.hh
 *
 * \ingroup Analysis
 * 
 * \brief Base class def for all analysis classes
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Analysis

    @{*/

#ifndef ANA_BASE_HH
#define ANA_BASE_HH

#include <TFile.h>
#include <TGraph.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TGraphErrors.h>

#include "larlight_base.hh"
#include "DataFormat-TypeDef.hh"

namespace larlight {
  /**
     \class ana_base
     A base class for analysis modules to be operated with event_waveform
     data class instance. 
  */
  class ana_base : public larlight_base{
    
  public:
    
    /// Default constructor
    ana_base() : larlight_base() { _name="ana_base"; _fout=0;}
    
    /// Default destructor
    virtual ~ana_base(){}
    
    /// Initialization method to be called before analyze any data
    virtual Bool_t initialize(){return true;}
    
    /// Analyze a data event-by-event  
    virtual Bool_t analyze(storage_manager* data){return (data);}
    
    /// Finalize method to be called after all events processed.
    virtual Bool_t finalize(){return true;}
    
    /// A setter for analysis output file poitner
    void set_output_file(TFile* fout){_fout=fout;}
    
  protected:
    
    TFile* _fout; ///< Analysis output file pointer
  };
}
#endif
  
/** @} */ // end of doxygen group 
  
