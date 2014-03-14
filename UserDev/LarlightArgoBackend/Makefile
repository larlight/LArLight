
.PHONY: all
all: depend bin

pwd := $(shell pwd)

ifeq ($(origin UBOONEDAQ_DATATYPES_DIR), undefined)
  uboonedaq           = $(HOME)/ubdaq/uboonedaq/projects
  uboonedaq_datatypes_src = $(HOME)/ubdaq/uboonedaq-datatypes/projects
  uboonedaq_datatypes_inc = $(HOME)/ubdaq/uboonedaq-datatypes/projects
else
  uboonedaq           = $(UBOONEDAQ_DIR)/source
  uboonedaq_datatypes_src = $(UBOONEDAQ_DATATYPES_DIR)/source
  uboonedaq_datatypes_inc = $(UBOONEDAQ_DATATYPES_DIR)/include
endif

#override, since setup is poor.
uboonedaq           = $(HOME)/development/uboonedaq/projects

CPPFLAGS := -g -I..
CPP := g++44 -std=c++0x
#CPP := g++ 
LD := $(CPP)
shlibsuffix=so
LDFLAGS = -g 

UNAME := $(shell uname -s)

ifeq ($(UNAME),Darwin)
  CPP = c++ -std=c++11 -stdlib=libc++
  LD = c++ -std=c++11 -stdlib=libc++
  shlibsuffix=dylib
  LDFLAGS = -bind_at_load -g
endif


SOURCES = $(notdir $(wildcard *.cpp)) \
          $(notdir $(wildcard $(uboonedaq_datatypes_src)/datatypes/*.cpp)) \
          Plexus.cpp \
          Client.cpp \
          DispatcherMessage.cpp \
          KvpSet.cpp \
          Logging.cpp \
          ConvertDispatcherToEventRecord.cpp \
          Timer.cpp \

OBJ =  $(addprefix tmp/, $(patsubst %.cpp,%.o,$(SOURCES)) ) 
DEPS = $(patsubst %.o,%.d,$(wildcard tmp/*.o))

backend_obj = \
tmp/MakePng.o       tmp/ResultComposer.o    tmp/TreeReader.o \
tmp/ColorMap.o      tmp/RootToJson.o        tmp/cencode.o \
tmp/FormString.o    tmp/SocketServer.o \
tmp/FormulaLooter.o tmp/Timer.o \
tmp/JsonElement.o   tmp/RecordComposer.o    tmp/TreeElementLooter.o

#Where to look for source files
vpath %.cpp $(uboonedaq_datatypes_src)/datatypes
vpath %.cpp $(uboonedaq)/dispatcher
vpath %.cpp $(uboonedaq)/online_monitor
#Where to look for include files

CPPFLAGS += -I $(uboonedaq)/projects
CPPFLAGS += -I $(uboonedaq_datatypes)/projects
CPPFLAGS += -D nullptr=0


# Don't use the default FCL and messagelogger.
CPPFLAGS += -D NO_MSGLOG=1 -D NO_FCL=1

# Boost.
BOOST_INC ?= boost/include # Usually defined for us, use local symlink ./boost if not
BOOST_LIB ?= boost/lib
BLIBS += -lboost_serialization -lboost_system -lboost_thread
ifeq ($(UNAME),Darwin)
  LIBS += -Wl,-rpath,@executable_path/boost/lib 
  CPPFLAGS +=  -ftemplate-depth=512
endif

CPPFLAGS += -I/$(BOOST_INC) 
BLIBS += -L${BOOST_LIB}


# Root.
export ROOTSYS ?= ${pwd}/root  # Usually defined for us, use local symlink ./root if not
export LD_LIBRARY_PATH=${ROOTSYS}/lib
ifeq ($(UNAME),Darwin)
	LDFLAGS += -Wl,-rpath,root -Wl,-rpath,$(ROOTSYS)
endif
PATH+=:${ROOTSYS}/bin
LIBS  += -L/usr/X11R6/lib -lTreePlayer $(shell ROOTSYS=${ROOTSYS} ${ROOTSYS}/bin/root-config --libs )  
LDFLAGS += $(shell ROOTSYS=${ROOTSYS} ${ROOTSYS}/bin/root-config --ldflags)
CPPFLAGS += $(shell ROOTSYS=${ROOTSYS} ${ROOTSYS}/bin/root-config --cflags) -I/opt/X11/include


# add any other desired libs here
# PNG
LIBS += -L/opt/X11/lib -lpng 

# postresql
ifeq ($(UNAME),Darwin)
  # Use the Postgres app, instead of a fullblown installation.
  BLIBS +=  -L/Applications/Postgres.app/Contents/MacOS/lib
  CPPFLAGS += -I/Applications/Postgres.app/Contents/MacOS/include
endif
ifneq ($(origin POSTGRES_INC), undefined)
	CPPFLAGS += -I ${POSTGRES_INC}
endif
POSTGRES_LIB ?= "postgres"  # Usually defined for us, use local symlink ./postgres if not
BLIBS += -L${POSTGRES_LIB} -lpq
	

#Dependency rule
tmp/%.d : %.cpp
	@mkdir -p tmp
	@echo "Building cpp dependency $@ $<"
	@$(CPP) -M -MT $(patsubst %.cpp,tmp/%.o,$<) $(CPPFLAGS) -o $@ -c $<

tmp/%.d : %.cc
	@mkdir -p tmp
	@echo "Building cc dependency $@ $< %.o"
	@$(CPP) -M -MT $(patsubst %.cc,tmp/%.o,$<) $(CPPFLAGS) -o $@ -c $<

ifneq ($(DEPS),)
-include $(DEPS)
endif


#Compilation rule
tmp/%.o : %.cpp
	@mkdir -p tmp
	@echo "compiling $@ $<"
	$(CPP) $(CPPFLAGS) -o $@ -c $<

tmp/%.o : %.cc
	@mkdir -p tmp
	@echo "compiling $@ $<"
	$(CPP) $(CPPFLAGS) -o $@ -c $<


#Rootcint rule:
RHEADERS = JsonElement.h FormulaLooter.h
Cint.cpp : $(RHEADERS) LinkDef.h
	@echo "Running rootcint... " $(RHEADERS) LinkDef.h
	LD_LIBRARY_FLAGS=${ROOTSYS}/lib
	rootcint -v3 -f $@ -c -g -I./ -pthread -m64 $(RHEADERS) LinkDef.h
	
tmp/Cint.o : Cint.cpp
	$(CPP) $(shell ROOTSYS=${ROOTSYS} ${ROOTSYS}/bin/root-config --cflags) -c $< -o $@ 



.PHONY: bin
bin: argo-backend argo-raw-backend myroot testx
argo-backend: $(backend_obj) tmp/argo-backend.o Makefile
	$(CPP) $(LDFLAGS) $(filter %.o,$^) $(LIBS) -o $@

argo-raw-backend: $(OBJ) tmp/argo-raw-backend.o Makefile
	$(CPP) $(LDFLAGS) $(filter %.o,$^) $(LIBS) $(BLIBS) -o $@

myroot: $(OBJ) tmp/myroot-main.o tmp/Cint.o Makefile
	@echo "Building " $@ $(OBJ)
	$(CPP) $(LDFLAGS) $(filter %.o,$^) $(LIBS) -o $@

testx: $(OBJ) tmp/test-main.o tmp/Cint.o Makefile
	@echo "Building " $@ $(OBJ)
	$(CPP) $(LDFLAGS) $(filter %.o,$^) $(LIBS) -o $@

depend: $(DEPS)

printenv: 
	@echo " CPPFLAGS = $(CPPFLAGS) "
	@echo " LIBS = $(LIBS)"
	@echo " Sources: $(SOURCES)"
	@echo " Objects: $(OBJ)"
	@echo " RHEADERS = $(RHEADERS)"

clean:
	rm -rf tmp/*
