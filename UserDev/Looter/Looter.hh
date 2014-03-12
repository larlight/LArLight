/**
 * \file Looter.hh
 *
 * \ingroup Looter
 * 
 * \brief Class def header for a class Looter
 *
 * @author tagg
 */

/** \addtogroup Looter

    @{*/
#ifndef LOOTER_HH
#define LOOTER_HH

#include <iostream>
#include <string>
#include "storage_manager.hh"

class TFile;
class TTree;
#include "TreeReader.h"

/**
   \class Looter
   User defined class Looter ... these comments are used to generate
   doxygen documentation!
 */
class Looter{

public:

  /// Default constructor
  Looter(std::string filename);

  /// Default destructor
  virtual ~Looter();

  virtual int loot(larlight::storage_manager& storage, Long64_t start_event = 0, Long64_t nevents = -1);
  
  bool bood() { return _good; }
  
protected:

  void read_hits(larlight::storage_manager& storage);
  void read_clusters(larlight::storage_manager& storage);

  std::vector<std::string> findLeafOfType(std::string pattern);

  bool        _good;
  std::string _filename;
  TFile*      _file;
  TTree*      _tree;
  TreeReader* _ftr;
  Long64_t    _entry;
  int         _run;
  int         _subrun;
  int         _event;
  
};

#endif
/** @} */ // end of doxygen group 

