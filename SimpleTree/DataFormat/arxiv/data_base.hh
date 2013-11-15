/**
 * \file stree_base.hh
 *
 * \ingroup SimpleTreeData
 * 
 * \brief Base class def for data holder class
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup SimpleTreeData

    @{*/
#ifndef STREE_BASE_HH
#define STREE_BASE_HH

#include "Base-TypeDef.hh"
#include <vector>
#include <limits>
#include <TObject.h>

/**
   \class stree_base
   A base coass for data holder classes. Note this is NOT data storage class,
   which is in AnaProcess/DataFormat for example. A data holder class is to 
   temporariry hold values in memory during the lifetime of the program, and
   not written to the root file. Instead, we write a simple Tuple-like TTree
   into the ROOT file, and implement data holder class to be able to interact
   with those trees. TTree will be a private member of data holder class though
   const pointer will be accessible from outside.
 */
class stree_base {

public:

  /// Default constructor
  stree_base();

  /// Default destructor
  virtual ~stree_base(){};

  /// Clear method
  virtual void clear_event();

  /// Clear everything including TTree (will be deleted)
  virtual void initialize();

  /// Set branch address for reading
  virtual void read_tree(TChain* ch);

  /// Get TTree pointer for writing
  virtual TTree* write_tree();

private:

  /// TTree pointer
  TTree* _tree;

  /// TTree Entry ID
  int _entry;

  /// Data type
  DATA::DATA_TYPE _type;
};

#endif
/** @} */ // end of doxygen group 
