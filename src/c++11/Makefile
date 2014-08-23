CXX=g++
CXXFLAGS=-pthread -std=c++11 -g -O2

EXE=$(basename $(wildcard *.cc))

all: $(EXE)

start_threads: start_threads.cc

clean:
	rm -fr $(EXE) *.dSYM

