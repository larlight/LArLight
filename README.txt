###################
#                 #
# LArLight README #
#                 #
###################

LArLight is a simple C++/ROOT code development framework.
Design goals include:
    
    (*) Simple configuration method

    	- single configuration script

    (*) Fast build

    	- nothing special: simple dependency & build system

    (*) Simple template generation

    	- One line command to generate an empty package w/ build system

    (*) Interpreter support

    	- Allows accessing compiled librari from python and CINT interpreter

A reason why it got a prefix "LAr" is because above features are used to
develop an extension that allows ala LArSoft data analysis.

    (*) ala LArSoft analysis framework

    	- Data products capable to store LArSoft data
	- Analysis framework ala EDAnalyzer/EDProducer
	- Utility classes include LArSoft's Geometry, LArProperties, DetectorProperties

########################################
#                                      #
# What you get under the top directory #
#                                      #
########################################

This repository includes two important sub-repositories:

(*) core ... contains framework building blocks

    - Defines data structure equivalent or close to that of LArSoft.
      The purpose is to store full details of LArSoft data while eliminating
      external package dependency (such as art, LArSoft), so that one can
      perform simple ROOT analysis on one's laptop. A shared data strcuture
      allows easier application code sharing among analyzers.

    - Provides ala LArSoft utility class such as Geometry, DetectorProperties,
      and LArProperties.

    - Provides I/O framework (like art::Event) for simple yet efficient event 
      loop data access

    - Provides Analysis framework (like art::EDAnalyzer/EDProducer) for 
      systematic event processing.

    - Ask Kazu for further details.

(*) UserDev ... packages developped by users

    - Contains packages developped by various users

    - Contains a template package generation script which comes with a build
      system. One can execute a single line command, and type "make": taht's it.

(*) config ... contains scripts to configure your shell for using LArLight

    - all you need is setup.sh 

(*) lib ... where your compiled libraries will be gathered and become visible

(*) doc ... documentation directory

    - contains doxygen script to generate html documentation of all codes

(*) NevisDecoder ... a custom binary file decoder framework for Nevis readout code
    
    - If you are not from Columbia University, just ignore it...

#################################
#                               # 
# Building the Framework (core) #
#                               #
#################################

If this is your first time ever, the default compilation is shown below
assuming you are in the same directory where this README exists:

> export MAKE_TOP_DIR=$PWD
> source config/setup.sh
> make 

This build core package with basic components including Basic,
DataFormat, and Analysis sub-directories. 

####################
#                  #
# Building UserDev # 
#                  #
####################

Under UserDev directory, you find various packages from various users.
That is where everyone is suggested to do his/her code development.
LArLight supports a user to specify which package under UserDev to be
compiled through a shell environment variable.

> export USER_MODULE="FEMPulseStudy FEMPulseReco"

The above line configures to compile three sub-directories unser UserDev
upon sourcing "config/setup.sh".

#########################
#                       #
# Building NevisDecoder #
#                       #
#########################

If one needs to compile NevisDecoder extension, set

export DECODER_MODULE="FileIO Algorithm Decoder"

prior to calling "source config/setup.sh". This will make the build system
to include NevisDecoder.

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



