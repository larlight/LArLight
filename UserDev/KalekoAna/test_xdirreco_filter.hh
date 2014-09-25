/**
 * \file test_xdirreco_filter.hh
 *
 * \ingroup KalekoAna
 * 
 * \brief Class def header for a class test_xdirreco_filter
 *
 * @author davidkaleko
 */

/** \addtogroup KalekoAna

    @{*/

#ifndef TEST_XDIRRECO_FILTER_HH
#define TEST_XDIRRECO_FILTER_HH

#include "ana_base.hh"

namespace kaleko {
  /**
     \class test_xdirreco_filter
     User custom analysis class made by davidkaleko
   */
  class test_xdirreco_filter : public larlight::ana_base{
  
  public:

    /// Default constructor
    test_xdirreco_filter(){ _name="test_xdirreco_filter"; _fout=0;};

    /// Default destructor
    virtual ~test_xdirreco_filter(){};

    /** IMPLEMENT in test_xdirreco_filter.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in test_xdirreco_filter.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(larlight::storage_manager* storage);

    /** IMPLEMENT in test_xdirreco_filter.cc! 
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
