#ifndef MCGETTER_CC
#define MCGETTER_CC

#include "MCgetter.hh"

namespace larlight {

  void MCgetter::fillParticleMap(){

    // if _event_part empty return
    if (_event_part->size() == 0){
      std::cout << "Event Part not filled...exit" << std::endl;
      return;
    }

    //clear map before fillint
    _particleMap.clear();
    _PDGlist.clear();
    //Fill the Map for particles and trackIDs
    for (size_t i=0; i< _event_part->size(); i++){
      if (_searchPDG){
	for (size_t n=0; n < _getPDGs.size(); n++){
	  if (_event_part->at(i).PdgCode() == _getPDGs.at(n))
	    _PDGlist.push_back(_event_part->at(i).TrackId());
	}
      }
      _particleMap[_event_part->at(i).TrackId()] = i;
    }
	
    return;

  }

  void MCgetter::setTreeTops(){

    // if _event_part empty return
    if (_event_part->size() == 0){
      std::cout << "Event Part not filled...exit" << std::endl;
      return;
    }
    
    _treeTops.clear();

    for (size_t i=0; i < _event_part->size(); i++){
      mcpart part = _event_part->at(i);
      if (part.Process() == "primary"){
	//if verbose draw daugher info to check
	if (_verbose)
	  drawDaughters(part);
	TreeTop TopTmp(part.TrackId());
	AddNodes(part,TopTmp);
	_treeTops.push_back(TopTmp);
      }
    }

    return;
  }

  void MCgetter::AddNodes(mcpart part, TreeNode& node){

    std::set<Int_t> daughters = part.Daughters();
    //loop over daugher iterator -> add daughters to node
    for (std::set<int>::iterator it=daughters.begin(); it!=daughters.end(); ++it){
      mcpart tmpPart = _event_part->at(this->searchParticleMap(it));
      TreeNode tmpNode(tmpPart.TrackId());
      //t is now the node for this specific daugher...call AddNodes for this particle
      node.AddChild(tmpNode);
      AddNodes(tmpPart, tmpNode);
    }

    return;
  }

  void MCgetter::drawDaughters(mcpart particle ){

    if (_event_part->size() == 0){
      std::cout << "event_part not filled...exit" << std::endl;
      return;
    }

    if (_particleMap.size() == 0){
      std::cout << "Particle Map not filled...nowhere to search for daughters..." << std::endl;
      return;
    }

    std::cout << particle.Process()
	      << "\tTrackID:\t" << particle.TrackId()
	      << "\tPdgCode:\t" << particle.PdgCode()
	      << "\tEnergy:\t"  << particle.Trajectory().at(0).E()
	      << "\tTime:\t"    << particle.Trajectory().at(0).T()
	      << "\tNDaughters:\t"  << particle.Daughters().size()
	      << std::endl;
    std::cout << "Daughters:" << std::endl;

    std::set<Int_t> daughters = particle.Daughters();
  
    //loop over all daughters
    for (std::set<int>::iterator it=daughters.begin(); it!=daughters.end(); ++it){

      mcpart tmpPart = _event_part->at( _particleMap.find(*it)->second );

      //only if energy > 0.1 MeV
      if ( tmpPart.Trajectory().at(0).E() > _Ecut )
	std::cout << tmpPart.Process()
		  << "\tTrackID:\t" << tmpPart.TrackId()
		  << "\tPdgCode:\t" << tmpPart.PdgCode()
		  << "\tEnergy:\t"  << tmpPart.Trajectory().at(0).E()
		  << "\tTime:\t"    << tmpPart.Trajectory().at(0).T()
		  << "\tNDaughters:\t"  << tmpPart.Daughters().size()
		  << std::endl;
    }
    std::cout << std::endl << std::endl;

    int track;
    std::cout << "Enter Track Number to Show Daughters for: [0 = quit]" << std::endl;
    std::cin  >> track;

    //if quitting
    if ( track == 0 )
      return;
    
    //otherwise find particle with this trackID
    mcpart shownext = _event_part->at( _particleMap.find(track)->second );
    drawDaughters(shownext);
    return;
  }

  
  int MCgetter::searchParticleMap(const int trackID){

    try{
      return _particleMap.at(trackID);
    }
    catch (const std::out_of_range& oor){
      //      std::cerr << "Out of Range Error: Mother TrackID not found. Likely a Primary particle." << std::endl;
    }
    return -1;
  }

  int MCgetter::getMother(const mcpart particle){

    try{
      return _particleMap.at(particle.Mother());
    }
    catch (const std::out_of_range& oor){
      //      std::cerr << "Out of Range Error: Mother TrackID not found. Likely a Primary particle." << std::endl;
    }
    return -1;
  }


  /// Return ancestor trackID
  int MCgetter::getAncestor(const mcpart particle){

    //if already top ancestor
    if (particle.Mother() == 0)
      return particle.TrackId();
    else
      return getAncestor( _event_part->at(getMother(particle)));
  }


  /// Returns vector of particle TrackIds that match the process requested 
  /// TrackId returned is that of the "final" particle in the process chain
  /// If the process is not found in the event -> return empty vector

  std::vector<int> MCgetter::findProcess(const std::vector<std::pair<int, std::string> > procList){

    std::vector<int> outputs;
    outputs.clear();

    std::vector<TreeNode> nodes;
    std::vector<int> branches;
    std::vector<int> thisTreeOutputs;

    //loop over all TreeTops in the event and search for the process in each
    for (size_t i=0; i < _treeTops.size(); i++){
      nodes.clear();
      branches.clear();
      thisTreeOutputs.clear();
      findProcess(_treeTops.at(i), 0, 0, nodes, branches, procList, thisTreeOutputs);
      for (size_t x=0; x < thisTreeOutputs.size(); x++)
	outputs.push_back(thisTreeOutputs.at(x));
    }

    return outputs;
  }
  
  void MCgetter::findProcess(TreeNode currentNode, // Which node is currently under examination
			     int currentProc,      // Which step in the process is being searched for
			     int currentBranch,    // branch position being looked at
			     std::vector<TreeNode> nodePath, // List of nodes succesfully matched
			     std::vector<int> branchPath,    // List of branches followed
			     const std::vector<std::pair<int,std::string> > procList,
			     std::vector<int>& outputs){

    if (_verbose) { std::cout << "findProcess called with parameters: " << std::endl
			      << "\tcurrentNode ID: " << currentNode.getNodeIndex() << std::endl
			      << "\tcurrent Proc: " << currentProc << std::endl
			      << "\tcurrent Branch: " << currentBranch << std::endl
			      << "searching for process item " << currentProc
			      << ": PDG " << procList.at(currentProc).first << ", process "
			      << procList.at(currentProc).second << std::endl;
    }
    
    // Looking for a process before primary...means we did not find anything
    if ( currentProc < 0 ){
      if (_verbose) { std::cout << "**** No match in this tree... Sorry! ****" << std::endl; }
      return;
    }
    // Reached end of process list! We found it and can return!    
    if ( currentProc == procList.size() ){
      if (_verbose) { std::cout << "**** Found all we needed! ****" << std::endl; }
      outputs.push_back(nodePath.back().getNodeIndex());
    }

    //get particle at this node
    mcpart partTmp = _event_part->at( this->searchParticleMap( currentNode.getNodeIndex() ) );
    if (_verbose) { std::cout << "current node information. PDG: " << partTmp.PdgCode() << ", process: " << partTmp.Process() << std::endl; }

    //if MATCH
    if ( (procList.at(currentProc).first == partTmp.PdgCode()) and (procList.at(currentProc).second == partTmp.Process()) ){
      if (_verbose) { std::cout << "match! continue..." << std::endl; }
      //if this was the last match necessary exit with 1
      if (currentProc+1 == procList.size()){
	if (_verbose) { std::cout << "**** We are done searching...found all we needed! ****" << std::endl; }
	outputs.push_back(partTmp.TrackId());
      }
      else if ( currentNode.getChildren().size() == 0 ){
	//then we won't find the next step since no children
	if (_verbose) { std::cout << "Not adding since next step cannot be matched (no children)" << std::endl; }
      }
      else {
	if (_verbose) { std::cout << "Move down to children!" << std::endl; }
	nodePath.push_back(currentNode);
	branchPath.push_back(currentBranch);
	return findProcess(currentNode.getChildren().at(0), currentProc+1, currentBranch, nodePath, branchPath, procList, outputs);
      }
    }//if MATCH
    
    //else: NOT MATCH -> move to sibling
    if (_verbose) { std::cout << "move to sibling..." << std::endl;}

    //if we are at the top then can't go anywhere else...we are done
    if (nodePath.size() == 0){
      if (_verbose) { std::cout << "done with this tree...need to move up or right but can't" << std::endl; }
      return;
    }
    // check if sibling exists...otherwise go to cousints
    while ( nodePath.back().getChildren().size() <= (currentBranch+1) ){
      if (_verbose) { std::cout << "sibling does not exist...move to cousin..." << std::endl; }
      nodePath.pop_back();
      currentBranch = branchPath.back();
      branchPath.pop_back();
      currentProc -= 1;
      if (nodePath.size() == 0){
	if (_verbose) { std::cout << "done with this tree...need to move up or right but can't" << std::endl; }
	return;
      }
    }
    if (_verbose) { std::cout << "about to check sibling..." << std::endl; }
    return findProcess(nodePath.back().getChildren().at(currentBranch+1), currentProc, currentBranch+1, nodePath, branchPath, procList, outputs);
  }


  std::vector<std::vector<float> > MCgetter::getTrajectoryPoints(mcpart part){
    
    std::vector<std::vector<float> > points;
    points.clear();
    
    mctrajectory traj = part.Trajectory();
    std::vector<float> tmp;
    for (size_t i=0; i < traj.size(); i++){
      tmp.clear();
      tmp.push_back(traj.at(i).X());
      tmp.push_back(traj.at(i).Y());
      tmp.push_back(traj.at(i).Z());
      points.push_back(tmp);
    }
    
    return points;
  }

  std::vector<std::vector<float> > MCgetter::getStartEndTrajectory(mcpart part){
    
    std::vector<std::vector<float> > points;
    points.clear();
    
    mctrajectory traj = part.Trajectory();
    std::vector<float> tmp;
    tmp.clear();
    tmp.push_back(traj.at(0).X());
    tmp.push_back(traj.at(0).Y());
    tmp.push_back(traj.at(0).Z());
    points.push_back(tmp);
    tmp.clear();
    tmp.push_back(traj.back().X());
    tmp.push_back(traj.back().Y());
    tmp.push_back(traj.back().Z());
    points.push_back(tmp);

    return points;
  }


  std::string MCgetter::getFullProcess(mcpart part){

    std::vector<std::string> proclist;
    proclist.push_back(std::to_string(part.PdgCode()));
    proclist.push_back(":");
    proclist.push_back(part.Process());

    mcpart tmpPart = part;
    while (getMother(tmpPart) >= 0){
      mcpart newPart = _event_part->at(searchParticleMap(tmpPart.Mother()));
      proclist.push_back("_");
      proclist.push_back(std::to_string(newPart.PdgCode()));
      proclist.push_back(":");
      proclist.push_back(newPart.Process());
      tmpPart = newPart;
    }
    
    //now invert the proclist so that more human-readable
    std::string returnproc = "";
    for (size_t i=proclist.size(); i > 0; i--)
      returnproc += proclist.at(i-1);
    
    return returnproc;
  }

}
#endif
