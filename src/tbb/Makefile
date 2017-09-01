# Allow CXX and CXXFLAGS to be overridden from the environment
CXX ?= g++
CXXFLAGS ?= -std=c++14 -g -O2
LDLIBS ?= -lpthread -ltbb -ltutorialutils -lm
LDFLAGS ?= -L.

ifdef DEBUG
CXXFLAGS += -DDEBUG=1
endif

# This implicit rule copes with the situation where a .o file
# has been left behind (e.g., by two step compile and link)
% : %.o
	$(CXX) $(LDFLAGS) $< $(LOADLIBES) $(LDLIBS) -o $@

% : %.cc

# tutorialutils builds our utility library, not a binary
EXE=$(basename $(filter-out tutorialutils.cc, $(wildcard *.cc)))

all: $(EXE)

# Not all executables actually depend on this utilities library
# but it's harmless to have make think they do
$(EXE): libtutorialutils.a

tutorialutils.o: tutorialutils.cc

libtutorialutils.a: tutorialutils.o
	ar rcs $@ $<


clean:
	rm -fr $(EXE) *.dSYM *.o libtutorialutils.a
