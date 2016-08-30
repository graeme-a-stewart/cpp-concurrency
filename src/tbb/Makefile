# Allow CXX and CXXFLAGS to be overridden from the environment
CXX ?= g++
CXXFLAGS ?= -std=c++11 -g -O2
LDLIBS ?= -lpthread -ltbb -lm

EXE=$(basename $(wildcard *.cc))

all: $(EXE)

clean:
	rm -fr $(EXE) *.dSYM
