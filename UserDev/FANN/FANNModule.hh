/**
 * \file FANNModule.hh
 *
 * \ingroup FANNModule
 * 
 * \brief Class def header for a class FANNModule
 *
 * @author guest27
 */

/** \addtogroup FANNModule

    @{*/
#ifndef FANNMODULE_HH
#define FANNMODULE_HH


#include <iostream>
#include <stdarg.h>

#include <vector>
#include <string>
#include "floatfann.h"
#include "fann_cpp.h"

/**
   \class FANNModule
   User defined class FANNModule ... these comments are used to generate
   doxygen documentation!
 */
namespace cluster{
  class FANNModule{

  public:
    /// Default constructor
    FANNModule(){
      fInputFileName = "default.net";
    }

    /// Default destructor
    //~FANNModule(){ if (ann) fann_destroy(ann);}
    ~FANNModule(){}


    /**
     * Sets the output file name
     * @param s output file name
     */
    void setFANNFileName(std::string s){fInputFileName = s;}

    void LoadFromFile(std::string s = "");

    void init();



    void run(float * data);
    void run(std::vector<float> & data);
    void run(std::vector<float> & data, std::vector<float> & result);

    void print_error(){std::cout << "Current Error: "<< net.get_MSE() << std::endl;}
    void print_connections(){net.print_connections();}
    int get_num_input() {return net.get_num_input();}
    int get_num_output(){return net.get_num_output();}

  private:
    std::string fInputFileName;

    // this is the actual neural network.  Right now only one per class:
    FANN::neural_net net;

  };
}

#endif
/** @} */ // end of doxygen group 

