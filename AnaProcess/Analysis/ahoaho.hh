/**
 * \file ahoaho.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class ahoaho
 *
 * @author kazuhiro
 */

/** \addtogroup Analysis

    @{*/

#ifndef AHOAHO_HH
#define AHOAHO_HH

#include "ana_base.hh"

/**
   \class ahoaho
   User custom analysis class made by kazuhiro
 */
class ahoaho : public ana_base{

public:

  /// Default constructor
  ahoaho(){ _name="ahoaho"; _fout=0;};

  /// Default destructor
  virtual ~ahoaho(){};

  /** IMPLEMENT in ahoaho.cc!
      Initialization method to be called before the analysis event loop.
  */ 
  virtual bool initialize();

  /** IMPLEMENT in ahoaho.cc! 
      Analyze a data event-by-event  
  */
  virtual bool analyze(storage_manager* storage);

  /** IMPLEMENT in ahoaho.cc! 
      Finalize method to be called after all events processed.
  */
  virtual bool finalize();

protected:

};

#endif

/** @} */ // end of doxygen group 
