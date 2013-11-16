/**
 * \file data_base.hh
 *
 * \ingroup SimpleTreeData
 * 
 * \brief Base class def for data holder class
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup SimpleTreeData

    @{*/
#ifndef DATA_BASE_HH
#define DATA_BASE_HH

#include "Base-TypeDef.hh"
#include <TChain.h>

/**
   \class data_base
   A base coass for data holder classes. Note this is NOT data storage class,
   which is in AnaProcess/DataFormat for example. A data holder class is to 
   temporariry hold values in memory during the lifetime of the program, and
   not written to the root file. Instead, we write a simple Tuple-like TTree
   into the ROOT file, and implement data holder class to be able to interact
   with those trees. TTree will be a private member of data holder class though
   const pointer will be accessible from outside.
 */
class data_base : public fmwk_base{

public:

  /// Default constructor
  data_base(DATA::DATA_TYPE type=DATA::DATA_TYPE_MAX);

  /// Default destructor
  virtual ~data_base(){};

  /// Clear method
  virtual void clear_event();

  /// Clear everything including TTree (will be deleted)
  virtual void initialize();

  //--- TTree I/O functions ---//

  /// Set branch address for reading
  virtual void set_address(TTree* t);

  //--- Member getter functions ---//
  
  /// Entry ID getter
  int entry() const {return _entry;};

  //--- Member setter functions ---//
  void entry(int n) {_entry=n;};

protected:

  /// TTree Entry ID
  int _entry;

  /// Data type
  DATA::DATA_TYPE _type;
};

#endif
/** @} */ // end of doxygen group 
