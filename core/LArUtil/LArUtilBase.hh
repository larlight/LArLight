/**
 * \file LArUtilBase.hh
 *
 * \ingroup LArUtil
 * 
 * \brief Class def header for a class LArUtilBase
 *
 * @author kazuhiro
 */

/** \addtogroup LArUtil

    @{*/
#ifndef LARUTILBASE_HH
#define LARUTILBASE_HH

#include <map>
#include <sstream>
#include <TChain.h>
#include <Base-TypeDef.hh>

#include "LArUtilException.hh"
#include "LArUtilConstants.hh"

namespace larutil {
  /**
     \class LArUtilBase
     Base class for utility classes. Currently we take a simple approach of reading-in parameter
     values from TTree. This base class implements a way to read TTree and also store output TTree.
     DB access may be implemented here in future.
  */
  class LArUtilBase : public larlight::larlight_base{
    
  public:
    
    /// Default constructor
    LArUtilBase(std::string file_name="", std::string tree_name="");
    
    /// Default destructor
    virtual ~LArUtilBase(){};

    /// Method to set data file name
    void SetFileName(const std::string filename)
    { _file_name = filename; }

    /// Method to store TTree in TFile
    const TTree* GetDataTree() const
    { return (TTree*)_data_tree; }

    /// Method to set data TTree name
    void SetTreeName(const std::string treename)
    { _tree_name = treename; }

    /// Method to load data TTree
    bool LoadData(bool force_reload=false);

  protected:

    /// Method to generate data TTree
    virtual void SetBranchAddress(){};

    std::string _file_name;
    std::string _tree_name;
    TChain *_data_tree;
    
  };
}

#endif
/** @} */ // end of doxygen group 

