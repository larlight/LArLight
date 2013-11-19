#ifndef DATA_BASE_HH
#define DATA_BASE_HH
/**
 * \file data_base.hh
 *
 * \ingroup SimpleTreeData
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup SimpleTreeData
    This package defines data storage classes that serves as an I/O interface with stored analysis tree. It includes a dedicated event loop I/O facility called storage_manager which also helps analyzers to obtain a simply chained-up TTrees for simple TTree::Draw analysis.
    @{*/

#include "Base-TypeDef.hh"
#include "DataTypeConstants.hh"
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
class data_base : public FrameworkBase{

public:

  /// Default constructor. Provide a corresponding data type defined in DATA namespace.
  data_base(DATA::DATA_TYPE type=DATA::DATA_TYPE_MAX);

  /// Default destructor
  virtual ~data_base(){};

  /**
     Method to clear event-wise data members.
     The argument boolean, "all", specifies whether the ALL data members to be cleared or not.
     It is purely for speed performance of using C-array as a data member.
     See daughters for specific implementation.
  */
  virtual void clear_event(bool all=false);

  /// Clear everything including TTree (will be deleted)
  virtual void initialize();

  //--- TTree I/O functions ---//

  /// Set branch address for reading
  virtual void set_address(TTree* t);

  //--- Member getter functions ---//
  
protected:

  /// Data type
  DATA::DATA_TYPE _type;
};

#endif
/** @} */ // end of doxygen group 
