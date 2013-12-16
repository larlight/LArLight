/**
 * \file KalekoClusAna.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class KalekoClusAna
 *
 * @author davidkaleko
 */

/** \addtogroup Analysis

    @{*/

#ifndef KALEKOCLUSANA_HH
#define KALEKOCLUSANA_HH

#include "ana_base.hh"
#include "ClusterAnaPrep.hh"

namespace kaleko {
  /**
     \class KalekoClusAna
     User custom analysis class made by davidkaleko
   */
  class KalekoClusAna : public larlight::ana_base{
  
  public:

    /// Default constructor
    KalekoClusAna(){ _name="KalekoClusAna"; _fout=0;};

    /// Default destructor
    virtual ~KalekoClusAna(){};

    /** IMPLEMENT in KalekoClusAna.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in KalekoClusAna.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(larlight::storage_manager* storage);

    /** IMPLEMENT in KalekoClusAna.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    protected:

  };
}
#endif

/** @} */ // end of doxygen group 
