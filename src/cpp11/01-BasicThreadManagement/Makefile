# Allow CXX and CXXFLAGS to be overridden from the environment
CXX ?= g++
CXXFLAGS ?= -pthread -std=c++14 -g -O2
LDLIBS ?= -lpthread

# This implicit rule copes with the situation where a .o file
# has been left behind (e.g., by two step compile and link)
% : %.o
	$(CXX) $(LDFLAGS) $< $(LOADLIBES) $(LDLIBS) -o $@

EXE=$(basename $(wildcard *.cc))

all: $(EXE)

clean:
	rm -fr $(EXE) *.dSYM *.o

