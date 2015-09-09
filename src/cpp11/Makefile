# Allow CXX and CXXFLAGS to be overridden from the environment
CXX ?= g++
CXXFLAGS ?= -pthread -std=c++11 -g -O2

EXE=$(basename $(wildcard *.cc))

all: $(EXE)

clean:
	rm -fr $(EXE) *.dSYM

