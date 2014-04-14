#ifndef FANNMODULE_CC
#define FANNMODULE_CC

#include "FANNModule.hh"
#include "LArUtilManager.hh"

namespace cluster{

  void FANNModule::LoadFromFile(std::string s){
    if (s != "") net.create_from_file(s);
    else
      net.create_from_file(fInputFileName);
  }

  void FANNModule::init(){

    larutil::LArUtilManager::Reconfigure(larlight::GEO::kArgoNeuT);
    return;
  }


  void FANNModule::run(float * data){
    float * result = net.run(data);
    for (int i = 0; i < net.get_num_output(); i ++){
      std::cout << "result " << i << " is : " << result[i] << std::endl; 
    }
  }

  void FANNModule::run(std::vector<float> & data){
    float * result = net.run(&data[0]);
    for (int i = 0; i < net.get_num_output(); i ++){
      std::cout << "result " << i << " is : " << result[i] << std::endl; 
    }
  }

  void FANNModule::run(std::vector<float> & data, std::vector<float> & returnResult){
    float * result = net.run(&data[0]);
    if (returnResult.size() != net.get_num_output()){
      returnResult.resize(net.get_num_output());
    } 
    for (int i = 0; i < net.get_num_output(); i ++){
      returnResult[i] = result[i]; 
    }
    return;
  }

}

#endif
