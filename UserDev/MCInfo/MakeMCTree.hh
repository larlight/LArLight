/**
 * \file MakeMCTree.hh
 *
 * \ingroup MCInfo
 * 
 * \brief Class def header for a class MakeMCTree
 *
 * @author david
 */

/** \addtogroup MCInfo

    @{*/

#ifndef MAKEMCTREE_HH
#define MAKEMCTREE_HH

#include "ana_base.hh"
#include "MCgetter.hh"
#include "TreeTop.hh"
#include "TreeNode.hh"
#include <string>

namespace larlight {
  /**
     \class MakeMCTree
     User custom analysis class made by david
   */
  class MakeMCTree : public ana_base{
  
  public:

    /// Default constructor
    MakeMCTree(){ _name="MakeMCTree"; _fout=0; _verbose=false; };

    /// Default destructor
    virtual ~MakeMCTree(){};

    /** IMPLEMENT in MakeMCTree.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in MakeMCTree.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in MakeMCTree.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void SetVerbose(bool on) { _verbose = on; }

    void AddNodes(const event_mcpart *event_part, mcpart part, TreeNode& node);

    int findProcess(const event_mcpart *event_part, TreeNode currentNode,
		    int proc, std::vector<int> trackIDpath, std::vector<TreeNode> NodeList,
		    const std::vector<std::pair<int,std::string> > procList,
		    std::vector<int> branchPositionList, int branchPosition);
    
    protected:

    /// verbose
    bool _verbose;

    /// MCgetter to make mc particle map
    MCgetter _MCgetter;

    /// list of TreeTops, one per "primary" particle
    std::vector<TreeTop> _treeTops;

    /// list of TreeNodes, one per "primary" particle
    std::vector<TreeNode> _treeNodes;

    int _nParticles;
    int _nEvts;


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
