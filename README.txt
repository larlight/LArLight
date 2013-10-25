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

(*) LArModule

    - Include LArSoft analyzer/producer modules.

    - In particular, DataScanner package shows a comprehensive example
      of LArSoft data container access method. It is a core code to convert
      LArSoft data into LArLight data format (see AnaProcess for details).

    - For code development guideline, refer to LArModule/README.

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

If $SRT_LOCAL is specified, then above command also build LArModule
sub-directories under $SRT_LOCAL. For this to happen, however,
one has to specify LArModule sub-directories to be built. This is
to avoid building all sub-directories which can take long time and
likely unnecessary. Specification of sub-directories is done through
defining LAR_MODULE env. var:

> export LAR_MODULE="DataScanner Example"

The above line configures to compile DataScanner and Example sub-directories
under LArModule upon sourcing "config/setup.sh". Note that the compilation
process proceeds in the specified ordering of sub-directories.

One can also explicitly specify AnaProcess sub packages to compile.

> export ANA_PROC_MODULE="Base DataFormat Analysis"

The above line configures to compile three sub-directories
under AnaProcess upon sourcing "config/setup.sh".

As a summary example, one can have the following lines in a configuration
script to be run upon ssh login.

(I assume you downloaded LArLight under $HOME/LArLIght)

> export MAKE_TOP_DIR=$HOME/LArLight
> export ANA_PROC_MODULE="Base DataFormat Analysis"
> export LAR_MODULE="DataScanner Example"
> source $MAKE_TOP_DIR/config/setup.sh
> make --directory=$MAKE_TOP_DIR

Contact kazu for further details.


