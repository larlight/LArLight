/**
 * \file ana_base.hh
 *
 * \ingroup Analysis
 * 
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Analysis

    Analysis package dedicated for high level analysis module development and driver code. Empty for now.

    @{*/

#ifndef ANA_BASE_HH
#define ANA_BASE_HH

#include <TFile.h>
#include "FrameworkBase.hh"
#include "DataFormat-TypeDef.hh"

/**
   \class ana_base
   A base class for analysis modules to be operated with event_waveform
   data class instance. 
 */
class ana_base : public FrameworkBase{

public:

  /// Default constructor
  ana_base() : FrameworkBase() { _name="ana_base"; _fout=0;};

  /// Default destructor
  virtual ~ana_base(){};

  /// Initialization method to be called before analyze any data
  virtual bool initialize()=0;

  /// Analyze a data event-by-event  
  virtual bool analyze(storage_manager* data)=0;

  /// Finalize method to be called after all events processed.
  virtual bool finalize()=0;

  /// A setter for analysis output file poitner
  void set_output_file(TFile* fout){_fout=fout;};

protected:

  TFile* _fout; ///< Analysis output file pointer
};

#endif

/** @} */ // end of doxygen group 
