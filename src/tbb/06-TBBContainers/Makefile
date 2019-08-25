# Allow CXX and CXXFLAGS to be overridden from the environment
CXX ?= g++
CXXFLAGS ?= -std=c++14 -g -O2
LDLIBS ?= -lpthread -ltbb -lm
LDFLAGS ?= -L.

ifdef DEBUG
CXXFLAGS += -DDEBUG=1
endif

# This implicit rule copes with the situation where a .o file
# has been left behind (e.g., by two step compile and link)
% : %.o
	$(CXX) $(LDFLAGS) $< $(LOADLIBES) $(LDLIBS) -o $@

# Delete the one step implicit rule because it adds
# libtutorialutils.a to the command line
% : %.cc

# tutorialutils builds our utility library, not a binary
EXE=$(basename $(wildcard *.cc))

all: $(EXE)
