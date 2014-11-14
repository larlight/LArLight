#ifndef MAKEMCTREE_CC
#define MAKEMCTREE_CC

#include "MakeMCTree.hh"

namespace larlight {

  bool MakeMCTree::initialize() {

    if (_verbose) { _MCgetter.SetVerbose(true); }
    _MCgetter.SetECut(0.);
    _nParticles = 0;
    _nEvts += 1;
    return true;
  }
  
  bool MakeMCTree::analyze(storage_manager* storage) {
    /*  
    std::vector<std::pair<int,std::string> > proclist;
    std::pair<int,std::string> pair;
    pair = std::make_pair(-13,"primary");
    proclist.push_back(pair);
    pair = std::make_pair(-11,"Decay");
    proclist.push_back(pair);
    */
    event_mcpart *event_part = (event_mcpart*)(storage->get_data(DATA::MCParticle));

    // make the particle map
    std::cout << "Particles for this event: " << event_part->size() << std::endl;
    _nParticles += event_part->size();
    _nEvts += 1;
    _MCgetter.Reset(event_part);
    /*
    std::vector<int> results = _MCgetter.findProcess(proclist);
    std::cout << "results: " << std::endl;
    for (size_t i=0; i < results.size(); i ++)
      std::cout << results.at(i) << "\t";
    std::cout << std::endl;
    */
    /*
    if (_verbose) { std::cout << "Tot number of particles: " << event_part->size() << std::endl; }
    int tot = 0;

    for (size_t i=0; i < _MCgetter.getTreeTops().size(); i++){

      if (_verbose) { std::cout << "children for root: " << _MCgetter.getTreeTops().at(i).getChildren().size() << std::endl; }
      int c = 0;
      _MCgetter.getTreeTops().at(i).countNodes(c);
      tot += c;
      if (_verbose) { std::cout << "This primary has: " << c << " offspring.\n\n\n" << std::endl; }
      
      //try searching for a process...need to initialize some variables
      std::vector<TreeNode> nodesfound;
      std::vector<int> trackIDs;
      std::vector<int> branchpositions;
      findProcess(event_part, _MCgetter.getTreeTops().at(i), 0, trackIDs, nodesfound, proclist, branchpositions, 0);
    }//for all TreeTops
    */
    return true;
  }
  
  bool MakeMCTree::finalize() {

    std::cout << "Avg. Num of particles/event = " << _nParticles/((float)(_nEvts)) << std::endl;
  
    return true;
  }

  void MakeMCTree::AddNodes(const event_mcpart *event_part, mcpart part, TreeNode& node){
    
    // get daughters of part
    if (_verbose) { std::cout << "current part. PDG: " << part.PdgCode() << ", process: " << part.Process() << std::endl; }
    if (_verbose) { std::cout << "current node children: " << node.getChildren().size() << std::endl; }
    std::set<Int_t> daughters = part.Daughters();
    //loop over daugher iterator -> add daughters to node
    for (std::set<int>::iterator it=daughters.begin(); it!=daughters.end(); ++it){
      mcpart tmpPart = event_part->at(_MCgetter.searchParticleMap(it));
      TreeNode tmpNode(tmpPart.TrackId());
      //t is now the node for this specific daugher...call AddNodes for this particle
      node.AddChild(tmpNode);
      AddNodes(event_part, tmpPart, tmpNode);

    }

    return;
  }

  
  int MakeMCTree::findProcess(const event_mcpart *event_part, TreeNode currentNode,
			      int currentProc, std::vector<int> trackIDpath, std::vector<TreeNode> NodeList,
			      const std::vector<std::pair<int,std::string> > procList,
			      std::vector<int> branchPositionList, int branchPosition){

    // Looking for a process before primary...meand we did not find anything
    if ( currentProc < 0 ){
      if (_verbose) { std::cout << "**** No match in this tree... Sorry! ****" << std::endl; }
      return 0;
    }
    // Reached end of process list! We found it and can return!    
    if ( currentProc == procList.size() ){
      if (_verbose) { std::cout << "**** Found all we needed! ****" << std::endl; }
      return 1;
    }

    if (_verbose){
      std::cout << "findProcess function called!" << std::endl;
      std::cout << "Currently searching for process item " << currentProc
		<< ": PDG " << procList.at(currentProc).first << ", process "
		<< procList.at(currentProc).second << std::endl;
    }

    //if this won't be the last match to do, and the current particle has no children, even if a match need to skip
    //\/\/\  if ( (currentNode.getChildren().size() == 0) and (currentProc < procList.size()-1) )

    //see if current particle matches info we are looking for...if so add
    mcpart partTmp = event_part->at( _MCgetter.searchParticleMap( currentNode.getNodeIndex() ) );
    if (_verbose) { std::cout << "current node information. PDG: " << partTmp.PdgCode() << ", process: " << partTmp.Process() << std::endl; }

    //if this is a match
    if ( (procList.at(currentProc).first == partTmp.PdgCode()) and (procList.at(currentProc).second == partTmp.Process()) ){
      if (_verbose) { std::cout << "match! move down the line!" << std::endl; }
      //if this was the last match necessary exit with 1
      if (currentProc+1 == procList.size()){
	if (_verbose) { std::cout << "**** We are done searching...found all we needed! ****" << std::endl; }
	return 1;
      }
      if ( currentNode.getChildren().size() == 0 ){
	//then we won't find the next step since no children
	if (_verbose) { std::cout << "Not adding since next step cannot be matched (no children)" << std::endl; }
      }
      trackIDpath.push_back(currentNode.getNodeIndex());
      NodeList.push_back(currentNode);
      branchPositionList.push_back(branchPosition);
      //found the match...now move down the line
      for (size_t i = branchPosition; i < currentNode.getChildren().size(); i++)
	if(findProcess(event_part, currentNode.getChildren().at(i), currentProc+1, trackIDpath, NodeList, procList, branchPositionList, 0))
	  return 1;
    }//if particle is good

    //else, if this is not a match
    else{
      //if the size of vectors to be filled is zero...then this was the primary and we are done
      if (trackIDpath.size() == 0)
	return 0;
      //then this current node is not good
      //move to the next to the right if available and check that
      //if reached the end of the "siblings" at this level, we need to go up one step
      if (_verbose) { std::cout << "Did not find what I was looking for under this node...move back one step..." << std::endl; }
      if ( branchPosition == NodeList.back().getChildren().size() ){
	//we finished all the possible children...move back up
	if (_verbose) { std::cout << "ran out of siblings, move back one step..." << std::endl; }
	trackIDpath.pop_back();
	NodeList.pop_back();
	branchPosition = branchPositionList.back() + 1;
	branchPositionList.pop_back();
	if( findProcess(event_part, NodeList.back().getChildren().at(branchPosition),
			currentProc-1, trackIDpath, NodeList, procList, branchPositionList, branchPosition) )
	  return 1;
      }
      else{
	//loop at next sibling
	if (_verbose) { std::cout << "moving over to the next sibling...branch position: " << branchPosition+1 << std::endl; }
	if (findProcess(event_part, NodeList.back().getChildren().at(branchPosition+1),
			currentProc, trackIDpath, NodeList, procList, branchPositionList, branchPosition+1) )
	  return 1;
      }
    }//if the current particle was not ok

    //finally, if we reach the end, move back one node
    trackIDpath.pop_back();
    NodeList.pop_back();
    branchPosition = branchPositionList.back() + 1;
    branchPositionList.pop_back();
    if ( trackIDpath.size() == 0) { return 0; }
    else
      if ( findProcess(event_part, NodeList.back().getChildren().at(branchPosition),
		       currentProc-1, trackIDpath, NodeList, procList, branchPositionList, branchPosition) )
	   return 1;

    return 0;
  }
  
}
#endif
