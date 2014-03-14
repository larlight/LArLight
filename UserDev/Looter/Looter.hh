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
   This class is an attempt to read larsoft data directly from a LArSoft .root file, without the need
    to run a LArSoft executable. It uses the ARgo method of directly interrogating the ROOT RTTI information to 
    yank data from the file.

   Although it currently works as proof-of-principle, it does nothing more than pull hits.  The problem is that
    I can't figure out how to correctly disambiguate all the event types by string, since I don't yet understand
    how larsoft generates these types.  This goes double for associations.  The functions at the bottom of the .cc
    file show how far I got, which isn't very far.

  Moreover, it's slow: it takes upwards of 2-3 seconds per event due to the extra load ROOT incurs for having
   to do the RTTI on event load.   This is much slower than Kazu's solution.

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

