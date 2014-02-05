###################
#                 #
# LArLight README #
#                 #
###################

This repository includes two sub-repositories:

(*) AnaProcess

    - Defines data structure equivalent or close to that of LArSoft.
      The purpose is to store full details of LArSoft data while eliminating
      external package dependency (such as art, LArSoft), so that one can
      perform simple ROOT analysis on one's laptop. A shared data strcuture
      allows easier application code sharing among analyzers.

    - Provides I/O framework for simple yet efficient event loop data access

    - Provides flexible data structure which allows storage of int, string,
      bool, double, and std::vector of those at run time, which means one
      does not need to create a new data container class nor TTree branch to
      store values in their code. This allows a simple and quick code development.

    - Provides C++ code development environment that can be set independent
      of the rest of the package. Provides a library generation scheme such
      that you can refer/use developed your function/class from anywhere,
      even from inside your LArSoft code or simple CINT/PyROOT interactive
      session.

    - For code development guideline, refer to AnaProcess/README.

    - Ask Kazu for further details.

(*) NevisDecoder

    - Include toolkit for decoding raw binary files output by Nevis readout 
      code (CAUTION: not by UB event builder)

    - Binary file I/O is handled by classes defined under FileIO. The class,
      bin_io_handler, can handle a block-read by specifying number of words
      to access at once. If this is enabled but # of words (a block size) is
      not specified, then a standard procedure is taken in which the 1st word
      is assumed to suggest # of words to be read as a block.      

    - Decoding algorithms are defined under Algorithm package. Currently 
      xmit readout for PMT/TPC (an option of huffman) + Trigger decoding
      algorithms are implemented. More possible upon request.

    - Decoding algorithm + bin file I/O + output ROOT storage I/O are
      handled by decode_manager class defined under Decoder directory. The
      idea is a user attaching different algorithm to this manager class
      to decode different binary files while file I/O interface can stay same.


################
#              # 
# How To Build #
#              #
################

If this is your first time ever, the default compilation is shown below
assuming you are in the same directory where this README exists:

> export MAKE_TOP_DIR=$PWD
> source config/setup.sh
> make 

This build AnaProcess package with basic components including Basic,
DataFormat, and Analysis sub-directories. 

It also buildds NevisDecoder directory with the minimal set of packages
including FileIO, Algorithm, Decoder. 

If one does not want to compile anything under NevisDecoder, do:

export DECODER_MODULE=IGNORE

prior to calling "source config/setup.sh". This will make the build system
to ignore the NevisDecoder sub directories.

###########################
#                         # 
# How To Build (Advanced) #
#                         #
###########################

One can also explicitly specify AnaProcess sub packages to compile.

> export ANA_PROC_MODULE="Base DataFormat Analysis"

The above line configures to compile three sub-directories
under AnaProcess upon sourcing "config/setup.sh".

As a summary example, one can have the following lines in a configuration
script to be run upon ssh login.

(I assume you downloaded LArLight under $HOME/LArLIght)

> export MAKE_TOP_DIR=$HOME/LArLight
> export ANA_PROC_MODULE="Base DataFormat Analysis"
> source $MAKE_TOP_DIR/config/setup.sh
> make --directory=$MAKE_TOP_DIR

##########################
#                        #
# More on Config Scripts #
#                        #
##########################

There are number of config scripts that can be found under config directory.
Other than "setup.sh" which usage is explained above, you can refer to 

  config/README.txt

to use other config scripts that are useful to:

  (*) set up mrb+git LArSoft environment
  (*) checkout / set up local LArSoft development environment
  (*) checkout / set up LArLight compatible LArSoft modules


Contact kazu for further details.



