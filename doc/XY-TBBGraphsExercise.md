# TBB Tutorial - Scientific Data Processing Exercise

In this final part of the TBB tutorial we shall build up a simple, but failrly
realistic, example of scientific data processing. The exercise is the
following:

- We have a detector that consists of a grid of pixels, 100x100.
 
  - The detector is designed to be sensitive to a new type of long lived
  particle, *the fooble*.
  
- The detector is read out each second and we have a number of timeframes
available to us in which there might be a fooble.

- Foobles are slow moving and the expected signal from a fooble is a large
energy spike in a cluster of pixels, that should last for 5 seconds.

![The Fooble Detector](https://graemes.web.cern.ch/graemes/teaching/gridka/fooble-sketch.png)

## Fooble Detector Data Processing

Processing the data from the fooble detector for a time frame does require
some extra steps:

1. Read raw data for a frame.
2. Subtract the detector's pedastal values from each cell (the pedastal is
a constant value per cell, but varies across the detector).
3. Mask hot pixels, where we know that we get a bad signal (in our example
these pixels are fixed for the whole dataset).
4. Search for a possible fooble signal in a 3x3 subset of cells by calculating
the average signal strength:
    1. If the average signal in this cluster exceeds 150 this could be a fooble.

The potential fooble clusters then need to be searched in different time
frames to see if they appear in 5 consecutive frames.

## Thinking About Parallelism

The first step in designing our data processing pipeline is to think about
what opportunies we have for parallelism when processing:

- Think about opportunities for parallelism for each single step above
- Think about opportunities for parallelism between each of the steps
- Think about opportunities for parallelism between different time frames

The goal is always to exploit parallelism at multiple levels!

## Designing A Data Flow Pipeline

Once parallel opportunities have been identified, think about how these could
be expressed in TBB, using what you learned so far in the tutorial.

## Solution

### Serial Solution

In the directory `XY-TBBGraphsExercise-Problem` you will find code that solves
the fooble detection problem in serial mode.

While in general serial code is not the correct place to start to think about
parallelism, this code will show solutions to many of the technical issues
you'll face in implementing a parallel solution, so it is useful from that 
point of view.

There is a header file `fdet.hpp` and some library code `fdet.cc` that will
define the basic classes and helper functions you will need to write your solution
(N.B. you should not change this code - take it as a given).

### Parallel Solution

The file `parallel-solution.cc` can be developed into your parallel solution.

N.B. to avoid issues with compilation here using `cmake` is strongly
recommended. 

```sh
mkdir build && cd build
cmake [PATH_TO]/cpp-concurrency/src/tbb/XY-TBBGraphExercise-Problem
make
```

The recommendation is to proceed via baby steps - implement the solution gradually
and check it's reasonable at each point.

In the tutorial we may take breaks to stop and discuss how we are going.

### Testing

Develop your solution on the following data file:

<https://graemes.web.cern.ch/graemes/teaching/gridka/dat-50.bin>

It has 50 time frames. You should find a single fooble there.

Test your solution on:

<https://graemes.web.cern.ch/graemes/teaching/gridka/dat-1000.bin>

Here there are 1000 time frames. How many foobles can you find there?

---

---

*Hints* (Look away now if you don't want to see these!)

- It will be easier to manage a parallel solution if you assume that you can
load all of the data into memory.

- Likewise, build up the signals into an in-memory data structure, similar to that
used in the serial case.

- Most of the `flow::graph` can be directed without copying whole frames of data
around, as long as you can pass a way to indicate the correct data into the nodes.

- Using a `flow::graph`, acting on single time frames you will be able to do
most, but not quite all of the work you need to do.

- *Penultimate hint:* Ask for help :-).

- *Ultimate hint*: There is a solution in the directory `XY-TBBGraphExercise-Solution`
that you can use if you really get stuck...
