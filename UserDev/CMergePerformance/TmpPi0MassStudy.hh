/**
 * \file TmpPi0MassStudy.hh
 *
 * \ingroup CMergePerformance
 * 
 * \brief Class def header for a class TmpPi0MassStudy
 *
 * @author davidkaleko
 */

/** \addtogroup CMergePerformance

    @{*/

#ifndef TMPPI0MASSSTUDY_HH
#define TMPPI0MASSSTUDY_HH

#include "ana_base.hh"
#include "ComputePi0Mass.hh"

namespace larlight {
  /**
     \class TmpPi0MassStudy
     User custom analysis class made by davidkaleko
   */
  class TmpPi0MassStudy : public ana_base{
  
  public:

    /// Default constructor
    TmpPi0MassStudy(){ _name="TmpPi0MassStudy"; _fout=0;};

    /// Default destructor
    virtual ~TmpPi0MassStudy(){};

    /** IMPLEMENT in TmpPi0MassStudy.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in TmpPi0MassStudy.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in TmpPi0MassStudy.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:

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
